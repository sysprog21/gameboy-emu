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
};

static const cpu_instruction_s CB_instrs[] = {
    {"RLC B", 2, false, true, op_rlc_b}, // 0xCB00
    {"RLC C", 2, false, true, op_rlc_c}, // 0xCB01
    {"RLC D", 2, false, true, op_rlc_d}, // 0xCB02
    {"RLC E", 2, false, true, op_rlc_e}, // 0xCB03
    {"RLC H", 2, false, true, op_rlc_h}, // 0xCB04
    {"RLC L", 2, false, true, op_rlc_l}, // 0xCB05
    {"RLC (HL)", 2, false, true, op_rlc_hlp}, // 0xCB06
    {"RLC A", 2, false, true, op_rlc_a}, // 0xCB07
    {"RRC B", 2, false, true, op_rrc_b}, // 0xCB08
    {"RRC C", 2, false, true, op_rrc_c}, // 0xCB09
    {"RRC D", 2, false, true, op_rrc_d}, // 0xCB0A
    {"RRC E", 2, false, true, op_rrc_e}, // 0xCB0B
    {"RRC H", 2, false, true, op_rrc_h}, // 0xCB0C
    {"RRC L", 2, false, true, op_rrc_l}, // 0xCB0D
    {"RRC (HL)", 2, false, true, op_rrc_hlp}, // 0xCB0E
    {"RRC A", 2, false, true, op_rrc_a}, // 0xCB0F
    {"RL B", 2, false, true, op_rl_b}, // 0xCB10
    {"RL C", 2, false, true, op_rl_c}, // 0xCB11
    {"RL D", 2, false, true, op_rl_d}, // 0xCB12
    {"RL E", 2, false, true, op_rl_e}, // 0xCB13
    {"RL H", 2, false, true, op_rl_h}, // 0xCB14
    {"RL L", 2, false, true, op_rl_l}, // 0xCB15
    {"RL (HL)", 2, false, true, op_rl_hlp}, // 0xCB16
    {"RL A", 2, false, true, op_rl_a}, // 0xCB17
    {"RR B", 2, false, true, op_rr_b}, // 0xCB18
    {"RR C", 2, false, true, op_rr_c}, // 0xCB19
    {"RR D", 2, false, true, op_rr_d}, // 0xCB1A
    {"RR E", 2, false, true, op_rr_e}, // 0xCB1B
    {"RR H", 2, false, true, op_rr_h}, // 0xCB1C
    {"RR L", 2, false, true, op_rr_l}, // 0xCB1D
    {"RR (HL)", 2, false, true, op_rr_hlp}, // 0xCB1E
    {"RR A", 2, false, true, op_rr_a}, // 0xCB1F
    {"SLA B", 2, false, true, op_sla_b}, // 0xCB20
    {"SLA C", 2, false, true, op_sla_c}, // 0xCB21
    {"SLA D", 2, false, true, op_sla_d}, // 0xCB22
    {"SLA E", 2, false, true, op_sla_e}, // 0xCB23
    {"SLA H", 2, false, true, op_sla_h}, // 0xCB24
    {"SLA L", 2, false, true, op_sla_l}, // 0xCB25
    {"SLA (HL)", 2, false, true, op_sla_hlp}, // 0xCB26
    {"SLA A", 2, false, true, op_sla_a}, // 0xCB27
    {"SRA B", 2, false, true, op_sra_b}, // 0xCB28
    {"SRA C", 2, false, true, op_sra_c}, // 0xCB29
    {"SRA D", 2, false, true, op_sra_d}, // 0xCB2A
    {"SRA E", 2, false, true, op_sra_e}, // 0xCB2B
    {"SRA H", 2, false, true, op_sra_h}, // 0xCB2C
    {"SRA L", 2, false, true, op_sra_l}, // 0xCB2D
    {"SRA (HL)", 2, false, true, op_sra_hlp}, // 0xCB2E
    {"SRA A", 2, false, true, op_sra_a}, // 0xCB2F
    {"SWAP B", 2, false, true, op_swap_b}, // 0xCB30
    {"SWAP C", 2, false, true, op_swap_c}, // 0xCB31
    {"SWAP D", 2, false, true, op_swap_d}, // 0xCB32
    {"SWAP E", 2, false, true, op_swap_e}, // 0xCB33
    {"SWAP H", 2, false, true, op_swap_h}, // 0xCB34
    {"SWAP L", 2, false, true, op_swap_l}, // 0xCB35
    {"SWAP (HL)", 2, false, true, op_swap_hlp}, // 0xCB36
    {"SWAP A", 2, false, true, op_swap_a}, // 0xCB37
    {"SRL B", 2, false, true, op_srl_b}, // 0xCB38
    {"SRL C", 2, false, true, op_srl_c}, // 0xCB39
    {"SRL D", 2, false, true, op_srl_d}, // 0xCB3A
    {"SRL E", 2, false, true, op_srl_e}, // 0xCB3B
    {"SRL H", 2, false, true, op_srl_h}, // 0xCB3C
    {"SRL L", 2, false, true, op_srl_l}, // 0xCB3D
    {"SRL (HL)", 2, false, true, op_srl_hlp}, // 0xCB3E
    {"SRL A", 2, false, true, op_srl_a}, // 0xCB3F
    {"BIT 0, B", 2, false, true, op_bit_0_b}, // 0xCB40
    {"BIT 0, C", 2, false, true, op_bit_0_c}, // 0xCB41
    {"BIT 0, D", 2, false, true, op_bit_0_d}, // 0xCB42
    {"BIT 0, E", 2, false, true, op_bit_0_e}, // 0xCB43
    {"BIT 0, H", 2, false, true, op_bit_0_h}, // 0xCB44
    {"BIT 0, L", 2, false, true, op_bit_0_l}, // 0xCB45
    {"BIT 0, (HL)", 2, false, true, op_bit_0_hlp}, // 0xCB46
    {"BIT 0, A", 2, false, true, op_bit_0_a}, // 0xCB47
    {"BIT 1, B", 2, false, true, op_bit_1_b}, // 0xCB48
    {"BIT 1, C", 2, false, true, op_bit_1_c}, // 0xCB49
    {"BIT 1, D", 2, false, true, op_bit_1_d}, // 0xCB4A
    {"BIT 1, E", 2, false, true, op_bit_1_e}, // 0xCB4B
    {"BIT 1, H", 2, false, true, op_bit_1_h}, // 0xCB4C
    {"BIT 1, L", 2, false, true, op_bit_1_l}, // 0xCB4D
    {"BIT 1, (HL)", 2, false, true, op_bit_1_hlp}, // 0xCB4E
    {"BIT 1, A", 2, false, true, op_bit_1_a}, // 0xCB4F
    {"BIT 2, B", 2, false, true, op_bit_2_b}, // 0xCB50
    {"BIT 2, C", 2, false, true, op_bit_2_c}, // 0xCB51
    {"BIT 2, D", 2, false, true, op_bit_2_d}, // 0xCB52
    {"BIT 2, E", 2, false, true, op_bit_2_e}, // 0xCB53
    {"BIT 2, H", 2, false, true, op_bit_2_h}, // 0xCB54
    {"BIT 2, L", 2, false, true, op_bit_2_l}, // 0xCB55
    {"BIT 2, (HL)", 2, false, true, op_bit_2_hlp}, // 0xCB56
    {"BIT 2, A", 2, false, true, op_bit_2_a}, // 0xCB57
    {"BIT 3, B", 2, false, true, op_bit_3_b}, // 0xCB58
    {"BIT 3, C", 2, false, true, op_bit_3_c}, // 0xCB59
    {"BIT 3, D", 2, false, true, op_bit_3_d}, // 0xCB5A
    {"BIT 3, E", 2, false, true, op_bit_3_e}, // 0xCB5B
    {"BIT 3, H", 2, false, true, op_bit_3_h}, // 0xCB5C
    {"BIT 3, L", 2, false, true, op_bit_3_l}, // 0xCB5D
    {"BIT 3, (HL)", 2, false, true, op_bit_3_hlp}, // 0xCB5E
    {"BIT 3, A", 2, false, true, op_bit_3_a}, // 0xCB5F
    {"BIT 4, B", 2, false, true, op_bit_4_b}, // 0xCB60
    {"BIT 4, C", 2, false, true, op_bit_4_c}, // 0xCB61
    {"BIT 4, D", 2, false, true, op_bit_4_d}, // 0xCB62
    {"BIT 4, E", 2, false, true, op_bit_4_e}, // 0xCB63
    {"BIT 4, H", 2, false, true, op_bit_4_h}, // 0xCB64
    {"BIT 4, L", 2, false, true, op_bit_4_l}, // 0xCB65
    {"BIT 4, (HL)", 2, false, true, op_bit_4_hlp}, // 0xCB66
    {"BIT 4, A", 2, false, true, op_bit_4_a}, // 0xCB67
    {"BIT 5, B", 2, false, true, op_bit_5_b}, // 0xCB68
    {"BIT 5, C", 2, false, true, op_bit_5_c}, // 0xCB69
    {"BIT 5, D", 2, false, true, op_bit_5_d}, // 0xCB6A
    {"BIT 5, E", 2, false, true, op_bit_5_e}, // 0xCB6B
    {"BIT 5, H", 2, false, true, op_bit_5_h}, // 0xCB6C
    {"BIT 5, L", 2, false, true, op_bit_5_l}, // 0xCB6D
    {"BIT 5, (HL)", 2, false, true, op_bit_5_hlp}, // 0xCB6E
    {"BIT 5, A", 2, false, true, op_bit_5_a}, // 0xCB6F
    {"BIT 6, B", 2, false, true, op_bit_6_b}, // 0xCB70
    {"BIT 6, C", 2, false, true, op_bit_6_c}, // 0xCB71
    {"BIT 6, D", 2, false, true, op_bit_6_d}, // 0xCB72
    {"BIT 6, E", 2, false, true, op_bit_6_e}, // 0xCB73
    {"BIT 6, H", 2, false, true, op_bit_6_h}, // 0xCB74
    {"BIT 6, L", 2, false, true, op_bit_6_l}, // 0xCB75
    {"BIT 6, (HL)", 2, false, true, op_bit_6_hlp}, // 0xCB76
    {"BIT 6, A", 2, false, true, op_bit_6_a}, // 0xCB77
    {"BIT 7, B", 2, false, true, op_bit_7_b}, // 0xCB78
    {"BIT 7, C", 2, false, true, op_bit_7_c}, // 0xCB79
    {"BIT 7, D", 2, false, true, op_bit_7_d}, // 0xCB7A
    {"BIT 7, E", 2, false, true, op_bit_7_e}, // 0xCB7B
    {"BIT 7, H", 2, false, true, op_bit_7_h}, // 0xCB7C
    {"BIT 7, L", 2, false, true, op_bit_7_l}, // 0xCB7D
    {"BIT 7, (HL)", 2, false, true, op_bit_7_hlp}, // 0xCB7E
    {"BIT 7, A", 2, false, true, op_bit_7_a}, // 0xCB7F
    {"RES 0, B", 2, false, true, op_res_0_b}, // 0xCB80
    {"RES 0, C", 2, false, true, op_res_0_c}, // 0xCB81
    {"RES 0, D", 2, false, true, op_res_0_d}, // 0xCB82
    {"RES 0, E", 2, false, true, op_res_0_e}, // 0xCB83
    {"RES 0, H", 2, false, true, op_res_0_h}, // 0xCB84
    {"RES 0, L", 2, false, true, op_res_0_l}, // 0xCB85
    {"RES 0, (HL)", 2, false, true, op_res_0_hlp}, // 0xCB86
    {"RES 0, A", 2, false, true, op_res_0_a}, // 0xCB87
    {"RES 1, B", 2, false, true, op_res_1_b}, // 0xCB88
    {"RES 1, C", 2, false, true, op_res_1_c}, // 0xCB89
    {"RES 1, D", 2, false, true, op_res_1_d}, // 0xCB8A
    {"RES 1, E", 2, false, true, op_res_1_e}, // 0xCB8B
    {"RES 1, H", 2, false, true, op_res_1_h}, // 0xCB8C
    {"RES 1, L", 2, false, true, op_res_1_l}, // 0xCB8D
    {"RES 1, (HL)", 2, false, true, op_res_1_hlp}, // 0xCB8E
    {"RES 1, A", 2, false, true, op_res_1_a}, // 0xCB8F
    {"RES 2, B", 2, false, true, op_res_2_b}, // 0xCB90
    {"RES 2, C", 2, false, true, op_res_2_c}, // 0xCB91
    {"RES 2, D", 2, false, true, op_res_2_d}, // 0xCB92
    {"RES 2, E", 2, false, true, op_res_2_e}, // 0xCB93
    {"RES 2, H", 2, false, true, op_res_2_h}, // 0xCB94
    {"RES 2, L", 2, false, true, op_res_2_l}, // 0xCB95
    {"RES 2, (HL)", 2, false, true, op_res_2_hlp}, // 0xCB96
    {"RES 2, A", 2, false, true, op_res_2_a}, // 0xCB97
    {"RES 3, B", 2, false, true, op_res_3_b}, // 0xCB98
    {"RES 3, C", 2, false, true, op_res_3_c}, // 0xCB99
    {"RES 3, D", 2, false, true, op_res_3_d}, // 0xCB9A
    {"RES 3, E", 2, false, true, op_res_3_e}, // 0xCB9B
    {"RES 3, H", 2, false, true, op_res_3_h}, // 0xCB9C
    {"RES 3, L", 2, false, true, op_res_3_l}, // 0xCB9D
    {"RES 3, (HL)", 2, false, true, op_res_3_hlp}, // 0xCB9E
    {"RES 3, A", 2, false, true, op_res_3_a}, // 0xCB9F
    {"RES 4, B", 2, false, true, op_res_4_b}, // 0xCBA0
    {"RES 4, C", 2, false, true, op_res_4_c}, // 0xCBA1
    {"RES 4, D", 2, false, true, op_res_4_d}, // 0xCBA2
    {"RES 4, E", 2, false, true, op_res_4_e}, // 0xCBA3
    {"RES 4, H", 2, false, true, op_res_4_h}, // 0xCBA4
    {"RES 4, L", 2, false, true, op_res_4_l}, // 0xCBA5
    {"RES 4, (HL)", 2, false, true, op_res_4_hlp}, // 0xCBA6
    {"RES 4, A", 2, false, true, op_res_4_a}, // 0xCBA7
    {"RES 5, B", 2, false, true, op_res_5_b}, // 0xCBA8
    {"RES 5, C", 2, false, true, op_res_5_c}, // 0xCBA9
    {"RES 5, D", 2, false, true, op_res_5_d}, // 0xCBAA
    {"RES 5, E", 2, false, true, op_res_5_e}, // 0xCBAB
    {"RES 5, H", 2, false, true, op_res_5_h}, // 0xCBAC
    {"RES 5, L", 2, false, true, op_res_5_l}, // 0xCBAD
    {"RES 5, (HL)", 2, false, true, op_res_5_hlp}, // 0xCBAE
    {"RES 5, A", 2, false, true, op_res_5_a}, // 0xCBAF
    {"RES 6, B", 2, false, true, op_res_6_b}, // 0xCBB0
    {"RES 6, C", 2, false, true, op_res_6_c}, // 0xCBB1
    {"RES 6, D", 2, false, true, op_res_6_d}, // 0xCBB2
    {"RES 6, E", 2, false, true, op_res_6_e}, // 0xCBB3
    {"RES 6, H", 2, false, true, op_res_6_h}, // 0xCBB4
    {"RES 6, L", 2, false, true, op_res_6_l}, // 0xCBB5
    {"RES 6, (HL)", 2, false, true, op_res_6_hlp}, // 0xCBB6
    {"RES 6, A", 2, false, true, op_res_6_a}, // 0xCBB7
    {"RES 7, B", 2, false, true, op_res_7_b}, // 0xCBB8
    {"RES 7, C", 2, false, true, op_res_7_c}, // 0xCBB9
    {"RES 7, D", 2, false, true, op_res_7_d}, // 0xCBBA
    {"RES 7, E", 2, false, true, op_res_7_e}, // 0xCBBB
    {"RES 7, H", 2, false, true, op_res_7_h}, // 0xCBBC
    {"RES 7, L", 2, false, true, op_res_7_l}, // 0xCBBD
    {"RES 7, (HL)", 2, false, true, op_res_7_hlp}, // 0xCBBE
    {"RES 7, A", 2, false, true, op_res_7_a}, // 0xCBBF
    {"SET 0, B", 2, false, true, op_set_0_b}, // 0xCBC0
    {"SET 0, C", 2, false, true, op_set_0_c}, // 0xCBC1
    {"SET 0, D", 2, false, true, op_set_0_d}, // 0xCBC2
    {"SET 0, E", 2, false, true, op_set_0_e}, // 0xCBC3
    {"SET 0, H", 2, false, true, op_set_0_h}, // 0xCBC4
    {"SET 0, L", 2, false, true, op_set_0_l}, // 0xCBC5
    {"SET 0, (HL)", 2, false, true, op_set_0_hlp}, // 0xCBC6
    {"SET 0, A", 2, false, true, op_set_0_a}, // 0xCBC7
    {"SET 1, B", 2, false, true, op_set_1_b}, // 0xCBC8
    {"SET 1, C", 2, false, true, op_set_1_c}, // 0xCBC9
    {"SET 1, D", 2, false, true, op_set_1_d}, // 0xCBCA
    {"SET 1, E", 2, false, true, op_set_1_e}, // 0xCBCB
    {"SET 1, H", 2, false, true, op_set_1_h}, // 0xCBCC
    {"SET 1, L", 2, false, true, op_set_1_l}, // 0xCBCD
    {"SET 1, (HL)", 2, false, true, op_set_1_hlp}, // 0xCBCE
    {"SET 1, A", 2, false, true, op_set_1_a}, // 0xCBCF
    {"SET 2, B", 2, false, true, op_set_2_b}, // 0xCBD0
    {"SET 2, C", 2, false, true, op_set_2_c}, // 0xCBD1
    {"SET 2, D", 2, false, true, op_set_2_d}, // 0xCBD2
    {"SET 2, E", 2, false, true, op_set_2_e}, // 0xCBD3
    {"SET 2, H", 2, false, true, op_set_2_h}, // 0xCBD4
    {"SET 2, L", 2, false, true, op_set_2_l}, // 0xCBD5
    {"SET 2, (HL)", 2, false, true, op_set_2_hlp}, // 0xCBD6
    {"SET 2, A", 2, false, true, op_set_2_a}, // 0xCBD7
    {"SET 3, B", 2, false, true, op_set_3_b}, // 0xCBD8
    {"SET 3, C", 2, false, true, op_set_3_c}, // 0xCBD9
    {"SET 3, D", 2, false, true, op_set_3_d}, // 0xCBDA
    {"SET 3, E", 2, false, true, op_set_3_e}, // 0xCBDB
    {"SET 3, H", 2, false, true, op_set_3_h}, // 0xCBDC
    {"SET 3, L", 2, false, true, op_set_3_l}, // 0xCBDD
    {"SET 3, (HL)", 2, false, true, op_set_3_hlp}, // 0xCBDE
    {"SET 3, A", 2, false, true, op_set_3_a}, // 0xCBDF
    {"SET 4, B", 2, false, true, op_set_4_b}, // 0xCBE0
    {"SET 4, C", 2, false, true, op_set_4_c}, // 0xCBE1
    {"SET 4, D", 2, false, true, op_set_4_d}, // 0xCBE2
    {"SET 4, E", 2, false, true, op_set_4_e}, // 0xCBE3
    {"SET 4, H", 2, false, true, op_set_4_h}, // 0xCBE4
    {"SET 4, L", 2, false, true, op_set_4_l}, // 0xCBE5
    {"SET 4, (HL)", 2, false, true, op_set_4_hlp}, // 0xCBE6
    {"SET 4, A", 2, false, true, op_set_4_a}, // 0xCBE7
    {"SET 5, B", 2, false, true, op_set_5_b}, // 0xCBE8
    {"SET 5, C", 2, false, true, op_set_5_c}, // 0xCBE9
    {"SET 5, D", 2, false, true, op_set_5_d}, // 0xCBEA
    {"SET 5, E", 2, false, true, op_set_5_e}, // 0xCBEB
    {"SET 5, H", 2, false, true, op_set_5_h}, // 0xCBEC
    {"SET 5, L", 2, false, true, op_set_5_l}, // 0xCBED
    {"SET 5, (HL)", 2, false, true, op_set_5_hlp}, // 0xCBEE
    {"SET 5, A", 2, false, true, op_set_5_a}, // 0xCBEF
    {"SET 6, B", 2, false, true, op_set_6_b}, // 0xCBF0
    {"SET 6, C", 2, false, true, op_set_6_c}, // 0xCBF1
    {"SET 6, D", 2, false, true, op_set_6_d}, // 0xCBF2
    {"SET 6, E", 2, false, true, op_set_6_e}, // 0xCBF3
    {"SET 6, H", 2, false, true, op_set_6_h}, // 0xCBF4
    {"SET 6, L", 2, false, true, op_set_6_l}, // 0xCBF5
    {"SET 6, (HL)", 2, false, true, op_set_6_hlp}, // 0xCBF6
    {"SET 6, A", 2, false, true, op_set_6_a}, // 0xCBF7
    {"SET 7, B", 2, false, true, op_set_7_b}, // 0xCBF8
    {"SET 7, C", 2, false, true, op_set_7_c}, // 0xCBF9
    {"SET 7, D", 2, false, true, op_set_7_d}, // 0xCBFA
    {"SET 7, E", 2, false, true, op_set_7_e}, // 0xCBFB
    {"SET 7, H", 2, false, true, op_set_7_h}, // 0xCBFC
    {"SET 7, L", 2, false, true, op_set_7_l}, // 0xCBFD
    {"SET 7, (HL)", 2, false, true, op_set_7_hlp}, // 0xCBFE
    {"SET 7, A", 2, false, true, op_set_7_a} // 0xCBFF
};
