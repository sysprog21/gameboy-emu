#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#if defined(ENABLE_SOUND)
#include "apu.h"
#endif

#define ENABLE_LCD 1
#include "gameboy.h"

struct priv_t {
    /* Pointer to allocated memory holding GB file */
    uint8_t *rom;
    /* Pointer to allocated memory holding save file */
    uint8_t *cart_ram;

    /* Color palette for each BG, OBJ0, and OBJ1 */
    uint16_t selected_palette[3][4];
    uint16_t fb[LCD_HEIGHT][LCD_WIDTH];
};

/* Return a byte from the ROM file at the given address */
uint8_t gb_rom_read(struct gb_s *gb, const uint_fast32_t addr)
{
    const struct priv_t *const p = gb->direct.priv;
    return p->rom[addr];
}

/* Return a byte from the cartridge RAM at the given address */
uint8_t gb_cart_ram_read(struct gb_s *gb, const uint_fast32_t addr)
{
    const struct priv_t *const p = gb->direct.priv;
    return p->cart_ram[addr];
}

/* Write a given byte to the cartridge RAM at the given address */
void gb_cart_ram_write(struct gb_s *gb,
                       const uint_fast32_t addr,
                       const uint8_t val)
{
    const struct priv_t *const p = gb->direct.priv;
    p->cart_ram[addr] = val;
}

/* Return a pointer to allocated space containing the ROM. Must be freed. */
uint8_t *read_rom_to_ram(const char *file_name)
{
    FILE *rom_file = fopen(file_name, "rb");
    if (!rom_file)
        return NULL;

    fseek(rom_file, 0, SEEK_END);
    size_t rom_size = ftell(rom_file);
    rewind(rom_file);
    uint8_t *rom = malloc(rom_size);

    if (fread(rom, sizeof(uint8_t), rom_size, rom_file) != rom_size) {
        free(rom);
        fclose(rom_file);
        return NULL;
    }

    fclose(rom_file);
    return rom;
}

void read_cart_ram_file(const char *save_file_name,
                        uint8_t **dest,
                        const size_t len)
{
    /* If save file is not required */
    if (len == 0) {
        *dest = NULL;
        return;
    }

    /* Allocate enough memory to hold save file */
    if ((*dest = malloc(len)) == NULL) {
        printf("Fail to allocate memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    FILE *f = fopen(save_file_name, "rb");
    /* It does not matter if the save file does not exist. We initialize the
     * save memory allocated above. The save file will be created on exit.
     */
    if (!f) {
        memset(*dest, 0xFF, len);
        return;
    }

    /* Read save file to allocated memory */
    fread(*dest, sizeof(uint8_t), len, f);
    fclose(f);
}

void write_cart_ram_file(const char *save_file_name,
                         uint8_t **dest,
                         const size_t len)
{
    if (!len || !*dest)
        return;

    FILE *f = fopen(save_file_name, "wb");
    if (!f) {
        printf("Unable to open save file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Record save file. */
    fwrite(*dest, 1, len, f);
    fclose(f);
}

/* Handle an error reported by the emulator. The emulator context may be used
 * to better understand why the error given in gb_err was reported.
 */
void gb_error(struct gb_s *gb, const gb_error_t gb_err, const uint16_t val)
{
    struct priv_t *priv = gb->direct.priv;

    switch (gb_err) {
    case GB_INVALID_OPCODE:
        /* compensate for the post-increment in the __gb_step_cpu function. */
        fprintf(stdout, "Invalid opcode %#04x at PC: %#06x, SP: %#06x\n", val,
                gb->cpu_reg.pc - 1, gb->cpu_reg.sp);
        break;

    /* Ignoring non fatal errors */
    case GB_INVALID_WRITE:
    case GB_INVALID_READ:
        return;

    default:
        printf("Unknown error");
        break;
    }

    /* FIXME: introduce canonical approaches to interact */
    fprintf(stderr, "Error. Press q to exit, or any other key to continue.");
    if (getchar() == 'q') {
        free(priv->rom);
        exit(EXIT_FAILURE);
    }

    return;
}

/* Assign color palette to the game using a given game checksum */
void auto_assign_palette(struct priv_t *priv, uint8_t game_checksum)
{
    size_t palette_bytes = 3 * 4 * sizeof(uint16_t);

    switch (game_checksum) {
    /* Balloon Kid and Tetris Blast */
    case 0x71:
    case 0xFF: {
        const uint16_t palette[3][4] = {
            {0x7FFF, 0x7E60, 0x7C00, 0x0000}, /* OBJ0 */
            {0x7FFF, 0x7E60, 0x7C00, 0x0000}, /* OBJ1 */
            {0x7FFF, 0x7E60, 0x7C00, 0x0000}  /* BG */
        };
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* Pokemon Yellow and Tetris */
    case 0x15:
    case 0xDB:
    case 0x95: { /* Not officially */
        const uint16_t palette[3][4] = {
            {0x7FFF, 0x7FE0, 0x7C00, 0x0000}, /* OBJ0 */
            {0x7FFF, 0x7FE0, 0x7C00, 0x0000}, /* OBJ1 */
            {0x7FFF, 0x7FE0, 0x7C00, 0x0000}  /* BG */
        };
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* Donkey Kong */
    case 0x19: {
        const uint16_t palette[3][4] = {
            {0x7FFF, 0x7E10, 0x48E7, 0x0000}, /* OBJ0 */
            {0x7FFF, 0x7E10, 0x48E7, 0x0000}, /* OBJ1 */
            {0x7FFF, 0x7E60, 0x7C00, 0x0000}  /* BG */
        };
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* Pokemon Blue */
    case 0x61:
    case 0x45:

    /* Pokemon Blue Star */
    case 0xD8: {
        const uint16_t palette[3][4] = {
            {0x7FFF, 0x7E10, 0x48E7, 0x0000}, /* OBJ0 */
            {0x7FFF, 0x329F, 0x001F, 0x0000}, /* OBJ1 */
            {0x7FFF, 0x329F, 0x001F, 0x0000}  /* BG */
        };
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* Pokemon Red */
    case 0x14: {
        const uint16_t palette[3][4] = {
            {0x7FFF, 0x3FE6, 0x0200, 0x0000}, /* OBJ0 */
            {0x7FFF, 0x7E10, 0x48E7, 0x0000}, /* OBJ1 */
            {0x7FFF, 0x7E10, 0x48E7, 0x0000}  /* BG */
        };
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* Pokemon Red Star */
    case 0x8B: {
        const uint16_t palette[3][4] = {
            {0x7FFF, 0x7E10, 0x48E7, 0x0000}, /* OBJ0 */
            {0x7FFF, 0x329F, 0x001F, 0x0000}, /* OBJ1 */
            {0x7FFF, 0x3FE6, 0x0200, 0x0000}  /* BG */
        };
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* Kirby */
    case 0x27:
    case 0x49:
    case 0x5C:
    case 0xB3: {
        const uint16_t palette[3][4] = {
            {0x7D8A, 0x6800, 0x3000, 0x0000}, /* OBJ0 */
            {0x001F, 0x7FFF, 0x7FEF, 0x021F}, /* OBJ1 */
            {0x527F, 0x7FE0, 0x0180, 0x0000}  /* BG */
        };
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* Donkey Kong Land [1/2/III] */
    case 0x18:
    case 0x6A:
    case 0x4B:
    case 0x6B: {
        const uint16_t palette[3][4] = {
            {0x7F08, 0x7F40, 0x48E0, 0x2400}, /* OBJ0 */
            {0x7FFF, 0x2EFF, 0x7C00, 0x001F}, /* OBJ1 */
            {0x7FFF, 0x463B, 0x2951, 0x0000}  /* BG */
        };
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* Link's Awakening */
    case 0x70: {
        const uint16_t palette[3][4] = {
            {0x7FFF, 0x03E0, 0x1A00, 0x0120}, /* OBJ0 */
            {0x7FFF, 0x329F, 0x001F, 0x001F}, /* OBJ1 */
            {0x7FFF, 0x7E10, 0x48E7, 0x0000}  /* BG */
        };
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* Super Mario Land */
    case 0x46: {
        const uint16_t palette[3][4] = {{0x7FFF, 0x7EAC, 0x40C0, 0x0000},
                                        {0x7FFF, 0x7EAC, 0x40C0, 0x0000},
                                        {0x7F98, 0x6670, 0x41A5, 0x2CC1}};
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* Mega Man [1/2/3] & others */
    case 0x01:
    case 0x10:
    case 0x29:
    case 0x52:
    case 0x5D:
    case 0x68:
    case 0x6D:
    case 0xF6: {
        const uint16_t palette[3][4] = {
            {0x7FFF, 0x329F, 0x001F, 0x0000}, /* OBJ0 */
            {0x7FFF, 0x3FE6, 0x0200, 0x0000}, /* OBJ1 */
            {0x7FFF, 0x7EAC, 0x40C0, 0x0000}  /* BG */
        };
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    default: {
        const uint16_t palette[3][4] = {{0x7FFF, 0x5294, 0x294A, 0x0000},
                                        {0x7FFF, 0x5294, 0x294A, 0x0000},
                                        {0x7FFF, 0x5294, 0x294A, 0x0000}};
        printf("No palette found for 0x%02X.\n", game_checksum);
        memcpy(priv->selected_palette, palette, palette_bytes);
    }
    }
}

/* Assign a palette. This is used to allow the user to manually select a
 * different color palette if one was not found automatically, or if the user
 * prefers a different color palette.
 * selection is the requestion color palette. This should be a maximum of
 * NUMBER_OF_PALETTES - 1. The default greyscale palette is selected otherwise.
 */
void manual_assign_palette(struct priv_t *priv, uint8_t selection)
{
#define NUMBER_OF_PALETTES 12
    size_t palette_bytes = 3 * 4 * sizeof(uint16_t);

    switch (selection) {
    /* 0x05 (Right) */
    case 0: {
        const uint16_t palette[3][4] = {{0x7FFF, 0x2BE0, 0x7D00, 0x0000},
                                        {0x7FFF, 0x2BE0, 0x7D00, 0x0000},
                                        {0x7FFF, 0x2BE0, 0x7D00, 0x0000}};
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* 0x07 (A + Down) */
    case 1: {
        const uint16_t palette[3][4] = {{0x7FFF, 0x7FE0, 0x7C00, 0x0000},
                                        {0x7FFF, 0x7FE0, 0x7C00, 0x0000},
                                        {0x7FFF, 0x7FE0, 0x7C00, 0x0000}};
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* 0x12 (Up) */
    case 2: {
        const uint16_t palette[3][4] = {{0x7FFF, 0x7EAC, 0x40C0, 0x0000},
                                        {0x7FFF, 0x7EAC, 0x40C0, 0x0000},
                                        {0x7FFF, 0x7EAC, 0x40C0, 0x0000}};
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* 0x13 (B + Right) */
    case 3: {
        const uint16_t palette[3][4] = {{0x0000, 0x0210, 0x7F60, 0x7FFF},
                                        {0x0000, 0x0210, 0x7F60, 0x7FFF},
                                        {0x0000, 0x0210, 0x7F60, 0x7FFF}};
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* 0x16 (B + Left, DMG Palette) */
    default:
    case 4: {
        const uint16_t palette[3][4] = {{0x7FFF, 0x5294, 0x294A, 0x0000},
                                        {0x7FFF, 0x5294, 0x294A, 0x0000},
                                        {0x7FFF, 0x5294, 0x294A, 0x0000}};
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* 0x17 (Down) */
    case 5: {
        const uint16_t palette[3][4] = {{0x7FF4, 0x7E52, 0x4A5F, 0x0000},
                                        {0x7FF4, 0x7E52, 0x4A5F, 0x0000},
                                        {0x7FF4, 0x7E52, 0x4A5F, 0x0000}};
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* 0x19 (B + Up) */
    case 6: {
        const uint16_t palette[3][4] = {{0x7FFF, 0x7EAC, 0x40C0, 0x0000},
                                        {0x7FFF, 0x7EAC, 0x40C0, 0x0000},
                                        {0x7F98, 0x6670, 0x41A5, 0x2CC1}};
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* 0x1C (A + Right) */
    case 7: {
        const uint16_t palette[3][4] = {{0x7FFF, 0x7E10, 0x48E7, 0x0000},
                                        {0x7FFF, 0x7E10, 0x48E7, 0x0000},
                                        {0x7FFF, 0x3FE6, 0x0198, 0x0000}};
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* 0x0D (A + Left) */
    case 8: {
        const uint16_t palette[3][4] = {{0x7FFF, 0x7E10, 0x48E7, 0x0000},
                                        {0x7FFF, 0x7EAC, 0x40C0, 0x0000},
                                        {0x7FFF, 0x463B, 0x2951, 0x0000}};
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* 0x10 (A + Up) */
    case 9: {
        const uint16_t palette[3][4] = {{0x7FFF, 0x3FE6, 0x0200, 0x0000},
                                        {0x7FFF, 0x329F, 0x001F, 0x0000},
                                        {0x7FFF, 0x7E10, 0x48E7, 0x0000}};
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* 0x18 (Left) */
    case 10: {
        const uint16_t palette[3][4] = {{0x7FFF, 0x7E10, 0x48E7, 0x0000},
                                        {0x7FFF, 0x3FE6, 0x0200, 0x0000},
                                        {0x7FFF, 0x329F, 0x001F, 0x0000}};
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }

    /* 0x1A (B + Down) */
    case 11: {
        const uint16_t palette[3][4] = {{0x7FFF, 0x329F, 0x001F, 0x0000},
                                        {0x7FFF, 0x3FE6, 0x0200, 0x0000},
                                        {0x7FFF, 0x7FE0, 0x3D20, 0x0000}};
        memcpy(priv->selected_palette, palette, palette_bytes);
        break;
    }
    }

    return;
}

/* Draw scanline into framebuffer */
static void lcd_draw_line(struct gb_s *gb,
                          const uint8_t pixels[160],
                          const uint_least8_t line)
{
    struct priv_t *priv = gb->direct.priv;

    for (unsigned int x = 0; x < LCD_WIDTH; x++)
        priv->fb[line][x] =
            priv->selected_palette[(pixels[x] & LCD_PALETTE_ALL) >> 4]
                                  [pixels[x] & 3];
}

int main(int argc, char **argv)
{
    struct gb_s gb;
    struct priv_t priv = {.rom = NULL};
    const double target_speed_ms = 1000.0 / VERTICAL_SYNC;
    double speed_compensation = 0.0;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;
    SDL_GameController *controller = NULL;
    uint_fast32_t new_ticks, old_ticks;
    gb_init_error_t gb_ret;
    unsigned int fast_mode = 1;
    unsigned int fast_mode_timer = 1;
    /* Record save file every 60 seconds */
    int save_timer = 60;
    /* Must be freed */
    char *rom_file_name = NULL;
    char *save_file_name = NULL;
    int ret = EXIT_SUCCESS;

    if (argc == 2) { /* ROM file was specified */
        rom_file_name = argv[1];
        /* FIXME: save_file_name is set to NULL. Make it configurable. */
    } else {
        printf("Usage: %s ROM\n", argv[0]);
        ret = EXIT_FAILURE;
        goto out;
    }

    /* Initialize frontend implementation, in this case, SDL2. */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) <
        0) {
        char buf[128];
        snprintf(buf, sizeof(buf), "Unable to initialize SDL2: %s\n",
                 SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", buf, NULL);
        ret = EXIT_FAILURE;
        goto out;
    }

    window =
        SDL_CreateWindow("Game Boy: Opening File", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, LCD_WIDTH * 2, LCD_HEIGHT * 2,
                         SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS);

    if (!window) {
        printf("Could not create window: %s\n", SDL_GetError());
        ret = EXIT_FAILURE;
        goto out;
    }

    /* Copy input ROM file to allocated memory. */
    if ((priv.rom = read_rom_to_ram(rom_file_name)) == NULL) {
        printf("Fail to read ROM: %s\n", strerror(errno));
        ret = EXIT_FAILURE;
        goto out;
    }

    /* If no save file is specified, copy save file (with specific name) to
     * allocated memory.
     */
    if (!save_file_name) {
        char *str_replace;
        const char extension[] = ".sav";

        /* Allocate enough space for the ROM file name, for the "sav"
         * extension and for the null terminator.
         */
        save_file_name = malloc(strlen(rom_file_name) + strlen(extension) + 1);
        if (!save_file_name) {
            printf("Fail to allocate memory: %s\n", strerror(errno));
            ret = EXIT_FAILURE;
            goto out;
        }

        /* Copy the ROM file name to allocated space. */
        strcpy(save_file_name, rom_file_name);

        /* If the file name does not have a dot, or the only dot is at
         * the start of the file name, set the pointer to begin
         * replacing the string to the end of the file name, otherwise
         * set it to the dot. */
        if ((str_replace = strrchr(save_file_name, '.')) == NULL ||
            str_replace == save_file_name)
            str_replace = save_file_name + strlen(save_file_name);

        /* Copy extension to string including terminating null byte. */
        for (unsigned int i = 0; i <= strlen(extension); i++)
            *(str_replace++) = extension[i];
    }

    /* TODO: Sanity check input GB file. */

    /* Initialize emulator context */
    gb_ret = gb_init(&gb, &gb_rom_read, &gb_cart_ram_read, &gb_cart_ram_write,
                     &gb_error, &priv);

    switch (gb_ret) {
    case GB_INIT_NO_ERROR:
        break;

    case GB_INIT_CARTRIDGE_UNSUPPORTED:
        puts("Unsupported cartridge.");
        ret = EXIT_FAILURE;
        goto out;

    case GB_INIT_INVALID_CHECKSUM:
        puts("Invalid ROM: Checksum failure.");
        ret = EXIT_FAILURE;
        goto out;

    default:
        printf("Unknown error: %d\n", gb_ret);
        ret = EXIT_FAILURE;
        goto out;
    }

    /* Load save file */
    read_cart_ram_file(save_file_name, &priv.cart_ram, gb_get_save_size(&gb));

    /* Set the RTC of the game cartridge. Only used by games that support it. */
    {
        time_t rawtime;
        time(&rawtime);
#ifdef _POSIX_C_SOURCE
        struct tm timeinfo;
        localtime_r(&rawtime, &timeinfo);
#else
        struct tm *timeinfo;
        timeinfo = localtime(&rawtime);
#endif

        /* You could potentially force the game to allow the player to
         * reset the time by setting the RTC to invalid values.
         *
         * Using memset(&gb->cart_rtc, 0xFF, sizeof(gb->cart_rtc)) for
         * example causes Pokemon Gold/Silver to say "TIME NOT SET",
         * allowing the player to set the time without having some dumb
         * password.
         *
         * The memset has to be done directly to gb->cart_rtc because
         * gb_set_rtc() processes the input values, which may cause
         * games to not detect invalid values.
         */

        /* Set RTC. Only games that specify support for RTC will use
         * these values. */
#ifdef _POSIX_C_SOURCE
        gb_set_rtc(&gb, &timeinfo);
#else
        gb_set_rtc(&gb, timeinfo);
#endif
    }

#if defined(ENABLE_SOUND)
    {
        SDL_AudioDeviceID dev;
        SDL_AudioSpec want, have;

        want.freq = AUDIO_SAMPLE_RATE;
        want.format = AUDIO_F32SYS, want.channels = 2;
        want.samples = AUDIO_SAMPLES;
        want.callback = audio_callback;
        want.userdata = NULL;

        printf("Audio driver: %s\n", SDL_GetAudioDeviceName(0, 0));

        if ((dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0)) == 0) {
            printf("SDL could not open audio device: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        audio_init();
        SDL_PauseAudioDevice(dev, 0);
    }
#endif

    gb_init_lcd(&gb, &lcd_draw_line);

    /* Allow the joystick input even if game is in background. */
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    if (SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt") < 0) {
        printf("Unable to assign joystick mappings: %s\n", SDL_GetError());
    }

    /* Open the first available controller. */
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (!SDL_IsGameController(i))
            continue;

        controller = SDL_GameControllerOpen(i);

        if (controller) {
            printf("Game Controller %s connected.\n",
                   SDL_GameControllerName(controller));
            break;
        } else {
            printf("Could not open game controller %i: %s\n", i,
                   SDL_GetError());
        }
    }

    {
        /* 12 for "Game Boy: " and a maximum of 16 for the title. */
        char title_str[28] = "Game Boy: ";
        printf("ROM: %s\n", gb_get_rom_name(&gb, title_str + 10));
        printf("MBC: %d\n", gb.mbc);
        SDL_SetWindowTitle(window, title_str);
    }

    SDL_SetWindowMinimumSize(window, LCD_WIDTH, LCD_HEIGHT);

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        ret = EXIT_FAILURE;
        goto out;
    }

    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) < 0) {
        printf("Renderer could not draw color: %s\n", SDL_GetError());
        ret = EXIT_FAILURE;
        goto out;
    }

    if (SDL_RenderClear(renderer) < 0) {
        printf("Renderer could not clear: %s\n", SDL_GetError());
        ret = EXIT_FAILURE;
        goto out;
    }

    SDL_RenderPresent(renderer);

    /* Use integer scale. */
    SDL_RenderSetLogicalSize(renderer, LCD_WIDTH, LCD_HEIGHT);
    SDL_RenderSetIntegerScale(renderer, 1);

    texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB555,
                          SDL_TEXTUREACCESS_STREAMING, LCD_WIDTH, LCD_HEIGHT);

    if (!texture) {
        printf("Texture could not be created: %s\n", SDL_GetError());
        ret = EXIT_FAILURE;
        goto out;
    }

    auto_assign_palette(&priv, gb_color_hash(&gb));

    while (SDL_QuitRequested() == SDL_FALSE) {
        int delay;
        static unsigned int rtc_timer = 0;
        static unsigned int selected_palette = 3;

        /* Calculate the time taken to draw frame, then later add a
         * delay to cap at 60 fps. */
        old_ticks = SDL_GetTicks();

        /* Get joypad input. */
        while (SDL_PollEvent(&event)) {
            static int fullscreen = 0;

            switch (event.type) {
            case SDL_QUIT:
                goto quit;

            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
                switch (event.cbutton.button) {
                case SDL_CONTROLLER_BUTTON_A:
                    gb.direct.joypad_bits.a = !event.cbutton.state;
                    break;

                case SDL_CONTROLLER_BUTTON_B:
                    gb.direct.joypad_bits.b = !event.cbutton.state;
                    break;

                case SDL_CONTROLLER_BUTTON_BACK:
                    gb.direct.joypad_bits.select = !event.cbutton.state;
                    break;

                case SDL_CONTROLLER_BUTTON_START:
                    gb.direct.joypad_bits.start = !event.cbutton.state;
                    break;

                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    gb.direct.joypad_bits.up = !event.cbutton.state;
                    break;

                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    gb.direct.joypad_bits.right = !event.cbutton.state;
                    break;

                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    gb.direct.joypad_bits.down = !event.cbutton.state;
                    break;

                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    gb.direct.joypad_bits.left = !event.cbutton.state;
                    break;
                }

                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_RETURN:
                    gb.direct.joypad_bits.start = 0;
                    break;

                case SDLK_BACKSPACE:
                    gb.direct.joypad_bits.select = 0;
                    break;

                case SDLK_z:
                    gb.direct.joypad_bits.a = 0;
                    break;

                case SDLK_x:
                    gb.direct.joypad_bits.b = 0;
                    break;

                case SDLK_UP:
                    gb.direct.joypad_bits.up = 0;
                    break;

                case SDLK_RIGHT:
                    gb.direct.joypad_bits.right = 0;
                    break;

                case SDLK_DOWN:
                    gb.direct.joypad_bits.down = 0;
                    break;

                case SDLK_LEFT:
                    gb.direct.joypad_bits.left = 0;
                    break;

                case SDLK_SPACE:
                    fast_mode = 2;
                    break;

                case SDLK_1:
                    fast_mode = 1;
                    break;

                case SDLK_2:
                    fast_mode = 2;
                    break;

                case SDLK_3:
                    fast_mode = 3;
                    break;

                case SDLK_4:
                    fast_mode = 4;
                    break;

                case SDLK_r:
                    gb_reset(&gb);
                    break;

                case SDLK_i:
                    gb.direct.interlace = ~gb.direct.interlace;
                    break;

                case SDLK_o:
                    gb.direct.frame_skip = ~gb.direct.frame_skip;
                    break;

                case SDLK_p:
                    if (event.key.keysym.mod == KMOD_LSHIFT) {
                        auto_assign_palette(&priv, gb_color_hash(&gb));
                        break;
                    }

                    if (++selected_palette == NUMBER_OF_PALETTES)
                        selected_palette = 0;

                    manual_assign_palette(&priv, selected_palette);
                    break;
                }

                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_RETURN:
                    gb.direct.joypad_bits.start = 1;
                    break;

                case SDLK_BACKSPACE:
                    gb.direct.joypad_bits.select = 1;
                    break;

                case SDLK_z:
                    gb.direct.joypad_bits.a = 1;
                    break;

                case SDLK_x:
                    gb.direct.joypad_bits.b = 1;
                    break;

                case SDLK_UP:
                    gb.direct.joypad_bits.up = 1;
                    break;

                case SDLK_RIGHT:
                    gb.direct.joypad_bits.right = 1;
                    break;

                case SDLK_DOWN:
                    gb.direct.joypad_bits.down = 1;
                    break;

                case SDLK_LEFT:
                    gb.direct.joypad_bits.left = 1;
                    break;

                case SDLK_SPACE:
                    fast_mode = 1;
                    break;

                case SDLK_f:
                    if (fullscreen) {
                        SDL_SetWindowFullscreen(window, 0);
                        fullscreen = 0;
                        SDL_ShowCursor(SDL_ENABLE);
                    } else {
                        SDL_SetWindowFullscreen(window,
                                                SDL_WINDOW_FULLSCREEN_DESKTOP);
                        fullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP;
                        SDL_ShowCursor(SDL_DISABLE);
                    }
                    break;

                case SDLK_F11: {
                    if (fullscreen) {
                        SDL_SetWindowFullscreen(window, 0);
                        fullscreen = 0;
                        SDL_ShowCursor(SDL_ENABLE);
                    } else {
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                        fullscreen = SDL_WINDOW_FULLSCREEN;
                        SDL_ShowCursor(SDL_DISABLE);
                    }
                } break;
                }

                break;
            }
        }

        /* Execute CPU cycles until the screen has to be redrawn. */
        gb_run_frame(&gb);

        /* Tick the internal RTC when 1 second has passed. */
        rtc_timer += target_speed_ms / fast_mode;

        if (rtc_timer >= 1000) {
            rtc_timer -= 1000;
            gb_tick_rtc(&gb);
        }

        /* Skip frames during fast mode. */
        if (fast_mode_timer > 1) {
            fast_mode_timer--;
            /* We continue here since the rest of the logic in the
             * loop is for drawing the screen and delaying. */
            continue;
        }

        fast_mode_timer = fast_mode;

        /* Copy frame buffer to SDL screen. */
        SDL_UpdateTexture(texture, NULL, &priv.fb,
                          LCD_WIDTH * sizeof(uint16_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        /* Use a delay that will draw the screen at a rate of 59.7275 Hz. */
        new_ticks = SDL_GetTicks();

        /* Since we can only delay for a maximum resolution of 1ms, we
         * can accumulate the error and compensate for the delay
         * accuracy when the delay compensation surpasses 1ms. */
        speed_compensation += target_speed_ms - (new_ticks - old_ticks);

        /* We cast the delay compensation value to an integer, since it
         * is the type used by SDL_Delay. This is where delay accuracy
         * is lost. */
        delay = (int) (speed_compensation);

        /* We then subtract the actual delay value by the requested
         * delay value. */
        speed_compensation -= delay;

        /* Only run delay logic if required. */
        if (delay > 0) {
            uint_fast32_t delay_ticks = SDL_GetTicks();
            uint_fast32_t after_delay_ticks;

            /* Tick the internal RTC when 1 second has passed. */
            rtc_timer += delay;

            if (rtc_timer >= 1000) {
                rtc_timer -= 1000;
                gb_tick_rtc(&gb);

                /* If 60 seconds has passed, record save file. It is required
                 * for the sake of unexpected abort by external library.
                 */
                --save_timer;
                if (!save_timer) {
                    write_cart_ram_file(save_file_name, &priv.cart_ram,
                                        gb_get_save_size(&gb));
                    save_timer = 60;
                }
            }

            /* This will delay for at least the number of milliseconds
             * requested, so we have to compensate for error here too.
             */
            SDL_Delay(delay);

            after_delay_ticks = SDL_GetTicks();
            speed_compensation +=
                (double) delay - (int) (after_delay_ticks - delay_ticks);
        }
    }

quit:
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_GameControllerClose(controller);
    SDL_Quit();

out:
    free(priv.rom);

    if (argc == 1)
        free(rom_file_name);

    return ret;
}
