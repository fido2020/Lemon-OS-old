#pragma once

#include <mischdr.h>

namespace HAL{
    extern memory_info_t mem_info;
    extern video_mode_t video_mode;

    void initialize_core(multiboot_info_t mb_info);

    void initialize_video();

    void initialize_hid();

    void initialize_all(multiboot_info_t mb_info);
}