#pragma once

const float PI = 3.14159265359;

const float PLAYER_X = 0.0f;
const float PLAYER_Y = -80.0f;
const float PLAYER_RADIUS = 8.2f;

const float GUN_X = 9.0f;
const float GUN_Y = -80.0f;

const float BOMB_RADIUS = 2.0f;
const float BOMB_SPEED = 0.5;
float BOMB_EXPLOSION_MAX_RADIUS = 6.0f;
const int BOMB_WAVE_TIME_SEC = 10;

const float BULLET_SPEED = 3.0f;

const string WORDS_FILE_PATH = "words.txt";
const int WORD_LIST_LENGTH = 431;

const int TOTAL_LIVES = 3;
int livesLeft = 3;
int score = 0;

enum GameState {
    START_SCREEN,
    PLAYING,
    GAME_OVER
};

const char* SOUND_BACKGROUND = "res/sounds/background.wav";
const char* SOUND_FIRING = "res/sounds/firing.wav";
const char* SOUND_GAME_OVER = "res/sounds/game_over.wav";
const char* SOUND_LIFE_LOSE = "res/sounds/life_lose.wav";

const vector<string> EMBEDDED_WORDS = 
{
    "apple", "book", "chair", "table", "glass", "spoon", "fork", "bowl", "bread", "rice", "fruit", "mango",
    "grape", "berry", "melon", "peach", "pear", "cocoa", "honey", "milk", "tea", "salt", "buter", "cheer",
    "clean", "calm", "kind", "hope", "dream", "love", "peace", "joy", "smile", "happy", "light", "moon",
    "sun", "star", "cloud", "sky", "ocean", "river", "hill", "tree", "leaf", "bird", "room", "door", "wall",
    "roof", "lamp", "pen", "note", "class", "play", "game", "song", "music", "art", "paint", "color",
    "photo", "story", "poem", "read", "walk", "run", "jump", "hug", "cool", "rain", "snow", "dew", "spring",
    "summer", "autum", "night", "day", "week", "time", "bell", "gift", "box", "bag", "cup", "park", "road",
    "bus", "car", "boat", "ship", "bike", "ride", "trip", "map", "sand", "wave", "farm", "town", "city",
    "earth", "globe", "seed", "root", "true", "fair", "good", "best", "nice", "kind", "pure", "calm",
    "soft", "warm", "cozy", "neat", "tidy", "wise", "brave", "lucky", "cute", "fine", "fair", "safe",
    "easy", "open", "calm", "cool", "joy", "hope", "play", "song", "tree", "bird", "fish", "star", "snow",
    "rain", "wind", "leaf", "rose", "lily", "dove", "pear", "plum", "lime", "kiwi", "corn", "bean", "peas",
    "cake", "milk", "eggs", "rice", "salt", "soup", "meal", "dish", "home", "roof", "yard", "gate", "path",
    "road", "park", "lane", "lake", "hill", "rock", "sand", "dust", "gold", "silver", "iron", "wood", "clay",
    "soft", "hard", "blue", "red", "pink", "gray", "aqua", "mint", "lime", "navy", "teal", "bold", "calm",
    "kind", "true", "fair", "nice", "good", "best", "ally", "amic", "area", "axis", "baby", "back", "ball",
    "band", "bank", "barn", "base", "bath", "beam", "bear", "beat", "bell", "belt", "bend", "best", "bird",
    "blue", "boat", "body", "bold", "book", "born", "boss", "calm", "card", "care", "case", "cash", "chat",
    "chef", "city", "clap", "clip", "club", "coat", "cold", "come", "cool", "cope", "corn", "cute", "date",
    "deal", "dear", "deep", "desk", "dove", "draw", "drop", "duty", "easy", "echo", "edge", "edit", "epic",
    "ever", "exit", "face", "fact", "fair", "farm", "fast", "fate", "fear", "feed", "feel", "film", "find",
    "fire", "firm", "fish", "five", "flat", "flow", "foam", "food", "foot", "form", "free", "full", "fund",
    "gain", "game", "gate", "gift", "girl", "give", "glow", "goal", "good", "grow", "hand", "hard", "harm",
    "heal", "hear", "heat", "help", "hero", "high", "hill", "hold", "home", "hope", "huge", "idea", "join",
    "joke", "jump", "kind", "king", "kiss", "lake", "land", "last", "leaf", "lend", "life", "lift", "like",
    "line", "link", "list", "live", "long", "look", "love", "luck", "made", "main", "make", "mark", "mate",
    "meal", "meet", "mild", "mind", "mine", "mint", "mode", "moon", "more", "move", "near", "neat", "nice",
    "note", "open", "page", "park", "part", "path", "peak", "pear", "plan", "play", "pure", "rain", "rare",
    "rate", "rest", "rich", "ride", "rise", "road", "room", "rule", "safe", "sale", "same", "save", "seed",
    "seek", "self", "sell", "send", "ship", "shop", "show", "side", "sign", "sing", "site", "skin", "slow",
    "snow", "soft", "solo", "song", "soon", "soul", "star", "stay", "step", "stop", "sure", "take", "talk",
    "task", "team", "tell", "tend", "term", "test", "time", "tiny", "tire", "tone", "tool", "town", "true",
    "unit", "view", "vote", "walk", "warm", "wash", "wave", "well", "west", "wide", "wild", "will", "wind",
    "wise", "wish", "wood", "word", "work", "yard", "year", "zone"
};