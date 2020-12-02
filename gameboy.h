/**
 * MIT License
 *
 * Copyright (c) 2020 National Cheng Kung University, Taiwan.
 * Copyright (c) 2018 Mahyar Koshkouei
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include <stdint.h>
#include <time.h>

/**
 * When audio_read() and audio_write() functions are provided, define this
 * to a non-zero value before including gameboy.h in order for these functions
 * to be used.
 */
#ifndef ENABLE_SOUND
#define ENABLE_SOUND 0
#endif

/* Enable LCD drawing. On by default. May be turned off for testing. */
#ifndef ENABLE_LCD
#define ENABLE_LCD 1
#endif

/* Memory section sizes for DMG */
#define WRAM_SIZE 0x2000
#define VRAM_SIZE 0x2000
#define HRAM_SIZE 0x0100
#define OAM_SIZE 0x00A0

/* DIV Register is incremented at rate of 16384Hz.
 * 4194304 / 16384 = 256 clock cycles for one increment.
 */
#define DIV_CYCLES 256

/* Serial clock locked to 8192Hz on DMG.
 * 4194304 / (8192 / 8) = 4096 clock cycles for sending 1 byte.
 */
#define SERIAL_CYCLES 4096

/* Calculating VSYNC. */
#define DMG_CLOCK_FREQ 4194304.0
#define SCREEN_REFRESH_CYCLES 70224.0
#define VERTICAL_SYNC (DMG_CLOCK_FREQ / SCREEN_REFRESH_CYCLES)

/* LCD characteristics */
#define LCD_LINE_CYCLES 456
#define LCD_MODE_0_CYCLES 0
#define LCD_MODE_2_CYCLES 204
#define LCD_MODE_3_CYCLES 284
#define LCD_VERT_LINES 154
#define LCD_WIDTH 160
#define LCD_HEIGHT 144

#define ROM_HEADER_CHECKSUM_LOC 0x014D

#ifndef MIN
/* Assume neither a nor b is a statement expression of increment, decrement, or
 * assignment.
 */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

struct cpu_registers_s {
    /* Combine A and F registers */
    union {
        struct {
            /* Define specific bits of Flag register */
            union {
                struct {
                    unsigned unused : 4;
                    unsigned c : 1; /* Carry flag */
                    unsigned h : 1; /* Half carry flag */
                    unsigned n : 1; /* Add/sub flag */
                    unsigned z : 1; /* Zero flag */
                } f_bits;
                uint8_t f;
            };
            uint8_t a;
        };
        uint16_t af;
    };

    union {
        struct {
            uint8_t c;
            uint8_t b;
        };
        uint16_t bc;
    };

    union {
        struct {
            uint8_t e;
            uint8_t d;
        };
        uint16_t de;
    };

    union {
        struct {
            uint8_t l;
            uint8_t h;
        };
        uint16_t hl;
    };

    uint16_t sp; /* Stack pointer */
    uint16_t pc; /* Program counter */
};

struct count_s {
    uint_fast16_t lcd_count;    /* LCD Timing */
    uint_fast16_t div_count;    /* Divider Register Counter */
    uint_fast16_t tima_count;   /* Timer Counter */
    uint_fast16_t serial_count; /* Serial Counter */
};

struct gb_registers_s {
    /* TODO: Sort variables in address order. */
    /* Timing */
    uint8_t TIMA, TMA, DIV;
    union {
        struct {
            unsigned tac_rate : 2;   /* Input clock select */
            unsigned tac_enable : 1; /* Timer enable */
            unsigned unused : 5;
        };
        uint8_t TAC;
    };

    /* LCD */
    uint8_t LCDC;
    uint8_t STAT;
    uint8_t SCY;
    uint8_t SCX;
    uint8_t LY;
    uint8_t LYC;
    uint8_t DMA;
    uint8_t BGP;
    uint8_t OBP0;
    uint8_t OBP1;
    uint8_t WY;
    uint8_t WX;

    /* Joypad info. */
    uint8_t P1;

    /* Serial data. */
    uint8_t SB;
    uint8_t SC;

    /* Interrupt flag. */
    uint8_t IF;

    /* Interrupt enable. */
    uint8_t IE;
};

#if ENABLE_LCD
/* Bit mask for the shade of pixel to display */
#define LCD_COLOR 0x03
/**
 * Bit mask for whether a pixel is OBJ0, OBJ1, or BG. Each may have a different
 * palette when playing a DMG game on CGB.
 */
#define LCD_PALETTE_OBJ 0x10
#define LCD_PALETTE_BG 0x20
/* Bit mask for the two bits listed above.
 * LCD_PALETTE_ALL == 0b00 --> OBJ0
 * LCD_PALETTE_ALL == 0b01 --> OBJ1
 * LCD_PALETTE_ALL == 0b10 --> BG
 * LCD_PALETTE_ALL == 0b11 --> NOT POSSIBLE
 */
#define LCD_PALETTE_ALL 0x30
#endif

/* Errors that may occur during emulation */
typedef enum {
    GB_UNKNOWN_ERROR,
    GB_INVALID_OPCODE,
    GB_INVALID_READ,
    GB_INVALID_WRITE
} gb_error_t;

/* Errors that may occur during initialization */
typedef enum {
    GB_INIT_NO_ERROR,
    GB_INIT_CARTRIDGE_UNSUPPORTED,
    GB_INIT_INVALID_CHECKSUM
} gb_init_error_t;

/* Return codes for serial receive function, mainly for clarity */
typedef enum {
    GB_SERIAL_RX_SUCCESS = 0,
    GB_SERIAL_RX_NO_CONNECTION = 1
} gb_serial_rx_ret_t;

/* Emulator context.
 *
 * Only values within the "direct" struct may be modified directly by the
 * front-end implementation. Other variables must not be modified.
 */
struct gb_s {
    /* Return byte from ROM at given address.
     *
     * \param gb_s  emulator context
     * \param addr  address
     * \return      byte at address in ROM
     */
    uint8_t (*gb_rom_read)(struct gb_s *, const uint_fast32_t addr);

    /* Return byte from cart RAM at given address.
     *
     * \param gb_s  emulator context
     * \param addr  address
     * \return      byte at address in RAM
     */
    uint8_t (*gb_cart_ram_read)(struct gb_s *, const uint_fast32_t addr);

    /* Write byte to cart RAM at given address.
     *
     * \param gb_s  emulator context
     * \param addr  address
     * \param val   value to write to address in RAM
     */
    void (*gb_cart_ram_write)(struct gb_s *,
                              const uint_fast32_t addr,
                              const uint8_t val);

    /* Notify front-end of error.
     *
     * \param gb_s        emulator context
     * \param gb_error_e  error code
     * \param val         arbitrary value related to error
     */
    void (*gb_error)(struct gb_s *, const gb_error_t, const uint16_t val);

    /* Transmit one byte and return the received byte. */
    void (*gb_serial_tx)(struct gb_s *, const uint8_t tx);
    gb_serial_rx_ret_t (*gb_serial_rx)(struct gb_s *, uint8_t *rx);

    struct {
        unsigned gb_halt : 1;
        unsigned gb_ime : 1;
        unsigned gb_bios_enable : 1;
        unsigned gb_frame : 1; /* New frame drawn. */

#define LCD_HBLANK 0
#define LCD_VBLANK 1
#define LCD_SEARCH_OAM 2
#define LCD_TRANSFER 3
        unsigned lcd_mode : 2;
    };

    /* Cartridge information:
     * Memory Bank Controller (MBC) type */
    uint8_t mbc;
    /* Whether the MBC has internal RAM */
    uint8_t cart_ram;
    /* Number of ROM banks in cartridge */
    uint16_t num_rom_banks;
    /* Number of RAM banks in cartridge */
    uint8_t num_ram_banks;

    uint16_t selected_rom_bank;
    /* WRAM and VRAM bank selection not available */
    uint8_t cart_ram_bank;
    uint8_t enable_cart_ram;
    /* Cartridge ROM/RAM mode select */
    uint8_t cart_mode_select;
    union {
        struct {
            uint8_t sec;
            uint8_t min;
            uint8_t hour;
            uint8_t yday;
            uint8_t high;
        } rtc_bits;
        uint8_t cart_rtc[5];
    };

    struct cpu_registers_s cpu_reg;
    struct gb_registers_s gb_reg;
    struct count_s counter;

    /* TODO: Allow implementation to allocate WRAM, VRAM and Frame Buffer */
    uint8_t wram[WRAM_SIZE];
    uint8_t vram[VRAM_SIZE];
    uint8_t hram[HRAM_SIZE];
    uint8_t oam[OAM_SIZE];

    struct {
        /* Draw line on screen.
         *
         * \param gb_s    emulator context
         * \param pixels  The 160 pixels to draw.
         *                Bits 1-0 are the color to draw.
         *                Bits 5-4 are the palette, where:
         *                OBJ0 = 0b00,
         *                OBJ1 = 0b01,
         *                BG = 0b10
         *                Other bits are undefined.
         *                Bits 5-4 are only required by front-ends which want
         *                to use a different color for different object
         *                palettes. This is what the Game Boy Color (CGB) does
         *                to DMG games.
         * \param line    Line to draw pixels on. This is
         * guaranteed to be between 0-144 inclusive.
         */
        void (*lcd_draw_line)(struct gb_s *gb,
                              const uint8_t *pixels,
                              const uint_fast8_t line);

        /* Palettes */
        uint8_t bg_palette[4];
        uint8_t sp_palette[8];

        uint8_t window_clear;
        uint8_t WY;

        /* Only support 30fps frame skip */
        unsigned frame_skip_count : 1;
        unsigned interlace_count : 1;
    } display;

    /* Variables that may be modified directly by the front-end.
     * This method seems to be easier and possibly less overhead than calling
     * a function to modify these variables each time.
     *
     * None of this is thread-safe.
     */
    struct {
        /* Set to enable interlacing. Interlacing will start immediately
         * (at the next line drawing).
         */
        unsigned interlace : 1;
        unsigned frame_skip : 1;

        union {
            struct {
                unsigned a : 1;
                unsigned b : 1;
                unsigned select : 1;
                unsigned start : 1;
                unsigned right : 1;
                unsigned left : 1;
                unsigned up : 1;
                unsigned down : 1;
            } joypad_bits;
            uint8_t joypad;
        };

        /* Implementation defined data. Set to NULL if not required. */
        void *priv;
    } direct;
};

/* Tick the internal RTC by one second */
void gb_tick_rtc(struct gb_s *gb);

/* Set initial values in RTC.
 * Should be called after gb_init().
 */
void gb_set_rtc(struct gb_s *gb, const struct tm *const time);

void gb_run_frame(struct gb_s *gb);

/* Gets the size of the save file required for the ROM. */
uint_fast32_t gb_get_save_size(struct gb_s *gb);

/* Set the function used to handle serial transfer in the front-end. This is
 * optional.
 * gb_serial_transfer takes a byte to transmit and returns the received byte. If
 * no cable is connected to the console, return 0xFF.
 */
void gb_init_serial(struct gb_s *gb,
                    void (*gb_serial_tx)(struct gb_s *, const uint8_t),
                    gb_serial_rx_ret_t (*gb_serial_rx)(struct gb_s *,
                                                       uint8_t *));

uint8_t gb_color_hash(struct gb_s *gb);

/* Resets the context, and initializes startup values. */
void gb_reset(struct gb_s *gb);

/* Initialize the emulator context. gb_reset() is also called to initialize
 * the CPU.
 */
gb_init_error_t gb_init(
    struct gb_s *gb,
    uint8_t (*gb_rom_read)(struct gb_s *, const uint_fast32_t),
    uint8_t (*gb_cart_ram_read)(struct gb_s *, const uint_fast32_t),
    void (*gb_cart_ram_write)(struct gb_s *,
                              const uint_fast32_t,
                              const uint8_t),
    void (*gb_error)(struct gb_s *, const gb_error_t, const uint16_t),
    void *priv);

/* Return the title of ROM.
 *
 * \param gb		Initialized context.
 * \param title_str	Allocated string at least 16 characters.
 * \returns		Pointer to start of string, null terminated.
 */
const char *gb_get_rom_name(struct gb_s *gb, char *title_str);

#if ENABLE_LCD
void gb_init_lcd(struct gb_s *gb,
                 void (*lcd_draw_line)(struct gb_s *gb,
                                       const uint8_t *pixels,
                                       const uint_fast8_t line));
#endif
