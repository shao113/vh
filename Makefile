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

# Note: cc1_v263 compiled with CFLAGS="-std=gnu89 -m32 -DBYTES_BIG_ENDIAN=0 -DTARGET_MEM_FUNCTIONS"
CC		     	:= cc1_v263_EL

SPLAT           := $(PYTHON) $(TOOLS_DIR)/splat/split.py
SORT_SYM        := $(PYTHON) $(TOOLS_DIR)/sortSymbols.py > symbol_addrs.$(BASENAME).txt

AS_FLAGS        := -march=r3000 -mtune=r3000 -Iinclude -G0
D_FLAGS       	:= -Dmips -D__GNUC__=2 -D__OPTIMIZE__ -D__mips__ -D__mips -Dpsx -D__psx__ -D__psx -D_PSYQ -D__EXTENSIONS__ -D_MIPSEL -D__CHAR_UNSIGNED__ -D_LANGUAGE_C -DLANGUAGE_C

OPT_FLAGS       := -O1
GP_OPT          := -G0

# Update: Removed -fno-builtin to enable generation of inline strcpy
CC_FLAGS        = $(GP_OPT) $(OPT_FLAGS) -Wall -mgas -mgpopt -msoft-float -fshort-enums -ffunction-cse -fpcc-struct-return -fgnu-linker


CPP_FLAGS       := -undef -Wall -lang-c $(D_FLAGS) -Iinclude -Iinclude/PsyQ -nostdinc
OBJCOPY_FLAGS   := -O binary
LD_FLAGS    	:= --cref -Map build/$(BASENAME).map -T $(BASENAME).ld -T undefined_syms_auto.$(BASENAME).txt -T undefined_funcs_auto.$(BASENAME).txt --no-check-sections



build/src/audio.c.s: CC := cc1_v258_messyhack2
build/src/audio.c.s: OPT_FLAGS := -O2

build/src/card.c.s: GP_OPT := -G8


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
	@if [[ "$$(grep -Fi nonmatchings '$<')" ]]; then \
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
