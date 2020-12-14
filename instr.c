#include "instr.h"

/* clang-format off */

#define DEFINE_INSTRUCTION_Z80(NAME, BODY) \
	void _Z80Instruction ## NAME (struct gb_s *gb) { \
		UNUSED(gb); \
		BODY; \
	}

#define DEFINE_CONDITION_INSTRUCTION_Z80(NAME, BODY) \
	void _Z80Instruction ## NAME (struct gb_s *gb, uint8_t *inst_cycles) { \
		UNUSED(gb); \
		BODY; \
	}

static inline uint16_t Z80ReadHL(struct gb_s *gb) {
	return __gb_read(gb, gb->cpu_reg.hl);
}

static inline void Z80WriteHL(struct gb_s *gb, uint16_t hl) {
	__gb_write(gb, gb->cpu_reg.hl, hl);
}

static inline uint16_t Z80ReadBC(struct gb_s *gb) {
	return __gb_read(gb, gb->cpu_reg.bc);
}

static inline void Z80WriteBC(struct gb_s *gb, uint16_t bc) {
	__gb_write(gb, gb->cpu_reg.bc, bc);
}

static inline uint16_t Z80ReadDE(struct gb_s *gb) {
	return __gb_read(gb, gb->cpu_reg.de);
}

static inline void Z80WriteDE(struct gb_s *gb, uint16_t de) {
	__gb_write(gb, gb->cpu_reg.de, de);
}

DEFINE_INSTRUCTION_Z80(NOP,);

#define DEFINE_JP_INSTRUCTION_Z80(CONDITION_NAME, CONDITION) \
	DEFINE_CONDITION_INSTRUCTION_Z80(JP ## CONDITION_NAME, \
	if (CONDITION) { \
		gb->cpu_reg.pc = __gb_read(gb, gb->cpu_reg.pc++); \
		gb->cpu_reg.pc |= __gb_read(gb, gb->cpu_reg.pc++) << 8; \
		*inst_cycles += 4; \
	} else \
		gb->cpu_reg.pc += 2;)

#define DEFINE_CONDITIONAL_INSTRUCTION_Z80(NAME) \
	DEFINE_ ## NAME ## _INSTRUCTION_Z80(C, gb->cpu_reg.f_bits.c) \
	DEFINE_ ## NAME ## _INSTRUCTION_Z80(Z, gb->cpu_reg.f_bits.z) \
	DEFINE_ ## NAME ## _INSTRUCTION_Z80(NC, !gb->cpu_reg.f_bits.c) \
	DEFINE_ ## NAME ## _INSTRUCTION_Z80(NZ, !gb->cpu_reg.f_bits.z)

DEFINE_CONDITIONAL_INSTRUCTION_Z80(JP);

DEFINE_INSTRUCTION_Z80(JP, // JP imm
    gb->cpu_reg.pc = __gb_read(gb, gb->cpu_reg.pc++);
    gb->cpu_reg.pc |= __gb_read(gb, gb->cpu_reg.pc) << 8;)

DEFINE_INSTRUCTION_Z80(JPHL, // JP HL
	gb->cpu_reg.pc = gb->cpu_reg.hl;)

#define DEFINE_JR_INSTRUCTION_Z80(CONDITION_NAME, CONDITION) \
	DEFINE_CONDITION_INSTRUCTION_Z80(JR ## CONDITION_NAME, \
	if (CONDITION) { \
		gb->cpu_reg.pc += (int8_t) __gb_read(gb, gb->cpu_reg.pc++); \
		*inst_cycles += 4; \
	} else \
		gb->cpu_reg.pc++;)

DEFINE_CONDITIONAL_INSTRUCTION_Z80(JR);

DEFINE_INSTRUCTION_Z80(JR
    int8_t diff = (int8_t) __gb_read(gb, gb->cpu_reg.pc++);
    gb->cpu_reg.pc += diff;)

#define DEFINE_CALL_INSTRUCTION_Z80(CONDITION_NAME, CONDITION) \
	  DEFINE_CONDITION_INSTRUCTION_Z80(CALL ## CONDITION_NAME, \
	  if (CONDITION) { \
		uint16_t diff = __gb_read(gb, gb->cpu_reg.pc++); \
		diff |= __gb_read(gb, gb->cpu_reg.pc++) << 8; \
		__gb_write(gb, --gb->cpu_reg.sp, gb->cpu_reg.pc >> 8); \
		__gb_write(gb, --gb->cpu_reg.sp, gb->cpu_reg.pc & 0xFF); \
		gb->cpu_reg.pc = diff; \
		*inst_cycles += 12; \
	} else \
		gb->cpu_reg.pc += 2;)

DEFINE_CONDITIONAL_INSTRUCTION_Z80(CALL);

DEFINE_INSTRUCTION_Z80(CALL,
    uint16_t diff = __gb_read(gb, gb->cpu_reg.pc++);
	  diff |= __gb_read(gb, gb->cpu_reg.pc++) << 8;
	  __gb_write(gb, --gb->cpu_reg.sp, gb->cpu_reg.pc >> 8);
	  __gb_write(gb, --gb->cpu_reg.sp, gb->cpu_reg.pc & 0xFF);
	  gb->cpu_reg.pc = diff;)

#define DEFINE_RET_INSTRUCTION_Z80(CONDITION_NAME, CONDITION) \
	DEFINE_CONDITION_INSTRUCTION_Z80(RET ## CONDITION_NAME, \
	if (CONDITION) { \
		gb->cpu_reg.pc = __gb_read(gb, gb->cpu_reg.sp++); \
		gb->cpu_reg.pc |= __gb_read(gb, gb->cpu_reg.sp++) << 8; \
        *inst_cycles += 12;})

DEFINE_CONDITIONAL_INSTRUCTION_Z80(RET);

DEFINE_INSTRUCTION_Z80(RET,
	  gb->cpu_reg.pc = __gb_read(gb, gb->cpu_reg.sp++);
    gb->cpu_reg.pc |= __gb_read(gb, gb->cpu_reg.sp++) << 8;)

DEFINE_INSTRUCTION_Z80(RETI,
    gb->cpu_reg.pc = __gb_read(gb, gb->cpu_reg.sp++);
    gb->cpu_reg.pc |= __gb_read(gb, gb->cpu_reg.sp++) << 8;
    gb->gb_ime = 1;)

#define DEFINE_AND_INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(AND ## NAME, \
		gb->cpu_reg.a &= OPERAND; \
		gb->cpu_reg.f_bits.z = !gb->cpu_reg.a; \
		gb->cpu_reg.f_bits.n = 0; \
		gb->cpu_reg.f_bits.c = 0; \
		gb->cpu_reg.f_bits.h = 1;)

#define DEFINE_XOR_INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(XOR ## NAME, \
		gb->cpu_reg.a ^= OPERAND; \
		gb->cpu_reg.f_bits.z = !gb->cpu_reg.a; \
		gb->cpu_reg.f_bits.n = 0; \
		gb->cpu_reg.f_bits.c = 0; \
		gb->cpu_reg.f_bits.h = 0;)

#define DEFINE_OR_INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(OR ## NAME, \
		gb->cpu_reg.a |= OPERAND; \
		gb->cpu_reg.f_bits.z = !gb->cpu_reg.a; \
		gb->cpu_reg.f_bits.n = 0; \
		gb->cpu_reg.f_bits.c = 0; \
		gb->cpu_reg.f_bits.h = 0;)

#define DEFINE_CP_INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(CP ## NAME, \
		int diff = gb->cpu_reg.a - OPERAND; \
		gb->cpu_reg.f_bits.n = 1; \
		gb->cpu_reg.f_bits.z = !(diff & 0xFF); \
		gb->cpu_reg.f_bits.h = (gb->cpu_reg.a & 0xF) - (OPERAND & 0xF) < 0; \
		gb->cpu_reg.f_bits.c = diff < 0;)

#define DEFINE_LDB__INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(LDB_ ## NAME, \
		gb->cpu_reg.b = OPERAND;)

#define DEFINE_LDC__INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(LDC_ ## NAME, \
		gb->cpu_reg.c = OPERAND;)

#define DEFINE_LDD__INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(LDD_ ## NAME, \
		gb->cpu_reg.d = OPERAND;)

#define DEFINE_LDE__INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(LDE_ ## NAME, \
		gb->cpu_reg.e = OPERAND;)

#define DEFINE_LDH__INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(LDH_ ## NAME, \
		gb->cpu_reg.h = OPERAND;)

#define DEFINE_LDL__INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(LDL_ ## NAME, \
		gb->cpu_reg.l = OPERAND;)

#define DEFINE_LDHL__INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(LDHL_ ## NAME, \
		__gb_write(gb, gb->cpu_reg.hl, OPERAND);)

#define DEFINE_LDA__INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(LDA_ ## NAME, \
		gb->cpu_reg.a = OPERAND;)

#define DEFINE_ALU_INSTRUCTION_Z80_NOHL(NAME) \
	DEFINE_ ## NAME ## _INSTRUCTION_Z80(A, gb->cpu_reg.a); \
	DEFINE_ ## NAME ## _INSTRUCTION_Z80(B, gb->cpu_reg.b); \
	DEFINE_ ## NAME ## _INSTRUCTION_Z80(C, gb->cpu_reg.c); \
	DEFINE_ ## NAME ## _INSTRUCTION_Z80(D, gb->cpu_reg.d); \
	DEFINE_ ## NAME ## _INSTRUCTION_Z80(E, gb->cpu_reg.e); \
	DEFINE_ ## NAME ## _INSTRUCTION_Z80(H, gb->cpu_reg.h); \
	DEFINE_ ## NAME ## _INSTRUCTION_Z80(L, gb->cpu_reg.l);

#define DEFINE_ALU_INSTRUCTION_Z80_MEM(NAME, REG) \
	DEFINE_ ## NAME ## _INSTRUCTION_Z80(REG, Z80Read ## REG (gb))

#define DEFINE_ALU_INSTRUCTION_Z80(NAME) \
	DEFINE_ ## NAME ## _INSTRUCTION_Z80(Imm, __gb_read(gb, gb->cpu_reg.pc++)); \
	DEFINE_ALU_INSTRUCTION_Z80_MEM(NAME, HL) \
	DEFINE_ALU_INSTRUCTION_Z80_NOHL(NAME)

DEFINE_ALU_INSTRUCTION_Z80(AND);
DEFINE_ALU_INSTRUCTION_Z80(XOR);
DEFINE_ALU_INSTRUCTION_Z80(OR);
DEFINE_ALU_INSTRUCTION_Z80(CP);

#define DEFINE_ADD_INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(ADD ## NAME, \
		int diff = gb->cpu_reg.a + OPERAND; \
		gb->cpu_reg.f_bits.n = 0; \
		gb->cpu_reg.f_bits.h = (gb->cpu_reg.a & 0xF) + (OPERAND & 0xF) >= 0x10; \
		gb->cpu_reg.f_bits.c = diff >= 0x100; \
		gb->cpu_reg.a = diff; \
		gb->cpu_reg.f_bits.z = !gb->cpu_reg.a;)

#define DEFINE_ADC_INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(ADC ## NAME, \
		int diff = gb->cpu_reg.a + OPERAND + gb->cpu_reg.f_bits.c; \
		gb->cpu_reg.f_bits.n = 0; \
		gb->cpu_reg.f_bits.h = (gb->cpu_reg.a & 0xF) + (OPERAND & 0xF) + gb->cpu_reg.f_bits.c >= 0x10; \
		gb->cpu_reg.f_bits.c = diff >= 0x100; \
		gb->cpu_reg.a = diff; \
		gb->cpu_reg.f_bits.z = !gb->cpu_reg.a;)

#define DEFINE_SUB_INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(SUB ## NAME, \
		int diff = gb->cpu_reg.a - OPERAND; \
		gb->cpu_reg.f_bits.n = 1; \
		gb->cpu_reg.f_bits.h = (gb->cpu_reg.a & 0xF) - (OPERAND & 0xF) < 0; \
		gb->cpu_reg.f_bits.c = diff < 0; \
		gb->cpu_reg.a = diff; \
		gb->cpu_reg.f_bits.z = !gb->cpu_reg.a;)

#define DEFINE_SBC_INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(SBC ## NAME, \
		int diff = gb->cpu_reg.a - OPERAND - gb->cpu_reg.f_bits.c; \
		gb->cpu_reg.f_bits.n = 1; \
		gb->cpu_reg.f_bits.h = (gb->cpu_reg.a & 0xF) - (OPERAND & 0xF) - gb->cpu_reg.f_bits.c < 0; \
		gb->cpu_reg.f_bits.c = diff < 0; \
		gb->cpu_reg.a = diff; \
		gb->cpu_reg.f_bits.z = !gb->cpu_reg.a;)

DEFINE_ALU_INSTRUCTION_Z80(LDB_);
DEFINE_ALU_INSTRUCTION_Z80(LDC_);
DEFINE_ALU_INSTRUCTION_Z80(LDD_);
DEFINE_ALU_INSTRUCTION_Z80(LDE_);
DEFINE_ALU_INSTRUCTION_Z80(LDH_);
DEFINE_ALU_INSTRUCTION_Z80(LDL_);
DEFINE_ALU_INSTRUCTION_Z80_NOHL(LDHL_);
DEFINE_ALU_INSTRUCTION_Z80(LDA_);
DEFINE_ALU_INSTRUCTION_Z80_MEM(LDA_, BC);
DEFINE_ALU_INSTRUCTION_Z80_MEM(LDA_, DE);

DEFINE_INSTRUCTION_Z80(LDBC_A, // LD_MEM_BC_REG_A
	Z80WriteBC(gb, gb->cpu_reg.a);)

DEFINE_INSTRUCTION_Z80(LDDE_A, // LD_MEM_DE_REG_A
	Z80WriteDE(gb, gb->cpu_reg.a);)

DEFINE_INSTRUCTION_Z80(LDImm_A, // LD_MEM_8_REG_A
	__gb_write(gb, 0xFF00 | __gb_read(gb, gb->cpu_reg.pc++), gb->cpu_reg.a);)

DEFINE_INSTRUCTION_Z80(LDImmC_A, // LD_MEM_C_REG_A
	__gb_write(gb, 0xFF00 | gb->cpu_reg.c, gb->cpu_reg.a);)

DEFINE_INSTRUCTION_Z80(LDImm16_A, // LD_MEM_16_REG_A
	uint16_t diff = __gb_read(gb, gb->cpu_reg.pc++);
    	diff |= __gb_read(gb, gb->cpu_reg.pc++) << 8;
    	__gb_write(gb, diff, gb->cpu_reg.a);)

DEFINE_INSTRUCTION_Z80(LDA_Imm16, // LD_REG_A_MEM_16
	uint16_t diff = __gb_read(gb, gb->cpu_reg.pc++);
	diff |= __gb_read(gb, gb->cpu_reg.pc++) << 8;
	gb->cpu_reg.a = __gb_read(gb, diff);)

DEFINE_INSTRUCTION_Z80(LDA_Mem, // LD_REG_A_MEM_8
	gb->cpu_reg.a = __gb_read(gb, 0xFF00 | __gb_read(gb, gb->cpu_reg.pc++));)

DEFINE_INSTRUCTION_Z80(LDA_ImmC, // LD_REG_A_MEM_C
	gb->cpu_reg.a = __gb_read(gb, 0xFF00 | gb->cpu_reg.c);)

DEFINE_INSTRUCTION_Z80(LDA_DEC, // LD_REG_A_MEM_DEC_HL
	gb->cpu_reg.a = __gb_read(gb, gb->cpu_reg.hl--);)

DEFINE_INSTRUCTION_Z80(LDA_INC, // LD_REG_A_MEM_INC_HL
	gb->cpu_reg.a = __gb_read(gb, gb->cpu_reg.hl++);)

DEFINE_INSTRUCTION_Z80(LDHL_Imm,
	Z80WriteHL(gb, __gb_read(gb, gb->cpu_reg.pc++));)

DEFINE_INSTRUCTION_Z80(LDHL_SPImm, // LD16_REG_HL_MEM_8
	int diff = (int8_t) __gb_read(gb, gb->cpu_reg.pc++);
	gb->cpu_reg.hl = gb->cpu_reg.sp + diff;
	gb->cpu_reg.f_bits.z = 0;
	gb->cpu_reg.f_bits.n = 0;
	gb->cpu_reg.f_bits.c = (diff & 0xFF) + (gb->cpu_reg.sp & 0xFF) >= 0x100;
	gb->cpu_reg.f_bits.h = (diff & 0xF) + (gb->cpu_reg.sp & 0xF) >= 0x10;)

DEFINE_INSTRUCTION_Z80(LDHL_Imm16, // LD16_REG_HL_IMM16
	gb->cpu_reg.l = __gb_read(gb, gb->cpu_reg.pc++);
	gb->cpu_reg.h = __gb_read(gb, gb->cpu_reg.pc++);)

DEFINE_INSTRUCTION_Z80(LDINC_HL_A, // LD_MEM_INC_HL_REG_A
	Z80WriteHL(gb, gb->cpu_reg.a);
	gb->cpu_reg.hl++;)

DEFINE_INSTRUCTION_Z80(LDDEC_HL_A, // LD_MEM_DEC_HL_REG_A
	Z80WriteHL(gb, gb->cpu_reg.a);
	gb->cpu_reg.hl--;)

DEFINE_INSTRUCTION_Z80(LDSP_HL, // LD16_REG_SP_REG_HL
	gb->cpu_reg.sp = Z80ReadHL(gb);)

DEFINE_INSTRUCTION_Z80(LDSP_Imm16, // LD16_REG_SP_IMM16
	gb->cpu_reg.sp = __gb_read(gb, gb->cpu_reg.pc++);
	gb->cpu_reg.sp |= __gb_read(gb, gb->cpu_reg.pc++) << 8;)

DEFINE_INSTRUCTION_Z80(LDBC_Imm16, // LD16_REG_BC_IMM16
	gb->cpu_reg.c = __gb_read(gb, gb->cpu_reg.pc++);
	gb->cpu_reg.b = __gb_read(gb, gb->cpu_reg.pc++);)

DEFINE_INSTRUCTION_Z80(LDDE_Imm16, // LD16_REG_DE_IMM16
	gb->cpu_reg.e = __gb_read(gb, gb->cpu_reg.pc++);
	gb->cpu_reg.d = __gb_read(gb, gb->cpu_reg.pc++);)

DEFINE_INSTRUCTION_Z80(LDImm16_SP, // LD16_MEM_16_REG_SP
	uint16_t diff = __gb_read(gb, gb->cpu_reg.pc++);
	diff |= __gb_read(gb, gb->cpu_reg.pc++) << 8;
	__gb_write(gb, diff++, gb->cpu_reg.sp & 0xFF);
	__gb_write(gb, diff, gb->cpu_reg.sp >> 8);)

DEFINE_ALU_INSTRUCTION_Z80(ADD);
DEFINE_ALU_INSTRUCTION_Z80(ADC);
DEFINE_ALU_INSTRUCTION_Z80(SUB);
DEFINE_ALU_INSTRUCTION_Z80(SBC);

DEFINE_INSTRUCTION_Z80(ADDSP_Imm,
	int diff = (int8_t) __gb_read(gb, gb->cpu_reg.pc++);
    gb->cpu_reg.f_bits.z = 0;
    gb->cpu_reg.f_bits.n = 0;
    gb->cpu_reg.f_bits.c = (diff & 0xFF) + (gb->cpu_reg.sp & 0xFF) >= 0x100;
    gb->cpu_reg.f_bits.h = (diff & 0xF) + (gb->cpu_reg.sp & 0xF) >= 0x10;
    gb->cpu_reg.sp += diff;)

#define DEFINE_ADD_HL_INSTRUCTION_Z80(REG, OPERAND) \
	DEFINE_INSTRUCTION_Z80(ADDHL_ ## REG, \
		uint_fast32_t diff = gb->cpu_reg.hl + OPERAND; \
		gb->cpu_reg.f_bits.n = 0; \
		gb->cpu_reg.f_bits.h = (diff ^ gb->cpu_reg.hl ^ OPERAND) & 0x1000 ? 1 : 0; \
        gb->cpu_reg.f_bits.c = (diff & 0xFFFF0000) ? 1 : 0; \
        gb->cpu_reg.hl = (diff & 0x0000FFFF);)

DEFINE_ADD_HL_INSTRUCTION_Z80(BC, gb->cpu_reg.bc);
DEFINE_ADD_HL_INSTRUCTION_Z80(DE, gb->cpu_reg.de);
DEFINE_ADD_HL_INSTRUCTION_Z80(HL, gb->cpu_reg.hl);
DEFINE_INSTRUCTION_Z80(ADDHL_SP, \
		uint_fast32_t diff = gb->cpu_reg.hl + gb->cpu_reg.sp; \
        gb->cpu_reg.f_bits.n = 0; \
        gb->cpu_reg.f_bits.h = ((gb->cpu_reg.hl & 0xFFF) + (gb->cpu_reg.sp & 0xFFF)) & 0x1000 ? 1 : 0; \
        gb->cpu_reg.f_bits.c = diff & 0x10000 ? 1 : 0; \
        gb->cpu_reg.hl = (uint16_t) diff;)

#define DEFINE_INC_INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(INC ## NAME, \
		int diff = OPERAND + 1; \
		gb->cpu_reg.f_bits.h = (OPERAND & 0xF) == 0xF; \
		OPERAND = diff; \
		gb->cpu_reg.f_bits.n = 0; \
		gb->cpu_reg.f_bits.z = !OPERAND;)

#define DEFINE_DEC_INSTRUCTION_Z80(NAME, OPERAND) \
	DEFINE_INSTRUCTION_Z80(DEC ## NAME, \
		int diff = OPERAND - 1; \
		gb->cpu_reg.f_bits.h = (OPERAND & 0xF) == 0x0; \
		OPERAND = diff; \
		gb->cpu_reg.f_bits.n = 1; \
		gb->cpu_reg.f_bits.z = !OPERAND;)

DEFINE_ALU_INSTRUCTION_Z80_NOHL(INC);
DEFINE_ALU_INSTRUCTION_Z80_NOHL(DEC);
DEFINE_INSTRUCTION_Z80(INC_Mem,
    uint8_t diff = Z80ReadHL(gb) + 1;
    gb->cpu_reg.f_bits.z = !diff;
    gb->cpu_reg.f_bits.n = 0;
    gb->cpu_reg.f_bits.h = ((diff & 0x0F) == 0x00);
    Z80WriteHL(gb, diff);)
DEFINE_INSTRUCTION_Z80(DEC_Mem,
    uint8_t diff = Z80ReadHL(gb) - 1;
    gb->cpu_reg.f_bits.z = !diff;
    gb->cpu_reg.f_bits.n = 1;
    gb->cpu_reg.f_bits.h = ((diff & 0x0F) == 0x0F);
    Z80WriteHL(gb, diff);)

DEFINE_INSTRUCTION_Z80(INC_BC,
	gb->cpu_reg.bc++;)
DEFINE_INSTRUCTION_Z80(INC_DE,
	gb->cpu_reg.de++;)
DEFINE_INSTRUCTION_Z80(INC_HL,
	gb->cpu_reg.hl++;)
DEFINE_INSTRUCTION_Z80(INC_SP,
	gb->cpu_reg.sp++;)

DEFINE_INSTRUCTION_Z80(DEC_BC,
	gb->cpu_reg.bc--;)
DEFINE_INSTRUCTION_Z80(DEC_DE,
	gb->cpu_reg.de--;)
DEFINE_INSTRUCTION_Z80(DEC_HL,
	gb->cpu_reg.hl--;)
DEFINE_INSTRUCTION_Z80(DEC_SP,
	gb->cpu_reg.sp--;)

DEFINE_INSTRUCTION_Z80(SCF,
	gb->cpu_reg.f_bits.c = 1;
	gb->cpu_reg.f_bits.h = 0;
	gb->cpu_reg.f_bits.n = 0;)

DEFINE_INSTRUCTION_Z80(CCF,
	gb->cpu_reg.f_bits.c ^= 1;
	gb->cpu_reg.f_bits.h = 0;
	gb->cpu_reg.f_bits.n = 0;)

DEFINE_INSTRUCTION_Z80(CPL_,
	gb->cpu_reg.a ^= 0xFF;
	gb->cpu_reg.f_bits.h = 1;
	gb->cpu_reg.f_bits.n = 1;)

DEFINE_INSTRUCTION_Z80(DAA,
	if (gb->cpu_reg.f_bits.n) {
		if (gb->cpu_reg.f_bits.h) {
			gb->cpu_reg.a += 0xFA;
		}
		if (gb->cpu_reg.f_bits.c) {
			gb->cpu_reg.a += 0xA0;
		}
	} else {
		int a = gb->cpu_reg.a;
		if ((gb->cpu_reg.a & 0xF) > 0x9 || gb->cpu_reg.f_bits.h) {
			a += 0x6;
		}
		if ((a & 0x1F0) > 0x90 || gb->cpu_reg.f_bits.c) {
			a += 0x60;
			gb->cpu_reg.f_bits.c = 1;
		} else {
			gb->cpu_reg.f_bits.c = 0;
		}
		gb->cpu_reg.a = a;
	}
	gb->cpu_reg.f_bits.h = 0;
	gb->cpu_reg.f_bits.z = !gb->cpu_reg.a;)

#define DEFINE_POPPUSH_INSTRUCTION_Z80(REG, H, L) \
	DEFINE_INSTRUCTION_Z80(POP ## REG, \
		gb->cpu_reg. ## L = __gb_read(gb, gb->cpu_reg.sp++); \
		gb->cpu_reg. ## H = __gb_read(gb, gb->cpu_reg.sp++);) \
	DEFINE_INSTRUCTION_Z80(PUSH ## REG, \
		__gb_write(gb, --gb->cpu_reg.sp, gb->cpu_reg. ## H); \
		__gb_write(gb, --gb->cpu_reg.sp, gb->cpu_reg. ## L);)

DEFINE_POPPUSH_INSTRUCTION_Z80(BC, b, c);
DEFINE_POPPUSH_INSTRUCTION_Z80(DE, d, e);
DEFINE_POPPUSH_INSTRUCTION_Z80(HL, h, l);
DEFINE_INSTRUCTION_Z80(POPAF,
	  uint8_t diff = __gb_read(gb, gb->cpu_reg.sp++);
	  gb->cpu_reg.f_bits.z = (diff >> 7) & 1;
	  gb->cpu_reg.f_bits.n = (diff >> 6) & 1;
	  gb->cpu_reg.f_bits.h = (diff >> 5) & 1;
	  gb->cpu_reg.f_bits.c = (diff >> 4) & 1;
	  gb->cpu_reg.a = __gb_read(gb, gb->cpu_reg.sp++);)
DEFINE_INSTRUCTION_Z80(PUSHAF,
	  __gb_write(gb, --gb->cpu_reg.sp, gb->cpu_reg.a);
	  __gb_write(gb, --gb->cpu_reg.sp,
               gb->cpu_reg.f_bits.z << 7 | gb->cpu_reg.f_bits.n << 6 |
               gb->cpu_reg.f_bits.h << 5 | gb->cpu_reg.f_bits.c << 4);)

DEFINE_INSTRUCTION_Z80(RLCA,
    gb->cpu_reg.a = (gb->cpu_reg.a << 1) | (gb->cpu_reg.a >> 7);
    gb->cpu_reg.f_bits.z = 0;
    gb->cpu_reg.f_bits.h = 0;
    gb->cpu_reg.f_bits.n = 0;
    gb->cpu_reg.f_bits.c = gb->cpu_reg.a & 1;)

DEFINE_INSTRUCTION_Z80(RLA,
    int wide = (gb->cpu_reg.a << 1) | gb->cpu_reg.f_bits.c;
    gb->cpu_reg.a = wide;
    gb->cpu_reg.f_bits.z = 0;
    gb->cpu_reg.f_bits.h = 0;
    gb->cpu_reg.f_bits.n = 0;
    gb->cpu_reg.f_bits.c = wide >> 8;)

DEFINE_INSTRUCTION_Z80(RRCA, 
    int low = gb->cpu_reg.a & 1;
    gb->cpu_reg.a = (gb->cpu_reg.a >> 1) | (low << 7);
    gb->cpu_reg.f_bits.z = 0;
    gb->cpu_reg.f_bits.h = 0;
    gb->cpu_reg.f_bits.n = 0;
    gb->cpu_reg.f_bits.c = low;)

DEFINE_INSTRUCTION_Z80(RRA,
    int low = gb->cpu_reg.a & 1;
    gb->cpu_reg.a = (gb->cpu_reg.a >> 1) | (gb->cpu_reg.f_bits.c << 7);
    gb->cpu_reg.f_bits.z = 0;
    gb->cpu_reg.f_bits.h = 0;
    gb->cpu_reg.f_bits.n = 0;
    gb->cpu_reg.f_bits.c = low;)

DEFINE_INSTRUCTION_Z80(DI, gb->gb_ime = 0;);
DEFINE_INSTRUCTION_Z80(EI, gb->gb_ime = 1;);
DEFINE_INSTRUCTION_Z80(HALT, /* gb->gb_halt = 1; */);

#define DEFINE_RST_INSTRUCTION_Z80(VEC)                          \
    DEFINE_INSTRUCTION_Z80(RST ## VEC,                           \ 
        __gb_write(gb, --gb->cpu_reg.sp, gb->cpu_reg.pc >> 8);   \
        __gb_write(gb, --gb->cpu_reg.sp, gb->cpu_reg.pc & 0xFF); \
        gb->cpu_reg.pc = 0x00 ## VEC;)

DEFINE_RST_INSTRUCTION_Z80(00);
DEFINE_RST_INSTRUCTION_Z80(08);
DEFINE_RST_INSTRUCTION_Z80(10);
DEFINE_RST_INSTRUCTION_Z80(18);
DEFINE_RST_INSTRUCTION_Z80(20);
DEFINE_RST_INSTRUCTION_Z80(28);
DEFINE_RST_INSTRUCTION_Z80(30);
DEFINE_RST_INSTRUCTION_Z80(38);

DEFINE_INSTRUCTION_Z80(STOP, /* gb->gb_halt = 1; */);

/* clang-format on */
