INCLUDES=-I../
LDFLAGS=-lpthread -lprotobuf -levent -L/usr/local/lib
CFLAGS=-g

COMMON = common.h rpc_controller.h svc_name2id.h

all: protoc_middleman server

%.o:%.cc
	c++ $(INCLUDES) $(CFLAGS) -o $@ -c $<


server: svr.o helloworld.pb.o
	c++ $(INCLUDES) -o server svr.o helloworld.pb.o  -L../ -lrpcsvr $(LDFLAGS)
protoc_middleman: helloworld.proto
	protoc --cpp_out=. helloworld.proto

clean:
	-rm *.o server
