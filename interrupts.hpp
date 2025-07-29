#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

extern bool g_button_left_pending;
extern bool g_button_up_pending;
extern bool g_button_down_pending;
extern bool g_button_right_pending;
extern bool g_button_enc1_push_pending;
extern bool g_button_enc2_push_pending;


extern bool g_enc1_val_pending;
extern bool g_enc2_val_pending;

#ifdef __cplusplus
}
#endif
