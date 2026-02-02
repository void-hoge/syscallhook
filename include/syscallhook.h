#pragma once

#include <poll.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char* pathname;
    int flags;
    int has_mode;
    mode_t mode;
} sh_open_args;

typedef struct {
    int fd;
} sh_close_args;

typedef struct {
    int fd;
    unsigned long request;
    void* argp;
} sh_ioctl_args;

typedef struct {
    void* addr;
    size_t length;
    int prot;
    int flags;
    int fd;
    off_t offset;
} sh_mmap_args;

typedef struct {
    void* addr;
    size_t length;
} sh_munmap_args;

typedef struct {
    struct pollfd* fds;
    nfds_t nfds;
    int timeout;
} sh_poll_args;

__attribute__((weak)) void syscallhook_open_pre(const sh_open_args* args);
__attribute__((weak)) void syscallhook_open_post(const sh_open_args* args, int* result, int* errno_inout);
__attribute__((weak)) void syscallhook_close_pre(const sh_close_args* args);
__attribute__((weak)) void syscallhook_close_post(const sh_close_args* args, int* result, int* errno_inout);
__attribute__((weak)) void syscallhook_ioctl_pre(const sh_ioctl_args* args);
__attribute__((weak)) void syscallhook_ioctl_post(const sh_ioctl_args* args, int* result, int* errno_inout);
__attribute__((weak)) void syscallhook_mmap_pre(const sh_mmap_args* args);
__attribute__((weak)) void syscallhook_mmap_post(const sh_mmap_args* args, void** result, int* errno_inout);
__attribute__((weak)) void syscallhook_munmap_pre(const sh_munmap_args* args);
__attribute__((weak)) void syscallhook_munmap_post(const sh_munmap_args* args, int* result, int* errno_inout);
__attribute__((weak)) void syscallhook_poll_pre(const sh_poll_args* args);
__attribute__((weak)) void syscallhook_poll_post(const sh_poll_args* args, int* result, int* errno_inout);

#ifdef __cplusplus
}
#endif
