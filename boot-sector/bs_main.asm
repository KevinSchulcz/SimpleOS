[org 0x7c00]

KERNEL_OFFSET equ 0x1000

    mov [BOOT_DRIVE], dl        ; BIOS set dl to our boot drive
    mov bp, 0x9000              ; Set stack for use before switching to 32PM
    mov sp, bp

    mov bx, MSG_16RM
    call print
    call print_newline

    call load_kernel
    call switch_to_32_pm
    
    jmp $

%include "bs_print.asm"
%include "bs_gdt.asm"
%include "bs_disk_load.asm"
%include "bs_print32.asm"
%include "bs_switch32.asm"

[bits 16]
load_kernel:
    mov bx, MSG_KERN
    call print
    call print_newline

    mov bx, KERNEL_OFFSET       ; start of sectors to load
    mov dh, 15                  ; load 15 sectors for now FIXME: booting as floppy w qemu seems to allow us to load more sectors than booting as drive for some reason drive <= 3
    mov dl, [BOOT_DRIVE]        ; kernel will be on same disk as BS
    call disk_load

    ret

[bits 32]
BEGIN_PM:

    mov ebx, MSG_32PM
    call print_32
    call KERNEL_OFFSET

    jmp $

; Data
BOOT_DRIVE db 0
MSG_16RM   db "Loaded BS in 16-bit real mode", 0
MSG_32PM   db "Switched into 32-bit protected mode successfully", 0
MSG_KERN   db "Loading kernel into memory", 0

times 510-($-$$) db 0
dw 0xaa55