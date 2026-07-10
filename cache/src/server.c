#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "./cb/cb.h"

int main() {
	
	int status, sfd, newfd, sent_bytes;
	int yes = 1;
	struct addrinfo hints, *res, *r;
	socklen_t sin_size;
	struct sockaddr_storage their_addr;
	char rec_buf[1024];
	char *str;	
	Circle_Buffer *cb = cb_init();


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = AI_PASSIVE; // Fill in IP address
	
	// call getaddrinfo to fill out the *res struct
	if ((status = getaddrinfo("0.0.0.0", "5500", &hints, &res) != 0)) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	};
	
	// iter over the res strcut to try and bind to the port in res.
	// Call socket() to get a socket file descriptor, check for error
	// Call bind to try and bind to a port
	for (r = res; r != NULL; r = r->ai_next) {
		// Get socket fd
		if ((sfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol)) == -1) {
			perror("Server: socket");
	  		continue;
		}
		
		// Able to use the same port that was in use prior
		if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockpot");
			exit(1);
		}

		// bind the sfd. If fails (returns -1) close the socket fd and continue
		if (bind(sfd, r->ai_addr, r->ai_addrlen) == -1) {
			close(sfd);
			perror("Server: bind");
			continue;
		}
		break;
	}

	freeaddrinfo(res); // Free the mem of res
	
	// If we iter over all of r then it did not bind.
	if (r = NULL) {
		fprintf(stderr, "Could not bind\n");
		return 1;
	}
	
	if (listen(sfd, 10) == -1) {
		perror("Listen");
		exit(1);
	}

	// TODO: Write the signal handler for reaping zombies. only if using fork

	printf("server: waiting for connections...\n");

	while(1) {
		sin_size = sizeof(their_addr);
		newfd = accept(sfd, (struct sockaddr*)&their_addr, &sin_size);
		if (newfd == -1) {
			perror("server: accept");
			continue;
		}

		int rec_bytes = recv(newfd, rec_buf, sizeof(rec_buf), 0);
		rec_buf[rec_bytes] = '\0';

		// char c;
		// Fill the working buffer
		for (int k = 0; k < rec_bytes; k++) {
			// TODO: Error handle for full buffer
			add_char(cb, rec_buf[k]);
			// if(pop_char(cb, &c) == 0) {
			// 	printf("Popped %c\n", c);
			// } else {
			// 	printf("Failed to pop Char");
			// };
		}
		
		printf("Server Received: %s\n", rec_buf);
		printf("Received Bytes: %d\n", rec_bytes);
		fflush(stdout);
		if(int e = (strcmp(rec_buf, "quit")) == 0) {
			exit(1);
		}
	}
} 
