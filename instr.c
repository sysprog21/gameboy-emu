
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
        REG(sp) = REG(hl);
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
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
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void INC16(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x03:
        REG(bc)++;
        break;
    case 0x13:
        REG(de)++;
        break;
    case 0x23:
        REG(hl)++;
        break;
    case 0x33:
        REG(sp)++;
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void INC(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x04:
        DEFINE_INC_DECODER(b)
        break;
    case 0x0c:
        DEFINE_INC_DECODER(c)
        break;
    case 0x14:
        DEFINE_INC_DECODER(d)
        break;
    case 0x1c:
        DEFINE_INC_DECODER(e)
        break;
    case 0x24:
        DEFINE_INC_DECODER(h)
        break;
    case 0x2c:
        DEFINE_INC_DECODER(l)
        break;
    case 0x34:
        uint8_t temp = READ8(REG(hl)) + 1;
        REG(f_bits.z) = (temp == 0x00);
        REG(f_bits.n) = 0;
        REG(f_bits.h) = ((REG(b) & 0x0F) == 0x00);
        WRITE8(REG(hl), temp);
        break;
    case 0x3c:
        DEFINE_INC_DECODER(a)
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void DEC16(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x0b:
        REG(bc)--;
        break;
    case 0x1b:
        REG(de)--;
        break;
    case 0x2b:
        REG(hl)--;
        break;
    case 0x3b:
        REG(sp)--;
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void DEC(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x05:
        DEFINE_DEC_DECODER(b)
        break;
    case 0x0d:
        DEFINE_DEC_DECODER(c)
        break;
    case 0x15:
        DEFINE_DEC_DECODER(d)
        break;
    case 0x1d:
        DEFINE_DEC_DECODER(e)
        break;
    case 0x25:
        DEFINE_DEC_DECODER(h)
        break;
    case 0x2d:
        DEFINE_DEC_DECODER(l)
        break;
    case 0x35:
        uint8_t temp = READ8(REG(hl)) - 1;
        REG(f_bits.z) = (temp == 0x00);
        REG(f_bits.n) = 1;
        REG(f_bits.h) = ((REG(b) & 0x0F) == 0x0F);
        WRITE8(REG(hl), temp);
        break;
    case 0x3d:
        DEFINE_DEC_DECODER(a)
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void RLC(struct gb_s *gb, uint8_t opcode) {}

void ADD16(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x09:
        uint_fast32_t temp = REG(hl) + REG(bc);
        REG(f_bits.n) = 0;
        REG(f_bits.h) = (temp ^ REG(hl) ^ REG(bc) & 0x1000 ? 1 : 0);
        REG(f_bits.c) = (temp & 0xFFFF0000) ? 1 : 0;
        REG(hl) = (temp & 0x0000FFFF);
        break;
    case 0x19:
        uint_fast32_t temp = REG(hl) + REG(de);
        REG(f_bits.n) = 0;
        REG(f_bits.h) = (temp ^ REG(hl) ^ REG(de) & 0x1000 ? 1 : 0);
        REG(f_bits.c) = (temp & 0xFFFF0000) ? 1 : 0;
        REG(hl) = (temp & 0x0000FFFF);
        break;
    case 0x29:
        uint_fast32_t temp = REG(hl) + REG(hl);
        REG(f_bits.n) = 0;
        REG(f_bits.h) = (temp & 0x1000 ? 1 : 0);
        REG(f_bits.c) = (temp & 0xFFFF0000) ? 1 : 0;
        REG(hl) = (temp & 0x0000FFFF);
        break;
    case 0x39:
        uint_fast32_t temp = REG(hl) + REG(sp);
        REG(f_bits.n) = 0;
        REG(f_bits.h) =
            ((REG(hl) & 0xFFF) + (REG(sp) & 0xFFF)) & 0x1000 ? 1 : 0;
        REG(f_bits.c) = temp & 0x10000 ? 1 : 0;
        REG(hl) = (uint16_t) temp;
        break;
    case 0xe8:
        int8_t offset = (int8_t) READ8(REG(pc)++);
        REG(f_bits.z) = 0;
        REG(f_bits.n) = 0;
        REG(f_bits.h) = ((REG(sp) & 0xF) + (offset & 0xF) > 0xF) ? 1 : 0;
        REG(f_bits.c) = ((REG(sp) & 0xFF) + (offset & 0xFF) > 0xFF);
        REG(sp) += offset;
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void ADD(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x80:
        uint16_t temp = REG(a) + REG(b);
        DEFINE_ALU_DECODER(b, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0x81:
        uint16_t temp = REG(a) + REG(c);
        DEFINE_ALU_DECODER(c, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0x82:
        uint16_t temp = REG(a) + REG(d);
        DEFINE_ALU_DECODER(d, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0x83:
        uint16_t temp = REG(a) + REG(e);
        DEFINE_ALU_DECODER(e, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0x84:
        uint16_t temp = REG(a) + REG(h);
        DEFINE_ALU_DECODER(h, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0x85:
        uint16_t temp = REG(a) + REG(l);
        DEFINE_ALU_DECODER(l, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0x86:
        uint8_t hl = READ8(REG(hl));
        uint16_t temp = REG(a) + hl;
        REG(f_bits.z) = ((temp & 0xFF) == 0x00);
        REG(f_bits.n) = 0;
        REG(f_bits.h) = (REG(a) ^ hl ^ temp) & 0x10 ? 1 : 0;
        REG(f_bits.c) = (temp & 0xFF00) ? 1 : 0;
        REG(a) = (temp & 0xFF);
        break;
    case 0x87:
        uint16_t temp = REG(a) + REG(a);
        DEFINE_ALU_DECODER(a, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0xc6:
        uint8_t value = READ8(REG(pc)++);
        uint16_t calc = REG(a) + value;
        REG(f_bits.z) = ((uint8_t) calc == 0) ? 1 : 0;
        REG(f_bits.h) = ((REG(a) & 0xF) + (value & 0xF) > 0x0F) ? 1 : 0;
        REG(f_bits.c) = calc > 0xFF ? 1 : 0;
        REG(f_bits.n) = 0;
        REG(a) = (uint8_t) calc;
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void RRC(struct gb_s *gb, uint8_t opcode) {}

void STOP(struct gb_s *gb, uint8_t opcode)
{
    // 0x10: gb->gb_halt = 1;
}

void RL(struct gb_s *gb, uint8_t opcode) {}

void JR(struct gb_s *gb, uint8_t opcode, uint8_t *inst_cycles)
{
    switch (opcode) {
    case 0x18:
        int8_t temp = (int8_t) READ8(REG(pc)++);
        REG(pc) += temp;
        break;
    case 0x20:
        if (!REG(f_bits.z)) {
            int8_t temp = (int8_t) READ8(REG(pc)++);
            REG(pc) += temp;
            *inst_cycles += 4;
        } else
            REG(pc)++;
        break;
    case 0x28:
        if (!REG(f_bits.z)) {
            int8_t temp = (int8_t) READ8(REG(pc)++);
            REG(pc) += temp;
            *inst_cycles += 4;
        } else
            REG(pc)++;
        break;
    case 0x30:
        if (!REG(f_bits.c)) {
            int8_t temp = (int8_t) READ8(REG(pc)++);
            REG(pc) += temp;
            *inst_cycles += 4;
        } else
            REG(pc)++;
        break;
    case 0x38:
        if (REG(f_bits.c)) {
            int8_t temp = (int8_t) READ8(REG(pc)++);
            REG(pc) += temp;
            *inst_cycles += 4;
        } else
            REG(pc)++;
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void RR(struct gb_s *gb, uint8_t opcode) {}

void DAA(struct gb_s *gb, uint8_t opcode)  // 0x27
{
    uint16_t a = REG(a);
    if (REG(f_bits.n)) {
        if (REG(f_bits.n))
            a = (a - 0x06) & 0xFF;
        if (REG(f_bits.c))
            a -= 0x60;
    } else {
        if (REG(f_bits.h) || (a & 0x0F) > 9)
            a += 0x06;
        if (REG(f_bits.c) || a > 0x9F)
            a += 0x60;
    }
    if ((a & 0x100) == 0x100)
        REG(f_bits.c) = 1;
    REG(a) = a;
    REG(f_bits.z) = (REG(a) == 0);
    REG(f_bits.h) = 0;
    break;
}

void CPL(struct gb_s *gb, uint8_t opcode)  // 0x2F
{
    REG(a) = ~REG(a);
    REG(f_bits.n) = 1;
    REG(f_bits.h) = 1;
}

void SCF(struct gb_s *gb, uint8_t opcode)  // 0x37
{
    REG(f_bits.n) = 0;
    REG(f_bits.h) = 0;
    REG(f_bits.c) = 1;
}

void CCF(struct gb_s *gb, uint8_t opcode)  // 0x3F
{
    REG(f_bits.n) = 0;
    REG(f_bits.h) = 0;
    REG(f_bits.c) = ~REG(f_bits.c);
}

void HALT(struct gb_s *gb, uint8_t opcode)  // 0x76
{
    gb->gb_halt = 1;
}

void ADC(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x88:
        uint16_t temp = REG(a) + REG(b) + REG(f_bits.c);
        DEFINE_ALU_DECODER(b, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0x89:
        uint16_t temp = REG(a) + REG(c) + REG(f_bits.c);
        DEFINE_ALU_DECODER(c, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0x8a:
        uint16_t temp = REG(a) + REG(d) + REG(f_bits.c);
        DEFINE_ALU_DECODER(d, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0x8b:
        uint16_t temp = REG(a) + REG(e) + REG(f_bits.c);
        DEFINE_ALU_DECODER(e, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0x8c:
        uint16_t temp = REG(a) + REG(h) + REG(f_bits.c);
        DEFINE_ALU_DECODER(h, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0x8d:
        uint16_t temp = REG(a) + REG(l) + REG(f_bits.c);
        DEFINE_ALU_DECODER(l, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0x8e:
        uint8_t val = READ8(REG(hl));
        uint16_t temp = REG(a) + val + REG(f_bits.c);
        REG(f_bits.z) = ((temp & 0xFF) == 0x00);
        REG(f_bits.n) = 0;
        REG(f_bits.h) = (REG(a) ^ val ^ temp) & 0x10 ? 1 : 0;
        REG(f_bits.c) = (temp & 0xFF00) ? 1 : 0;
        REG(a) = (temp & 0xFF);
        break;
    case 0x8f:
        uint16_t temp = REG(a) + REG(a) + REG(f_bits.c);
        DEFINE_ALU_DECODER(a, 0)
        REG(a) = (temp & 0xFF);
        break;
    case 0xce:
        uint8_t value, a, carry;
        value = READ8(REG(pc)++);
        a = REG(a);
        carry = REG(f_bits.c);
        REG(a) = a + value + carry;
        REG(f_bits.z) = (REG(a) == 0) ? 1 : 0;
        REG(f_bits.h) = ((a & 0xF) + (value & 0xF) + carry > 0x0F) ? 1 : 0;
        REG(f_bits.c) =
            (((uint16_t) a) + ((uint16_t) value) + carry > 0xFF) ? 1 : 0;
        REG(f_bits.n) = 0;
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void SUB(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x90:
        uint16_t temp = REG(a) - REG(b);
        DEFINE_ALU_DECODER(b, 1)
        REG(a) = (temp & 0xFF);
        break;
    case 0x91:
        uint16_t temp = REG(a) - REG(c);
        DEFINE_ALU_DECODER(c, 1)
        REG(a) = (temp & 0xFF);
        break;
    case 0x92:
        uint16_t temp = REG(a) - REG(d);
        DEFINE_ALU_DECODER(d, 1)
        REG(a) = (temp & 0xFF);
        break;
    case 0x93:
        uint16_t temp = REG(a) - REG(e);
        DEFINE_ALU_DECODER(e, 1)
        REG(a) = (temp & 0xFF);
        break;
    case 0x94:
        uint16_t temp = REG(a) - REG(h);
        DEFINE_ALU_DECODER(h, 1)
        REG(a) = (temp & 0xFF);
        break;
    case 0x95:
        uint16_t temp = REG(a) - REG(l);
        DEFINE_ALU_DECODER(l, 1)
        REG(a) = (temp & 0xFF);
        break;
    case 0x96:
        uint8_t val = READ8(REG(hl));
        uint16_t temp = REG(a) - val;
        REG(f_bits.z) = ((temp & 0xFF) == 0x00);
        REG(f_bits.n) = 1;
        REG(f_bits.h) = (REG(a) ^ val ^ temp) & 0x10 ? 1 : 0;
        REG(f_bits.c) = (temp & 0xFF00) ? 1 : 0;
        REG(a) = (temp & 0xFF);
        break;
    case 0x97:
        REG(a) = 0;
        REG(f_bits.z) = 1;
        REG(f_bits.n) = 1;
        REG(f_bits.h) = 0;
        REG(f_bits.c) = 0;
        break;
    case 0xd6:
        uint8_t val = READ8(REG(pc)++);
        uint16_t temp = REG(a) - val;
        REG(f_bits.z) = ((temp & 0xFF) == 0x00);
        REG(f_bits.n) = 1;
        REG(f_bits.h) = (REG(a) ^ val ^ temp) & 0x10 ? 1 : 0;
        REG(f_bits.c) = (temp & 0xFF00) ? 1 : 0;
        REG(a) = (temp & 0xFF);
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void SBC(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0x98:
        uint16_t temp = REG(a) - REG(b) - REG(f_bits.c);
        DEFINE_ALU_DECODER(b, 1)
        REG(a) = (temp & 0xFF);
        break;
    case 0x99:
        uint16_t temp = REG(a) - REG(c) - REG(f_bits.c);
        DEFINE_ALU_DECODER(c, 1)
        REG(a) = (temp & 0xFF);
        break;
    case 0x9a:
        uint16_t temp = REG(a) - REG(d) - REG(f_bits.c);
        DEFINE_ALU_DECODER(d, 1)
        REG(a) = (temp & 0xFF);
        break;
    case 0x9b:
        uint16_t temp = REG(a) - REG(e) - REG(f_bits.c);
        DEFINE_ALU_DECODER(e, 1)
        REG(a) = (temp & 0xFF);
        break;
    case 0x9c:
        uint16_t temp = REG(a) - REG(h) - REG(f_bits.c);
        DEFINE_ALU_DECODER(h, 1)
        REG(a) = (temp & 0xFF);
        break;
    case 0x9d:
        uint16_t temp = REG(a) - REG(l) - REG(f_bits.c);
        DEFINE_ALU_DECODER(l, 1)
        REG(a) = (temp & 0xFF);
        break;
    case 0x9e:
        uint8_t val = READ8(REG(hl));
        uint16_t temp = REG(a) - val - REG(f_bits.c);
        REG(f_bits.z) = ((temp & 0xFF) == 0x00);
        REG(f_bits.n) = 1;
        REG(f_bits.h) = (REG(a) ^ val ^ temp) & 0x10 ? 1 : 0;
        REG(f_bits.c) = (temp & 0xFF00) ? 1 : 0;
        REG(a) = (temp & 0xFF);
        break;
    case 0x9f:
        REG(a) = REG(f_bits.c) ? 0xFF : 0x00;
        REG(f_bits.z) = REG(f_bits.c) ? 0x00 : 0x01;
        REG(f_bits.n) = 1;
        REG(f_bits.h) = REG(f_bits.c);
        break;
    case 0xde:
        uint8_t temp_8 = READ8(REG(pc)++);
        uint16_t temp_16 = REG(a) - temp_8 - REG(f_bits.c);
        REG(f_bits.z) = ((temp_16 & 0xFF) == 0x00);
        REG(f_bits.n) = 1;
        REG(f_bits.h) = (REG(a) ^ temp_8 ^ temp_16) & 0x10 ? 1 : 0;
        REG(f_bits.c) = (temp_16 & 0xFF00) ? 1 : 0;
        REG(a) = (temp_16 & 0xFF);
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void AND(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0xa0:
        REG(a) = REG(a) & REG(b);
        DEFINE_LOGIC_DECODER(0, 1, 0)
        break;
    case 0xa1:
        REG(a) = REG(a) & REG(c);
        DEFINE_LOGIC_DECODER(0, 1, 0)
        break;
    case 0xa2:
        REG(a) = REG(a) & REG(d);
        DEFINE_LOGIC_DECODER(0, 1, 0)
        break;
    case 0xa3:
        REG(a) = REG(a) & REG(e);
        DEFINE_LOGIC_DECODER(0, 1, 0)
        break;
    case 0xa4:
        REG(a) = REG(a) & REG(h);
        DEFINE_LOGIC_DECODER(0, 1, 0)
        break;
    case 0xa5:
        REG(a) = REG(a) & REG(l);
        DEFINE_LOGIC_DECODER(0, 1, 0)
        break;
    case 0xa6:
        REG(a) = REG(a) & READ8(REG(hl));
        DEFINE_LOGIC_DECODER(0, 1, 0)
        break;
    case 0xa7:
        DEFINE_LOGIC_DECODER(0, 1, 0)
        break;
    case 0xe6:
        REG(a) = REG(a) & READ8(REG(pc)++);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void XOR(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0xa8:
        REG(a) = REG(a) ^ REG(b);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xa9:
        REG(a) = REG(a) ^ REG(c);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xaa:
        REG(a) = REG(a) ^ REG(d);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xab:
        REG(a) = REG(a) ^ REG(e);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xac:
        REG(a) = REG(a) ^ REG(h);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xad:
        REG(a) = REG(a) ^ REG(l);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xae:
        REG(a) = REG(a) ^ READ8(REG(hl));
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xaf:
        REG(a) = 0x00;
        REG(f_bits.z) = 1;
        REG(f_bits.n) = 0;
        REG(f_bits.h) = 0;
        REG(f_bits.c) = 0;
        break;
    case 0xee:
        REG(a) = REG(a) ^ READ8(REG(pc)++);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void OR(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0xb0:
        REG(a) = REG(a) | REG(b);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xb1:
        REG(a) = REG(a) | REG(c);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xb2:
        REG(a) = REG(a) | REG(d);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xb3:
        REG(a) = REG(a) | REG(e);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xb4:
        REG(a) = REG(a) | REG(h);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xb5:
        REG(a) = REG(a) | REG(l);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xb6:
        REG(a) = REG(a) | READ8(REG(hl));
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xb7:
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    case 0xf6:
        REG(a) = REG(a) | READ8(REG(pc)++);
        DEFINE_LOGIC_DECODER(0, 0, 0)
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void CP(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0xb8:
        uint16_t temp = REG(a) - REG(b);
        DEFINE_ALU_DECODER(b, 1)
        break;
    case 0xb9:
        uint16_t temp = REG(a) - REG(c);
        DEFINE_ALU_DECODER(c, 1)
        break;
    case 0xba:
        uint16_t temp = REG(a) - REG(d);
        DEFINE_ALU_DECODER(d, 1)
        break;
    case 0xbb:
        uint16_t temp = REG(a) - REG(e);
        DEFINE_ALU_DECODER(e, 1)
        break;
    case 0xbc:
        uint16_t temp = REG(a) - REG(h);
        DEFINE_ALU_DECODER(h, 1)
        break;
    case 0xbd:
        uint16_t temp = REG(a) - REG(l);
        DEFINE_ALU_DECODER(l, 1)
        break;
    case 0xbe:
        uint8_t val = READ8(REG(hl));
        uint16_t temp = REG(a) - val;
        REG(f_bits.z) = ((temp & 0xFF) == 0x00);
        REG(f_bits.n) = 1;
        REG(f_bits.h) = (REG(a) ^ val ^ temp) & 0x10 ? 1 : 0;
        REG(f_bits.c) = (temp & 0xFF00) ? 1 : 0;
        break;
    case 0xbf:
        REG(f_bits.z) = 1;
        REG(f_bits.n) = 1;
        REG(f_bits.h) = 0;
        REG(f_bits.c) = 0;
        break;
    case 0xfe:
        uint8_t temp_8 = READ8(REG(pc)++);
        uint16_t temp_16 = REG(a) - temp_8;
        REG(f_bits.z) = ((temp_16 & 0xFF) == 0x00);
        REG(f_bits.n) = 1;
        REG(f_bits.h) = (REG(a) ^ temp_8 ^ temp_16) & 0x10 ? 1 : 0;
        REG(f_bits.c) = (temp_16 & 0xFF00) ? 1 : 0;
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void RET(struct gb_s *gb, uint8_t opcode, uint8_t *inst_cycles)
{
    switch (opcode) {
    case 0xc0:
        if (!REG(f_bits.z)) {
            REG(pc) = READ8(REG(sp)++);
            REG(pc) |= READ8(REG(sp)++) << 8;
            *inst_cycles += 12;
        }
    case 0xc8:
    case 0xc9:
    case 0xd0:
    case 0xd8:
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void POP(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0xc1:
        REG(c) = READ8(REG(sp)++);
        REG(b) = READ8(REG(sp)++);
        break;
    case 0xd1:
        REG(e) = READ8(REG(sp)++);
        REG(d) = READ8(REG(sp)++);
        break;
    case 0xe1:
        REG(l) = READ8(REG(sp)++);
        REG(h) = READ8(REG(sp)++);
        break;
    case 0xf1:
        uint8_t temp_8 = READ8(REG(sp)++);
        REG(f_bits.z) = (temp_8 >> 7) & 1;
        REG(f_bits.n) = (temp_8 >> 6) & 1;
        REG(f_bits.h) = (temp_8 >> 5) & 1;
        REG(f_bits.c) = (temp_8 >> 4) & 1;
        REG(a) = READ8(REG(sp)++);
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void JP(struct gb_s *gb, uint8_t opcode, uint8_t *inst_cycles)
{
    switch (opcode) {
    case 0xc2:
        if (!REG(f_bits.z)) {
            uint16_t temp = READ8(REG(pc)++);
            temp |= READ8(REG(pc)++) << 8;
            REG(pc) = temp;
            *inst_cycles += 4;
        } else
            REG(pc) += 2;
        break;
    case 0xc3:
    case 0xca:
    case 0xd2:
    case 0xda:
    case 0xe9:
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void CALL(struct gb_s *gb, uint8_t opcode, uint8_t *inst_cycles)
{
    switch (opcode) {
    case 0xc4:
        if (!REG(f_bits.z)) {
            uint16_t temp = READ8(REG(pc)++);
            temp |= READ8(REG(pc)++) << 8;
            WRITE8(--REG(sp), REG(pc) >> 8);
            WRITE8(--REG(sp), REG(pc) & 0xFF);
            REG(pc) = temp;
            *inst_cycles += 12;
        } else
            REG(pc) += 2;
        break;
    case 0xcc:
    case 0xcd:
    case 0xd4:
    case 0xdc:
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void PUSH(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0xc5:
        WRITE8(--REG(sp), REG(b));
        WRITE8(--REG(sp), REG(c));
        break;
    case 0xd5:
        WRITE8(--REG(sp), REG(d));
        WRITE8(--REG(sp), REG(e));
        break;
    case 0xe5:
        WRITE8(--REG(sp), REG(h));
        WRITE8(--REG(sp), REG(l));
        break;
    case 0xf5:
        WRITE8(--REG(sp), REG(a));
        WRITE8(--REG(sp), REG(f_bits.z) << 7 | REG(f_bits.n) << 6 |
                              REG(f_bits.h) << 5 | REG(f_bits.c) << 4);
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void RST(struct gb_s *gb, uint8_t opcode)
{
    switch (opcode) {
    case 0xc7:
        WRITE8(--REG(sp), REG(pc) >> 8);
        WRITE8(--REG(sp), REG(pc) & 0xFF);
        REG(pc) = 0x0000;
        break;
    case 0xcf:
        WRITE8(--REG(sp), REG(pc) >> 8);
        WRITE8(--REG(sp), REG(pc) & 0xFF);
        REG(pc) = 0x0008;
        break;
    case 0xd7:
        WRITE8(--REG(sp), REG(pc) >> 8);
        WRITE8(--REG(sp), REG(pc) & 0xFF);
        REG(pc) = 0x0010;
        break;
    case 0xdf:
        WRITE8(--REG(sp), REG(pc) >> 8);
        WRITE8(--REG(sp), REG(pc) & 0xFF);
        REG(pc) = 0x0018;
        break;
    case 0xe7:
        WRITE8(--REG(sp), REG(pc) >> 8);
        WRITE8(--REG(sp), REG(pc) & 0xFF);
        REG(pc) = 0x0020;
        break;
    case 0xef:
        WRITE8(--REG(sp), REG(pc) >> 8);
        WRITE8(--REG(sp), REG(pc) & 0xFF);
        REG(pc) = 0x0028;
        break;
    case 0xf7:
        WRITE8(--REG(sp), REG(pc) >> 8);
        WRITE8(--REG(sp), REG(pc) & 0xFF);
        REG(pc) = 0x0030;
        break;
    case 0xff:
        WRITE8(--REG(sp), REG(pc) >> 8);
        WRITE8(--REG(sp), REG(pc) & 0xFF);
        REG(pc) = 0x0038;
        break;
    default:
        (gb->gb_error)(gb, GB_INVALID_OPCODE, opcode);
        break;
    }
}

void RETI(struct gb_s *gb, uint8_t opcode)  // 0xD9
{
    uint16_t temp = READ8(REG(sp)++);
    temp |= READ8(REG(sp)++) << 8;
    REG(pc) = temp;
    gb->gb_ime = 1;
}

void DI(struct gb_s *gb, uint8_t opcode)  // 0xF3
{
    gb->gb_ime = 0;
}

void EI(struct gb_s *gb, uint8_t opcode)  // 0xFB
{
    gb->gb_ime = 1;
}

void SLA(struct gb_s *gb, uint8_t opcode) {}

void SRA(struct gb_s *gb, uint8_t opcode) {}

void SWAP(struct gb_s *gb, uint8_t opcode) {}

void SRL(struct gb_s *gb, uint8_t opcode) {}

void BIT(struct gb_s *gb, uint8_t opcode) {}

void RES(struct gb_s *gb, uint8_t opcode) {}

void SET(struct gb_s *gb, uint8_t opcode) {}

void ERROR(struct gb_s *gb, uint8_t opcode) {}
