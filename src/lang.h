/*  lang.h
 *  Presentation Manager Robots v1.4
 *  6-language runtime string support
 *
 *  LICENSE: GNU GPL V3
 */

#ifndef LANG_H
#define LANG_H

#define LANG_EN     0
#define LANG_ES     1
#define LANG_NL     2
#define LANG_DE     3
#define LANG_FR     4
#define LANG_IT     5
#define LANG_COUNT  6

enum {
    STR_MENU_GAME = 0,
    STR_MENU_NEW,
    STR_MENU_SONIC,
    STR_MENU_TELEP,
    STR_MENU_WAIT,
    STR_MENU_HSCORES,
    STR_MENU_CLEAR,
    STR_MENU_PAUSE,
    STR_MENU_EXIT,
    STR_MENU_OPTIONS,
    STR_MENU_LANGUAGE,
    STR_MENU_SAVEONEXIT,
    STR_MENU_BACKGRND,
    STR_MENU_FRAME,
    STR_MENU_HELP,
    STR_MENU_HOWTOPLAY,
    STR_MENU_ABOUT,
    STR_COUNT
};

extern int current_lang;
extern const char *lang_strings[LANG_COUNT][STR_COUNT];

#define tr(id) ((char*)lang_strings[current_lang][(id)])

#endif /* LANG_H */
