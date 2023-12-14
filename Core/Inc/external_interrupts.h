/**
 *******************************************************************************
 * @file external_interrupts.h
 * @brief Declarations for external_interrupts.c
 *
 * @author Erwin Bauernschmitt
 * @date 5/12/2023
 *******************************************************************************
 */


#include <stdint.h>
#include "hardware_defines.h"
#include "state_machine.h"
#include "colour_control.h"


typedef struct {
    uint32_t *last_time;
    ButtonState *state;
    EventType short_press_event;
    EventType long_press_event;
    ButtonState *other_button1_state;
    ButtonState *other_button2_state;
} ButtonInfo;


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void handle_button(ButtonInfo *button, uint32_t current_time);
void initialise_button_states(void);
void determine_led_errors(void);
void print_binary(uint16_t value);
