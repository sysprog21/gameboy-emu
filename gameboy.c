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