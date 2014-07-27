# OPT ?= -O2 -DNDEBUG       # (A) Production use (optimized mode)
 OPT ?= -g2              # (B) Debug mode, w/ full line-level debugging symbols
# OPT ?= -O2 -g2 -DNDEBUG # (C) Profiling mode: opt, but w/debugging symbols
#-----------------------------------------------

$(shell CC="$(CC)" CXX="$(CXX)" TARGET_OS="$(TARGET_OS)" \
		./build_detect_platform build_config.mk ./)
include build_config.mk

CFLAGS += -I. -I../include $(PLATFORM_CCFLAGS) $(OPT)
CXXFLAGS += -I. -I../include $(PLATFORM_CXXFLAGS) $(OPT)

LDFLAGS += $(PLATFORM_LDFLAGS)
LIBS += $(PLATFORM_LIBS)

LIBOBJECTS= $(SOURCES:.cc=.o)

TESTUTIL = ./testutil.o
TESTHARNESS = ./testharness.o $(TESTUTIL) 

TESTS = \
	coding_test \
	arena_test \
	cache_test \
	skiplist_test \
	log_test \
	dbformat_test \
	table_test

all: $(TESTS) 

coding_test: ./coding_test.o $(LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

cache_test: cache_test.o $(LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

skiplist_test: ./skiplist_test.o $(LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

arena_test: arena_test.o $(LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS) 

dbformat_test: dbformat_test.o $(LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

log_test: ./log_test.o $(LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

table_test: ./table_test.o $(LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

version_edit_test: ./version_edit_test.o $(LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

version_set_test: ./version_set_test.o $(LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

db_test: ./db_test.o $(LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

clean:
	rm *.o $(TESTS) build_config.mk

tags:
	ctags -R --languages=c++ --langmap=c++:+.inl -h +.inl --c++-kinds=+px --fields=+aiKSz --extra=+q --exclude=lex.yy.cc --exclude=copy_lex.yy.cc

