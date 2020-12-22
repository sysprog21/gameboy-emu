#include "mmu.h"
#include "apu.h"

/* Internal function used to read bytes. */
uint8_t mmu_read(struct gb_s *gb, const uint_fast16_t addr)
{
#ifdef DEBUG
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
void mmu_write(struct gb_s *gb, const uint_fast16_t addr, const uint8_t val)
{
#ifdef DEBUG
    gb->gb_cart_ram_write(gb, addr, val);
    return;
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
                gb->oam[i] = mmu_read(gb, (gb->gb_reg.DMA << 8) + i);

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
