# Now we have cmake, which is more elegant, so we actually doesn't need this shit :).

CXX = g++
CXXFLAGS = -Wall -g -O2 \
		   -I../vendor/muduo \
		   -I../vendor/leveldb/include\
		   -DMUDUO_STD_STRING 
LDFLAGS = ../vendor/leveldb/libleveldb.a -L../vendor/build/release/lib -lmuduo_base -lmuduo_net -lpthread -lgmp -lrt -lsnappy

levmu: main.o codec.o server.o request.o response.o
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm *.o levmu

.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@
