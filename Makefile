CC=x86_64-elf-gcc -m32			# opt needed since this gcc compiles elf64bit by default
CFLAGS=
VPATH=boot-sector:kernel:driver	# expands searching area for src files

ASMB=nasm
LD86=x86_64-elf-ld -m elf_i386	# opt needed since this binutils links to 64bit output by default
QEMU86=qemu-system-x86_64

SRC=$(wildcard kernel/*.c driver/*.c)
INC=$(wildcard kernel/*.h driver/*.h)
OBJ=$(SRC:.c=.o kernel/interrupt.o)
ASMINC=-Iboot-sector

# Output Coloring 
# (might need to add -e opt for echo)
# (having other output highlighted in red)
RED=\033[0;31m
CYAN=\033[1;36m
NC=\033[0m

all: run clean

run: os-image.bin
	@echo "$(CYAN)Running QEMU with $<$(RED)"
#	Big issue with loading as drive, our disk loader wont load more than 3 sectors ?
#	no problem to load at least 15 sectors as floppy
#	@$(QEMU86) -drive format=raw,file=$(addprefix bin/, $<)
#	use this for floppy instead of -fda:
	@$(QEMU86) -drive format=raw,index=0,if=floppy,file=$(addprefix bin/, $<)

# Kernel Image Building
os-image.bin: bs_main.bin kernel.bin
	@echo "$(NC)[LOG] Building Kernel Image with: $^$(RED)"
	@cat $(addprefix bin/, $^) > $(addprefix bin/, $@)

# Kernel Linking
#	NOTE Currently setting kernel entry to 0x1000 since the linker will default the 
#	program entry point to there if it doesn't find a labeled start, look into 
#	labeling kernel with _start symbol possibly.
kernel.bin: kernel/kernel_entry.o $(OBJ)
	@echo "$(NC)[LOG] Linking $@ with: $^$(RED)"
	@$(LD86) -Ttext 0x1000 $^ --oformat binary -o $(addprefix bin/, $@)
 
%.o: %.c $(INC)
	@echo "$(NC)[LOG] Compiling $@$(RED)"
	@$(CC) -ffreestanding -c $< -o $@

%.o: %.asm
	@echo "$(NC)[LOG] Assembling $@$(RED)"
	@$(ASMB) $< -f elf -o $@

# Currently only for bootsector assembly
%.bin: %.asm
	@echo "$(NC)[LOG] Assembling $@$(RED)"
	@$(ASMB) $< -f bin $(ASMINC) -o $(addprefix bin/, $@)

# Cleanup
.PHONY: clean
clean:
	@echo "$(NC)Cleaning .bin .o files$(RED)"
	@rm -fr bin/*.bin 
	@rm -fr kernel/*.o driver/*.o