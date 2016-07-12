/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.

 */
#ifndef __RF24_CONFIG_H__
#define __RF24_CONFIG_H__

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define _BV(x) (1<<(x))
#define SERIAL_DEBUG 1

#undef SERIAL_DEBUG
#ifdef SERIAL_DEBUG
#define IF_SERIAL_DEBUG(x) ({x;})
#else
#define IF_SERIAL_DEBUG(x)
#endif

#define pgm_read_word(p) (*(p))
//#define ce_edge "none"
//#define ce_active_low "1" 
//#define ce_direction "out"

const char* const ce_edge = "none";
const char* const ce_active_low = "1";
const char* const ce_direction = "out";
const char* const gpio_str = "2_pb13";
const int int_gpio_num = 2;

#endif // __RF24_CONFIG_H__
