#include "../kernel/types.h"
#include "screen.h"
#include "port.h"
#include "../kernel/util.h"

static int get_screen_offset(int row, int col);
static int get_cursor_offset(void);
static void set_cursor_offset(int offset);
static int handle_scroll(int offset);
static void print_char(char c, int row, int col, char attribute);
static int offset_to_row(int offset);
static int offset_to_col(int offset);
static void itoa(int num, char *str);

//
// NOTE: should prob refactor print funcs to only a few small API calls
//  then have setter functions to set properties such as colour, etc.
//  that all the API calls follow. (maybe change types to custom types as well)
//

//
// Kernel API
//

// Prints a null terminated string at pos(row,col) on screen 
//  Will print at cursor pos if pos(row,col) is invalid
void print_at(char *msg, int row, int col)
{
    int i = 0;
    while (msg[i] != '\0')
    {
        print_char(msg[i], row, col, WHITE_ON_BLACK);

        // print_char updates cursor so we can just
        //  use the updated pos to get the next pos
        //  to print to
        row = offset_to_row(get_cursor_offset());
        col = offset_to_col(get_cursor_offset());
        i++;
    }
}

// Prints a null terminated string on screen at cursor
void print(char *msg)
{
    print_at(msg, -1, -1);
}

// Prints a null terminated string + a newline on screen at cursor
void println(char *msg)
{
    print_at(msg, -1, -1);
    print_char('\n', -1, -1, WHITE_ON_BLACK);
}

// Prints a uint32 number in hex on screen at cursor
void print_hex(uint32 num)
{
    // FIXME: prob a better way to do this entire function
    //  itoa returns in decimal so we need to convert to 
    //  hex before passing it to itoa
    //  make sure to change print_hex_at too
    char hex_str[33];
    itoa(num, hex_str);
    
    print("0x");
    print(hex_str);
    
}

// Prints a uint32 number in hex on screen at pos(row, col)
//  Will print at cursor pos if pos(row,col) is invalid
//  Virtually same as print_hex just calls on print_at instead
void print_hex_at(uint32 num, int row, int col)
{
    char hex_str[33];
    itoa(num, hex_str);

    print_at("0x", row, col);
    col += 2;                       // FIXME: not checking if col goes past edge of screen
    print_at(hex_str, row, col);
}

// Helper function for print_hexs, adds a null terminator
//  at the end of the number
//  could probably be put somewhere else
static void itoa(int num, char *str)
{
    // Reverse the number so we can convert in-order
    int digit   = 0;
    int num_rev = 0;
    while (num > 0)
    {
        num_rev *= 10;

        digit = num % 10;
        num_rev += digit;

        num /= 10;
    }

    // Now convert directly into ascii
    int ptr = 0;
    while (num_rev > 0)
    {
        digit = num_rev % 10;
        str[ptr] = (char)(digit + 48);

        ptr++;
        num_rev /= 10;
    }
    str[ptr] = '\0';
}

// Clears all chars from VGA mem
void clear_screen(void)
{
    uint8 *VGA_mem = (uint8 *)VGA_ADDRESS;

    for (int i = 0; i < MAX_ROW * MAX_COL; i++)
    {
        VGA_mem[i * 2]     = ' ';
        VGA_mem[i * 2 + 1] = WHITE_ON_BLACK;
    }

    set_cursor_offset(get_screen_offset(0, 0));
}

//
// Private Helper Functions
//

// Places c with attribute into VGA memory at row-col
//  If row/col are invalid will be placed at cursor
//  If attribute is invalid (0) fallbacks to default
static void print_char(char c, int row, int col, char attribute)
{
    uint8 *VGA_mem = (uint8 *)VGA_ADDRESS;

    if (!attribute)
    {
        attribute = WHITE_ON_BLACK;
    }

    // Calculate where to place in VGA mem
    int offset = 0;
    if (row < 0 || col < 0 || row >= MAX_ROW || col >= MAX_COL)
    {
        offset = get_cursor_offset();
    }
    else
    {
        offset = get_screen_offset(row, col);
    }

    // Adjust VGA mem (if newline we just need to set offset to \n\cr)
    if (c == '\n')
    {
        offset = get_screen_offset(offset_to_row(offset) + 1, 0);
    }
    else
    {
        VGA_mem[offset]     = c;
        VGA_mem[offset + 1] = attribute;
        offset += 2;
    }

    // Reposition Cursor
    offset = handle_scroll(offset);
    set_cursor_offset(offset);
}

// Calculates mem offset for particular row/col placement in VGA mem
static int get_screen_offset(int row, int col)
{
    return ((row * MAX_COL) + col) * 2;         // *2: we need 2 bytes (char,attr)
}

// Retrieves and returns the cursor offset in VGA mem
static int get_cursor_offset(void)
{
    int offset = 0;

    port_byte_out(SCREEN_REG_CTRL, CUR_OFF_HIGH_BYTE_REG);
    offset = port_byte_in(SCREEN_REG_DATA) << 8;
    port_byte_out(SCREEN_REG_CTRL, CUR_OFF_LOW_BYTE_REG);
    offset += port_byte_in(SCREEN_REG_DATA);

    // Offset position is in terms of chars NOT bytes
    //  so we need to account for attribute bytes
    offset *= 2;

    return offset;
}

// Sets the screen device cursor offset registers to requested offset
static void set_cursor_offset(int offset)
{
    // The regs need the offset in terms of chars
    offset /= 2;

    port_byte_out(SCREEN_REG_CTRL, CUR_OFF_HIGH_BYTE_REG);
    port_byte_out(SCREEN_REG_DATA, (unsigned char)(offset >> 8));
    port_byte_out(SCREEN_REG_CTRL, CUR_OFF_LOW_BYTE_REG);
    port_byte_out(SCREEN_REG_DATA, (unsigned char)(offset & 0xff));
}

// Returns the screen row given the VGA mem offset
static int offset_to_row(int offset)
{
    return offset / (2 * MAX_COL);
}

// Returns the screen col given the VGA mem offset
static int offset_to_col(int offset)
{
    return (offset - (offset_to_row(offset) * 2 * MAX_COL)) / 2;
}

// Shifts screen text one line up given an off-screen offset
static int handle_scroll(int offset)
{
    if (offset < MAX_ROW * MAX_COL * 2)
        return offset;

    for (int i = 1; i < MAX_ROW; i++)
    {
        mem_cpy(  (uint8 *)VGA_ADDRESS + get_screen_offset(i-1, 0)
                , (uint8 *)VGA_ADDRESS + get_screen_offset(i, 0)
                , MAX_COL * 2);
    }

    uint8 *bottom_row = (uint8 *)(VGA_ADDRESS + get_screen_offset(MAX_ROW - 1, 0));
    for (int i = 0; i < MAX_COL; i++)
    {
        bottom_row[i * 2]     = ' ';
        bottom_row[i * 2 + 1] = WHITE_ON_BLACK;
    }

    return get_screen_offset(MAX_ROW - 1, 0);
}