#pragma once

#include "cpu.h"

static const cpu_instr_s instr_table[] = {
    [0x00] = {"NOP", 1, false, false, op_nop},
    [0x01] = {"LD BC, %04x", 3, false, false, op_ld_bc_d16},
    [0x02] = {"LD (BC), A", 1, false, false, op_ld_bcp_a},
    [0x03] = {"INC BC", 1, false, false, op_inc_bc},
    [0x04] = {"INC B", 1, false, false, op_inc_b},
    [0x05] = {"DEC B", 1, false, false, op_dec_b},
    [0x06] = {"LD B, %02x", 2, false, false, op_ld_b_d8},
    [0x07] = {"RLCA", 1, false, false, op_rlca},
    [0x08] = {"LD (%04x), SP", 3, false, false, op_ld_a16p_sp},
    [0x09] = {"ADD HL, BC", 1, false, false, op_add_hl_bc},
    [0x0A] = {"LD A, (BC)", 1, false, false, op_ld_a_bcp},
    [0x0B] = {"DEC BC", 1, false, false, op_dec_bc},
    [0x0C] = {"INC C", 1, false, false, op_inc_c},
    [0x0D] = {"DEC C", 1, false, false, op_dec_c},
    [0x0E] = {"LD C, %02x", 2, false, false, op_ld_c_d8},
    [0x0F] = {"RRCA", 1, false, false, op_rrca},
    [0x10] = {"STOP", 1, false, false, op_stop},
    [0x11] = {"LD DE, %04x", 3, false, false, op_ld_de_d16},
    [0x12] = {"LD (DE), A", 1, false, false, op_ld_dep_a},
    [0x13] = {"INC DE", 1, false, false, op_inc_de},
    [0x14] = {"INC D", 1, false, false, op_inc_d},
    [0x15] = {"DEC D", 1, false, false, op_dec_d},
    [0x16] = {"LD D, %02x", 2, false, false, op_ld_d_d8},
    [0x17] = {"RLA", 1, false, false, op_rla},
    [0x18] = {"JR %02x", 2, true, false, op_jr_r8},
    [0x19] = {"ADD HL, DE", 1, false, false, op_add_hl_de},
    [0x1A] = {"LD A, (DE)", 1, false, false, op_ld_a_dep},
    [0x1B] = {"DEC DE", 1, false, false, op_dec_de},
    [0x1C] = {"INC E", 1, false, false, op_inc_e},
    [0x1D] = {"DEC E", 1, false, false, op_dec_e},
    [0x1E] = {"LD E, %02x", 2, false, false, op_ld_e_d8},
    [0x1F] = {"RRA", 1, false, false, op_rra},
    [0x20] = {"JR NZ, %02x", 2, true, false, op_jr_nz_r8},
    [0x21] = {"LD HL, %04x", 3, false, false, op_ld_hl_d16},
    [0x22] = {"LD (HL+), A", 1, false, false, op_ld_hlpp_a},
    [0x23] = {"INC HL", 1, false, false, op_inc_hl},
    [0x24] = {"INC H", 1, false, false, op_inc_h},
    [0x25] = {"DEC H", 1, false, false, op_dec_h},
    [0x26] = {"LD H, %02x", 2, false, false, op_ld_h_d8},
    [0x27] = {"DAA", 1, false, false, op_daa},
    [0x28] = {"JR Z, %02x", 2, true, false, op_jr_z_r8},
    [0x29] = {"ADD HL, HL", 1, false, false, op_add_hl_hl},
    [0x2A] = {"LD A, (HL+)", 1, false, false, op_ld_a_hlpp},
    [0x2B] = {"DEC HL", 1, false, false, op_dec_hl},
    [0x2C] = {"INC L", 1, false, false, op_inc_l},
    [0x2D] = {"DEC L", 1, false, false, op_dec_l},
    [0x2E] = {"LD L, %02x", 2, false, false, op_ld_l_d8},
    [0x2F] = {"CPL", 1, false, false, op_cpl},
    [0x30] = {"JR NC, %02x", 2, true, false, op_jr_nc_r8},
    [0x31] = {"LD SP, %04x", 3, false, false, op_ld_sp_d16},
    [0x32] = {"LD (HL-), A", 1, false, false, op_ld_hlmp_a},
    [0x33] = {"INC SP", 1, false, false, op_inc_sp},
    [0x34] = {"INC (HL)", 1, false, false, op_inc_hlp},
    [0x35] = {"DEC (HL)", 1, false, false, op_dec_hlp},
    [0x36] = {"LD (HL), %02x", 2, false, false, op_ld_hlp_d8},
    [0x37] = {"SCF", 1, false, false, op_scf},
    [0x38] = {"JR C, %02x", 2, true, false, op_jr_c_r8},
    [0x39] = {"ADD HL, SP", 1, false, false, op_add_hl_sp},
    [0x3A] = {"LD A, (HL-)", 1, false, false, op_ld_a_hlmp},
    [0x3B] = {"DEC SP", 1, false, false, op_dec_sp},
    [0x3C] = {"INC A", 1, false, false, op_inc_a},
    [0x3D] = {"DEC A", 1, false, false, op_dec_a},
    [0x3E] = {"LD A, %02x", 2, false, false, op_ld_a_d8},
    [0x3F] = {"CCF", 1, false, false, op_ccf},
    [0x40] = {"LD B, B", 1, false, false, op_ld_b_b},
    [0x41] = {"LD B, C", 1, false, false, op_ld_b_c},
    [0x42] = {"LD B, D", 1, false, false, op_ld_b_d},
    [0x43] = {"LD B, E", 1, false, false, op_ld_b_e},
    [0x44] = {"LD B, H", 1, false, false, op_ld_b_h},
    [0x45] = {"LD B, L", 1, false, false, op_ld_b_l},
    [0x46] = {"LD B, (HL)", 1, false, false, op_ld_b_hlp},
    [0x47] = {"LD B, A", 1, false, false, op_ld_b_a},
    [0x48] = {"LD C, B", 1, false, false, op_ld_c_b},
    [0x49] = {"LD C, C", 1, false, false, op_ld_c_c},
    [0x4A] = {"LD C, D", 1, false, false, op_ld_c_d},
    [0x4B] = {"LD C, E", 1, false, false, op_ld_c_e},
    [0x4C] = {"LD C, H", 1, false, false, op_ld_c_h},
    [0x4D] = {"LD C, L", 1, false, false, op_ld_c_l},
    [0x4E] = {"LD C, (HL)", 1, false, false, op_ld_c_hlp},
    [0x4F] = {"LD C, A", 1, false, false, op_ld_c_a},
    [0x50] = {"LD D, B", 1, false, false, op_ld_d_b},
    [0x51] = {"LD D, C", 1, false, false, op_ld_d_c},
    [0x52] = {"LD D, D", 1, false, false, op_ld_d_d},
    [0x53] = {"LD D, E", 1, false, false, op_ld_d_e},
    [0x54] = {"LD D, H", 1, false, false, op_ld_d_h},
    [0x55] = {"LD D, L", 1, false, false, op_ld_d_l},
    [0x56] = {"LD D, (HL)", 1, false, false, op_ld_d_hlp},
    [0x57] = {"LD D, A", 1, false, false, op_ld_d_a},
    [0x58] = {"LD E, B", 1, false, false, op_ld_e_b},
    [0x59] = {"LD E, C", 1, false, false, op_ld_e_c},
    [0x5A] = {"LD E, D", 1, false, false, op_ld_e_d},
    [0x5B] = {"LD E, E", 1, false, false, op_ld_e_e},
    [0x5C] = {"LD E, H", 1, false, false, op_ld_e_h},
    [0x5D] = {"LD E, L", 1, false, false, op_ld_e_l},
    [0x5E] = {"LD E, (HL)", 1, false, false, op_ld_e_hlp},
    [0x5F] = {"LD E, A", 1, false, false, op_ld_e_a},
    [0x60] = {"LD H, B", 1, false, false, op_ld_h_b},
    [0x61] = {"LD H, C", 1, false, false, op_ld_h_c},
    [0x62] = {"LD H, D", 1, false, false, op_ld_h_d},
    [0x63] = {"LD H, E", 1, false, false, op_ld_h_e},
    [0x64] = {"LD H, H", 1, false, false, op_ld_h_h},
    [0x65] = {"LD H, L", 1, false, false, op_ld_h_l},
    [0x66] = {"LD H, (HL)", 1, false, false, op_ld_h_hlp},
    [0x67] = {"LD H, A", 1, false, false, op_ld_h_a},
    [0x68] = {"LD L, B", 1, false, false, op_ld_l_b},
    [0x69] = {"LD L, C", 1, false, false, op_ld_l_c},
    [0x6A] = {"LD L, D", 1, false, false, op_ld_l_d},
    [0x6B] = {"LD L, E", 1, false, false, op_ld_l_e},
    [0x6C] = {"LD L, H", 1, false, false, op_ld_l_h},
    [0x6D] = {"LD L, L", 1, false, false, op_ld_l_l},
    [0x6E] = {"LD L, (HL)", 1, false, false, op_ld_l_hlp},
    [0x6F] = {"LD L, A", 1, false, false, op_ld_l_a},
    [0x70] = {"LD (HL), B", 1, false, false, op_ld_hlp_b},
    [0x71] = {"LD (HL), C", 1, false, false, op_ld_hlp_c},
    [0x72] = {"LD (HL), D", 1, false, false, op_ld_hlp_d},
    [0x73] = {"LD (HL), E", 1, false, false, op_ld_hlp_e},
    [0x74] = {"LD (HL), H", 1, false, false, op_ld_hlp_h},
    [0x75] = {"LD (HL), L", 1, false, false, op_ld_hlp_l},
    [0x76] = {"HALT", 1, false, false, op_halt},
    [0x77] = {"LD (HL), A", 1, false, false, op_ld_hlp_a},
    [0x78] = {"LD A, B", 1, false, false, op_ld_a_b},
    [0x79] = {"LD A, C", 1, false, false, op_ld_a_c},
    [0x7A] = {"LD A, D", 1, false, false, op_ld_a_d},
    [0x7B] = {"LD A, E", 1, false, false, op_ld_a_e},
    [0x7C] = {"LD A, H", 1, false, false, op_ld_a_h},
    [0x7D] = {"LD A, L", 1, false, false, op_ld_a_l},
    [0x7E] = {"LD A, (HL)", 1, false, false, op_ld_a_hlp},
    [0x7F] = {"LD A, A", 1, false, false, op_ld_a_a},
    [0x80] = {"ADD A, B", 1, false, false, op_add_a_b},
    [0x81] = {"ADD A, C", 1, false, false, op_add_a_c},
    [0x82] = {"ADD A, D", 1, false, false, op_add_a_d},
    [0x83] = {"ADD A, E", 1, false, false, op_add_a_e},
    [0x84] = {"ADD A, H", 1, false, false, op_add_a_h},
    [0x85] = {"ADD A, L", 1, false, false, op_add_a_l},
    [0x86] = {"ADD A, (HL)", 1, false, false, op_add_a_hlp},
    [0x87] = {"ADD A, A", 1, false, false, op_add_a_a},
    [0x88] = {"ADC A, B", 1, false, false, op_adc_a_b},
    [0x89] = {"ADC A, C", 1, false, false, op_adc_a_c},
    [0x8A] = {"ADC A, D", 1, false, false, op_adc_a_d},
    [0x8B] = {"ADC A, E", 1, false, false, op_adc_a_e},
    [0x8C] = {"ADC A, H", 1, false, false, op_adc_a_h},
    [0x8D] = {"ADC A, L", 1, false, false, op_adc_a_l},
    [0x8E] = {"ADC A, (HL)", 1, false, false, op_adc_a_hlp},
    [0x8F] = {"ADC A, A", 1, false, false, op_adc_a_a},
    [0x90] = {"SUB B", 1, false, false, op_sub_b},
    [0x91] = {"SUB C", 1, false, false, op_sub_c},
    [0x92] = {"SUB D", 1, false, false, op_sub_d},
    [0x93] = {"SUB E", 1, false, false, op_sub_e},
    [0x94] = {"SUB H", 1, false, false, op_sub_h},
    [0x95] = {"SUB L", 1, false, false, op_sub_l},
    [0x96] = {"SUB (HL)", 1, false, false, op_sub_hlp},
    [0x97] = {"SUB A", 1, false, false, op_sub_a},
    [0x98] = {"SBC A, B", 1, false, false, op_sbc_a_b},
    [0x99] = {"SBC A, C", 1, false, false, op_sbc_a_c},
    [0x9A] = {"SBC A, D", 1, false, false, op_sbc_a_d},
    [0x9B] = {"SBC A, E", 1, false, false, op_sbc_a_e},
    [0x9C] = {"SBC A, H", 1, false, false, op_sbc_a_h},
    [0x9D] = {"SBC A, L", 1, false, false, op_sbc_a_l},
    [0x9E] = {"SBC A, (HL)", 1, false, false, op_sbc_a_hlp},
    [0x9F] = {"SBC A, A", 1, false, false, op_sbc_a_a},
    [0xA0] = {"AND B", 1, false, false, op_and_b},
    [0xA1] = {"AND C", 1, false, false, op_and_c},
    [0xA2] = {"AND D", 1, false, false, op_and_d},
    [0xA3] = {"AND E", 1, false, false, op_and_e},
    [0xA4] = {"AND H", 1, false, false, op_and_h},
    [0xA5] = {"AND L", 1, false, false, op_and_l},
    [0xA6] = {"AND (HL)", 1, false, false, op_and_hlp},
    [0xA7] = {"AND A", 1, false, false, op_and_a},
    [0xA8] = {"XOR B", 1, false, false, op_xor_b},
    [0xA9] = {"XOR C", 1, false, false, op_xor_c},
    [0xAA] = {"XOR D", 1, false, false, op_xor_d},
    [0xAB] = {"XOR E", 1, false, false, op_xor_e},
    [0xAC] = {"XOR H", 1, false, false, op_xor_h},
    [0xAD] = {"XOR L", 1, false, false, op_xor_l},
    [0xAE] = {"XOR (HL)", 1, false, false, op_xor_hlp},
    [0xAF] = {"XOR A", 1, false, false, op_xor_a},
    [0xB0] = {"OR B", 1, false, false, op_or_b},
    [0xB1] = {"OR C", 1, false, false, op_or_c},
    [0xB2] = {"OR D", 1, false, false, op_or_d},
    [0xB3] = {"OR E", 1, false, false, op_or_e},
    [0xB4] = {"OR H", 1, false, false, op_or_h},
    [0xB5] = {"OR L", 1, false, false, op_or_l},
    [0xB6] = {"OR (HL)", 1, false, false, op_or_hlp},
    [0xB7] = {"OR A", 1, false, false, op_or_a},
    [0xB8] = {"CP B", 1, false, false, op_cp_b},
    [0xB9] = {"CP C", 1, false, false, op_cp_c},
    [0xBA] = {"CP D", 1, false, false, op_cp_d},
    [0xBB] = {"CP E", 1, false, false, op_cp_e},
    [0xBC] = {"CP H", 1, false, false, op_cp_h},
    [0xBD] = {"CP L", 1, false, false, op_cp_l},
    [0xBE] = {"CP (HL)", 1, false, false, op_cp_hlp},
    [0xBF] = {"CP A", 1, false, false, op_cp_a},
    [0xC0] = {"RET NZ", 1, false, false, op_ret_nz},
    [0xC1] = {"POP BC", 1, false, false, op_pop_bc},
    [0xC2] = {"JP NZ, %04x", 3, false, false, op_jp_nz_a16},
    [0xC3] = {"JP %04x", 3, false, false, op_jp_a16},
    [0xC4] = {"CALL NZ, %04x", 3, false, false, op_call_nz_a16},
    [0xC5] = {"PUSH BC", 1, false, false, op_push_bc},
    [0xC6] = {"ADD A, %02x", 2, false, false, op_add_a_d8},
    [0xC7] = {"RST 00H", 1, false, false, op_rst_00h},
    [0xC8] = {"RET Z", 1, false, false, op_ret_z},
    [0xC9] = {"RET", 1, false, false, op_ret},
    [0xCA] = {"JP Z, %04x", 3, false, false, op_jp_z_a16},
    [0xCB] = {"PREFIX CB", 1, false, false, op_prefix_cb},
    [0xCC] = {"CALL Z, %04x", 3, false, false, op_call_z_a16},
    [0xCD] = {"CALL %04x", 3, false, false, op_call_a16},
    [0xCE] = {"ADC A, %02x", 2, false, false, op_adc_a_d8},
    [0xCF] = {"RST 08H", 1, false, false, op_rst_08h},
    [0xD0] = {"RET NC", 1, false, false, op_ret_nc},
    [0xD1] = {"POP DE", 1, false, false, op_pop_de},
    [0xD2] = {"JP NC, %04x", 3, false, false, op_jp_nc_a16},
    [0xD3] = {"NOP", 1, false, false, op_nop},
    [0xD4] = {"CALL NC, %04x", 3, false, false, op_call_nc_a16},
    [0xD5] = {"PUSH DE", 1, false, false, op_push_de},
    [0xD6] = {"SUB %02x", 2, false, false, op_sub_d8},
    [0xD7] = {"RST 10H", 1, false, false, op_rst_10h},
    [0xD8] = {"RET C", 1, false, false, op_ret_c},
    [0xD9] = {"RETI", 1, false, false, op_reti},
    [0xDA] = {"JP C, %04x", 3, false, false, op_jp_c_a16},
    [0xDB] = {"NOP", 1, false, false, op_nop},
    [0xDC] = {"CALL C, %04x", 3, false, false, op_call_c_a16},
    [0xDD] = {"NOP", 1, false, false, op_nop},
    [0xDE] = {"SBC A, %02x", 2, false, false, op_sbc_a_d8},
    [0xDF] = {"RST 18H", 1, false, false, op_rst_18h},
    [0xE0] = {"LDH (%02x), A", 2, false, false, op_ldh_a8p_a},
    [0xE1] = {"POP HL", 1, false, false, op_pop_hl},
    [0xE2] = {"LD (C), A", 1, false, false, op_ld_cp_a},
    [0xE3] = {"NOP", 1, false, false, op_nop},
    [0xE4] = {"NOP", 1, false, false, op_nop},
    [0xE5] = {"PUSH HL", 1, false, false, op_push_hl},
    [0xE6] = {"AND %02x", 2, false, false, op_and_d8},
    [0xE7] = {"RST 20H", 1, false, false, op_rst_20h},
    [0xE8] = {"ADD SP, %02x", 2, true, false, op_add_sp_r8},
    [0xE9] = {"JP (HL)", 1, false, false, op_jp_hlp},
    [0xEA] = {"LD (%04x), A", 3, false, false, op_ld_a16p_a},
    [0xEB] = {"NOP", 1, false, false, op_nop},
    [0xEC] = {"NOP", 1, false, false, op_nop},
    [0xED] = {"NOP", 1, false, false, op_nop},
    [0xEE] = {"XOR %02x", 2, false, false, op_xor_d8},
    [0xEF] = {"RST 28H", 1, false, false, op_rst_28h},
    [0xF0] = {"LDH A, (%02x)", 2, false, false, op_ldh_a_a8p},
    [0xF1] = {"POP AF", 1, false, false, op_pop_af},
    [0xF2] = {"LD A, (C)", 1, false, false, op_ld_a_cp},
    [0xF3] = {"DI", 1, false, false, op_di},
    [0xF4] = {"NOP", 1, false, false, op_nop},
    [0xF5] = {"PUSH AF", 1, false, false, op_push_af},
    [0xF6] = {"OR %02x", 2, false, false, op_or_d8},
    [0xF7] = {"RST 30H", 1, false, false, op_rst_30h},
    [0xF8] = {"LD HL, SP+%02x", 2, true, false, op_ld_hl_sppr8},
    [0xF9] = {"LD SP, HL", 1, false, false, op_ld_sp_hl},
    [0xFA] = {"LD A, (%04x)", 3, false, false, op_ld_a_a16p},
    [0xFB] = {"EI", 1, false, false, op_ei},
    [0xFC] = {"NOP", 1, false, false, op_nop},
    [0xFD] = {"NOP", 1, false, false, op_nop},
    [0xFE] = {"CP %02x", 2, false, false, op_cp_d8},
    [0xFF] = {"RST 38H", 1, false, false, op_rst_38h}
};

static const cpu_instr_s cb_instr_table[] = {
    [0x00] = {"RLC B", 2, false, true, op_rlc_b},
    [0x01] = {"RLC C", 2, false, true, op_rlc_c},
    [0x02] = {"RLC D", 2, false, true, op_rlc_d},
    [0x03] = {"RLC E", 2, false, true, op_rlc_e},
    [0x04] = {"RLC H", 2, false, true, op_rlc_h},
    [0x05] = {"RLC L", 2, false, true, op_rlc_l},
    [0x06] = {"RLC (HL)", 2, false, true, op_rlc_hlp},
    [0x07] = {"RLC A", 2, false, true, op_rlc_a},
    [0x08] = {"RRC B", 2, false, true, op_rrc_b},
    [0x09] = {"RRC C", 2, false, true, op_rrc_c},
    [0x0A] = {"RRC D", 2, false, true, op_rrc_d},
    [0x0B] = {"RRC E", 2, false, true, op_rrc_e},
    [0x0C] = {"RRC H", 2, false, true, op_rrc_h},
    [0x0D] = {"RRC L", 2, false, true, op_rrc_l},
    [0x0E] = {"RRC (HL)", 2, false, true, op_rrc_hlp},
    [0x0F] = {"RRC A", 2, false, true, op_rrc_a},
    [0x10] = {"RL B", 2, false, true, op_rl_b},
    [0x11] = {"RL C", 2, false, true, op_rl_c},
    [0x12] = {"RL D", 2, false, true, op_rl_d},
    [0x13] = {"RL E", 2, false, true, op_rl_e},
    [0x14] = {"RL H", 2, false, true, op_rl_h},
    [0x15] = {"RL L", 2, false, true, op_rl_l},
    [0x16] = {"RL (HL)", 2, false, true, op_rl_hlp},
    [0x17] = {"RL A", 2, false, true, op_rl_a},
    [0x18] = {"RR B", 2, false, true, op_rr_b},
    [0x19] = {"RR C", 2, false, true, op_rr_c},
    [0x1A] = {"RR D", 2, false, true, op_rr_d},
    [0x1B] = {"RR E", 2, false, true, op_rr_e},
    [0x1C] = {"RR H", 2, false, true, op_rr_h},
    [0x1D] = {"RR L", 2, false, true, op_rr_l},
    [0x1E] = {"RR (HL)", 2, false, true, op_rr_hlp},
    [0x1F] = {"RR A", 2, false, true, op_rr_a},
    [0x20] = {"SLA B", 2, false, true, op_sla_b},
    [0x21] = {"SLA C", 2, false, true, op_sla_c},
    [0x22] = {"SLA D", 2, false, true, op_sla_d},
    [0x23] = {"SLA E", 2, false, true, op_sla_e},
    [0x24] = {"SLA H", 2, false, true, op_sla_h},
    [0x25] = {"SLA L", 2, false, true, op_sla_l},
    [0x26] = {"SLA (HL)", 2, false, true, op_sla_hlp},
    [0x27] = {"SLA A", 2, false, true, op_sla_a},
    [0x28] = {"SRA B", 2, false, true, op_sra_b},
    [0x29] = {"SRA C", 2, false, true, op_sra_c},
    [0x2A] = {"SRA D", 2, false, true, op_sra_d},
    [0x2B] = {"SRA E", 2, false, true, op_sra_e},
    [0x2C] = {"SRA H", 2, false, true, op_sra_h},
    [0x2D] = {"SRA L", 2, false, true, op_sra_l},
    [0x2E] = {"SRA (HL)", 2, false, true, op_sra_hlp},
    [0x2F] = {"SRA A", 2, false, true, op_sra_a},
    [0x30] = {"SWAP B", 2, false, true, op_swap_b},
    [0x31] = {"SWAP C", 2, false, true, op_swap_c},
    [0x32] = {"SWAP D", 2, false, true, op_swap_d},
    [0x33] = {"SWAP E", 2, false, true, op_swap_e},
    [0x34] = {"SWAP H", 2, false, true, op_swap_h},
    [0x35] = {"SWAP L", 2, false, true, op_swap_l},
    [0x36] = {"SWAP (HL)", 2, false, true, op_swap_hlp},
    [0x37] = {"SWAP A", 2, false, true, op_swap_a},
    [0x38] = {"SRL B", 2, false, true, op_srl_b},
    [0x39] = {"SRL C", 2, false, true, op_srl_c},
    [0x3A] = {"SRL D", 2, false, true, op_srl_d},
    [0x3B] = {"SRL E", 2, false, true, op_srl_e},
    [0x3C] = {"SRL H", 2, false, true, op_srl_h},
    [0x3D] = {"SRL L", 2, false, true, op_srl_l},
    [0x3E] = {"SRL (HL)", 2, false, true, op_srl_hlp},
    [0x3F] = {"SRL A", 2, false, true, op_srl_a},
    [0x40] = {"BIT 0, B", 2, false, true, op_bit_0_b},
    [0x41] = {"BIT 0, C", 2, false, true, op_bit_0_c},
    [0x42] = {"BIT 0, D", 2, false, true, op_bit_0_d},
    [0x43] = {"BIT 0, E", 2, false, true, op_bit_0_e},
    [0x44] = {"BIT 0, H", 2, false, true, op_bit_0_h},
    [0x45] = {"BIT 0, L", 2, false, true, op_bit_0_l},
    [0x46] = {"BIT 0, (HL)", 2, false, true, op_bit_0_hlp},
    [0x47] = {"BIT 0, A", 2, false, true, op_bit_0_a},
    [0x48] = {"BIT 1, B", 2, false, true, op_bit_1_b},
    [0x49] = {"BIT 1, C", 2, false, true, op_bit_1_c},
    [0x4A] = {"BIT 1, D", 2, false, true, op_bit_1_d},
    [0x4B] = {"BIT 1, E", 2, false, true, op_bit_1_e},
    [0x4C] = {"BIT 1, H", 2, false, true, op_bit_1_h},
    [0x4D] = {"BIT 1, L", 2, false, true, op_bit_1_l},
    [0x4E] = {"BIT 1, (HL)", 2, false, true, op_bit_1_hlp},
    [0x4F] = {"BIT 1, A", 2, false, true, op_bit_1_a},
    [0x50] = {"BIT 2, B", 2, false, true, op_bit_2_b},
    [0x51] = {"BIT 2, C", 2, false, true, op_bit_2_c},
    [0x52] = {"BIT 2, D", 2, false, true, op_bit_2_d},
    [0x53] = {"BIT 2, E", 2, false, true, op_bit_2_e},
    [0x54] = {"BIT 2, H", 2, false, true, op_bit_2_h},
    [0x55] = {"BIT 2, L", 2, false, true, op_bit_2_l},
    [0x56] = {"BIT 2, (HL)", 2, false, true, op_bit_2_hlp},
    [0x57] = {"BIT 2, A", 2, false, true, op_bit_2_a},
    [0x58] = {"BIT 3, B", 2, false, true, op_bit_3_b},
    [0x59] = {"BIT 3, C", 2, false, true, op_bit_3_c},
    [0x5A] = {"BIT 3, D", 2, false, true, op_bit_3_d},
    [0x5B] = {"BIT 3, E", 2, false, true, op_bit_3_e},
    [0x5C] = {"BIT 3, H", 2, false, true, op_bit_3_h},
    [0x5D] = {"BIT 3, L", 2, false, true, op_bit_3_l},
    [0x5E] = {"BIT 3, (HL)", 2, false, true, op_bit_3_hlp},
    [0x5F] = {"BIT 3, A", 2, false, true, op_bit_3_a},
    [0x60] = {"BIT 4, B", 2, false, true, op_bit_4_b},
    [0x61] = {"BIT 4, C", 2, false, true, op_bit_4_c},
    [0x62] = {"BIT 4, D", 2, false, true, op_bit_4_d},
    [0x63] = {"BIT 4, E", 2, false, true, op_bit_4_e},
    [0x64] = {"BIT 4, H", 2, false, true, op_bit_4_h},
    [0x65] = {"BIT 4, L", 2, false, true, op_bit_4_l},
    [0x66] = {"BIT 4, (HL)", 2, false, true, op_bit_4_hlp},
    [0x67] = {"BIT 4, A", 2, false, true, op_bit_4_a},
    [0x68] = {"BIT 5, B", 2, false, true, op_bit_5_b},
    [0x69] = {"BIT 5, C", 2, false, true, op_bit_5_c},
    [0x6A] = {"BIT 5, D", 2, false, true, op_bit_5_d},
    [0x6B] = {"BIT 5, E", 2, false, true, op_bit_5_e},
    [0x6C] = {"BIT 5, H", 2, false, true, op_bit_5_h},
    [0x6D] = {"BIT 5, L", 2, false, true, op_bit_5_l},
    [0x6E] = {"BIT 5, (HL)", 2, false, true, op_bit_5_hlp},
    [0x6F] = {"BIT 5, A", 2, false, true, op_bit_5_a},
    [0x70] = {"BIT 6, B", 2, false, true, op_bit_6_b},
    [0x71] = {"BIT 6, C", 2, false, true, op_bit_6_c},
    [0x72] = {"BIT 6, D", 2, false, true, op_bit_6_d},
    [0x73] = {"BIT 6, E", 2, false, true, op_bit_6_e},
    [0x74] = {"BIT 6, H", 2, false, true, op_bit_6_h},
    [0x75] = {"BIT 6, L", 2, false, true, op_bit_6_l},
    [0x76] = {"BIT 6, (HL)", 2, false, true, op_bit_6_hlp},
    [0x77] = {"BIT 6, A", 2, false, true, op_bit_6_a},
    [0x78] = {"BIT 7, B", 2, false, true, op_bit_7_b},
    [0x79] = {"BIT 7, C", 2, false, true, op_bit_7_c},
    [0x7A] = {"BIT 7, D", 2, false, true, op_bit_7_d},
    [0x7B] = {"BIT 7, E", 2, false, true, op_bit_7_e},
    [0x7C] = {"BIT 7, H", 2, false, true, op_bit_7_h},
    [0x7D] = {"BIT 7, L", 2, false, true, op_bit_7_l},
    [0x7E] = {"BIT 7, (HL)", 2, false, true, op_bit_7_hlp},
    [0x7F] = {"BIT 7, A", 2, false, true, op_bit_7_a},
    [0x80] = {"RES 0, B", 2, false, true, op_res_0_b},
    [0x81] = {"RES 0, C", 2, false, true, op_res_0_c},
    [0x82] = {"RES 0, D", 2, false, true, op_res_0_d},
    [0x83] = {"RES 0, E", 2, false, true, op_res_0_e},
    [0x84] = {"RES 0, H", 2, false, true, op_res_0_h},
    [0x85] = {"RES 0, L", 2, false, true, op_res_0_l},
    [0x86] = {"RES 0, (HL)", 2, false, true, op_res_0_hlp},
    [0x87] = {"RES 0, A", 2, false, true, op_res_0_a},
    [0x88] = {"RES 1, B", 2, false, true, op_res_1_b},
    [0x89] = {"RES 1, C", 2, false, true, op_res_1_c},
    [0x8A] = {"RES 1, D", 2, false, true, op_res_1_d},
    [0x8B] = {"RES 1, E", 2, false, true, op_res_1_e},
    [0x8C] = {"RES 1, H", 2, false, true, op_res_1_h},
    [0x8D] = {"RES 1, L", 2, false, true, op_res_1_l},
    [0x8E] = {"RES 1, (HL)", 2, false, true, op_res_1_hlp},
    [0x8F] = {"RES 1, A", 2, false, true, op_res_1_a},
    [0x90] = {"RES 2, B", 2, false, true, op_res_2_b},
    [0x91] = {"RES 2, C", 2, false, true, op_res_2_c},
    [0x92] = {"RES 2, D", 2, false, true, op_res_2_d},
    [0x93] = {"RES 2, E", 2, false, true, op_res_2_e},
    [0x94] = {"RES 2, H", 2, false, true, op_res_2_h},
    [0x95] = {"RES 2, L", 2, false, true, op_res_2_l},
    [0x96] = {"RES 2, (HL)", 2, false, true, op_res_2_hlp},
    [0x97] = {"RES 2, A", 2, false, true, op_res_2_a},
    [0x98] = {"RES 3, B", 2, false, true, op_res_3_b},
    [0x99] = {"RES 3, C", 2, false, true, op_res_3_c},
    [0x9A] = {"RES 3, D", 2, false, true, op_res_3_d},
    [0x9B] = {"RES 3, E", 2, false, true, op_res_3_e},
    [0x9C] = {"RES 3, H", 2, false, true, op_res_3_h},
    [0x9D] = {"RES 3, L", 2, false, true, op_res_3_l},
    [0x9E] = {"RES 3, (HL)", 2, false, true, op_res_3_hlp},
    [0x9F] = {"RES 3, A", 2, false, true, op_res_3_a},
    [0xA0] = {"RES 4, B", 2, false, true, op_res_4_b},
    [0xA1] = {"RES 4, C", 2, false, true, op_res_4_c},
    [0xA2] = {"RES 4, D", 2, false, true, op_res_4_d},
    [0xA3] = {"RES 4, E", 2, false, true, op_res_4_e},
    [0xA4] = {"RES 4, H", 2, false, true, op_res_4_h},
    [0xA5] = {"RES 4, L", 2, false, true, op_res_4_l},
    [0xA6] = {"RES 4, (HL)", 2, false, true, op_res_4_hlp},
    [0xA7] = {"RES 4, A", 2, false, true, op_res_4_a},
    [0xA8] = {"RES 5, B", 2, false, true, op_res_5_b},
    [0xA9] = {"RES 5, C", 2, false, true, op_res_5_c},
    [0xAA] = {"RES 5, D", 2, false, true, op_res_5_d},
    [0xAB] = {"RES 5, E", 2, false, true, op_res_5_e},
    [0xAC] = {"RES 5, H", 2, false, true, op_res_5_h},
    [0xAD] = {"RES 5, L", 2, false, true, op_res_5_l},
    [0xAE] = {"RES 5, (HL)", 2, false, true, op_res_5_hlp},
    [0xAF] = {"RES 5, A", 2, false, true, op_res_5_a},
    [0xB0] = {"RES 6, B", 2, false, true, op_res_6_b},
    [0xB1] = {"RES 6, C", 2, false, true, op_res_6_c},
    [0xB2] = {"RES 6, D", 2, false, true, op_res_6_d},
    [0xB3] = {"RES 6, E", 2, false, true, op_res_6_e},
    [0xB4] = {"RES 6, H", 2, false, true, op_res_6_h},
    [0xB5] = {"RES 6, L", 2, false, true, op_res_6_l},
    [0xB6] = {"RES 6, (HL)", 2, false, true, op_res_6_hlp},
    [0xB7] = {"RES 6, A", 2, false, true, op_res_6_a},
    [0xB8] = {"RES 7, B", 2, false, true, op_res_7_b},
    [0xB9] = {"RES 7, C", 2, false, true, op_res_7_c},
    [0xBA] = {"RES 7, D", 2, false, true, op_res_7_d},
    [0xBB] = {"RES 7, E", 2, false, true, op_res_7_e},
    [0xBC] = {"RES 7, H", 2, false, true, op_res_7_h},
    [0xBD] = {"RES 7, L", 2, false, true, op_res_7_l},
    [0xBE] = {"RES 7, (HL)", 2, false, true, op_res_7_hlp},
    [0xBF] = {"RES 7, A", 2, false, true, op_res_7_a},
    [0xC0] = {"SET 0, B", 2, false, true, op_set_0_b},
    [0xC1] = {"SET 0, C", 2, false, true, op_set_0_c},
    [0xC2] = {"SET 0, D", 2, false, true, op_set_0_d},
    [0xC3] = {"SET 0, E", 2, false, true, op_set_0_e},
    [0xC4] = {"SET 0, H", 2, false, true, op_set_0_h},
    [0xC5] = {"SET 0, L", 2, false, true, op_set_0_l},
    [0xC6] = {"SET 0, (HL)", 2, false, true, op_set_0_hlp},
    [0xC7] = {"SET 0, A", 2, false, true, op_set_0_a},
    [0xC8] = {"SET 1, B", 2, false, true, op_set_1_b},
    [0xC9] = {"SET 1, C", 2, false, true, op_set_1_c},
    [0xCA] = {"SET 1, D", 2, false, true, op_set_1_d},
    [0xCB] = {"SET 1, E", 2, false, true, op_set_1_e},
    [0xCC] = {"SET 1, H", 2, false, true, op_set_1_h},
    [0xCD] = {"SET 1, L", 2, false, true, op_set_1_l},
    [0xCE] = {"SET 1, (HL)", 2, false, true, op_set_1_hlp},
    [0xCF] = {"SET 1, A", 2, false, true, op_set_1_a},
    [0xD0] = {"SET 2, B", 2, false, true, op_set_2_b},
    [0xD1] = {"SET 2, C", 2, false, true, op_set_2_c},
    [0xD2] = {"SET 2, D", 2, false, true, op_set_2_d},
    [0xD3] = {"SET 2, E", 2, false, true, op_set_2_e},
    [0xD4] = {"SET 2, H", 2, false, true, op_set_2_h},
    [0xD5] = {"SET 2, L", 2, false, true, op_set_2_l},
    [0xD6] = {"SET 2, (HL)", 2, false, true, op_set_2_hlp},
    [0xD7] = {"SET 2, A", 2, false, true, op_set_2_a},
    [0xD8] = {"SET 3, B", 2, false, true, op_set_3_b},
    [0xD9] = {"SET 3, C", 2, false, true, op_set_3_c},
    [0xDA] = {"SET 3, D", 2, false, true, op_set_3_d},
    [0xDB] = {"SET 3, E", 2, false, true, op_set_3_e},
    [0xDC] = {"SET 3, H", 2, false, true, op_set_3_h},
    [0xDD] = {"SET 3, L", 2, false, true, op_set_3_l},
    [0xDE] = {"SET 3, (HL)", 2, false, true, op_set_3_hlp},
    [0xDF] = {"SET 3, A", 2, false, true, op_set_3_a},
    [0xE0] = {"SET 4, B", 2, false, true, op_set_4_b},
    [0xE1] = {"SET 4, C", 2, false, true, op_set_4_c},
    [0xE2] = {"SET 4, D", 2, false, true, op_set_4_d},
    [0xE3] = {"SET 4, E", 2, false, true, op_set_4_e},
    [0xE4] = {"SET 4, H", 2, false, true, op_set_4_h},
    [0xE5] = {"SET 4, L", 2, false, true, op_set_4_l},
    [0xE6] = {"SET 4, (HL)", 2, false, true, op_set_4_hlp},
    [0xE7] = {"SET 4, A", 2, false, true, op_set_4_a},
    [0xE8] = {"SET 5, B", 2, false, true, op_set_5_b},
    [0xE9] = {"SET 5, C", 2, false, true, op_set_5_c},
    [0xEA] = {"SET 5, D", 2, false, true, op_set_5_d},
    [0xEB] = {"SET 5, E", 2, false, true, op_set_5_e},
    [0xEC] = {"SET 5, H", 2, false, true, op_set_5_h},
    [0xED] = {"SET 5, L", 2, false, true, op_set_5_l},
    [0xEE] = {"SET 5, (HL)", 2, false, true, op_set_5_hlp},
    [0xEF] = {"SET 5, A", 2, false, true, op_set_5_a},
    [0xF0] = {"SET 6, B", 2, false, true, op_set_6_b},
    [0xF1] = {"SET 6, C", 2, false, true, op_set_6_c},
    [0xF2] = {"SET 6, D", 2, false, true, op_set_6_d},
    [0xF3] = {"SET 6, E", 2, false, true, op_set_6_e},
    [0xF4] = {"SET 6, H", 2, false, true, op_set_6_h},
    [0xF5] = {"SET 6, L", 2, false, true, op_set_6_l},
    [0xF6] = {"SET 6, (HL)", 2, false, true, op_set_6_hlp},
    [0xF7] = {"SET 6, A", 2, false, true, op_set_6_a},
    [0xF8] = {"SET 7, B", 2, false, true, op_set_7_b},
    [0xF9] = {"SET 7, C", 2, false, true, op_set_7_c},
    [0xFA] = {"SET 7, D", 2, false, true, op_set_7_d},
    [0xFB] = {"SET 7, E", 2, false, true, op_set_7_e},
    [0xFC] = {"SET 7, H", 2, false, true, op_set_7_h},
    [0xFD] = {"SET 7, L", 2, false, true, op_set_7_l},
    [0xFE] = {"SET 7, (HL)", 2, false, true, op_set_7_hlp},
    [0xFF] = {"SET 7, A", 2, false, true, op_set_7_a}
};
