#include <stdio.h>
#include <windows.h>

#include "SDL2/SDL.h"
#include "chip8.h"
#include "chip8memory.h"
#include "chip8stack.h"
#include "chip8keyboard.h"
#include "chip8screen.h"

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
    SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f};

int main(int argc, char **argv)
{

    if (argc < 2) // Checking if commandline arg provided
    {
        printf("You must provide a file to load\n");
        return -1;
    }

    const char *filename = argv[1];
    printf("The filename to load is: %s\n", filename);

    FILE *f = fopen(filename, "rb"); // Opening file in read-mode
    if (!f)
    {
        printf("Failed to open the file\n");
        return -1;
    }

    fseek(f, 0, SEEK_END); // Going to the end of the file and checking what position we're at
    long size = ftell(f);  // Gives size of the file
    fseek(f, 0, SEEK_SET); // Seeks back to the start of file

    char buf[size];
    int res = fread(buf, size, 1, f);
    if (res != 1)
    {
        printf("Failed to read from file\n");
        return -1;
    }

    struct chip8 chip8; // Making Chip8
    chip8_init(&chip8);
    chip8_load(&chip8, buf, size);
    chip8_keyboard_set_map(&chip8.keyboard, keyboard_map);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_WIDTH * CHIP8_WINDOW_MULTIPLIER,
        CHIP8_HEIGHT * CHIP8_WINDOW_MULTIPLIER,
        SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    while (1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                goto out;
                break;

            case SDL_KEYDOWN:
            {
                // This line is essentailly using SDL to map what you are presssing
                char key = event.key.keysym.sym;

                // We will use this to do the mapping part for the virtual keyboard by provding key as a parameter
                // Btw ffffffffff is essentially -1 that will appear if you put something other than 0-F

                int vkey = chip8_keyboard_map(&chip8.keyboard, key);

                if (vkey != -1)
                {
                    chip8_keyboard_down(&chip8.keyboard, vkey);
                }
            }
            break;

            case SDL_KEYUP:
            {
                // This line is essentailly using SDL to map what you are presssing

                char key = event.key.keysym.sym;

                // We will use this to do the mapping part for the virtual keyboard by provding key as a parameter
                // Btw ffffffffff is essentially -1 that will appear if you put something other than 0-F

                int vkey = chip8_keyboard_map(&chip8.keyboard, key);

                if (vkey != -1)
                {
                    chip8_keyboard_up(&chip8.keyboard, vkey);
                }
            }
            break;
            };
        }

        // Setting draw color and clearing screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        // For loop to draw a single pixel
        // Loop it multiple times and you can draw whatever you want

        for (int x = 0; x < CHIP8_WIDTH; x++)
        {
            for (int y = 0; y < CHIP8_HEIGHT; y++)
            {
                if (chip8_screen_is_set(&chip8.screen, x, y))
                {
                    SDL_Rect r;
                    r.x = x * CHIP8_WINDOW_MULTIPLIER;
                    r.y = y * CHIP8_WINDOW_MULTIPLIER;

                    // Multiplier because the Chip8 Window itself is extremely small in real
                    // Imagine if you wanted to display something on an already tiny dispaly
                    // Not gonna work right? Thats why we use the multiplier

                    r.w = CHIP8_WINDOW_MULTIPLIER;
                    r.h = CHIP8_WINDOW_MULTIPLIER;

                    // Filling up a spot with white pixel on rectangle
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }

        // Display said spot that was filled
        SDL_RenderPresent(renderer);

        // Delay Timer
        if (chip8.registers.delay_timer > 0)
        {
            Sleep(100);
            chip8.registers.delay_timer -= 1;
        }

        // Sound Timer
        if (chip8.registers.sound_timer > 0)
        {
            Beep(15000, 100 * chip8.registers.sound_timer);
            chip8.registers.sound_timer = 0;
        }

        unsigned short opcode = chip8_memory_get_short(&chip8.memory, chip8.registers.PC);
        chip8.registers.PC += 2;
        chip8_exec(&chip8, opcode);
    }

out:
    SDL_DestroyWindow(window);
    return 0;
}