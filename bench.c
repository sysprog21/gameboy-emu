#define ENABLE_SOUND 0
#define ENABLE_LCD 0
#include "gameboy.h"

#if defined(__linux__)
#include "prof.h"
#elif defined(__MACH__)
#include <mach/mach_time.h>
#include <math.h>
#define PROF_START(...)                                    \
    static mach_timebase_info_data_t timebase;             \
    mach_timebase_info(&timebase);                         \
    double conversion_factor =                             \
        (double) timebase.numer / (double) timebase.denom; \
    uint64_t start = mach_absolute_time()
#define PROF_STDOUT(...)                                                 \
    uint64_t elapsed = mach_absolute_time() - start;                     \
    double duration = (double) elapsed * conversion_factor / __exp10(9); \
    printf("elapsed time: %lf s\n", duration)
#else
#error "Unsupported platform"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Return byte from Blarrg test ROM, written by Shay Green <gblargg@gmail.com>
 * https://github.com/retrio/gb-test-roms/tree/master/cpu_instrs
 */
uint8_t gb_rom_read(struct gb_s *gb, const uint_fast32_t addr)
{
    /* Import the test ROM */
#include "build/cpu_instrs.h"
    (void) gb;
    return cpu_instrs_gb[addr];
}

/* Ignore cart RAM writes */
void gb_cart_ram_write(struct gb_s *gb,
                       const uint_fast32_t addr,
                       const uint8_t val)
{
    (void) gb;
    (void) addr;
    (void) val;
    return;
}

/* Ignore cart RAM reads */
uint8_t gb_cart_ram_read(struct gb_s *gb, const uint_fast32_t addr)
{
    (void) gb;
    (void) addr;
    return 0xFF;
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

    ret = gb_init(&gb, &gb_rom_read, &gb_cart_ram_read, &gb_cart_ram_write,
                  &gb_error, NULL);

    if (ret != GB_INIT_NO_ERROR) {
        printf("Error: %d\n", ret);
        exit(EXIT_FAILURE);
    }

    /* Step CPU until test is complete */
    while (gb.cpu_reg.pc != pc_end) {
        printf("%x\n",gb.cpu_reg.pc);
        gb_run_frame(&gb);
    }

    PROF_STDOUT();

    return 0;
}
