
#include "instr.h"

void NOP(struct gb_s *gb, uint8_t opcode)
{
    // Do nothing
}

void LD16(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x08:  // MEM_16 | REG_SP:
        uint16_t temp = READ8(REG(pc)++);
        temp |= READ8(REG(pc)++) << 8;
        WRITE8(temp++, REG(sp) & 0xFF);
        WRITE8(temp, REG(sp) >> 8);
        break;
    case 0x01:  // REG_BC | IMM16:
        REG(c) = READ8(REG(pc)++);
        REG(b) = READ8(REG(pc)++);
        break;
    case 0x11:  // REG_DE | IMM16:
        REG(e) = READ8(REG(pc)++);
        REG(d) = READ8(REG(pc)++);
        break;
    case 0x21:  // REG_HL | IMM16:
        REG(l) = READ8(REG(pc)++);
        REG(h) = READ8(REG(pc)++);
        break;
    case 0xf8:  // REG_HL | MEM_8:
        int8_t offset = (int8_t) READ8(REG(pc)++);
        REG(hl) = REG(sp) + offset;
        REG(f_bits.z) = 0;
        REG(f_bits.n) = 0;
        REG(f_bits.h) = ((REG(sp) & 0xF) + (offset & 0xF) > 0xF) ? 1 : 0;
        REG(f_bits.c) = ((REG(sp) & 0xFF) + (offset & 0xFF) > 0xFF) ? 1 : 0;
        break;
    case 0x31:  // REG_SP | IMM16:
        REG(sp) = READ8(REG(pc)++);
        REG(sp) |= READ8(REG(pc)++) << 8;
        break;
    case 0xf9:  // REG_SP | REG_HL:
        REG(sp) = REG(hl) break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
    }
}

void LD(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x02:  // MEM_BC | REG_A:
        WRITE8(REG(bc), REG(a));
        break;
    case 0x06:  // REG_B | IMM8:
        REG(b) = READ8(REG(pc)++);
        break;
    case 0x0a:  // REG_A | MEM_BC:
        REG(a) = READ8(REG(bc));
        break;
    case 0x0e:  // REG_C | IMM8:
        REG(c) = READ8(REG(pc)++);
        break;
    case 0x12:  // MEM_DE | REG_A:
        WRITE8(REG(de), REG(a));
        break;
    case 0x16:  // REG_D | IMM8:
        REG(d) = READ8(REG(pc)++);
        break;
    case 0x1a:  // REG_A | MEM_DE:
        REG(a) = READ8(REG(de));
        break;
    case 0x1e:  // REG_E | IMM8:
        REG(e) = READ8(REG(pc)++);
        break;
    case 0x22:  // MEM_INC_HL | REG_A:
        WRITE8(REG(hl), REG(a));
        REG(hl)++;
        break;
    case 0x26:  // REG_H | IMM8:
        REG(h) = READ8(REG(pc)++);
        break;
    case 0x2a:  // REG_A | MEM_INC_HL:
        REG(a) = READ8(REG(hl)++);
        break;
    case 0x2e:  // REG_L | IMM8:
        REG(l) = READ8(REG(pc)++) break;
    case 0x32:  // MEM_DEC_HL | REG_A:
        WRITE8(REG(hl), REG(a));
        REG(hl)--;
        break;
    case 0x36:  // MEM_HL | IMM8:
        WRITE8(REG(hl), READ8(REG(pc)++));
        break;
    case 0x3a:  // REG_A | MEM_DEC_HL:
        REG(a) = READ8(REG(hl)--);
        break;
    case 0x3e:  // REG_A | IMM8:
        REG(a) = READ8(REG(pc)++);
        break;
    case 0x41:  // REG_B | REG_C:
        REG(b) = REG(c);
        break;
    case 0x42:  // REG_B | REG_D:
        REG(b) = REG(d);
        break;
    case 0x43:  // REG_B | REG_E:
        REG(b) = REG(e);
        break;
    case 0x44:  // REG_B | REG_H:
        REG(b) = REG(h);
        break;
    case 0x45:  // REG_B | REG_L:
        REG(b) = REG(l);
        break;
    case 0x46:  // REG_B | MEM_HL:
        REG(b) = READ8(REG(hl));
        break;
    case 0x47:  // REG_B | REG_A:
        REG(b) = REG(a);
        break;
    case 0x48:  // REG_C | REG_B:
        REG(c) = REG(b);
        break;
    case 0x4a:  // REG_C | REG_D:
        REG(c) = REG(d);
        break;
    case 0x4b:  // REG_C | REG_E:
        REG(c) = REG(e);
        break;
    case 0x4c:  // REG_C | REG_H:
        REG(c) = REG(h);
        break;
    case 0x4d:  // REG_C | REG_L:
        REG(c) = REG(l);
        break;
    case 0x4e:  // REG_C | MEM_HL:
        REG(c) = READ8(REG(hl));
        break;
    case 0x4f:  // REG_C | REG_A:
        REG(c) = REG(a);
        break;
    case 0x50:  // REG_D | REG_B:
        REG(d) = REG(b);
        break;
    case 0x51:  // REG_D | REG_C:
        REG(d) = REG(c);
        break;
    case 0x53:  // REG_D | REG_E:
        REG(d) = REG(e);
        break;
    case 0x54:  // REG_D | REG_H:
        REG(d) = REG(h);
        break;
    case 0x55:  // REG_D | REG_L:
        REG(d) = REG(l);
        break;
    case 0x56:  // REG_D | MEM_HL:
        REG(d) = READ8(REG(hl));
        break;
    case 0x57:  // REG_D | REG_A:
        REG(d) = REG(a);
        break;
    case 0x58:  // REG_E | REG_B:
        REG(e) = REG(b);
        break;
    case 0x59:  // REG_E | REG_C:
        REG(e) = REG(c);
        break;
    case 0x5a:  // REG_E | REG_D:
        REG(e) = REG(d);
        break;
    case 0x5c:  // REG_E | REG_H:
        REG(e) = REG(h);
        break;
    case 0x5d:  // REG_E | REG_L:
        REG(e) = REG(l);
        break;
    case 0x5e:  // REG_E | MEM_HL:
        REG(e) = READ8(REG(hl));
        break;
    case 0x5f:  // REG_E | REG_A:
        REG(e) = REG(a);
        break;
    case 0x60:  // REG_H | REG_B:
        REG(h) = REG(b);
        break;
    case 0x61:  // REG_H | REG_C:
        REG(h) = REG(c);
        break;
    case 0x62:  // REG_H | REG_D:
        REG(h) = REG(d);
        break;
    case 0x63:  // REG_H | REG_E:
        REG(h) = REG(e);
        break;
    case 0x65:  // REG_H | REG_L:
        REG(h) = REG(l);
        break;
    case 0x66:  // REG_H | MEM_HL:
        REG(h) = READ8(REG(hl));
        break;
    case 0x67:  // REG_H | REG_A:
        REG(h) = REG(a);
        break;
    case 0x68:  // REG_L | REG_B:
        REG(l) = REG(b);
        break;
    case 0x69:  // REG_L | REG_C:
        REG(l) = REG(c);
        break;
    case 0x6a:  // REG_L | REG_D:
        REG(l) = REG(d);
        break;
    case 0x6b:  // REG_L | REG_E:
        REG(l) = REG(e);
        break;
    case 0x6c:  // REG_L | REG_H:
        REG(l) = REG(h);
        break;
    case 0x6e:  // REG_L | MEM_HL:
        REG(l) = READ8(REG(hl));
        break;
    case 0x6f:  // REG_L | REG_A:
        REG(l) = REG(a);
        break;
    case 0x70:  // MEM_HL | REG_B:
        WRITE8(REG(hl), REG(b));
        break;
    case 0x71:  // MEM_HL | REG_C:
        WRITE8(REG(hl), REG(c));
        break;
    case 0x72:  // MEM_HL | REG_D:
        WRITE8(REG(hl), REG(d));
        break;
    case 0x73:  // MEM_HL | REG_E:
        WRITE8(REG(hl), REG(e));
        break;
    case 0x74:  // MEM_HL | REG_H:
        WRITE8(REG(hl), REG(h));
        break;
    case 0x75:  // MEM_HL | REG_L:
        WRITE8(REG(hl), REG(l));
        break;
    case 0x77:  // MEM_HL | REG_A:
        WRITE8(REG(hl), REG(a));
        break;
    case 0x78:  // REG_A | REG_B
        REG(a) = REG(b);
        break;
    case 0x79:
        REG(a) = REG(c);
        break;
    case 0x7a:
        REG(a) = REG(d);
        break;
    case 0x7b:
        REG(a) = REG(e);
        break;
    case 0x7c:
        REG(a) = REG(h);
        break;
    case 0x7d:
        REG(a) = REG(l);
        break;
    case 0x7e:
        REG(a) = READ8(REG(hl));
        break;
    case 0xe0:  // MEM_8 | REG_A
        WRITE8(0xFF00 | READ8(REG(pc)++), REG(a));
        break;
    case 0xe2:
        WRITE8(0xFF00 | REG(c), REG(a));
        break;
    case 0xea:
        uint16_t addr = READ8(REG(pc)++);
        addr |= READ8(REG(pc)++) << 8;
        WRITE8(addr, REG(a));
        break;
    case 0xf0:
        REG(a) = READ8(0xFF00 | READ8(REG(pc)++));
        break;
    case 0xf2:
        REG(a) = READ8(0xFF00 | REG(c));
        break;
    case 0xfa:
        uint16_t addr = READ8(REG(pc)++);
        addr |= READ8(REG(pc)++) << 8;
        REG(a) = READ8(addr);
        break;
    }
}

void INC16(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x03:
    case 0x13:
    case 0x23:
    case 0x33:
    }
}

void INC(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x04:
    case 0x0c:
    case 0x14:
    case 0x1c:
    case 0x24:
    case 0x2c:
    case 0x34:
    case 0x3c:
    }
}

void DEC16(struct gb_s *gb, uint8_t opcode) {}

void DEC(struct gb_s *gb, uint8_t opcode) {}

void RLC(struct gb_s *gb, uint8_t opcode) {}

void ADD16(struct gb_s *gb, uint8_t opcode) {}

void ADD(struct gb_s *gb, uint8_t opcode) {}

void RRC(struct gb_s *gb, uint8_t opcode) {}

void STOP(struct gb_s *gb, uint8_t opcode) {}

void RL(struct gb_s *gb, uint8_t opcode) {}

void JR(struct gb_s *gb, uint8_t opcode) {}

void RR(struct gb_s *gb, uint8_t opcode) {}

void DAA(struct gb_s *gb, uint8_t opcode) {}

void CPL(struct gb_s *gb, uint8_t opcode) {}

void SCF(struct gb_s *gb, uint8_t opcode) {}

void CCF(struct gb_s *gb, uint8_t opcode) {}

void HALT(struct gb_s *gb, uint8_t opcode) {}

void ADC(struct gb_s *gb, uint8_t opcode) {}

void SUB(struct gb_s *gb, uint8_t opcode) {}

void SBC(struct gb_s *gb, uint8_t opcode) {}

void AND(struct gb_s *gb, uint8_t opcode) {}

void XOR(struct gb_s *gb, uint8_t opcode) {}

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
