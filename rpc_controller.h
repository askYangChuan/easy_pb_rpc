#include <google/protobuf/service.h>
#include <string>

namespace PBRPC {
	using std::string;
	class RpcController : public google::protobuf::RpcController {
		string _error_str;
        int _errno;
		public:
		RpcController() { Reset(); }
		void Reset() {
			_error_str = "";
			_errno = 0;
		}
		bool Failed() const {
			return _errno != 0;
		}
		string ErrorText() const {
			return _error_str;
		}
		void StartCancel() { // NOT IMPL
			return ;
		}
		void SetFailed(const string &reason) {
			_error_str = reason;
            _errno = 1;
		}
		bool IsCanceled() const { // NOT IMPL
			return false;
		}
  		
		void NotifyOnCancel(google::protobuf::Closure* callback) { // NOT IMPL
			return;
		}
	};
}
