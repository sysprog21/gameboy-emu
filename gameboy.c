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