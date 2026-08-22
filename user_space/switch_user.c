#include "switch_user.h"

void switch_user_mode(uint32_t user_stack_top, uint32_t user_entry_point) {

    user_entry(user_stack_top, user_entry_point);

}