; GDT specifications for memory segmentation
;   Currently implmenting a simple "basic flat model" with
;   only two segments Code & Data (+null).
gdt_start:

gdt_null:                   ; Null descriptor (required first entry)
    dd 0x0
    dd 0x0

gdt_code:                   ; Code descriptor
    dw 0xffff               ; Limit (bits 0-15)
    dw 0x0                  ; Base  (bits 0-15)
    db 0x0                  ; Base  (bits 16-23)
    db 10011010b            ; Present(1) Privilege(00) DescType(1) Code(1) Conforming(0) Readable(1) Accessed(0)
    db 11001111b            ; Granularity(1) 32bDefault(1) 64bSegments(0) AVL(0) Limit(bits 16-19)
    db 0x0                  ; Base  (bits 24-31)

gdt_data:                   ; Data descriptor
    dw 0xffff               ; Limit (bits 0-15)
    dw 0x0                  ; Base  (bits 0-15)
    db 0x0                  ; Base  (bits 16-23)
    db 10010010b            ; Present(1) Privilege(00) DescType(1) Code(0) ExpandDown(0) Writable(1) Accessed(0)
    db 11001111b            ; Granularity(1) 32bDefault(1) 64bSegments(0) AVL(0) Limit(bits 16-19)
    db 0x0                  ; Base  (bits 24-31)

gdt_end:


; GDT descriptor
;   Predefined protocol for CPU to digest our custom GDT.
gdt_descriptor:
    dw gdt_end - gdt_start - 1      ; Size of GDT (need one less)
    dd gdt_start                    ; Start address of GDT


; Constants For PM
;   The segment registers must contain the location of 
;   their respective segment descriptors.
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start