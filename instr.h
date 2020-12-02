
#include "cpu.h"

static const cpu_instr_s cpu_instr_table[] = {
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
}
