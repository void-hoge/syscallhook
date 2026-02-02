#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

#include "syscallhook.hpp"

static thread_local int g_recursion_guard = 0;

struct RecursionGuard {
    RecursionGuard() {
        g_recursion_guard++;
    }
    ~RecursionGuard() {
        g_recursion_guard--;
    }
};

static inline bool hook_enabled() {
    return g_recursion_guard == 0;
}

using open_fn = int(*)(const char*, int, ...);
using close_fn = int(*)(int);
using ioctl_fn = int(*)(int, unsigned long, ...);
using mmap_fn = void*(*)(void*, size_t, int, int, int, off_t);
using munmap_fn = int(*)(void*, size_t);
using poll_fn = int(*)(struct pollfd*, nfds_t, int);

static open_fn real_open = nullptr;
static close_fn real_close = nullptr;
static ioctl_fn real_ioctl = nullptr;
static mmap_fn real_mmap = nullptr;
static munmap_fn real_munmap = nullptr;
static poll_fn real_poll = nullptr;

static void resolve_real_syscalls() {
    if (!real_open) real_open = (open_fn)dlsym(RTLD_NEXT, "open");
    if (!real_close) real_close = (close_fn)dlsym(RTLD_NEXT, "close");
    if (!real_ioctl) real_ioctl = (ioctl_fn)dlsym(RTLD_NEXT, "ioctl");
    if (!real_mmap) real_mmap = (mmap_fn)dlsym(RTLD_NEXT, "mmap");
    if (!real_munmap) real_munmap = (munmap_fn)dlsym(RTLD_NEXT, "munmap");
    if (!real_poll) real_poll = (poll_fn)dlsym(RTLD_NEXT, "poll");
}

__attribute__((constructor))
static void syscallhook_init() {
    resolve_real_syscalls();
}

extern "C" int open(const char* pathname, int flags, ...) {
    mode_t mode = 0;
    int has_mode = 0;

    if (flags & O_CREAT) {
        has_mode = 1;
        va_list ap;
        va_start(ap, flags);
        mode = (mode_t)va_arg(ap, int);
        va_end(ap);
    }

    sh_open_args a{pathname, flags, has_mode, mode};

    if (hook_enabled()) {
        RecursionGuard g;
        if (syscallhook_open_pre) syscallhook_open_pre(&a);
    }

    int saved_errno = 0;
    int r = has_mode ? real_open(pathname, flags, mode) : real_open(pathname, flags);

    saved_errno = errno;

    if (hook_enabled()) {
        RecursionGuard g;
        int rr = r;
        int ee = saved_errno;
        if (syscallhook_open_post) syscallhook_open_post(&a, &rr, &ee);
        r = rr;
        saved_errno = ee;
    }

    errno = saved_errno;
    return r;
}

extern "C" int close(int fd) {
    resolve_real_syscalls();
    sh_close_args a{fd};

    if (hook_enabled()) {
        RecursionGuard g;
        if (syscallhook_close_pre) syscallhook_close_pre(&a);
    }

    int r = real_close(fd);
    int saved_errno = errno;

    if (hook_enabled()) {
        RecursionGuard g;
        int rr = r;
        int ee = saved_errno;
        if (syscallhook_close_post) syscallhook_close_post(&a, &rr, &ee);
        r = rr;
        saved_errno = ee;
    }

    errno = saved_errno;
    return r;
}

extern "C" int ioctl(int fd, unsigned long request, ...) {
    resolve_real_syscalls();
    void* argp = nullptr;
    va_list ap;
    va_start(ap, request);
    argp = va_arg(ap, void*);
    va_end(ap);

    sh_ioctl_args a{fd, request, argp};

    if (hook_enabled()) {
        RecursionGuard g;
        if (syscallhook_ioctl_pre) syscallhook_ioctl_pre(&a);
    }

    int r = real_ioctl(fd, request, argp);
    int saved_errno = errno;

    if (hook_enabled()) {
        RecursionGuard g;
        int rr = r;
        int ee = saved_errno;
        if (syscallhook_ioctl_post) syscallhook_ioctl_post(&a, &rr, &ee);
        r = rr;
        saved_errno = ee;
    }

    errno = saved_errno;
    return r;
}

extern "C" void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset) {
    resolve_real_syscalls();
    sh_mmap_args a{addr, length, prot, flags, fd, offset};

    if (hook_enabled()) {
        RecursionGuard g;
        if (syscallhook_mmap_pre) syscallhook_mmap_pre(&a);
    }

    void* r = real_mmap(addr, length, prot, flags, fd, offset);
    int saved_errno = errno;

    if (hook_enabled()) {
        RecursionGuard g;
        void* rr = r;
        int ee = saved_errno;
        if (syscallhook_mmap_post) syscallhook_mmap_post(&a, &rr, &ee);
        r = rr;
        saved_errno = ee;
    }

    errno = saved_errno;
    return r;
}

extern "C" int munmap(void* addr, size_t length) {
    resolve_real_syscalls();
    sh_munmap_args a{addr, length};

    if (hook_enabled()) {
        RecursionGuard g;
        if (syscallhook_munmap_pre) syscallhook_munmap_pre(&a);
    }

    int r = real_munmap(addr, length);
    int saved_errno = errno;

    if (hook_enabled()) {
        RecursionGuard g;
        int rr = r;
        int ee = saved_errno;
        if (syscallhook_munmap_post) syscallhook_munmap_post(&a, &rr, &ee);
        r = rr;
        saved_errno = ee;
    }

    errno = saved_errno;
    return r;
}

extern "C" int poll(struct pollfd* fds, nfds_t nfds, int timeout) {
    resolve_real_syscalls();
    sh_poll_args a{fds, nfds, timeout};

    if (hook_enabled()) {
        RecursionGuard g;
        if (syscallhook_poll_pre) syscallhook_poll_pre(&a);
    }

    int r = real_poll(fds, nfds, timeout);
    int saved_errno = errno;

    if (hook_enabled()) {
        RecursionGuard g;
        int rr = r;
        int ee = saved_errno;
        if (syscallhook_poll_post) syscallhook_poll_post(&a, &rr, &ee);
        r = rr;
        saved_errno = ee;
    }

    errno = saved_errno;
    return r;
}
