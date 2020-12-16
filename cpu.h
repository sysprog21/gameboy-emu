#pragma once

#include <stdint.h>
#include "gameboy.h"

/* Interrupt masks */
#define VBLANK_INTR 0x01
#define LCDC_INTR 0x02
#define TIMER_INTR 0x04
#define SERIAL_INTR 0x08
#define CONTROL_INTR 0x10
#define ANY_INTR 0x1F

/* SERIAL SC register masks. */
#define SERIAL_SC_TX_START 0x80
#define SERIAL_SC_CLOCK_SRC 0x01

/* Interrupt jump addresses */
#define VBLANK_INTR_ADDR 0x0040
#define LCDC_INTR_ADDR 0x0048
#define TIMER_INTR_ADDR 0x0050
#define SERIAL_INTR_ADDR 0x0058
#define CONTROL_INTR_ADDR 0x0060

/* SPRITE controls */
#define NUM_SPRITES 0x28
#define MAX_SPRITES_LINE 0x0A
#define OBJ_PRIORITY 0x80
#define OBJ_FLIP_Y 0x40
#define OBJ_FLIP_X 0x20
#define OBJ_PALETTE 0x10

uint8_t __cpu_execute_cb(struct gb_s *gb);

#ifdef DEBUG
int cpu_step(struct gb_s *gb);
#else
void cpu_step(struct gb_s *gb);
#endif

#if ENABLE_LCD
void __gb_draw_line(struct gb_s *gb);
#endif

void __cpu_timer(struct gb_s *gb, uint8_t inst_cycles);
