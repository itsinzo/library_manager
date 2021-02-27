
#ifndef TP1_AFFICHAGE_H
#define TP1_AFFICHAGE_H

#include <windows.h>

COORD co={0,0};
void gotoxy(int x, int y)
{
    co.X=x;
    co.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),co);
}
void Color(int couleurDuTexte,int couleurDeFond) // fonction d'affichage de couleurs
{   // fonction d'affichage de couleurs
/*
0 : Noir
1 : Bleu foncé
2 : Vert foncé
3 : Turquoise
4 : Rouge foncé
5 : Violet
6 : Vert caca d'oie
7 : Gris clair
8 : Gris foncé
9 : Bleu fluo
10 : Vert fluo
11 : Turquoise
12 : Rouge fluo
13 : Violet 2
14 : Jaune
15 : Blanc*/

    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}

void acceuil()
{

    printf("      _______   ___   _______      ");Color(9,0);printf(" \n");Color(7,0);
    printf("     |  ___  | |___| |  ___  |     ");Color(9,0); Color(15,0); printf(" \n");Color(7,0);
    printf("     | |___| |  ___  | |___| |     _____    _____   ");Color(9,0);printf(" __\n");Color(7,0);
    printf("     |  _____| |   | |  _____|    |  ___|  |  ___|  ");Color(9,0);printf("|__|"); Color(15,0); printf(" Ecole Nationale\n");Color(7,0);
    printf("     | |_____  |   | | |_____     | |___   | |___    __\n");
    printf("     |  ____ | |   | |  ____ |    |  ___|  |___  |  |  |"); Color(15,0); printf(" Superieur\n");Color(7,0);
    printf("     | |____|| |   | | |____||    | |___    ___| |  |  |\n");
    printf("     |_______| |___| |_______|    |_____|  |_____|  |__|"); Color(15,0); printf(" d'Informatique\n");
    Color(15,0);
    gotoxy(25,10);
    printf("TP Structures de fichiers et structures de donnees (SFSD)\n",130);
    Color(9,0);
    gotoxy(28,11);
    printf("Appuyez sur 'Enter' pour lancer le programme !!\n");
    for(int i=1;i<=120;i++)
    {
        printf("_");
    }
    gotoxy(50,15);
    printf("REALISER PAR :");
    gotoxy(40,16);
    Color(9,0); printf("      INEZARENE Abdelghafour"); Color(15,0);
    gotoxy(45,17);
    Color(15,0); printf("  SECTION B | Groupe 06");
    gotoxy(88,18);
    printf("ANNEE UNIVERSITAIRE : 2020/2021");
}

void entree(void)
{
    char enter = 0;
    fflush(stdin);
    while (enter != '\r' && enter != '\n')
        enter = getchar();
}




#endif //TP1_AFFICHAGE_H
