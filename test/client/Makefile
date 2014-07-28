# Now we have cmake, which is more elegant, so we actually doesn't need this shit :).

CC = gcc
CXX = g++
CFLAGS = -Wall -g -O2
CXXFLAGS = -Wall -g -O2 -DMUDUO_STD_STRING -I../../vendor/muduo -I../../vendor/leveldb/include
LDFLAGS = ../../vendor/leveldb/libleveldb.a -L../../vendor/build/release/lib -lmuduo_base -lmuduo_net -lpthread -lgmp

client: client.o hiredis.o sds.o
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm *.o

.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
