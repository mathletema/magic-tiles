#include <iostream>
#include <queue>
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "display.hpp"
#include "song.hpp"

namespace Game {

Mix_Music* gMusic = NULL;

class Game {
private:
    const char* TITLE;
    int WIDTH, HEIGHT;
    int SPEED = 2;
    Display::Display* display;
    SDL_Event event;
    deque<Bar::Bar> bars; 
    Song::Song song;
    int cur_beat[4];

    int init();
    void spawnBar(int col, float length);
    void scheduleBars(int col, float t);
    void moveBars(float dt);
    void drawBars();
    void barsGC();
    bool songOver();
    void handleKeyDown(SDL_Event e);
    int quit();

public:
    Game(const char* title, int width, int height);
    ~Game();
    void setSong(Song::Song song_);
    void run();
};

Game::Game(const char* title, int width, int height) : TITLE(title), WIDTH(width), HEIGHT(height), display(nullptr) {
    init();
}

int Game::init() {
    display = new Display::Display(TITLE, WIDTH, HEIGHT);

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        }
    gMusic = Mix_LoadMUS( "assets/music/senorita.wav" );

    return 0;
}

void Game::setSong(Song::Song song_) {
    song = song_;
    for (int i = 0; i < 4; i++) cur_beat[i] = 0;
}

void Game::spawnBar(int col, float length) {
    bars.push_back({.col= col, .pos = -length, .len=length});
}

void Game::scheduleBars(int col, float t) {
    if (cur_beat[col] >= song.num_beats[col]) return;
    if (SPEED * t > song.start[col][cur_beat[col]]) {
        spawnBar(col, 1 * (song.end[col][cur_beat[col]] - song.start[col][cur_beat[col]]));
        cur_beat[col]++;
    }
}

void Game::moveBars(float dt) {
    if (bars.empty()) return;
    for (size_t i = 0; i < bars.size(); i++) {
        bars[i].pos += SPEED * dt;
    }
}

void Game::drawBars() {
    if (bars.empty()) return;
    int i = 0;
    for (auto b: bars) {
        display->draw_bar(b);
        ++i;
    }
}

void Game::barsGC() {
    while (!bars.empty() && bars.front().pos >= 1) {
        bars.pop_front();
    }
}

bool Game::songOver() {
    for (int i = 0; i < 4; i++) {
        if (cur_beat[i] < song.num_beats[i]) {
            return false;
        }
    }
    return true;
}

void Game::run() {
    int quit = 0;

    double t = 0.0;
    double dt = 1.0 / 60.0;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                    handleKeyDown(event);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                        spawnBar(event.button.x * 4 / WIDTH, 0.25);
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
    }
    if (c >= 0) {
        if (bars.empty()) return;
        for (size_t i = 0; i < bars.size(); i++) {
            if (bars[i].col == c && !bars[i].touched) {
                bars[i].touched = true;
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

} // namespace Game
