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
#include <thread>

#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <type_traits>
#include <cppcomponents/cppcomponents.hpp>
#include <memory>
#include <sstream>
#include <string>

#define FIB_UNTIL 25
//uv_loop_t *loop;


void print_tid(const std::string& str  = "On Thread "){
	std::stringstream s;
	s << str << " " << uv_thread_self() << "\n";

	fputs(s.str().c_str(),stderr);
}

template<class T>
struct storage_and_error{
	cppcomponents::error_code error_;

	typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type storage_;
	std::atomic<bool> storage_initialized_;
	std::atomic<bool> finished_;





	storage_and_error():error_(0),storage_initialized_(false),finished_(false){

	}

	bool finished()const{
		return finished_.load();
	}
	
	template<class F>
	void do_assign(F& f){
		try{
			void* data = &storage_;
			new(data) T(f());
			storage_initialized_.store(true);

		}
		catch (std::exception& e){
			error_ = cppcomponents::error_mapper::error_code_from_exception(e);
		}
		finished_.store(true);
	}

	void check_get()const {
		if (!finished_.load()){
			throw std::runtime_error("Operation not finished");
		}
		cppcomponents::throw_if_error(error_);
		if (!storage_initialized_.load()){
			throw std::runtime_error("Storage not initialized");
		}
	}

	T& get(){
		check_get();

		void* data = &storage_;
		auto& ret = *static_cast<T*>(data);
		return ret;
	}
	const T& get()const{
		check_get();
		const void* data = &storage_;
		return *static_cast<const T*>(data);
	}


	void set_error(cppcomponents::error_code ec){
		error_ = ec;
		finished_.store(true);
	}

	void set(T t){
		try{
			void* data = &storage_;
			new(data) T(std::move(t));
			storage_initialized_.store(true);

		}
		catch (std::exception& e){
			error_ = cppcomponents::error_mapper::error_code_from_exception(e);
		}
		finished_.store(true);
	}

	~storage_and_error(){
		if (storage_initialized_.load()){
			void* data = &storage_;
			static_cast<const T*>(data)->~T();
		}
	}
};

template<>
struct storage_and_error<void>{
	cppcomponents::error_code error_;

	std::atomic<bool> finished_;





	storage_and_error():error_(0), finished_(false){

	}

	bool finished()const{
		return finished_.load();
	}

	template<class F>
	void do_assign(F& f){
		try{
			f();

		}
		catch (std::exception& e){
			error_ = cppcomponents::error_mapper::error_code_from_exception(e);
		}
		finished_.store(true);
	}

	void check_get()const{
		if (!finished_.load()){
			throw std::runtime_error("Operation not finished");
		}
		cppcomponents::throw_if_error(error_);

	}
	void set(){
		finished_.store(true);
	}
	void set_error(cppcomponents::error_code ec){
		error_ = ec;
		finished_.store(true);
	}

	void get()const{
		check_get();
	}


	~storage_and_error(){

	}
};

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


	template<class F>
	work(F f)
		: w_{}, then_called_{ ATOMIC_FLAG_INIT },
		has_then_{ false }, worker(f), then_added_(false), started_(false)
	{
		w_.data = this;
		++work_count;
	}

	void set_shared_self(){
		shared_self_ = this->shared_from_this();

	}

	void start(){
		set_shared_self();
		started_.store(true);
		uv_queue_work(uv_default_loop(), &w_, do_work, after_work_cb);
	}
	void start_on_default_loop(){
		set_shared_self();
		started_.store(true);
		uv_queue_work(uv_default_loop(), &w_, do_no_work, work_on_default_loop);
	}


	T get()const{
		return storage_.get();
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
		auto pw = std::make_shared<work<R>>([self_, f](){
			return f(self_);

		});
		then_ = [pw, use_default_loop](){
			if (use_default_loop){
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
		print_tid("In after work");
	}

	// For then on main loop
	static void do_no_work(uv_work_t*){}

	static void work_on_default_loop(uv_work_t* req, int status){
		do_work(req);
		after_work_cb(req, status);
	}


	~work(){
		--work_count;
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
struct value_waiter_helper {

	mutable std::mutex mu_;
	mutable std::condition_variable cond_var_;

	storage_and_error<T> storage_;

	T get(){
		std::unique_lock<std::mutex> lk(mu_);
		cond_var_.wait(lk, [this](){return storage_.finished(); });
		return storage_.get();
	}

	void set(T t){
		std::unique_lock<std::mutex> lk(mu_);
		storage_.set(std::move(t));
		cond_var_.notify_all();
	}




};
template<>
struct value_waiter_helper<void> {

	mutable std::mutex mu_;
	mutable std::condition_variable cond_var_;

	storage_and_error<void> storage_;

	void get()const {
		std::unique_lock<std::mutex> lk(mu_);
		cond_var_.wait(lk, [this](){return storage_.finished(); });
		return storage_.get();
	}

	void set(){
		std::unique_lock<std::mutex> lk(mu_);
		storage_.set();
		cond_var_.notify_all();
	}




};

template<class T>
struct value_waiter{
	typedef value_waiter_helper<T> v_t;
	std::shared_ptr<v_t> p_;

	value_waiter():p_(std::make_shared<v_t>()){}
	value_waiter(std::shared_ptr<v_t> p) : p_(p){}

	T get()const{
		return p_->get();
	}

	void set(T t){
		p_->set(std::move(t));
	}

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


	template<class F>
	auto then(bool run_on_default_loop, F f)->future < decltype(f(future(pw_))) > {
		return pw_->then(run_on_default_loop, [f](std::shared_ptr<w_t> w){
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


	//loop = uv_default_loop();
	print_tid("Main thread");
	auto w = std::make_shared<work<long>>([](){return fib(12); });
	w->start();
	w->then(true,[](std::shared_ptr < work < long >> w){
		auto f = w->get();
		fprintf(stderr, "%dth fibonacci is %lu\n", 12,f);
		print_tid("In then");
		return 0;
	});
	value_waiter<long> waiter;
	future<void> fut([waiter]()mutable{waiter.set(fib(15)); });
	future<void>([waiter](){
		auto f = waiter.get();
		fprintf(stderr, "%dth fibonacci is %lu\n", 15, f);
		print_tid("In then");

	})
	.then([](future<void> fu){
		return std::string("Hello World");})
	.then([](future<std::string> fu){
		auto s = fu.get();
		fputs(fu.get().c_str(), stderr);
	fprintf(stderr,"\nHello world work count = %d\n", work_count.load());
		});

	return uv_run(uv_default_loop(), UV_RUN_DEFAULT);

}