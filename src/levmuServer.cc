#include "levmuServer.h"
#include "levmuRequest.h"

#include <muduo/base/Logging.h>

#include <leveldb/status.h>
#include <leveldb/filter_policy.h>

#include <boost/bind.hpp>

#include <stdio.h>
#include <unistd.h> //for _exit(1)

levmuServer::levmuServer(muduo::net::EventLoop* loop,
                       const muduo::net::InetAddress& listenAddr,
                       int32_t dbn)
  : server_(loop, listenAddr, "levmuServer"), 
    db_num(dbn),
    db_path("./levmu.db"),
    clients_num(0),
    codec_(this) {
    server_.setConnectionCallback(
            boost::bind(&levmuServer::onConnection, this, _1));
    server_.setMessageCallback(
            boost::bind(&redisCodec::onMessage, &codec_, _1, _2, _3));

    leveldb::Status status;
    if(db_num<1){
        options = new leveldb::Options[1];
        options[0].create_if_missing = true;
        options[0].filter_policy = leveldb::NewBloomFilterPolicy(10);

        db=new leveldb::DB*[1];
        status = leveldb::DB::Open(options[0], db_path.c_str(), &db[0]);
        if(!status.ok()) {
            puts("leveldb open error");
            //exit(); is NOT thread safe. P94
            //so we could use _exit(); but 
            //it would not destruct global objects,
            //flush stdout
            _exit(1);
        }
    }else{
        options = new leveldb::Options[db_num];

        db=new leveldb::DB*[db_num];
        char buf[16];
        for(int32_t i=0;i<db_num;i++){
            options[i].create_if_missing = true;
            options[i].filter_policy = leveldb::NewBloomFilterPolicy(16);

            int count = sprintf(buf, "/db-%03d", i);
            //TODO the db path
            status = leveldb::DB::Open(options[i], (db_path+std::string(buf,count)).c_str(), &db[i]);
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

void levmuServer::onConnection(const muduo::net::TcpConnectionPtr& conn) {
  LOG_INFO << "levmuServer - " << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");
}

void levmuServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
                           muduo::net::Buffer* buf,
                           muduo::Timestamp time) {
  //not used..
  std::string msg(buf->retrieveAllAsString());
  LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
           << "data received at " << time.toString();
}

