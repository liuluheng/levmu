levmu
=====
this is a repository to learn leveldb and muduo.

It has implemented several redis protocol, like SET, GET, INCR, to store data in leveldb, using muduo for communication.

It can implement more, but the main purpose is not for real use.

ALSO the Implementation has been referred to redis-leveldb...

Compile
=====
the repository use the -DMUDUO_STD_STRING, so you need add this to muduo before you compile muduo...
