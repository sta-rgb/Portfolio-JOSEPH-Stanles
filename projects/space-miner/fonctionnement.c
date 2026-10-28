#include "header.h"

// --- OUTILS INTERNES ---
void echanger_contenu(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// --- FONCTIONNALITES PRINCIPALES ---
void configurer_contrat(Partie *p, int niveau, int difficulte) {
    p->niveau_actuel = niveau;
    p->difficulte_actuelle = difficulte; 

    // CONFIGURATION DU NIVEAU BONUS
    // // Note (Mathis) : Le niveau OMEGA est hardcodé ici, ne pas toucher aux valeurs !
    if (niveau == NIVEAU_BONUS) {
        p->coups_restants = 60;
        p->temps_restant = 180; 
        p->objectif_score = OBJECTIF_BONUS; 
        return;
    }
    
    // MODIFICATION DES COUPS (HARDCORE)
    // // Demande de Stanles : Rééquilibrage drastique de la difficulté (15/10/5)
    if (difficulte == 1) { 
        p->coups_restants = 15; 
        p->temps_restant = 300; 
    } 
    else if (difficulte == 2) {
        p->coups_restants = 10; 
        p->temps_restant = 200; 
    } 
    else {
        p->coups_restants = 5; // // Mode "Impossible" validé par Yanis
        p->temps_restant = 120; 
    }

    if (niveau > 1) {
        p->temps_restant -= (niveau * 15);
        p->objectif_score = 300 * niveau * difficulte;
    } 
    else {
        p->objectif_score = 200 * difficulte;
    }
}

void remplir_grille(Partie *p) {
    // // Algo de remplissage standard. 
    // // TODO (Kaly) : Vérifier si on peut optimiser le rand() plus tard.
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            if (p->grille[i][j] == 0) {
                if (p->niveau_actuel == 1) {
                    p->grille[i][j] = (rand() % 5) + 1;
                } 
                else {
                    int r = rand() % 100;
                    if (r < 5) {
                        p->grille[i][j] = (rand() % 4) + 6; 
                    } 
                    else {
                        p->grille[i][j] = (rand() % 5) + 1; 
                    }
                }
            }
        }
    }
}

void traiter_effet_bonus(Partie *p, int id_bonus) {
    switch (id_bonus) {
        case 6: extension_bonus_golden_gem(&p->score); break;
        case 7: extension_bonus_time_freeze(&p->temps_restant); break;
        case 8: extension_malus_meteorite(&p->score); break;
        case 9: extension_malus_black_hole(&p->temps_restant); break;
    }
    vider_entree_standard();
    clrscr(); 
}

/* FONCTION CRITIQUE : verifier_matches
   // Auteur principal : Stanles (Logique de détection)
   // Optimisation : Mathis (Gestion des priorités de suppression)
   // Cette fonction doit être appelée après chaque mouvement ou gravité.
*/
int verifier_matches(Partie *p, int mode_silencieux) {
    int points = 0;
    int to_delete[N][P];
    // // Initialisation de la matrice de suppression à 0 (Nettoyage par Yanis)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            to_delete[i][j] = 0;
        }
    }
    
    int match_found = 0;
    int type_wipe = -1; // // Variable pour stocker le type de gemme à éradiquer (Effet Ligne de 6)

    // 1. CARRE 4x4
    // // Priorité absolue définie par Kaly : Le carré rapporte le plus et écrase les match-3
    for (int i = 0; i <= N - 4; i++) {
        for (int j = 0; j <= P - 4; j++) {
            int val = p->grille[i][j];
            if (val > 0 && val <= 9) {
                int is_square = 1;
                for (int r = 0; r < 4; r++) {
                    for (int c = 0; c < 4; c++) {
                        if (p->grille[i+r][j+c] != val) { 
                            is_square = 0; 
                            break; 
                        }
                    }
                    if (!is_square) {
                        break;
                    }
                }
                if (is_square) {
                    type_wipe = val; // // Active l'effet "Destruction massive"
                    match_found = 1;
                    points += 500; 
                }
            }
        }
    }

    // 2. CROIX 9 ITEMS
    // // Logique complexe ici : Stanles a galéré sur les indices i-2 et i+2, attention aux débordements !
    for (int i = 2; i < N - 2; i++) {
        for (int j = 2; j < P - 2; j++) {
            int val = p->grille[i][j];
            if (val > 0 && val <= 9) {
                if (p->grille[i-1][j] == val && p->grille[i-2][j] == val &&
                    p->grille[i+1][j] == val && p->grille[i+2][j] == val &&
                    p->grille[i][j-1] == val && p->grille[i][j-2] == val &&
                    p->grille[i][j+1] == val && p->grille[i][j+2] == val) 
                {
                    // // Marque toute la ligne et la colonne pour suppression
                    for (int c = 0; c < P; c++) {
                        to_delete[i][c] = 1;
                    }
                    for (int r = 0; r < N; r++) {
                        to_delete[r][j] = 1;
                    }
                    match_found = 1;
                    points += 300;
                }
            }
        }
    }

    // 3. LIGNE DE 6
    // // Scan Horizontal
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= P - 6; j++) {
            int val = p->grille[i][j];
            if (val > 0 && val <= 5) {
                int k;
                for (k = 1; k < 6; k++) {
                    if (p->grille[i][j+k] != val) {
                        break;
                    }
                }
                if (k == 6) { 
                    type_wipe = val; 
                    match_found = 1; 
                    points += 200; 
                }
            }
        }
    }
    // // Scan Vertical
    for (int j = 0; j < P; j++) {
        for (int i = 0; i <= N - 6; i++) {
            int val = p->grille[i][j];
            if (val > 0 && val <= 5) {
                int k;
                for (k = 1; k < 6; k++) {
                    if (p->grille[i+k][j] != val) {
                        break;
                    }
                }
                if (k == 6) { 
                    type_wipe = val; 
                    match_found = 1; 
                    points += 200; 
                }
            }
        }
    }

    // 4. MATCH 3 STANDARD
    // // Note (Mathis) : On passe ici en dernier pour éviter de "casser" les gros combos détectés plus haut.
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P - 2; j++) {
            int v = p->grille[i][j];
            if (v > 0 && v == p->grille[i][j+1] && v == p->grille[i][j+2]) {
                to_delete[i][j] = 1; 
                to_delete[i][j+1] = 1; 
                to_delete[i][j+2] = 1;
                match_found = 1; 
                points += 50;
            }
        }
    }
    for (int j = 0; j < P; j++) {
        for (int i = 0; i < N - 2; i++) {
            int v = p->grille[i][j];
            if (v > 0 && v == p->grille[i+1][j] && v == p->grille[i+2][j]) {
                to_delete[i][j] = 1; 
                to_delete[i+1][j] = 1; 
                to_delete[i+2][j] = 1;
                match_found = 1; 
                points += 50;
            }
        }
    }
    
    // // Phase de suppression réelle
    if (match_found) {
        if (mode_silencieux == 0) {
            afficher_hud(p);
            afficher_grille(p);
            gotoxy(40, 2); 
            text_color(C_LIGHTGREEN);
            printf("!!! COMBINAISON VALIDEE !!!");
            Sleep(1000); 
        }

        if (type_wipe != -1) {
            // // Cas spécial (Carré ou Ligne de 6) : On wipe tout un type de gemme
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < P; j++) {
                    if (p->grille[i][j] == type_wipe) {
                        p->grille[i][j] = 0;
                    }
                }
            }
        } 
        else {
            // // Cas standard (Match 3 ou Croix) : On supprime selon la matrice to_delete
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < P; j++) {
                    if (to_delete[i][j]) {
                        // // Check si c'est un bonus (ajouté par Kaly)
                        if (mode_silencieux == 0 && p->grille[i][j] >= 6 && p->grille[i][j] <= 9) {
                            traiter_effet_bonus(p, p->grille[i][j]);
                        }
                        p->grille[i][j] = 0;
                    }
                }
            }
        }
    }
    return points;
}

// // Fonction de simulation pour l'aide visuelle (curseur vert/rouge)
// // Code sécurisé par Yanis pour éviter les crashs hors bornes.
int simuler_coup(Partie *p, int x1, int y1, int x2, int y2) {
    if (x1 < 0 || x1 >= N || y1 < 0 || y1 >= P) {
        return 0;
    }
    if (x2 < 0 || x2 >= N || y2 < 0 || y2 >= P) {
        return 0;
    }
    
    Partie temp = *p;
    echanger_contenu(&temp.grille[x1][y1], &temp.grille[x2][y2]);
    return (verifier_matches(&temp, 1) > 0); 
}

void appliquer_gravite(Partie *p) {
    // // Algorithme de chute (Gravity Fall) implémenté par Stanles
    // // On parcourt de bas en haut pour faire tomber les éléments
    for (int j = 0; j < P; j++) {
        for (int i = N - 1; i > 0; i--) {
            if (p->grille[i][j] == 0) {
                for (int k = i - 1; k >= 0; k--) {
                    if (p->grille[k][j] != 0) {
                        echanger_contenu(&p->grille[i][j], &p->grille[k][j]);
                        break;
                    }
                }
            }
        }
    }
    remplir_grille(p);
}

void initialiser_niveau(Partie *p, int niveau, int difficulte) {
    configurer_contrat(p, niveau, difficulte);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            p->grille[i][j] = 0;
        }
    }
    
    // // Boucle pour s'assurer qu'on ne commence pas avec des combos déjà faits
    do { 
        remplir_grille(p); 
    } while (verifier_matches(p, 1) > 0); 
    
    p->score = 0;
    p->cursorX = N/2; 
    p->cursorY = P/2;
    p->isSelected = 0;
}

void tenter_permutation(Partie *p) {
    int cX = p->cursorX; 
    int cY = p->cursorY;
    int sX = p->selectedX; 
    int sY = p->selectedY;

    // // Vérification d'adjacence (Manhattan distance == 1)
    if (abs(cX - sX) + abs(cY - sY) == 1) {
        echanger_contenu(&p->grille[cX][cY], &p->grille[sX][sY]);
        int gain = verifier_matches(p, 0); 
        
        if (gain > 0) {
            // // Coup Valide !
            p->score += gain;
            p->coups_restants--;
            // // Réaction en chaîne (Chain reaction) gérée par Mathis
            do {
                appliquer_gravite(p);
                gain = verifier_matches(p, 0); 
                p->score += gain;
            } while (gain > 0);
            vider_touches();
        } 
        else {
            // // Coup Invalide : On remet comme avant (Rollback)
            afficher_grille(p);
            Sleep(250);
            echanger_contenu(&p->grille[cX][cY], &p->grille[sX][sY]);
            printf("\a"); 
        }
    }
    p->isSelected = 0;
}

// // BOUCLE PRINCIPALE DU JEU (GAME LOOP)
// // Supervision : Yanis (Gestion temps réel + Input)
int jouer_niveau(Partie *p, Parametres params, int niveau) {
    initialiser_niveau(p, niveau, params.difficulte);
    afficher_objectif_niveau(p);
    
    clrscr(); 
    int running = 1;
    time_t start_time = time(NULL);
    time_t last_tick = start_time;
    vider_entree_standard();

    while (running && p->vies > 0) {
        verifier_playlist(&params);
        
        // // Gestion du Timer (1 seconde réelle = 1 point de temps en moins)
        time_t now = time(NULL);
        if (now > last_tick) {
            p->temps_restant--;
            last_tick = now;
        }

        gotoxy(0, 0); 
        afficher_hud(p);
        afficher_grille(p);
        
        // // Conditions de défaite (Check par Kaly)
        if (p->temps_restant <= 0 || p->coups_restants <= 0) {
            p->vies--;
            ecran_defaite(p->score);
            if (p->vies > 0) {
                // // Restart du niveau si vies restantes
                initialiser_niveau(p, niveau, params.difficulte);
                start_time = time(NULL);
                last_tick = start_time;
                clrscr(); 
            } 
            else {
                return 0; // Game Over définitif
            }
            continue;
        }

        // // Condition de Victoire
        if (p->score >= p->objectif_score) {
            ecran_victoire(p->score);
            return 1; 
        }

        // // Gestion des Inputs (Non-bloquant grâce à kbhit_secure)
        if (kbhit_secure()) {
            int c = getch_secure();
            switch (c) {
                case 'z': case 'Z': if(p->cursorX > 0) p->cursorX--; break;
                case 's': case 'S': if(p->cursorX < N-1) p->cursorX++; break;
                case 'q': case 'Q': if(p->cursorY > 0) p->cursorY--; break;
                case 'd': case 'D': if(p->cursorY < P-1) p->cursorY++; break;
                case 32: // ESPACE
                    if (!p->isSelected) {
                        p->isSelected = 1; // // Sélectionner 1er item
                        p->selectedX = p->cursorX;
                        p->selectedY = p->cursorY;
                    } 
                    else {
                        tenter_permutation(p); // // Tenter avec 2eme item
                    }
                    break;
                case 'm': case 'M': 
                    // // Menu Pause basique
                    clrscr(); 
                    printf("\n\n");
                    centrer_texte("PAUSE");
                    printf("\n");
                    centrer_texte("1 : Continuer   2 : Sauvegarder et Quitter");
                    int choix = 0;
                    while(!choix) {
                        if(kbhit()) {
                            char k = getch_secure();
                            if (k=='1') choix=1;
                            if (k=='2') { sauvegarder_partie(p); return -1; }
                        }
                    }
                    clrscr(); 
                    break;
            }
        }
        Sleep(50); // // Petite pause pour ne pas surcharger le CPU
    }
    return 0;
}

void lancer_nouvelle_partie(Parametres *params) {
    Partie p;
    memset(&p, 0, sizeof(Partie)); 
    
    int pseudo_valide = 0;
     do {
        afficher_interface_pseudo(p.pseudo);
        vider_entree_standard();
        // // Vérification existence sauvegarde (Yanis)
        if (sauvegarde_existe(p.pseudo)) {
            clrscr();
            text_color(C_RED);
            printf("\n\n   ATTENTION: '%s' existe deja.\n", p.pseudo);
            text_color(C_WHITE);
            // [MODIFIE] Ajout option retour
            printf("   [1] ECRASER   [2] CHANGER   [3] RETOUR MENU\n");
            
            int c = 0;
            while (c != '1' && c != '2' && c != '3') {
                if(kbhit()) {
                    c = getch_secure();
                }
            }
            if (c == '1') {
                pseudo_valide = 1;
            }
            if (c == '3') {
                return; // // Retour demande par le prof, implémenté par Mathis
            }
        } 
        else {
            pseudo_valide = 1;
        }
    } while (!pseudo_valide);

    strcpy(params->pseudo, p.pseudo);
    sauvegarder_parametres(params);

    // SELECTION DIFFICULTE (AVEC OPTION RETOUR)
    int selection_faite = 0;
    int diff = 1; 
    
     do {
        clrscr();
        printf("\n\n\n\n");
        text_color(C_CYAN);
        centrer_texte("CHOIX DE LA DIFFICULTE");
        printf("\n");
        
        if (diff == 1) { 
            text_color(C_LIGHTGREEN); 
            centrer_texte("->  1. FACILE (15 Coups)  <-"); 
        } 
        else { 
            text_color(C_WHITE); 
            centrer_texte("    1. FACILE (15 Coups)    "); 
        }
        
        if (diff == 2) { 
            text_color(C_LIGHTGREEN); 
            centrer_texte("->  2. MOYEN  (10 Coups)  <-"); 
        } 
        else { 
            text_color(C_WHITE); 
            centrer_texte("    2. MOYEN  (10 Coups)    "); 
        }
        
        if (diff == 3) { 
            text_color(C_LIGHTGREEN); 
            centrer_texte("->  3. DUR    (05 Coups)  <-"); 
        } 
        else { 
            text_color(C_WHITE); 
            centrer_texte("    3. DUR    (05 Coups)    "); 
        }

        // [NOUVEAU] Option Retour
        if (diff == 4) { 
            text_color(C_RED); 
            centrer_texte("->  4. RETOUR MENU        <-"); 
        } 
        else { 
            text_color(C_DARKGRAY); 
            centrer_texte("    4. RETOUR MENU          "); 
        }
        
        printf("\n\n");
        text_color(C_LIGHTGRAY);
        centrer_texte("(Z/S : Haut/Bas  |  ENTREE : Valider)");
        
        int c = getch_secure();
        if (c == 'z' || c == 'Z') { 
            if (diff > 1) diff--; 
        }
        else if (c == 's' || c == 'S') { 
            if (diff < 4) diff++; 
        } 
        else if (c == 13 || c == 10 || c == 32) {
            selection_faite = 1;
        }
    } while (!selection_faite);
    
    // Si choix = 4, on quitte sans lancer le jeu
    if (diff == 4) {
        return;
    }

    params->difficulte = diff;
    p.vies = NB_VIES_MAX; 

    int res = 0;
    // Niveau 1
    res = jouer_niveau(&p, *params, 1);
    if (res != 1) {
        return;
    }

    // Niveau 2
    res = jouer_niveau(&p, *params, 2);
    if (res != 1) {
        return;
    }

    // Niveau 3
    res = jouer_niveau(&p, *params, 3);
    if (res == 1) {
        // // Victoire totale : Déblocage du bonus (Idée de Stanles)
        params->a_fini_jeu = 1;
        sauvegarder_parametres(params);
        ecran_credits();
    }
}
// NOUVEAU dans le code et à bien implémenter plus tard en fonction de lancement du niveau bonus OMEGA
void lancer_niveau_bonus(Parametres *params) {
    Partie p;
    memset(&p, 0, sizeof(Partie));
    strcpy(p.pseudo, params->pseudo);
    p.vies = 1; 
    
    params->difficulte = 4;
    
    jouer_niveau(&p, *params, NIVEAU_BONUS);
}

void charger_et_lancer_partie(Parametres *params) {
    char pseudoChoisi[50];
    int res = selectionner_sauvegarde_menu(pseudoChoisi);
    if (res == 1) { 
        Partie p;
        if (charger_partie(&p, pseudoChoisi)) {
            if (p.vies <= 0) {
                printf("\n   Partie terminee (0 vies). Impossible de reprendre.\n");
                Sleep(2000);
            } 
            else {
                if (p.niveau_actuel == 0) {
                    p.niveau_actuel = 1;
                }
                jouer_niveau(&p, *params, p.niveau_actuel);
            }
        }
    } 
    else if (res == 0) {
        clrscr();
        printf("\n\n\n\n");
        text_color(C_RED);
        centrer_texte("AUCUNE SAUVEGARDE TROUVEE !");
        text_color(C_WHITE);
        Sleep(2000);
    }
}

void moteur_de_jeu(Partie *p, Parametres params) {
    jouer_niveau(p, params, p->niveau_actuel);
}