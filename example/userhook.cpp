#include "syscallhook.hpp"
#include <stdio.h>
#include <iostream>

extern "C" void syscallhook_open_pre(const sh_open_args* a) {
    std::cout << "syscallhook_open_pre" << std::endl;;
}

extern "C" void syscallhook_open_post(const sh_open_args* a, int* result, int* errno_inout) {
    std::cout << "syscallhook_open_post" << std::endl;;
}

extern "C" void syscallhook_close_pre(const sh_close_args* a) {
    std::cout << "syscallhook_close_pre" << std::endl;
}

extern "C" void syscallhook_close_post(const sh_close_args* a, int* result, int* errno_inout) {
    std::cout << "syscallhook_close_post" << std::endl;
}

extern "C" void syscallhook_ioctl_pre(const sh_ioctl_args* args) {
    std::cout << "syscallhook_ioctl_pre" << std::endl;
}

extern "C" void syscallhook_ioctl_post(const sh_ioctl_args* args, int* result, int* errno_inout) {
    std::cout << "syscallhook_ioctl_post" << std::endl;
}

extern "C" void syscallhook_mmap_pre(const sh_mmap_args* args) {
    std::cout << "syscallhook_mmap_pre" << std::endl;
}

extern "C" void syscallhook_mmap_post(const sh_mmap_args* args, void** result, int* errno_inout) {
    std::cout << "syscallhook_mmap_post" << std::endl;
}

extern "C" void syscallhook_munmap_pre(const sh_munmap_args* args) {
    std::cout << "syscallhook_munmap_pre" << std::endl;
}

extern "C" void syscallhook_munmap_post(const sh_munmap_args* args, int* result, int* errno_inout) {
    std::cout << "syscallhook_munmap_post" << std::endl;
}

extern "C" void syscallhook_poll_pre(const sh_poll_args* args) {
    std::cout << "syscallhook_poll_pre" << std::endl;
}

extern "C" void syscallhook_poll_post(const sh_poll_args* args, int* result, int* errno_inout) {
    std::cout << "syscallhook_poll_post" << std::endl;
}
