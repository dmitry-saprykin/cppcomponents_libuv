#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "Psapi.lib")

#include "cppcomponents_libuv.hpp"
#include <uv.h>

using namespace cppcomponents;
using namespace cppcomponents_libuv;

namespace clv = cppcomponents_libuv;

inline std::string dummyid(){
	return "4103373e-57db-4928-95e7-17eb88178c2e";
}
//struct IRequest;
//struct IGetAddrinfoRequest;
//struct IShutdownRequest;
//struct IWriteRequest;
//struct IConnectRequest;
//struct IUdpSendRequest;
//struct IUdpRecvRequest;
//struct IFsRequest;
//struct IWorkRequest;


uv_loop_t* as_uv_type(use<ILoop> loop){
	return static_cast<uv_loop_t*>(loop.UvHandle());
}
uv_work_t* as_uv_type(use<IWorkRequest> w){
		return static_cast<uv_work_t*>(w.UvHandle());
}
uv_stream_t* as_uv_type(use<clv::IStream> s){
	return static_cast<uv_stream_t*>(s.UvHandle());
}
uv_buf_t* as_uv_type(clv::Buffer* b){
	return reinterpret_cast<uv_buf_t*>(b);
}
uv_connect_t* as_uv_type(use<IConnectRequest> c){
	return static_cast<uv_connect_t*>(c.UvHandle());
}

uv_udp_send_t* as_uv_type(use<IUdpSendRequest> r){
	return static_cast<uv_udp_send_t*>(r.UvHandle());
}
uv_write_t* as_uv_type(use<IWriteRequest> r){
	return static_cast<uv_write_t*>(r.UvHandle());
}
uv_getaddrinfo_t* as_uv_type(use<IGetAddrinfoRequest> r){
	return static_cast<uv_getaddrinfo_t*>(r.UvHandle());
}
uv_fs_t* as_uv_type(use<IFsRequest> r){
	return static_cast<uv_fs_t*>(r.UvHandle());
}


typedef runtime_class<dummyid, object_interfaces<IGetAddrinfoRequest>> GetAddrinfoRequest_t;
typedef runtime_class<dummyid, object_interfaces<IShutdownRequest>> ShutdownRequest_t;
typedef runtime_class<dummyid, object_interfaces<IWriteRequest>> WriteRequest_t;
typedef runtime_class<dummyid, object_interfaces<IConnectRequest>> ConnectRequest_t;
typedef runtime_class<dummyid, object_interfaces<IUdpSendRequest>> UdpSendRequest_t;
typedef runtime_class<dummyid, object_interfaces<IFsRequest>> FsRequest_t;
typedef runtime_class<dummyid, object_interfaces<IWorkRequest>> WorkRequest_t;

#define SWALLOW_EXCEPTIONS(s)try{s;}catch(...){}

template<class Derived, class RC>
struct ImpRequest
	: public implement_runtime_class<Derived, RC>
{
	void* data_;
	uv_req_t& AsReq(){
		return *reinterpret_cast<uv_req_t*>(
			&static_cast<Derived*>(this)->uv_t_);
	}

	int RequestType(){ return AsReq().type; }
	int Cancel(){
		return uv_cancel(&AsReq());

	}
	void* GetData(){ return data_; }
	void SetData(void* v){ data_ = v; };

	void* UvHandle(){
		return &static_cast<Derived*>(this)->uv_t_;
	}

	ImpRequest() : data_(nullptr){}

	typedef ImpRequest imp_t;
};

struct ImpGetAddrinfoRequest
	: public ImpRequest<ImpGetAddrinfoRequest, GetAddrinfoRequest_t>
{

	using ImpRequest<ImpGetAddrinfoRequest, GetAddrinfoRequest_t>::RequestType;

	uv_getaddrinfo_t uv_t_;
	use<GetAddrinfoCallback> cb_;
	use<IGetAddrinfoRequest> self_;
	use<clv::ILoop> loop_;

	use<clv::ILoop> GetLoop(){
		return loop_;
	}


	ImpGetAddrinfoRequest(){
		throw error_not_implemented();
	}
	ImpGetAddrinfoRequest(use<GetAddrinfoCallback> cb, use<clv::ILoop> loop)
		: cb_(cb), self_(this->QueryInterface<IGetAddrinfoRequest>()),
		loop_(loop)

	{
		uv_t_.data = this;
	}

	static void RequestCb(uv_getaddrinfo_t* req, int status, addrinfo* res){
		auto& imp = *static_cast<ImpGetAddrinfoRequest*>(req->data);
		auto s = imp.self_;
		imp.self_ = nullptr;
		SWALLOW_EXCEPTIONS(imp.cb_(s, status, res));
	}


};

CPPCOMPONENTS_REGISTER(ImpGetAddrinfoRequest)


struct ImpShutdownRequest
	: public ImpRequest<ImpShutdownRequest, ShutdownRequest_t>
{



	uv_shutdown_t uv_t_;
	use<ShutdownCallback> cb_;
	use<IShutdownRequest> self_;
	use<clv::IStream> handle_;

	use<clv::IStream> GetHandle(){
		return handle_;
	}


	ImpShutdownRequest(){
		throw error_not_implemented();
	}
	ImpShutdownRequest(use<ShutdownCallback> cb, use<clv::IStream> h)
		: cb_(cb),
		self_(this->QueryInterface<IShutdownRequest>()),
		handle_(h)
	{
		uv_t_.data = this;
	}

	static void RequestCb(uv_shutdown_t* req, int status){
		auto& imp = *static_cast<ImpShutdownRequest*>(req->data);
		auto s = imp.self_;
		imp.self_ = nullptr;
		SWALLOW_EXCEPTIONS(imp.cb_(s, status));
	}


};
CPPCOMPONENTS_REGISTER(ImpShutdownRequest)


struct ImpWriteRequest
	: public ImpRequest<ImpWriteRequest, WriteRequest_t>
{





	uv_write_t uv_t_;
	use<WriteCallback> cb_;
	use<IWriteRequest> self_;
	use<clv::IStream> handle_;
	use<clv::IStream> send_handle_;

	use<clv::IStream> GetHandle(){
		return handle_;
	}
	use<clv::IStream> GetSendHandle(){
		return send_handle_;
	}


	ImpWriteRequest(){
		throw error_not_implemented();
	}
	ImpWriteRequest(use<WriteCallback> cb, use<clv::IStream> h,
		use<clv::IStream> sh = nullptr) : cb_(cb),
		self_(this->QueryInterface<IWriteRequest>()),
		handle_(h), send_handle_(sh)
	{
		uv_t_.data = this;
	}

	static void RequestCb(uv_write_t* req, int status){
		auto& imp = *static_cast<ImpWriteRequest*>(req->data);
		auto s = imp.self_;
		imp.self_ = nullptr;
		SWALLOW_EXCEPTIONS(imp.cb_(s, status));
	}


};
CPPCOMPONENTS_REGISTER(ImpWriteRequest)


struct ImpConnectRequest
	: public ImpRequest<ImpConnectRequest, ConnectRequest_t>
{

	uv_connect_t uv_t_;
	use<ConnectCallback> cb_;
	use<IConnectRequest> self_;
	use<clv::IStream> handle_;

	use<clv::IStream> GetHandle(){
		return handle_;
	}



	ImpConnectRequest(){
		throw error_not_implemented();
	}
	ImpConnectRequest(use<ConnectCallback> cb, use<clv::IStream> h) : cb_(cb),
		self_(this->QueryInterface<IConnectRequest>()),
		handle_(h)
	{
		uv_t_.data = this;
	}

	static void RequestCb(uv_connect_t* req, int status){
		auto& imp = *static_cast<ImpConnectRequest*>(req->data);
		auto s = imp.self_;
		imp.self_ = nullptr;
		SWALLOW_EXCEPTIONS(imp.cb_(s, status));
	}


};
CPPCOMPONENTS_REGISTER(ImpConnectRequest)


struct ImpUdpSendRequest
	: public ImpRequest<ImpUdpSendRequest, UdpSendRequest_t>
{



	uv_udp_send_t uv_t_;
	use<UdpSendCallback> cb_;
	use<IUdpSendRequest> self_;
	use<clv::IStream> handle_;

	use<clv::IStream> GetHandle(){
		return handle_;
	}



	ImpUdpSendRequest(){
		throw error_not_implemented();
	}
	ImpUdpSendRequest(use<UdpSendCallback> cb, use<clv::IStream> h) : cb_(cb),
		self_(this->QueryInterface<IUdpSendRequest>()),
		handle_(h)
	{
		uv_t_.data = this;
	}

	static void RequestCb(uv_udp_send_t* req, int status){
		auto& imp = *static_cast<ImpUdpSendRequest*>(req->data);
		auto s = imp.self_;
		imp.self_ = nullptr;
		SWALLOW_EXCEPTIONS(imp.cb_(s, status));
	}


};
CPPCOMPONENTS_REGISTER(ImpUdpSendRequest)

struct ImpFsRequest
	: public ImpRequest<ImpFsRequest, FsRequest_t>
{



	uv_fs_t uv_t_;
	use<FsCallback> cb_;
	use<IFsRequest> self_;
	use<clv::ILoop> loop_;

	void Cleanup(){ return uv_fs_req_cleanup(&uv_t_); }

	use<clv::ILoop> GetLoop(){
		return loop_;
	}
	std::intptr_t GetResult(){
		return uv_t_.result;
	}

	void* GetPtr(){
		return uv_t_.ptr;
	}

	cr_string GetPath(){
		return uv_t_.path;
	}

	Stat_t GetStatBuf(){
		static_assert(sizeof(Stat_t) == sizeof(uv_stat_t),
			"Stat does not correlate with uv_stat_t");
		Stat_t ret;
		memcpy(&ret, &uv_t_.statbuf, sizeof(Stat_t));
		return ret;
	}

	ImpFsRequest(){
		throw error_not_implemented();
	}
	ImpFsRequest(use<FsCallback> cb, use<clv::ILoop> loop,
		use<clv::IStream> sh = nullptr) : cb_(cb),
		self_(this->QueryInterface<IFsRequest>()),
		loop_(loop)
	{
		uv_t_.data = this;
	}

	static void RequestCb(uv_fs_t* req){
		auto& imp = *static_cast<ImpFsRequest*>(req->data);
		auto s = imp.self_;
		imp.self_ = nullptr;
		SWALLOW_EXCEPTIONS(imp.cb_(s));
	}


};


CPPCOMPONENTS_REGISTER(ImpFsRequest)


struct ImpWorkRequest
	: public ImpRequest<ImpWorkRequest, WorkRequest_t>
{



	uv_work_t uv_t_;
	use<WorkCallback> cb_;
	use<AfterWorkCallback> awcb_;
	use<IWorkRequest> self_;
	use<clv::ILoop> loop_;

	use<clv::ILoop> GetLoop(){
		return loop_;
	}


	ImpWorkRequest(){
		throw error_not_implemented();
	}
	ImpWorkRequest(use<WorkCallback> cb, use<AfterWorkCallback> awcb,
		use<clv::ILoop> loop)
		: cb_(cb), awcb_(awcb),
		self_(this->QueryInterface<IWorkRequest>()),
		loop_(loop)

	{
		uv_t_.data = this;
	}

	static void RequestCb(uv_work_t* req){
		auto& imp = *static_cast<ImpWorkRequest*>(req->data);
		auto s = imp.self_;
		SWALLOW_EXCEPTIONS( imp.cb_(s));
	}
	static void AfterRequestCb(uv_work_t* req, int status){
		auto& imp = *static_cast<ImpWorkRequest*>(req->data);
		auto s = imp.self_;
		imp.self_ = nullptr;
		SWALLOW_EXCEPTIONS(imp.awcb_(s, status));
	}


};

CPPCOMPONENTS_REGISTER(ImpWorkRequest)



typedef runtime_class<dummyid, object_interfaces<IHandle>, factory_interface<NoConstructorFactoryInterface>> Handle_t;

template<class Derived, class HType>
struct ImpHandleBase{

	uv_handle_t* handle_;
	bool close_called_ = false;
	bool close_completed_ = false;
	bool deleteonclose_ = false;
	void* pcb_ = nullptr;

	HType* get(){
		return reinterpret_cast<HType*>(handle_);
	}

	ImpHandleBase(HType* h) : handle_{ reinterpret_cast<uv_handle_t*>(h) }
	{}

	int HandleType(){
		return handle_->type;
	}

	bool IsActive(){
		return uv_is_active(handle_) != 0;
	}

	static void CloseCallbackRaw(uv_handle_t *h){
		auto pthis = static_cast<ImpHandleBase*>(h->data);
		if (pthis->pcb_){
			use<CloseCallback> cb(cppcomponents::reinterpret_portable_base<CloseCallback>(static_cast<portable_base*>(pthis->pcb_)), false);
			SWALLOW_EXCEPTIONS(cb());
		}
		h->data = nullptr;
		pthis->close_completed_ = true;
		if (pthis->deleteonclose_){
			delete static_cast<Derived*>(pthis);
		}
		else{
		}
	}
	void ReleaseImplementationDestroy(){ 
		static_cast<Derived*>(this)->ResetCallbacks();
		if (close_completed_){
			delete static_cast<Derived*>(this);
		}
		else if (close_called_){
			deleteonclose_ = true;
		}
		else{
			deleteonclose_ = true;
			CloseRaw(nullptr);
		}
	}

	void ResetCallbacks(){}

	bool closed(){ return close_completed_; }

	void CloseRaw(use<CloseCallback> cb){
		if (close_called_) return;
		close_called_ = true;
		handle_->data = this;
		pcb_ = cb.get_portable_base_addref();
		uv_close(handle_, CloseCallbackRaw);
	}

	void Ref(){
		uv_ref(handle_);
	}

	void Unref(){
		uv_unref(handle_);
	}

	bool HasRef(){
		return uv_has_ref(handle_) != 0;
	}
	bool IsClosing(){
		return uv_is_closing(handle_) != 0;
	}
	void* UvHandle(){
		return handle_;
	}


};

struct ImpHandleNonOwning :
	ImpHandleBase<ImpHandleNonOwning,uv_handle_t>,
	implement_runtime_class<ImpHandleNonOwning, Handle_t>{




		ImpHandleNonOwning(uv_handle_t* h) : ImpHandleBase<ImpHandleNonOwning,uv_handle_t>(h)
		{}

		void ReleaseImplementationDestroy(){

		}

};
CPPCOMPONENTS_REGISTER(ImpHandleNonOwning)


struct ImpLoop : implement_runtime_class<ImpLoop, Loop_t>{

	// Whether we own the loop, so we can delete it in destructor
	bool owner_;
	uv_loop_t* loop_;
	ImpLoop() : owner_{ true }, loop_{ uv_loop_new() }
	{
	}

	ImpLoop(uv_loop_t* loop, bool owner = false) : owner_{ false }, loop_{ loop }
	{}

	~ImpLoop(){
		if (owner_){
			uv_loop_delete(loop_);
		}
	}

	static use<ILoop> DefaultLoop(){
		struct LoopHolder{
			use<ILoop> loop_;
			LoopHolder(uv_loop_t* loop) : loop_{ImpLoop::create(loop,false).QueryInterface<ILoop>()}{}
		};
		return cross_compiler_interface::detail::safe_static_init < LoopHolder, LoopHolder>::get(uv_default_loop()).loop_;
	}

	void Run(){
		throw_if_error(uv_run(loop_, UV_RUN_DEFAULT));
	}
	void RunOnce(){
		throw_if_error(uv_run(loop_, UV_RUN_ONCE));
	}
	void RunNoWait(){
		throw_if_error(uv_run(loop_, UV_RUN_NOWAIT));
	}

	void Stop(){
		uv_stop(loop_);
	}

	void UpdateTime(){
		uv_update_time(loop_);
	}

	std::uint64_t Now(){
		return uv_now(loop_);
	}

	int BackendFd(){
		return uv_backend_fd(loop_);
	}

	int BackendTimeout(){
		return uv_backend_timeout(loop_);
	}

	void* UvHandle(){
		return loop_;
	}

	static void WalkCallbackRaw(uv_handle_t *h, void* arg){
		use<WalkCallback> cb(cppcomponents::reinterpret_portable_base<WalkCallback>(static_cast<portable_base*>(arg)), true);
		cb(ImpHandleNonOwning::create(h).QueryInterface<IHandle>());
	}

	void WalkRaw(use<WalkCallback> cb){

		uv_walk(loop_, WalkCallbackRaw, cb.get_portable_base());
	}

	use<IWorkRequest> QueueWorkRaw(use<WorkCallback> wcb, use<AfterWorkCallback> awcb){
		auto wr = ImpWorkRequest::create(wcb, awcb, this->QueryInterface<ILoop>()).QueryInterface<IWorkRequest>();
		uv_queue_work(loop_, as_uv_type(wr),ImpWorkRequest::RequestCb,ImpWorkRequest::AfterRequestCb);
		return wr;
	}



};
CPPCOMPONENTS_REGISTER(ImpLoop)

void AllocCallbackRaw(uv_handle_t* handle, size_t suggested_size, uv_buf_t* ret){
	try{
		auto buf = cppcomponents::Buffer::Create(suggested_size);
		 *ret = uv_buf_init(buf.Begin(),suggested_size);
		 buf.get_portable_base_addref();
	}
	catch (std::exception&){
		*ret = uv_buf_init(nullptr, 0);
	}
}

template<class HType,class Derived>
struct ImpStreamBase : ImpHandleBase<Derived,uv_stream_t>{


	typedef cppcomponents::delegate < void(int) > ConnectionCallbackHelper;


	use<ConnectionCallback> connection_cb_;
	use<ReadCallback> read_cb_;

	bool reading_ = false;
	use<Read2Callback> read2_cb_;

	bool reading2_ = false;


	void ResetCallbacks(){
		connection_cb_ = nullptr;
		read_cb_ = nullptr;
	}
	use<IShutdownRequest> ShutdownRaw(cppcomponents::use<ShutdownCallback> cb){
		use<clv::IStream> is = static_cast<Derived*>(this)->template QueryInterface<clv::IStream>();
		return ImpShutdownRequest::create(cb, is).template QueryInterface<IShutdownRequest>();
	}

	static void ConnectionCallbackRaw(uv_stream_t* server, int status){
		auto derived = static_cast<Derived*>(reinterpret_cast<HType*>(server));
		ImpStreamBase& imp = *derived;
		if (!imp.connection_cb_){
			return;
		}
		auto is = derived->template QueryInterface<clv::IStream>();
		SWALLOW_EXCEPTIONS( imp.connection_cb_(is,status));
	}

	void ListenRaw(int backlog, use<ConnectionCallback> cb){

		connection_cb_ = cb; 
		throw_if_error(uv_listen(this->get(), backlog, ConnectionCallbackRaw));
	}

	void ClearListener(){
		connection_cb_ = nullptr; 
	}

	void Accept(use<clv::IStream> client){
		throw_if_error(uv_accept(this->get(), as_uv_type(client)));
	}

	static void ReadCallbackRaw(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf){
		//auto ibuf = cppcomponents::Buffer::OwningIBufferFromPointer(buf.base);
		auto b = cppcomponents::Buffer::OwningIBufferFromPointer(buf->base);
		if (nread >= 0){
			b.SetSize(nread);
		}

		auto derived = static_cast<Derived*>(reinterpret_cast<HType*>(stream));
		ImpStreamBase& imp = *derived;
		auto is = derived->template QueryInterface<clv::IStream>();
		SWALLOW_EXCEPTIONS( imp.read_cb_(is, nread, b));
		if (imp.reading_ == false){
			imp.read_cb_ = nullptr;
		}
	}

	void ReadStartRaw(use<ReadCallback> cb){
		reading_ = true;
		read_cb_ = cb;
		//istream_self_ = static_cast<Derived*>(this)->template QueryInterface<clv::IStream>();;
		try{
		throw_if_error(uv_read_start(this->get(), AllocCallbackRaw, ReadCallbackRaw));
		}
		catch (...){
			//istream_self_ = nullptr;
			reading_ = false;
			read_cb_ = nullptr;
			throw;
		}

	}

	void ReadStop(){
		uv_read_stop(this->get());
		reading_ = false;
		//istream_self_ = nullptr;
	}
	static void Read2CallbackRaw(uv_pipe_t* stream,ssize_t nread,	const uv_buf_t* buf, uv_handle_type pending){
		//auto ibuf = cppcomponents::Buffer::OwningIBufferFromPointer(buf.base);
		auto b = cppcomponents::Buffer::OwningIBufferFromPointer(buf->base);
		if (nread >= 0){
			b.SetSize(nread);
		}

		auto derived = static_cast<Derived*>(reinterpret_cast<HType*>(stream));
		ImpStreamBase& imp = *derived;
		auto is = derived->template QueryInterface<clv::IStream>();
		SWALLOW_EXCEPTIONS(imp.read2_cb_(is, nread, b,pending));
		if (imp.reading2_ == false){
			imp.read2_cb_ = nullptr;
		}
	}

	void Read2StartRaw(use<Read2Callback> cb){
		reading2_ = true;
		read2_cb_ = cb;
		try{
			throw_if_error(uv_read2_start(this->get(), AllocCallbackRaw, Read2CallbackRaw));
		}
		catch (...){
			reading_ = false;
			read_cb_ = nullptr;
			throw;
		}

	}

	void Read2Stop(){
		uv_read2_stop(this->get());
		reading2_ = false;
	}

	use<IWriteRequest> WriteRaw(clv::Buffer* bufs, int bufcnt, use<WriteCallback> cb){
		static_assert(sizeof(clv::Buffer) == sizeof(uv_buf_t), "clv::Buffer and uv_buf_t not compatible");
		auto wr = ImpWriteRequest::create(cb, static_cast<Derived*>(this)->template QueryInterface<clv::IStream>()).template QueryInterface<IWriteRequest>();
		throw_if_error(uv_write(as_uv_type(wr), this->get(), as_uv_type(bufs), bufcnt,
			ImpWriteRequest::RequestCb));

		return wr;

	}	
	use<IWriteRequest> Write2Raw(clv::Buffer* bufs, int bufcnt, use<clv::IStream> is,use<WriteCallback> cb){
		static_assert(sizeof(clv::Buffer) == sizeof(uv_buf_t), "clv::Buffer and uv_buf_t not compatible");
		auto wr = ImpWriteRequest::create(cb, static_cast<Derived*>(this)->template QueryInterface<clv::IStream>()).template QueryInterface<IWriteRequest>();;
		throw_if_error(uv_write2(as_uv_type(wr), this->get(), as_uv_type(bufs), bufcnt,
			as_uv_type(is),ImpWriteRequest::RequestCb));
		return wr;
	}

	bool IsReadable(){
		return uv_is_readable(this->get()) != 0;
	}

	bool IsWritable(){
		return uv_is_writable(this->get()) != 0;
	}

	void SetBlocking(bool blocking){
		uv_stream_set_blocking(this->get(), blocking);
	}

	ImpStreamBase(HType* h) : ImpHandleBase<Derived, uv_stream_t>(reinterpret_cast<uv_stream_t*>(h)){}



};




struct ImpTcpStream : uv_tcp_t, ImpStreamBase<uv_tcp_t,ImpTcpStream>, implement_runtime_class<ImpTcpStream, TcpStream_t>{

	typedef ImpStreamBase<uv_tcp_t, ImpTcpStream> imp_base_t;

	using imp_base_t::ReleaseImplementationDestroy;

	ImpTcpStream(use<ILoop> loop) : imp_base_t(this){
		throw_if_error(uv_tcp_init(as_uv_type(loop), this));
	}

	void Open(SocketOsType sock){
		throw_if_error(uv_tcp_open(this, static_cast<uv_os_sock_t>(sock)));
	}

	void NoDelay(bool enable){
		throw_if_error(uv_tcp_nodelay(this, enable));
	}

	void KeepAlive(bool enable, std::uint32_t delay){
		throw_if_error(uv_tcp_keepalive(this, enable, delay));
	}

	void SimultaneousAccepts(bool enable){
		throw_if_error(uv_tcp_simultaneous_accepts(this, enable));
	}

	void BindRaw(const sockaddr* in){
		throw_if_error(uv_tcp_bind(this, in));
	}



	void Getsockname(sockaddr* name, int* namelen){
		
		throw_if_error(uv_tcp_getsockname(this, name, namelen));
	}

	void Getpeername(sockaddr* name, int* namelen){
		throw_if_error(uv_tcp_getpeername(this, name, namelen));
	}

	use<IConnectRequest> ConnectRaw(const sockaddr* address, cppcomponents::use<ConnectCallback> cb){
		auto cr = ImpConnectRequest::create(cb, this->QueryInterface<clv::IStream>()).QueryInterface<IConnectRequest>();
		throw_if_error(uv_tcp_connect(as_uv_type(cr), this, address, ImpConnectRequest::RequestCb));
		return cr;
	}

	SocketOsType GetSocket(){
		return this->socket;
	}



};
CPPCOMPONENTS_REGISTER(ImpTcpStream)
struct ImpUdpStream : uv_udp_t, ImpStreamBase<uv_udp_t, ImpUdpStream>, implement_runtime_class<ImpUdpStream, UdpStream_t>
{
	typedef ImpStreamBase<uv_udp_t, ImpUdpStream> imp_base_t;
	using imp_base_t::ReleaseImplementationDestroy;


	use<UdpRecvCallback> cb_; 
	bool receiving_ = false;

	void ResetCallbacks(){
		cb_ = nullptr;
	}

	ImpUdpStream(use<ILoop> loop) : imp_base_t(this){
		throw_if_error(uv_udp_init(as_uv_type(loop), this));
	}


	void Open(SocketOsType sock){
		throw_if_error(uv_udp_open(this, static_cast<uv_os_sock_t>(sock)));
	}

	void BindRaw(const sockaddr* in, std::uint32_t flags){
		throw_if_error(uv_udp_bind(this, in, flags));
	}
	void GetSockname(sockaddr* name, int* namelen){
		throw_if_error(uv_udp_getsockname(this, name, namelen));
	}
	void SetMembership(cppcomponents::cr_string multicast_addr, cppcomponents::cr_string interface_addr,
		std::uint32_t membership){
			throw_if_error(uv_udp_set_membership(this, multicast_addr.data(), interface_addr.data(), 
				membership==UV_LEAVE_GROUP?UV_LEAVE_GROUP:UV_JOIN_GROUP));

	}
	void SetMulticastLoop(bool on){
		throw_if_error(uv_udp_set_multicast_loop(this, on));
	}
	void SetMulticastTtl(std::int32_t ttl){
		throw_if_error(uv_udp_set_multicast_ttl(this, ttl));
	}
	void SetBroadcast(bool on){
		throw_if_error(uv_udp_set_broadcast(this,on));
	}
	void SetTtl(std::int32_t ttl){
		throw_if_error(uv_udp_set_ttl(this, ttl));
	}
	use<IUdpSendRequest> SendRaw(clv::Buffer* bufs, int buffcnt, const sockaddr* addr,
		cppcomponents::use<UdpSendCallback> cb){
			auto sr = ImpUdpSendRequest::create(cb, this->QueryInterface<clv::IStream>()).QueryInterface<IUdpSendRequest>();
			throw_if_error(uv_udp_send(as_uv_type(sr), this,
				as_uv_type(bufs),buffcnt, addr, ImpUdpSendRequest::RequestCb));
			return sr;
	}



	static void RecvCallbackRaw(uv_udp_t* handle, ssize_t nread,const uv_buf_t* buf,
	const struct sockaddr* addr, unsigned flags){
		auto b = cppcomponents::Buffer::OwningIBufferFromPointer(buf->base);
		if (nread >= 0){
			b.SetSize(nread);
		}
		auto& imp = *static_cast <ImpUdpStream*>(handle);


		SWALLOW_EXCEPTIONS( imp.cb_(imp.QueryInterface<IUdpStream>(), nread, b,addr,flags));
		if (imp.receiving_ == false){
			imp.cb_= nullptr;
		}
	}


	void RecvStartRaw(cppcomponents::use<UdpRecvCallback> cb){
		cb_ = cb;
		receiving_ = true;
		try{
			throw_if_error(uv_udp_recv_start(this, AllocCallbackRaw, RecvCallbackRaw));

		}
		catch (...){
			receiving_ = false;
			cb_ = nullptr;
			throw;
		}
	}
	void RecvStop(){
		throw_if_error(uv_udp_recv_stop(this)); 
		receiving_ = false;
	}

	SocketOsType GetSocket(){
		return this->socket;
	}

};
CPPCOMPONENTS_REGISTER(ImpUdpStream)

struct ImpTtyStream : uv_tty_t, ImpStreamBase<uv_tty_t, ImpTtyStream>, implement_runtime_class<ImpTtyStream, Tty_t>
{
	typedef ImpStreamBase<uv_tty_t, ImpTtyStream> imp_base_t;
	using imp_base_t::ReleaseImplementationDestroy;


	ImpTtyStream(use<ILoop> loop, FileOsType fd, bool readable)
		: imp_base_t(this)
	{
		throw_if_error(uv_tty_init(as_uv_type(loop), this, fd, readable));
	}

	void SetMode(int mode){
		throw_if_error(uv_tty_set_mode(this, mode));
	}

	std::pair<int, int> GetWinsize(){
		std::pair<int, int> ret;
		throw_if_error(uv_tty_get_winsize(this, &ret.first, &ret.second));
		return ret;
	}

	static void ResetMode(){
		uv_tty_reset_mode();
	}




};
CPPCOMPONENTS_REGISTER(ImpTtyStream)

struct ImpPipe : uv_pipe_t, ImpStreamBase<uv_pipe_t,ImpPipe>, implement_runtime_class<ImpPipe, Pipe_t>
{
	typedef ImpStreamBase<uv_pipe_t, ImpPipe> imp_base_t;
	using imp_base_t::ReleaseImplementationDestroy;


	ImpPipe(use<ILoop> loop, bool ipc) : imp_base_t(this)
	{
		throw_if_error(uv_pipe_init(as_uv_type(loop), this, ipc));
	}

	void Open(FileOsType file){
		throw_if_error(uv_pipe_open(this, file));
	}

	void Bind(cr_string name){
		throw_if_error(uv_pipe_bind(this, name.data()));
	}

	use<IConnectRequest> ConnectRaw(cr_string name, use<ConnectCallback> cb){
		auto cr = ImpConnectRequest::create(cb, this->QueryInterface<clv::IStream>()).QueryInterface<IConnectRequest>();
		uv_pipe_connect(as_uv_type(cr), this, name.data(), ImpConnectRequest::RequestCb);
		return cr;
	}

	void PendingInstances(int count){
		uv_pipe_pending_instances(this, count);
	}


};
CPPCOMPONENTS_REGISTER(ImpPipe)
struct ImpPoll : uv_poll_t, ImpHandleBase<ImpPoll,uv_poll_t>, implement_runtime_class<ImpPoll, Poll_t>
{
	typedef ImpHandleBase<ImpPoll,uv_poll_t> imp_base_t;
	using imp_base_t::ReleaseImplementationDestroy;

	use<PollCallback> cb_; 

	void ResetCallbacks(){
		cb_ = nullptr;
	}

	ImpPoll(use<ILoop> loop, std::int64_t fileorsocket, bool isfile) : imp_base_t(this){
		if (isfile){
			throw_if_error(uv_poll_init(as_uv_type(loop), this, static_cast<FileOsType>(fileorsocket)));

		}
		else{
		throw_if_error(uv_poll_init_socket(as_uv_type(loop), this, static_cast<uv_os_sock_t>(fileorsocket)));

		}
	}


	static void PollCallbackRaw(uv_poll_t* handle, int status, int events){
		auto& imp = *static_cast<ImpPoll*>(handle);
		SWALLOW_EXCEPTIONS( imp.cb_(imp.QueryInterface<IPoll>(), status, events));

	}

	void StartRaw(int events, use<PollCallback> cb){
		cb_ = cb;
		try{
			throw_if_error(uv_poll_start(this, events,PollCallbackRaw));

		}
		catch (...){
			cb_ = nullptr;
			throw;
		}
	}

	void Stop(){
		throw_if_error(uv_poll_stop(this));
		cb_ = nullptr;
	}

	
};
CPPCOMPONENTS_REGISTER(ImpPoll)
struct ImpPrepare : uv_prepare_t, ImpHandleBase<ImpPrepare,uv_prepare_t>, implement_runtime_class<ImpPrepare, Prepare_t>
{
	typedef ImpHandleBase<ImpPrepare,uv_prepare_t> imp_base_t;

	void ReleaseImplementationDestroy(){
		imp_base_t::ReleaseImplementationDestroy();
	}

	use<PrepareCallback> cb_;


	void ResetCallbacks(){
		cb_ = nullptr;
	}

	ImpPrepare(use<ILoop> loop) : imp_base_t(this){
		throw_if_error(uv_prepare_init(as_uv_type(loop), this));
	}


	static void PrepareCallbackRaw(uv_prepare_t* handle, int status){
		auto& imp = *static_cast<ImpPrepare*>(handle);
		SWALLOW_EXCEPTIONS( imp.cb_(imp.QueryInterface<IPrepare>(), status));

	}

	void StartRaw(use<PrepareCallback> cb){
		cb_ = cb;
		try{
			throw_if_error(uv_prepare_start(this,  PrepareCallbackRaw));

		}
		catch (...){
			cb_ = nullptr;
			throw;
		}
	}

	void Stop(){
		throw_if_error(uv_prepare_stop(this));
		cb_ = nullptr;
	}



};
CPPCOMPONENTS_REGISTER(ImpPrepare)

struct ImpCheck : uv_check_t, ImpHandleBase<ImpCheck, uv_check_t>, implement_runtime_class<ImpCheck, Check_t>
{
	typedef ImpHandleBase<ImpCheck, uv_check_t> imp_base_t;
	using imp_base_t::ReleaseImplementationDestroy;

	use<CheckCallback> cb_;

	void ResetCallbacks(){
		cb_ = nullptr;
	}

	ImpCheck(use<ILoop> loop) : imp_base_t(this){
		throw_if_error(uv_check_init(as_uv_type(loop), this));
	}


	static void CheckCallbackRaw(uv_check_t* handle, int status){
		auto& imp = *static_cast<ImpCheck*>(handle);
		SWALLOW_EXCEPTIONS( imp.cb_(imp.QueryInterface<ICheck>(), status));

	}

	void StartRaw(use<CheckCallback> cb){
		cb_ = cb;
		try{
			throw_if_error(uv_check_start(this, CheckCallbackRaw));

		}
		catch (...){
			cb_ = nullptr;
			throw;
		}
	}

	void Stop(){
		throw_if_error(uv_check_stop(this));
		cb_ = nullptr;
	}


};
CPPCOMPONENTS_REGISTER(ImpCheck)


struct ImpIdle : uv_idle_t, ImpHandleBase<ImpIdle,uv_idle_t>, implement_runtime_class<ImpIdle, Idle_t>
{
	typedef ImpHandleBase<ImpIdle, uv_idle_t> imp_base_t;	
	using imp_base_t::ReleaseImplementationDestroy;

	use<IdleCallback> cb_;

	void ResetCallbacks(){
		cb_ = nullptr;
	}

	ImpIdle(use<ILoop> loop) : imp_base_t(this){
		throw_if_error(uv_idle_init(as_uv_type(loop), this));
	}


	static void IdleCallbackRaw(uv_idle_t* handle, int status){
		auto& imp = *static_cast<ImpIdle*>(handle);
		SWALLOW_EXCEPTIONS( imp.cb_(imp.QueryInterface<IIdle>(), status));

	}

	void StartRaw(use<IdleCallback> cb){
		cb_ = cb;
		try{
			throw_if_error(uv_idle_start(this, IdleCallbackRaw));

		}
		catch (...){
			cb_ = nullptr;
			throw;
		}
	}

	void Stop(){
		throw_if_error(uv_idle_stop(this));
		cb_ = nullptr;
	}




};

CPPCOMPONENTS_REGISTER(ImpIdle)

struct ImpAsync : uv_async_t, ImpHandleBase<ImpAsync,uv_async_t>, implement_runtime_class<ImpAsync, Async_t>
{
	typedef ImpHandleBase<ImpAsync,uv_async_t> imp_base_t;
	using imp_base_t::ReleaseImplementationDestroy;


	use<AsyncCallback> cb_;

	void ResetCallbacks(){
		cb_ = nullptr;
	}

	static void AsyncCallbackRaw(uv_async_t* handle, int status){
		auto& imp = *static_cast<ImpAsync*>(handle);
		SWALLOW_EXCEPTIONS(imp.cb_(imp.QueryInterface<IAsync>(), status));

	}



	ImpAsync(use<ILoop> loop,use<AsyncCallback> cb) : imp_base_t(this),cb_(cb)
	{
		throw_if_error(uv_async_init(as_uv_type(loop), this,AsyncCallbackRaw));


	}



	void Send(){
		throw_if_error(uv_async_send(this));
	}




};
CPPCOMPONENTS_REGISTER(ImpAsync)


struct ImpTimer :
	uv_timer_t, ImpHandleBase<ImpTimer,uv_timer_t>, cppcomponents::implement_runtime_class<ImpTimer, Timer_t>{
		typedef ImpHandleBase<ImpTimer,uv_timer_t> imp_base_t;
		using imp_base_t::ReleaseImplementationDestroy;

		use<TimerCallback> cb_;

		void ResetCallbacks(){
			cb_ = nullptr;
		}
		static void TimerCallbackRaw(uv_timer_t* handle, int status){
			auto& imp = *static_cast<ImpTimer*>(handle);
			SWALLOW_EXCEPTIONS( imp.cb_(imp.QueryInterface<ITimer>(), status));
		}

		void StartRaw(cppcomponents::use<TimerCallback> cb, std::uint64_t timeout, std::uint64_t repeat){
			try{
				cb_ = cb;

				throw_if_error(uv_timer_start(this, TimerCallbackRaw, timeout, repeat));

			}
			catch (...){
				cb_ = nullptr;
				throw;
			}
		}
	void Stop(){
		throw_if_error(uv_timer_stop(this));
	}
	void Again(){
		throw_if_error(uv_timer_again(this));
	}
	void SetRepeat(std::uint64_t repeat){
		uv_timer_set_repeat(this, repeat);
	}
	std::uint64_t GetRepeat(){
		return uv_timer_get_repeat(this);
	}

	ImpTimer(use<ILoop> loop) : imp_base_t(this){
		throw_if_error(uv_timer_init(as_uv_type(loop), this));
	}




};
CPPCOMPONENTS_REGISTER(ImpTimer)

struct ImpCpuInfo : implement_runtime_class<ImpCpuInfo, CpuInfo_t>{

	uv_cpu_info_t info_;

	ImpCpuInfo(const uv_cpu_info_t& info) : info_(info){}

	cppcomponents::cr_string GetModel(){
		cr_string ret{ info_.model};
		return ret;
	}
	std::int32_t GetSpeed(){
		return info_.speed;
	}
	std::int64_t GetUser(){
		return info_.cpu_times.user;
	}
	std::int64_t GetNice(){
		return info_.cpu_times.nice;
	}
	std::int64_t GetSys(){
		return info_.cpu_times.sys;
	}
	std::int64_t GetIdle(){
		return info_.cpu_times.idle;
	}
	std::int64_t GetIrq(){
		return info_.cpu_times.irq;
	}


	
};
CPPCOMPONENTS_REGISTER(ImpCpuInfo)


struct ImpInterfaceAddress : implement_runtime_class<ImpInterfaceAddress, InterfaceAddress_t>{
	uv_interface_address_t ia_;

	ImpInterfaceAddress(const uv_interface_address_t& i) : ia_(i){}
	cppcomponents::cr_string GetName(){
		return cr_string{ ia_.name };
	}
	cppcomponents::cr_string GetPhysAddr(){
		return cr_string{ ia_.phys_addr,sizeof(ia_.phys_addr)/sizeof(ia_.phys_addr[0]) };
	}
	bool IsInternal(){
		return ia_.is_internal != 0;
	}
	sockaddr_in GetAddress4(){
		return ia_.address.address4;
	}
	sockaddr_in6 GetAddress6(){
		return ia_.address.address6;
	}
	sockaddr_in GetNetMask4(){
		return ia_.netmask.netmask4;
	}
	sockaddr_in6 GetNetMask6(){
		return ia_.netmask.netmask6;
	}

};


CPPCOMPONENTS_REGISTER(ImpInterfaceAddress)



void assure_null_terminated(cr_string s){
	//if (s.size() && s[s.size() - 1] != 0)throw error_invalid_arg();
}

struct ImpUv : implement_runtime_class<ImpUv, Uv_t>{
	static int Version(){
		return uv_version();
	}
	static cppcomponents::cr_string VersionString(){
		return cr_string{ uv_version_string() };
	}
	static cppcomponents::cr_string Strerror(int err){
		return cr_string{ uv_strerror(err) };
	}
	static cppcomponents::cr_string ErrName(int err){
		return cr_string{ uv_err_name(err) };
	}
	static std::size_t HandleSize(int type){
		return uv_handle_size(static_cast<uv_handle_type>(type));
	}
	static std::size_t ReqSize(int type){
		return uv_req_size(static_cast<uv_req_type>(type));
	}
	static clv::Buffer BufInit(void* base, std::uint32_t len){
		auto b = uv_buf_init(static_cast<char*>(base), len);
		clv::Buffer ret;
		ret.base = b.base;
		ret.len = b.len;
		return ret;
	}
	static std::size_t Strlcpy(char* dst, const char* src, std::size_t size){
		return uv_strlcpy(dst, src, size);
	}
	static std::size_t Strlcat(char* dst, const char* src, std::size_t size){
		return uv_strlcat(dst, src, size);
	}
	static int GuessHandle(FileOsType file){
		return uv_guess_handle(file);
	}

	static use<IGetAddrinfoRequest> GetaddrinfoRaw(cppcomponents::use<ILoop> loop, cppcomponents::use<GetAddrinfoCallback> cb, cppcomponents::cr_string node,
		cppcomponents::cr_string service, addrinfo* hints){
			assure_null_terminated(node);
			assure_null_terminated(service);
			auto gr = ImpGetAddrinfoRequest::create(cb, loop).QueryInterface<IGetAddrinfoRequest>();
			throw_if_error(uv_getaddrinfo(as_uv_type(loop),as_uv_type(gr), ImpGetAddrinfoRequest::RequestCb,node.data(),service.data(),hints));
			return gr;
	}

	static void Freeaddrinfo(addrinfo* ai){
		uv_freeaddrinfo(ai);
	}

	static char** SetupArgs(int argc, char** argv){
		return  uv_setup_args(argc, argv);
	}

	static std::string GetProcessTitle(){
		std::array<char, 256> ar;
		throw_if_error(uv_get_process_title(ar.data(), ar.size()));
		return std::string{ ar.data() };
		
	}
	static void SetProcessTitle(cppcomponents::cr_string title){
		assure_null_terminated(title);
		throw_if_error(uv_set_process_title(title.data()));
	}
	static std::size_t ResidentSetMemory(){
		std::size_t ret = 0;
		throw_if_error(uv_resident_set_memory(&ret));
		return ret;
	}
	static double Uptime(){
		double ret = 0.0;
		throw_if_error(uv_uptime(&ret));
		return ret;
	}

	static std::vector<cppcomponents::use<ICpuInfo>> CpuInfo(){
		std::vector<use<ICpuInfo> > ret;
		int count = 0;
		uv_cpu_info_t* info = nullptr;
		throw_if_error(uv_cpu_info(&info, &count));

		auto deleter = [count](uv_cpu_info_t* info){
			uv_free_cpu_info(info, count);
		};
		std::unique_ptr<uv_cpu_info_t, decltype(deleter)> ptr(info, deleter);
		
		ret.reserve(count);
		std::transform(info, info + count, std::back_inserter(ret), [](const uv_cpu_info_t& i){
			return ImpCpuInfo::create(i).QueryInterface<ICpuInfo>();
		});
		return ret;

	}
	static std::vector<cppcomponents::use<IInterfaceAddress>> InterfaceAddresses(){
		std::vector<use<IInterfaceAddress> > ret;
		int count = 0;
		uv_interface_address_t* info = nullptr;
		throw_if_error(uv_interface_addresses(&info, &count));

		auto deleter = [count](uv_interface_address_t* info){
			uv_free_interface_addresses(info, count);
		};
		std::unique_ptr<uv_interface_address_t, decltype(deleter)> ptr(info, deleter);

		ret.reserve(count);
		std::transform(info, info + count, std::back_inserter(ret), [](const uv_interface_address_t& i){
			return ImpInterfaceAddress::create(i).QueryInterface<IInterfaceAddress>();
		});
		return ret;
	}
	static std::vector<double> Loadavg(){
		std::vector<double> ret(3);
		uv_loadavg(&ret[0]);
		return ret;
	}

	static sockaddr_in Ip4Addr(cr_string ip, int port){
		assure_null_terminated(ip);
		sockaddr_in ret{};
		throw_if_error(uv_ip4_addr(ip.data(), port,&ret));
		return ret;
	}
	static sockaddr_in6 Ip6Addr(cr_string ip, int port){
		assure_null_terminated(ip);
		sockaddr_in6 ret{};
		throw_if_error(uv_ip6_addr(ip.data(), port,&ret));
		return ret;

	}

	static std::string Ip4Name(sockaddr_in* src){
		std::array<char, 256> ar;
		throw_if_error(uv_ip4_name(src, ar.data(), ar.size()));
		return std::string(ar.data());
	}
	static std::string Ip6Name(sockaddr_in6* src){
		std::array<char, 256> ar;
		throw_if_error(uv_ip6_name(src, ar.data(), ar.size()));
		return std::string(ar.data());
	}

	static std::string InetNtop(int af, const void* src){
		std::array<char, 256> ar;
		throw_if_error(uv_inet_ntop(af,src, ar.data(), ar.size()));
		return std::string(ar.data());
	}
	static void InetPton(int af, cr_string src, void* dst){
		assure_null_terminated(src);
		throw_if_error(uv_inet_pton(af, src.data(), dst));
	}

	static std::string Exepath(){
		std::array<char, 512> ar;
		auto size = ar.size();
		throw_if_error(uv_exepath(ar.data(), &size));
		return std::string(ar.data(),size);

	}
	static std::string Cwd(){
		std::array<char, 512> ar;
		throw_if_error(uv_cwd(ar.data(), ar.size()));
		return std::string(ar.data());

	}
	static void Chdir(cr_string dir){
		assure_null_terminated(dir);
		throw_if_error(uv_chdir(dir.data()));
	}

	static std::uint64_t GetFreeMemory(){
		return uv_get_free_memory();
	}
	static std::uint64_t GetTotalMemory(){
		return uv_get_total_memory();
	}

	static std::uint64_t Hrtime(){
		return uv_hrtime();
	}

	static void DisableStdioInheritance(){
		uv_disable_stdio_inheritance();
	}

	static use<IUvExecutor> DefaultExecutor(){
		struct uniq{};
		return cross_compiler_interface::detail::safe_static_init<Executor, uniq>::get(Loop::DefaultLoop());
	}

	ImpUv(){}
};
CPPCOMPONENTS_REGISTER(ImpUv)

struct ImpProcessOptions : implement_runtime_class<ImpProcessOptions, ProcessOptions_t>
{
	use<ExitCallback> exit_cb_;

	uv_process_options_t p_;
	void SetExitCallback(cppcomponents::use<ExitCallback> cb){
		exit_cb_ = cb;
	}
	cppcomponents::use<ExitCallback> GetExitCallback(){
		return exit_cb_;
	}

	std::string file_;
	void SetFile(cppcomponents::cr_string file){
		file_ = file.to_string();
	}
	std::string GetFile(){
		return file_;
	}
	std::vector<std::string> args_;
	void SetArgs(std::vector<cppcomponents::cr_string> args){
		args_.reserve(args.size());
		std::transform(args.begin(), args.end(), std::back_inserter(args_), [](cr_string s){
			return s.to_string();
		});
	}
	std::vector<cppcomponents::cr_string> GetArgs(){
		std::vector<cr_string> ret(args_.begin(), args_.end());
		return ret;
	}
	std::vector<std::string> env_;
	void SetEnv(std::vector<cppcomponents::cr_string> env){
		env_.reserve(env.size());
		std::transform(env.begin(), env.end(), std::back_inserter(env_), [](cr_string s){
			return s.to_string();
		});
	}
	std::vector<cppcomponents::cr_string> GetEnv(){
		std::vector<cr_string> ret(env_.begin(), env_.end());
		return ret;

	}
	std::string cwd_;
	void SetCwd(cppcomponents::cr_string cwd){
		cwd_ = cwd.to_string();
	}
	std::string GetCwd(){
		return cwd_;
	}
	unsigned int flags_; 
	unsigned int GetFlags(){
		return flags_;
	}
	void SetFlags(unsigned int flags){
		flags_ = flags;
	}

	std::vector<use<IStdioContainer>> container_;
	void SetStdio(std::vector < use < IStdioContainer >> container){
		container_ = container;
	}
	std::vector<use<IStdioContainer>> GetStdio(){
		return container_;
	}

	unsigned char uid_;
	void SetUid(unsigned char uid){
		uid_ = uid;
	}
	unsigned char GetUid(){
		return uid_;
	}
	unsigned char gid_;
	void SetGid(unsigned char gid){
		gid_ = gid;
	}
	unsigned char GetGid(){
		return gid_;
	}

	ImpProcessOptions()
		: flags_{ 0 }, uid_{ 0 }, gid_{ 0 }{}
};
CPPCOMPONENTS_REGISTER(ImpProcessOptions)

struct ImpStdioContainer : implement_runtime_class<ImpStdioContainer, StdioContainer_t>{

	int flags_;
	int GetFlags(){
		return flags_;
	}
	void SetFlags(int flags){
		flags_ = flags;
	}
	bool bstream_;
	bool IsStream(){
		return bstream_;
	}
	use<clv::IStream> stream_;
	void SetStream(cppcomponents::use<clv::IStream> s){
		stream_ = s;
		bstream_ = true;
	}
	cppcomponents::use<clv::IStream> GetStream(){
		return stream_;
	}
	FileOsType fd_;
	void SetFd(FileOsType fd){
		fd_ = fd;
		bstream_ = false;
	}
	FileOsType GetFd(){
		return fd_;
	}

	ImpStdioContainer()
		: flags_{ 0 }, bstream_{ false }, fd_{ 0 }{}
};
CPPCOMPONENTS_REGISTER(ImpStdioContainer)


struct ImpProcess :uv_process_t, ImpHandleBase<ImpProcess,uv_process_t>, implement_runtime_class<ImpProcess, Process_t>
{
	typedef ImpHandleBase<ImpProcess,uv_process_t> imp_base_t;
	using imp_base_t::ReleaseImplementationDestroy;

	use <ExitCallback> cb_;

	void ResetCallbacks(){
		cb_ = nullptr;
	}
	void Kill(int signum){
		throw_if_error(uv_process_kill(this, signum));

	}

	int GetPid(){
		return this->pid;
	}

	static void KillProcess(int pid, int signum){
		throw_if_error(uv_kill(pid, signum));
	}


	static void ExitCallbackRaw(uv_process_t* p, int64_t exit_status, int sig_term){
		auto& imp = *static_cast<ImpProcess*>(p);
		SWALLOW_EXCEPTIONS( imp.cb_(imp.QueryInterface<IProcess>(), exit_status, sig_term));
	}

	ImpProcess(use<ILoop> loop, use<IProcessOptions> po):imp_base_t{this}{
		cb_ = po.GetExitCallback();
		uv_process_options_t uvpo = { 0 };
		std::vector<const char*> args;
		for (auto& s : po.GetArgs()){
			args.push_back(s.data());
		}

		// Add null to end of arguments array per convention
		args.push_back(nullptr);

		uvpo.args = const_cast<char**>(&args[0]);
		uvpo.cwd = const_cast<char*>(po.GetCwd().c_str());

		auto venv = po.GetEnv();
		if (venv.empty()){
			uvpo.env = nullptr;
		}
		else{
			std::vector<const char*> env;
			for (auto& s : venv){
				env.push_back(s.data());
			}
			uvpo.env = const_cast<char**>(&env[0]);

		}
	
		if (cb_){
			uvpo.exit_cb = ExitCallbackRaw;
		}
		std::string file = po.GetFile();
		uvpo.file = file.c_str();

		uvpo.flags = po.GetFlags();
		uvpo.gid = po.GetGid();
		auto stdio = po.GetStdio();
		if (!stdio.empty()){
			std::vector<uv_stdio_container_t> uvstdio;
			for (auto& c : stdio){
				uv_stdio_container_t uvc = { };
				uvc.flags =static_cast<uv_stdio_flags>(c.GetFlags());
				if (c.IsStream()){
					uvc.data.stream = as_uv_type(c.GetStream());
				}
				else{
					uvc.data.fd = c.GetFd();
				}
				uvstdio.push_back(uvc);
			}
			uvpo.stdio = &uvstdio[0];
			uvpo.stdio_count = uvstdio.size();
		}
		uvpo.uid = po.GetUid();
		throw_if_error(uv_spawn(as_uv_type(loop), this, &uvpo));
	}

};
CPPCOMPONENTS_REGISTER(ImpProcess)


struct ImpFs : implement_runtime_class<ImpFs, Fs_t>{
	static use<IFsRequest> CloseRaw(use<ILoop> loop , FileOsType file, use<FsCallback> cb){
		auto ret = ImpFsRequest::create(cb,loop).QueryInterface<IFsRequest>();
		throw_if_error(uv_fs_close(as_uv_type(loop), as_uv_type(ret), file, ImpFsRequest::RequestCb));
		return ret;
	}

	static use<IFsRequest> OpenRaw(use<ILoop> loop, cr_string path, int flags,
		int mode, use<FsCallback> cb){
			assure_null_terminated(path);
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_open(as_uv_type(loop), as_uv_type(ret), path.data(),flags,mode, ImpFsRequest::RequestCb));
			return ret;
	}

	static use<IFsRequest> ReadRaw(use<ILoop> loop, FileOsType file, void* buf,
		std::size_t length, std::int64_t offset, use<FsCallback> cb){
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_read(as_uv_type(loop), as_uv_type(ret), file, buf,length,offset,ImpFsRequest::RequestCb));
			return ret;

	}

	static use<IFsRequest> UnlinkRaw(use<ILoop> loop, cr_string path, use<FsCallback> cb){
		auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
		throw_if_error(uv_fs_unlink(as_uv_type(loop), as_uv_type(ret), path.data(), ImpFsRequest::RequestCb));
		return ret;

	}

	static use<IFsRequest> WriteRaw(use<ILoop> loop, FileOsType file, const void* buf,
		std::size_t length, std::int64_t offset, use<FsCallback> cb){
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_write(as_uv_type(loop), as_uv_type(ret), file,buf,length,offset, ImpFsRequest::RequestCb));
			return ret;

	}

	static use<IFsRequest> MkdirRaw(use<ILoop> loop, cr_string path, int mode, use<FsCallback> cb){
		assure_null_terminated(path);
		auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
		throw_if_error(uv_fs_mkdir(as_uv_type(loop), as_uv_type(ret), path.data(),mode, ImpFsRequest::RequestCb));
		return ret;

	}
	static use<IFsRequest> RmdirRaw(use<ILoop> loop, cr_string path, use<FsCallback> cb){
		assure_null_terminated(path);
		auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
		throw_if_error(uv_fs_rmdir(as_uv_type(loop), as_uv_type(ret), path.data(),ImpFsRequest::RequestCb));
		return ret;

	}

	static use<IFsRequest> ReaddirRaw(use<ILoop> loop, cr_string path, int flags, use<FsCallback> cb){
		assure_null_terminated(path);
		auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
		throw_if_error(uv_fs_readdir(as_uv_type(loop), as_uv_type(ret), path.data(),flags,ImpFsRequest::RequestCb));
		return ret;
	}
	static use<IFsRequest> StatRaw(use<ILoop> loop, cr_string path, use<FsCallback> cb){
		assure_null_terminated(path);
		auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
		throw_if_error(uv_fs_stat(as_uv_type(loop), as_uv_type(ret), path.data(),ImpFsRequest::RequestCb));
		return ret;

	}
	static use<IFsRequest> FstatRaw(use<ILoop> loop, FileOsType file, use<FsCallback> cb){
		auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
		throw_if_error(uv_fs_fstat(as_uv_type(loop), as_uv_type(ret), file, ImpFsRequest::RequestCb));
		return ret;

	}
	static use<IFsRequest> RenameRaw(use<ILoop> loop, cr_string path, cr_string new_path, use<FsCallback> cb){
		assure_null_terminated(path);
		assure_null_terminated(new_path);
		auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
		throw_if_error(uv_fs_rename(as_uv_type(loop), as_uv_type(ret), path.data(),new_path.data(),ImpFsRequest::RequestCb));
		return ret;

	}
	static use<IFsRequest> FsyncRaw(use<ILoop> loop, FileOsType file, use<FsCallback> cb){
		auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
		throw_if_error(uv_fs_fsync(as_uv_type(loop), as_uv_type(ret), file, ImpFsRequest::RequestCb));
		return ret;

	}
	static use<IFsRequest> FdatasyncRaw(use<ILoop> loop, FileOsType file, use<FsCallback> cb){
		auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
		throw_if_error(uv_fs_fdatasync(as_uv_type(loop), as_uv_type(ret), file, ImpFsRequest::RequestCb));
		return ret;

	}
	static use<IFsRequest> FtruncateRaw(use<ILoop> loop, FileOsType file,
		std::int64_t offset, use<FsCallback> cb){
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_ftruncate(as_uv_type(loop), as_uv_type(ret), file,offset, ImpFsRequest::RequestCb));
			return ret;

	}
	static use<IFsRequest> SendfileRaw(use<ILoop> loop, FileOsType file_out, FileOsType file_in,
		std::int64_t in_offset, std::size_t length, use<FsCallback> cb){
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_sendfile(as_uv_type(loop), as_uv_type(ret), file_out,file_in,in_offset,length, ImpFsRequest::RequestCb));
			return ret;

	}
	static use<IFsRequest> ChmodRaw(use<ILoop> loop, cr_string path,
		int mode, use<FsCallback> cb){
			assure_null_terminated(path);
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_chmod(as_uv_type(loop), as_uv_type(ret), path.data(),mode, ImpFsRequest::RequestCb));
			return ret;

	}
	static use<IFsRequest> UtimeRaw(use<ILoop> loop, cr_string path, double atime,
		double mtime, use<FsCallback> cb){
			assure_null_terminated(path);
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_utime(as_uv_type(loop), as_uv_type(ret), path.data(),atime,mtime, ImpFsRequest::RequestCb));
			return ret;

	}
	static use<IFsRequest> FutimeRaw(use<ILoop> loop, FileOsType file, double atime,
		double mtime, use<FsCallback> cb){
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_futime(as_uv_type(loop), as_uv_type(ret), file,atime,mtime, ImpFsRequest::RequestCb));
			return ret;

	}
	static use<IFsRequest> LstatRaw(use<ILoop> loop, cr_string path, use<FsCallback>cb){
			assure_null_terminated(path);
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_lstat(as_uv_type(loop), as_uv_type(ret), path.data(), ImpFsRequest::RequestCb));
			return ret;

	}
	static use<IFsRequest> LinkRaw(use<ILoop> loop, cr_string path, cr_string new_path,
		use<FsCallback> cb){
			assure_null_terminated(path);
			assure_null_terminated(new_path);
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_link(as_uv_type(loop), as_uv_type(ret), path.data(),new_path.data(), ImpFsRequest::RequestCb));
			return ret;

	}

	static use<IFsRequest> SymlinkRaw(use<ILoop> loop, cr_string path,
		cr_string new_path, int flags, use<FsCallback> cb){
			assure_null_terminated(path);
			assure_null_terminated(new_path);
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_symlink(as_uv_type(loop), as_uv_type(ret), path.data(),new_path.data(),flags, ImpFsRequest::RequestCb));
			return ret;

	}
	static use<IFsRequest> ReadlinkRaw(use<ILoop>loop, cr_string path, use<FsCallback> cb){
			assure_null_terminated(path);
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_readlink(as_uv_type(loop), as_uv_type(ret), path.data(), ImpFsRequest::RequestCb));
			return ret;



	}

	static use<IFsRequest> FchmodRaw(use<ILoop> loop, FileOsType file,
		int mode, use<FsCallback> cb){
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_fchmod(as_uv_type(loop), as_uv_type(ret), file,mode, ImpFsRequest::RequestCb));
			return ret;
	}

	static use<IFsRequest> ChownRaw(use<ILoop> loop, cr_string path, unsigned char uid,
		unsigned char gid, use<FsCallback> cb){
			assure_null_terminated(path);
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_chown(as_uv_type(loop), as_uv_type(ret), path.data(),uid,gid, ImpFsRequest::RequestCb));
			return ret;
	}

	static use<IFsRequest> FchownRaw(use<ILoop> loop, FileOsType file, unsigned char uid,
		unsigned char gid, use<FsCallback> cb){
			auto ret = ImpFsRequest::create(cb, loop).QueryInterface<IFsRequest>();
			throw_if_error(uv_fs_fchown(as_uv_type(loop), as_uv_type(ret), file,uid,gid,ImpFsRequest::RequestCb));
			return ret;

	}

	ImpFs(){}

};


CPPCOMPONENTS_REGISTER(ImpFs)


struct ImpFsPoll : uv_fs_poll_t, ImpHandleBase<ImpFsPoll,uv_fs_poll_t>, implement_runtime_class<ImpFsPoll, FsPoll_t>{
	typedef ImpHandleBase<ImpFsPoll,uv_fs_poll_t> imp_base_t;
	using imp_base_t::ReleaseImplementationDestroy;


	use<FsPollCallback> cb_;
	void ResetCallbacks(){
		cb_ = nullptr;
	}

	static void PollCallbackRaw(uv_fs_poll_t* handle,int status,
		const uv_stat_t* prev, const uv_stat_t* curr){
		auto& imp = *static_cast<ImpFsPoll*>(handle);
		static_assert(sizeof(cppcomponents_libuv::Stat_t) == sizeof(uv_stat_t), "Mismatch Stat, uv_stat_t");
		SWALLOW_EXCEPTIONS(imp.cb_(imp.QueryInterface<IFsPoll>(), status, reinterpret_cast<const Stat_t*>(prev),
			reinterpret_cast<const Stat_t*>(curr)));
	}
	void StartRaw(use<FsPollCallback> cb, cr_string path, unsigned int msinterval){
		cb_ = cb;
		try{
			throw_if_error(uv_fs_poll_start(this, PollCallbackRaw, path.data(), msinterval));
		}
		catch (...){
			ResetCallbacks();
			throw;
		}
	}
	void Stop(){
		throw_if_error(uv_fs_poll_stop(this));
		ResetCallbacks();
	}

	ImpFsPoll(use<ILoop> loop) : imp_base_t{ this }
	{
		throw_if_error(uv_fs_poll_init(as_uv_type(loop), this));
	}




};
CPPCOMPONENTS_REGISTER(ImpFsPoll)


struct ImpSignal :uv_signal_t,ImpHandleBase<ImpSignal,uv_signal_t>, implement_runtime_class<ImpSignal, Signal_t>{
	typedef ImpHandleBase<ImpSignal,uv_signal_t> imp_base_t;
	using imp_base_t::ReleaseImplementationDestroy;

	use<SignalCallback> cb_;
	

	void ResetCallbacks(){
		cb_ = nullptr;
	}
	static void SignalCallbackRaw(uv_signal_t* handle, int signum){
		auto& imp = *static_cast<ImpSignal*>(handle);
		SWALLOW_EXCEPTIONS(imp.cb_(imp.QueryInterface<ISignal>(), signum));
	}
	void StartRaw(use<SignalCallback> cb, int signum){
		cb_ = cb;
		try{
			throw_if_error(uv_signal_start(this, SignalCallbackRaw, signum));
		}
		catch (...){
			ResetCallbacks();
			throw;
		}
	}
	void Stop(){
		throw_if_error(uv_signal_stop(this));
		ResetCallbacks();

	}
	ImpSignal(use<ILoop> loop) : imp_base_t{ this }
	{
		throw_if_error(uv_signal_init(as_uv_type(loop), this));
	}




};
CPPCOMPONENTS_REGISTER(ImpSignal)

struct ImpFsEvent : uv_fs_event_t, ImpHandleBase<ImpFsEvent,uv_fs_event_t>
	, implement_runtime_class<ImpFsEvent, FsEvent_t>{

		typedef ImpHandleBase<ImpFsEvent,uv_fs_event_t> imp_base_t;
		using imp_base_t::ReleaseImplementationDestroy;


		use<FsEventCallback> cb_;
		void ResetCallbacks(){
			cb_ = nullptr;
		}
		static void FsEventCallbackRaw(uv_fs_event_t* handle, const char* filename,
			int events, int status){
				auto& imp = *static_cast<ImpFsEvent*>(handle);
				cr_string fname(filename);
				SWALLOW_EXCEPTIONS( imp.cb_(imp.QueryInterface<IFsEvent>(), fname,events,status));

		}

		ImpFsEvent(use<ILoop> loop, cr_string filename, use<FsEventCallback> cb, int flags)
			:imp_base_t{this}
		{
			assure_null_terminated(filename);
			cb_ = cb;
			throw_if_error(uv_fs_event_init(as_uv_type(loop), this, filename.data(), FsEventCallbackRaw, flags));
		}
		




};

CPPCOMPONENTS_REGISTER(ImpFsEvent)


struct ImpMutex : implement_runtime_class<ImpMutex, Mutex_t>{
	uv_mutex_t mut_;
	
	void Lock(){
		uv_mutex_lock(&mut_);
	}
	bool Trylock(){
		return uv_mutex_trylock(&mut_)==0;
	}
	void Unlock(){
		uv_mutex_unlock(&mut_);
	}
	void* UvHandle(){
		return &mut_;
	}

	ImpMutex(){
		throw_if_error(uv_mutex_init(&mut_));
	}

	~ImpMutex(){
		uv_mutex_destroy(&mut_);
	}
};
CPPCOMPONENTS_REGISTER(ImpMutex)
struct ImpRwlock : implement_runtime_class<ImpRwlock, Rwlock_t>{

	uv_rwlock_t h_;

	void Rdlock(){
		uv_rwlock_rdlock(&h_);
	}
	bool Tryrdlock(){
		return uv_rwlock_tryrdlock(&h_) == 0;
	}
	void Rdunlock(){
		uv_rwlock_rdunlock(&h_);
	}
	void Wrlock(){
		uv_rwlock_wrlock(&h_);
	}
	bool Trywrlock(){
		return uv_rwlock_trywrlock(&h_) == 0;
	}
	void Wrunlock(){
		uv_rwlock_wrunlock(&h_);
	}

	ImpRwlock(){
		throw_if_error(uv_rwlock_init(&h_));
	}

	~ImpRwlock(){
		uv_rwlock_destroy(&h_);
	}
};
CPPCOMPONENTS_REGISTER(ImpRwlock)

struct ImpExecutor : cppcomponents::implement_runtime_class<ImpExecutor,Executor_t>{

	cppcomponents::LoopExecutor exec_;
	use<IPrepare> prep_;
	use<IIdle> idle_;
	use<ILoop> loop_;
	std::atomic<bool> stop_;

	ImpExecutor(use<ILoop> loop) :loop_{ loop }, prep_{ Prepare{ loop } }, idle_{ Idle{ loop } }, stop_{ false }{
	}


	~ImpExecutor(){
		prep_ = nullptr;
		idle_ = nullptr;
		loop_.RunNoWait();
	}

	typedef delegate < void()> ClosureType;

	void AddDelegate(use<ClosureType> f){
		exec_.Add(f);
	}
	std::size_t NumPendingClosures(){
		return exec_.NumPendingClosures();
	}
	void SetupPrepare(){
		auto pthis = this;
		// This assures the loop will at least run once
		prep_.Start([pthis](cppcomponents::use<IPrepare> prep, int){
			pthis->exec_.RunQueuedClosures();
			if (pthis->stop_.load() == true){
				pthis->loop_.Stop();
			}
		});
		// This assures the loop will at least run once
		idle_.Start([pthis](cppcomponents::use<IIdle> , int){
			pthis->exec_.RunQueuedClosures();
			if (pthis->stop_.load() == true){
				pthis->loop_.Stop();
			}
		});


	}
	void SetupPrepareOne(){
		auto pthis = this;
		// This assures the loop will at least run once
		prep_.Start([pthis](cppcomponents::use<IPrepare> prep, int){
			pthis->exec_.TryOneClosure();
		});
	}

	void Run(){
		stop_.store(false);
		SetupPrepare();
		loop_.Run();
		while (exec_.NumPendingClosures() > 0){
			loop_.Run();
		}
		prep_.Stop();
	}	
	void Loop(){
		stop_.store(false);
		SetupPrepare();
		while (stop_.load()==false){
			loop_.Run();
		}
		loop_.RunNoWait();
		prep_.Stop();
		idle_.Stop();
	}
	void RunQueuedClosures(){
		stop_.store(false);
		SetupPrepare();
		loop_.RunNoWait();
		prep_.Stop();
		idle_.Stop();
	}
	bool TryOneClosure(){
		stop_.store(false);
		SetupPrepareOne();
		loop_.RunOnce();
		prep_.Stop();
		idle_.Stop();

		// Always return true 
		// since we don't know that loop did not do any work
		return true;
	}
	void MakeLoopExit(){
		stop_.store(true);
		exec_.MakeLoopExit();
	}

	use<ILoop> GetLoop(){
		return loop_;
	}
};


CPPCOMPONENTS_REGISTER(ImpExecutor)


struct ImpThreadPoolExecutor :implement_runtime_class<ImpThreadPoolExecutor, ThreadPoolExecutor_t>
{
	typedef cppcomponents::delegate < void() > ClosureType;
	std::atomic<std::uint32_t> count_;

	use<IUvExecutor> exec_;


	ImpThreadPoolExecutor(use<IUvExecutor> exec)
		:count_(0), exec_{ exec }
	{}

	use<ILoop> GetLoop(){ return exec_.GetLoop(); }

	void AddDelegate(use<ClosureType> f){

		// The iface keeps a reference to this so it does not get destroyed
		// until the closure completes
		auto iface = this->QueryInterface<InterfaceUnknown>();
		count_.fetch_add(1);

		// We cannot directly call loop.QueueWork becase it like almost all of libuv is not 
		// thread safe. Therefore we call the executer.Add function (which is thread-safe)
		// To execute on the main loop
		auto func = [iface, this,f]()mutable{
			exec_.GetLoop().QueueWork(f).Then([iface, this](Future<void> f)mutable{
				this->count_.fetch_sub(1);
				iface = nullptr;
			});
			iface = nullptr;
			f = nullptr;
		};
		exec_.Add(func);
	}
	std::size_t NumPendingClosures(){
		return count_.load();
	}



};
CPPCOMPONENTS_REGISTER(ImpThreadPoolExecutor)

CPPCOMPONENTS_DEFINE_FACTORY();

extern "C"{

	CROSS_CALL_EXPORT_FUNCTION std::size_t CROSS_CALL_CALLING_CONVENTION GetObjectCount(){
		return cross_compiler_interface::object_counter::get().get_count();
	}
}
