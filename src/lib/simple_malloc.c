/*
 * MIT License
 *
 * Copyright 2022 Jenna Fligor
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>

#include <unistd.h>
#include <sys/mman.h>

typedef struct {
    size_t size;
    void* prev;
    void* next;
} FreeNode;

#define MIN_SIZE sizeof(FreeNode)

atomic_flag g_locked = ATOMIC_FLAG_INIT;

size_t g_pagesize = 0;

FreeNode* g_list_front = NULL;
FreeNode* g_list_back  = NULL;

void lock() {
    while (atomic_flag_test_and_set(&g_locked));
}

void unlock() {
    atomic_flag_clear(&g_locked);
}

void append_node(FreeNode* node) {
    if (g_list_front == NULL && g_list_back == NULL) {
        node->next = NULL;
        node->prev = NULL;
        g_list_front = node;
        g_list_back = node;
        return;
    }

    node->prev = g_list_back;
    g_list_back->next = node;

    node->next = NULL;
    g_list_back = node;
}

void unlink_node(FreeNode* node) {
    if (node == NULL) return;

    FreeNode* prev_node = node->prev;
    FreeNode* next_node = node->next;

    if (next_node == NULL && prev_node == NULL) {
        g_list_front = NULL;
        g_list_back = NULL;
    } else if (prev_node == NULL) {
        next_node->prev = NULL;
        g_list_front = next_node;
    } else if (next_node == NULL) {
        prev_node->next = NULL;
        g_list_back = prev_node;
    } else {
        prev_node->next = next_node;
        next_node->prev = prev_node;
    }
}

int new_pages(size_t size) {
    if (g_pagesize == 0) g_pagesize = sysconf(_SC_PAGESIZE);

    size_t size_page_aligned = ((size + (g_pagesize - 1)) / g_pagesize) * g_pagesize;

    void* pages = mmap(NULL, size_page_aligned, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (pages == MAP_FAILED) return 1;

    ((FreeNode*)pages)->size = size_page_aligned;
    append_node(pages);

    return 0;
}

void* simple_malloc(size_t n) {
    if (n == 0) return NULL;

    size_t size = n;
    if (size < MIN_SIZE) size = MIN_SIZE;
    else size = ((size + (8 - 1)) / 8) * 8;
    size += sizeof(size_t);

    lock(); // start of access to the freelist
    if (g_list_front == NULL && g_list_back == NULL) {
        if (new_pages(size) != 0) return NULL;
    }

    FreeNode* node = g_list_front;
    for (;;) {
        if (node->size >= size) break;
        if (node->next != NULL) {
            node = node->next;
        } else {
            if (new_pages(size) != 0) return NULL;
            node = node->next;
        }
    }

    if (node->size >= size + MIN_SIZE) {
        FreeNode* new_node = (void*)((size_t)node+size);
        new_node->size = node->size - size;
        node->size = size;
        append_node(new_node);
    }

    unlink_node(node);
    unlock(); // end of access to freelist

    return (void*)((size_t)node+sizeof(size_t));
}

void simple_free(void* ptr) {
    if (ptr == NULL) return;

    FreeNode* node = (FreeNode*)((size_t)ptr-sizeof(size_t));
    node->prev = NULL;
    node->next = NULL;

    lock(); // start of access to freelist
    if (g_list_front == NULL && g_list_back == NULL) {
        g_list_front = node;
        g_list_back = node;
        unlock();
        return;
    }

    bool joined_left = false;
    bool joined_right = false;

    size_t node_end = ((size_t)node)+(node->size);
    FreeNode* list_node = g_list_front;
    for (;;) {
        if (!joined_left && ((size_t)list_node)+(list_node->size) == (size_t)node) {
            list_node->size += node->size;
            node = list_node;
            joined_left = true;
            if (joined_left && joined_right) break;
        }
        if (!joined_right && node_end == (size_t)list_node) {
            node->size += list_node->size;
            unlink_node(list_node);
            joined_right = true;
            if (joined_left && joined_right) break;
        }
        if (list_node->next == NULL) {
            break;
        }
        list_node = list_node->next;
    }

    if (!joined_left) append_node(node);
    unlock(); // end of access to freelist
    return;
}
