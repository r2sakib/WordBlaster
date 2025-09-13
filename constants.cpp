#pragma once

const float PI = 3.14159265359;

const float PLAYER_X = 0.0f;
const float PLAYER_Y = -80.0f;
const float PLAYER_RADIUS = 8.2f;

const float GUN_X = 9.0f;
const float GUN_Y = -80.0f;

const float BOMB_RADIUS = 2.0f;
const float BOMB_SPEED = 0.13;
float BOMB_EXPLOSION_MAX_RADIUS = 6.0f;
const int BOMB_WAVE_TIME_SEC = 12;
int NUM_OF_BOMBS_PER_WAVE = 5;

const float BULLET_SPEED = 3.0f;


const int TOTAL_LIVES = 3;
int livesLeft = TOTAL_LIVES;
int score = 0;

enum GameState {
    HOME_SCREEN,
    PLAYING,
    GAME_OVER
};

const string WORDS_FILE_PATH = "res/words.txt";
const int WORD_LIST_LENGTH = 431;

const char* SOUND_BACKGROUND = "res/sounds/background.wav";
const char* SOUND_FIRING = "res/sounds/firing.wav";
const char* SOUND_GAME_OVER = "res/sounds/game_over.wav";
const char* SOUND_LIFE_LOSE = "res/sounds/life_lose.wav";
const char* SOUND_TRANSITION = "res/sounds/transition.wav";