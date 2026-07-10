#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct addrinfo setup_socket();
int get_sfd(struct addrinfo res);
int connect_to_server(int sfd, struct addrinfo res);
int get_user_input(char *input_buf, size_t len);
int send_msg(int sfd, char *message);

int main()
{
	int status, sfd, s, conn, numbytes;
	struct addrinfo hints, *res, *r;
	char buf[1024];
	
	hints = setup_socket();
	if ((s = getaddrinfo("192.168.68.124","5500", &hints, &res) != 0)) {
		perror("client: getaddrinfo");
	};
	for (r = res; r != NULL; r = r->ai_next) {
		sfd = get_sfd(*res);
		conn = connect_to_server(sfd, *res);
		break;	
	}

	if (r == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}
	
	// TODO: This needs parsing and error handling
	get_user_input(buf, sizeof(buf));
	freeaddrinfo(res);
	send_msg(sfd, buf); 
	close(sfd);
	return 0;
}

struct addrinfo setup_socket()
{
	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	return hints;
}
//socket
int get_sfd(struct addrinfo res) {
	int sfd; 

	if ((sfd = socket(res.ai_family, res.ai_socktype, res.ai_protocol)) == -1) {
		perror("client: socket");
	};
	return sfd;
}
int connect_to_server(int sfd, struct addrinfo res) 
{
	int rc;

	if((rc = connect(sfd, res.ai_addr, res.ai_addrlen)) != 0) {
		perror("Client: connect");
		close(sfd);
		return -1;
	};
	return 0;
}

int get_user_input(char *input_buf, size_t len) {
	printf("What would you like to send to the server?\n");
	if (fgets(input_buf, len, stdin) != NULL) {
		input_buf[strcspn(input_buf, "\n")] = '\0';
	}

	printf("You entered: %s\n", input_buf);
	return 1;	
}

int send_msg(int sfd, char *message) 
{
	int bytes_sent;
	bytes_sent = send(sfd, message, strlen(message), 0);

	printf("sent bytes: %d\n", bytes_sent);
}

