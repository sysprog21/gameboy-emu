CFLAGS = -std=gnu99 -O2 -Wall -Wextra
LDFLAGS = -lm

# SDL
CFLAGS += -D ENABLE_LCD
CFLAGS += -D ENABLE_SOUND
CFLAGS += `sdl2-config --cflags`
LDFLAGS += `sdl2-config --libs`

# Computed-goto
CFLAGS += -fno-gcse -fno-crossjumping

# Control the build verbosity
ifeq ("$(VERBOSE)","1")
    Q :=
    VECHO = @true
else
    Q := @
    VECHO = @printf
endif

OUT ?= build
SHELL_HACK := $(shell mkdir -p $(OUT))

BIN = $(OUT)/emu $(OUT)/bench

all: $(BIN)

OBJS = \
	apu.o \
	cpu.o \
	mmu.o \
	gameboy.o \
	main.o

OBJS := $(addprefix $(OUT)/, $(OBJS))
deps := $(OBJS:%.o=%.o.d)

$(OUT)/%.o: %.c
	$(VECHO) "  CC\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c -MMD -MF $@.d $<

$(OUT)/emu: $(OBJS)
	$(VECHO) "  LD\t$@\n"
	$(Q)$(CC) -o $@ $^ $(LDFLAGS)

# Self-contained C/C++ profiler library for Linux
# https://github.com/cyrus-and/prof
prof.h:
	$(VECHO) "  Downloading the latest $@ ...\n"
	@wget -q https://raw.githubusercontent.com/cyrus-and/prof/master/prof.h

$(OUT)/cpu_instrs.h: tests/cpu_instrs.gb tests/rom2h.c
	$(VECHO) "  Generating C header from Blargg's Gameboy test ROM...\n"
	$(Q)$(CC) -o $(OUT)/rom2h tests/rom2h.c
	@$(OUT)/rom2h

$(OUT)/bench: $(OUT)/cpu_instrs.h prof.h bench.c gameboy.h cpu.c mmu.c
	$(VECHO) "  CC+LD\t$@\n"
	$(Q)$(CC) -DDEBUG -o $@ bench.c gameboy.c cpu.c mmu.c build/apu.o lib/*.c

# Download Game Boy ROMs with full source
download_rom:
	mkdir -p roms
	wget -O roms/FlappyBoy.gb https://github.com/bitnenfer/flappy-boy-asm/blob/master/build/flappyboy.gb?raw=true
	wget -O roms/HungryBirds.gb https://github.com/oshf/hungry_birds/blob/master/bin/HungryBirds.gb?raw=true
	wget -O roms/Snake.gb https://github.com/brovador/GBsnake/blob/master/dist/gbsnake.gb?raw=true
	wget -O roms/Kanoid.gb https://github.com/untoxa/a_kanoid/blob/master/a_kanoid.gb?raw=true
	wget -O roms/Frapball.gb https://github.com/1r3n33/frapball/releases/download/refs%2Fheads%2Fmaster/game.gb

clean:
	$(RM) $(BIN) $(OBJS) $(deps)
distclean: clean
	$(RM) prof.h
	$(RM) -r roms
	$(RM) $(OUT)/rom2h $(OUT)/cpu_instrs.h

-include $(deps)

