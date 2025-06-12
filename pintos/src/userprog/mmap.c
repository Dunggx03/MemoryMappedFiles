#include "userprog/mmap.h"
#include "threads/malloc.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "threads/palloc.h"
#include "lib/kernel/list.h"
#include "threads/vaddr.h"
#include "userprog/syscall.h"
#include "filesys/file.h"
#include "userprog/process.h"
#include "vm/page.h"

void mmap_init () {
    list_init (&thread_current()->mmap_list);
}

mapid_t mmap(int fd, void *addr) {
  if (addr == NULL || pg_ofs(addr) != 0)
    return -1;

  struct file *file = process_get_file(fd);
  if (file == NULL)
    return -1;

  off_t length = file_length(file);
  if (length == 0)
    return -1;

  // reopen file to avoid file descriptor sharing issues
  struct file *reopened_file = file_reopen(file);
  if (reopened_file == NULL)
    return -1;

  size_t read_bytes = length;
  size_t zero_bytes =  PGSIZE - length;

  void *upage = addr;
  off_t ofs = 0;

  struct thread *cur = thread_current();
//   struct hash *spt = &cur->suppl_page_table;
  // check for overlap before inserting
for (void *page = upage; page < upage + read_bytes + zero_bytes; page += PGSIZE) {
    if (pagedir_get_page(cur->pagedir, page) != NULL) {
      file_close(reopened_file);
      return -1;
    }
  }

  // mmap id
  mapid_t mapid = cur->next_mapid++;
  
  // store mmap mapping info
  struct mmap_entry *me = malloc(sizeof(struct mmap_entry));
  if (me == NULL) {
    file_close(reopened_file);
    return -1;
  }

  me->mapid = mapid;
  me->file = reopened_file;
  me->start_addr = addr;
  me->length = length;
  list_push_back(&cur->mmap_list, &me->elem);

  // insert mmap supplemental pages
  while (read_bytes > 0 || zero_bytes > 0) {
    size_t page_read_bytes = read_bytes < PGSIZE ? read_bytes : PGSIZE;
    size_t page_zero_bytes = PGSIZE - page_read_bytes;

    if (!suppl_pt_insert_mmf(reopened_file, ofs, upage, page_read_bytes)) {
      munmap(mapid); // rollback on error
      return -1;
    }

    read_bytes -= page_read_bytes;
    zero_bytes -= page_zero_bytes;
    ofs += page_read_bytes;
    upage += PGSIZE;
  }

  return mapid;
}

void munmap (int mapid) {
    struct list *mmap_list = &thread_current()->mmap_list;
    struct list_elem *e = list_begin(mmap_list);
    while (e != list_end(mmap_list)) {
        struct mmap_entry *mme = list_entry(e, struct mmap_entry, elem);
        if (mme->mapid == mapid) {
            void *upage = mme->start_addr;
            size_t remaining = mme->length;

            while (remaining > 0) {
                struct suppl_pte *spte = get_suppl_pte(thread_current()->suppl_page_table, upage);
                if (spte && spte->is_loaded && pagedir_is_dirty(thread_current()->pagedir, upage)) {
                    write_page_back_to_file_wo_lock(spte);
                }
                if (spte) {
                    free_suppl_pte(&spte->elem, upage);
                }
                
                pagedir_clear_page(thread_current()->pagedir, upage);

                upage += PGSIZE;
                remaining = remaining < PGSIZE ? 0 : remaining - PGSIZE;
            }

            file_close(mme->file);
            list_remove(e);
            free(mme);
            return;
        }
        e = list_next(e);
    }
}

void munmap_all () {
    struct list *mmap_list = &thread_current()->mmap_list;
    while (!list_empty(mmap_list)) {
        struct mmap_entry *mme = list_entry(list_front(mmap_list), struct mmap_entry, elem);
        munmap(mme->mapid);
    }
}
