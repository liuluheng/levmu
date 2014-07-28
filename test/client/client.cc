#include "hiredis.h"

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
<<<<<<< HEAD
#include <muduo/base/Thread.h>
=======
>>>>>>> 624d7343b4bed17244c6aa1858a5f1cd2da23df4
#include <muduo/base/Condition.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpClient.h>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

<<<<<<< HEAD
namespace levmu {
class Client : boost::noncopyable {
 public:
  Client(EventLoop* loop, const InetAddress& serverAddr)
    : client_(loop, serverAddr, "Client"),
      mutex_(),
      cond_(mutex_) {
    client_.setConnectionCallback(
        boost::bind(&Client::onConnection, this, _1));
    client_.setMessageCallback(
        boost::bind(&Client::onMessage, this, _1, _2, _3));
    client_.enableRetry();
  }

  void connect() {
    client_.connect();
  }

  void disconnect() {
=======
class levmuClient : boost::noncopyable
{
 public:
  levmuClient(EventLoop* loop, const InetAddress& serverAddr)
    : client_(loop, serverAddr, "levmuClient"),
      mutex_(),
      cond_(mutex_) {
    client_.setConnectionCallback(
        boost::bind(&levmuClient::onConnection, this, _1));
    client_.setMessageCallback(
        boost::bind(&levmuClient::onMessage, this, _1, _2, _3));
    client_.enableRetry();
  }

  void connect()
  {
    client_.connect();
  }

  void disconnect()
  {
>>>>>>> 624d7343b4bed17244c6aa1858a5f1cd2da23df4
    // client_.disconnect();
  }

  void send(const char *msg) {
      MutexLockGuard lock(mutex_);
      while (!connection_) {
          cond_.wait();
      }
      connection_->send(msg, strlen(msg));
  }

 private:
  void onConnection(const TcpConnectionPtr& conn) {
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");

    MutexLockGuard lock(mutex_);
    if (conn->connected()) {
      connection_ = conn;
      cond_.notify();

    } else {
      connection_.reset();
    }
  }

  void onMessage(const TcpConnectionPtr&,
                 Buffer* buf,
                 Timestamp) {
    //printf("%s\n", buf->peek());
    //printf("%d\n", buf->readableBytes());

    while (buf->readableBytes()) {
        void *reply;
        redisReader* r = redisReaderCreate();
        redisReaderFeed(r, buf->peek(), buf->readableBytes());
        redisReaderGetReply(r, &reply);
        buf->retrieve(r->pos);
        if (*(redisReply **)(&reply)) {
            if ((*(redisReply **)(&reply))->str)
                printf("%s\n", (*(redisReply **)(&reply))->str);
            if ((*(redisReply **)(&reply))->integer)
                printf("%lld\n", (*(redisReply **)(&reply))->integer);
        }
        redisReaderFree(r);
        if (*(redisReply **)(&reply)) {
            freeReplyObject(reply);
        }
    }
  }

  TcpClient client_;
  MutexLock mutex_;
  Condition cond_;
  TcpConnectionPtr connection_;
};

<<<<<<< HEAD
}  //namespace levmu

void* thread_test(levmu::Client *client) 
{
    char *a;
    redisFormatCommand(&a, "SET key %s", std::string(1024, 'b').c_str());
=======
void* thread_test(void *arg) {
    levmuClient* client = static_cast<levmuClient*>(arg);
    char b[1025];
    memset(b, 'b', 1024);
    b[1024] = static_cast<char>(0);

    char *a;
    redisFormatCommand(&a, "SET key %s", b);
>>>>>>> 624d7343b4bed17244c6aa1858a5f1cd2da23df4
    client->send(a);
    redisFormatCommand(&a, "GET key");
    client->send(a);

    redisFormatCommand(&a, "SET incr_key %b", (size_t)0);
    client->send(a);
    redisFormatCommand(&a, "GET incr_key");
    client->send(a);
    redisFormatCommand(&a, "INCR incr_key");
    client->send(a);
    redisFormatCommand(&a, "GET incr_key");
    client->send(a);

<<<<<<< HEAD
=======

>>>>>>> 624d7343b4bed17244c6aa1858a5f1cd2da23df4
    return NULL;
}


int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  EventLoop loop;
  uint16_t port = static_cast<uint16_t>(atoi("8323"));
  InetAddress serverAddr("127.0.0.1", port);

<<<<<<< HEAD
  levmu::Client client(&loop, serverAddr);
  client.connect();

  Thread test(boost::bind(thread_test, &client), "test_thread");
  test.start();
  
=======
  levmuClient client(&loop, serverAddr);
  client.connect();

  //Thread test(thread_test);
  //test.start();
  //
  pthread_t pid;
  pthread_create(&pid, NULL, thread_test, static_cast<void*>(&client));

>>>>>>> 624d7343b4bed17244c6aa1858a5f1cd2da23df4
  loop.loop();
  client.disconnect();
}

