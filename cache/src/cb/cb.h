#include <stddef.h>
#define MAXSIZE 2048

typedef struct {
	//Use size_t because it is used for count of bytes per man page
	size_t capacity; // Total capacity of the CB
	size_t head; // Read ptr
	size_t tail; // Write ptr
	size_t len; // How many spaces are filled
	char data[MAXSIZE];
} Circle_Buffer;

Circle_Buffer *cb_init();
int add_char(Circle_Buffer *cb, char data);
int pop_char(Circle_Buffer *cb, char *c);
