#define ENABLE_SOUND 0
#define ENABLE_LCD 0
#include "cpu.h"
#include "gameboy.h"

#if defined(__linux__)
#include "prof.h"
#elif defined(__MACH__)
#include <mach/mach_time.h>
#include <math.h>
#define PROF_START(...)                                    \
    static mach_timebase_info_data_t timebase;             \
    mach_timebase_info(&timebase);                         \
    double conversion_factor =                             \
        (double) timebase.numer / (double) timebase.denom; \
    uint64_t start = mach_absolute_time()
#define PROF_STDOUT(...)                                                 \
    uint64_t elapsed = mach_absolute_time() - start;                     \
    double duration = (double) elapsed * conversion_factor / __exp10(9); \
    printf("elapsed time: %lf s\n", duration)
#else
#error "Unsupported platform"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lib/tester.h"

struct gb_s gb;
static size_t instruction_mem_size;
static uint8_t *instruction_mem;

static int num_mem_accesses;
static struct mem_access mem_accesses[16];

/* Return byte from Blarrg test ROM, written by Shay Green <gblargg@gmail.com>
 * https://github.com/retrio/gb-test-roms/tree/master/cpu_instrs
 */
uint8_t gb_rom_read(struct gb_s *gb, const uint_fast32_t addr)
{
    /* Import the test ROM */
#include "build/cpu_instrs.h"
    (void) gb;
    return cpu_instrs_gb[addr];
}

/* Ignore cart RAM writes */
void gb_cart_ram_write(struct gb_s *gb,
                       const uint_fast32_t addr,
                       const uint8_t val)
{
    (void) gb;
    struct mem_access *access = &mem_accesses[num_mem_accesses++];
    access->type = MEM_ACCESS_WRITE;
    access->addr = addr;
    access->val = val;
    return;
}

/* Ignore cart RAM reads */
uint8_t gb_cart_ram_read(struct gb_s *gb, const uint_fast32_t addr)
{
    (void) gb;
    if (addr < instruction_mem_size)
        return instruction_mem[addr];
    else
        return 0xaa;
}

/* Ignore all errors */
void gb_error(struct gb_s *gb, const gb_error_t gb_err, const uint16_t val)
{
    (void) gb;
    (void) gb_err;
    (void) val;
    return;
}

static struct tester_flags flags = {
    .keep_going_on_mismatch = 0,
    .enable_cb_instruction_testing = 1,
    .print_tested_instruction = 0,
    .print_verbose_inputs = 0,
};

static void mycpu_init(size_t tester_instruction_mem_size,
                       uint8_t *tester_instruction_mem)
{
    instruction_mem_size = tester_instruction_mem_size;
    instruction_mem = tester_instruction_mem;
    /* ... Initialize your CPU here ... */
    gb_init(&gb, &gb_rom_read, &gb_cart_ram_read, &gb_cart_ram_write, &gb_error,
            NULL);
}

/*
 *  * Resets the CPU state (e.g., registers) to a given state state.
 *   */
static void mycpu_set_state(struct state *state)
{
    gb.cpu_reg.a = state->reg8.A;
    gb.cpu_reg.f = state->reg8.F;
    gb.cpu_reg.bc = state->reg16.BC;
    gb.cpu_reg.de = state->reg16.DE;
    gb.cpu_reg.hl = state->reg16.HL;
    gb.cpu_reg.sp = state->SP;
    gb.cpu_reg.pc = state->PC;
    gb.gb_halt = state->halted;
    gb.gb_ime = state->interrupts_master_enabled;
    /* ... Load your CPU with state as described (e.g., registers) ... */
}

/*
 *  * Query the current state of the CPU.
 *   */
static void mycpu_get_state(struct state *state)
{
    state->num_mem_accesses = num_mem_accesses;
    memcpy(state->mem_accesses, mem_accesses, sizeof(mem_accesses));
    state->reg8.A = gb.cpu_reg.a;
    state->reg8.F = gb.cpu_reg.f;
    state->reg16.BC = gb.cpu_reg.bc;
    state->reg16.DE = gb.cpu_reg.de;
    state->reg16.HL = gb.cpu_reg.hl;
    state->SP = gb.cpu_reg.sp;
    state->PC = gb.cpu_reg.pc;
    state->halted = gb.gb_halt;
    state->interrupts_master_enabled = gb.gb_ime;
    num_mem_accesses = 0;
    /* ... Copy your current CPU state into the provided struct ... */
}

/*
 *  * Step a single instruction of the CPU. Returns the amount of cycles this
 * took
 *   * (e.g., NOP should return 4).
 *    */
static int mycpu_step(void)
{
    int cycles = 0;

    /* ... Execute a single instruction in your CPU ... */
    cycles = cpu_step(&gb);
    return cycles;
}

struct tester_operations myops = {
    .init = mycpu_init,
    .set_state = mycpu_set_state,
    .get_state = mycpu_get_state,
    .step = mycpu_step,
};

int main(void)
{
    PROF_START();
    tester_run(&flags, &myops);
    PROF_STDOUT();
    return 0;
}
