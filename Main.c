#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_touch.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>
#include <SDL2/SDL_ttf.h>


#define WINDOW_TITLE "Shiu Editor"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_BUFFER 2048

struct Editor {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Texture *text_texture;
    SDL_Rect text_rect;
    char buffer[MAX_BUFFER];
};

//Function definitions
bool sdl_initialize(struct Editor *editor);
void editor_cleanup(struct Editor *editor, int exit_status);
void window_customize(struct Editor *editor);
void handle_key_presses(struct Editor *editor, SDL_Keysym keysym, char *buffer);
void update_text_texture(struct Editor *editor);

int main() {
    struct Editor editor = {0};

    if(sdl_initialize(&editor)){
        editor_cleanup(&editor, EXIT_FAILURE);
    };
    
    //Here is the main editor logic

    window_customize(&editor);
    
    SDL_Event event;

    editor.text_rect.x = 50;
    editor.text_rect.y = 50;

    bool needs_update = true;
    while (true) {
        while (SDL_PollEvent(&event)) {
            switch(event.type){
                case SDL_QUIT: 
                    editor_cleanup(&editor, EXIT_SUCCESS);
                    break;
                
                case SDL_TEXTINPUT: 
                    if (strlen(editor.buffer) + strlen(event.text.text) < MAX_BUFFER - 1) {
                        strcat(editor.buffer, event.text.text);
                        needs_update = true;
                    }
                    break;
                // case SDL_KEYDOWN:
                //     handle_key_presses(&editor, event.key.keysym, buffer);
                //     break;
                default:
                    break;
            }
        }
        
        if (needs_update) {
            update_text_texture(&editor);
            needs_update = false;
        }
        
        SDL_SetRenderDrawColor(editor.renderer, 30, 30, 30, 255);
        SDL_RenderClear(editor.renderer);
        SDL_RenderCopy(editor.renderer, editor.text_texture, NULL, &editor.text_rect);
        SDL_RenderPresent(editor.renderer);
        
        
        //Delay the loop to be around 60 frames/s
        SDL_Delay(16);
    }



    editor_cleanup(&editor, EXIT_SUCCESS);
    return 0;
}


void editor_cleanup(struct Editor *editor, int exit_status){
    SDL_DestroyRenderer(editor -> renderer);
    SDL_DestroyWindow(editor -> window);
    SDL_Quit();
    exit(exit_status);
}

bool sdl_initialize(struct Editor *editor){
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    editor->window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_HIDDEN
    );

    if (!editor -> window){
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    editor -> renderer = SDL_CreateRenderer(editor->window, -1, 0);

    if (!editor -> renderer){
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return 1;
    }

    SDL_ShowWindow(editor->window);
    
    TTF_Init();
    
    //Font Local to my Linux machine.
    editor->font = TTF_OpenFont("/usr/share/fonts/X11/Type1/C059-Italic.pfb", 24);
    if (!editor->font) {
        fprintf(stderr, "Font load error: %s\n", TTF_GetError());
        return 1;
    }
    
    SDL_StartTextInput();

    return 0;
}

void window_customize(struct Editor *editor){
    SDL_Color color = {};
}

void handle_key_presses(struct Editor *editor, SDL_Keysym keysym, char *buffer){
    switch(keysym.sym){
        case SDLK_DOWN: 
            break;
            
        default:
            buffer[0] = keysym.sym;
            printf("text : %s", buffer);
            break;
    }
}
void update_text_texture(struct Editor *editor) {
    if (editor->text_texture) {
        SDL_DestroyTexture(editor->text_texture);
    }

    const char *text_to_render = (strlen(editor->buffer) > 0) ? editor->buffer : " ";

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderUTF8_Blended(editor->font, text_to_render, white);
    
    if (surface) {
        editor->text_texture = SDL_CreateTextureFromSurface(editor->renderer, surface);
        editor->text_rect.w = surface->w;
        editor->text_rect.h = surface->h;
        SDL_FreeSurface(surface);
    }
}