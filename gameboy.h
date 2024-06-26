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

#include <time.h>
#include "core.h"
#include "cpu.h"

/* Tick the internal RTC by one second */
void gb_tick_rtc(struct gb_s *gb)
{
    /* is timer running? */
    if ((gb->cart_rtc[4] & 0x40) == 0) {
        if (++gb->rtc_bits.sec == 60) {
            gb->rtc_bits.sec = 0;

            if (++gb->rtc_bits.min == 60) {
                gb->rtc_bits.min = 0;

                if (++gb->rtc_bits.hour == 24) {
                    gb->rtc_bits.hour = 0;

                    if (++gb->rtc_bits.yday == 0) {
                        if (gb->rtc_bits.high & 1)     /* Bit 8 of days */
                            gb->rtc_bits.high |= 0x80; /* Overflow bit */

                        gb->rtc_bits.high ^= 1;
                    }
                }
            }
        }
    }
}

/* Set initial values in RTC.
 * Should be called after gb_init().
 */
void gb_set_rtc(struct gb_s *gb, const struct tm *const time)
{
    gb->cart_rtc[0] = time->tm_sec;
    gb->cart_rtc[1] = time->tm_min;
    gb->cart_rtc[2] = time->tm_hour;
    gb->cart_rtc[3] = time->tm_yday & 0xFF; /* Low 8 bits of day counter */
    gb->cart_rtc[4] = time->tm_yday >> 8;   /* High 1 bit of day counter */
}

/* Internal function used to read bytes. */
uint8_t __gb_read(struct gb_s *gb, const uint_fast16_t addr)
{
#ifdef GBIT
    return gb->gb_cart_ram_read(gb, addr);
#else
    switch (addr >> 12) {
    case 0x0:

    /* TODO: BIOS support */
    case 0x1:
    case 0x2:
    case 0x3:
        return gb->gb_rom_read(gb, addr);

    case 0x4:
    case 0x5:
    case 0x6:
    case 0x7:
        if (gb->mbc == 1 && gb->cart_mode_select)
            return gb->gb_rom_read(
                gb,
                addr + ((gb->selected_rom_bank & 0x1F) - 1) * ROM_BANK_SIZE);
        return gb->gb_rom_read(
            gb, addr + (gb->selected_rom_bank - 1) * ROM_BANK_SIZE);

    case 0x8:
    case 0x9:
        return gb->vram[addr - VRAM_ADDR];

    case 0xA:
    case 0xB:
        if (gb->cart_ram && gb->enable_cart_ram) {
            if (gb->mbc == 3 && gb->cart_ram_bank >= 0x08)
                return gb->cart_rtc[gb->cart_ram_bank - 0x08];
            if ((gb->cart_mode_select || gb->mbc != 1) &&
                gb->cart_ram_bank < gb->num_ram_banks)
                return gb->gb_cart_ram_read(
                    gb, addr - CART_RAM_ADDR +
                            (gb->cart_ram_bank * CRAM_BANK_SIZE));
            return gb->gb_cart_ram_read(gb, addr - CART_RAM_ADDR);
        }

        return 0;

    case 0xC:
        return gb->wram[addr - WRAM_0_ADDR];

    case 0xD:
        return gb->wram[addr - WRAM_1_ADDR + WRAM_BANK_SIZE];

    case 0xE:
        return gb->wram[addr - ECHO_ADDR];

    case 0xF:
        if (addr < OAM_ADDR)
            return gb->wram[addr - ECHO_ADDR];

        if (addr < UNUSED_ADDR)
            return gb->oam[addr - OAM_ADDR];

        /* Unusable memory area. Reading from this area returns 0. */
        if (addr < IO_ADDR)
            return 0xFF;

        /* HRAM */
        if (HRAM_ADDR <= addr && addr < INTR_EN_ADDR)
            return gb->hram[addr - HRAM_ADDR];

        if ((addr >= 0xFF10) && (addr <= 0xFF3F)) {
#if ENABLE_SOUND
            return audio_read(addr);
#else
            return 1;
#endif
        }

        /* IO and Interrupts */
        switch (addr & 0xFF) {
        /* IO Registers */
        case 0x00:
            return 0xC0 | gb->gb_reg.P1;

        case 0x01:
            return gb->gb_reg.SB;

        case 0x02:
            return gb->gb_reg.SC;

        /* Timer Registers */
        case 0x04:
            return gb->gb_reg.DIV;

        case 0x05:
            return gb->gb_reg.TIMA;

        case 0x06:
            return gb->gb_reg.TMA;

        case 0x07:
            return gb->gb_reg.TAC;

        /* Interrupt Flag Register */
        case 0x0F:
            return gb->gb_reg.IF;

        /* LCD Registers */
        case 0x40:
            return gb->gb_reg.LCDC;

        case 0x41:
            return (gb->gb_reg.STAT & STAT_USER_BITS) |
                   (gb->gb_reg.LCDC & LCDC_ENABLE ? gb->lcd_mode : LCD_VBLANK);

        case 0x42:
            return gb->gb_reg.SCY;

        case 0x43:
            return gb->gb_reg.SCX;

        case 0x44:
            return gb->gb_reg.LY;

        case 0x45:
            return gb->gb_reg.LYC;

        /* DMA Register */
        case 0x46:
            return gb->gb_reg.DMA;

        /* DMG Palette Registers */
        case 0x47:
            return gb->gb_reg.BGP;

        case 0x48:
            return gb->gb_reg.OBP0;

        case 0x49:
            return gb->gb_reg.OBP1;

        /* Window Position Registers */
        case 0x4A:
            return gb->gb_reg.WY;

        case 0x4B:
            return gb->gb_reg.WX;

        /* Interrupt Enable Register */
        case 0xFF:
            return gb->gb_reg.IE;

        /* Unused registers return 1 */
        default:
            return 0xFF;
        }
    }

    (gb->gb_error)(gb, GB_INVALID_READ, addr);
    return 0xFF;
#endif
}

/* Internal function used to write bytes */
void __gb_write(struct gb_s *gb, const uint_fast16_t addr, const uint8_t val)
{
#ifdef GBIT
    gb->gb_cart_ram_write(gb, addr, val);
#else
    switch (addr >> 12) {
    case 0x0:
    case 0x1:
        if (gb->mbc == 2 && addr & 0x10)
            return;
        else if (gb->mbc > 0 && gb->cart_ram)
            gb->enable_cart_ram = ((val & 0x0F) == 0x0A);

        return;

    case 0x2:
        if (gb->mbc == 5) {
            gb->selected_rom_bank = (gb->selected_rom_bank & 0x100) | val;
            gb->selected_rom_bank = gb->selected_rom_bank % gb->num_rom_banks;
            return;
        }

    /* Intentional fall through */
    case 0x3:
        if (gb->mbc == 1) {
            // selected_rom_bank = val & 0x7;
            gb->selected_rom_bank =
                (val & 0x1F) | (gb->selected_rom_bank & 0x60);

            if ((gb->selected_rom_bank & 0x1F) == 0x00)
                gb->selected_rom_bank++;
        } else if (gb->mbc == 2 && addr & 0x10) {
            gb->selected_rom_bank = val & 0x0F;

            if (!gb->selected_rom_bank)
                gb->selected_rom_bank++;
        } else if (gb->mbc == 3) {
            gb->selected_rom_bank = val & 0x7F;

            if (!gb->selected_rom_bank)
                gb->selected_rom_bank++;
        } else if (gb->mbc == 5)
            gb->selected_rom_bank =
                (val & 0x01) << 8 | (gb->selected_rom_bank & 0xFF);

        gb->selected_rom_bank = gb->selected_rom_bank % gb->num_rom_banks;
        return;

    case 0x4:
    case 0x5:
        if (gb->mbc == 1) {
            gb->cart_ram_bank = (val & 3);
            gb->selected_rom_bank =
                ((val & 3) << 5) | (gb->selected_rom_bank & 0x1F);
            gb->selected_rom_bank = gb->selected_rom_bank % gb->num_rom_banks;
        } else if (gb->mbc == 3)
            gb->cart_ram_bank = val;
        else if (gb->mbc == 5)
            gb->cart_ram_bank = (val & 0x0F);

        return;

    case 0x6:
    case 0x7:
        gb->cart_mode_select = (val & 1);
        return;

    case 0x8:
    case 0x9:
        gb->vram[addr - VRAM_ADDR] = val;
        return;

    case 0xA:
    case 0xB:
        if (gb->cart_ram && gb->enable_cart_ram) {
            if (gb->mbc == 3 && gb->cart_ram_bank >= 0x08)
                gb->cart_rtc[gb->cart_ram_bank - 0x08] = val;
            else if (gb->cart_mode_select &&
                     gb->cart_ram_bank < gb->num_ram_banks) {
                gb->gb_cart_ram_write(
                    gb,
                    addr - CART_RAM_ADDR + (gb->cart_ram_bank * CRAM_BANK_SIZE),
                    val);
            } else if (gb->num_ram_banks)
                gb->gb_cart_ram_write(gb, addr - CART_RAM_ADDR, val);
        }

        return;

    case 0xC:
        gb->wram[addr - WRAM_0_ADDR] = val;
        return;

    case 0xD:
        gb->wram[addr - WRAM_1_ADDR + WRAM_BANK_SIZE] = val;
        return;

    case 0xE:
        gb->wram[addr - ECHO_ADDR] = val;
        return;

    case 0xF:
        if (addr < OAM_ADDR) {
            gb->wram[addr - ECHO_ADDR] = val;
            return;
        }

        if (addr < UNUSED_ADDR) {
            gb->oam[addr - OAM_ADDR] = val;
            return;
        }

        /* Unusable memory area. */
        if (addr < IO_ADDR)
            return;

        if (HRAM_ADDR <= addr && addr < INTR_EN_ADDR) {
            gb->hram[addr - HRAM_ADDR] = val;
            return;
        }

        if ((addr >= 0xFF10) && (addr <= 0xFF3F)) {
#if ENABLE_SOUND
            audio_write(addr, val);
#endif
            return;
        }

        /* IO and Interrupts. */
        switch (addr & 0xFF) {
        /* Joypad */
        case 0x00:
            /* Only bits 5 and 4 are R/W.
             * The lower bits are overwritten later, and the two most
             * significant bits are unused. */
            gb->gb_reg.P1 = val;

            /* Direction keys selected */
            if ((gb->gb_reg.P1 & 0b010000) == 0)
                gb->gb_reg.P1 |= (gb->direct.joypad >> 4);
            /* Button keys selected */
            else
                gb->gb_reg.P1 |= (gb->direct.joypad & 0x0F);

            return;

        /* Serial */
        case 0x01:
            gb->gb_reg.SB = val;
            return;

        case 0x02:
            gb->gb_reg.SC = val;
            return;

        /* Timer Registers */
        case 0x04:
            gb->gb_reg.DIV = 0x00;
            return;

        case 0x05:
            gb->gb_reg.TIMA = val;
            return;

        case 0x06:
            gb->gb_reg.TMA = val;
            return;

        case 0x07:
            gb->gb_reg.TAC = val;
            return;

        /* Interrupt Flag Register */
        case 0x0F:
            gb->gb_reg.IF = (val | 0b11100000);
            return;

        /* LCD Registers */
        case 0x40:
            gb->gb_reg.LCDC = val;

            /* LY fixed to 0 when LCD turned off. */
            if ((gb->gb_reg.LCDC & LCDC_ENABLE) == 0) {
                /* Do not turn off LCD outside of VBLANK. This may happen due
                 * to poor timing in this emulator.
                 */
                if (gb->lcd_mode != LCD_VBLANK) {
                    gb->gb_reg.LCDC |= LCDC_ENABLE;
                    return;
                }

                gb->gb_reg.STAT = (gb->gb_reg.STAT & ~0x03) | LCD_VBLANK;
                gb->gb_reg.LY = 0;
                gb->counter.lcd_count = 0;
            }

            return;

        case 0x41:
            gb->gb_reg.STAT = (val & 0b01111000);
            return;

        case 0x42:
            gb->gb_reg.SCY = val;
            return;

        case 0x43:
            gb->gb_reg.SCX = val;
            return;

        /* LY (0xFF44) is read only */
        case 0x45:
            gb->gb_reg.LYC = val;
            return;

        /* DMA Register */
        case 0x46:
            gb->gb_reg.DMA = (val % 0xF1);

            for (uint8_t i = 0; i < OAM_SIZE; i++)
                gb->oam[i] = __gb_read(gb, (gb->gb_reg.DMA << 8) + i);

            return;

        /* DMG Palette Registers */
        case 0x47:
            gb->gb_reg.BGP = val;
            gb->display.bg_palette[0] = (gb->gb_reg.BGP & 0x03);
            gb->display.bg_palette[1] = (gb->gb_reg.BGP >> 2) & 0x03;
            gb->display.bg_palette[2] = (gb->gb_reg.BGP >> 4) & 0x03;
            gb->display.bg_palette[3] = (gb->gb_reg.BGP >> 6) & 0x03;
            return;

        case 0x48:
            gb->gb_reg.OBP0 = val;
            gb->display.sp_palette[0] = (gb->gb_reg.OBP0 & 0x03);
            gb->display.sp_palette[1] = (gb->gb_reg.OBP0 >> 2) & 0x03;
            gb->display.sp_palette[2] = (gb->gb_reg.OBP0 >> 4) & 0x03;
            gb->display.sp_palette[3] = (gb->gb_reg.OBP0 >> 6) & 0x03;
            return;

        case 0x49:
            gb->gb_reg.OBP1 = val;
            gb->display.sp_palette[4] = (gb->gb_reg.OBP1 & 0x03);
            gb->display.sp_palette[5] = (gb->gb_reg.OBP1 >> 2) & 0x03;
            gb->display.sp_palette[6] = (gb->gb_reg.OBP1 >> 4) & 0x03;
            gb->display.sp_palette[7] = (gb->gb_reg.OBP1 >> 6) & 0x03;
            return;

        /* Window Position Registers */
        case 0x4A:
            gb->gb_reg.WY = val;
            return;

        case 0x4B:
            gb->gb_reg.WX = val;
            return;

        /* Turn off boot ROM */
        case 0x50:
            gb->gb_bios_enable = 0;
            return;

        /* Interrupt Enable Register */
        case 0xFF:
            gb->gb_reg.IE = val;
            return;
        }
    }

    (gb->gb_error)(gb, GB_INVALID_WRITE, addr);
#endif
}

#if ENABLE_LCD
void __gb_draw_line(struct gb_s *gb)
{
    uint8_t pixels[160] = {0};

    /* If LCD not initialized by front-end, don't render anything. */
    if (gb->display.lcd_draw_line == NULL)
        return;

    if (gb->direct.frame_skip && !gb->display.frame_skip_count)
        return;

    /* If interlaced mode is activated, check if we need to draw the current
     * line.
     */
    if (gb->direct.interlace) {
        if ((gb->display.interlace_count == 0 && (gb->gb_reg.LY & 1) == 0) ||
            (gb->display.interlace_count == 1 && (gb->gb_reg.LY & 1) == 1)) {
            /* Compensate for missing window draw if required. */
            if (gb->gb_reg.LCDC & LCDC_WINDOW_ENABLE &&
                gb->gb_reg.LY >= gb->display.WY && gb->gb_reg.WX <= 166)
                gb->display.window_clear++;

            return;
        }
    }

    /* If background is enabled, draw it. */
    if (gb->gb_reg.LCDC & LCDC_BG_ENABLE) {
        /* Calculate current background line to draw. Constant because
         * this function draws only this one line each time it is
         * called. */
        const uint8_t bg_y = gb->gb_reg.LY + gb->gb_reg.SCY;

        /* Get selected background map address for first tile
         * corresponding to current line.
         * 0x20 (32) is the width of a background tile, and the bit
         * shift is to calculate the address. */
        const uint16_t bg_map =
            ((gb->gb_reg.LCDC & LCDC_BG_MAP) ? VRAM_BMAP_2 : VRAM_BMAP_1) +
            (bg_y >> 3) * 0x20;

        /* The displays (what the player sees) X coordinate, drawn right
         * to left. */
        uint8_t disp_x = LCD_WIDTH - 1;

        /* The X coordinate to begin drawing the background at. */
        uint8_t bg_x = disp_x + gb->gb_reg.SCX;

        /* Get tile index for current background tile. */
        uint8_t idx = gb->vram[bg_map + (bg_x >> 3)];
        /* Y coordinate of tile pixel to draw. */
        const uint8_t py = (bg_y & 0x07);
        /* X coordinate of tile pixel to draw. */
        uint8_t px = 7 - (bg_x & 0x07);

        uint16_t tile;

        /* Select addressing mode. */
        if (gb->gb_reg.LCDC & LCDC_TILE_SELECT)
            tile = VRAM_TILES_1 + idx * 0x10;
        else
            tile = VRAM_TILES_2 + ((idx + 0x80) % 0x100) * 0x10;

        tile += 2 * py;

        /* fetch first tile */
        uint8_t t1 = gb->vram[tile] >> px;
        uint8_t t2 = gb->vram[tile + 1] >> px;

        for (; disp_x != 0xFF; disp_x--) {
            if (px == 8) {
                /* fetch next tile */
                px = 0;
                bg_x = disp_x + gb->gb_reg.SCX;
                idx = gb->vram[bg_map + (bg_x >> 3)];

                if (gb->gb_reg.LCDC & LCDC_TILE_SELECT)
                    tile = VRAM_TILES_1 + idx * 0x10;
                else
                    tile = VRAM_TILES_2 + ((idx + 0x80) % 0x100) * 0x10;

                tile += 2 * py;
                t1 = gb->vram[tile];
                t2 = gb->vram[tile + 1];
            }

            /* copy background */
            uint8_t c = (t1 & 0x1) | ((t2 & 0x1) << 1);
            pixels[disp_x] = gb->display.bg_palette[c];
            pixels[disp_x] |= LCD_PALETTE_BG;
            t1 = t1 >> 1;
            t2 = t2 >> 1;
            px++;
        }
    }

    /* draw window */
    if (gb->gb_reg.LCDC & LCDC_WINDOW_ENABLE &&
        gb->gb_reg.LY >= gb->display.WY && gb->gb_reg.WX <= 166) {
        /* calculate window map address */
        uint16_t win_line =
            (gb->gb_reg.LCDC & LCDC_WINDOW_MAP) ? VRAM_BMAP_2 : VRAM_BMAP_1;
        win_line += (gb->display.window_clear >> 3) * 0x20;

        uint8_t disp_x = LCD_WIDTH - 1;
        uint8_t win_x = disp_x - gb->gb_reg.WX + 7;

        /* look up tile */
        uint8_t py = gb->display.window_clear & 0x07;
        uint8_t px = 7 - (win_x & 0x07);
        uint8_t idx = gb->vram[win_line + (win_x >> 3)];

        uint16_t tile;

        if (gb->gb_reg.LCDC & LCDC_TILE_SELECT)
            tile = VRAM_TILES_1 + idx * 0x10;
        else
            tile = VRAM_TILES_2 + ((idx + 0x80) % 0x100) * 0x10;

        tile += 2 * py;

        /* fetch first tile */
        uint8_t t1 = gb->vram[tile] >> px;
        uint8_t t2 = gb->vram[tile + 1] >> px;

        /* loop & copy window */
        uint8_t end = (gb->gb_reg.WX < 7 ? 0 : gb->gb_reg.WX - 7) - 1;

        for (; disp_x != end; disp_x--) {
            if (px == 8) {
                /* fetch next tile */
                px = 0;
                win_x = disp_x - gb->gb_reg.WX + 7;
                idx = gb->vram[win_line + (win_x >> 3)];

                if (gb->gb_reg.LCDC & LCDC_TILE_SELECT)
                    tile = VRAM_TILES_1 + idx * 0x10;
                else
                    tile = VRAM_TILES_2 + ((idx + 0x80) % 0x100) * 0x10;

                tile += 2 * py;
                t1 = gb->vram[tile];
                t2 = gb->vram[tile + 1];
            }

            /* copy window */
            uint8_t c = (t1 & 0x1) | ((t2 & 0x1) << 1);
            pixels[disp_x] = gb->display.bg_palette[c];
            pixels[disp_x] |= LCD_PALETTE_BG;
            t1 = t1 >> 1;
            t2 = t2 >> 1;
            px++;
        }

        gb->display.window_clear++; /* advance window line */
    }

    /* draw sprites */
    if (gb->gb_reg.LCDC & LCDC_OBJ_ENABLE) {
        uint8_t count = 0;

        for (uint8_t s = NUM_SPRITES - 1;
             s != 0xFF /* && count < MAX_SPRITES_LINE */; s--) {
            /* Sprite Y position */
            uint8_t OY = gb->oam[4 * s + 0];
            /* Sprite X position */
            uint8_t OX = gb->oam[4 * s + 1];
            /* Sprite Tile/Pattern Number */
            uint8_t OT = gb->oam[4 * s + 2] &
                         (gb->gb_reg.LCDC & LCDC_OBJ_SIZE ? 0xFE : 0xFF);
            /* Additional attributes */
            uint8_t OF = gb->oam[4 * s + 3];

            /* If sprite isn't on this line, continue. */
            if (gb->gb_reg.LY + (gb->gb_reg.LCDC & LCDC_OBJ_SIZE ? 0 : 8) >=
                    OY ||
                gb->gb_reg.LY + 16 < OY)
                continue;

            count++;

            /* Continue if sprite not visible. */
            if (OX == 0 || OX >= 168)
                continue;

            /* y flip */
            uint8_t py = gb->gb_reg.LY - OY + 16;

            if (OF & OBJ_FLIP_Y)
                py = (gb->gb_reg.LCDC & LCDC_OBJ_SIZE ? 15 : 7) - py;

            /* fetch the tile */
            uint8_t t1 = gb->vram[VRAM_TILES_1 + OT * 0x10 + 2 * py];
            uint8_t t2 = gb->vram[VRAM_TILES_1 + OT * 0x10 + 2 * py + 1];

            /* handle x flip */
            uint8_t dir, start, end, shift;

            if (OF & OBJ_FLIP_X) {
                dir = 1;
                start = (OX < 8 ? 0 : OX - 8);
                end = MIN(OX, LCD_WIDTH);
                shift = 8 - OX + start;
            } else {
                dir = -1;
                start = MIN(OX, LCD_WIDTH) - 1;
                end = (OX < 8 ? 0 : OX - 8) - 1;
                shift = OX - (start + 1);
            }

            /* copy tile */
            t1 >>= shift, t2 >>= shift;

            for (uint8_t disp_x = start; disp_x != end; disp_x += dir) {
                uint8_t c = (t1 & 0x1) | ((t2 & 0x1) << 1);
                /* TODO: check transparency / sprite overlap / background
                 * overlap.
                 */
                if (c && !(OF & OBJ_PRIORITY && pixels[disp_x] & 0x3)) {
                    /* Set pixel color. */
                    pixels[disp_x] = (OF & OBJ_PALETTE)
                                         ? gb->display.sp_palette[c + 4]
                                         : gb->display.sp_palette[c];
                    /* Set pixel palette (OBJ0 or OBJ1) */
                    pixels[disp_x] |= (OF & OBJ_PALETTE);
                    /* Deselect BG palette */
                    pixels[disp_x] &= ~LCD_PALETTE_BG;
                }

                t1 = t1 >> 1, t2 = t2 >> 1;
            }
        }
    }

    gb->display.lcd_draw_line(gb, pixels, gb->gb_reg.LY);
}
#endif

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
    /* Avoid using __gb_write when instruction tests, since __gb_write() is called to
     * write data to shared memory with gbit functions.
     */
#ifndef GBIT
    __gb_write(gb, 0xFF47, 0xFC); /* BGP */
    __gb_write(gb, 0xFF48, 0xFF); /* OBJP0 */
    __gb_write(gb, 0xFF49, 0x0F); /* OBJP1 */
#endif
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

/* Return the title of ROM.
 *
 * \param gb		Initialized context.
 * \param title_str	Allocated string at least 16 characters.
 * \returns		Pointer to start of string, null terminated.
 */
const char *gb_get_rom_name(struct gb_s *gb, char *title_str)
{
    uint_fast16_t title_loc = 0x134;
    /* End of title may be 0x13E for newer games. */
    const uint_fast16_t title_end = 0x143;
    const char *title_start = title_str;

    for (; title_loc <= title_end; title_loc++) {
        const char title_char = gb->gb_rom_read(gb, title_loc);

        if (title_char >= ' ' && title_char <= '_') {
            *title_str = title_char;
            title_str++;
        } else
            break;
    }

    *title_str = '\0';
    return title_start;
}

#if ENABLE_LCD
void gb_init_lcd(struct gb_s *gb,
                 void (*lcd_draw_line)(struct gb_s *gb,
                                       const uint8_t *pixels,
                                       const uint_fast8_t line))
{
    gb->display.lcd_draw_line = lcd_draw_line;

    gb->direct.interlace = 0;
    gb->display.interlace_count = 0;
    gb->direct.frame_skip = 0;
    gb->display.frame_skip_count = 0;

    gb->display.window_clear = 0;
    gb->display.WY = 0;
}
#endif
