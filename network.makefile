C_PROGRAMS += test/network-server
SH_PROGRAMS += test/run-network-server
RUN_TESTS += test/run-network-server

network-tests: test/network-server
network-tests: test/run-network-server

test/network-server: src/network/test/tcp/server.test.o
test/network-server: src/network/network.o
test/network-server: src/convert/fd/sink.o
test/network-server: src/convert/fd/source.o
test/network-server: src/convert/source.o
test/network-server: src/convert/sink.o
test/network-server: src/log/log.o
test/network-server: src/window/alloc.o

test/run-network-server: src/network/test/tcp/run-server.test.sh

tests: network-tests
