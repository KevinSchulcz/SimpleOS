; Ensures we jump to the correct spot in the kernel code
[bits 32]
[extern main]
call main
jmp $