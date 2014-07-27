#ifndef _LEVMU_SERVER_H_
#define _LEVMU_SERVER_H_ 

#include "codec.h"

#include <muduo/net/TcpServer.h>

#include <leveldb/db.h>
#include <leveldb/options.h>

class levmuRequest;

class levmuServer
{
 public:
  levmuServer(muduo::net::EventLoop* loop,
             const muduo::net::InetAddress& listenAddr,
             int32_t dbn = 0);

  void setThreadNum(int thread_nm) {
      server_.setThreadNum(thread_nm);
  }

  void start() {
      server_.start();
  }

  leveldb::DB * get_db(int dbn = 0) {
      return db[dbn];
  }
  const leveldb::ReadOptions& read_options() {
      return read_options_;
  }
  const leveldb::WriteOptions& write_options() {
      return write_options_;
  }

 private:
  void onConnection(const muduo::net::TcpConnectionPtr& conn);

  void onMessage(const muduo::net::TcpConnectionPtr& conn,
                 muduo::net::Buffer* buf,
                 muduo::Timestamp time);

  muduo::net::TcpServer server_;

  int32_t db_num;
  std::string db_path;
  int clients_num;

  leveldb::Options *options;
  leveldb::ReadOptions read_options_;
  leveldb::WriteOptions write_options_;
  leveldb::DB **db;

  redisCodec codec_;
};

#endif  // _LEVMU_SERVER_H_
