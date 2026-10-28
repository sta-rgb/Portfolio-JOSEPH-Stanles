#include "header.h"
#include <windows.h>

int main() {
    /* ===== CORRECTION AFFICHAGE CONSOLE (UTF-8 Windows) ===== */
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    //on  initialise la console en UTF-8
    init_console_utf8();
    system("mode con: cols=120 lines=45");
    SetConsoleTitle("SPACE MINER - ULTRA EDITION");
    srand(time(NULL));
    hide_cursor();

    Parametres params;
    charger_parametres(&params);
    init_audio(params);

    int offset_musique = 0;
    int lastTrackID = -1;

    while (1) {
        afficher_menu_principal(params); // Passe params pour vérifier si le bonus est débloqué
        
        char texte_musique[300];
        char nom_brut[100];
        
        if (params.trackID >= 1 && params.trackID <= NB_PISTES) {
            strcpy(nom_brut, PLAYLIST[params.trackID - 1]);
        } 
        else {
            strcpy(nom_brut, "Inconnu");
        }
        
        char* nom_propre = nom_brut; 
        if (strncmp(nom_brut, "Sons\\", 5) == 0) {
            nom_propre += 5; 
        }
        
        sprintf(texte_musique, " EN LECTURE : %s ...      SPACE MINER OST ...      ", nom_propre);

        int choix_fait = 0;
        vider_touches(); 

        while (!choix_fait) {
            if (params.trackID != lastTrackID) {
                lastTrackID = params.trackID;
                if (params.trackID >= 1 && params.trackID <= NB_PISTES) {
                    strcpy(nom_brut, PLAYLIST[params.trackID - 1]);
                }
                nom_propre = nom_brut;
                if (strncmp(nom_brut, "Sons\\", 5) == 0) {
                    nom_propre += 5;
                }
                sprintf(texte_musique, " EN LECTURE : %s ...      SPACE MINER OST ...      ", nom_propre);
            }

            afficher_barre_musique(30, 25, 60, offset_musique, texte_musique);
            
            fflush(stdout); 
            Sleep(50); 
            offset_musique++; 

            if (kbhit_secure()) {
                int c = getch_secure(); 
                switch (c) {
                    case '1': 
                        if (afficher_regles()) {
                            params.regles_acceptees = 1;
                            sauvegarder_parametres(&params);
                        }
                        choix_fait = 1; 
                        break;

                    case '2': 
                        if (params.regles_acceptees == 0) {
                            if (!afficher_regles()) {
                                choix_fait = 1; 
                                break; 
                            }
                            params.regles_acceptees = 1;
                            sauvegarder_parametres(&params);
                        }
                        lancer_nouvelle_partie(&params);
                        params.trackID = (rand() % NB_PISTES) + 1;
                        init_audio(params);
                        choix_fait = 1;
                        break;

                    case '3': 
                        charger_et_lancer_partie(&params);
                        params.trackID = (rand() % NB_PISTES) + 1;
                        init_audio(params);
                        choix_fait = 1;
                        break;

                    case '4': 
                        menu_options(&params);
                        choix_fait = 1;
                        break;

                    case '5': 
                        stop_audio();
                        clrscr();
                        text_color(C_CYAN);
                        printf("\n   Au revoir, Mineur %s !\n", params.pseudo);
                        Sleep(1000);
                        return 0;
                        
                    // [NOUVEAU] Gestion du Bonus
                    case '6':
                        if (params.a_fini_jeu == 1) {
                             lancer_niveau_bonus(&params);
                             choix_fait = 1;
                        }
                        break;
                }
            }
            verifier_playlist(&params);
        }
    }
    return 0;
}