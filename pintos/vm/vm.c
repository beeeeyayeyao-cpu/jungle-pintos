/* vm.c: Generic interface for virtual memory objects. */

#include "threads/malloc.h"
#include "vm/vm.h"
#include "vm/inspect.h"

/* Initializes the virtual memory subsystem by invoking each subsystem's
 * intialize codes. */
void
vm_init (void) {
	vm_anon_init ();
	vm_file_init ();
#ifdef EFILESYS  /* For project 4 */
	pagecache_init ();
#endif
	register_inspect_intr ();
	/* DO NOT MODIFY UPPER LINES. */
	/* TODO: Your code goes here. */
}

/* Get the type of the page. This function is useful if you want to know the
 * type of the page after it will be initialized.
 * This function is fully implemented now. */
enum vm_type
page_get_type (struct page *page) {
	int ty = VM_TYPE (page->operations->type);
	switch (ty) {
		case VM_UNINIT:
			return VM_TYPE (page->uninit.type);
		default:
			return ty;
	}
}

/* Helpers */
static struct frame *vm_get_victim (void);
static bool vm_do_claim_page (struct page *page);
static struct frame *vm_evict_frame (void);

/* Create the pending page object with initializer. If you want to create a
 * page, do not create it directly and make it through this function or
 * `vm_alloc_page`. */
/*
	페이지 구조체를 할당하고 페이지 유형에 따라 적절한 초기화 함수를 설정하여 새 페이지를 초기화한 후 사용자 프로그램으로 제어를 반환합니다.
	페이지를 생성해야 할 경우, 직접 생성하지 말고 이 함수나 vm_alloc_page`를 통해 생성하십시오.

	지정된 타입으로 초기화되지 않은(uninitialized) 페이지를 생성하십시오. 
	이 페이지의 `swap_in` 핸들러는 타입에 맞춰 페이지를 자동으로 초기화하고, 주어진 `AUX`를 인자로 `INIT` 함수를 호출합니다. 
	페이지 구조체(`page struct`)를 생성한 후에는 이를 해당 프로세스의 spt에 삽입하십시오. 이때 `vm.h`에 정의된 `VM_TYPE` 매크로를 활용하면 편리합니다.

	lazy_load_segment 함수는 실행 파일의 페이지를 초기화하는 역할을 하며, 페이지 폴트(page fault)가 발생할 때 호출됩니다.
	이 함수는 `page` 구조체와 `aux`를 인자로 받는데, 여기서 `aux`는 `load_segment`에서 설정한 정보입니다. 
	이 정보를 활용하여 세그먼트를 읽어올 파일을 찾고, 최종적으로 해당 세그먼트를 메모리로 읽어와야 합니다.
*/
bool
vm_alloc_page_with_initializer (enum vm_type type, void *upage, bool writable,
		vm_initializer *init, void *aux) {

	ASSERT (VM_TYPE(type) != VM_UNINIT)

	struct supplemental_page_table *spt = &thread_current ()->spt;

	/* Check wheter the upage is already occupied or not. */
	if (spt_find_page (spt, upage) == NULL) {
		/* TODO: Create the page, fetch the initialier according to the VM type,
		 * TODO: and then create "uninit" page struct by calling uninit_new. You
		 * TODO: should modify the field after calling the uninit_new. */

		/* TODO: Insert the page into the spt. */
	}
err:
	return false;
}

/* Find VA from spt and return page. On error, return NULL. */
//struct page제공된 추가 페이지 표에서 va에 해당하는 값을 찾으십시오 . 찾지 못하면 NULL을 반환합니다.
struct page *
spt_find_page (struct supplemental_page_table *spt UNUSED, void *va UNUSED) {
	struct page *page = NULL;
	/* TODO: Fill this function. */

	return page;
}

/* Insert PAGE into spt with validation. */
/*
	struct page주어진 spt에 삽입합니다 . 
	이 함수는 가상 주소가 주어진 spt에 존재하지 않는지 확인해야 합니다.
*/
bool
spt_insert_page (struct supplemental_page_table *spt UNUSED,
	struct page *page UNUSED) {
	int succ = false;
	/* TODO: Fill this function. */

	return succ;
}

void
spt_remove_page (struct supplemental_page_table *spt, struct page *page) {
	vm_dealloc_page (page);
	return true;
}

/* Get the struct frame, that will be evicted. */
/*
	page.page() 를 호출하여 사용자 풀에서 새 물리적 페이지를 가져옵니다 palloc_get_page. 
	사용자 풀에서 페이지를 성공적으로 가져오면 프레임을 할당하고 멤버를 초기화한 후 반환합니다. 
	`page.page()`를 구현한 후에는 vm_get_frame 모든 사용자 공간 페이지(PALLOC_USER)를 이 함수를 통해 할당해야 합니다. 
	페이지 할당 실패 시 스왑 아웃 처리는 현재로서는 필요하지 않으며, 해당 경우를 `swapout`으로 표시하기만 하면 됩니다 PANIC ("todo") .
*/
static struct frame *
vm_get_victim (void) {
	struct frame *victim = NULL;
	 /* TODO: The policy for eviction is up to you. */

	return victim;
}

/* Evict one page and return the corresponding frame.
 * Return NULL on error.*/
static struct frame *
vm_evict_frame (void) {
	struct frame *victim UNUSED = vm_get_victim ();
	/* TODO: swap out the victim and return the evicted frame. */

	return NULL;
}

/* palloc() and get frame. If there is no available page, evict the page
 * and return it. This always return valid address. That is, if the user pool
 * memory is full, this function evicts the frame to get the available memory
 * space.*/
static struct frame *
vm_get_frame (void) {
	struct frame *frame = NULL;
	/* TODO: Fill this function. */

	ASSERT (frame != NULL);
	ASSERT (frame->page == NULL);
	return frame;
}

/* Growing the stack. */
static void
vm_stack_growth (void *addr UNUSED) {
}

/* Handle the fault on write_protected page */
static bool
vm_handle_wp (struct page *page UNUSED) {
}

/* Return true on success */
/*
	spt에서 폴트난 페이지를 찾는다. 메모리 참조가 유효하다면, 페이지에 있는 데이터(파일 시스템이나 스왑 슬롯에 있거나, all-zero 페이지일지도 모름)를 
	찾기 위해 spt 엔트리를 사용한다.
	만약 공유(i.e. copy on write)하는 것을 구현한다면, 데이터의 페이지는 이미 페이지 프레임 안에 있을 수 있지만 페이지 테이블에는 없을 수 있다.
	만약 spt가 유저 프로세스가 접근하려는 주소에 어떤 데이터도 없어야 한다고 하는 경우, 
	또는 페이지가 커널 가상 메모리 내에서 있는 경우, 
	또는 접근이 read-only page에 write하는 것을 시도하는 경우, 
	그 접근은 유효하지 않다. 유효하지 않은 접근은 프로세스를 종료하고 모든 자원들을 확보한다.
	만약 공유하는 것을 구현한다면, 필요로 하는 데이터는 이미 프레임 안에 있을 수 있으며, 이러한 경우 그 프레임을 찾을 수 있어야 한다.
	file system, swap, zeroing it 등으로부터 프레임에 데이터를 가져온다. 


	만약 공유하는 것을 구현한다면, 필요로 하는 데이터는 이미 프레임 안에 있을 것이며 이 경우 따로 작업을 할 필요가 없다.
	폴트가 있는 가상 주소에 대한 페이지 테이블 엔트리를 물리 페이지로 가리킨다. threads/mmu.c에 있는 함수를 사용할 수 있다.
*/
bool
vm_try_handle_fault (struct intr_frame *f UNUSED, void *addr UNUSED, bool user UNUSED, bool write UNUSED, bool not_present UNUSED) 
{
	struct supplemental_page_table *spt UNUSED = &thread_current ()->spt;
	struct page *page = NULL;
	/* TODO: Validate the fault */
	/* TODO: Your code goes here */

	return vm_do_claim_page (page);
}

/* Free the page.
 * DO NOT MODIFY THIS FUNCTION. */
void
vm_dealloc_page (struct page *page) {
	destroy (page);
	free (page);
}

/* Claim the page that allocate on VA. */
/*
	할당할 페이지를 확보합니다 va. 
	먼저 페이지를 가져온 다음, 해당 페이지를 사용하여 vm_do_claim_page 함수를 호출해야 합니다.
*/
bool
vm_claim_page (void *va UNUSED) {
	struct page *page = NULL;
	/* TODO: Fill this function */

	return vm_do_claim_page (page);
}

/* Claim the PAGE and set up the mmu. */

/*
	클레임은 물리적 프레임, 즉 페이지를 할당하는 것을 의미합니다. 
	먼저 vm_get_frame(템플릿에서 이미 수행되어 있는) 함수를 호출하여 프레임을 가져옵니다. 
	그런 다음 MMU를 설정해야 합니다. 
	다시 말해, 페이지 테이블에서 가상 주소와 물리적 주소 간의 매핑을 추가해야 합니다. 
	반환 값은 작업의 성공 여부를 나타내야 합니다.
*/
static bool
vm_do_claim_page (struct page *page) {
	struct frame *frame = vm_get_frame ();

	/* Set links */
	frame->page = page;
	page->frame = frame;

	/* TODO: Insert page table entry to map page's VA to frame's PA. */

	return swap_in (page, frame->kva);
}

/* Initialize new supplemental page table */
/*
	spt를 초기화합니다. 
	spt에 사용할 데이터 구조를 선택할 수 있습니다. 
	이 함수는 새 프로세스가 시작될 때( initd) 와 프로세스가 포크될 때( __do_fork) 호출됩니다
*/
void
supplemental_page_table_init (struct supplemental_page_table *spt UNUSED) {
}

/* Copy supplemental page table from src to dst */
/*
	src 의 spt를 dst로 복사합니다. 
	이는 자식 프로세스가 부모 프로세스의 실행 컨텍스트를 상속받아야 할 때(예: `fork()`) 사용됩니다.
	`src`의 spt에 있는 각 페이지를 순회하며, 
	해당 항목(entry)을 `dst`의 ​​spt에 그대로 복사하십시오. 
	이때 초기화되지 않은 페이지(uninit page)를 할당하고, 즉시 해당 페이지를 확보(claim)해야 합니다.
*/
bool
supplemental_page_table_copy (struct supplemental_page_table *dst UNUSED,
		struct supplemental_page_table *src UNUSED) {
}

/*
	spt가 점유하고 있던 모든 자원을 해제합니다. 
	이 함수는 프로세스가 종료될 때(userprog/process.c 내의 process_exit()) 호출됩니다.
	테이블 내의 페이지 항목들을 순회하며 각 페이지에 대해 destroy(page)를 호출해야 합니다. 
	이 함수 내에서 실제 페이지 테이블(pml4)이나 물리 메모리(palloc을 통해 할당된 메모리)에 대해서는 신경 쓸 필요가 없습니다. 
	spt가 정리된 후, 이 함수를 호출한 측에서 해당 자원들을 정리하기 때문입니다.
*/
/* Free the resource hold by the supplemental page table */
void
supplemental_page_table_kill (struct supplemental_page_table *spt UNUSED) {
	/* TODO: Destroy all the supplemental_page_table hold by thread and
	 * TODO: writeback all the modified contents to the storage. */
}
