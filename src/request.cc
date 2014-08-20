// Use of this source code is governed by a MIT-style license
// that can be found in the License file.
//
// Author: Liu Lu Heng

#include "request.h"
#include "server.h"
#include "response.h"

#include <leveldb/status.h>
#include <leveldb/slice.h>

#include <muduo/base/Logging.h>

#include <gmp.h>

namespace levmu {

std::map<std::string, Request::COMMAND> Request::cmd_map_;
bool _ignored = Request::init_cmd_map();

bool Request::init_cmd_map() {
  cmd_map_["set"]              = &Request::levmu_set;
  cmd_map_["get"]              = &Request::levmu_get;
  cmd_map_["incr"]             = &Request::levmu_incr;
  cmd_map_["incrby"]           = &Request::levmu_incrby;

  return true;
}

Request::Request(Server *server, 
                 const muduo::net::TcpConnectionPtr& conn)
  :server_(server),
    db_index_(0),
    arg_count_(-1),
    name_(""),
    response_(new Response(conn)) {
    }

void Request::_run() {
  auto it = cmd_map_.find(name_);
  if (it != cmd_map_.end() ) {
    (this->*it->second)();
  } else {
    LOG_ERROR << " option " << name_.c_str()
        << " has not been supported :(";
  }
}

void Request::run() {
  _run();
}

void Request::levmu_set() {

  if (args_.size() != 2) {
    LOG_ERROR << "ERR wrong number of arguments for 'set' command";
    response_->write_error("ERR wrong number of arguments for 'set' command");
    return;
  }

  leveldb::Status status;
  status = server_->get_db()->Put(server_->write_options(),
                                  args_[0], args_[1]);

  if(!status.ok()) {
    LOG_ERROR << "SET ERROR 1";
    response_->write_error("SET ERROR 1");
  } else {
    //LOG_INFO << "OK";
    response_->write_status("OK");
  }
}

void Request::levmu_get() {

  if (args_.size() != 1) {
    response_->write_error("ERR wrong number of arguments for 'get' command");
    return;
  }

  std::string out;
  leveldb::Status status;
  status = server_->get_db()->Get(server_->read_options(),
                                  args_[0], &out);

  if (status.IsNotFound()) {
    response_->write_nil();
  } else if (status.ok()) {
    response_->write_bulk(out);
  } else {
    response_->write_error("GET ERROR 1");
  }
}

void Request::levmu_incr(){

  std::string out;
  leveldb::Status status;

  status = server_->get_db()->Get(server_->read_options(),
                                  args_[0], &out);

  char *str_oldv=NULL;
  if(status.IsNotFound()){
    str_oldv = strdup("0");
  }else if(status.ok()){
    str_oldv=static_cast<char*>(malloc(out.size()+1));
    memcpy(str_oldv,out.data(),out.size());
    str_oldv[out.size()]=0;
  }else{
    response_->write_error("INCR ERROR 1");
    return;
  }

  mpz_t old_v;
  mpz_init(old_v);
  mpz_set_str(old_v,str_oldv,10);
  free(str_oldv);
  mpz_add_ui(old_v,old_v,1);
  char *str_newv=mpz_get_str(NULL,10,old_v);
  mpz_clear(old_v);

  status = server_->get_db()->Put(server_->write_options(),
                                  args_[0], 
                                  leveldb::Slice(str_newv, strlen(str_newv)));

  if(!status.ok()) {
    response_->write_error("INCR ERROR 2");
  } else {
    response_->write_integer(str_newv, strlen(str_newv));
  }
  free(str_newv);
}

void Request::levmu_incrby(){

  std::string out;
  leveldb::Status status;
  status = server_->get_db()->Get(server_->read_options(),
                                  args_[0], &out);

  mpz_t delta;
  mpz_init(delta);
  mpz_set_str(delta,args_[1].c_str(),10);

  char *str_oldv=NULL;
  if(status.IsNotFound()){
    str_oldv = strdup("0");
  }else if(status.ok()){
    str_oldv=static_cast<char*>(malloc(out.size()+1));
    memcpy(str_oldv,out.data(),out.size());
    str_oldv[out.size()]=0;
  }else{
    response_->write_error("INCRBY ERROR 1");
    return;
  }

  mpz_t old_v;
  mpz_init(old_v);
  mpz_set_str(old_v,str_oldv,10);
  free(str_oldv);
  mpz_add(old_v,old_v,delta);
  char *str_newv=mpz_get_str(NULL,10,old_v);
  mpz_clear(delta);
  mpz_clear(old_v);

  status = server_->get_db()->Put(server_->write_options(),
                                  args_[0], 
                                  leveldb::Slice(str_newv, strlen(str_newv)));

  if(!status.ok()) {
    response_->write_error("INCRBY ERROR 2");
  }else{
    response_->write_integer(str_newv, strlen(str_newv));
  }
  free(str_newv);
}

}

