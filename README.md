# SimpleOS

A small operating system made for Intel's 386 processor model. The kernel uses a monolithic design. SimpleOS comes with a custom bootloader written in NASM assembly that loads the kernel code (written in a higher-level language - C) and launches it in 32-bit protected mode. SimpleOS was developed using QEMU, but is programmed agnostic of the emulator/VM, only the build system would need adjusting. More detailed doc is on the way!

## Setup

### Emulator

SimpleOS requires any emulator/VM that can target x86 systems 32 OR 64 bit. This is because Intel's x86 models are designed to be backwards compatible, meaning any program written for x86-32 should run without issue on x86-64. The makefile in this repo is written for [QEMU](https://www.qemu.org), and is what I would recommend using. To swap emulator, you just need to change QEMU to your chosen emulator in the makefile.

### Linker

The code for SimpleOS is compiled in [ELF-i386](https://wiki.osdev.org/ELF) and thus must be linked accordingly. A linker that is able to link in a ELF-i386 32-bit format is required to build the OS. I would recommend using [binutils](https://www.gnu.org/software/binutils/) which comes with a linker that can link using ELF-i386. Again if you choose to use a differnt linked, remember to change the linker in the makefile.

### Assembler

SimpleOS uses NASM assembly for its bootloader and some other important low-level functionality, which means you'll need [NASM assembler](https://www.nasm.us) to build it.

### Compiler

Your C compiler must be able to compile into ELF-i386 32bit. If your current compiler cannot, I recommend using the [GNU compiler](https://gcc.gnu.org), which comes with an [option](https://gcc.gnu.org/onlinedocs/gcc-4.5.3/gcc/i386-and-x86_002d64-Options.html) to compile to ELF-i386 32bit. Remember to change compilers in the makefile if you use your own.

## Running

If you've gotten the emulator/linker/assembler/compiler setup, the only thing left to do is build and run SimpleOS by running the makefile in the root directory:

```
make
```



## Authors

Kevin Schulcz