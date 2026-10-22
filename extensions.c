#include "header.h"

static void boom(void) {
    mciSendString("play \"Sons\\RandomBoom.mp3\" from 0", NULL, 0, NULL);
}
//extensions diverses (bonus/malus) ajoutées par kaly et stanles
void extension_bonus_golden_gem(int *score) {
    boom();
    clrscr();
    text_color(C_YELLOW);
    centrer_texte(" * GEMME D'OR * ");
    centrer_texte(" SCORE +100 ");
    *score += 100;
    Sleep(700);
    text_color(C_WHITE);
}
// --- FONCTIONS D'EXTENSIONS DIVERSES (BONUS/MALUS) ---
//elles fonctionnent toutes de la même manière : on affiche un message, on applique l'effet, on attend un peu, puis on remet la couleur du texte normale
void extension_bonus_time_freeze(int *secondes) {
    boom();
    clrscr();
    text_color(C_CYAN);
    centrer_texte(" * TIME FREEZE * ");
    centrer_texte(" TEMPS +30s ");
    *secondes += 30;
    Sleep(700);
    text_color(C_WHITE);
}

void extension_malus_meteorite(int *score) {
    boom();
    clrscr();
    text_color(C_RED);
    centrer_texte(" !!! METEORITE !!! ");
    centrer_texte(" SCORE -50 ");
    *score -= 50;
    if (*score < 0) {
        *score = 0;
    }
    Sleep(900);
    text_color(C_WHITE);
}

void extension_malus_black_hole(int *secondes) {
    boom();
    clrscr();
    text_color(C_MAGENTA);
    centrer_texte(" !!! TROU NOIR !!! ");
    centrer_texte(" TEMPS DIVISE / 2 ");
    if (*secondes > 10) {
        *secondes /= 2;
    }
    Sleep(1100);
    text_color(C_WHITE);
}