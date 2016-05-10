
all: dvr

clean:
	@rm -rf dvr

dvr: dvr.c dvr.h node0.c node1.c node2.c node3.c
	clang -g -o dvr dvr.c node0.c node1.c node2.c node3.c
