# target1 [target2 ...]: [pre-req-1 pre-req-2 ...]
# 	[command1
# 	 command2
# 	 ......]

all: clear format client_socket server_socket io

sockets: client_socket server_socket

client_socket:
	gcc -o build/client_socket src/client_socket.c src/common.c

server_socket:
	gcc -o build/server_socket src/server_socket.c src/common.c

io:
	gcc -o build/io src/io.c src/common.c

clear:
	rm -f build/client_socket
	rm -f build/server_socket
	rm -f build/io

format:
	clang-format -i -style=WebKit src/*.c
