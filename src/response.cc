// Use of this source code is governed by a MIT-style license
// that can be found in the License file.
//
// Author: Liu Lu Heng

#include "response.h"

#include <stdio.h>

#include <muduo/base/StringPiece.h>
#include <muduo/net/TcpConnection.h>

namespace levmu {

Response::~Response() {
  conn_->send(muduo::StringPiece(write_buffer_));
}

void Response::write_nil() {
  write_buffer_ += "$-1\r\n";
}

void Response::write_error(const char* msg) {
  write_buffer_ += "-";
  write_buffer_ += muduo::string(msg, strlen(msg));
  write_buffer_ += "\r\n";
}

void Response::write_status(const char* msg) {
  write_buffer_ += "+";
  write_buffer_ += muduo::string(msg, strlen(msg));
  write_buffer_ += "\r\n";
}

void Response::write_integer(const char *out, size_t out_size) {
  write_buffer_ += ":";
  write_buffer_ += muduo::string(out, out_size);
  write_buffer_ += "\r\n";
}

void Response::write_bulk(const char *out, size_t out_size) {
  char buf[32];
  int count = sprintf(buf, "%zu", out_size);
  write_buffer_ += "$";
  write_buffer_ += muduo::string(buf, count);
  write_buffer_ += "\r\n";
  write_buffer_ += muduo::string(out, out_size);
  write_buffer_ += "\r\n";
}

void Response::write_bulk(const muduo::string &out) {
  write_bulk(out.c_str(), out.size());
}

void Response::write_mbulk_header(int n) {
  char buf[32];
  int count = sprintf(buf, "%d", n);
  write_buffer_ += "*";
  write_buffer_ += muduo::string(buf, count);
  write_buffer_ += "\r\n";
}

}
