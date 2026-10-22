#ifndef header_h_INCLUDED
#define header_h_INCLUDED

// --- BIBLIOTHEQUES ---
#include <stdio.h>
// pour les fonctions d'entrée/sortie standard
#include <stdlib.h>
// pour les fonctions de manipulation de chaînes
#include <string.h>
// pour les fonctions de gestion du temps
#include <time.h>
// pour les fonctions Windows spécifiques
#include <windows.h>
// pour les fonctions de manipulation de la console
#include <conio.h> 
// pour les fonctions de manipulation de fichiers
#include <io.h>
// pour les fonctions de manipulation de chaînes très complexes
#include <math.h> 
// Pour la gestion audio
#include <mmsystem.h>
// Lien avec la bibliothèque winmm.lib pour les fonctions audio
#pragma comment(lib, "winmm.lib")

// --- CONSTANTES ---
#define N 20
#define P 20
#define NB_VIES_MAX 3
#define FICHIER_CONFIG "config.txt"
#define NB_PISTES 11

// CONSTANTES BONUS (Ajout Mathis)
#define NIVEAU_BONUS 4
#define OBJECTIF_BONUS 5000

// --- COULEURS ---
#define C_BLACK 0
#define C_BLUE 1
#define C_GREEN 2
#define C_CYAN 3
#define C_RED 4
#define C_MAGENTA 5
#define C_BROWN 6
#define C_LIGHTGRAY 7
#define C_DARKGRAY 8
#define C_LIGHTBLUE 9
#define C_LIGHTGREEN 10
#define C_LIGHTCYAN 11
#define C_LIGHTRED 12
#define C_LIGHTMAGENTA 13
#define C_YELLOW 14
#define C_WHITE 15

// --- STRUCTURES ---
typedef struct {
    int volume;
    int trackID;
    int difficulte; 
    int regles_acceptees;
    int a_fini_jeu; // [NOUVEAU] Variable de progression (Check par Stanles)
    char pseudo[50];
} Parametres;

typedef struct {
    int grille[N][P];       
    int score;
    int vies;
    int niveau_actuel;      
    int coups_restants;
    int temps_restant;      
    int objectif_score;     
    int cursorX;
    int cursorY;
    int isSelected;         
    int selectedX;
    int selectedY;
    char pseudo[50];
    int difficulte_actuelle; // [NOUVEAU] Pour affichage HUD
} Partie;

extern const char* PLAYLIST[NB_PISTES];

// --- PROTOTYPES ---

// Outils (affichage.c)
void init_console_utf8(void);
void clrscr(void);
void gotoxy(int x, int y);
void text_color(int color);
void hide_cursor(void);
int getch_secure(void); 
int kbhit_secure(void);
void vider_touches(void);
void vider_entree_standard(void);
void centrer_texte(const char* texte);

// Affichage (affichage.c)
void afficher_menu_principal(Parametres p); 
int afficher_regles(void); 
void afficher_interface_pseudo(char* buffer_pseudo);
void afficher_hud(Partie *p);
void afficher_grille(Partie *p);
void afficher_barre_musique(int x, int y, int largeur, int offset, const char* texte);
void ecran_victoire(int score_final);
void ecran_defaite(int score_final);
void ecran_credits(void);
void afficher_objectif_niveau(Partie *p);

// Moteur (fonctionnement.c)
void lancer_nouvelle_partie(Parametres *params);
void lancer_niveau_bonus(Parametres *params); 
void charger_et_lancer_partie(Parametres *params);
int jouer_niveau(Partie *p, Parametres params, int niveau); 
void configurer_contrat(Partie *p, int niveau, int difficulte);
void remplir_grille(Partie *p);
void traiter_effet_bonus(Partie *p, int id_bonus);
void appliquer_gravite(Partie *p);
int verifier_matches(Partie *p, int mode_silencieux);
int simuler_coup(Partie *p, int x1, int y1, int x2, int y2);
void tenter_permutation(Partie *p);
void echanger_contenu(int *a, int *b);

// Paramètres (parametres.c)
void charger_parametres(Parametres *p);
void sauvegarder_parametres(Parametres *p);
void init_audio(Parametres p);
void stop_audio(void);
void updateVolume(int volumePercent);
void verifier_playlist(Parametres *p);
int charger_partie(Partie *p, char* pseudo);
void sauvegarder_partie(Partie *p);
int sauvegarde_existe(char* pseudo);
int selectionner_sauvegarde_menu(char* buffer_pseudo_choisi);

// Menus (menu_options.c)
void menu_options(Parametres *p);
void afficherInterfaceOptions(Parametres temp, int curseur);

// Extensions (extensions.c)
void extension_bonus_golden_gem(int *score);
void extension_bonus_time_freeze(int *secondes);
void extension_malus_meteorite(int *score);
void extension_malus_black_hole(int *secondes);

#endif // header_h_INCLUDED