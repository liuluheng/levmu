# Now we have cmake, which is more elegant, so we actually doesn't need this shit :).

CXX = g++
CXXFLAGS = -Wall -g -O2 -DMUDUO_STD_STRING -I../../vendor/muduo -I../../vendor/leveldb/include
LDFLAGS = ../../vendor/leveldb/libleveldb.a -L../../vendor/build/release/lib -lmuduo_base -lmuduo_net -lpthread -lgmp

response_test : response_test.o ../../src/response.o status.o testharness.o
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o

realclean:
	rm -f *.o response_test
