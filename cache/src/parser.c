#include "./cb/cb.h"
#include <stdio.h>
#include <string.h>

int parse_incoming_request(Circle_Buffer *cb)
{

	char *ptr = strstr(cb->data, "\r\n\r\n");
	printf("before: %c\n", *(ptr-1));
	if (ptr == NULL) {
		printf("No delimiter found\n");
		return -1;
	} else {
		printf("Found the header\n");
		return 0;
	}

};

int main(){
	Circle_Buffer *cb = cb_init();
	
	add_char(cb, 't');
	add_char(cb, '\r');
	add_char(cb, '\n');
	add_char(cb, '\r');
	add_char(cb, '\n');
	add_char(cb, '\0');
	int rc = parse_incoming_request(cb);
	printf("RC: %d", rc);
};
