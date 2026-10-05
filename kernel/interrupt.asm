[extern isr_handler]
[extern irq_handler]

; FIXME: gdt is not linked with the rest of the kernel
;   it is prepended at the end with bs_main, should prob
;   find another solution instead of hardcoding numbers
;[extern DATA_SEG]           ; defined in ../boot-sector/bs_gdt.asm

; ************
; *   ISRs   *
; ************

; ISR "pre-process" function
;   called by all ISRs
;   saves processor state
;   sets segment regs to enter kernel mode
;   then calls ISR handler
isr_common_stub:
    pusha

    mov ax, ds
    push eax                      ; save current processes' data seg reg on stack

    mov ax, 0x10                  ; FIXME: see phone replace 0x10 w DATA_SEG ; load kernel space segments (not ss)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler

    pop eax                 ; restore process state
    mov ds, ax              ; FIXME: extra segments might not have been the same as ds 
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8              ; pop error code and int number
    sti
    iret                    ; pops cs, eip, eflags, ss, esp (pushed by original int)

%macro ISR_W_DUMMY_CODE 1
[global isr%1]
isr%1:
    cli
    push byte 0             ; dummy error code to follow proper fmt
    push byte %1
    jmp isr_common_stub
%endmacro

%macro ISR_W_CODE 1
[global isr%1]
isr%1:
    cli
    push byte %1            ; int pushes an error code on the stack itself
    jmp isr_common_stub
%endmacro

                            ; ISR(0-31) reserved for CPU exceptions
ISR_W_DUMMY_CODE 0          ; div by zero
ISR_W_DUMMY_CODE 1          ; debug
ISR_W_DUMMY_CODE 2          ; non-maskable interrupt
ISR_W_DUMMY_CODE 3          ; breakpoint
ISR_W_DUMMY_CODE 4          ; overflow
ISR_W_DUMMY_CODE 5          ; bounds check
ISR_W_DUMMY_CODE 6          ; invalid opcode
ISR_W_DUMMY_CODE 7          ; coprocessor not available
ISR_W_CODE 8                ; double fault (when an exception is invoked within a handler and the two cannot be handled serially)
ISR_W_DUMMY_CODE 9          ; coprocessor segment overrun
ISR_W_CODE 10               ; invalid TTS
ISR_W_CODE 11               ; segment not present
ISR_W_CODE 12               ; stack exception
ISR_W_CODE 13               ; general protection exception
ISR_W_CODE 14               ; page fault
ISR_W_DUMMY_CODE 15
ISR_W_DUMMY_CODE 16         ; coprocessor error
ISR_W_DUMMY_CODE 17
ISR_W_DUMMY_CODE 18
ISR_W_DUMMY_CODE 19
ISR_W_DUMMY_CODE 20
ISR_W_DUMMY_CODE 21
ISR_W_DUMMY_CODE 22
ISR_W_DUMMY_CODE 23
ISR_W_DUMMY_CODE 24
ISR_W_DUMMY_CODE 25
ISR_W_DUMMY_CODE 26
ISR_W_DUMMY_CODE 27
ISR_W_DUMMY_CODE 28
ISR_W_DUMMY_CODE 29
ISR_W_DUMMY_CODE 30
ISR_W_DUMMY_CODE 31

; ************
; *   IRQs   *
; ************

; IRQ "pre-process" function
;   Identical to ISR stub except call to handler
irq_common_stub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10 ;DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq_handler        ; Different handler than ISRs
    pop ebx                 ;

    mov ds, bx 
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    sti
    iret

%macro IRQ_W_CODE 2
[global irq%1]
irq%1:
    cli
    push 0                  ; dummy error code since IRQs don't push one itself
    push byte %2
    jmp irq_common_stub
%endmacro

; IRQ number => ISR number it is mapped to
IRQ_W_CODE 0,  32
IRQ_W_CODE 1,  33
IRQ_W_CODE 2,  34
IRQ_W_CODE 3,  35
IRQ_W_CODE 4,  36
IRQ_W_CODE 5,  37
IRQ_W_CODE 6,  38
IRQ_W_CODE 7,  39
IRQ_W_CODE 8,  40
IRQ_W_CODE 9,  41
IRQ_W_CODE 10, 42
IRQ_W_CODE 11, 43
IRQ_W_CODE 12, 44
IRQ_W_CODE 13, 45
IRQ_W_CODE 14, 46
IRQ_W_CODE 15, 47