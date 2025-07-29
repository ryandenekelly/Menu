#include "main.h"
#include "interrupts.hpp"
#include <cstdint>

bool g_button_left_pending = false;
bool g_button_up_pending = false;
bool g_button_down_pending = false;
bool g_button_right_pending = false;
bool g_button_enc1_push_pending = false;
bool g_button_enc2_push_pending = false;

bool g_enc1_val_pending = false;
bool g_enc2_val_pending = false;


const std::uint32_t DEBOUNCE_T = 20;
volatile std::uint32_t button_right_last_t = 0;
volatile std::uint32_t button_up_last_t = 0;
volatile std::uint32_t button_down_last_t = 0;
volatile std::uint32_t button_left_last_t = 0;
volatile std::uint32_t button_enc1_push_last_t = 0;
volatile std::uint32_t button_enc2_push_last_t = 0;

void Menu_proccessButtons(uint16_t GPIO_Pin) {
	std::uint8_t current_t = HAL_GetTick();
	switch (GPIO_Pin) {
	default:
		break;
	}
}



