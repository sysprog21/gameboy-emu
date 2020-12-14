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

/* SERIAL SC register masks. */
#define SERIAL_SC_TX_START 0x80
#define SERIAL_SC_CLOCK_SRC 0x01

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

/* Internal function used to read bytes. */
uint8_t __gb_read(struct gb_s *gb, const uint_fast16_t addr);

/* Internal function used to write bytes */
void __gb_write(struct gb_s *gb, const uint_fast16_t addr, const uint8_t val);

uint8_t __gb_execute_cb(struct gb_s *gb);

void __gb_step_cpu(struct gb_s *gb);

#if ENABLE_LCD
void __gb_draw_line(struct gb_s *gb);
#endif
/*
struct cpu_instr_s {
    const char mnemonic;
    uint8_t length;
    bool signed_operand;
    bool extended;
    void *execute;
}
*/

typedef struct cpu_instr_s {
    enum {
        NOP,
        LD16,
        LD,
        INC16,
        INC,
        DEC16,
        DEC,
        RLC,
        ADD16,
        ADD,
        RRC,
        STOP,
        RL,
        JR,
        RR,
        DAA,
        CPL,
        SCF,
        CCF,
        HALT,
        ADC,
        SUB,
        SBC,
        AND,
        XOR,
        OR,
        CP,
        RET,
        POP,
        JP,
        CALL,
        PUSH,
        RST,
        RETI,
        DI,
        EI,
        SLA,
        SRA,
        SWAP,
        SRL,
        BIT,
        RES,
        SET,
        JP_TARGET,
        JP_BWD,
        JP_FWD,
        ERROR
    } opcode;
    enum {
        NONE,
        REG_A,
        REG_B,
        REG_C,
        REG_D,
        REG_E,
        REG_H,
        REG_L,
        REG_AF,
        REG_BC,
        REG_DE,
        REG_HL,
        REG_SP,
        IMM8,
        IMM16 /* = 14 */,
        MEM_BC,
        MEM_DE,
        MEM_HL,
        MEM_16,
        MEM_8,
        MEM_C,
        MEM_INC_DE,
        MEM_INC_HL,
        MEM_DEC_HL /*=22*/,
        CC_Z,
        CC_C,
        CC_NZ,
        CC_NC /*=26*/,
        MEM_0x00,
        MEM_0x08,
        MEM_0x10,
        MEM_0x18,
        MEM_0x20,
        MEM_0x28,
        MEM_0x30,
        MEM_0x38,
        BIT_0,
        BIT_1,
        BIT_2,
        BIT_3,
        BIT_4,
        BIT_5,
        BIT_6,
        BIT_7,
        TARGET_1,
        TARGET_2,
        WAIT_LY,
        WAIT_STAT3
    } op1,
        op2;
    uint8_t *args;
    uint16_t address;
    uint8_t bytes;
    uint8_t cycles, alt_cycles;
    enum {
        INST_FLAG_NONE = 0x00,
        INST_FLAG_PRESERVE_CC = 0x01,
        INST_FLAG_PERS_WRITE = 0x02,
        INST_FLAG_USES_CC = 0x04,
        INST_FLAG_AFFECTS_CC = 0x08,
        INST_FLAG_ENDS_BLOCK = 0x10,
        INST_FLAG_SAVE_CC = 0x20,
        INST_FLAG_RESTORE_CC = 0x40
    } flags;
} cpu_instr;
