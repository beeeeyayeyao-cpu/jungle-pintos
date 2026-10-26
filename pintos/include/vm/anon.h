#ifndef VM_ANON_H
#define VM_ANON_H
#include "vm/vm.h"

//anonymous page를 위한 기능을 제공함 = VM_ANON

struct page;
enum vm_type;

struct anon_page {
};

void vm_anon_init (void);
bool anon_initializer (struct page *page, enum vm_type type, void *kva);

#endif
