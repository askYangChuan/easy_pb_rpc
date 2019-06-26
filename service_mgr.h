#include <iostream>
#include <map>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "common.h"
#include "rpc.pb.h"

namespace PBRPC {

using google::protobuf::MethodDescriptor;
using google::protobuf::Message;
using google::protobuf::Service;

// if using boost
//#include <boost/function.hpp>
//#include <boost/noncopyable.hpp>
//#include <boost/shared_ptr.hpp>


class RpcServiceMgr  {
public:


	struct ServiceData {
		google::protobuf::Service *_rpc_service;
		const MethodDescriptor *_method_descriptor;
		const Message *_request_proto;
		const Message *_response_proto;
	};

private:
    typedef std::map<std::string, ServiceData> RpcServiceMap;
    RpcServiceMap _services;
	//ServiceData *_services;

protected:
	bool AddMethod (std::string service_name, const MethodDescriptor *method_descriptor, const Message *request_proto, const Message *response_proto, Service *rpc_service);

    
    ServiceData *get_new_service(std::string service_name);
	ServiceData *GetService(std::string service_name);

public:
	RpcServiceMgr() {}
	~RpcServiceMgr();


    void HandleDefaultRpcCall(RPC::RpcRequestData &rpc_data, std::string &ret_data);
	void HandleRpcCall(unsigned char *call_data, size_t length, std::string &ret_data, google::protobuf::RpcController *);
	bool RegisterRpcService(::google::protobuf::Service *rpc_service);
};
}
