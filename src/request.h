// Use of this source code is governed by a MIT-style license
// that can be found in the License file.
//
// Author: Liu Lu Heng

#ifndef _LEVMU_REQUEST_H_
#define _LEVMU_REQUEST_H_

#include <vector>
#include <map>
//#include <string>

#include <stdint.h>

#include <muduo/base/Types.h>
#include <muduo/net/Callbacks.h>

#include <boost/shared_ptr.hpp>

namespace levmu {

class Server;
class Response;

class Request {
  friend class redisCodec;
 public:

  explicit Request(Server *server,
                   const muduo::net::TcpConnectionPtr& conn); 

  bool completed() { 
      return arg_count_ >= 0 && arg_count_ - args_.size() == 0;
  }
  void run();

  static bool init_cmd_map();
 private:
  void _run();
  void levmu_set();
  void levmu_get();
  void levmu_incr();
  void levmu_incrby();

  Server *server_;
  int32_t db_index_;
  int32_t arg_count_;
  muduo::string name_;
  std::vector<muduo::string> args_;

  typedef void (Request::*COMMAND)();
  static std::map<muduo::string, COMMAND> cmd_map_;

  boost::shared_ptr<Response> response_;

  //None copyable
  Request(const Request&);
  void operator=(const Request&);
};

}

#endif
