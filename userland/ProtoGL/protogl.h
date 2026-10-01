#pragma once
#include <stdint.h>

typedef struct ProtoWindow{
    uint8_t* framebuffer;
} ProtoWindow;

typedef struct ProtoColor {

    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

} ProtoColor;


bool init_protogl();
ProtoWindow* create_proto_window(int, int, uint32_t, uint32_t);

void clear_proto_window(ProtoColor);
void update_proto_window(ProtoWindow*);


uint32_t get_proto_time();
