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
    size_t alloc_size = count * sizeof(int);
    if (count > 0 && alloc_size / count != sizeof(int)) {
        return NULL;
    }

    Buffer *buf = malloc(sizeof(Buffer));
    if (!buf) return NULL;

    buf->data = malloc(alloc_size);
    if (!buf->data) {
        free(buf);
        return NULL;
    }

    buf->size = 0;
    buf->capacity = count;
    return buf;
}

void destroy_buffer(Buffer *buf) {
    if (buf) {
        free(buf->data);
        free(buf);
    }
}

void fill_buffer(Buffer *buf, int *src, size_t n) {
    if (!buf || !src) return;

    for (size_t i = 0; i <= n; ++i) {
        buf->data[i] = src[i];
        buf->size++;
    }
}

int access_element(Buffer *buf, int index) {
    return buf->data[index];
}

int main(int argc, char *argv[]) {
    printf("Lab1:Start...\n");

    if (argc < 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int count = atoi(argv[1]);

    //printf("Lab1:Step 1\n");
    int *temp = calloc(count + 1, sizeof(int));
    if (!temp) return 1;
    for (int i = 0; i <= count; ++i) {
        temp[i] = i * 2;
    }

    //printf("Lab1:Step 2\n");
    Buffer *buf = create_buffer(count);
    if (!buf) {
        printf("Failed to allocate buffer\n");
        free(temp);
        return 1;
    }

    //printf("Lab1:Step 3\n");
    fill_buffer(buf, temp, count);

    //printf("Lab1:Step 4\n");
    destroy_buffer(buf);

    //printf("Lab1:Step 5\n");
    printf("Accessing data: %d\n", buf->data[0]);

    //printf("Lab1:Step 6\n");
    printf("Accessing index: %d\n", access_element(buf, count + 5));

    free(temp);

    printf("Lab1:Stop...\n");
    return 0;
}
