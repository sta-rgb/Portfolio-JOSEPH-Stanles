#include "header.h"

void afficherInterfaceOptions(Parametres temp, int curseur) {
    clrscr();
    gotoxy(0, 0); 
    printf("\n\n\n");
    text_color(C_CYAN);
    centrer_texte("=== PARAMETRES DE SPACE MINER ===");
    printf("\n");

    // VOLUME
    if (curseur == 0) { 
        text_color(C_YELLOW); 
        printf("   -> "); 
    } else { 
        text_color(C_DARKGRAY); 
        printf("      "); 
    }
    
    text_color(C_WHITE);
    printf("VOLUME DU JEU     : [");
    
    for(int i=0; i<10; i++) {
        if (i < temp.volume/10) { 
            text_color(C_GREEN); 
            printf("#"); 
        } else { 
            text_color(C_DARKGRAY); 
            printf("-"); 
        }
    }
    text_color(C_WHITE);
    printf("] %d%%   \n", temp.volume); 

    // MUSIQUE
    if (curseur == 1) { 
        text_color(C_YELLOW); 
        printf("   -> "); 
    } else { 
        text_color(C_DARKGRAY); 
        printf("      "); 
    }
    
    text_color(C_WHITE);
    const char* nom_piste = PLAYLIST[temp.trackID - 1];
    
    if (strncmp(nom_piste, "Sons\\", 5) == 0) {
        nom_piste += 5;
    }
    
    printf("MUSIQUE EN LECTURE    : Piste %d / %d (%s)   \n", temp.trackID, NB_PISTES, nom_piste);

    // DIFFICULTE
    if (curseur == 2) { 
        text_color(C_YELLOW); 
        printf("   -> "); 
    } else { 
        text_color(C_DARKGRAY); 
        printf("      "); 
    }
    
    text_color(C_WHITE);
    char* diff_str;
    if (temp.difficulte == 1) { 
        diff_str = "FACILE"; 
    } else if (temp.difficulte == 2) { 
        diff_str = "MOYEN"; 
    } else { 
        diff_str = "DIFFICILE"; 
    }
    
    printf("DIFFICULTE : %s   \n\n", diff_str);

    // SAUVEGARDER
    if (curseur == 3) { 
        text_color(C_GREEN); 
        printf("   -> SAUVEGARDER ET QUITTER                  \n"); 
    } else { 
        text_color(C_WHITE); 
        printf("      SAUVEGARDER ET QUITTER                  \n"); 
    }

    // CHANGER PSEUDO
    if (curseur == 4) { 
        text_color(C_YELLOW); 
        printf("   -> CHANGER LE PSEUDO                       \n"); 
    } else { 
        text_color(C_WHITE); 
        printf("      CHANGER LE PSEUDO                       \n"); 
    }

    // REINITIALISER
    if (curseur == 5) { 
        text_color(C_RED); 
        printf("   -> REINITIALISER LES PARAMETRES            \n"); 
    } else { 
        text_color(C_WHITE); 
        printf("      REINITIALISER LES PARAMETRES            \n"); 
    }

    // RETOUR
    if (curseur == 6) { 
        text_color(C_MAGENTA); 
        printf("   -> RETOUR AU MENU PRINCIPAL                \n"); 
    } else { 
        text_color(C_WHITE); 
        printf("      RETOUR AU MENU PRINCIPAL                \n"); 
    }

    text_color(C_LIGHTGRAY);
    printf("\n\n   (Z/S : Haut/Bas | Q/D : Modifier | Entree : Valider)   ");
}

void menu_options(Parametres *p) {
    int curseur = 0;
    Parametres temp = *p;
    
    clrscr(); 
    vider_entree_standard(); 

    while (1) {
        afficherInterfaceOptions(temp, curseur);
        int touche = getch_secure();

        // NAVIGATION
        if (touche == 'z' || touche == 'Z') {
            if (curseur > 0) {
                curseur--;
            }
        }
        if (touche == 's' || touche == 'S') {
            if (curseur < 6) {
                curseur++;
            }
        }
        if (touche == 224 || touche == 0) {
            touche = getch_secure();
            if (touche == 72 && curseur > 0) {
                curseur--;
            }
            if (touche == 80 && curseur < 6) {
                curseur++;
            }
        }

        // MODIFICATION
        if (curseur <= 2) {
            int sens = 0;
            if (touche == 'q' || touche == 'Q' || touche == 75) {
                sens = -1;
            }
            if (touche == 'd' || touche == 'D' || touche == 77) {
                sens = 1;
            }

            if (sens != 0) {
                switch(curseur) {
                    case 0: 
                        temp.volume += (10 * sens);
                        if (temp.volume > 100) temp.volume = 100;
                        if (temp.volume < 0) temp.volume = 0;
                        updateVolume(temp.volume); 
                        break;
                        
                    case 1: 
                        temp.trackID += sens;
                        if (temp.trackID > NB_PISTES) temp.trackID = 1;
                        if (temp.trackID < 1) temp.trackID = NB_PISTES;
                        init_audio(temp); 
                        break;
                        
                    case 2: 
                        temp.difficulte += sens;
                        if (temp.difficulte > 3) temp.difficulte = 1;
                        if (temp.difficulte < 1) temp.difficulte = 3; 
                        break;
                }
            }
        }

        // VALIDATION
        if (touche == 13) {
            if (curseur == 3) { 
                *p = temp; 
                sauvegarder_parametres(p); 
                clrscr(); 
                text_color(C_GREEN);
                printf("\n\n   PARAMETRES SAUVEGARDES !\n"); 
                Sleep(1000); 
                break; 
            }
            else if (curseur == 4) { 
                char np[50]; 
                afficher_interface_pseudo(np);
                strcpy(temp.pseudo, np); 
                clrscr(); 
            }
            else if (curseur == 5) { 
                temp.volume = 50; 
                temp.trackID = 1; 
                temp.difficulte = 1;
                strcpy(temp.pseudo, "Joueur"); 
                init_audio(temp);
                text_color(C_RED); 
                printf("\n   RESET OK !\n"); 
                Sleep(800);
            }
            else if (curseur == 6) { 
                init_audio(*p); 
                break; 
            }
        }
    }
}