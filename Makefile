INCLUDE = -I/opt/software/mysql-connector-c-commercial-6.1.3-linux-glibc2.5-x86_64/include \
		  -I/opt/software/openssl-1.0.1e/include \
          -I/opt/software/boost_1_59_0 \
          -I../

#$(info ${PATH})
#$(info $(ConfigName))

ifeq ($(ConfigName),Debug)
CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 $(INCLUDE)
else
CXXFLAGS =	-O2 -Wall -fmessage-length=0 $(INCLUDE)
endif

LIBS = -lpthread \
	   -lmysqlclient_r \
	   -lrt \
	   -lboost_random \
	   -lboost_regex \
       -lboost_chrono \
	   -lboost_timer \
	   -lboost_system \
	   -lboost_thread \
	   -lboost_context \
	   -lboost_coroutine \
       -lboost_unit_test_framework
	  

LIBPATH = -L/opt/software/boost_1_59_0/stage/lib \
		  -L/opt/mysql-connector-c-commercial-6.1.3-linux-glibc2.5-x86_64/lib

OBJS = 	main.o \
		boost_random.o \
		boost_function.o \
		boost_algorithms.o \
		boost_minmax.o \
		boost_range_adaptor.o \
		boost_range_mutating.o \
		boost_range_nonmutating.o \
		boost_range_new.o \
		boost_string_algo.o \
		boost_container.o \
		boost_pointer_container.o \
		boost_property_tree.o \
		boost_multiindex.o \
		boost_asio_timer.o \
		boost_asio_signal_set.o \
		boost_atomic.o \
		boost_thread.o \
		boost_signals2.o \
		boost_coroutine.o \
		stl_standard.o
		
TARGET = test

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(LIBPATH);

all: $(TARGET)

debug: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)



