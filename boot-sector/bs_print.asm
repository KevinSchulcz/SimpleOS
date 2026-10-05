; Print Function
;   Uses ISR 0x10 to print null terminated string at address in bx
;
print:
    pusha

    print_loop_start:
        mov al, [bx]
        cmp al, 0
        je print_loop_end

        mov ah, 0x0e                    ; teletype output routine, setting every loop in-case other code messes with ax
        int 0x10                        ; BIOS added own ISR for 0x10 to be screen related i/o
        add bx, 1
        jmp print_loop_start
    print_loop_end:

    popa
    ret

; Print Newline Function
;   Uses ISR 0x10 to print a single newline character
;
print_newline:
    pusha

    mov ah, 0x0e
    mov al, 0x0a    ; '\n'
    int 0x10
    mov al, 0x0d    ; '\r' carriage return
    int 0x10

    popa
    ret

; Print Byte (Hex) Function
;   Uses ISR 0x10 to print the byte contents of dx in hexadecimal
;   (requires global var HEX_OUT)
print_hex:
    pusha

    mov cx, 0

    print_hex_loop_start:
        cmp cx, 4
        je print_hex_loop_end

        mov ax, dx
        and ax, 0x000f              ; isolate 4bits we will print
        add al, 48                  ; ASCII 1 = 48
        cmp al, 57                  ; 10+ -> we need letters A-F
        jle print_hex_loop_format
        add al, 7

    print_hex_loop_format:
        mov bx, HEX_OUT + 5
        sub bx, cx
        mov [bx], al
        shr dx, 4

        add cx, 1
        jmp print_hex_loop_start
    print_hex_loop_end:

    mov bx, HEX_OUT
    call print

    popa
    ret

HEX_OUT: 
    db '0x0000', 0