/*
 * irq.h
 *
 *  Created on: Sep 8, 2021
 *      Author: salon
 */

#ifndef SRC_IRQ_H_
#define SRC_IRQ_H_

#include "timers.h"
#include "oscillators.h"
#include "gpio.h"
#include "scheduler.h"
#include "../app.h"

#include "em_letimer.h"
#include "sl_i2cspm.h"
#include "em_i2c.h"
#include "main.h"

uint32_t letimerMilliseconds();


#endif /* SRC_IRQ_H_ */
