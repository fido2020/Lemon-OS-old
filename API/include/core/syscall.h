#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

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

void syscall(uint32_t call, uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);

#endif
