#ifndef MSG_HANDLING_H
#define MSG_HANDLING_H

typedef struct {
	uint32_t sender_pid; // PID of Sender
	uint32_t reciever_pid; // PID of Reciever
	uint32_t msg; // The Message
	uint32_t data; // Message Data
} __attribute__((packed)) ipc_message_t;

void message_send_async();
void message_send_sync();

uint32_t message_queue_next(ipc_message_t* msg);

#endif 
