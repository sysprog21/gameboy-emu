
#include "instr.h"

void NOP(struct gb_s *gb, uint8_t opcode)
{
    // Do nothing
}

void LD16(struct gb_s *gb, uint8_t opcode)
{
    uint16_t temp;
    switch (opcode) {
    case 0x08:  // MEM_16 | REG_SP:
        break;
    case 0x01:  // REG_BC | IMM16:
        break;
    case 0x11:  // REG_DE | IMM16:
        break;
    case 0x21:  // REG_HL | IMM16:
    case 0xf8:  // REG_HL | MEM_8:
        break;
    case 0x31:  // REG_SP | IMM16:
    case 0xf9:  // REG_SP | REG_HL:
        break;
    }
}

void LD(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x02:  // MEM_BC | REG_A:
        break;
    case 0x06:  // REG_B | IMM8:
        break;
    case 0x0a:  // REG_A | MEM_BC:
        break;
    case  // REG_C | IMM8:
        break;
        case  // MEM_DE | REG_A:
        break;
        case  // REG_D | IMM8:
        break;
        case  // REG_A | MEM_DE:
        break;
        case  // REG_E | IMM8:
        break;
        case  // MEM_INC_HL | REG_A:
        break;
        case  // REG_H | IMM8:
        break;
        case REG_A | MEM_INC_HL:
        break;
    case REG_L | IMM8:
        break;
    case MEM_DEC_HL | REG_A:
        break;
    case MEM_HL | IMM8:
        break;
    case REG_A | MEM_DEC_HL:
        break;
    case REG_A | IMM8:
        break;
    case REG_B | REG_C:
    case REG_B | REG_D:
    case REG_B | REG_E:
    case REG_B | REG_H:
    case REG_B | REG_L:
    case REG_B | MEM_HL:
    case REG_B | REG_A:
        break;
    case REG_C | REG_B:
    case REG_C | REG_D:
    case REG_C | REG_E:
    case REG_C | REG_H:
    case REG_C | REG_L:
    case REG_C | MEM_HL:
    case REG_C | REG_A:
        break;
    case REG_D | REG_B:
    case REG_D | REG_C:
    case REG_D | REG_E:
    case REG_D | REG_H:
    case REG_D | REG_L:
    case REG_D | MEM_HL:
    case REG_D | REG_A:
        break;
    case REG_E | REG_B:
    case REG_E | REG_C:
    case REG_E | REG_D:
    case REG_E | REG_H:
    case REG_E | REG_L:
    case REG_E | MEM_HL:
    case REG_E | REG_A:
        break;
    case REG_H | REG_B:
    case REG_H | REG_C:
    case REG_H | REG_D:
    case REG_H | REG_E:
    case REG_H | REG_L:
    case REG_H | MEM_HL:
    case REG_H | REG_A:
        break;
    case REG_L | REG_B:
    case REG_L | REG_C:
    case REG_L | REG_D:
    case REG_L | REG_E:
    case REG_L | REG_H:
    case REG_L | MEM_HL:
    case REG_L | REG_A:
        break;
    case MEM_HL | REG_B:
    case MEM_HL | REG_C:
    case MEM_HL | REG_D:
    case MEM_HL | REG_E:
    case MEM_HL | REG_H:
    case MEM_HL | REG_L:
    case MEM_HL | REG_A:
        break;
    }
}

void INC16(struct gb_s *gb, cpu_instr instr) {}

void INC(struct gb_s *gb, cpu_instr instr) {}

void DEC16(struct gb_s *gb, cpu_instr instr) {}

void DEC(struct gb_s *gb, cpu_instr instr) {}

void RLC(struct gb_s *gb, cpu_instr instr) {}

void ADD16(struct gb_s *gb, cpu_instr instr) {}

void ADD(struct gb_s *gb, cpu_instr instr) {}

void RRC(struct gb_s *gb, cpu_instr instr) {}

void STOP(struct gb_s *gb, cpu_instr instr) {}

void RL(struct gb_s *gb, cpu_instr instr) {}

void JR(struct gb_s *gb, cpu_instr instr) {}

void RR(struct gb_s *gb, cpu_instr instr) {}

void DAA(struct gb_s *gb, cpu_instr instr) {}

void CPL(struct gb_s *gb, cpu_instr instr) {}

void SCF(struct gb_s *gb, cpu_instr instr) {}

void CCF(struct gb_s *gb, cpu_instr instr) {}

void HALT(struct gb_s *gb, cpu_instr instr) {}

void ADC(struct gb_s *gb, cpu_instr instr) {}

void SUB(struct gb_s *gb, cpu_instr instr) {}

void SBC(struct gb_s *gb, cpu_instr instr) {}

void AND(struct gb_s *gb, cpu_instr instr) {}

void XOR(struct gb_s *gb, cpu_instr instr) {}

void OR(struct gb_s *gb, cpu_instr instr) {}

void CP(struct gb_s *gb, cpu_instr instr) {}

void RET(struct gb_s *gb, cpu_instr instr) {}

void POP(struct gb_s *gb, cpu_instr instr) {}

void JP(struct gb_s *gb, cpu_instr instr) {}

void CALL(struct gb_s *gb, cpu_instr instr) {}

void PUSH(struct gb_s *gb, cpu_instr instr) {}

void RST(struct gb_s *gb, cpu_instr instr) {}

void RETI(struct gb_s *gb, cpu_instr instr) {}

void DI(struct gb_s *gb, cpu_instr instr) {}

void EI(struct gb_s *gb, cpu_instr instr) {}

void SLA(struct gb_s *gb, cpu_instr instr) {}

void SRA(struct gb_s *gb, cpu_instr instr) {}

void SWAP(struct gb_s *gb, cpu_instr instr) {}

void SRL(struct gb_s *gb, cpu_instr instr) {}

void BIT(struct gb_s *gb, cpu_instr instr) {}

void RES(struct gb_s *gb, cpu_instr instr) {}

void SET(struct gb_s *gb, cpu_instr instr) {}

void JP_TARGET(struct gb_s *gb, cpu_instr instr) {}

void JP_BWD(struct gb_s *gb, cpu_instr instr) {}

void JP_FWD(struct gb_s *gb, cpu_instr instr) {}

void ERROR(struct gb_s *gb, cpu_instr instr) {}
