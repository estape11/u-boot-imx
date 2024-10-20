/*
 * Copyright (C) 2016-2022 Technologic Systems, Inc. dba embeddedTS
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/gpio.h>
#include <asm/gpio.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/mx6-pins.h>
#include <status_led.h>

static unsigned int saved_state[4] = {STATUS_LED_OFF,
	STATUS_LED_OFF, STATUS_LED_OFF, STATUS_LED_OFF};

iomux_v3_cfg_t const led_pads[] = {
	MX6_PAD_EIM_D27__GPIO3_IO27    | MUX_PAD_CTRL(NO_PAD_CTRL), // Green
	MX6_PAD_GPIO_2__GPIO1_IO02     | MUX_PAD_CTRL(NO_PAD_CTRL), // Red
	MX6_PAD_GPIO_9__GPIO1_IO09     | MUX_PAD_CTRL(NO_PAD_CTRL), // Yellow
	MX6_PAD_DISP0_DAT4__GPIO4_IO25 | MUX_PAD_CTRL(NO_PAD_CTRL), // Blue
};

void coloured_LED_init(void)
{
	imx_iomux_v3_setup_multiple_pads(led_pads, ARRAY_SIZE(led_pads));
}

void red_led_on(void)
{
	gpio_direction_output(CONFIG_RED_LED, 0);
	saved_state[STATUS_LED_RED] = STATUS_LED_ON;
}

void red_led_off(void)
{
	gpio_direction_output(CONFIG_RED_LED, 1);
	saved_state[STATUS_LED_RED] = STATUS_LED_OFF;
}

void blue_led_on(void)
{
	gpio_direction_output(CONFIG_BLUE_LED, 1);
	saved_state[STATUS_LED_BLUE] = STATUS_LED_ON;
}

void blue_led_off(void)
{
	gpio_direction_output(CONFIG_BLUE_LED, 0);
	saved_state[STATUS_LED_BLUE] = STATUS_LED_OFF;
}

void yellow_led_on(void)
{
	gpio_direction_output(CONFIG_YEL_LED, 0);
	saved_state[STATUS_LED_YELLOW] = STATUS_LED_ON;
}

void yellow_led_off(void)
{
	gpio_direction_output(CONFIG_YEL_LED, 1);
	saved_state[STATUS_LED_YELLOW] = STATUS_LED_OFF;
}

void green_led_on(void)
{
	gpio_direction_output(CONFIG_GREEN_LED, 0);
	saved_state[STATUS_LED_GREEN] = STATUS_LED_ON;
}

void green_led_off(void)
{
	gpio_direction_output(CONFIG_GREEN_LED, 1);
	saved_state[STATUS_LED_GREEN] = STATUS_LED_OFF;
}

void __led_init(led_id_t mask, int state)
{
	__led_set(mask, state);
}

void __led_toggle(led_id_t mask)
{
	if (STATUS_LED_RED == mask) {
		if (STATUS_LED_ON == saved_state[STATUS_LED_RED])
			red_led_off();
		else
			red_led_on();
	} 
	if (STATUS_LED_GREEN == mask) {
		if (STATUS_LED_ON == saved_state[STATUS_LED_GREEN])
			green_led_off();
		else
			green_led_on();
	}
	if (STATUS_LED_YELLOW == mask) {
		if (STATUS_LED_ON == saved_state[STATUS_LED_YELLOW])
			yellow_led_off();
		else
			yellow_led_on();
	}
	if (STATUS_LED_BLUE == mask) {
		if (STATUS_LED_ON == saved_state[STATUS_LED_BLUE])
			blue_led_off();
		else
			blue_led_on();
	}
}

void __led_set(led_id_t mask, int state)
{
	if (STATUS_LED_RED == mask) {
		if (STATUS_LED_ON == state)
			red_led_on();
		else
			red_led_off();

	}
	if (STATUS_LED_GREEN == mask) {
		if (STATUS_LED_ON == state)
			green_led_on();
		else
			green_led_off();

	}
	if (STATUS_LED_YELLOW == mask) {
		if (STATUS_LED_ON == state)
			yellow_led_on();
		else
			yellow_led_off();
	}
	if (STATUS_LED_BLUE == mask) {
		if (STATUS_LED_ON == state)
			blue_led_on();
		else
			blue_led_off();
	}
}
