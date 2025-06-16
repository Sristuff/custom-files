/*
 * gcc -Wall -Wextra -g -fsanitize=address,undefined -fstack-protector-strong -D_FORTIFY_SOURCE=2 -O1 -o lab1_soln lab1_soln.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer *create_buffer(size_t count) {
    if (count == 0 || count > SIZE_MAX / sizeof(int)) {
        return NULL;
    }
    Buffer *buf = malloc(sizeof(Buffer));
    if (!buf) return NULL;
    buf->data = calloc(count, sizeof(int));
    if (!buf->data) {
        free(buf);
        return NULL;
    }
    buf->size = 0;
    buf->capacity = count;
    return buf;
}

void destroy_buffer(Buffer **buf_ref) {
    if (buf_ref && *buf_ref) {
        free((*buf_ref)->data);
        (*buf_ref)->data = NULL;
        free(*buf_ref);
        *buf_ref = NULL;
    }
}

int fill_buffer(Buffer *buf, int *src, size_t n) {
    if (!buf || !buf->data || !src) return -1;
    if (n > buf->capacity) return -2;
    for (size_t i = 0; i < n; ++i) {
        buf->data[i] = src[i];
        buf->size++;
    }
    return 0;
}

int access_element(Buffer *buf, int index, int *value) {
    if (!buf || !value) return -1;
    if (index < 0 || (size_t)index >= buf->capacity) return -2;
    *value = buf->data[index];
    return 0;
}

int main(int argc, char *argv[]) {
    printf("Lab1:Solution:Start...\n");
    if (argc < 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }
    int count = atoi(argv[1]);
    if (count <= 0 || count > 1000) {
        printf("Invalid count. Choose a number between 1 and 1000.\n");
        return 1;
    }

    int *temp = calloc(count, sizeof(int));
    if (!temp) return 1;
    for (int i = 0; i < count; ++i) temp[i] = i * 2;

    Buffer *buf = create_buffer(count);
    if (!buf) {
        printf("Failed to allocate buffer\n");
        free(temp);
        return 1;
    }

    if (fill_buffer(buf, temp, count) != 0) {
        printf("Failed to fill buffer safely.\n");
        destroy_buffer(&buf);
        free(temp);
        return 1;
    }

    int value;
    if (access_element(buf, count / 2, &value) == 0) {
        printf("Accessed element safely: %d\n", value);
    } else {
        printf("Failed to access element safely.\n");
    }

    destroy_buffer(&buf);
    free(temp);
    printf("Lab1:Solution:End....\n");
    return 0;
}
