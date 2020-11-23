CFLAGS = -std=gnu99 -O2 -Wall -Wextra

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

deps := $(OBJS:%.o=%.o.d)

%.o: %.c
	$(VECHO) "  CC\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c -MMD -MF $@.d $<

$(OUT)/emu: $(OBJS)
	$(VECHO) "  LD\t$@\n"
	$(Q)$(CC) -o $@ $^ $(LDFLAGS)

$(OUT)/bench:
	$(VECHO) "  CC+LD\t$@\n"
	$(Q)$(CC) -o $@ -MMD -MF $@.d bench.c

clean:
	$(RM) $(BIN) $(OBJS) $(deps)

-include $(deps)

