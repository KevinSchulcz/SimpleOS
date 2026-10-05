[bits 32]

VIDEO_MEM      equ 0xb8000      ; where VGA mem starts
WHITE_ON_BLACK equ 0x0f

; Print 32-PM Function
;   Prints null terminated string at address bx in 32-Protected Mode
;
print_32:
    pusha
    mov edx, VIDEO_MEM

print_32_loop_start:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK
    cmp al, 0
    je print_32_loop_end

    mov [edx], ax
    add ebx, 1
    add edx, 2                  ; VGA uses 2bytes per char to display
    jmp print_32_loop_start
print_32_loop_end:

    popa
    ret