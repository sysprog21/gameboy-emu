#define ENABLE_SOUND 0
#define ENABLE_LCD 0
#include "gameboy.h"

#if defined(__linux__)
#include "prof.h"
#else
#define PROF_START(...)
#define PROF_STDOUT(...)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Return byte from blarrg test ROM */
uint8_t gb_rom_read(struct gb_s *gb, const uint_fast32_t addr)
{
    /* Import the test ROM */
#include "tests/cpu_instrs.h"
    (void) gb;
    return cpu_instrs_gb[addr];
}

/* Ignore all errors */
void gb_error(struct gb_s *gb, const gb_error_t gb_err, const uint16_t val)
{
    (void) gb;
    (void) gb_err;
    (void) val;
    return;
}

int main(void)
{
    const unsigned short pc_end = 0x06F1; /* Test ends when PC is this value. */
    struct gb_s gb;
    int ret;

    PROF_START();

    ret = gb_init(&gb, &gb_rom_read, &gb_error, NULL);

    if (ret != GB_INIT_NO_ERROR) {
        printf("Error: %d\n", ret);
        exit(EXIT_FAILURE);
    }

    /* Step CPU until test is complete */
    while (gb.cpu_reg.pc != pc_end)
        gb_run_frame(&gb);

    PROF_STDOUT();

    return 0;
}
