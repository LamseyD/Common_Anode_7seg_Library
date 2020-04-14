/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    SEG_DISPLAY.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC802.h"
#include "fsl_debug_console.h"
#include "seven_seg.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
#define data_pin (12)
#define latch_pin_rclk (8)
#define clock_pin_srclk (9)
int main(void) {

  	/* Init board hardware. */
	SYSCON -> SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_GPIO0_MASK | SYSCON_SYSAHBCLKCTRL0_GPIO_INT_MASK); //turn on GPIO
	SYSCON -> PRESETCTRL0 &= ~ (SYSCON_PRESETCTRL0_GPIO0_RST_N_MASK | SYSCON_PRESETCTRL0_GPIOINT_RST_N_MASK); //bit to 0 (reset) GPIO AND INTERRUP
	SYSCON -> PRESETCTRL0 |= (SYSCON_PRESETCTRL0_GPIO0_RST_N_MASK | SYSCON_PRESETCTRL0_GPIOINT_RST_N_MASK); //bit to 1 (set) CLEAR  BOTH
	GPIO -> DIRSET[0] = (1UL << data_pin);
	GPIO -> DIRSET[0] = (1UL << latch_pin_rclk);
	GPIO -> DIRSET[0] = (1UL << clock_pin_srclk);

	GPIO -> CLR[0] = (1UL << data_pin); //turn on LED
	GPIO -> CLR[0] = (1UL << latch_pin_rclk);
	GPIO -> CLR[0] = (1UL << clock_pin_srclk);

	setup_pins(latch_pin_rclk,data_pin,clock_pin_srclk);

    /* Force the counter to be placed into memory. */
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        display_seg7(3425.0);
        __asm volatile ("nop");
    }
    return 0 ;
}
