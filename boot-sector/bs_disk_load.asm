; Disk Load Function
;   Loads dh sectors from frive dl to es:bx
;
disk_load:
    pusha

    push dx
                        ; BIOS Routine:
    mov ah, 0x02        ; read sector routine
    mov al, dh          ; num sectors to read
    mov ch, 0x00        ; cylinder 0
    mov dh, 0x00        ; head 0
    mov cl, 0x02        ; start reading after boot sector
    int 0x13

    jc disk_load_error  ; jmp if carry flag set

    pop dx
    cmp dh, al
    jne disk_sector_num_error

    disk_load_end:
    popa
    ret

disk_load_error:
    mov bx, DISK_LOAD_ERROR_MSG
    call print
    call print_newline
    jmp disk_load_end

disk_sector_num_error:
    mov bx, DISK_SECTOR_NUM_ERROR_MSG
    call print
    call print_newline
    jmp disk_load_end
    
DISK_LOAD_ERROR_MSG       db 'Disk Load Error.', 0
DISK_SECTOR_NUM_ERROR_MSG db 'Num of Sectors Read Unequal to Amount Requested.', 0