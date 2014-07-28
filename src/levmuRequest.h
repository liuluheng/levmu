#ifndef _LEVMU_REQUEST_H_
#define _LEVMU_REQUEST_H_

#include <vector>
#include <map>
#include <string>

#include <stdint.h>

#include <muduo/net/Callbacks.h>

#include <boost/shared_ptr.hpp>


class levmuServer;
class levmuResponse;

class levmuRequest {
  friend class redisCodec;
 public:

  explicit levmuRequest(levmuServer *,
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

  levmuServer *server_;
  int32_t db_index;
  int32_t arg_count;
  std::string name;
  std::vector<std::string> args;

  typedef void (levmuRequest::*COMMAND)();
  static std::map<std::string, COMMAND> cmd_map;

  boost::shared_ptr<levmuResponse> response_;

  //None copyable
  levmuRequest(const levmuRequest&);
  void operator=(const levmuRequest&);
};


#endif
