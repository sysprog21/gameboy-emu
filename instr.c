
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
      case 0x02: // MEM_BC | REG_A:
        break;
      case 0x06: // REG_B | IMM8:
        break;
      case 0x0a: // REG_A | MEM_BC:
        break;
      case 0x0e: // REG_C | IMM8:
        break;
      case 0x12: // MEM_DE | REG_A:
        break;
      case 0x16: // REG_D | IMM8:
        break;
      case 0x1a: // REG_A | MEM_DE:
        break;
      case 0x1e: // REG_E | IMM8:
        break;
      case 0x22: // MEM_INC_HL | REG_A:
        break;
      case 0x26: // REG_H | IMM8:
        break;
      case 0x2a: // REG_A | MEM_INC_HL:
        break;
      case 0x2e: // REG_L | IMM8:
        break;
      case 0x32: // MEM_DEC_HL | REG_A:
        break;
      case 0x36: // MEM_HL | IMM8:
        break;
      case 0x3a: // REG_A | MEM_DEC_HL:
        break;
      case 0x3e: // REG_A | IMM8:
        break;
      case 0x41: // REG_B | REG_C:
      case 0x42: // REG_B | REG_D:
      case 0x43: // REG_B | REG_E:
      case 0x44: // REG_B | REG_H:
      case 0x45: // REG_B | REG_L:
      case 0x46: // REG_B | MEM_HL:
      case 0x47: // REG_B | REG_A:
        break;
      case 0x48: // REG_C | REG_B:
      case 0x4a: // REG_C | REG_D:
      case 0x4b: // REG_C | REG_E:
      case 0x4c: // REG_C | REG_H:
      case 0x4d: // REG_C | REG_L:
      case 0x4e: // REG_C | MEM_HL:
      case 0x4f: // REG_C | REG_A:
        break;
      case 0x50: // REG_D | REG_B:
      case 0x51: // REG_D | REG_C:
      case 0x53: // REG_D | REG_E:
      case 0x54: // REG_D | REG_H:
      case 0x55: // REG_D | REG_L:
      case 0x56: // REG_D | MEM_HL:
      case 0x57: // REG_D | REG_A:
        break;
      case 0x58: // REG_E | REG_B:
      case 0x59: // REG_E | REG_C:
      case 0x5a: // REG_E | REG_D:
      case 0x5c: // REG_E | REG_H:
      case 0x5d: // REG_E | REG_L:
      case 0x5e: // REG_E | MEM_HL:
      case 0x5f: // REG_E | REG_A:
        break;
      case 0x60: // REG_H | REG_B:
      case 0x61: // REG_H | REG_C:
      case 0x62: // REG_H | REG_D:
      case 0x63: // REG_H | REG_E:
      case 0x65: // REG_H | REG_L:
      case 0x66: // REG_H | MEM_HL:
      case 0x67: // REG_H | REG_A:
        break;
      case 0x68: // REG_L | REG_B:
      case 0x69: // REG_L | REG_C:
      case 0x6a: // REG_L | REG_D:
      case 0x6b: // REG_L | REG_E:
      case 0x6c: // REG_L | REG_H:
      case 0x6e: // REG_L | MEM_HL:
      case 0x6f: // REG_L | REG_A:
        break;
      case 0x70: // MEM_HL | REG_B:
      case 0x71: // MEM_HL | REG_C:
      case 0x72: // MEM_HL | REG_D:
      case 0x73: // MEM_HL | REG_E:
      case 0x74: // MEM_HL | REG_H:
      case 0x75: // MEM_HL | REG_L:
      case 0x77: // MEM_HL | REG_A:
        break;
      case 0x78: // REG_A | REG_B
      case 0x79:
      case 0x7a:
      case 0x7b:
      case 0x7c:
      case 0x7d:
      case 0x7e:
        break;
      case 0xe0: // MEM_8 | REG_A
      case 0xe2:
      case 0xea:
      case 0xf0:
      case 0xf2:
      case 0xfa:
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
