#include "header.h"

// --- OUTILS ---
void init_console_utf8(void) { 
    system("chcp 65001 > nul");
    system("mode con: cols=120 lines=45");
}
// Efface l'écran
void clrscr(void) { 
    system("cls"); 
}
// Déplace le curseur à la position (x, y)
void gotoxy(int x, int y) {
    COORD c; 
    c.X = (SHORT)x; 
    c.Y = (SHORT)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
// Change la couleur du texte
void text_color(int color) { 
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color); 
}
// Cache le curseur
void hide_cursor(void) {
    CONSOLE_CURSOR_INFO info; 
    info.dwSize = 100; 
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

int getch_secure(void) { 
    return _getch(); 
}

int kbhit_secure(void) { 
    return _kbhit(); 
}

void vider_touches(void) {
    while (kbhit()) { 
        getch(); 
    }
}

void vider_entree_standard(void) {
    fflush(stdin);
}

void centrer_texte(const char* texte) {
    int padding = (120 - strlen(texte)) / 2;
    if (padding < 0) {
        padding = 0;
    }
    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", texte);
}

// --- MENUS ---

void afficher_menu_principal(Parametres p) {
    clrscr();
    text_color(C_CYAN);
    // // ASCII ART réalisé par Kaly
    printf("\n");
    centrer_texte("   _____  _____  _______  _____  ______ ");
    centrer_texte("  / ____||  __ \\|  __   |/ ____||  ____|");
    centrer_texte(" | (___  | |__) || |__| || |     | |__   ");
    centrer_texte("  \\___ \\ |  ___/ |  __  || |     |  __|  ");
    centrer_texte("  ____) || |     | |  | || |____ | |____ ");
    centrer_texte(" |_____/ |_|     |_|  |_| \\_____||______|");
    printf("\n");
    centrer_texte("  __  __  _____  _    _  ______  _____  ");
    centrer_texte(" |  \\/  ||_   _|| \\ | ||  ____||  __ \\ ");
    centrer_texte(" | \\  / |  | |  |  \\| || |__   | |__) |");
    centrer_texte(" | |\\/| |  | |  | . ` ||  __|  |  _  / ");
    centrer_texte(" | |  | | _| |_ | |\\  || |____ | | \\ \\ ");
    centrer_texte(" |_|  |_||_____||_| \\_||______||_|  \\_\\");
    printf("\n");
    text_color(C_YELLOW);
    centrer_texte("=== ULTRA MINER EDITION ===");
    text_color(C_WHITE);
    printf("\n\n");
    centrer_texte(" 1  :  LIRE LES REGLES DU JEU    ");
    centrer_texte(" 2  :  LANCER UNE NOUVELLE PARTIE");
    centrer_texte(" 3  :  REPRENDRE UNE PARTIE      ");
    centrer_texte(" 4  :  OPTIONS DE SPACE MINER    ");
    centrer_texte(" 5  :  QUITTER SPACE MINER       ");
    
    // // Option cachée (Stanles) : N'apparaît que si le jeu est fini
    if (p.a_fini_jeu == 1) {
        printf("\n");
        text_color(C_LIGHTMAGENTA);
        centrer_texte(" 6  :  !!! CONTRAT BONUS (OMEGA) !!! ");
    }

    printf("\n\n");
    gotoxy(50, 29); 
    text_color(C_LIGHTGREEN);
    printf("Votre choix : ");
}

int afficher_regles(void) {
    clrscr();
    int choix = 0;
    printf("=====================================================================================================\n");
    printf("                                             REGLES DU JEU SPACE MINER                               \n");
    printf("=====================================================================================================\n");
    printf("\n");
    printf("   1. Le but du jeu est de miner un maximum de minerais.\n");
    printf("\n");
    printf("   2. Pour reussir les niveaux ils vous faut remplir des contrats de mission constitue de : \n");
    printf("          - Nombre de minerais a miner. \n");
    printf("          - Nombre maximum de coups. \n");
    printf("          - Temps impartie defini. \n");
    printf("\n");
    
    printf("   Conseil: FIGURES SPECIALES :\n");
    printf("      - Ligne de 6 (H/V) : Elimine TOUS les items de ce type sur le tableau.\n");
    printf("      - Croix de 9 : Elimine la ligne ET la colonne entiere.\n");
    printf("      - Carre 4x4 : Elimine tous les items contenus dans le carre.\n");

    printf("   3. Utilisez les touches ZQSD pour se deplacer et la touche ESPACE pour miner. \n");
    printf("\n");
    printf("   4. Faire attention de ne pas epuiser ses 3 vies ( sinon game over ! ).\n");
    printf("          - Vous perdez une vie lorsque le temps defini est ecoule, \n");
    printf("            et que votre contrat de mission n'est pas rempli.\n");
    printf("\n");
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("\n");
    printf("    Acceptez-vous ces regles pour jouer ?\n");
    printf("\n");
    printf("      1 : OUI, je veux aller miner !\n");
    printf("      2 : RETOUR au menu principal.\n"); 
    printf("      3 : NON, je quitte le jeu.\n");     
    printf("\n");
    printf("    Votre choix : ");

    if (scanf("%d", &choix) != 1) {
        while(getchar() != '\n'); 
    }
    vider_entree_standard();

    if (choix == 3) {
        exit(0);
    }
    if (choix == 2) {
        return 0; // Retourne 0 pour dire "ne pas lancer, retour menu"
    }
    return 1;
}

void afficher_interface_pseudo(char* buffer_pseudo) {
    clrscr();
    printf("\n\n\n\n");
    text_color(C_CYAN);
    centrer_texte("+=============================================+");
    centrer_texte("|           IDENTIFICATION REQUISE            |");
    centrer_texte("+=============================================+");
    centrer_texte("|                                             |");
    centrer_texte("|      Veuillez entrer votre Pseudo :         |");
    centrer_texte("|                                             |");
    centrer_texte("|          > _____________________            |");
    centrer_texte("|                                             |");
    centrer_texte("+=============================================+");
    gotoxy(65, 10); 
    text_color(C_YELLOW);
    
    vider_touches();
    fflush(stdin); 
    
    if (fgets(buffer_pseudo, 49, stdin) != NULL) {
        buffer_pseudo[strcspn(buffer_pseudo, "\n")] = 0; 
        for (int i = 0; buffer_pseudo[i]; i++) {
            if (buffer_pseudo[i] == ' ') {
                buffer_pseudo[i] = '_';
            }
        }
        if (strlen(buffer_pseudo) == 0) {
            strcpy(buffer_pseudo, "Joueur");
        }
    } 
    else {
        strcpy(buffer_pseudo, "Joueur");
    }
    
    text_color(C_GREEN);
    gotoxy(0, 15);
    centrer_texte("Pseudo enregistre !");
    Sleep(500);
}

void afficher_hud(Partie *p) {
    gotoxy(0, 0); 
    text_color(C_YELLOW);
    
    char diffStr[20];
    if (p->difficulte_actuelle == 1) {
        strcpy(diffStr, "FACILE");
    }
    else if (p->difficulte_actuelle == 2) {
        strcpy(diffStr, "MOYEN");
    }
    else if (p->difficulte_actuelle == 3) {
        strcpy(diffStr, "DIFFICILE");
    }
    else {
        strcpy(diffStr, "OMEGA"); 
    }

    printf(" JOUEUR: %-10s | NIV: %d | MODE: %-7s | SCORE: %-6d | VIES: ", 
           p->pseudo, p->niveau_actuel, diffStr, p->score);
    
    text_color(C_RED);
    for (int i = 0; i < NB_VIES_MAX; i++) {
        if (i < p->vies) {
            printf("O "); 
        }
        else {
            printf("_ ");
        }
    }
    
    text_color(C_CYAN);
    
    // // Ajout calcul reste à faire pour demande "items restant à éliminer" (Mathis)
    int reste_a_faire = p->objectif_score - p->score;
    if (reste_a_faire < 0) {
        reste_a_faire = 0;
    }

    printf("\n CONTRAT: %02d Coups | %03d Sec | Obj: %d pts | RESTE: %d pts\n", 
           p->coups_restants, p->temps_restant, p->objectif_score, reste_a_faire);
    
    text_color(C_WHITE);
    for (int i = 0; i < 120; i++) {
        printf("_");
    }
}

/* AFFICHAGE DE LA GRILLE (Complexité : Elevée)
   // Responsable : Kaly (Choix des emojis)
   // Intégration Curseur : Mathis (Gestion des couleurs de fond)
   // Aide Visuelle (Vert/Rouge) : Yanis
*/
void afficher_grille(Partie *p) {
    int offsetX = 30; 
    int offsetY = 4; 

    gotoxy(offsetX, offsetY);
    text_color(C_WHITE);
    printf("+");
    for (int j = 0; j < P; j++) {
        printf("---"); 
    }
    printf("+");

    for (int i = 0; i < N; i++) {
        gotoxy(offsetX, offsetY + 1 + i); 
        printf("|"); 

        for (int j = 0; j < P; j++) {
            int isCursor = (i == p->cursorX && j == p->cursorY);
            int isSelected = (p->isSelected && i == p->selectedX && j == p->selectedY);

            // // Gestion des couleurs du curseur (Logic by Mathis)
            if (isCursor) {
                if (p->isSelected) {
                    // // Prédiction du coup (Vert=Bon, Rouge=Mauvais) - Ajout Yanis
                    if (simuler_coup(p, p->selectedX, p->selectedY, i, j)) {
                        text_color(BACKGROUND_GREEN | C_WHITE);
                    }
                    else {
                        text_color(BACKGROUND_RED | C_WHITE);
                    }
                } 
                else {
                    text_color(BACKGROUND_BLUE | C_WHITE);
                }
            } 
            else if (isSelected) {
                 text_color(BACKGROUND_GREEN | C_BLACK);
            } 
            else {
                text_color(C_BLACK); 
            }
            
            // // Couleurs des gemmes pour compatibilité Console (Stanles)
            int val = p->grille[i][j];
            if (!isCursor && !isSelected) {
                 switch(val) {
                    case 1: text_color(C_RED); break;        
                    case 2: text_color(C_BLUE); break;       
                    case 3: text_color(C_GREEN); break;      
                    case 4: text_color(C_YELLOW); break;     
                    case 5: text_color(C_MAGENTA); break;    
                    default: text_color(C_WHITE); break;
                 }
            }

            // // Mapping des Emojis (Choix artistique de Kaly)
            const char *sym;
            switch (val) {
                case 0: sym = " ";  break;
                case 1: sym = "■"; break;
                case 2: sym = "●"; break;
                case 3: sym = "▲"; break;
                case 4: sym = "◆"; break;
                case 5: sym = "★"; break;
                case 6: sym = "♛"; break; // Bonus Golden
                case 7: sym = "✖"; break; // Malus Time
                case 8: sym = "✦"; break; // Bonus Time
                case 9: sym = "░"; break; // Malus Points
                default: sym = " "; break;
            }
            printf(" %s ", sym);
            text_color(C_WHITE); 
        }
        printf("|"); 
    }

    gotoxy(offsetX, offsetY + 1 + N);
    text_color(C_WHITE);
    printf("+");
    for (int j = 0; j < P; j++) {
        printf("---");
    }
    printf("+");

    gotoxy(offsetX, offsetY + N + 2);
    text_color(C_LIGHTGRAY);
    printf("[ZQSD] Deplacer   [ESPACE] Selection   [M] Pause");
}

/* SCROLLING TEXT BAR (Animation)
   // Auteur : Stanles
   // Note : Utilise un offset modulo pour l'effet de défilement infini
*/
void afficher_barre_musique(int x, int y, int largeur, int offset, const char* texte) {
    int len_reelle = strlen(texte);
    if (len_reelle == 0) {
        return;
    }
    int len_virtuelle = len_reelle + 15;
    
    gotoxy(x, y);
    for (int k = 0; k < largeur + 6; k++) {
        printf(" "); 
    }
    
    gotoxy(x, y);
    text_color(C_LIGHTMAGENTA); 
    printf("\U0001F3B5 "); 
    text_color(C_CYAN);
    
    for (int i = 0; i < largeur; i++) {
        // // Calcul de l'index circulaire (Mathis m'a aidé sur le modulo)
        int index = (offset + i) % len_virtuelle;
        if (index < len_reelle) {
            printf("%c", texte[index]);
        }
        else {
            printf(" ");
        }
    }
    text_color(C_LIGHTMAGENTA); 
    printf(" \U0001F3B5"); 
    text_color(C_WHITE);
}

void ecran_victoire(int score_final) {
    clrscr();
    printf("\n\n\n\n");
    text_color(C_GREEN);
    centrer_texte("NIVEAU REUSSI !");
    printf("\n");
    text_color(C_WHITE);
    char buffer[50];
    sprintf(buffer, "SCORE FINAL : %d", score_final);
    centrer_texte(buffer);
    getch_secure();
}

void ecran_defaite(int score_final) {
    clrscr();
    printf("\n\n\n\n");
    text_color(C_RED);
    centrer_texte("E C H E C  !");
    printf("\n");
    text_color(C_WHITE);
    centrer_texte("Contrat non rempli. Vous perdez une vie.");
    getch_secure();
}

void ecran_credits(void) {
    clrscr();
    printf("\n\n\n\n");
    text_color(C_LIGHTMAGENTA);
    centrer_texte("MERCI D'AVOIR JOUE !");
    printf("\n");
    text_color(C_YELLOW);
    centrer_texte("Felicitation ! Vous avez debloque le CONTRAT BONUS.");
    centrer_texte("Retournez au menu principal pour l'essayer (Touche 6).");
    getch_secure();
}

void afficher_objectif_niveau(Partie *p) {
    clrscr();
    printf("\n\n");
    text_color(C_CYAN);
    centrer_texte("=== MISSION EN COURS ===");
    printf("\n");
    text_color(C_WHITE);
    char buf[100];
    
    if (p->niveau_actuel == NIVEAU_BONUS) {
        sprintf(buf, "NIVEAU BONUS - OMEGA");
    } 
    else {
        sprintf(buf, "Niveau : %d", p->niveau_actuel);
    }
    centrer_texte(buf);
    
    sprintf(buf, "Objectif : %d pts", p->objectif_score);
    centrer_texte(buf);
    printf("\n");
    centrer_texte("COMBINAISONS :");
    centrer_texte("- Ligne de 6 (H/V) : Destruction Type");
    centrer_texte("- Carre 4x4        : Destruction Type");
    centrer_texte("- Croix de 9       : Destruction Ligne + Colonne");
    printf("\n\n");
    centrer_texte("[APPUIE SUR UNE TOUCHE]");
    getch_secure();
    clrscr(); 
}