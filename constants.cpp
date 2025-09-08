#pragma once

const float PI = 3.14159265359;

const float PLAYER_X = 0.0f;
const float PLAYER_Y = -80.0f;
const float PLAYER_RADIUS = 8.2f;

const float GUN_X = 9.0f;
const float GUN_Y = -80.0f;

const float BOMB_RADIUS = 2.0f;
const float BOMB_SPEED = 0.15;
float BOMB_EXPLOSION_MAX_RADIUS = 6.0f;
const int BOMB_WAVE_TIME_SEC = 10;

const float BULLET_SPEED = 3.0f;

const string WORDS_FILE_PATH = "words.txt";
const int WORD_LIST_LENGTH = 431;

const int TOTAL_LIVES = 3;
int livesLeft = 3;
int score = 0;

const char* SOUND_BACKGROUND = "sounds/background.wav";
const char* SOUND_GAME_OVER = "sounds/game_over.wav";
const char* SOUND_EXPLOSION = "sounds/life_lose.wav";
