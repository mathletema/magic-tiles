#include "stdio.h"
#include "SDL2\SDL.h"
#include "SDL2\SDL_mixer.h"
#include "game.hpp"
#include "song.hpp"

const char* TITLE = "Ishank's game";
int WIDTH = 400;
int HEIGHT = 700;
const char* SONG = "assets/songs/badguy.song";

int main (int argc, char* argv[]) {
    Game g((const char*) TITLE, WIDTH, HEIGHT);
    song_t s = loadSong(SONG);
    g.setSong(s);
    g.run();
    return 0;
}