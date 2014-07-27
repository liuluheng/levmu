# Now we have cmake, which is more elegant, so we actually doesn't need this shit :).

CXX = g++
CXXFLAGS = -Wall -g -O2 -DMUDUO_STD_STRING -I../vendor/muduo -I../vendor/leveldb/include
LDFLAGS = ../vendor/leveldb/libleveldb.a -L../vendor/build/release/lib -lmuduo_base -lmuduo_net -lpthread -lgmp

levmu : levmu_main.o codec.o levmuServer.o levmuRequest.o levmuResponse.o
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm *.o levmu
