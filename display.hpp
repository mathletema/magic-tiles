#include <iostream>
#include "SDL2/SDL.h"
#include "bar.hpp"

using namespace std;

namespace Display {
class Display {
private:
    float WIDTH, HEIGHT;
    const char *TITLE;
    SDL_Window *win;
    SDL_Renderer *ren;
    SDL_Color bg_fill = {0xff, 0xff, 0xff};
    SDL_Color rect_fill = {0x33, 0x33, 0x33};
    SDL_Color tchd_fill = {0x35, 0xA7, 0xFF};
    int init();
    int quit();

public:
    Display(const char *title, int width, int height);
    ~Display();
    void clear();
    void draw(int col, float pos);
    void draw_bar(Bar::Bar bar);
    void render();
}; 

Display::Display(const char *title, int width, int height) {
    WIDTH = width; HEIGHT = height;
    TITLE = title;

    if (init() != 0) throw "SDL failed to initialize";
    printf("SDL Succesfully init\n");
}

Display::~Display() {
    quit();
}

int Display::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) return 1;

    win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        SDL_DestroyWindow(win);
        cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    return 0;
}

void Display::clear() {
    SDL_SetRenderDrawColor(ren, bg_fill.r, bg_fill.g, bg_fill.b, bg_fill.a);
    SDL_RenderClear(ren);
}

void Display::draw(int col, float pos) {
    
    SDL_Rect rect;
    rect.x = col * WIDTH / 4;
    rect.w = WIDTH / 4;
    rect.y = pos * HEIGHT;
    rect.h = 200;

    SDL_SetRenderDrawColor(ren, rect_fill.r, rect_fill.g, rect_fill.b, rect_fill.a);
    SDL_RenderFillRect(ren, &rect);
}

void Display::draw_bar(Bar::Bar bar) {
    SDL_Rect rect;
    rect.x = (float) bar.col * WIDTH / 4;
    rect.w = (float) WIDTH / 4;
    rect.y = (float) bar.pos * HEIGHT;
    rect.h = (float) bar.len * HEIGHT;

    if (bar.touched) SDL_SetRenderDrawColor(ren, tchd_fill.r, tchd_fill.g, tchd_fill.b, tchd_fill.a);
    else SDL_SetRenderDrawColor(ren, rect_fill.r, rect_fill.g, rect_fill.b, rect_fill.a);

    SDL_RenderFillRect(ren, &rect);
}

void Display::render() {
    SDL_RenderPresent(ren);
}

int Display::quit() {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
}