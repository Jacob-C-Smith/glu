FINDGOAL=$(findstring $(1),$(MAKECMDGOALS))
CFLAGS:=$(CFLAGS) -m32 -std=gnu99 -ffreestanding -Wall -Wextra
LDFLAGS:=$(LDFLAGS) -m32 -T scripts/linker.ld -ffreestanding -nostdlib

ifeq ($(call FINDGOAL,rel),rel)
	OBJDIR:=o/rel
	CFLAGS+=-O2
else
	OBJDIR:=o/dbg
	CFLAGS+=-O0 -ggdb
endif
.DEFAULT_GOAL: dbg

TARGET:=i686-elf
SRCDIR:=src
ISODIR:=isodir
OUTFILE:=$(OBJDIR)/glu.iso

PREFIX?=$(HOME)/opt/cross/bin
AS:=$(PREFIX)/$(TARGET)-as
CC:=$(PREFIX)/$(TARGET)-gcc

SRC:=$(shell find $(SRCDIR) -name "*.[c|h]")
ASM:=$(shell find $(SRCDIR) -name "*.s")
GETOBJS=$(patsubst %.$(2),$(OBJDIR)/%.o,$(filter %.$(2),$(1)))
OBJS:=$(call GETOBJS,$(SRC),c) $(call GETOBJS,$(ASM),s)

dbg: $(OUTFILE)
rel: $(OUTFILE)
run: $(OUTFILE)
	qemu-system-i386 -s -S -daemonize -cdrom $<
	if which gdb; then \
		gdb -ex "target remote localhost:1234" $<; \
	else \
		lldb --one-line "gdb-remote localhost:1234" $<; \
	fi

$(OUTFILE): $(OBJS)
	mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $(ISODIR)/boot/glu.bin $^
	grub-mkrescue -o $@ $(ISODIR)

# $(1) c source file
define MAKE_COBJ
$(OBJDIR)/$(dir $(1))$(shell gcc $(CFLAGS) -M $(1) | tr -d '\\\n')
	mkdir -p $$(dir $$@)
	$(CC) -c $$< -o $$@ $(CFLAGS)
endef

$(foreach src,$(SRC),$(eval $(call MAKE_COBJ,$(src))))

# $(1) assembly source file
define MAKE_ASMOBJ
$(call GETOBJS,$(1),s): $(1)
	mkdir -p $$(dir $$@)
	$(AS) $< -o $@
endef

$(foreach asm,$(ASM),$(eval $(call MAKE_ASMOBJ,$(asm))))

.PHONY: clean
clean:
	rm -rf o/ zig-out/ .zig-cache/
