// Use of this source code is governed by a MIT-style license
// that can be found in the License file.
//
// Author: Liu Lu Heng

#ifndef _LEVMU_RESPONSE_H_
#define _LEVMU_RESPONSE_H_

#include <muduo/net/Callbacks.h>

//#include <string>

namespace levmu {

class Response {
    const muduo::net::TcpConnectionPtr conn_;
    muduo::string write_buffer;

    public:
    explicit 
    Response(const muduo::net::TcpConnectionPtr& conn)
        : conn_(conn), write_buffer("") {}

    ~Response();

    void write_nil();
    void write_error(const char* msg);
    void write_status(const char* msg);
    void write_integer(const char *out, size_t out_size);
    void write_bulk(const char *out, size_t out_size);
    void write_bulk(const muduo::string &out);
    void write_mbulk_header(int n);
    private:
    //None copyable
    Response(const Response&);
    void operator=(const Response&);
};

}

#endif // _LEVMU_RESPONSE_H_
