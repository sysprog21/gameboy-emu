#include "cpu.h"
#include "apu.h"
#include "mmu.h"
#include "instr.h"

#define DISPATCH() goto OUTPUT
#define DISPATCH_2() goto *dispatch_table[opcode]

uint8_t __cpu_execute_cb(struct gb_s *gb)
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
void cpu_step(struct gb_s *gb)
{
    uint8_t opcode, inst_cycles;
    cpu_instr table;
    static const void *dispatch_table[] = {
        &&NOP_NONE_NONE,       &&LD16_REG_BC_IMM16,   &&LD_MEM_BC_REG_A,
        &&INC16_REG_BC_NONE,   &&INC_REG_B_NONE,      &&DEC_REG_B_NONE,
        &&LD_REG_B_IMM8,       &&RLC_REG_A_NONE,      &&LD16_MEM_16_REG_SP,
        &&ADD16_REG_HL_REG_BC, &&LD_REG_A_MEM_BC,     &&DEC16_REG_BC_NONE,
        &&INC_REG_C_NONE,      &&DEC_REG_C_NONE,      &&LD_REG_C_IMM8,
        &&RRC_REG_A_NONE,      &&STOP_NONE_NONE,      &&LD16_REG_DE_IMM16,
        &&LD_MEM_DE_REG_A,     &&INC16_REG_DE_NONE,   &&INC_REG_D_NONE,
        &&DEC_REG_D_NONE,      &&LD_REG_D_IMM8,       &&RL_REG_A_NONE,
        &&JR_NONE_IMM8,        &&ADD16_REG_HL_REG_DE, &&LD_REG_A_MEM_DE,
        &&DEC16_REG_DE_NONE,   &&INC_REG_E_NONE,      &&DEC_REG_E_NONE,
        &&LD_REG_E_IMM8,       &&RR_REG_A_NONE,       &&JR_CC_NZ_IMM8,
        &&LD16_REG_HL_IMM16,   &&LD_MEM_INC_HL_REG_A, &&INC16_REG_HL_NONE,
        &&INC_REG_H_NONE,      &&DEC_REG_H_NONE,      &&LD_REG_H_IMM8,
        &&DAA_NONE_NONE,       &&JR_CC_Z_IMM8,        &&ADD16_REG_HL_REG_HL,
        &&LD_REG_A_MEM_INC_HL, &&DEC16_REG_HL_NONE,   &&INC_REG_L_NONE,
        &&DEC_REG_L_NONE,      &&LD_REG_L_IMM8,       &&CPL_REG_A_NONE,
        &&JR_CC_NC_IMM8,       &&LD16_REG_SP_IMM16,   &&LD_MEM_DEC_HL_REG_A,
        &&INC16_REG_SP_NONE,   &&INC_MEM_HL_NONE,     &&DEC_MEM_HL_NONE,
        &&LD_MEM_HL_IMM8,      &&SCF_NONE_NONE,       &&JR_CC_C_IMM8,
        &&ADD16_REG_HL_REG_SP, &&LD_REG_A_MEM_DEC_HL, &&DEC16_REG_SP_NONE,
        &&INC_REG_A_NONE,      &&DEC_REG_A_NONE,      &&LD_REG_A_IMM8,
        &&CCF_NONE_NONE,       &&NOP_NONE_NONE,       &&LD_REG_B_REG_C,
        &&LD_REG_B_REG_D,      &&LD_REG_B_REG_E,      &&LD_REG_B_REG_H,
        &&LD_REG_B_REG_L,      &&LD_REG_B_MEM_HL,     &&LD_REG_B_REG_A,
        &&LD_REG_C_REG_B,      &&NOP_NONE_NONE,       &&LD_REG_C_REG_D,
        &&LD_REG_C_REG_E,      &&LD_REG_C_REG_H,      &&LD_REG_C_REG_L,
        &&LD_REG_C_MEM_HL,     &&LD_REG_C_REG_A,      &&LD_REG_D_REG_B,
        &&LD_REG_D_REG_C,      &&NOP_NONE_NONE,       &&LD_REG_D_REG_E,
        &&LD_REG_D_REG_H,      &&LD_REG_D_REG_L,      &&LD_REG_D_MEM_HL,
        &&LD_REG_D_REG_A,      &&LD_REG_E_REG_B,      &&LD_REG_E_REG_C,
        &&LD_REG_E_REG_D,      &&NOP_NONE_NONE,       &&LD_REG_E_REG_H,
        &&LD_REG_E_REG_L,      &&LD_REG_E_MEM_HL,     &&LD_REG_E_REG_A,
        &&LD_REG_H_REG_B,      &&LD_REG_H_REG_C,      &&LD_REG_H_REG_D,
        &&LD_REG_H_REG_E,      &&NOP_NONE_NONE,       &&LD_REG_H_REG_L,
        &&LD_REG_H_MEM_HL,     &&LD_REG_H_REG_A,      &&LD_REG_L_REG_B,
        &&LD_REG_L_REG_C,      &&LD_REG_L_REG_D,      &&LD_REG_L_REG_E,
        &&LD_REG_L_REG_H,      &&NOP_NONE_NONE,       &&LD_REG_L_MEM_HL,
        &&LD_REG_L_REG_A,      &&LD_MEM_HL_REG_B,     &&LD_MEM_HL_REG_C,
        &&LD_MEM_HL_REG_D,     &&LD_MEM_HL_REG_E,     &&LD_MEM_HL_REG_H,
        &&LD_MEM_HL_REG_L,     &&HALT_NONE_NONE,      &&LD_MEM_HL_REG_A,
        &&LD_REG_A_REG_B,      &&LD_REG_A_REG_C,      &&LD_REG_A_REG_D,
        &&LD_REG_A_REG_E,      &&LD_REG_A_REG_H,      &&LD_REG_A_REG_L,
        &&LD_REG_A_MEM_HL,     &&NOP_NONE_NONE,       &&ADD_REG_A_REG_B,
        &&ADD_REG_A_REG_C,     &&ADD_REG_A_REG_D,     &&ADD_REG_A_REG_E,
        &&ADD_REG_A_REG_H,     &&ADD_REG_A_REG_L,     &&ADD_REG_A_MEM_HL,
        &&ADD_REG_A_REG_A,     &&ADC_REG_A_REG_B,     &&ADC_REG_A_REG_C,
        &&ADC_REG_A_REG_D,     &&ADC_REG_A_REG_E,     &&ADC_REG_A_REG_H,
        &&ADC_REG_A_REG_L,     &&ADC_REG_A_MEM_HL,    &&ADC_REG_A_REG_A,
        &&SUB_REG_A_REG_B,     &&SUB_REG_A_REG_C,     &&SUB_REG_A_REG_D,
        &&SUB_REG_A_REG_E,     &&SUB_REG_A_REG_H,     &&SUB_REG_A_REG_L,
        &&SUB_REG_A_MEM_HL,    &&SUB_REG_A_REG_A,     &&SBC_REG_A_REG_B,
        &&SBC_REG_A_REG_C,     &&SBC_REG_A_REG_D,     &&SBC_REG_A_REG_E,
        &&SBC_REG_A_REG_H,     &&SBC_REG_A_REG_L,     &&SBC_REG_A_MEM_HL,
        &&SBC_REG_A_REG_A,     &&AND_REG_A_REG_B,     &&AND_REG_A_REG_C,
        &&AND_REG_A_REG_D,     &&AND_REG_A_REG_E,     &&AND_REG_A_REG_H,
        &&AND_REG_A_REG_L,     &&AND_REG_A_MEM_HL,    &&AND_REG_A_REG_A,
        &&XOR_REG_A_REG_B,     &&XOR_REG_A_REG_C,     &&XOR_REG_A_REG_D,
        &&XOR_REG_A_REG_E,     &&XOR_REG_A_REG_H,     &&XOR_REG_A_REG_L,
        &&XOR_REG_A_MEM_HL,    &&XOR_REG_A_REG_A,     &&OR_REG_A_REG_B,
        &&OR_REG_A_REG_C,      &&OR_REG_A_REG_D,      &&OR_REG_A_REG_E,
        &&OR_REG_A_REG_H,      &&OR_REG_A_REG_L,      &&OR_REG_A_MEM_HL,
        &&OR_REG_A_REG_A,      &&CP_REG_A_REG_B,      &&CP_REG_A_REG_C,
        &&CP_REG_A_REG_D,      &&CP_REG_A_REG_E,      &&CP_REG_A_REG_H,
        &&CP_REG_A_REG_L,      &&CP_REG_A_MEM_HL,     &&CP_REG_A_REG_A,
        &&RET_CC_NZ_NONE,      &&POP_REG_BC_NONE,     &&JP_CC_NZ_IMM16,
        &&JP_NONE_IMM16,       &&CALL_CC_NZ_IMM16,    &&PUSH_REG_BC_NONE,
        &&ADD_REG_A_IMM8,      &&RST_NONE_MEM_0x00,   &&RET_CC_Z_NONE,
        &&RET_NONE_NONE,       &&JP_CC_Z_IMM16,       &&CB,
        &&CALL_CC_Z_IMM16,     &&CALL_NONE_IMM16,     &&ADC_REG_A_IMM8,
        &&RST_NONE_MEM_0x08,   &&RET_CC_NC_NONE,      &&POP_REG_DE_NONE,
        &&JP_CC_NC_IMM16,      &&ERROR_NONE_NONE,     &&CALL_CC_NC_IMM16,
        &&PUSH_REG_DE_NONE,    &&SUB_REG_A_IMM8,      &&RST_NONE_MEM_0x10,
        &&RET_CC_C_NONE,       &&RETI_NONE_NONE,      &&JP_CC_C_IMM16,
        &&ERROR_NONE_NONE,     &&CALL_CC_C_IMM16,     &&ERROR_NONE_NONE,
        &&SBC_REG_A_IMM8,      &&RST_NONE_MEM_0x18,   &&LD_MEM_8_REG_A,
        &&POP_REG_HL_NONE,     &&LD_MEM_C_REG_A,      &&ERROR_NONE_NONE,
        &&ERROR_NONE_NONE,     &&PUSH_REG_HL_NONE,    &&AND_REG_A_IMM8,
        &&RST_NONE_MEM_0x20,   &&ADD16_REG_SP_IMM8,   &&JP_NONE_MEM_HL,
        &&LD_MEM_16_REG_A,     &&ERROR_NONE_NONE,     &&ERROR_NONE_NONE,
        &&ERROR_NONE_NONE,     &&XOR_REG_A_IMM8,      &&RST_NONE_MEM_0x28,
        &&LD_REG_A_MEM_8,      &&POP_REG_AF_NONE,     &&LD_REG_A_MEM_C,
        &&DI_NONE_NONE,        &&ERROR_NONE_NONE,     &&PUSH_REG_AF_NONE,
        &&OR_REG_A_IMM8,       &&RST_NONE_MEM_0x30,   &&LD16_REG_HL_MEM_8,
        &&LD16_REG_SP_REG_HL,  &&LD_REG_A_MEM_16,     &&EI_NONE_NONE,
        &&ERROR_NONE_NONE,     &&ERROR_NONE_NONE,     &&CP_REG_A_IMM8,
        &&RST_NONE_MEM_0x38};

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
    opcode = (gb->gb_halt ? 0x00 : __gb_read(gb, gb->cpu_reg.pc++));
    table = instr_table[opcode];
    inst_cycles = table.alt_cycles << 2;

    DISPATCH_2();

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
LD_REG_L_REG_A:
    _Z80InstructionLDL_A(gb);
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
    inst_cycles = __cpu_execute_cb(gb);
    DISPATCH();
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
OUTPUT:
    __cpu_timer(gb, inst_cycles);

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

void __cpu_timer(struct gb_s *gb, uint8_t inst_cycles)
{
    /* DIV register timing */
    gb->counter.div_count += inst_cycles;

    if (gb->counter.div_count >= DIV_CYCLES) {
        gb->gb_reg.DIV++;
        gb->counter.div_count -= DIV_CYCLES;
    }
}
