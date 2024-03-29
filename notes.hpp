#include <cstdint>
#include <fstream>
#include <iostream>
#include "SDL2/SDL_mixer.h"

using namespace std;

typedef uint8_t key_t;

key_t string_to_key(const char* inp) {
    if (inp[1] == 'b') {
        return  ((inp[2] - '0') << 4) + ((inp[0] - 'A') << 1) + 1;
    } else {
        return ((inp[1] - '0') << 4) + ((inp[0] - 'A') << 1);
    }
}

string key_to_string(key_t inp) {
    string res;
    res.push_back('A' + ((inp >> 1) & 0b111));
    if (inp & 0b1) res.push_back('b');
    res.push_back('0' + (inp >> 4));
    return res;
}

class Keyboard {
private:
    Mix_Chunk* notes[128];
    Mix_Chunk* fail;
public:
    Keyboard();
    ~Keyboard();
    void play_note(const char *note);
    void play_fail();
};

Keyboard::Keyboard() {
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    Mix_AllocateChannels(1024);

    for (int i = 0; i < 128; i++) {
        notes[i] = Mix_LoadWAV(("./assets/notes/" + key_to_string(i) + ".wav").c_str());
    }

    fail = Mix_LoadWAV("./assets/effects/fail.wav");
}

void Keyboard::play_note(const char* note) {
    key_t note_ = string_to_key(note);
    Mix_PlayChannel(-1 , notes[note_], 0);
}

void Keyboard::play_fail() {
    Mix_PlayChannel(-1, fail, 0);
}

Keyboard::~Keyboard() {
    for (int i = 0; i < 128; i++) {
        Mix_FreeChunk(notes[i]);
        notes[i] = NULL;
    }
    Mix_Quit();
}