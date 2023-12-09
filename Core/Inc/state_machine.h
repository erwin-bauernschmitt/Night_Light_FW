/**
 *******************************************************************************
 * @file state_machine.h
 * @brief Declarations for state_machine.c
 *
 * @author Erwin Bauernschmitt
 * @date 3/12/2023
 *******************************************************************************
 */


#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H


/**
 * @brief States the night light can be in.
 */
typedef enum {
	STANDBY,					///< Light off (high ambient light).
	WHITE_LIGHT,				///< Light on (cool/warm white light spectrum).
	RGB_LIGHT,					///< Light on (RGB light spectrum).
	POT_CALIBRATION,			///< Potentiometer calibration mode.
	LED_CALIBRATION,			///< LED dot correction calibration mode.
	AMBIENT_LIGHT_CALIBRATION	///< Ambient light sensor calibration mode.
} State;


/**
 * @brief Substates of the potentiometer calibration mode.
 */
typedef enum {
	POT_CALIBRATION_START,		///< Start of potentiometer calibration process.
	POT_1_LOWER,				///< Capture ADC value of Pot 1 lower limit.
	POT_1_UPPER,				///< Capture ADC value of Pot 1 upper limit.
	POT_2_LOWER,				///< Capture ADC value of Pot 2 lower limit.
	POT_2_UPPER,				///< Capture ADC value of Pot 2 upper limit.
	POT_3_LOWER,				///< Capture ADC value of Pot 3 lower limit.
	POT_3_UPPER,				///< Capture ADC value of Pot 3 upper limit.
} PotCalibrationSubstate;


/**
 * @brief Substates of the LED dot corretion calibration mode.
 */
typedef enum {
	LED_CALIBRATION_START,		///< Start of LED calibration process.
	LED_1,						///< Tune LED 1 colour channels and save.
	LED_2,						///< Tune LED 2 colour channels and save.
	LED_3,						///< Tune LED 3 colour channels and save.
	LED_4,						///< Tune LED 4 colour channels and save.
	LED_5,						///< Tune LED 5 colour channels and save.
	LED_6,						///< Tune LED 6 colour channels and save.
	LED_7,						///< Tune LED 7 colour channels and save.
	LED_8,						///< Tune LED 8 colour channels and save.
	LED_9,						///< Tune LED 9 colour channels and save.
	LED_10,						///< Tune LED 10 colour channels and save.
	LED_11,						///< Tune LED 11 colour channels and save.
	LED_12,						///< Tune LED 12 colour channels and save.
	LED_13,						///< Tune LED 13 colour channels and save.
	LED_14,						///< Tune LED 14 colour channels and save.
	LED_15,						///< Tune LED 15 colour channels and save.
	LED_16,						///< Tune LED 16 colour channels and save.
} LEDCalibrationSubstate;


/**
 * @brief Substates of the ambient light sensor calibration mode.
 */
typedef enum {
	LIGHT_CALIBRATION_START,	///< Start of ambient light calibration process.
	LIT_ROOM,					///< Capture ambient effect of LEDs (lit room).
	DARK_ROOM,					///< Capture ambient effect of LEDs (dark room).
} LightCalibrationSubstate;


/**
 * @brief Types of events that could trigger a state change.
 */
typedef enum {
	POT_1_BUTTON_PRESS,			///< Press and release of pot 1 button (<5s).
	POT_2_BUTTON_PRESS,			///< Press and release of pot 2 button (<5s).
	POT_3_BUTTON_PRESS,			///< Press and release of pot 3 button (<5s).
	POT_1_BUTTON_HOLD,			///< Hold of pot 1 button (>=5s).
	POT_2_BUTTON_HOLD,			///< Hold of pot 2 button (>=5s).
	POT_3_BUTTON_HOLD,			///< Hold of pot 3 button (>=5s).
	AMBIENT_LIGHT_TURN_ON,		///< Ambient above upper hysteresis threshold.
	AMBIENT_LIGHT_TURN_OFF,		///< Ambient below lower hysteresis threshold.
	NO_EVENT = -1				///< No event.
} EventType;


typedef enum {
	RELEASED,
	PRESSED,
	INVALID = -1
} InputFlag;


void update_state(EventType event);

void handle_standby(EventType event);
void handle_white_light(EventType event);
void handle_RGB_light(EventType event);
void update_pot_cal_substate(EventType event);
void update_led_cal_substate(EventType event);
void update_light_cal_substate(EventType event);


#endif /* STATE_MACHINE_H */
