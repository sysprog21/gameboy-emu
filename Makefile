CFLAGS = -std=gnu99 -O2 -Wall -Wextra
LDFLAGS = -lm

# SDL
CFLAGS += -D ENABLE_LCD
CFLAGS += -D ENABLE_SOUND
CFLAGS += `sdl2-config --cflags`
LDFLAGS += `sdl2-config --libs`

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
 	sdl.o	

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

$(OUT)/bench: prof.h bench.c gameboy.h
	$(VECHO) "  CC+LD\t$@\n"
	$(Q)$(CC) -o $@ bench.c

# Download Game Boy ROMs with full source
download_rom:
	mkdir -p roms
	wget -O roms/HungryBirds.gb https://github.com/oshf/hungry_birds/blob/master/bin/HungryBirds.gb
	wget -O roms/Snake.gb https://github.com/brovador/GBsnake/blob/master/dist/gbsnake.gb

clean:
	$(RM) $(BIN) $(OBJS) $(deps)
distclean: clean
	$(RM) prof.h
	$(RM) roms

-include $(deps)

