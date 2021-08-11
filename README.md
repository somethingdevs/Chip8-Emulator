# Chip8-Emulator
Chip 8 Emulator using C

About
-----
Interpreter from mid 1970s
Essentially a virtual machine but really basic
4096 bytes of memory


Memory Layout
-------------
Stack(array) 16 16-bit values - Store address that Chip8 should return when returning from subroutine
Allows for up to 16 levels of nested subroutines
P.S Stack is not part of main memory, it is seperate


Registers 
---------
16 8-bit data registers (Each 8-bit register can hold 1 byte of information)- Stored in the form an array. Store general data. V0-VF
16-bit register I - Used to store memory address
16-bit PC - Used to point address of the current instruction
8-bit Stack Pointer - Points to location in the stack8-bit Sound Timer - plays beep when timer != 0
8-bit Delay Time - Stops executing instrcuctions when delay timer > 0

Display
-------
64x32 Pixels monochrome display

Drawing
-------
Refers to how things are displayed/drawn on the display and how they appear
Drawing is done using sprites and not pixels, if overflow screen it wraps around to the other side

Sprites
-------
Sprites are many pixels grouped together that can be drawn to the screen as a whole
Can be max of 8 bits wide(X-Axis) and 15 bits in lenght(Y-Axis)
Sprites get XOR'ed onto the dispaly

Keyboard
--------
Chip8 keyboard has 16 keys from 0-F
Can be represented as a 16 byte array
True value when key is pressed, false when not

Instruction Set
---------------
36 different instrctions that need to be implemented
Instruction for Mathematical Operations, drawing, etc
