#pragma once
#include "gameboy.h"

/* Memory addresses */
#define ROM_0_ADDR 0x0000
#define ROM_N_ADDR 0x4000
#define VRAM_ADDR 0x8000
#define CART_RAM_ADDR 0xA000
#define WRAM_0_ADDR 0xC000
#define WRAM_1_ADDR 0xD000
#define ECHO_ADDR 0xE000
#define OAM_ADDR 0xFE00
#define UNUSED_ADDR 0xFEA0
#define IO_ADDR 0xFF00
#define HRAM_ADDR 0xFF80
#define INTR_EN_ADDR 0xFFFF

/* Cart section sizes */
#define ROM_BANK_SIZE 0x4000
#define WRAM_BANK_SIZE 0x1000
#define CRAM_BANK_SIZE 0x2000
#define VRAM_BANK_SIZE 0x2000

/* STAT register masks */
#define STAT_LYC_INTR 0x40
#define STAT_MODE_2_INTR 0x20
#define STAT_MODE_1_INTR 0x10
#define STAT_MODE_0_INTR 0x08
#define STAT_LYC_COINC 0x04
#define STAT_MODE 0x03
#define STAT_USER_BITS 0xF8

/* LCDC control masks */
#define LCDC_ENABLE 0x80
#define LCDC_WINDOW_MAP 0x40
#define LCDC_WINDOW_ENABLE 0x20
#define LCDC_TILE_SELECT 0x10
#define LCDC_BG_MAP 0x08
#define LCDC_OBJ_SIZE 0x04
#define LCDC_OBJ_ENABLE 0x02
#define LCDC_BG_ENABLE 0x01

/* VRAM Locations */
#define VRAM_TILES_1 (0x8000 - VRAM_ADDR)
#define VRAM_TILES_2 (0x8800 - VRAM_ADDR)
#define VRAM_BMAP_1 (0x9800 - VRAM_ADDR)
#define VRAM_BMAP_2 (0x9C00 - VRAM_ADDR)
#define VRAM_TILES_3 (0x8000 - VRAM_ADDR + VRAM_BANK_SIZE)
#define VRAM_TILES_4 (0x8800 - VRAM_ADDR + VRAM_BANK_SIZE)

/* Internal function used to read bytes. */
uint8_t mmu_read(struct gb_s *gb, const uint_fast16_t addr);

/* Internal function used to write bytes */
void mmu_write(struct gb_s *gb, const uint_fast16_t addr, const uint8_t val);
