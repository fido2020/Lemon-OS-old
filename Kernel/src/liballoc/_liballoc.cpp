#include <paging.h>

extern "C" {

	int liballoc_lock() {
		asm("cli");
		return 0;
	}

	int liballoc_unlock() {
		asm("sti");
		return 0;
	}

	void* liballoc_alloc(int pages) {
		return (void*)pages_allocate(pages);
	}

	int liballoc_free(void* addr, int pages) {
		pages_free((uint32_t)addr, pages);
		return 0;
	}
}