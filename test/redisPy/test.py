#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
import redis
import time
import threading
from threadpool import ThreadPool


def functional_test():
    r = redis.StrictRedis(host='localhost', port=8323)
    # set/get
    print r.set("key", 'b'*56000)
    print len(r.get("key"))

    # incr
    print r.set("incr_key",0)
    print r.get("incr_key")
    print r.incr('incr_key')
    print r.get("incr_key")

def press_test():
    r = redis.StrictRedis(host='localhost', port=8323)
    for i in range(10000):
        key = 'foo_%d'%i
        r.set(key, 'b'*i)
        if i%1000==0:
            print key, "->", len(r.get(key))

if __name__=="__main__":
    #functional_test()
    # Create thread pool with nums threads
    pool = ThreadPool(32)
    # Add a task into pool
    for n in range(10):
        pool.add_task(functional_test)
        pool.add_task(press_test)
    # Join and destroy all threads
    pool.destroy()
