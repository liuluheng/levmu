// Use of this source code is governed by a MIT-style license
// that can be found in the License file.
//
// Author: Liu Lu Heng

#include "server.h"
#include "request.h"

#include <muduo/base/Logging.h>

#include <leveldb/status.h>
#include <leveldb/filter_policy.h>

#include <boost/bind.hpp>

#include <stdio.h>
#include <unistd.h> //for _exit(1);

namespace levmu {

Server::Server(muduo::net::EventLoop* loop,
               const muduo::net::InetAddress& listenAddr,
               int32_t dbn)
    : server_(loop, listenAddr, "Server"), 
    clients_num_(0),
    db_num_(dbn),
    db_path_("./levmu.db"),
    codec_(this) {
      server_.setConnectionCallback(
          boost::bind(&Server::onConnection, this, _1));
      server_.setMessageCallback(
          boost::bind(&redisCodec::onMessage, &codec_, _1, _2, _3));
      db_init();
    }

void Server::db_init(void)
{
  leveldb::Status status;
  if (db_num_ < 1) {
    options_ = new leveldb::Options[1];
    options_[0].create_if_missing = true;
    options_[0].filter_policy = leveldb::NewBloomFilterPolicy(10);

    db_ = new leveldb::DB*[1];
    status = leveldb::DB::Open(options_[0], db_path_.c_str(), &db_[0]);
    if(!status.ok()) {
      puts("leveldb open error");
      //exit(); is NOT thread safe. P94
      //so we could use _exit(); but 
      //it would not destruct global objects,
      //flush stdout
      _exit(1);
    }
  } else {
    options_ = new leveldb::Options[db_num_];

    db_ = new leveldb::DB*[db_num_];
    char buf[16];
    for(int32_t i=0;i<db_num_;i++){
      options_[i].create_if_missing = true;
      options_[i].filter_policy = leveldb::NewBloomFilterPolicy(16);

      int count = sprintf(buf, "/db-%03d", i);
      //TODO the db path
      status = leveldb::DB::Open(options_[i], 
                                 (db_path_ +
                                  std::string(buf,count)).c_str(),
                                 &db_[i]);
      if(!status.ok()) {
        puts("leveldb open error");
        //exit(); is NOT thread safe. P94
        //so we could use _exit(); but 
        //it would not destruct global objects,
        //flush stdout
        puts(buf);
        _exit(1);
      }
    }
  }
}

void Server::onConnection(const muduo::net::TcpConnectionPtr& conn) {
  LOG_INFO << "Server - " << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");
}

//not used..
void Server::onMessage(const muduo::net::TcpConnectionPtr& conn,
                       muduo::net::Buffer* buf,
                       muduo::Timestamp time) {
  std::string msg(buf->retrieveAllAsString());
  LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
      << "data received at " << time.toString();
}

}

