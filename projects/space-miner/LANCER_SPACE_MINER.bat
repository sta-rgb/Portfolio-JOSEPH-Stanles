@echo off
TITLE Compilation SPACE_MINER
CLS
color 0B

echo.
echo                          SPACE MINER - COMPILATION SCRIPT
echo                         =================================
echo                               ULTRA MINER EDITION      
echo                         =================================
echo.

:: --- 1. VERIFICATION DU DOSSIER SONS ---
if not exist "Sons\somebodyme.mp3" (
    color 0C
    echo.
    echo [ERREUR] Dossier 'Sons' manquant !
    pause
    exit
)

:: --- 2. COMPILATION ---
echo 1. Nettoyage...
del *.o 2>nul
del SPACE_MINER.exe 2>nul

echo 2. Compilation...
gcc main.c parametres.c menu_options.c affichage.c fonctionnement.c extensions.c -o SPACE_MINER.exe -lwinmm
:: Note: -lwinmm est requis pour la gestion audio sous Windows
:: --- 3. EXECUTION ---
if exist "SPACE_MINER.exe" (
    color 0A
    echo 3. Lancement...
    timeout /t 1 >nul
    SPACE_MINER.exe
) else (
    color 0C
    echo [ECHEC] Erreur compilation.
)

echo.
echo Fin du programme.
PAUSE