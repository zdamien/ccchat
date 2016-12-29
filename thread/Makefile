ccchat: ccchat_main.cc exc_socket.cc exc_socket.h
	g++ exc_socket.cc ccchat_main.cc -o ccchat

tSQ: syn_queue.h tSQ.cc
	g++ -l pthread tSQ.cc -o tSQ  

clean:
	rm a.out tSQ ccchat ec es *.o

echo: exc_socket.h exc_socket.cc echo_server.cc echo_client.cc unp_brief.h readline.cc
	g++ exc_socket.cc readline.cc echo_client.cc -o ec
	g++ exc_socket.cc echo_server.cc -o es
