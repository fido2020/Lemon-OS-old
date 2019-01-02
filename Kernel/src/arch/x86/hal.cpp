#include <hal.h>

#include <gdt.h>
#include <idt.h>
#include <fatal.h>
#include <paging.h>
#include <video.h>
#include <mischdr.h>
#include <string.h>
#include <console.h>
#include <keyboard.h>
#include <serial.h>
#include <initrd.h>
#include <memory.h>
#include <physallocator.h>
#include <cpuid.h>

namespace HAL{
    memory_info_t mem_info;
    video_mode_t video_mode;

    multiboot_info_t multiboot_info;
    void initialize_core(multiboot_info_t mb_info){ // ALWAYS call this first
        multiboot_info = mb_info;

        // Check if Debugging Mode is enabled and if so initialize serial port
        init_serial();
        write_serial_string("Initializing Lemon...\r\n");
        
        // Initialize GDT and IDT
        gdt_initialize();
        idt_initialize();

        // Initialize Paging/Virtual Memory Manager
        paging_initialize();

        // Get Memory Map
        map_page(mb_info.mmapAddr, mb_info.mmapAddr, mb_info.mmapLength / PAGE_SIZE + 1);
        multiboot_memory_map_t* memory_map = (multiboot_memory_map_t*)mb_info.mmapAddr;

        // Initialize Memory Info Structure to pass to Physical Memory Allocator
        mem_info.memory_high = mb_info.memoryHi;
        mem_info.memory_low = mb_info.memoryLo;
        mem_info.mem_map = memory_map;
        mem_info.memory_map_len = mb_info.mmapLength;

        // Initialize Physical Memory Allocator
        physalloc_init(&mem_info);
    } 

    void initialize_video(){
        // Map Video Memory
        uint32_t vid_mem_size = multiboot_info.framebufferHeight*multiboot_info.framebufferPitch;
        //map_page(multiboot_info.framebufferAddr, multiboot_info.framebufferAddr, vid_mem_size / PAGE_SIZE + 1);
        uint32_t virtual_address = kernel_pages_allocate(vid_mem_size / PAGE_SIZE + 1);
	    map_page(multiboot_info.framebufferAddr, virtual_address, vid_mem_size / PAGE_SIZE + 1);
        physalloc_mark_region_used(multiboot_info.framebufferAddr, vid_mem_size);

        // Initialize Video Mode structure
        video_mode.width = multiboot_info.framebufferWidth;
        video_mode.height = multiboot_info.framebufferHeight;
        video_mode.bpp = multiboot_info.framebufferBpp;
        video_mode.pitch = multiboot_info.framebufferPitch;
        video_mode.address = /*multiboot_info.framebufferAddr*/virtual_address;
        video_mode.type = Graphical;

        video_initialize(video_mode);
    }

    void initialize_hid(){
        // Intialize Keyboard

        // Initalize Mouse
    }

    void initialize_all(multiboot_info_t mb_info){
        initialize_core(mb_info);
        initialize_video();
        initialize_hid();
    }
}