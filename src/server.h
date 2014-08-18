// Use of this source code is governed by a MIT-style license
// that can be found in the License file.
//
// Author: Liu Lu Heng

#ifndef _LEVMU_SERVER_H_
#define _LEVMU_SERVER_H_ 

#include "codec.h" 
#include <muduo/net/TcpServer.h>

#include <leveldb/db.h>
#include <leveldb/options.h>

namespace levmu {

class Server {
 public:
  Server(muduo::net::EventLoop* loop,
         const muduo::net::InetAddress& listenAddr,
         int32_t dbn = 0);

  void setThreadNum(int thread_nm) {
      server_.setThreadNum(thread_nm);
  }

  void start() {
      server_.start();
  }

  //leveldb::Status db_put(const leveldb::Slice& key,
  //                       const leveldb::Slice& value) {
  //  DB->Put(key, value);
  //}

  leveldb::DB *get_db(int dbn = 0) {
      return db_[dbn];
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
  void db_init(void);

  muduo::net::TcpServer server_;

  int clients_num_;
  int32_t db_num_;
  std::string db_path_;

  leveldb::Options *options_;
  leveldb::DB **db_;
  leveldb::ReadOptions read_options_;
  leveldb::WriteOptions write_options_;

  redisCodec codec_;
};

}

#endif  // _LEVMU_SERVER_H_
