INCLUDES=-I../
LDFLAGS=-lpthread -lprotobuf -levent -L/usr/local/lib
CFLAGS=-g

COMMON = common.h rpc_controller.h

all: protoc_middleman client

%.o:%.cc
	c++ $(INCLUDES) $(CFLAGS) -o $@ -c $<

client: clt.o helloworld.pb.o
	c++ $(INCLUDES) -o client clt.o helloworld.pb.o -L../ -lrpcclt $(LDFLAGS) 

protoc_middleman: helloworld.proto
	protoc --cpp_out=. helloworld.proto

clean:
	-rm *.o client
