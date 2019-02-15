#include <syscalls.h>

#include <video.h>
#include <window.h>
#include <gui.h>
#include <physallocator.h>
#include <paging.h>
#include <multitasking.h>
#include <hal.h>
#include <string.h>

#define NUM_SYSCALLS 21

#define SYS_EXIT 0
#define SYS_EXEC 1
#define SYS_READ 2
#define SYS_WRITE 3
#define SYS_OPEN 4
#define SYS_CLOSE 5
#define SYS_CREATE 6
#define SYS_LINK 7
#define SYS_UNLINK 8
#define SYS_GETPID 9
#define SYS_ALLOC_PAGES 10
#define SYS_MSG_SEND_ASYNC 11
#define SYS_MSG_SEND_SYNC 12
#define SYS_MSG_RECIEVE 13
#define SYS_CREATE_WINDOW 14
#define SYS_CREATE_DESKTOP 15
#define SYS_PAINT_DESKTOP 16
#define SYS_DESKTOP_GET_WIN_AMOUNT 17
#define SYS_DESKTOP_GET_WIN_HANDLES 18
#define SYS_GET_WINDOW_INFO 19
#define SYS_COPY_WIN_SURFACE_RECT 20

typedef int(*syscall_t)(regs32_t*);

int sys_exit(regs32_t* r){
	return 0;
}

int sys_exec(regs32_t* r){
	return 0;
}

int sys_read(regs32_t* r){
	return 0;
}

int sys_write(regs32_t* r){
	return 0;
}

int sys_open(regs32_t* r){
	return 0;
}

int sys_close(regs32_t* r){
	return 0;
}

int sys_create(regs32_t* r){
	return 0;
}

int sys_link(regs32_t* r){
	return 0;
}

int sys_unlink(regs32_t* r){
	return 0;
}

int sys_getpid(regs32_t* r){
	uint32_t* pid_ptr = (uint32_t*)r->ebx;
	*pid_ptr = get_current_proc()->pid;
	return 0;
}

int sys_alloc_pages(regs32_t* r){

	uint32_t virt = pages_allocate(r->ebx, get_current_proc()->page_directory);

	for(uint32_t i = 0; i < r->ebx; i++){
		uint32_t phys = physalloc_alloc_block();
		map_page(phys,virt + i * PAGE_SIZE, 1, get_current_proc()->page_directory);
	}

	uint32_t* addr_ptr = (uint32_t*)r->ecx;
	*addr_ptr = virt;
	return 0;
}

int sys_msg_send_async(regs32_t* r){
	uint32_t pid = r->ebx;
	uint32_t id = r->ecx;
	uint32_t data = r->edx;

	message_t msg;
	msg.sender_pid = get_current_proc()->pid;
	msg.reciever_pid = pid;
	msg.id = id;
	msg.data = data;

	process_t* proc = find_by_pid(pid);
	if(proc == 0) return 1;

	proc->message_queue.add_back(msg);

	return 0;
}

int sys_msg_send_sync(regs32_t* r){

}

int sys_msg_recieve(regs32_t* r){
	message_t* message_ptr = (message_t*)r->ebx;
	*message_ptr = get_current_proc()->message_queue[0];
	get_current_proc()->message_queue.remove_at(0);

	return 0;
}

int sys_create_window(regs32_t* r){
	whandle_t* handle_ptr = (whandle_t*)r->ebx;
	win_info_t winfo = *(win_info_t*)r->ecx;
	whandle_t handle = (whandle_t)malloc(sizeof(window_t));
	if(handle_ptr) *handle_ptr = handle;
	register_handle(handle);
	window_t* win = (window_t*)handle;
	win->x = winfo.x;
	win->y = winfo.y;
	win->width = winfo.width;
	win->height = winfo.height;
	win->flags = winfo.flags;

	write_serial_string("\r\nwin height: ");
	write_serial_string(itoa(winfo.height));

	win->buffer = (uint8_t*)malloc(win->width*win->height*4);
	get_main_desktop()->windows->add_back((window_t*)handle);
	return 0;
}

int sys_create_desktop(regs32_t* r){
	handle_t* handle_ptr = (handle_t*)r->ebx;
	handle_t handle = (handle_t)malloc(sizeof(desktop_t));
	register_handle(handle);
	desktop_t* desktop = (desktop_t*)handle;
	set_main_desktop(desktop);
	desktop->windows = new List<window_t*>();
	desktop->surface.buffer = (uint8_t*)malloc(HAL::video_mode.pitch*HAL::video_mode.height);
	desktop->surface.height = HAL::video_mode.height;
	*handle_ptr = handle;

	return 0;
}

int sys_paint_desktop(regs32_t* r){
	handle_t handle = (handle_t)r->ebx;
	surface_t* surface = (surface_t*)r->ecx;

	if(!(find_handle(handle).handle)) return 1;
	desktop_t* desktop = (desktop_t*)handle;

	memcpy(desktop->surface.buffer,surface->buffer,surface->width*surface->height*4);
}

int sys_desktop_get_win_amount(regs32_t* r){
	handle_t handle = (handle_t)r->ebx;

	if(!(find_handle(handle).handle)) return 1;
	desktop_t* desktop = (desktop_t*)handle;

	uint32_t* amount = (uint32_t*)r->ecx;
	*amount = desktop->windows->get_length();

	return 0;
}

int sys_desktop_get_window_handles(regs32_t* r){
	handle_t handle = (handle_t)r->ebx;

	if(!(find_handle(handle).handle)) return 1;
	desktop_t* desktop = (desktop_t*)handle;

	uint32_t* handle_array = (uint32_t*)r->ecx;

	for(int i = 0; i < desktop->windows->get_length(); i++){
		handle_array[i] = (uint32_t)(*(desktop->windows))[i];
	}
}

int sys_get_window_info(regs32_t* r){
	handle_t handle = (handle_t)r->ebx;
	win_info_t* info = (win_info_t*)r->ecx;

	if(!(find_handle(handle).handle)) return 1;
	window_t* win = (window_t*)handle;

	info->width = win->width;
	info->height = win->height;
	info->x = win->x;
	info->y = win->y;
	info->flags = win->flags;

	

	//write_serial_string("\r\nWindow Amount: ");
	//write_serial_string(itoa(info->height));
}

int sys_copy_win_surface_rect(regs32_t* r){
	surface_t* dest = (surface_t*)r->ebx;
	handle_t src = (handle_t)r->ecx;
	rect_t rect = *(rect_t*)r->edx;
	vector2i_t pos = *(vector2i_t*)r->esi;

	if(!(find_handle(src).handle)) return 1;
	window_t* win = (window_t*)src;

	for(int i = 0; i < rect.size.y && (i + pos.y) < HAL::video_mode.height; i++){
		memcpy((void*)(dest->buffer + dest->width * (i + pos.y) * 4 + pos.x*4), (void*)(win->buffer + win->width * (i + rect.pos.y) * 4 + (rect.pos.x * 4)), rect.size.x * 4);
	}
}

syscall_t syscalls[NUM_SYSCALLS]{
	sys_exit,					// 0
	sys_exec,
	sys_read,
	sys_write,
	sys_open,
	sys_close,					// 5
	sys_create,
	sys_link,
	sys_unlink,
	sys_getpid,
	sys_alloc_pages,			// 10
	sys_msg_send_async,
	sys_msg_send_sync,
	sys_msg_recieve,
	sys_create_window,
	sys_create_desktop,			// 15
	sys_paint_desktop,
	sys_desktop_get_win_amount,
	sys_desktop_get_window_handles,
	sys_get_window_info,
	sys_copy_win_surface_rect
};

void syscalls_init() {
	interrupt_register_handler(0x69,syscall_handler);
}

void syscall_handler(regs32_t* regs) {
	if (regs->eax >= NUM_SYSCALLS) // If syscall is non-existant then return
		return;
	//write_serial_string("\r\nSyscall: ");
	//write_serial_string(itoa(regs->eax));
	int ret = syscalls[regs->eax](regs); // Call syscall
	regs->eax = ret;
}
