#ifndef SONG_H
#define SONG_H

#include <fstream>
#include <vector>
using namespace std;

namespace Song {

typedef struct {
    float *start[4];
    float *end[4];
    int num_beats[4];
} Song;

void freeSong(Song song) {
    for (int i = 0; i < 4; i++) {
        free(song.start[i]);
        free(song.end[i]);
    }
}

Song loadSong(const char* path) {
    ifstream file;
    Song song;

    file.open(path);
    
    for (int i = 0; i < 4; i++) {
        file >> song.num_beats[i];
        song.start[i] = (float*) malloc(song.num_beats[i] * sizeof(float));
        song.end[i] = (float*) malloc(song.num_beats[i] * sizeof(float));
        for (int j = 0; j < song.num_beats[i]; j++) {
            file >> song.start[i][j] >> song.end[i][j];
        }
    }

    return song;
}

}

#endif