#ifndef _LEVMU_CODEC_H_
#define _LEVMU_CODEC_H_

#include <boost/noncopyable.hpp>

#include <muduo/net/Buffer.h>
#include <muduo/net/TcpConnection.h>


namespace levmu {
class Server;
class Request;

class redisCodec : boost::noncopyable {
  public:
   explicit 
   redisCodec(Server *server)
               : server_(server),
                 msg_len_(0) {}

   void onMessage(const muduo::net::TcpConnectionPtr& conn,
                  muduo::net::Buffer* buf,
                  muduo::Timestamp receiveTime); 

  private:
   size_t get_int(muduo::net::Buffer* buf, 
                  const char **next_idx);

   void do_request(boost::shared_ptr<Request>);

   Server  *server_;

   int msg_len_;
};

}

#endif  // _LEVMU_CODEC_H_
