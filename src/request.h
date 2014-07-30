#ifndef _LEVMU_REQUEST_H_
#define _LEVMU_REQUEST_H_

#include <vector>
#include <map>
#include <string>

#include <stdint.h>

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
      return arg_count>=0 && arg_count-args.size()==0;
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
  int32_t db_index;
  int32_t arg_count;
  std::string name;
  std::vector<std::string> args;

  typedef void (Request::*COMMAND)();
  static std::map<std::string, COMMAND> cmd_map;

  boost::shared_ptr<Response> response_;

  //None copyable
  Request(const Request&);
  void operator=(const Request&);
};

}

#endif
