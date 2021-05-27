
#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @note
 * A file to organise all constants used throughout the game.
 */
#define HAND_SIZE               6
#define MAX_LINE_SIZE           6
#define SCORE_BONUS             6
#define BOARD_LENGTH            26
#define ASCII_NUMERICAL_BEGIN   48
#define ASCII_ALPHABET_BEGIN    65
#define ASCII_ALPHABET_END      90
#define MAX_ASCII_DIGIT         9
#define ASCII_COMMA             44

#define FIRST_POSITION          0
#define SECOND_POSITION         1
#define THIRD_POSITION          2

#define STRING_SIZE_2           2
#define STRING_SIZE_3           3

#define ERROR_MESSAGE           "Invalid input"
#define NEW_SAVE_FILE_FORMAT    "#Format by S3823710"

#define BOARD_SIZE_LINE_INDEX           1
#define BOARD_TILES_LINE_INDEX          2
#define TILEBAG_LINE_INDEX              3
#define CURRENT_PLAYER_LINE_INDEX       4
#define SETTINGS_LINE_INDEX_BEGIN       5
#define SETTINGS_LINE_INDEX_END         11
#define PLAYER_NAME_LINE_INDEX_BEGIN    12
#define PLAYER_SCORE_LINE_INDEX_BEGIN   13
#define PLAYER_HAND_LINE_INDEX_BEGIN    14
#define HIGH_SCORES_FILE_NAME           "highScores.data"

#define SPLASH_SCREEN           "\n░█▀▀█ ░█──░█ ▀█▀ ░█▀▀█ ░█─▄▀ ░█─── ░█▀▀▀\n░█─░█ ░█░█░█ ░█─ ░█▄▄▀ ░█▀▄─ ░█─── ░█▀▀▀\n─▀▀█▄ ░█▄▀▄█ ▄█▄ ░█─░█ ░█─░█ ░█▄▄█ ░█▄▄▄\n"

#define GET_SETTING_NAME(name) (#name)

#endif // CONSTANTS_H
