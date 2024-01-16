#include <iostream>
#include <queue>
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "display.hpp"
#include "song.hpp"
#include "notes.hpp"

Mix_Music* gMusic = NULL;

class Game {
private:
    const char* TITLE;
    int WIDTH, HEIGHT;
    int SPEED = 2.2;
    
    Display* display;
    SDL_Event event;
    
    Keyboard keyboard;

    song_t song;
    deque<bar_t> bars[4];
    size_t cur_beat[4];

    int init();
    void spawnBar(int col, bar_t bar);
    void scheduleBars(int col, float t);
    void moveBars(float dt);
    void drawBars();
    void barsGC();
    bool songOver();
    bool gameOver();
    void handleKeyDown(SDL_Event e);
    int quit();

public:
    Game(const char* title, int width, int height);
    ~Game();
    void setSong(song_t song_);
    void run();
};

Game::Game(const char* title, int width, int height) : TITLE(title), WIDTH(width), HEIGHT(height), display(nullptr) {
    init();
}

int Game::init() {
    display = new Display(TITLE, WIDTH, HEIGHT);

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        }
    gMusic = Mix_LoadMUS( "assets/notes/senorita.wav" );

    cout << "Game started!\n";
    return 0;
}

void Game::setSong(song_t song_) {
    song = song_;
    for (int i = 0; i < 4; i++) cur_beat[i] = 0;
}

void Game::spawnBar(int col, bar_t bar) {
    bar_t b;
    b.start = -1 - bar.len; b.len=bar.len; b.touched=false; b.freq=bar.freq;
    bars[col].push_back(b);
}

void Game::scheduleBars(int col, float t) {
    if (cur_beat[col] >= song.bars[col].size()) return;
    if (SPEED * t > song.bars[col][cur_beat[col]].start) {
        spawnBar(col, song.bars[col][cur_beat[col]]);
        cur_beat[col]++;
    }
}

void Game::moveBars(float dt) {
    for (int col = 0; col < 4; col++) {
        for (size_t i = 0; i < bars[col].size(); i++) {
            bars[col][i].start += SPEED * dt;
        }
    }
    
}

void Game::drawBars() {
    int i = 0;
    for (int col = 0; col < 4; col++) {
        for (auto b: bars[col]) {
            display->draw_bar(b, col);
            ++i;
        }
    }
}

void Game::barsGC() {
    for (int col = 0; col < 4; col++) {
        while (!bars[col].empty() && bars[col].front().start >= 1) {
            bars[col].pop_front();
        }
    }
}

bool Game::gameOver() {
    bool all_empty = true;
    for (int col = 0; col < 4; col++) {
        if (!bars[col].empty()) all_empty = false;
    }
    if (all_empty) return false;

    for (int col = 0; col < 4; col++) {
        for (auto b: bars[col]) {
            if (!b.touched && b.start + b.len > 1) return true;
        }
    }
    return false;
}

bool Game::songOver() {
    for (int col = 0; col < 4; col++) {
        if (cur_beat[col] < song.bars[col].size()) {
            return false;
        }
    }
    return true;
}

void Game::run() {
    int quit = 0;

    double t = 0.0;
    double dt = 1.0 / 60.0;

    cout << "Starting game loop!\n";
    while (!quit) {
        if (gameOver()) {
            quit = true;
            keyboard.play_fail();
            cout << "GAME OVER!\n";
            SDL_Delay(1000);
            break;
        }

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                    handleKeyDown(event);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        bar_t b; b.len = 0.25; b.freq = "c4";
                        spawnBar(event.button.x * 4 / WIDTH, b);
                    }
                    break;
            }
        }

        if( Mix_PlayingMusic() == 0 )
        {
            Mix_PlayMusic( gMusic, -1 );
        }

        for (int i = 0; i < 4; i++) {
            scheduleBars(i, t);
        }
        moveBars(dt);
        barsGC();

        display->clear();
        drawBars();
        display->render();

        t += dt;
        
    }
}

void Game::handleKeyDown(SDL_Event e) {
    int c = -1;
    switch (e.key.keysym.sym) {
        case SDLK_1:
            c = 0; break;
        case SDLK_2:
            c = 1; break;
        case SDLK_3:
            c = 2; break;
        case SDLK_4:
            c = 3; break;
        case SDLK_a:
            keyboard.play_note("C4"); break;
        case SDLK_s:
            keyboard.play_note("D4"); break;
        case SDLK_d:
            keyboard.play_note("E4"); break;
        case SDLK_f:
            keyboard.play_note("F4"); break;
        case SDLK_g:
            keyboard.play_note("G4"); break;
        case SDLK_h:
            keyboard.play_note("A4"); break;
        case SDLK_j:
            keyboard.play_note("B4"); break;
        case SDLK_k:
            keyboard.play_note("C5"); break;
    }
    if (c >= 0 && !bars[c].empty()) {
        for (size_t i = 0; i < bars[c].size(); i++) {
            if (!bars[c][i].touched) {
                bars[c][i].touched = true;
                keyboard.play_note(bars[c][i].freq.c_str());
                break;
            } 
        }
    }
}

int Game::quit() {
    delete display;
    return 0;
}

Game::~Game() {
    cout << "Game quitting\n";
    quit();
}