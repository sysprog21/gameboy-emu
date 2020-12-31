#ifndef GAMEBOYEMU_CORE
#define GAMEBOYEMU_CORE

#include <stdint.h>
#include <stddef.h>

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

/* Interrupt masks */
#define VBLANK_INTR 0x01
#define LCDC_INTR 0x02
#define TIMER_INTR 0x04
#define SERIAL_INTR 0x08
#define CONTROL_INTR 0x10
#define ANY_INTR 0x1F

/* Memory section sizes for DMG */
#define WRAM_SIZE 0x2000
#define VRAM_SIZE 0x2000
#define HRAM_SIZE 0x0100
#define OAM_SIZE 0x00A0

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

/* LCD characteristics */
#define LCD_LINE_CYCLES 456
#define LCD_MODE_0_CYCLES 0
#define LCD_MODE_2_CYCLES 204
#define LCD_MODE_3_CYCLES 284
#define LCD_VERT_LINES 154
#define LCD_WIDTH 160
#define LCD_HEIGHT 144

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
#endif
