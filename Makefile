BASENAME := vandalhearts

TOOLS_DIR       := ~/stuff

ASM_DIRS    	:= asm asm/data
C_DIRS      	:= src
BIN_DIRS    	:= assets

TARGET    		:= build/$(BASENAME)

S_FILES   		:= $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s))
C_FILES     	:= $(foreach dir,$(C_DIRS),$(wildcard $(dir)/*.c))
BIN_FILES   	:= $(foreach dir,$(BIN_DIRS),$(wildcard $(dir)/*.bin))

O_FILES     	:= $(foreach file,$(S_FILES),build/$(file).o) \
				   $(foreach file,$(C_FILES),build/$(file).o) \
				   $(foreach file,$(BIN_FILES),build/$(file).o)

PYTHON          := python3.10
WINE            := /usr/bin/wine
CPP             := cpp -P
AS              := mips-elf-as -EL
LD              := mips-elf-ld -EL
OBJCOPY         := mips-elf-objcopy
#CC		     	:= $(TOOLS_DIR)/wine-cc.sh
CC		     	:= cc1_v263_EL
SPLAT           := $(PYTHON) $(TOOLS_DIR)/splat/split.py
SORT_SYM        := $(PYTHON) $(TOOLS_DIR)/sortSymbols.py > symbol_addrs.$(BASENAME).txt

AS_FLAGS        := -march=r3000 -mtune=r3000 -Iinclude -G0
D_FLAGS       	:= -Dmips -D__GNUC__=2 -D__OPTIMIZE__ -D__mips__ -D__mips -Dpsx -D__psx__ -D__psx -D_PSYQ -D__EXTENSIONS__ -D_MIPSEL -D__CHAR_UNSIGNED__ -D_LANGUAGE_C -DLANGUAGE_C

OPT_FLAGS       := -O1
CC_FLAGS        = -G0 $(OPT_FLAGS) -Wall -mgas -mgpopt -msoft-float -fshort-enums -fno-builtin -ffunction-cse -fpcc-struct-return -fgnu-linker

CPP_FLAGS       := -undef -Wall -lang-c $(D_FLAGS) -Iinclude -nostdinc
OBJCOPY_FLAGS   := -O binary
LD_FLAGS    	:= --cref -Map build/$(BASENAME).map -T $(BASENAME).ld -T undefined_syms_auto.$(BASENAME).txt -T undefined_funcs_auto.$(BASENAME).txt --no-check-sections




build/src/audio.c.s: CC := cc1_v258_messyhack2
build/src/audio.c.s: OPT_FLAGS := -O2

# For correct byte order in struct assignments:
#build/src/card.c.s: CC := cc1_v272
#build/src/card.c.s: CC_FLAGS += -mel
# Update: Incomplete match with v272; Re-compiling cc1_v263 with -DBYTES_BIG_ENDIAN=0 also seems to work, but breaks
# an earlier (dubious) match; just going to hack around that for a bit while things are in flux;


default: dirs check
	
clean:
	#mv --backup=numbered src/*_asm_auto.c old/backup
	rm -rf batch.tmp asm assets build
	
soft-clean:
	rm -rf batch.tmp build
	
extract:
	$(SORT_SYM)
	$(SPLAT) $(BASENAME).yaml

check: dirs $(TARGET).dat
	md5sum "$(BASENAME).psx" "$(TARGET).dat"

dirs:
	$(foreach dir,$(ASM_DIRS) $(BIN_DIRS) $(C_DIRS),$(shell mkdir -p build/$(dir)))

$(TARGET).dat: $(TARGET).elf
	$(OBJCOPY) $(OBJCOPY_FLAGS) $< $@

$(TARGET).elf: $(O_FILES)
	$(LD) $(LD_FLAGS) -o $@


build/%.s.o: %.s
	$(AS) $(AS_FLAGS) -o $@ $<

build/%.bin.o: %.bin
	$(LD) -r -b binary -o $@ $<


build/%.i: %.c
	$(CPP) $(CPP_FLAGS) -o $@ $<
	
build/%.c.s: build/%.i
	$(CC) $(CC_FLAGS) -o $@ $<
#	$(TOOLS_DIR)/wine-cc.sh $@ $<
	
build/%.c.o: build/%.c.s
#	$(AS) $(AS_FLAGS) -o $@ $<
# Compiler doesn't correctly handle mixing C functions w/ INCLUDE_ASM, so just assuming any files
# containing "nonmatchings" are from splat-generated c and can be handled by gnu assembler

#temp-disable:
	if [[ "$$(grep -Fi nonmatchings '$<')" ]]; then \
		$(AS) $(AS_FLAGS) -o $@ $< ;\
	else \
		$(TOOLS_DIR)/wine-as.sh $@ $< ;\
	fi
	
# Batch assemble src files with a single dosbox launch
batch: $(foreach file,$(C_FILES),build/$(file).s)
	$(TOOLS_DIR)/dosbox-assemble-all.sh $^
	
# Batch assemble (modified only)
batch.tmp: $(foreach file,$(C_FILES),build/$(file).s)
	$(TOOLS_DIR)/dosbox-assemble-all.sh $?
	@touch batch.tmp


.SECONDARY:
.PHONY: clean default extract check batch soft-clean
SHELL = /bin/bash -e -o pipefail
