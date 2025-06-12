#include "userprog/memstat.h"
#include "threads/thread.h"
#include "threads/palloc.h"
#include "threads/synch.h"
#include "vm/page.h"
#include <stdio.h>
#include "lib/kernel/hash.h"
static size_t frame_alloc_count = 0;
static size_t swap_used_count = 0;
static struct hash *current_spt = NULL;

static struct lock memstat_lock;

void memstat_frame_alloc(void) {
    lock_acquire(&memstat_lock);
    frame_alloc_count++;
    lock_release(&memstat_lock);
}

void memstat_frame_free(void) {
    lock_acquire(&memstat_lock);
    if (frame_alloc_count > 0)
        frame_alloc_count--;
    lock_release(&memstat_lock);
}

void memstat_set_swap_used(size_t used) {
    lock_acquire(&memstat_lock);
    swap_used_count = used;
    lock_release(&memstat_lock);
}

void memstat_set_spt(struct hash *spt) {
    lock_acquire(&memstat_lock);
    current_spt = spt;
    lock_release(&memstat_lock);
}

void print_memory_usage(void) {
    lock_acquire(&memstat_lock);
    size_t spt_size = current_spt ? hash_size(current_spt) : 0;

    printf("==== Memory Usage Statistics ====\n");
    printf("Allocated frames:    %zu\n", frame_alloc_count);
    printf("Used swap slots:     %zu\n", swap_used_count);
    printf("SPT entries:         %zu\n", spt_size);
    printf("=================================\n");

    lock_release(&memstat_lock);
}

__attribute__((constructor))
static void memstat_init(void) {
    lock_init(&memstat_lock);
}
