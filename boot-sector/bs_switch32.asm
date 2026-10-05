[bits 16]
switch_to_32_pm:

    cli                     ; switch off interrupts until we set the new ones

    lgdt [gdt_descriptor]

    mov eax, cr0            ; Set first bit in CPU control register to enable 32bPM
    or eax, 0x1
    mov cr0, eax
    
    jmp CODE_SEG:init_pm    ; Need to "far" jump so CPU flushes and pipelined tasks (which would still be for 16bRM)


[bits 32]
init_pm:

    mov ax, DATA_SEG        ; Need to populate segment registers with proper addresses
    mov ds, ax              ; Data Segment
    mov es, ax              ; Extra Segments
    mov fs, ax              ;
    mov gs, ax              ;
    mov ss, ax              ; Stack Segment

    mov ebp, 0x90000        ; Position stack at the top of free space
    mov esp, ebp

    call BEGIN_PM