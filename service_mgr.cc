#include "rpc.pb.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include "service_mgr.h"

using namespace ::google::protobuf;
using namespace PBRPC;


RpcServiceMgr::~RpcServiceMgr() {
}

RpcServiceMgr::ServiceData *RpcServiceMgr::get_new_service(std::string service_name)
{
    RpcServiceMap::iterator it = _services.find(service_name);
    if(it != _services.end()) {
        return &it->second;
    }

    printf("Register %s\n", service_name.c_str());
    _services.insert(make_pair<std::string, ServiceData>(service_name, ServiceData()));
    //_services.insert(make_pair(service_name, ServiceData()));

    return &_services[service_name];
}

RpcServiceMgr::ServiceData *RpcServiceMgr::GetService(std::string service_name)
{
    RpcServiceMap::iterator it = _services.find(service_name);
    if(it != _services.end()) {
        return &it->second;
    }
    
    return NULL;
}


bool RpcServiceMgr::AddMethod (std::string service_name, const MethodDescriptor *method_descriptor, 
	const Message *request_proto, const Message *response_proto, Service *rpc_service) {
        ServiceData *service_data = get_new_service(service_name);
		service_data->_method_descriptor = method_descriptor;
		service_data->_request_proto = request_proto;
		service_data->_response_proto = response_proto;
		service_data->_rpc_service = rpc_service;
		return true;
}

bool RpcServiceMgr::RegisterRpcService(Service *rpc_service) {
	const ServiceDescriptor *service_descriptor = rpc_service->GetDescriptor();
	for (int i = 0; i < service_descriptor->method_count(); i++) {
		const MethodDescriptor *method_descriptor = service_descriptor->method(i);	
		const Message *request_proto = &rpc_service->GetRequestPrototype(method_descriptor); 
		const Message *response_proto = &rpc_service->GetResponsePrototype(method_descriptor);
		if (!AddMethod(method_descriptor->full_name(), method_descriptor,request_proto, response_proto, rpc_service))
			return false;
	}
	return true;
}

void RpcServiceMgr::HandleDefaultRpcCall(RPC::RpcRequestData &rpc_data, std::string &ret_data)
{
    RPC::RpcResponseData response_data;
    std::string content;

    printf("unknown Request %s\n", rpc_data.service_name().c_str());
    fflush(stdout);

    response_data.set_err(1);
    response_data.set_call_id(rpc_data.call_id());
    response_data.set_content(""); 
    response_data.SerializeToString(&ret_data);
    return;
}

void RpcServiceMgr::HandleRpcCall(unsigned char *call_data, size_t length, std::string &ret_data, google::protobuf::RpcController *controller) {
	RPC::RpcRequestData rpc_data;
	rpc_data.ParseFromArray(call_data, length);
	controller->Reset();

	RpcServiceMgr::ServiceData *service_data = GetService(rpc_data.service_name());
    if(service_data == NULL) {
        return HandleDefaultRpcCall(rpc_data, ret_data);
    }
	Service *rpc_service = service_data->_rpc_service;

	Message *request = service_data->_request_proto->New();
	Message *response = service_data->_response_proto->New();
	request->ParseFromString(rpc_data.content());

	rpc_service->CallMethod(service_data->_method_descriptor, controller, request, response, NULL);

	RPC::RpcResponseData response_data;
	std::string content;
    response_data.set_err(0);
	response_data.set_call_id(rpc_data.call_id());
	response->SerializeToString(&content);
	response_data.set_content(content); 
	response_data.SerializeToString(&ret_data);

	delete request;
	delete response;
}
