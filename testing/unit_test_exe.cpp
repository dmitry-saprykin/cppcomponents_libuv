#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "Psapi.lib")
#include <uv.h>

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

namespace{
	struct MemLeakCheckInit{
		MemLeakCheckInit(){
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		}
	};

	MemLeakCheckInit mlcinit;
}

#endif
#include <stdio.h>
#include <stdlib.h>
#include <atomic>
#include <type_traits>
#include "../cppcomponents_libuv/cppcomponents_libuv.hpp"
#include <memory>
#include <sstream>
#include <string>
#include <iostream>





template<class D, class T>
struct work_move{
	T && move(){
		return std::move(static_cast<D*>(this)->storage_.get());
	}
};

template<class D>
struct work_move<D,void>{

};

std::atomic<int> work_count{0};

struct default_loop_ref_unref{
	uv_work_t* w_;
	default_loop_ref_unref(uv_work_t& w):w_(&w){
		uv_ref((uv_handle_t*) w_);
	}	
	~default_loop_ref_unref(){
		uv_unref((uv_handle_t*) w_);
	}
};

template<class T>
struct work:std::enable_shared_from_this<work<T>>,work_move<work<T>,T>{
	uv_work_t w_;
	std::atomic_flag then_called_;
	std::atomic<bool> has_then_;
	std::function < void()> then_;
	std::function < T()> worker;
	std::atomic<bool> started_;


	storage_and_error<T> storage_;

	std::shared_ptr<work> shared_self_;

	bool then_added_;

	uv_async_t async_start_;
	uv_async_t async_start_on_default_loop_;

	template<class F>
	work(F f)
		: w_{},then_called_{ ATOMIC_FLAG_INIT },
		has_then_{ false }, worker(f), then_added_(false), started_(false)
	{
		auto tid = uv_thread_self();

		w_.data = this;
		uv_async_init(uv_default_loop(), &async_start_, do_queue_start);
		uv_async_init(uv_default_loop(), &async_start_on_default_loop_, do_queue_start_on_default);
		async_start_.data = this;
		async_start_on_default_loop_.data = this;

		fprintf(stderr, "Work constructor %d\n", work_count.fetch_add(1) + 1);
	}

	void set_shared_self(){
		shared_self_ = this->shared_from_this();

	}

	static void do_queue_start(uv_async_t *handle, int status /*UNUSED*/){
		auto i = uv_thread_self();
		void* data = handle->data;
		auto& w = *static_cast<work*>(data);
		uv_queue_work(uv_default_loop(), &w.w_, do_work, after_work_cb);
		uv_close(reinterpret_cast<uv_handle_t*>(&w.async_start_), nullptr);
		uv_close(reinterpret_cast<uv_handle_t*>(&w.async_start_on_default_loop_), nullptr);

	}
	static void do_queue_start_on_default(uv_async_t *handle, int status /*UNUSED*/){
		auto i = uv_thread_self();
		void* data = handle->data;
		auto& w = *static_cast<work*>(data);
		uv_queue_work(uv_default_loop(), &w.w_, do_no_work, work_on_default_loop);
		uv_close(reinterpret_cast<uv_handle_t*>(&w.async_start_), nullptr);
		uv_close(reinterpret_cast<uv_handle_t*>(&w.async_start_on_default_loop_), nullptr);

	}

	void start(){
		set_shared_self();
		started_.store(true);
		uv_async_send(&async_start_);
	}
	void start_on_default_loop(){
		set_shared_self();
		started_.store(true);
		uv_async_send(&async_start_on_default_loop_);
	}


	T get()const{
		return storage_.get();
	}


	bool ready()const{
		return storage_.finished();
	}


	template<class TF>
	auto then(bool use_default_loop,TF f)->std::shared_ptr<work<decltype(f(shared_self_))>>{
		assert(shared_self_.get());
		if (then_added_){
			throw std::runtime_error("Then called more than once");
		}
		then_added_ = true;
		typedef decltype(f(shared_self_)) R;
		auto self_ = shared_self_;
		auto pw = std::make_shared<work<R>>([self_, f]()mutable{
			return f(self_);

		});
		then_ = [pw, use_default_loop](){
			if (false&&use_default_loop){
				pw->start_on_default_loop();
			}
			else{
				pw->start();
			}
		};
		has_then_.store(true);
		if (storage_.finished()){
			if (then_called_.test_and_set() == false){
				then_();
				then_ = [](){};
			}
		}

		return pw;
	}

	template<class TF>
	auto then(TF f)->decltype(then(false, f)){
		return then(false, f);
	}
	static void do_work(uv_work_t* wt){
		auto& w = *static_cast<work*>(wt->data);
		w.storage_.do_assign(w.worker);

		if (w.has_then_.load()){
			if (w.then_called_.test_and_set()==false){
				w.then_();		
				w.then_ = [](){};
			}

		}
	}

	

	static void after_work_cb(uv_work_t* req, int status){
		auto& w = *static_cast<work*>(req->data);
		w.shared_self_ = nullptr;
	}

	// For then on main loop
	static void do_no_work(uv_work_t*){
	}

	static void work_on_default_loop(uv_work_t* req, int status){
		do_work(req);
		after_work_cb(req, status);
	}


	~work(){
		fprintf(stderr,"Work destructor %d\n",work_count.fetch_sub(1));
	}
};


template<class D, class T>
struct future_move{
	T && move(){
		return static_cast<D*>(this)->pw_->move();
	}
};

template<class D>
struct future_move<D, void>{

};




template<class T>
class future:future_move<future<T>,T>{
	typedef work<T> w_t;
	std::shared_ptr<w_t> pw_;

	friend struct future_move<future<T>, T>;
public:
	future(std::shared_ptr < work < T >> p) : pw_(p){
		
		pw_->set_shared_self();
	}
	template<class F>
	future(F f) : pw_(std::make_shared<w_t>(f)){
		pw_->start();
	}
	future(value_waiter<T> w) : pw_(std::make_shared<w_t>([w](){return w.get();})){
			pw_->start();
	}


	template<class F>
	auto then(bool run_on_default_loop, F f)->future < decltype(f(future(pw_))) > {
		return pw_->then(run_on_default_loop, [f](std::shared_ptr<w_t> w)mutable{
			future fut(w);
			return f(fut);
		});
	}
	template<class F>
	auto then(F f)->future < decltype(f(future(pw_))) > {
		return this->then(false, f);
	}

	T get()const {
		return pw_->get();
	}

	bool ready()const{
		return pw_->ready();
	}


};



long fib_(long t) {
	if (t == 0 || t == 1)
		return 1;
	else
		return fib_(t - 1) + fib_(t - 2);
}

long fib(int n) {

	long f = fib_(n);
	return f;
}

void after_fib(uv_work_t *req, int status) {
	fprintf(stderr, "Done calculating %dth fibonacci\n", *(int *) req->data);
}

struct wc_printer{
	~wc_printer(){
		fprintf(stderr, "\nFinal work count = %d\n", work_count.load());

	}
};

wc_printer pr_;

int main() {


	std::cout << "main thread is thread " << uv_thread_self() << "\n";

	value_waiter<long> waiter;
	value_waiter<void> done;
	//future<long> fut(waiter);
	//	future<void>([waiter]()mutable{waiter.set(fib(15)); });
	//	waiter = value_waiter<long>();
	future<long> fut([](){return fib(15); });
	fut.then(true,[](future<long> fut){
		assert(fut.ready());
		auto f = fut.get();
		fprintf(stderr, "%dth fibonacci is %lu\n", 15, f);

	});
	//.then([](future<void> fu){
	//	return std::string("Hello World");})
	//.then([](future<std::string> fu){
	//	auto s = fu.get();
	//	fputs(fu.get().c_str(), stderr);
	//fprintf(stderr,"\nHello world work count = %d\n", work_count.load());
	//	});
		//.then(true,[done](future<void> f)mutable{
		//	f.get();
		//	std::cout << "That's all" << " folks " << "(P.S. Notice how our cout did not get messed up)\n"
		//		<< "We are in thread " << uv_thread_self() << "\n";

		//	//done.set();
		//});

		//assert(fut.ready() == false);


		std::cout << "Cout at the beginning\n";
	auto ret = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	//done.get();
	return ret;

}