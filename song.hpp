#ifndef SONG_H
#define SONG_H

#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

typedef struct {
    float start;
    float len;
    bool touched;
    string freq;
} bar_t;

typedef struct {
    vector<bar_t> bars[4];
} song_t;

song_t loadSong(const char* path) {
    cout << "Loading song " << path << "\n";
    ifstream file;
    int num_beats;
    int col; float start, len; string freq;
    song_t song;

    file.open(path);
    if (!file.is_open()) {
        cout << "Error in opening " << path << "\n";
        return song;
    }

    file >> num_beats;
    printf("NUM_BEATS: %d\n", num_beats);
    for (int i = 0; i < num_beats; i++) {
        file >> col;
        file >> start >> len;
        file >> freq;
        song.bars[col].push_back({
            .start = start,
            .len = len,
            .touched = false,
            .freq = freq
        });
    }
    cout << "Song succesfully loaded!\n";
    return song;
}

#endif