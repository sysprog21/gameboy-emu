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
} 
