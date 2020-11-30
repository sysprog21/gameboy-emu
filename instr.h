#pragma once

static const cpu_instruction_s instrs[] = {
    {"NOP", 1, false, false, op_nop},               // 0x00
    {"LD BC, %04x", 3, false, false, op_ld_bc_d16}, // 0x01
    {"LD (BC), A", 1, false, false, op_ld_bcp_a},   // 0x02
    {"INC BC", 1, false, false, op_inc_bc},         // 0x03
    {"INC B", 1, false, false, op_inc_b},           // 0x04
    {"DEC B", 1, false, false, op_dec_b},           // 0x05
    {"LD B, %02x", 2, false, false, op_ld_b_d8}, // 0x06
    {"RLCA", 1, false, false, op_rlca}, // 0x07
    {"LD (%04x), SP", 3, false, false, op_ld_a16p_sp}, // 0x08
    {"ADD HL, BC", 1, false, false, op_add_hl_bc}, // 0x09
    {"LD A, (BC)", 1, false, false, op_ld_a_bcp}, // 0x0A
    {"DEC BC", 1, false, false, op_dec_bc}, // 0x0B
    {"INC C", 1, false, false, op_inc_c}, // 0x0C
    {"DEC C", 1, false, false, op_dec_c}, // 0x0D
    {"LD C, %02x", 2, false, false, op_ld_c_d8}, // 0x0E
    {"RRCA", 1, false, false, op_rrca}, // 0x0F
    {"STOP", 1, false, false, op_stop}, // 0x10
    {"LD DE, %04x", 3, false, false, op_ld_de_d16}, // 0x11
    {"LD (DE), A", 1, false, false, op_ld_dep_a}, // 0x12
    {"INC DE", 1, false, false, op_inc_de}, // 0x13
    {"INC D", 1, false, false, op_inc_d}, // 0x14
    {"DEC D", 1, false, false, op_dec_d}, // 0x15
    {"LD D, %02x", 2, false, false, op_ld_d_d8}, // 0x16
    {"RLA", 1, false, false, op_rla}, // 0x17
    {"JR %02x", 2, true, false, op_jr_r8}, // 0x18
    {"ADD HL, DE", 1, false, false, op_add_hl_de}, // 0x19
    {"LD A, (DE)", 1, false, false, op_ld_a_dep}, // 0x1A
    {"DEC DE", 1, false, false, op_dec_de}, // 0x1B
    {"INC E", 1, false, false, op_inc_e}, // 0x1C
    {"DEC E", 1, false, false, op_dec_e}, // 0x1D
    {"LD E, %02x", 2, false, false, op_ld_e_d8}, // 0x1E
    {"RRA", 1, false, false, op_rra}, // 0x1F
    {"JR NZ, %02x", 2, true, false, op_jr_nz_r8}, // 0x20
    {"LD HL, %04x", 3, false, false, op_ld_hl_d16}, // 0x21
    {"LD (HL+), A", 1, false, false, op_ld_hlpp_a}, // 0x22
    {"INC HL", 1, false, false, op_inc_hl}, // 0x23
    {"INC H", 1, false, false, op_inc_h}, // 0x24
    {"DEC H", 1, false, false, op_dec_h}, // 0x25
    {"LD H, %02x", 2, false, false, op_ld_h_d8}, // 0x26
    {"DAA", 1, false, false, op_daa}, // 0x27
    {"JR Z, %02x", 2, true, false, op_jr_z_r8}, // 0x28
    {"ADD HL, HL", 1, false, false, op_add_hl_hl}, // 0x29
    {"LD A, (HL+)", 1, false, false, op_ld_a_hlpp}, // 0x2A
    {"DEC HL", 1, false, false, op_dec_hl}, // 0x2B
    {"INC L", 1, false, false, op_inc_l}, // 0x2C
    {"DEC L", 1, false, false, op_dec_l}, // 0x2D
    {"LD L, %02x", 2, false, false, op_ld_l_d8}, // 0x2E
    {"CPL", 1, false, false, op_cpl}, // 0x2F
    {"JR NC, %02x", 2, true, false, op_jr_nc_r8}, // 0x30
    {"LD SP, %04x", 3, false, false, op_ld_sp_d16}, // 0x31
    {"LD (HL-), A", 1, false, false, op_ld_hlmp_a}, // 0x32
    {"INC SP", 1, false, false, op_inc_sp}, // 0x33
    {"INC (HL)", 1, false, false, op_inc_hlp}, // 0x34
    {"DEC (HL)", 1, false, false, op_dec_hlp}, // 0x35
    {"LD (HL), %02x", 2, false, false, op_ld_hlp_d8}, // 0x36
    {"SCF", 1, false, false, op_scf}, // 0x37
    {"JR C, %02x", 2, true, false, op_jr_c_r8}, // 0x38
    {"ADD HL, SP", 1, false, false, op_add_hl_sp}, // 0x39
    {"LD A, (HL-)", 1, false, false, op_ld_a_hlmp}, // 0x3A
    {"DEC SP", 1, false, false, op_dec_sp}, // 0x3B
    {"INC A", 1, false, false, op_inc_a}, // 0x3C
    {"DEC A", 1, false, false, op_dec_a}, // 0x3D
    {"LD A, %02x", 2, false, false, op_ld_a_d8}, // 0x3E
    {"CCF", 1, false, false, op_ccf}, // 0x3F
    {"LD B, B", 1, false, false, op_ld_b_b}, // 0x40
    {"LD B, C", 1, false, false, op_ld_b_c}, // 0x41
    {"LD B, D", 1, false, false, op_ld_b_d}, // 0x42
    {"LD B, E", 1, false, false, op_ld_b_e}, // 0x43
    {"LD B, H", 1, false, false, op_ld_b_h}, // 0x44
    {"LD B, L", 1, false, false, op_ld_b_l}, // 0x45
    {"LD B, (HL)", 1, false, false, op_ld_b_hlp}, // 0x46
    {"LD B, A", 1, false, false, op_ld_b_a}, // 0x47
    {"LD C, B", 1, false, false, op_ld_c_b}, // 0x48
    {"LD C, C", 1, false, false, op_ld_c_c}, // 0x49
    {"LD C, D", 1, false, false, op_ld_c_d}, // 0x4A
    {"LD C, E", 1, false, false, op_ld_c_e}, // 0x4B
    {"LD C, H", 1, false, false, op_ld_c_h}, // 0x4C
    {"LD C, L", 1, false, false, op_ld_c_l}, // 0x4D
    {"LD C, (HL)", 1, false, false, op_ld_c_hlp}, // 0x4E
    {"LD C, A", 1, false, false, op_ld_c_a}, // 0x4F
    {"LD D, B", 1, false, false, op_ld_d_b}, // 0x50
    {"LD D, C", 1, false, false, op_ld_d_c}, // 0x51
    {"LD D, D", 1, false, false, op_ld_d_d}, // 0x52
    {"LD D, E", 1, false, false, op_ld_d_e}, // 0x53
    {"LD D, H", 1, false, false, op_ld_d_h}, // 0x54
    {"LD D, L", 1, false, false, op_ld_d_l}, // 0x55
    {"LD D, (HL)", 1, false, false, op_ld_d_hlp}, // 0x56
    {"LD D, A", 1, false, false, op_ld_d_a}, // 0x57
    {"LD E, B", 1, false, false, op_ld_e_b}, // 0x58
    {"LD E, C", 1, false, false, op_ld_e_c}, // 0x59
    {"LD E, D", 1, false, false, op_ld_e_d}, // 0x5A
    {"LD E, E", 1, false, false, op_ld_e_e}, // 0x5B
    {"LD E, H", 1, false, false, op_ld_e_h}, // 0x5C
    {"LD E, L", 1, false, false, op_ld_e_l}, // 0x5D
    {"LD E, (HL)", 1, false, false, op_ld_e_hlp}, // 0x5E
    {"LD E, A", 1, false, false, op_ld_e_a}, // 0x5F
    {"LD H, B", 1, false, false, op_ld_h_b}, // 0x60
    {"LD H, C", 1, false, false, op_ld_h_c}, // 0x61
    {"LD H, D", 1, false, false, op_ld_h_d}, // 0x62
    {"LD H, E", 1, false, false, op_ld_h_e}, // 0x63
    {"LD H, H", 1, false, false, op_ld_h_h}, // 0x64
    {"LD H, L", 1, false, false, op_ld_h_l}, // 0x65
    {"LD H, (HL)", 1, false, false, op_ld_h_hlp}, // 0x66
    {"LD H, A", 1, false, false, op_ld_h_a}, // 0x67
    {"LD L, B", 1, false, false, op_ld_l_b}, // 0x68
    {"LD L, C", 1, false, false, op_ld_l_c}, // 0x69
    {"LD L, D", 1, false, false, op_ld_l_d}, // 0x6A
    {"LD L, E", 1, false, false, op_ld_l_e}, // 0x6B
    {"LD L, H", 1, false, false, op_ld_l_h}, // 0x6C
    {"LD L, L", 1, false, false, op_ld_l_l}, // 0x6D
    {"LD L, (HL)", 1, false, false, op_ld_l_hlp}, // 0x6E
    {"LD L, A", 1, false, false, op_ld_l_a}, // 0x6F
    {"LD (HL), B", 1, false, false, op_ld_hlp_b}, // 0x70
    {"LD (HL), C", 1, false, false, op_ld_hlp_c}, // 0x71
    {"LD (HL), D", 1, false, false, op_ld_hlp_d}, // 0x72
    {"LD (HL), E", 1, false, false, op_ld_hlp_e}, // 0x73
    {"LD (HL), H", 1, false, false, op_ld_hlp_h}, // 0x74
    {"LD (HL), L", 1, false, false, op_ld_hlp_l}, // 0x75
    {"HALT", 1, false, false, op_halt}, // 0x76
    {"LD (HL), A", 1, false, false, op_ld_hlp_a}, // 0x77
    {"LD A, B", 1, false, false, op_ld_a_b}, // 0x78
    {"LD A, C", 1, false, false, op_ld_a_c}, // 0x79
    {"LD A, D", 1, false, false, op_ld_a_d}, // 0x7A
    {"LD A, E", 1, false, false, op_ld_a_e}, // 0x7B
    {"LD A, H", 1, false, false, op_ld_a_h}, // 0x7C
    {"LD A, L", 1, false, false, op_ld_a_l}, // 0x7D
    {"LD A, (HL)", 1, false, false, op_ld_a_hlp}, // 0x7E
    {"LD A, A", 1, false, false, op_ld_a_a}, // 0x7F
    {"ADD A, B", 1, false, false, op_add_a_b}, // 0x80
    {"ADD A, C", 1, false, false, op_add_a_c}, // 0x81
    {"ADD A, D", 1, false, false, op_add_a_d}, // 0x82
    {"ADD A, E", 1, false, false, op_add_a_e}, // 0x83
    {"ADD A, H", 1, false, false, op_add_a_h}, // 0x84
    {"ADD A, L", 1, false, false, op_add_a_l}, // 0x85
    {"ADD A, (HL)", 1, false, false, op_add_a_hlp}, // 0x86
    {"ADD A, A", 1, false, false, op_add_a_a}, // 0x87
    {"ADC A, B", 1, false, false, op_adc_a_b}, // 0x88
    {"ADC A, C", 1, false, false, op_adc_a_c}, // 0x89
    {"ADC A, D", 1, false, false, op_adc_a_d}, // 0x8A
    {"ADC A, E", 1, false, false, op_adc_a_e}, // 0x8B
    {"ADC A, H", 1, false, false, op_adc_a_h}, // 0x8C
    {"ADC A, L", 1, false, false, op_adc_a_l}, // 0x8D
    {"ADC A, (HL)", 1, false, false, op_adc_a_hlp}, // 0x8E
    {"ADC A, A", 1, false, false, op_adc_a_a}, // 0x8F
    {"SUB B", 1, false, false, op_sub_b}, // 0x90
    {"SUB C", 1, false, false, op_sub_c}, // 0x91
    {"SUB D", 1, false, false, op_sub_d}, // 0x92
    {"SUB E", 1, false, false, op_sub_e}, // 0x93
    {"SUB H", 1, false, false, op_sub_h}, // 0x94
    {"SUB L", 1, false, false, op_sub_l}, // 0x95
    {"SUB (HL)", 1, false, false, op_sub_hlp}, // 0x96
    {"SUB A", 1, false, false, op_sub_a}, // 0x97
    {"SBC A, B", 1, false, false, op_sbc_a_b}, // 0x98
    {"SBC A, C", 1, false, false, op_sbc_a_c}, // 0x99
    {"SBC A, D", 1, false, false, op_sbc_a_d}, // 0x9A
    {"SBC A, E", 1, false, false, op_sbc_a_e}, // 0x9B
    {"SBC A, H", 1, false, false, op_sbc_a_h}, // 0x9C
    {"SBC A, L", 1, false, false, op_sbc_a_l}, // 0x9D
    {"SBC A, (HL)", 1, false, false, op_sbc_a_hlp}, // 0x9E
    {"SBC A, A", 1, false, false, op_sbc_a_a}, // 0x9F
    {"AND B", 1, false, false, op_and_b}, // 0xA0
    {"AND C", 1, false, false, op_and_c}, // 0xA1
    {"AND D", 1, false, false, op_and_d}, // 0xA2
    {"AND E", 1, false, false, op_and_e}, // 0xA3
    {"AND H", 1, false, false, op_and_h}, // 0xA4
    {"AND L", 1, false, false, op_and_l}, // 0xA5
    {"AND (HL)", 1, false, false, op_and_hlp}, // 0xA6
    {"AND A", 1, false, false, op_and_a}, // 0xA7
    {"XOR B", 1, false, false, op_xor_b}, // 0xA8
    {"XOR C", 1, false, false, op_xor_c}, // 0xA9
    {"XOR D", 1, false, false, op_xor_d}, // 0xAA
    {"XOR E", 1, false, false, op_xor_e}, // 0xAB
    {"XOR H", 1, false, false, op_xor_h}, // 0xAC
    {"XOR L", 1, false, false, op_xor_l}, // 0xAD
    {"XOR (HL)", 1, false, false, op_xor_hlp}, // 0xAE
    {"XOR A", 1, false, false, op_xor_a}, // 0xAF
    {"OR B", 1, false, false, op_or_b}, // 0xB0
    {"OR C", 1, false, false, op_or_c}, // 0xB1
    {"OR D", 1, false, false, op_or_d}, // 0xB2
    {"OR E", 1, false, false, op_or_e}, // 0xB3
    {"OR H", 1, false, false, op_or_h}, // 0xB4
    {"OR L", 1, false, false, op_or_l}, // 0xB5
    {"OR (HL)", 1, false, false, op_or_hlp}, // 0xB6
    {"OR A", 1, false, false, op_or_a}, // 0xB7
    {"CP B", 1, false, false, op_cp_b}, // 0xB8
    {"CP C", 1, false, false, op_cp_c}, // 0xB9
    {"CP D", 1, false, false, op_cp_d}, // 0xBA
    {"CP E", 1, false, false, op_cp_e}, // 0xBB
    {"CP H", 1, false, false, op_cp_h}, // 0xBC
    {"CP L", 1, false, false, op_cp_l}, // 0xBD
    {"CP (HL)", 1, false, false, op_cp_hlp}, // 0xBE
    {"CP A", 1, false, false, op_cp_a}, // 0xBF
    {"RET NZ", 1, false, false, op_ret_nz}, // 0xC0
    {"POP BC", 1, false, false, op_pop_bc}, // 0xC1
    {"JP NZ, %04x", 3, false, false, op_jp_nz_a16}, // 0xC2
    {"JP %04x", 3, false, false, op_jp_a16}, // 0xC3
    {"CALL NZ, %04x", 3, false, false, op_call_nz_a16}, // 0xC4
    {"PUSH BC", 1, false, false, op_push_bc}, // 0xC5
    {"ADD A, %02x", 2, false, false, op_add_a_d8}, // 0xC6
    {"RST 00H", 1, false, false, op_rst_00h}, // 0xC7
    {"RET Z", 1, false, false, op_ret_z}, // 0xC8
    {"RET", 1, false, false, op_ret}, // 0xC9
    {"JP Z, %04x", 3, false, false, op_jp_z_a16}, // 0xCA
    {"PREFIX CB", 1, false, false, op_prefix_cb}, // 0xCB
    {"CALL Z, %04x", 3, false, false, op_call_z_a16}, // 0xCC
    {"CALL %04x", 3, false, false, op_call_a16}, // 0xCD
    {"ADC A, %02x", 2, false, false, op_adc_a_d8}, // 0xCE
    {"RST 08H", 1, false, false, op_rst_08h}, // 0xCF
    {"RET NC", 1, false, false, op_ret_nc}, // 0xD0
    {"POP DE", 1, false, false, op_pop_de}, // 0xD1
    {"JP NC, %04x", 3, false, false, op_jp_nc_a16}, // 0xD2
    {"NOP", 1, false, false, op_nop}, // 0xD3
    {"CALL NC, %04x", 3, false, false, op_call_nc_a16}, // 0xD4
    {"PUSH DE", 1, false, false, op_push_de}, // 0xD5
    {"SUB %02x", 2, false, false, op_sub_d8}, // 0xD6
    {"RST 10H", 1, false, false, op_rst_10h}, // 0xD7
    {"RET C", 1, false, false, op_ret_c}, // 0xD8
    {"RETI", 1, false, false, op_reti}, // 0xD9
    {"JP C, %04x", 3, false, false, op_jp_c_a16}, // 0xDA
    {"NOP", 1, false, false, op_nop}, // 0xDB
    {"CALL C, %04x", 3, false, false, op_call_c_a16}, // 0xDC
    {"NOP", 1, false, false, op_nop}, // 0xDD
    {"SBC A, %02x", 2, false, false, op_sbc_a_d8}, // 0xDE
    {"RST 18H", 1, false, false, op_rst_18h}, // 0xDF
    {"LDH (%02x), A", 2, false, false, op_ldh_a8p_a}, // 0xE0
    {"POP HL", 1, false, false, op_pop_hl}, // 0xE1
    {"LD (C), A", 1, false, false, op_ld_cp_a}, // 0xE2
    {"NOP", 1, false, false, op_nop}, // 0xE3
    {"NOP", 1, false, false, op_nop}, // 0xE4
    {"PUSH HL", 1, false, false, op_push_hl}, // 0xE5
    {"AND %02x", 2, false, false, op_and_d8}, // 0xE6
    {"RST 20H", 1, false, false, op_rst_20h}, // 0xE7
    {"ADD SP, %02x", 2, true, false, op_add_sp_r8}, // 0xE8
    {"JP (HL)", 1, false, false, op_jp_hlp}, // 0xE9
    {"LD (%04x), A", 3, false, false, op_ld_a16p_a}, // 0xEA
    {"NOP", 1, false, false, op_nop}, // 0xEB
    {"NOP", 1, false, false, op_nop}, // 0xEC
    {"NOP", 1, false, false, op_nop}, // 0xED
    {"XOR %02x", 2, false, false, op_xor_d8}, // 0xEE
    {"RST 28H", 1, false, false, op_rst_28h}, // 0xEF
    {"LDH A, (%02x)", 2, false, false, op_ldh_a_a8p}, // 0xF0
    {"POP AF", 1, false, false, op_pop_af}, // 0xF1
    {"LD A, (C)", 1, false, false, op_ld_a_cp}, // 0xF2
    {"DI", 1, false, false, op_di}, // 0xF3
    {"NOP", 1, false, false, op_nop}, // 0xF4
    {"PUSH AF", 1, false, false, op_push_af}, // 0xF5
    {"OR %02x", 2, false, false, op_or_d8}, // 0xF6
    {"RST 30H", 1, false, false, op_rst_30h}, // 0xF7
    {"LD HL, SP+%02x", 2, true, false, op_ld_hl_sppr8}, // 0xF8
    {"LD SP, HL", 1, false, false, op_ld_sp_hl}, // 0xF9
    {"LD A, (%04x)", 3, false, false, op_ld_a_a16p}, // 0xFA
    {"EI", 1, false, false, op_ei}, // 0xFB
    {"NOP", 1, false, false, op_nop}, // 0xFC
    {"NOP", 1, false, false, op_nop}, // 0xFD
    {"CP %02x", 2, false, false, op_cp_d8}, // 0xFE
    {"RST 38H", 1, false, false, op_rst_38h} // 0xFF
} 
