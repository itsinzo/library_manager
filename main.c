#include "machine_abs.h"
#include "fonctions_tp.h"
#include "affichage.h"


int main() {

    FichierLOV7C *f=NULL;
    srand(time(NULL));
    int id,numart,numbloc; BlocLOV7C *blocadr;
    int cout,creeindex=0,creeperiodique=0; TOF *fich;



    acceuil();
    entree();

    printf("\n");Color(0,9);printf("BIENVENUE DANS NOTRE PROGRAMME\n\n");

    int selectionneur,entered=0,selectionneur2,entered2=0;
    while (entered==0) {
        printf("VOULEZ VOUS ?\n"); Color(0,2);
        printf("01/ OUVRIR UN ANCIEN FICHIER OUVRAGES EXISTANT \n");
        printf("02/ CREER UN NOUVEAU FICHIER OUVRAGES          \n");
        Color(0,12);
        printf("03/QUITTER LE PROGRAMME\n");
        Color(15,0);
        scanf("%d", &selectionneur);
        switch (selectionneur) {
            case 1:
                entered=1;
                ouvrir_LOV7C(&f,"ouvrages.bin",'a');
                break;
            case 2:
                entered=1;
                ouvrir_LOV7C(&f,"ouvrages.bin",'n');
                creer_fichier(f);
                break;
            case 3: //quitter sans rien faire
                Color(0,9);
                printf("Merci d'avoir utilise notre programme");
                Color(15,0);
                return 0;
                break;
            default:
                printf("Faites votre choix SVP\n");
        }
    }
    entered=0;
    printf("\n\n     Choisissez une des options suivantes\n");
    while (entered==0) {
        Color(15,0);
        printf("\n                               ***MENU PRINCIPAL***\n");
        printf("                 * (1) Afficher le fichier 'Ouvrages.bin'\n");
        printf("                 * (2) Recherche d'un ouvrage dans le fichier 'Ouvrages.bin'\n");
        printf("                 * (3) Insertion d'un nouvel ouvrage dans le fichier 'Ouvrages.bin\n");
        printf("                 * (4) Modification de l'etat (disponibilitee) d'un ouvrage du fichier 'Ouvrages.bin'\n");
        printf("                 * (5) Creation du fichier 'Livres.bin' a partir du fichier 'Ouvrages.bin'\n");
        printf("                 * (6) Creation du fichier 'Periodiques.bin' a partir du fichier 'Ouvrages.bin'\n");
        printf("                 * (7) Aller vers les options du fichier 'Periodiques.bin'\n");
        printf("                 ");Color(15,12);printf("* (0) Pour quitter le Programme");Color(15,0);printf("\n");
        scanf("%d", &selectionneur);
        switch(selectionneur){
            case 0:
                Color(15,0);
                entered=1;
                break;
            case 1:
                afficher_fichier(f);
                system("pause");
                break;
            case 2:
                printf("\n entrez la cle de l'ouvrage recherche\n");
                scanf("%d",&id);
                cout=rechercher(f,id,&blocadr,&numart,&numbloc);
                if ((numart != 0 )&&(numbloc != 0)){
                    Color(0,2);printf("ELEMENT TROUVE\n");
                    printf("L'adesse du bloc: %p\n",blocadr);
                    printf("Le num de l'article: %d",numart);
                }
                else{
                    Color(0,12);printf("ELEMENT NON TROUVE");
                }
                Color(15,0); printf("\n");
                Color(0,14);printf("**** Le cout de la recherche dans le fichier 'C2' est: %d *****",cout);Color(15,0);printf("\n");
                system("pause");
                break;
            case 3:
                insertion(f);
                system("pause");
                break;
            case 4:
                printf("\n entrez la cle de l'ouvrage dont vous voulez modifier l'etat\n");
                scanf("%d",&id);
                modif_etat(f,id);
                system("pause");
                break;
            case 5:
                cree_livres(f);
                system("pause");
                break;
            case 6:
                creeperiodique=1;
                cree_periodique(f);
                system("pause");
                break;
            case 7:
                if (creeperiodique==1){
                    ouvrir_TOF(&fich,"periodique.bin",'a');
                    while (entered2==0) {
                        Color(15, 0);
                        printf("\n                               ***OPTIONS du fichier periodiques***\n");
                        printf("                 * (1) Afficher le fichier 'periodiques.bin' \n");
                        printf("                 * (2) Recherche d'un periodique \n");
                        printf("                 * (3) Creation de la table d'index du fichier periodiques\n");
                        printf("                 * (4) Recherche d'un periodique a partir de l'index\n");
                        printf("                 ");Color(15,12);printf("* (0) Pour quitter le menu index et revenir au menu principale");Color(15,0);printf("\n");
                        scanf("%d", &selectionneur2);
                        switch(selectionneur2){
                            case 0:
                                entered2=1;
                                break;
                            case 1:
                                afficher_periodique(fich);
                                system("pause");
                                break;
                            case 2:
                                recherche_periodique(fich);
                                system("pause");
                                break;
                            case 3:
                                creeindex=1;
                                creer_index(fich);
                                system("pause");
                                break;
                            case 4:
                                if(creeindex==1){
                                    recherche_periodique_index(fich);
                                    system("pause");
                                }
                                else{
                                    printf("Fichier index non cree, creez le d'abord\n");
                                    system("pause");
                                }
                                break;
                            default :
                                printf("\nEntree ne correspond a aucune option, Veuillez choisir une des options suivantes svp\n");
                        }
                    }
                    fermer_TOF(fich);
                }
                else{
                    printf("fichier periodique non cree, creez le d'abord\n");
                    system("pause");
                }
                break;
            default :
                printf("\nEntree ne correspond a aucune option, Veuillez choisir une des options suivantes svp\n");
        }
    }
    fermer_FichierLOV7C(f);
    Color(0,9);
    printf("Merci d'avoir utilise notre programme");
    Color(15,0);
}
