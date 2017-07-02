/* Thanks to BrokenThorn for Memory Management code*/

#include <stddef.h>
#include <stdint.h>

void* memset(void* dest, int src, size_t count){
    char* temp = (char*)dest;
    for(; count != 0; count--) *temp++ = src;
    return dest;
}

#define BLKS_PER_BYTE 8

#define BLK_SIZE 4096

#define BLK_ALIGN BLK_SIZE

static	uint32_t memsize=0;

static	uint32_t used_blks=0;

static	uint32_t max_blks=0;

static	uint32_t* memmap= 0;

inline void set (int bit);

inline void unset (int bit);

inline bool test (int bit);

int first_free ();

int first_free_s (size_t size);

inline void set (int bit) {

  memmap[bit / 32] |= (1 << (bit % 32));
}

inline void unset (int bit) {

  memmap[bit / 32] &= ~ (1 << (bit % 32));
}

inline bool test (int bit) {

	return memmap[bit / 32] &  (1 << (bit % 32));
}

int first_free () {

	for (uint32_t i=0; i< max_blks /32; i++)
		if (memmap[i] != 0xffffffff)
			for (int j=0; j<32; j++) {				//! test each bit in the dword
				int bit = 1 << j;
				if (! (memmap[i] & bit) )
					return i*4*8+j;
			}
	return -1;
}

int first_free_s (size_t size) {

	if (size==0)
		return -1;

	if (size==1)
		return first_free ();

	for (uint32_t i=0; i<max_blks /32; i++)
		if (memmap[i] != 0xffffffff)
			for (int j=0; j<32; j++) {

				int bit = 1<<j;
				if (! (memmap[i] & bit) ) {

					int startingBit = i*32;
					startingBit+=bit;

					uint32_t free=0; //loop through each bit to see if its enough space
					for (uint32_t count=0; count<=size;count++) {

						if (! test (startingBit+count) )
							free++;	// this bit is clear (free frame)

						if (free==size)
							return i*4*8+j; //free count==size needed; return index
					}
				}
			}

	return -1;
}

void memmgr_init (size_t memSize, uint32_t bitmap) {

	memsize	=	memSize;
	memmap	=	(uint32_t*) bitmap;
	max_blks	=	(memsize*1024) / BLK_SIZE;
	used_blks	=	max_blks;

	memset (memmap, 0xf, max_blks / BLKS_PER_BYTE );
}

void memmgr_init_region (uint32_t base, size_t size) {

	int align = base / BLK_SIZE;
	int blks = size / BLK_SIZE;

	for (; blks>=0; blks--) {
		unset (align++);
		used_blks--;


	}

	set (0);	//first blk is always set. This insures allocs cant be 0
}

void memmgr_deinit_region (uint32_t base, size_t size) {

	int align = base / BLK_SIZE;
	int blks = size / BLK_SIZE;

	for (; blks>=0; blks--) {
		set (align++);
		used_blks++;
	}

}

void* alloc_blk () {

	if ((max_blks - used_blks) <= 0)
		return 0;	//out of memory

	int frame = first_free ();

	if (frame == -1)
		return 0;	//out of memory
        
    set (frame);

	uint32_t addr = frame * BLK_SIZE;
	used_blks++;

	return (void*)addr;
}

void free_blk (void* p) {

	uint32_t addr = (uint32_t)p;
	int frame = addr / BLK_SIZE;

	unset (frame);

	used_blks--;
}

void* malloc (size_t size) {

	if ((max_blks - used_blks) <= size)
		return 0;	//not enough space

	int frame = first_free_s (size);

	if (frame == -1)
		return 0;	//not enough space

	for (uint32_t i=0; i<size; i++)
		set (frame+i);

	uint32_t addr = frame * BLK_SIZE;
	used_blks+=size;

	return (void*)addr;
}

void free (void* p, size_t size) {

	uint32_t addr = (uint32_t)p;
	int frame = addr / BLK_SIZE;

	for (uint32_t i=0; i<size; i++)
		unset (frame+i);

	used_blks-=size;
}

size_t	get_mem_size () { return memsize; }

uint32_t get_blk_count () { return max_blks; }

uint32_t get_used_blk_count () { return used_blks; }

uint32_t get_free_blk_count () { return max_blks - used_blks; }

uint32_t get_BLK_SIZE () { return BLK_SIZE; }