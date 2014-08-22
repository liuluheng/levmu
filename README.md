# levmu
=====
This repository was created for learning leveldb and muduo.

It has implemented several redis protocol, like SET, GET, INCR, to store data in leveldb, using muduo for communication.It could implemented more, but the main purpose is not for real use.

The implementation has referred to redis-leveldb...

## Acquire && Compile && run

    git clone git@github.com:liuluheng/levmu.git
    cd levmu
    git submodule init
    git submodule update
    cd vendor/muduo
    ./build.sh
    cd ../leveldb
    make -j2
    cd ../../
    ./build.sh
    ./levmu

## Progress
  - Compiles  :white_check_mark:
  - Builds    :white_check_mark:
  - Run       :white_check_mark:
