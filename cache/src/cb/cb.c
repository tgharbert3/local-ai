#include "cb.h"
#include <stddef.h>
#include <stdio.h>

Circle_Buffer cb;

Circle_Buffer *cb_init() 
{
	cb.capacity = MAXSIZE;
	cb.len = 0;
	cb.head = 0;
	cb.tail = 0;
	return &cb;
}

int add_char(Circle_Buffer *buf, char c)
{
	if (buf->len == buf->capacity) {
		printf("CB: Buffer is full");
		return -1;
	}

	buf->data[buf->tail] = c;
	buf->tail++;

	buf->len = buf->len + 1;

	if (buf->capacity == buf->tail) {
		buf->tail = 0;
	}	
	return 0;
}

int pop_char(Circle_Buffer *buf, char *c)
{
	if (buf->len == 0) {
		printf("CB: Buffer is empty");
		return -1;
	}

	*c = buf->data[buf->head];
	buf->head++;
	
	buf->len = buf->len - 1;

	if (buf->capacity == buf->head) {
		buf->head = 0;
	}

	return 0;
}

