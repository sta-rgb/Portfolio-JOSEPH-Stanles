#include "header.h"
//ici, on définit la playlist complète des musiques disponibles dans le jeu
const char* PLAYLIST[NB_PISTES] = {
    "Sons\\StillDre.mp3",
    "Sons\\Randomgames1.mp3",
    "Sons\\50cent.mp3",
    "Sons\\buffyvampires.mp3",
    "Sons\\nokia.mp3",
    "Sons\\RandomBoom.mp3",
    "Sons\\Randommetro1.mp3",
    "Sons\\Randomwaves2.mp3",
    "Sons\\somebodyme.mp3",
    "Sons\\tweaker.mp3",
    "Sons\\zeze.mp3"
};
// --- FONCTIONS PRINCIPALES ---
// Gestion des paramètres utilisateur et audio, réalisation des sauvegardes grace à des fichiers binaires
void charger_parametres(Parametres *p) {
    FILE *f = fopen(FICHIER_CONFIG, "rb");
    if (f) {
        fread(p, sizeof(Parametres), 1, f);
        fclose(f);
    } 
    else {
        // Valeurs par défaut
        p->volume = 50; 
        p->trackID = 1; 
        p->difficulte = 1;
        p->regles_acceptees = 0; 
        p->a_fini_jeu = 0; // [NOUVEAU] Par défaut, jeu non fini
        strcpy(p->pseudo, "Joueur");
    }
}
// Sauvegarde des paramètres utilisateur dans un fichier binaire
void sauvegarder_parametres(Parametres *p) {
    FILE *f = fopen(FICHIER_CONFIG, "wb");
    if (f) { 
        fwrite(p, sizeof(Parametres), 1, f); 
        fclose(f); 
    }
}

void updateVolume(int volumePercent) {
    char vol[50];
    sprintf(vol, "setaudio bgm volume to %d", volumePercent * 10);
    mciSendString(vol, NULL, 0, NULL);
}

void init_audio(Parametres p) {
    mciSendString("close all", NULL, 0, NULL);
    char cmd[256];
    sprintf(cmd, "open \"%s\" type mpegvideo alias bgm", PLAYLIST[p.trackID - 1]);
    
    if (mciSendString(cmd, NULL, 0, NULL) == 0) {
        mciSendString("play bgm", NULL, 0, NULL);
        updateVolume(p.volume);
    }
}

void verifier_playlist(Parametres *p) {
    char status[128];
    mciSendString("status bgm mode", status, 128, NULL);
    if (strcmp(status, "stopped") == 0) {
        p->trackID++;
        if (p->trackID > NB_PISTES) { 
            p->trackID = 1; 
        }
        init_audio(*p);
        sauvegarder_parametres(p);
    }
}

void stop_audio(void) { 
    mciSendString("close all", NULL, 0, NULL); 
}

int charger_partie(Partie *p, char* pseudo) {
    char f[100]; 
    sprintf(f, "save_%s.dat", pseudo);
    FILE *file = fopen(f, "rb");
    if (file) { 
        if (fread(p, sizeof(Partie), 1, file) == 1) { 
            fclose(file); 
            return 1; 
        }
        fclose(file);
    }
    return 0; 
}

void sauvegarder_partie(Partie *p) {
    char f[100]; 
    sprintf(f, "save_%s.dat", p->pseudo);
    FILE *file = fopen(f, "wb");
    if (file) { 
        fwrite(p, sizeof(Partie), 1, file); 
        fclose(file); 
    }
}

int sauvegarde_existe(char* pseudo) {
    char f[100];
    sprintf(f, "save_%s.dat", pseudo);
    FILE *file = fopen(f, "rb");
    if (file) { 
        fclose(file); 
        return 1; 
    }
    return 0;
}

/* GESTION DES SAUVEGARDES (Menu Dynamique)
   // Responsable : Stanles (Lecture des fichiers binaires)
   // Utilisation de l'API Windows FindFirstFile/FindNextFile
*/
int selectionner_sauvegarde_menu(char* buffer_pseudo_choisi) {
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile("save_*.dat", &findData);
    
    typedef struct {
        char pseudo[50];
        int score;
        int vies;
        int niveau;
    } InfoSave;
    
    InfoSave liste[50];
    int count = 0;

    if (hFind == INVALID_HANDLE_VALUE) {
        return 0;
    }

     // Boucle de lecture des fichiers save_X.dat
    do {
        char nomFichier[100];
        strcpy(nomFichier, findData.cFileName);
        
        char pseudo_temp[50];
        int len = strlen(nomFichier);
        if (len > 9) {
            strncpy(pseudo_temp, nomFichier + 5, len - 9);
            pseudo_temp[len - 9] = '\0';
            
            FILE *f = fopen(nomFichier, "rb");
            if (f) {
                Partie tempP;
                // // Vérification intégrité fichier (Mathis)
                if (fread(&tempP, sizeof(Partie), 1, f) == 1) {
                    strcpy(liste[count].pseudo, pseudo_temp);
                    liste[count].score = tempP.score;
                    liste[count].vies = tempP.vies;
                    liste[count].niveau = tempP.niveau_actuel;
                    count++;
                }
                fclose(f);
            }
        }
    } while (FindNextFile(hFind, &findData) != 0 && count < 50);
    
    FindClose(hFind);

    if (count == 0) {
        return 0;
    }

    int choix = -1;
    while (1) {
        clrscr();
        printf("\n\n");
        text_color(C_CYAN);
        centrer_texte("=== HISTORIQUE DES PARTIES ENREGISTREES ===");
        printf("\n");
        
        text_color(C_WHITE);
        printf("   %-4s | %-20s | %-10s | %-8s | %-5s\n", "NO", "PSEUDO", "SCORE", "NIVEAU", "VIES");
        printf("   -----+----------------------+------------+----------+-------\n");
        
        for (int i = 0; i < count; i++) {
            if (liste[i].vies > 0) {
                text_color(C_GREEN); 
            } 
            else {
                text_color(C_RED);   
            }
            
            printf("    %-2d  | %-20s | %-10d | %-8d | %d \n", 
                   i + 1, 
                   liste[i].pseudo, 
                   liste[i].score, 
                   liste[i].niveau, 
                   liste[i].vies);
        }
        
        text_color(C_YELLOW);
        printf("\n   [0] RETOUR AU MENU PRINCIPAL\n");
        printf("\n   Entrez le numero de la partie a charger : ");
        
        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n'); 
            choix = -1;
        } 
        else {
            while (getchar() != '\n');
        }

        if (choix == 0) {
            return -1; 
        }
        
        if (choix > 0 && choix <= count) {
            strcpy(buffer_pseudo_choisi, liste[choix - 1].pseudo);
            return 1; 
        }
    }
}