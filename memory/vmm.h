#pragma once
#include <stdint.h>

void vmm_map_page(uint32_t, uint32_t*);

void vmm_handle_pagefault(uint32_t);