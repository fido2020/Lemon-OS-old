#include <core/syscall.h>
#include <core/messagehandling.h>

uint32_t message_queue_next(ipc_message_t* msg){
	uint32_t queue_size;
	syscall(SYS_MSG_RECIEVE, msg /*Pointer to message*/, &queue_size /*Pointer to value with amount of messages in queue*/, 0, 0, 0);
	return queue_size;
}

void message_send_async(uint32_t pid, ipc_message_t msg){
	uint32_t m = msg.msg;
	uint32_t data = msg.data;

	syscall(SYS_MSG_RECIEVE, pid /*Target PID*/, m /*Message ID*/, data/*Message Data*/, 0, 0);
}

void message_send_sync(uint32_t pid, ipc_message_t msg){
	// Not implemented
}
