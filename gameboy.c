#include "gameboy.h"

void gb_run_frame(struct gb_s *gb)
{
    gb->gb_frame = 0;

    while (!gb->gb_frame)
        __gb_step_cpu(gb);
}

/* Gets the size of the save file required for the ROM. */
uint_fast32_t gb_get_save_size(struct gb_s *gb)
{
    const uint_fast16_t ram_size_location = 0x0149;
    const uint_fast32_t ram_sizes[] = {0x00, 0x800, 0x2000, 0x8000, 0x20000};
    uint8_t ram_size = gb->gb_rom_read(gb, ram_size_location);
    return ram_sizes[ram_size];
}

/* Set the function used to handle serial transfer in the front-end. This is
 * optional.
 * gb_serial_transfer takes a byte to transmit and returns the received byte. If
 * no cable is connected to the console, return 0xFF.
 */
void gb_init_serial(struct gb_s *gb,
                    void (*gb_serial_tx)(struct gb_s *, const uint8_t),
                    gb_serial_rx_ret_t (*gb_serial_rx)(struct gb_s *,
                                                       uint8_t *))
{
    gb->gb_serial_tx = gb_serial_tx;
    gb->gb_serial_rx = gb_serial_rx;
}

uint8_t gb_color_hash(struct gb_s *gb)
{
#define ROM_TITLE_START_ADDR 0x0134
#define ROM_TITLE_END_ADDR 0x0143

    uint8_t x = 0;

    for (uint16_t i = ROM_TITLE_START_ADDR; i <= ROM_TITLE_END_ADDR; i++)
        x += gb->gb_rom_read(gb, i);

    return x;
}

/* Resets the context, and initializes startup values. */
void gb_reset(struct gb_s *gb)
{
    gb->gb_halt = 0;
    gb->gb_ime = 1;
    gb->gb_bios_enable = 0;
    gb->lcd_mode = LCD_HBLANK;

    /* Initialize MBC values. */
    gb->selected_rom_bank = 1;
    gb->cart_ram_bank = 0;
    gb->enable_cart_ram = 0;
    gb->cart_mode_select = 0;

    /* Initialize CPU registers as though a DMG. */
    gb->cpu_reg.af = 0x01B0;
    gb->cpu_reg.bc = 0x0013;
    gb->cpu_reg.de = 0x00D8;
    gb->cpu_reg.hl = 0x014D;
    gb->cpu_reg.sp = 0xFFFE;
    /* TODO: Add BIOS support. */
    gb->cpu_reg.pc = 0x0100;

    gb->counter.lcd_count = 0;
    gb->counter.div_count = 0;
    gb->counter.tima_count = 0;
    gb->counter.serial_count = 0;

    gb->gb_reg.TIMA = 0x00;
    gb->gb_reg.TMA = 0x00;
    gb->gb_reg.TAC = 0xF8;
    gb->gb_reg.DIV = 0xAC;

    gb->gb_reg.IF = 0xE1;

    gb->gb_reg.LCDC = 0x91;
    gb->gb_reg.SCY = 0x00;
    gb->gb_reg.SCX = 0x00;
    gb->gb_reg.LYC = 0x00;

    /* Appease valgrind for invalid reads and unconditional jumps. */
    gb->gb_reg.SC = 0x7E;
    gb->gb_reg.STAT = 0;
    gb->gb_reg.LY = 0;

    __gb_write(gb, 0xFF47, 0xFC); /* BGP */
    __gb_write(gb, 0xFF48, 0xFF); /* OBJP0 */
    __gb_write(gb, 0xFF49, 0x0F); /* OBJP1 */
    gb->gb_reg.WY = 0x00;
    gb->gb_reg.WX = 0x00;
    gb->gb_reg.IE = 0x00;

    gb->direct.joypad = 0xFF;
    gb->gb_reg.P1 = 0xCF;
}

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
    void *priv)
{
    const uint16_t mbc_location = 0x0147;
    const uint16_t bank_count_location = 0x0148;
    const uint16_t ram_size_location = 0x0149;

    /* Table for cartridge type (MBC). -1 if invalid.
     * TODO: MMM01 is untested.
     * TODO: MBC6 is untested.
     * TODO: MBC7 is unsupported.
     * TODO: POCKET CAMERA is unsupported.
     * TODO: BANDAI TAMA5 is unsupported.
     * TODO: HuC3 is unsupported.
     * TODO: HuC1 is unsupported.
     **/
    const uint8_t cart_mbc[] = {0,  1,  1,  1,  -1, 2, 2, -1, 0, 0,  -1,
                                0,  0,  0,  -1, 3,  3, 3, 3,  3, -1, -1,
                                -1, -1, -1, 5,  5,  5, 5, 5,  5, -1};
    const uint8_t cart_ram[] = {0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
                                1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0};
    const uint16_t num_rom_banks[] = {
        2, 4, 8,  16, 32, 64, 128, 256, 512, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0,  0,  0,  0,  0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0,  0,  0,  0,  0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0,  0,  0,  0,  0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 72, 80, 96, 0,  0,   0,   0,   0, 0, 0, 0, 0, 0, 0};
    const uint8_t num_ram_banks[] = {0, 1, 1, 4, 16, 8};

    gb->gb_rom_read = gb_rom_read;
    gb->gb_cart_ram_read = gb_cart_ram_read;
    gb->gb_cart_ram_write = gb_cart_ram_write;
    gb->gb_error = gb_error;
    gb->direct.priv = priv;

    /* Initialize serial transfer function to NULL. If the front-end does
     * not provide serial support, it will emulate no cable connected
     * automatically.
     */
    gb->gb_serial_tx = NULL;
    gb->gb_serial_rx = NULL;

    /* Check valid ROM using checksum value. */
    {
        uint8_t x = 0;

        for (uint16_t i = 0x0134; i <= 0x014C; i++)
            x = x - gb->gb_rom_read(gb, i) - 1;

        if (x != gb->gb_rom_read(gb, ROM_HEADER_CHECKSUM_LOC))
            return GB_INIT_INVALID_CHECKSUM;
    }

    /* Check if cartridge type is supported, and set MBC type. */
    {
        const uint8_t mbc_value = gb->gb_rom_read(gb, mbc_location);

        if (mbc_value > sizeof(cart_mbc) - 1 ||
            (gb->mbc = cart_mbc[gb->gb_rom_read(gb, mbc_location)]) == 255u)
            return GB_INIT_CARTRIDGE_UNSUPPORTED;
    }

    gb->cart_ram = cart_ram[gb->gb_rom_read(gb, mbc_location)];
    gb->num_rom_banks = num_rom_banks[gb->gb_rom_read(gb, bank_count_location)];
    gb->num_ram_banks = num_ram_banks[gb->gb_rom_read(gb, ram_size_location)];

    gb->display.lcd_draw_line = NULL;

    gb_reset(gb);

    return GB_INIT_NO_ERROR;
}