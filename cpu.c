#include "cpu.h"
#include "apu.h"
#include "instr.h"

static const void *dispatch_table[] = {
    /* clang-format off */ 
  [0x00] = {&&NOP_NONE_NONE},
  [0x01] = {&&LD16_REG_BC_IMM16},
  [0x02] = {&&LD_MEM_BC_REG_A},
  [0x03] = {&&INC16_REG_BC_NONE},
  [0x04] = {&&INC_REG_B_NONE},
  [0x05] = {&&DEC_REG_B_NONE},
  [0x06] = {&&LD_REG_B_IMM8},
  [0x07] = {&&RLC_REG_A_NONE},
  [0x08] = {&&LD16_MEM_16_REG_SP},
  [0x09] = {&&ADD16_REG_HL_REG_BC},
  [0x0a] = {&&LD_REG_A_MEM_BC},
  [0x0b] = {&&DEC16_REG_BC_NONE},
  [0x0c] = {&&INC_REG_C_NONE},
  [0x0d] = {&&DEC_REG_C_NONE},
  [0x0e] = {&&LD_REG_C_IMM8},
  [0x0f] = {&&RRC_REG_A_NONE},
  [0x10] = {&&STOP_NONE_NONE},
  [0x11] = {&&LD16_REG_DE_IMM16},
  [0x12] = {&&LD_MEM_DE_REG_A},
  [0x13] = {&&INC16_REG_DE_NONE},
  [0x14] = {&&INC_REG_D_NONE},
  [0x15] = {&&DEC_REG_D_NONE},
  [0x16] = {&&LD_REG_D_IMM8},
  [0x17] = {&&RL_REG_A_NONE},
  [0x18] = {&&JR_NONE_IMM8},
  [0x19] = {&&ADD16_REG_HL_REG_DE},
  [0x1a] = {&&LD_REG_A_MEM_DE},
  [0x1b] = {&&DEC16_REG_DE_NONE},
  [0x1c] = {&&INC_REG_E_NONE},
  [0x1d] = {&&DEC_REG_E_NONE},
  [0x1e] = {&&LD_REG_E_IMM8},
  [0x1f] = {&&RR_REG_A_NONE},
  [0x20] = {&&JR_CC_NZ_IMM8},
  [0x21] = {&&LD16_REG_HL_IMM16},
  [0x22] = {&&LD_MEM_INC_HL_REG_A},
  [0x23] = {&&INC16_REG_HL_NONE},
  [0x24] = {&&INC_REG_H_NONE},
  [0x25] = {&&DEC_REG_H_NONE},
  [0x26] = {&&LD_REG_H_IMM8},
  [0x27] = {&&DAA_NONE_NONE},
  [0x28] = {&&JR_CC_Z_IMM8},
  [0x29] = {&&ADD16_REG_HL_REG_HL},
  [0x2a] = {&&LD_REG_A_MEM_INC_HL},
  [0x2b] = {&&DEC16_REG_HL_NONE},
  [0x2c] = {&&INC_REG_L_NONE},
  [0x2d] = {&&DEC_REG_L_NONE},
  [0x2e] = {&&LD_REG_L_IMM8},
  [0x2f] = {&&CPL_REG_A_NONE},
  [0x30] = {&&JR_CC_NC_IMM8},
  [0x31] = {&&LD16_REG_SP_IMM16},
  [0x32] = {&&LD_MEM_DEC_HL_REG_A},
  [0x33] = {&&INC16_REG_SP_NONE},
  [0x34] = {&&INC_MEM_HL_NONE},
  [0x35] = {&&DEC_MEM_HL_NONE},
  [0x36] = {&&LD_MEM_HL_IMM8},
  [0x37] = {&&SCF_NONE_NONE},
  [0x38] = {&&JR_CC_C_IMM8},
  [0x39] = {&&ADD16_REG_HL_REG_SP},
  [0x3a] = {&&LD_REG_A_MEM_DEC_HL},
  [0x3b] = {&&DEC16_REG_SP_NONE},
  [0x3c] = {&&INC_REG_A_NONE},
  [0x3d] = {&&DEC_REG_A_NONE},
  [0x3e] = {&&LD_REG_A_IMM8},
  [0x3f] = {&&CCF_NONE_NONE},
  [0x40] = {&&NOP_NONE_NONE},
  [0x41] = {&&LD_REG_B_REG_C},
  [0x42] = {&&LD_REG_B_REG_D},
  [0x43] = {&&LD_REG_B_REG_E},
  [0x44] = {&&LD_REG_B_REG_H},
  [0x45] = {&&LD_REG_B_REG_L},
  [0x46] = {&&LD_REG_B_MEM_HL},
  [0x47] = {&&LD_REG_B_REG_A},
  [0x48] = {&&LD_REG_C_REG_B},
  [0x49] = {&&NOP_NONE_NONE},
  [0x4a] = {&&LD_REG_C_REG_D},
  [0x4b] = {&&LD_REG_C_REG_E},
  [0x4c] = {&&LD_REG_C_REG_H},
  [0x4d] = {&&LD_REG_C_REG_L},
  [0x4e] = {&&LD_REG_C_MEM_HL},
  [0x4f] = {&&LD_REG_C_REG_A},
  [0x50] = {&&LD_REG_D_REG_B},
  [0x51] = {&&LD_REG_D_REG_C},
  [0x52] = {&&NOP_NONE_NONE},
  [0x53] = {&&LD_REG_D_REG_E},
  [0x54] = {&&LD_REG_D_REG_H},
  [0x55] = {&&LD_REG_D_REG_L},
  [0x56] = {&&LD_REG_D_MEM_HL},
  [0x57] = {&&LD_REG_D_REG_A},
  [0x58] = {&&LD_REG_E_REG_B},
  [0x59] = {&&LD_REG_E_REG_C},
  [0x5a] = {&&LD_REG_E_REG_D},
  [0x5b] = {&&NOP_NONE_NONE},
  [0x5c] = {&&LD_REG_E_REG_H},
  [0x5d] = {&&LD_REG_E_REG_L},
  [0x5e] = {&&LD_REG_E_MEM_HL},
  [0x5f] = {&&LD_REG_E_REG_A},
  [0x60] = {&&LD_REG_H_REG_B},
  [0x61] = {&&LD_REG_H_REG_C},
  [0x62] = {&&LD_REG_H_REG_D},
  [0x63] = {&&LD_REG_H_REG_E},
  [0x64] = {&&NOP_NONE_NONE},
  [0x65] = {&&LD_REG_H_REG_L},
  [0x66] = {&&LD_REG_H_MEM_HL},
  [0x67] = {&&LD_REG_H_REG_A},
  [0x68] = {&&LD_REG_L_REG_B},
  [0x69] = {&&LD_REG_L_REG_C},
  [0x6a] = {&&LD_REG_L_REG_D},
  [0x6b] = {&&LD_REG_L_REG_E},
  [0x6c] = {&&LD_REG_L_REG_H},
  [0x6d] = {&&NOP_NONE_NONE},
  [0x6e] = {&&LD_REG_L_MEM_HL},
  [0x6f] = {&&D_REG_L_REG_A},
  [0x70] = {&&LD_MEM_HL_REG_B},
  [0x71] = {&&LD_MEM_HL_REG_C},
  [0x72] = {&&LD_MEM_HL_REG_D},
  [0x73] = {&&LD_MEM_HL_REG_E},
  [0x74] = {&&LD_MEM_HL_REG_H},
  [0x75] = {&&LD_MEM_HL_REG_L},
  [0x76] = {&&HALT_NONE_NONE},
  [0x77] = {&&LD_MEM_HL_REG_A},
  [0x78] = {&&LD_REG_A_REG_B},
  [0x79] = {&&LD_REG_A_REG_C},
  [0x7a] = {&&LD_REG_A_REG_D},
  [0x7b] = {&&LD_REG_A_REG_E},
  [0x7c] = {&&LD_REG_A_REG_H},
  [0x7d] = {&&LD_REG_A_REG_L},
  [0x7e] = {&&LD_REG_A_MEM_HL},
  [0x7f] = {&&NOP_NONE_NONE},
  [0x80] = {&&ADD_REG_A_REG_B},
  [0x81] = {&&ADD_REG_A_REG_C},
  [0x82] = {&&ADD_REG_A_REG_D},
  [0x83] = {&&ADD_REG_A_REG_E},
  [0x84] = {&&ADD_REG_A_REG_H},
  [0x85] = {&&ADD_REG_A_REG_L},
  [0x86] = {&&ADD_REG_A_MEM_HL},
  [0x87] = {&&ADD_REG_A_REG_A},
  [0x88] = {&&ADC_REG_A_REG_B},
  [0x89] = {&&ADC_REG_A_REG_C},
  [0x8a] = {&&ADC_REG_A_REG_D},
  [0x8b] = {&&ADC_REG_A_REG_E},
  [0x8c] = {&&ADC_REG_A_REG_H},
  [0x8d] = {&&ADC_REG_A_REG_L},
  [0x8e] = {&&ADC_REG_A_MEM_HL},
  [0x8f] = {&&ADC_REG_A_REG_A},
  [0x90] = {&&SUB_REG_A_REG_B},
  [0x91] = {&&SUB_REG_A_REG_C},
  [0x92] = {&&SUB_REG_A_REG_D},
  [0x93] = {&&SUB_REG_A_REG_E},
  [0x94] = {&&SUB_REG_A_REG_H},
  [0x95] = {&&SUB_REG_A_REG_L},
  [0x96] = {&&SUB_REG_A_MEM_HL},
  [0x97] = {&&SUB_REG_A_REG_A},
  [0x98] = {&&SBC_REG_A_REG_B},
  [0x99] = {&&SBC_REG_A_REG_C},
  [0x9a] = {&&SBC_REG_A_REG_D},
  [0x9b] = {&&SBC_REG_A_REG_E},
  [0x9c] = {&&SBC_REG_A_REG_H},
  [0x9d] = {&&SBC_REG_A_REG_L},
  [0x9e] = {&&SBC_REG_A_MEM_HL},
  [0x9f] = {&&SBC_REG_A_REG_A},
  [0xa0] = {&&AND_REG_A_REG_B},
  [0xa1] = {&&AND_REG_A_REG_C},
  [0xa2] = {&&AND_REG_A_REG_D},
  [0xa3] = {&&AND_REG_A_REG_E},
  [0xa4] = {&&AND_REG_A_REG_H},
  [0xa5] = {&&AND_REG_A_REG_L},
  [0xa6] = {&&AND_REG_A_MEM_HL},
  [0xa7] = {&&AND_REG_A_REG_A},
  [0xa8] = {&&XOR_REG_A_REG_B},
  [0xa9] = {&&XOR_REG_A_REG_C},
  [0xaa] = {&&XOR_REG_A_REG_D},
  [0xab] = {&&XOR_REG_A_REG_E},
  [0xac] = {&&XOR_REG_A_REG_H},
  [0xad] = {&&XOR_REG_A_REG_L},
  [0xae] = {&&XOR_REG_A_MEM_HL},
  [0xaf] = {&&XOR_REG_A_REG_A},
  [0xb0] = {&&OR_REG_A_REG_B},
  [0xb1] = {&&OR_REG_A_REG_C},
  [0xb2] = {&&OR_REG_A_REG_D},
  [0xb3] = {&&OR_REG_A_REG_E},
  [0xb4] = {&&OR_REG_A_REG_H},
  [0xb5] = {&&OR_REG_A_REG_L},
  [0xb6] = {&&OR_REG_A_MEM_HL},
  [0xb7] = {&&OR_REG_A_REG_A},
  [0xb8] = {&&CP_REG_A_REG_B},
  [0xb9] = {&&CP_REG_A_REG_C},
  [0xba] = {&&CP_REG_A_REG_D},
  [0xbb] = {&&CP_REG_A_REG_E},
  [0xbc] = {&&CP_REG_A_REG_H},
  [0xbd] = {&&CP_REG_A_REG_L},
  [0xbe] = {&&CP_REG_A_MEM_HL},
  [0xbf] = {&&CP_REG_A_REG_A},
  [0xc0] = {&&RET_CC_NZ_NONE},
  [0xc1] = {&&POP_REG_BC_NONE},
  [0xc2] = {&&JP_CC_NZ_IMM16},
  [0xc3] = {&&JP_NONE_IMM16},
  [0xc4] = {&&CALL_CC_NZ_IMM16},
  [0xc5] = {&&PUSH_REG_BC_NONE},
  [0xc6] = {&&ADD_REG_A_IMM8},
  [0xc7] = {&&RST_NONE_MEM_0x00},
  [0xc8] = {&&RET_CC_Z_NONE},
  [0xc9] = {&&RET_NONE_NONE},
  [0xca] = {&&JP_CC_Z_IMM16},
  [0xcb] = {&&CB},
  [0xcc] = {&&CALL_CC_Z_IMM16},
  [0xcd] = {&&CALL_NONE_IMM16},
  [0xce] = {&&ADC_REG_A_IMM8},
  [0xcf] = {&&RST_NONE_MEM_0x08},
  [0xd0] = {&&RET_CC_NC_NONE},
  [0xd1] = {&&POP_REG_DE_NONE},
  [0xd2] = {&&JP_CC_NC_IMM16},
  [0xd3] = {&&ERROR_NONE_NONE},
  [0xd4] = {&&CALL_CC_NC_IMM16},
  [0xd5] = {&&PUSH_REG_DE_NONE},
  [0xd6] = {&&SUB_REG_A_IMM8},
  [0xd7] = {&&RST_NONE_MEM_0x10},
  [0xd8] = {&&RET_CC_C_NONE},
  [0xd9] = {&&RETI_NONE_NONE},
  [0xda] = {&&JP_CC_C_IMM16},
  [0xdb] = {&&ERROR_NONE_NONE},
  [0xdc] = {&&CALL_CC_C_IMM16},
  [0xdd] = {&&ERROR_NONE_NONE},
  [0xde] = {&&SBC_REG_A_IMM8},
  [0xdf] = {&&RST_NONE_MEM_0x18},
  [0xe0] = {&&LD_MEM_8_REG_A},
  [0xe1] = {&&POP_REG_HL_NONE},
  [0xe2] = {&&LD_MEM_C_REG_A},
  [0xe3] = {&&ERROR_NONE_NONE},
  [0xe4] = {&&ERROR_NONE_NONE},
  [0xe5] = {&&PUSH_REG_HL_NONE},
  [0xe6] = {&&AND_REG_A_IMM8},
  [0xe7] = {&&RST_NONE_MEM_0x20},
  [0xe8] = {&&ADD16_REG_SP_IMM8},
  [0xe9] = {&&JP_NONE_MEM_HL},
  [0xea] = {&&LD_MEM_16_REG_A},
  [0xeb] = {&&ERROR_NONE_NONE},
  [0xec] = {&&ERROR_NONE_NONE},
  [0xed] = {&&ERROR_NONE_NONE},
  [0xee] = {&&XOR_REG_A_IMM8},
  [0xef] = {&&RST_NONE_MEM_0x28},
  [0xf0] = {&&LD_REG_A_MEM_8},
  [0xf1] = {&&POP_REG_AF_NONE},
  [0xf2] = {&&LD_REG_A_MEM_C},
  [0xf3] = {&&DI_NONE_NONE},
  [0xf4] = {&&ERROR_NONE_NONE},
  [0xf5] = {&&PUSH_REG_AF_NONE},
  [0xf6] = {&&OR_REG_A_IMM8},
  [0xf7] = {&&RST_NONE_MEM_0x30},
  [0xf8] = {&&LD16_REG_HL_MEM_8},
  [0xf9] = {&&LD16_REG_SP_REG_HL},
  [0xfa] = {&&LD_REG_A_MEM_16},
  [0xfb] = {&&EI_NONE_NONE},
  [0xfc] = {&&ERROR_NONE_NONE},
  [0xfd] = {&&ERROR_NONE_NONE},
  [0xfe] = {&&CP_REG_A_IMM8},
  [0xff] = {&&RST_NONE_MEM_0x38}
    /* clang-format on */
};
#define DISPATCH() \
    goto *dispatch_table[(gb->gb_halt ? 0x00 : __gb_read(gb, gb->cpu_reg.pc++))]

/* Internal function used to read bytes. */
uint8_t __gb_read(struct gb_s *gb, const uint_fast16_t addr)
{
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
}

/* Internal function used to write bytes */
void __gb_write(struct gb_s *gb, const uint_fast16_t addr, const uint8_t val)
{
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
}

uint8_t __gb_execute_cb(struct gb_s *gb)
{
    uint8_t inst_cycles;
    uint8_t cbop = __gb_read(gb, gb->cpu_reg.pc++);
    uint8_t r = (cbop & 0x7);
    uint8_t b = (cbop >> 3) & 0x7;
    uint8_t d = (cbop >> 3) & 0x1;
    uint8_t val;
    uint8_t writeback = 1;

    inst_cycles = 8;
    /* Add an additional 8 cycles to these sets of instructions. */
    switch (cbop & 0xC7) {
    case 0x06:
    case 0x86:
    case 0xC6:
        inst_cycles += 8;
        break;
    case 0x46:
        inst_cycles += 4;
        break;
    }

    switch (r) {
    case 0:
        val = gb->cpu_reg.b;
        break;

    case 1:
        val = gb->cpu_reg.c;
        break;

    case 2:
        val = gb->cpu_reg.d;
        break;

    case 3:
        val = gb->cpu_reg.e;
        break;

    case 4:
        val = gb->cpu_reg.h;
        break;

    case 5:
        val = gb->cpu_reg.l;
        break;

    case 6:
        val = __gb_read(gb, gb->cpu_reg.hl);
        break;

    /* Only values 0-7 are possible here, so we make the final case
     * default to satisfy -Wmaybe-uninitialized warning. */
    default:
        val = gb->cpu_reg.a;
        break;
    }

    /* TODO: Find out WTF this is doing. */
    switch (cbop >> 6) {
    case 0x0:
        cbop = (cbop >> 4) & 0x3;

        switch (cbop) {
        case 0x0:    /* RdC R */
        case 0x1:    /* Rd R */
            if (d) { /* RRC R / RR R */
                uint8_t temp = val;
                val = (val >> 1);
                val |= cbop ? (gb->cpu_reg.f_bits.c << 7) : (temp << 7);
                gb->cpu_reg.f_bits.z = (val == 0x00);
                gb->cpu_reg.f_bits.n = 0;
                gb->cpu_reg.f_bits.h = 0;
                gb->cpu_reg.f_bits.c = (temp & 0x01);
            } else { /* RLC R / RL R */
                uint8_t temp = val;
                val = (val << 1);
                val |= cbop ? gb->cpu_reg.f_bits.c : (temp >> 7);
                gb->cpu_reg.f_bits.z = (val == 0x00);
                gb->cpu_reg.f_bits.n = 0;
                gb->cpu_reg.f_bits.h = 0;
                gb->cpu_reg.f_bits.c = (temp >> 7);
            }
            break;

        case 0x2:
            if (d) { /* SRA R */
                gb->cpu_reg.f_bits.c = val & 0x01;
                val = (val >> 1) | (val & 0x80);
                gb->cpu_reg.f_bits.z = (val == 0x00);
                gb->cpu_reg.f_bits.n = 0;
                gb->cpu_reg.f_bits.h = 0;
            } else { /* SLA R */
                gb->cpu_reg.f_bits.c = (val >> 7);
                val = val << 1;
                gb->cpu_reg.f_bits.z = (val == 0x00);
                gb->cpu_reg.f_bits.n = 0;
                gb->cpu_reg.f_bits.h = 0;
            }
            break;

        case 0x3:
            if (d) { /* SRL R */
                gb->cpu_reg.f_bits.c = val & 0x01;
                val = val >> 1;
                gb->cpu_reg.f_bits.z = (val == 0x00);
                gb->cpu_reg.f_bits.n = 0;
                gb->cpu_reg.f_bits.h = 0;
            } else { /* SWAP R */
                uint8_t temp = (val >> 4) & 0x0F;
                temp |= (val << 4) & 0xF0;
                val = temp;
                gb->cpu_reg.f_bits.z = (val == 0x00);
                gb->cpu_reg.f_bits.n = 0;
                gb->cpu_reg.f_bits.h = 0;
                gb->cpu_reg.f_bits.c = 0;
            }
            break;
        }
        break;

    case 0x1: /* BIT B, R */
        gb->cpu_reg.f_bits.z = !((val >> b) & 0x1);
        gb->cpu_reg.f_bits.n = 0;
        gb->cpu_reg.f_bits.h = 1;
        writeback = 0;
        break;

    case 0x2: /* RES B, R */
        val &= (0xFE << b) | (0xFF >> (8 - b));
        break;

    case 0x3: /* SET B, R */
        val |= (0x1 << b);
        break;
    }

    if (writeback) {
        switch (r) {
        case 0:
            gb->cpu_reg.b = val;
            break;

        case 1:
            gb->cpu_reg.c = val;
            break;

        case 2:
            gb->cpu_reg.d = val;
            break;

        case 3:
            gb->cpu_reg.e = val;
            break;

        case 4:
            gb->cpu_reg.h = val;
            break;

        case 5:
            gb->cpu_reg.l = val;
            break;

        case 6:
            __gb_write(gb, gb->cpu_reg.hl, val);
            break;

        case 7:
            gb->cpu_reg.a = val;
            break;
        }
    }
    return inst_cycles;
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

/* Internal function used to step the CPU */
void __gb_step_cpu(struct gb_s *gb)
{
    uint8_t opcode, inst_cycles;
    static const uint8_t op_cycles[0x100] = {
        /* clang-format off */
        /*          0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */
        /* 0x00 */  4, 12,  8,  8,  4,  4,  8,  4, 20,  8,  8,  8,  4,  4,  8,  4,
        /* 0x10 */  4, 12,  8,  8,  4,  4,  8,  4, 12,  8,  8,  8,  4,  4,  8,  4,
        /* 0x20 */  8, 12,  8,  8,  4,  4,  8,  4,  8,  8,  8,  8,  4,  4,  8,  4,
        /* 0x30 */  8, 12,  8,  8, 12, 12, 12,  4,  8,  8,  8,  8,  4,  4,  8,  4,
        /* 0x40 */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
        /* 0x50 */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
        /* 0x60 */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
        /* 0x70 */  8,  8,  8,  8,  8,  8,  4,  8,  4,  4,  4,  4,  4,  4,  8,  4,
        /* 0x80 */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
        /* 0x90 */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
        /* 0xA0 */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
        /* 0xB0 */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
        /* 0xC0 */  8, 12, 12, 16, 12, 16,  8, 16,  8, 16, 12,  8, 12, 24,  8, 16,
        /* 0xD0 */  8, 12, 12,  0, 12, 16,  8, 16,  8, 16, 12,  0, 12,  0,  8, 16,
        /* 0xE0 */ 12, 12,  8,  0,  0, 16,  8, 16, 16,  4, 16,  0,  0,  0,  8, 16,
        /* 0xF0 */ 12, 12,  8,  4,  0, 16,  8, 16, 12,  8, 16,  4,  0,  0,  8, 16,
        /* clang-format on */
    };

    /* Handle interrupts */
    if ((gb->gb_ime || gb->gb_halt) &&
        (gb->gb_reg.IF & gb->gb_reg.IE & ANY_INTR)) {
        gb->gb_halt = 0;

        if (gb->gb_ime) {
            /* Disable interrupts */
            gb->gb_ime = 0;

            /* Push program counter */
            __gb_write(gb, --gb->cpu_reg.sp, gb->cpu_reg.pc >> 8);
            __gb_write(gb, --gb->cpu_reg.sp, gb->cpu_reg.pc & 0xFF);

            /* Call interrupt handler if required. */
            if (gb->gb_reg.IF & gb->gb_reg.IE & VBLANK_INTR) {
                gb->cpu_reg.pc = VBLANK_INTR_ADDR;
                gb->gb_reg.IF ^= VBLANK_INTR;
            } else if (gb->gb_reg.IF & gb->gb_reg.IE & LCDC_INTR) {
                gb->cpu_reg.pc = LCDC_INTR_ADDR;
                gb->gb_reg.IF ^= LCDC_INTR;
            } else if (gb->gb_reg.IF & gb->gb_reg.IE & TIMER_INTR) {
                gb->cpu_reg.pc = TIMER_INTR_ADDR;
                gb->gb_reg.IF ^= TIMER_INTR;
            } else if (gb->gb_reg.IF & gb->gb_reg.IE & SERIAL_INTR) {
                gb->cpu_reg.pc = SERIAL_INTR_ADDR;
                gb->gb_reg.IF ^= SERIAL_INTR;
            } else if (gb->gb_reg.IF & gb->gb_reg.IE & CONTROL_INTR) {
                gb->cpu_reg.pc = CONTROL_INTR_ADDR;
                gb->gb_reg.IF ^= CONTROL_INTR;
            }
        }
    }

/* Obtain opcode */
// opcode = (gb->gb_halt ? 0x00 : __gb_read(gb, gb->cpu_reg.pc++));
// inst_cycles = op_cycles[opcode];

/* cpu_instr */
// cpu_instr table;
// void (*opcode_function)() = table.execute;
// if (opcode == 0xcb)
//    table = cb_table[READ8(REG(pc)++)];
// else
//    table = instr_table[opcode];

// inst_cycles = table.alt_cycles * 4;
// if (table.flags == INST_FLAG_USES_CC)
//    opcode_function(gb, opcode, &inst_cycles);
// else
//    opcode_function(gb, opcode);
NOP_NONE_NONE:
    _Z80InstructionNOP(gb);
    DISPATCH();
LD16_REG_BC_IMM16:
    _Z80InstructionLDBC_Imm16(gb);
    DISPATCH();
LD_MEM_BC_REG_A:
    _Z80InstructionLDBC_A(gb);
    DISPATCH();
INC16_REG_BC_NONE:
    _Z80InstructionINC_BC(gb);
    DISPATCH();
INC_REG_B_NONE:
    _Z80InstructionINCB(gb);
    DISPATCH();
DEC_REG_B_NONE:
    _Z80InstructionDECB(gb);
    DISPATCH();
LD_REG_B_IMM8:
    _Z80InstructionLDB_Imm(gb);
    DISPATCH();
RLC_REG_A_NONE:
    _Z80InstructionRLCA(gb);
    DISPATCH();
LD16_MEM_16_REG_SP:
    _Z80InstructionLDImm16_SP(gb);
    DISPATCH();
ADD16_REG_HL_REG_BC:
    _Z80InstructionADDHL_BC(gb);
    DISPATCH();
LD_REG_A_MEM_BC:
    _Z80InstructionLDA_BC(gb);
    DISPATCH();
DEC16_REG_BC_NONE:
    _Z80InstructionDEC_BC(gb);
    DISPATCH();
INC_REG_C_NONE:
    _Z80InstructionINCC(gb);
    DISPATCH();
DEC_REG_C_NONE:
    _Z80InstructionDECC(gb);
    DISPATCH();
LD_REG_C_IMM8:
    _Z80InstructionLDC_Imm(gb);
    DISPATCH();
RRC_REG_A_NONE:
    _Z80InstructionRRCA(gb);
    DISPATCH();
STOP_NONE_NONE:
    _Z80InstructionSTOP(gb);
    DISPATCH();
LD16_REG_DE_IMM16:
    _Z80InstructionLDDE_Imm16(gb);
    DISPATCH();
LD_MEM_DE_REG_A:
    _Z80InstructionLDDE_A(gb);
    DISPATCH();
INC16_REG_DE_NONE:
    _Z80InstructionINC_DE(gb);
    DISPATCH();
INC_REG_D_NONE:
    _Z80InstructionINCD(gb);
    DISPATCH();
DEC_REG_D_NONE:
    _Z80InstructionDECD(gb);
    DISPATCH();
LD_REG_D_IMM8:
    _Z80InstructionLDD_Imm(gb);
    DISPATCH();
RL_REG_A_NONE:
    _Z80InstructionRLA(gb);
    DISPATCH();
JR_NONE_IMM8:
    _Z80InstructionJR(gb);
    DISPATCH();
ADD16_REG_HL_REG_DE:
    _Z80InstructionADDHL_DE(gb);
    DISPATCH();
LD_REG_A_MEM_DE:
    _Z80InstructionLDA_DE(gb);
    DISPATCH();
DEC16_REG_DE_NONE:
    _Z80InstructionDEC_DE(gb);
    DISPATCH();
INC_REG_E_NONE:
    _Z80InstructionINCE(gb);
    DISPATCH();
DEC_REG_E_NONE:
    _Z80InstructionDECE(gb);
    DISPATCH();
LD_REG_E_IMM8:
    _Z80InstructionLDE_Imm(gb);
    DISPATCH();
RR_REG_A_NONE:
    _Z80InstructionRRA(gb);
    DISPATCH();
JR_CC_NZ_IMM8:
    _Z80InstructionJRNZ(gb, &inst_cycles);
    DISPATCH();
LD16_REG_HL_IMM16:
    _Z80InstructionLDHL_Imm16(gb);
    DISPATCH();
LD_MEM_INC_HL_REG_A:
    _Z80InstructionLDINC_A(gb);
    DISPATCH();
INC16_REG_HL_NONE:
    _Z80InstructionINC_HL(gb);
    DISPATCH();
INC_REG_H_NONE:
    _Z80InstructionINCH(gb);
    DISPATCH();
DEC_REG_H_NONE:
    _Z80InstructionDECH(gb);
    DISPATCH();
LD_REG_H_IMM8:
    _Z80InstructionLDH_Imm(gb);
    DISPATCH();
DAA_NONE_NONE:
    _Z80InstructionDAA(gb);
    DISPATCH();
JR_CC_Z_IMM8:
    _Z80InstructionJRZ(gb, &inst_cycles);
    DISPATCH();
ADD16_REG_HL_REG_HL:
    _Z80InstructionADDHL_HL(gb);
    DISPATCH();
LD_REG_A_MEM_INC_HL:
    _Z80InstructionLDA_INC(gb);
    DISPATCH();
DEC16_REG_HL_NONE:
    _Z80InstructionDEC_HL(gb);
    DISPATCH();
INC_REG_L_NONE:
    _Z80InstructionINCL(gb);
    DISPATCH();
DEC_REG_L_NONE:
    _Z80InstructionDECL(gb);
    DISPATCH();
LD_REG_L_IMM8:
    _Z80InstructionLDL_Imm(gb);
    DISPATCH();
CPL_REG_A_NONE:
    _Z80InstructionCPL_(gb);
    DISPATCH();
JR_CC_NC_IMM8:
    _Z80InstructionJRNC(gb, &inst_cycles);
    DISPATCH();
LD16_REG_SP_IMM16:
    _Z80InstructionLDSP_Imm16(gb);
    DISPATCH();
LD_MEM_DEC_HL_REG_A:
    _Z80InstructionLDDEC_A(gb);
    DISPATCH();
INC16_REG_SP_NONE:
    _Z80InstructionINC_SP(gb);
    DISPATCH();
INC_MEM_HL_NONE:
    _Z80InstructionINC_Mem(gb);
    DISPATCH();
DEC_MEM_HL_NONE:
    _Z80InstructionDEC_Mem(gb);
    DISPATCH();
LD_MEM_HL_IMM8:
    _Z80InstructionLDHL_Imm(gb);
    DISPATCH();
SCF_NONE_NONE:
    _Z80InstructionSCF(gb);
    DISPATCH();
JR_CC_C_IMM8:
    _Z80InstructionJRC(gb, &inst_cycles);
    DISPATCH();
ADD16_REG_HL_REG_SP:
    _Z80InstructionADDHL_SP(gb);
    DISPATCH();
LD_REG_A_MEM_DEC_HL:
    _Z80InstructionLDA_DEC(gb);
    DISPATCH();
DEC16_REG_SP_NONE:
    _Z80InstructionDEC_SP(gb);
    DISPATCH();
INC_REG_A_NONE:
    _Z80InstructionINCA(gb);
    DISPATCH();
DEC_REG_A_NONE:
    _Z80InstructionDECA(gb);
    DISPATCH();
LD_REG_A_IMM8:
    _Z80InstructionLDA_Imm(gb);
    DISPATCH();
CCF_NONE_NONE:
    _Z80InstructionCCF(gb);
    DISPATCH();
LD_REG_B_REG_C:
    _Z80InstructionLDB_C(gb);
    DISPATCH();
LD_REG_B_REG_D:
    _Z80InstructionLDB_D(gb);
    DISPATCH();
LD_REG_B_REG_E:
    _Z80InstructionLDB_E(gb);
    DISPATCH();
LD_REG_B_REG_H:
    _Z80InstructionLDB_H(gb);
    DISPATCH();
LD_REG_B_REG_L:
    _Z80InstructionLDB_L(gb);
    DISPATCH();
LD_REG_B_MEM_HL:
    _Z80InstructionLDB_HL(gb);
    DISPATCH();
LD_REG_B_REG_A:
    _Z80InstructionLDB_A(gb);
    DISPATCH();
LD_REG_C_REG_B:
    _Z80InstructionLDC_B(gb);
    DISPATCH();
LD_REG_C_REG_D:
    _Z80InstructionLDC_D(gb);
    DISPATCH();
LD_REG_C_REG_E:
    _Z80InstructionLDC_E(gb);
    DISPATCH();
LD_REG_C_REG_H:
    _Z80InstructionLDC_H(gb);
    DISPATCH();
LD_REG_C_REG_L:
    _Z80InstructionLDC_L(gb);
    DISPATCH();
LD_REG_C_MEM_HL:
    _Z80InstructionLDC_HL(gb);
    DISPATCH();
LD_REG_C_REG_A:
    _Z80InstructionLDC_A(gb);
    DISPATCH();
LD_REG_D_REG_B:
    _Z80InstructionLDD_B(gb);
    DISPATCH();
LD_REG_D_REG_C:
    _Z80InstructionLDD_C(gb);
    DISPATCH();
LD_REG_D_REG_E:
    _Z80InstructionLDD_E(gb);
    DISPATCH();
LD_REG_D_REG_H:
    _Z80InstructionLDD_H(gb);
    DISPATCH();
LD_REG_D_REG_L:
    _Z80InstructionLDD_L(gb);
    DISPATCH();
LD_REG_D_MEM_HL:
    _Z80InstructionLDD_HL(gb);
    DISPATCH();
LD_REG_D_REG_A:
    _Z80InstructionLDD_A(gb);
    DISPATCH();
LD_REG_E_REG_B:
    _Z80InstructionLDE_B(gb);
    DISPATCH();
LD_REG_E_REG_C:
    _Z80InstructionLDE_C(gb);
    DISPATCH();
LD_REG_E_REG_D:
    _Z80InstructionLDE_D(gb);
    DISPATCH();
LD_REG_E_REG_H:
    _Z80InstructionLDE_H(gb);
    DISPATCH();
LD_REG_E_REG_L:
    _Z80InstructionLDE_L(gb);
    DISPATCH();
LD_REG_E_MEM_HL:
    _Z80InstructionLDE_HL(gb);
    DISPATCH();
LD_REG_E_REG_A:
    _Z80InstructionLDE_A(gb);
    DISPATCH();
LD_REG_H_REG_B:
    _Z80InstructionLDH_B(gb);
    DISPATCH();
LD_REG_H_REG_C:
    _Z80InstructionLDH_C(gb);
    DISPATCH();
LD_REG_H_REG_D:
    _Z80InstructionLDH_D(gb);
    DISPATCH();
LD_REG_H_REG_E:
    _Z80InstructionLDH_E(gb);
    DISPATCH();
LD_REG_H_REG_L:
    _Z80InstructionLDH_L(gb);
    DISPATCH();
LD_REG_H_MEM_HL:
    _Z80InstructionLDH_HL(gb);
    DISPATCH();
LD_REG_H_REG_A:
    _Z80InstructionLDH_A(gb);
    DISPATCH();
LD_REG_L_REG_B:
    _Z80InstructionLDL_B(gb);
    DISPATCH();
LD_REG_L_REG_C:
    _Z80InstructionLDL_C(gb);
    DISPATCH();
LD_REG_L_REG_D:
    _Z80InstructionLDL_D(gb);
    DISPATCH();
LD_REG_L_REG_E:
    _Z80InstructionLDL_E(gb);
    DISPATCH();
LD_REG_L_REG_H:
    _Z80InstructionLDL_H(gb);
    DISPATCH();
LD_REG_L_MEM_HL:
    _Z80InstructionLDL_HL(gb);
    DISPATCH();
D_REG_L_REG_A:
    DISPATCH();
LD_MEM_HL_REG_B:
    _Z80InstructionLDHL_B(gb);
    DISPATCH();
LD_MEM_HL_REG_C:
    _Z80InstructionLDHL_C(gb);
    DISPATCH();
LD_MEM_HL_REG_D:
    _Z80InstructionLDHL_D(gb);
    DISPATCH();
LD_MEM_HL_REG_E:
    _Z80InstructionLDHL_E(gb);
    DISPATCH();
LD_MEM_HL_REG_H:
    _Z80InstructionLDHL_H(gb);
    DISPATCH();
LD_MEM_HL_REG_L:
    _Z80InstructionLDHL_L(gb);
    DISPATCH();
HALT_NONE_NONE:
    _Z80InstructionHALT(gb);
    DISPATCH();
LD_MEM_HL_REG_A:
    _Z80InstructionLDHL_A(gb);
    DISPATCH();
LD_REG_A_REG_B:
    _Z80InstructionLDA_B(gb);
    DISPATCH();
LD_REG_A_REG_C:
    _Z80InstructionLDA_C(gb);
    DISPATCH();
LD_REG_A_REG_D:
    _Z80InstructionLDA_D(gb);
    DISPATCH();
LD_REG_A_REG_E:
    _Z80InstructionLDA_E(gb);
    DISPATCH();
LD_REG_A_REG_H:
    _Z80InstructionLDA_H(gb);
    DISPATCH();
LD_REG_A_REG_L:
    _Z80InstructionLDA_L(gb);
    DISPATCH();
LD_REG_A_MEM_HL:
    _Z80InstructionLDA_HL(gb);
    DISPATCH();
ADD_REG_A_REG_B:
    _Z80InstructionADDB(gb);
    DISPATCH();
ADD_REG_A_REG_C:
    _Z80InstructionADDC(gb);
    DISPATCH();
ADD_REG_A_REG_D:
    _Z80InstructionADDD(gb);
    DISPATCH();
ADD_REG_A_REG_E:
    _Z80InstructionADDE(gb);
    DISPATCH();
ADD_REG_A_REG_H:
    _Z80InstructionADDH(gb);
    DISPATCH();
ADD_REG_A_REG_L:
    _Z80InstructionADDL(gb);
    DISPATCH();
ADD_REG_A_MEM_HL:
    _Z80InstructionADDHL(gb);
    DISPATCH();
ADD_REG_A_REG_A:
    _Z80InstructionADDA(gb);
    DISPATCH();
ADC_REG_A_REG_B:
    _Z80InstructionADCB(gb);
    DISPATCH();
ADC_REG_A_REG_C:
    _Z80InstructionADCC(gb);
    DISPATCH();
ADC_REG_A_REG_D:
    _Z80InstructionADCD(gb);
    DISPATCH();
ADC_REG_A_REG_E:
    _Z80InstructionADCE(gb);
    DISPATCH();
ADC_REG_A_REG_H:
    _Z80InstructionADCH(gb);
    DISPATCH();
ADC_REG_A_REG_L:
    _Z80InstructionADCL(gb);
    DISPATCH();
ADC_REG_A_MEM_HL:
    _Z80InstructionADCHL(gb);
    DISPATCH();
ADC_REG_A_REG_A:
    _Z80InstructionADCA(gb);
    DISPATCH();
SUB_REG_A_REG_B:
    _Z80InstructionSUBB(gb);
    DISPATCH();
SUB_REG_A_REG_C:
    _Z80InstructionSUBC(gb);
    DISPATCH();
SUB_REG_A_REG_D:
    _Z80InstructionSUBD(gb);
    DISPATCH();
SUB_REG_A_REG_E:
    _Z80InstructionSUBE(gb);
    DISPATCH();
SUB_REG_A_REG_H:
    _Z80InstructionSUBH(gb);
    DISPATCH();
SUB_REG_A_REG_L:
    _Z80InstructionSUBL(gb);
    DISPATCH();
SUB_REG_A_MEM_HL:
    _Z80InstructionSUBHL(gb);
    DISPATCH();
SUB_REG_A_REG_A:
    _Z80InstructionSUBA(gb);
    DISPATCH();
SBC_REG_A_REG_B:
    _Z80InstructionSBCB(gb);
    DISPATCH();
SBC_REG_A_REG_C:
    _Z80InstructionSBCC(gb);
    DISPATCH();
SBC_REG_A_REG_D:
    _Z80InstructionSBCD(gb);
    DISPATCH();
SBC_REG_A_REG_E:
    _Z80InstructionSBCE(gb);
    DISPATCH();
SBC_REG_A_REG_H:
    _Z80InstructionSBCH(gb);
    DISPATCH();
SBC_REG_A_REG_L:
    _Z80InstructionSBCL(gb);
    DISPATCH();
SBC_REG_A_MEM_HL:
    _Z80InstructionSBCHL(gb);
    DISPATCH();
SBC_REG_A_REG_A:
    _Z80InstructionSBCA(gb);
    DISPATCH();
AND_REG_A_REG_B:
    _Z80InstructionANDB(gb);
    DISPATCH();
AND_REG_A_REG_C:
    _Z80InstructionANDC(gb);
    DISPATCH();
AND_REG_A_REG_D:
    _Z80InstructionANDD(gb);
    DISPATCH();
AND_REG_A_REG_E:
    _Z80InstructionANDE(gb);
    DISPATCH();
AND_REG_A_REG_H:
    _Z80InstructionANDH(gb);
    DISPATCH();
AND_REG_A_REG_L:
    _Z80InstructionANDL(gb);
    DISPATCH();
AND_REG_A_MEM_HL:
    _Z80InstructionANDHL(gb);
    DISPATCH();
AND_REG_A_REG_A:
    _Z80InstructionANDA(gb);
    DISPATCH();
XOR_REG_A_REG_B:
    _Z80InstructionXORB(gb);
    DISPATCH();
XOR_REG_A_REG_C:
    _Z80InstructionXORC(gb);
    DISPATCH();
XOR_REG_A_REG_D:
    _Z80InstructionXORD(gb);
    DISPATCH();
XOR_REG_A_REG_E:
    _Z80InstructionXORE(gb);
    DISPATCH();
XOR_REG_A_REG_H:
    _Z80InstructionXORH(gb);
    DISPATCH();
XOR_REG_A_REG_L:
    _Z80InstructionXORL(gb);
    DISPATCH();
XOR_REG_A_MEM_HL:
    _Z80InstructionXORHL(gb);
    DISPATCH();
XOR_REG_A_REG_A:
    _Z80InstructionXORA(gb);
    DISPATCH();
OR_REG_A_REG_B:
    _Z80InstructionORB(gb);
    DISPATCH();
OR_REG_A_REG_C:
    _Z80InstructionORC(gb);
    DISPATCH();
OR_REG_A_REG_D:
    _Z80InstructionORD(gb);
    DISPATCH();
OR_REG_A_REG_E:
    _Z80InstructionORE(gb);
    DISPATCH();
OR_REG_A_REG_H:
    _Z80InstructionORH(gb);
    DISPATCH();
OR_REG_A_REG_L:
    _Z80InstructionORL(gb);
    DISPATCH();
OR_REG_A_MEM_HL:
    _Z80InstructionORHL(gb);
    DISPATCH();
OR_REG_A_REG_A:
    _Z80InstructionORA(gb);
    DISPATCH();
CP_REG_A_REG_B:
    _Z80InstructionCPB(gb);
    DISPATCH();
CP_REG_A_REG_C:
    _Z80InstructionCPC(gb);
    DISPATCH();
CP_REG_A_REG_D:
    _Z80InstructionCPD(gb);
    DISPATCH();
CP_REG_A_REG_E:
    _Z80InstructionCPE(gb);
    DISPATCH();
CP_REG_A_REG_H:
    _Z80InstructionCPH(gb);
    DISPATCH();
CP_REG_A_REG_L:
    _Z80InstructionCPL(gb);
    DISPATCH();
CP_REG_A_MEM_HL:
    _Z80InstructionCPHL(gb);
    DISPATCH();
CP_REG_A_REG_A:
    _Z80InstructionCPA(gb);
    DISPATCH();
RET_CC_NZ_NONE:
    _Z80InstructionRETNZ(gb, &inst_cycles);
    DISPATCH();
POP_REG_BC_NONE:
    _Z80InstructionPOPBC(gb);
    DISPATCH();
JP_CC_NZ_IMM16:
    _Z80InstructionJPNZ(gb, &inst_cycles);
    DISPATCH();
JP_NONE_IMM16:
    _Z80InstructionJP(gb);
    DISPATCH();
CALL_CC_NZ_IMM16:
    _Z80InstructionCALLNZ(gb, &inst_cycles);
    DISPATCH();
PUSH_REG_BC_NONE:
    _Z80InstructionPUSHBC(gb);
    DISPATCH();
ADD_REG_A_IMM8:
    _Z80InstructionADDImm(gb);
    DISPATCH();
RST_NONE_MEM_0x00:
    _Z80InstructionRST00(gb);
    DISPATCH();
RET_CC_Z_NONE:
    _Z80InstructionRETZ(gb, &inst_cycles);
    DISPATCH();
RET_NONE_NONE:
    _Z80InstructionRET(gb);
    DISPATCH();
JP_CC_Z_IMM16:
    _Z80InstructionJPZ(gb, &inst_cycles);
    DISPATCH();
CB:
    inst_cycles = __gb_execute_cb(gb);
ERROR_NONE_NONE:
    (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
    DISPATCH();
CALL_CC_Z_IMM16:
    _Z80InstructionCALLZ(gb, &inst_cycles);
    DISPATCH();
CALL_NONE_IMM16:
    _Z80InstructionCALL(gb);
    DISPATCH();
ADC_REG_A_IMM8:
    _Z80InstructionADCImm(gb);
    DISPATCH();
RST_NONE_MEM_0x08:
    _Z80InstructionRST08(gb);
    DISPATCH();
RET_CC_NC_NONE:
    _Z80InstructionRETNC(gb, &inst_cycles);
    DISPATCH();
POP_REG_DE_NONE:
    _Z80InstructionPOPDE(gb);
    DISPATCH();
JP_CC_NC_IMM16:
    _Z80InstructionJPNC(gb, &inst_cycles);
    DISPATCH();
CALL_CC_NC_IMM16:
    _Z80InstructionCALLNC(gb, &inst_cycles);
    DISPATCH();
PUSH_REG_DE_NONE:
    _Z80InstructionPUSHDE(gb);
    DISPATCH();
SUB_REG_A_IMM8:
    _Z80InstructionSUBImm(gb);
    DISPATCH();
RST_NONE_MEM_0x10:
    _Z80InstructionRST10(gb);
    DISPATCH();
RET_CC_C_NONE:
    _Z80InstructionRETC(gb, &inst_cycles);
    DISPATCH();
RETI_NONE_NONE:
    _Z80InstructionRETI(gb);
    DISPATCH();
JP_CC_C_IMM16:
    _Z80InstructionJPC(gb, &inst_cycles);
    DISPATCH();
CALL_CC_C_IMM16:
    _Z80InstructionCALLC(gb, &inst_cycles);
    DISPATCH();
SBC_REG_A_IMM8:
    _Z80InstructionSBCImm(gb);
    DISPATCH();
RST_NONE_MEM_0x18:
    _Z80InstructionRST18(gb);
    DISPATCH();
LD_MEM_8_REG_A:
    _Z80InstructionLDImm_A(gb);
    DISPATCH();
POP_REG_HL_NONE:
    _Z80InstructionPOPHL(gb);
    DISPATCH();
LD_MEM_C_REG_A:
    _Z80InstructionLDImmC_A(gb);
    DISPATCH();
PUSH_REG_HL_NONE:
    _Z80InstructionPUSHHL(gb);
    DISPATCH();
AND_REG_A_IMM8:
    _Z80InstructionANDImm(gb);
    DISPATCH();
RST_NONE_MEM_0x20:
    _Z80InstructionRST20(gb);
    DISPATCH();
ADD16_REG_SP_IMM8:
    _Z80InstructionADDSP_Imm(gb);
    DISPATCH();
JP_NONE_MEM_HL:
    _Z80InstructionJPHL(gb);
    DISPATCH();
LD_MEM_16_REG_A:
    _Z80InstructionLDImm16_A(gb);
    DISPATCH();
XOR_REG_A_IMM8:
    _Z80InstructionXORImm(gb);
    DISPATCH();
RST_NONE_MEM_0x28:
    _Z80InstructionRST28(gb);
    DISPATCH();
LD_REG_A_MEM_8:
    _Z80InstructionLDA_Mem(gb);
    DISPATCH();
POP_REG_AF_NONE:
    _Z80InstructionPOPAF(gb);
    DISPATCH();
LD_REG_A_MEM_C:
    _Z80InstructionLDA_ImmC(gb);
    DISPATCH();
DI_NONE_NONE:
    _Z80InstructionDI(gb);
    DISPATCH();
PUSH_REG_AF_NONE:
    _Z80InstructionPUSHAF(gb);
    DISPATCH();
OR_REG_A_IMM8:
    _Z80InstructionORImm(gb);
    DISPATCH();
RST_NONE_MEM_0x30:
    _Z80InstructionRST30(gb);
    DISPATCH();
LD16_REG_HL_MEM_8:
    _Z80InstructionLDHL_SPImm(gb);
    DISPATCH();
LD16_REG_SP_REG_HL:
    _Z80InstructionLDSP_HL(gb);
    DISPATCH();
LD_REG_A_MEM_16:
    _Z80InstructionLDA_Imm16(gb);
    DISPATCH();
EI_NONE_NONE:
    _Z80InstructionEI(gb);
    DISPATCH();
CP_REG_A_IMM8:
    _Z80InstructionCPImm(gb);
    DISPATCH();
RST_NONE_MEM_0x38:
    _Z80InstructionRST38(gb);
    DISPATCH();
Tail_Handler:

    /* DIV register timing */
    gb->counter.div_count += inst_cycles;

    if (gb->counter.div_count >= DIV_CYCLES) {
        gb->gb_reg.DIV++;
        gb->counter.div_count -= DIV_CYCLES;
    }

    /* Check serial transmission */
    if (gb->gb_reg.SC & SERIAL_SC_TX_START) {
        /* If new transfer, call TX function. */
        if (gb->counter.serial_count == 0 && gb->gb_serial_tx != NULL)
            (gb->gb_serial_tx)(gb, gb->gb_reg.SB);

        gb->counter.serial_count += inst_cycles;

        /* If it's time to receive byte, call RX function. */
        if (gb->counter.serial_count >= SERIAL_CYCLES) {
            /* If RX can be done, do it. */
            /* If RX failed, do not change SB if using external
             * clock, or set to 0xFF if using internal clock. */
            uint8_t rx;

            if (gb->gb_serial_rx != NULL &&
                (gb->gb_serial_rx(gb, &rx) == GB_SERIAL_RX_SUCCESS)) {
                gb->gb_reg.SB = rx;

                /* Inform game of serial TX/RX completion. */
                gb->gb_reg.SC &= 0x01;
                gb->gb_reg.IF |= SERIAL_INTR;
            } else if (gb->gb_reg.SC & SERIAL_SC_CLOCK_SRC) {
                /* If using internal clock, and console is not
                 * attached to any external peripheral, shifted
                 * bits are replaced with logic 1. */
                gb->gb_reg.SB = 0xFF;

                /* Inform game of serial TX/RX completion. */
                gb->gb_reg.SC &= 0x01;
                gb->gb_reg.IF |= SERIAL_INTR;
            } else {
                /* If using external clock, and console is not
                 * attached to any external peripheral, bits are
                 * not shifted, so SB is not modified. */
            }

            gb->counter.serial_count = 0;
        }
    }

    /* TIMA register timing */
    /* TODO: Change tac_enable to struct of TAC timer control bits. */
    if (gb->gb_reg.tac_enable) {
        static const uint_fast16_t TAC_CYCLES[4] = {1024, 16, 64, 256};

        gb->counter.tima_count += inst_cycles;

        while (gb->counter.tima_count >= TAC_CYCLES[gb->gb_reg.tac_rate]) {
            gb->counter.tima_count -= TAC_CYCLES[gb->gb_reg.tac_rate];

            if (++gb->gb_reg.TIMA == 0) {
                gb->gb_reg.IF |= TIMER_INTR;
                /* On overflow, set TMA to TIMA. */
                gb->gb_reg.TIMA = gb->gb_reg.TMA;
            }
        }
    }

    /* TODO Check behaviour of LCD during LCD power off state. */
    /* If LCD is off, don't update LCD state.
     */
    if ((gb->gb_reg.LCDC & LCDC_ENABLE) == 0)
        return;

    /* LCD Timing */
    gb->counter.lcd_count += inst_cycles;

    /* New Scanline */
    if (gb->counter.lcd_count > LCD_LINE_CYCLES) {
        gb->counter.lcd_count -= LCD_LINE_CYCLES;

        /* LYC Update */
        if (gb->gb_reg.LY == gb->gb_reg.LYC) {
            gb->gb_reg.STAT |= STAT_LYC_COINC;

            if (gb->gb_reg.STAT & STAT_LYC_INTR)
                gb->gb_reg.IF |= LCDC_INTR;
        } else
            gb->gb_reg.STAT &= 0xFB;

        /* Next line */
        gb->gb_reg.LY = (gb->gb_reg.LY + 1) % LCD_VERT_LINES;

        /* VBLANK Start */
        if (gb->gb_reg.LY == LCD_HEIGHT) {
            gb->lcd_mode = LCD_VBLANK;
            gb->gb_frame = 1;
            gb->gb_reg.IF |= VBLANK_INTR;

            if (gb->gb_reg.STAT & STAT_MODE_1_INTR)
                gb->gb_reg.IF |= LCDC_INTR;

#if ENABLE_LCD
            /* If frame skip is activated, check if we need to draw
             * the frame or skip it. */
            if (gb->direct.frame_skip) {
                gb->display.frame_skip_count = !gb->display.frame_skip_count;
            }

            /* If interlaced is activated, change which lines get
             * updated. Also, only update lines on frames that are
             * actually drawn when frame skip is enabled. */
            if (gb->direct.interlace &&
                (!gb->direct.frame_skip || gb->display.frame_skip_count)) {
                gb->display.interlace_count = !gb->display.interlace_count;
            }
#endif
        }
        /* Normal Line */
        else if (gb->gb_reg.LY < LCD_HEIGHT) {
            if (gb->gb_reg.LY == 0) {
                /* Clear Screen */
                gb->display.WY = gb->gb_reg.WY;
                gb->display.window_clear = 0;
            }

            gb->lcd_mode = LCD_HBLANK;

            if (gb->gb_reg.STAT & STAT_MODE_0_INTR)
                gb->gb_reg.IF |= LCDC_INTR;
        }
    }
    /* OAM access */
    else if (gb->lcd_mode == LCD_HBLANK &&
             gb->counter.lcd_count >= LCD_MODE_2_CYCLES) {
        gb->lcd_mode = LCD_SEARCH_OAM;

        if (gb->gb_reg.STAT & STAT_MODE_2_INTR)
            gb->gb_reg.IF |= LCDC_INTR;
    }
    /* Update LCD */
    else if (gb->lcd_mode == LCD_SEARCH_OAM &&
             gb->counter.lcd_count >= LCD_MODE_3_CYCLES) {
        gb->lcd_mode = LCD_TRANSFER;
#if ENABLE_LCD
        __gb_draw_line(gb);
#endif
    }
}
