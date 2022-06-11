#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "simple_malloc.h"

void write_bytes(char* buf, size_t len, char ch) {
    for (size_t i = 0; i < len; ++i) {
        buf[i] = ch;
    }
}

int main() {

    srand(time(NULL));

    unsigned int count = 16000;
    unsigned int max_size = 15999;
    printf("performing %u random allocations of 1-%u bytes: (in groups of 5)\n", count * 5, max_size+1);
    for (size_t i = 0; i < count; ++i) {
        size_t size1 = (rand() % max_size) + 1;
        char* ptr1 = simple_malloc(size1);
        if (ptr1 == NULL) printf("warn: simple_malloc() returned NULL, failed to allocate %lu bytes of memory", size1);
        else write_bytes(ptr1, size1, (char)(rand() % 255));

        size_t size2 = (rand() % max_size) + 1;
        char* ptr2 = simple_malloc(size2);
        if (ptr2 == NULL) printf("warn: simple_malloc() returned NULL, failed to allocate %lu bytes of memory", size2);
        else write_bytes(ptr2, size2, (char)(rand() % 255));

        size_t size3 = (rand() % max_size) + 1;
        char* ptr3 = simple_malloc(size3);
        if (ptr3 == NULL) printf("warn: simple_malloc() returned NULL, failed to allocate %lu bytes of memory", size3);
        else write_bytes(ptr3, size3, (char)(rand() % 255));

        size_t size4 = (rand() % max_size) + 1;
        char* ptr4 = simple_malloc(size4);
        if (ptr4 == NULL) printf("warn: simple_malloc() returned NULL, failed to allocate %lu bytes of memory", size4);
        else write_bytes(ptr4, size4, (char)(rand() % 255));

        size_t size5 = (rand() % max_size) + 1;
        char* ptr5 = simple_malloc(size5);
        if (ptr5 == NULL) printf("warn: simple_malloc() returned NULL, failed to allocate %lu bytes of memory", size5);
        else write_bytes(ptr5, size5, (char)(rand() % 255));

        simple_free(ptr5);
        simple_free(ptr3);
        simple_free(ptr4);
        simple_free(ptr1);
        simple_free(ptr2);
    }

    printf("No fatal allocation errors\n");
    return EXIT_SUCCESS;

}
