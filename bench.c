#define ENABLE_SOUND 0
#define ENABLE_LCD 0
#include "gameboy.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

struct priv_t {
    /* Pointer to allocated memory holding GB file */
    uint8_t *rom;
};

/* Return byte from blarrg test ROM */
uint8_t gb_rom_read(struct gb_s *gb, const uint32_t addr)
{
    const struct priv_t *const p = gb->direct.priv;
    return p->rom[addr];
}

/* Ignore all errors */
void gb_error(struct gb_s *gb, const gb_error_t gb_err, const uint16_t val)
{
    (void) gb;
    (void) gb_err;
    (void) val;
    return;
}

/* Return a pointer to allocated space containing the ROM. Must be freed. */
uint8_t *read_rom_to_ram(const char *file_name)
{
    FILE *rom_file = fopen(file_name, "rb");
    size_t rom_size;
    uint8_t *rom = NULL;

    if (!rom_file)
        return NULL;

    fseek(rom_file, 0, SEEK_END);
    rom_size = ftell(rom_file);
    rewind(rom_file);
    rom = malloc(rom_size);

    if (fread(rom, sizeof(uint8_t), rom_size, rom_file) != rom_size) {
        free(rom);
        fclose(rom_file);
        return NULL;
    }

    fclose(rom_file);
    return rom;
}

int main(int argc, char **argv)
{
    uint32_t bench_ticks_total = 0;
    uint32_t bench_fps_total = 0;
    struct priv_t priv = {.rom = NULL};

    if (argc != 2) {
        printf("%s ROM\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Copy input ROM file to allocated memory */
    if ((priv.rom = read_rom_to_ram(argv[1])) == NULL) {
        printf("Error(%d): %s\n", __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }

    puts("Benchmark started");

    for (unsigned int i = 0; i < 5; i++) {
        /* Start benchmark */
        struct gb_s gb;
        const uint_fast64_t start_time = (uint_fast64_t) clock();
        uint_fast64_t bench_ticks;
        uint_fast64_t bench_fps;
        uint_fast64_t frames_to_run = 60 * 60 * 2; /* 2 Minutes of frames */
        const uint_fast64_t frames = frames_to_run;
        int ret;

        /* Initialize context */
        ret = gb_init(&gb, &gb_rom_read, &gb_error, &priv);

        if (ret != GB_INIT_NO_ERROR) {
            printf("Error(%d): %d\n", __LINE__, ret);
            exit(EXIT_FAILURE);
        }

        /* Step CPU until test is complete */
        do {
            gb_run_frame(&gb);
        } while (--frames_to_run);

        /* End benchmark */
        bench_ticks = (uint_fast64_t) clock() - start_time;
        bench_ticks_total += bench_ticks;

        bench_fps = frames / ((double) bench_ticks / CLOCKS_PER_SEC);
        bench_fps_total += bench_fps;

        printf("Benchmark %i: %" PRIu64 "\tFPS: %" PRIu64 "\n", i, bench_ticks,
               bench_fps);
    }

    printf("Average    : %" PRIu32 "\tFPS: %" PRIu32 "\n",
           bench_ticks_total / 5, bench_fps_total / 5);

    return 0;
}
