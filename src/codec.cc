// Use of this source code is governed by a MIT-style license
// that can be found in the License file.
//
// Author: Liu Lu Heng

#include "codec.h"
#include "request.h"

#include <muduo/base/Logging.h>

#include <boost/shared_ptr.hpp>
#include <stdio.h>

#define CHECK_BUFFER(N) do {\
  if ((N) > (buf->beginWrite() - next_idx)/* || (N) == 1*/) {\
    LOG_ERROR << "not a full msg";\
    return;\
  }\
} while (0)

namespace levmu {

void redisCodec::onMessage(const muduo::net::TcpConnectionPtr& conn,
                           muduo::net::Buffer* buf,
                           muduo::Timestamp) {
  boost::shared_ptr<Request> current_req;

  const char *next_idx = buf->peek();
  msg_len_ = 0;

  while (buf->readableBytes()) {
    if (!current_req.get()) {
      current_req.reset(new Request(server_, conn));
    }

    // 1. read the arg count:
    if (current_req->arg_count_ < 0) {
      CHECK_BUFFER(4);
      if (*next_idx++ != '*') {
        LOG_ERROR << "msg has no '*'";
        //conn->shutdown();  // FIXME: disable reading
        return;
      }
      current_req->arg_count_ = get_int(buf, &next_idx);
      current_req->arg_count_--;
    }

    // 2. read the request name
    if(current_req->arg_count_ >= 0 
       && current_req->name_.empty()) {
      CHECK_BUFFER(4);
      if(*next_idx++ != '$') {
        LOG_ERROR << "msg has no '$'";
        //conn->shutdown();  // FIXME: disable reading
        return;
      }
      int len = get_int(buf, &next_idx);
      CHECK_BUFFER(len + 2);
      current_req->name_ = muduo::string(next_idx, next_idx + len);
      std::transform(current_req->name_.begin(), 
                     current_req->name_.end(),
                     current_req->name_.begin(), 
                     ::tolower);
      next_idx += len + 2;
      msg_len_ += len + 2;
    }

    // 3. read a arg
    if(current_req->arg_count_ >= 0 
       && current_req->arg_count_ - current_req->args_.size() > 0) {
      CHECK_BUFFER(4);
      if(*next_idx++ != '$') {
        LOG_ERROR << "msg has no '$'";
        //conn->shutdown();  // FIXME: disable reading
        return;
      }
      int len = get_int(buf, &next_idx);
      CHECK_BUFFER(len + 2);
      current_req->args_.push_back(muduo::string(next_idx, 
                                                 next_idx + len));
      next_idx += len + 2;
      msg_len_ += len + 2;
    }

    // 4. do the request
    if (current_req->completed()) {
      assert(current_req.unique());
      buf->retrieve(msg_len_);
      msg_len_ = 0;
      do_request(current_req);
      assert(current_req.unique());
      current_req.reset();
    }
  }
}

size_t redisCodec::get_int(muduo::net::Buffer* buf,
                           const char **next_idx) {
  const char *b = *next_idx;
  size_t val = 0;
  while (*b != '\r') {
    val *= 10;
    val += (*b++ - '0');
  }

  if (b <= buf->beginWrite()) {
    b += 2;
    msg_len_ += b - *next_idx + 1;
    *next_idx = b;
    return val;
  }

  LOG_ERROR << "get_int::not a full msg";
  return -1;
}

void redisCodec::do_request(boost::shared_ptr<Request> current_req) {
  assert(current_req.use_count() == 2);
  if(current_req && current_req->completed()){
    current_req->run();
  }
}

}

