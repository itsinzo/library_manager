#include <stdio.h>
#include <stdlib.h>
#ifndef TP1_MACHINE_ABS_H
#define TP1_MACHINE_ABS_H

#define TAILLE_TAB_BLOC 500
#define taille_titre 30
#define taille_aut 55
#define NbMaxArt 30       // NB ART MAX -1  // moins de 99



typedef struct EnteteLOV7C{

    int tete;
    int nb_car_ins; // nb total de caractères insérés
    int nb_car_suppr; // nb total de caracts supprimés
    int nb_bloc; // nb total de blocs dans le fichier


}EnteteLOV7C;


typedef struct FichierLOV7C{

    FILE* fichier;
    EnteteLOV7C entete;

}FichierLOV7C;


typedef struct BlocLOV7C{

    char tab[TAILLE_TAB_BLOC];
    struct BlocLOV7C *suiv; //pointeur vers l'adresse du prochain maillon

}BlocLOV7C;





void lireDirLOV7C(FichierLOV7C *f,int num_bloc,BlocLOV7C **buffer)///l'indexation commence du 1
{
    ///localiser la position du bloc dans le fichier
    fseek(f->fichier,sizeof(EnteteLOV7C)+(num_bloc-1)*sizeof(BlocLOV7C),SEEK_SET);
    ///lire et mettre
    fread((*buffer),sizeof(BlocLOV7C),1,f->fichier);

}


void ecrireDirLOV7C(FichierLOV7C *f,int num_bloc,BlocLOV7C *buffer)
{
    ///localiser la position du bloc dans le fichier
    fseek(f->fichier,sizeof(EnteteLOV7C)+(num_bloc-1)*sizeof(BlocLOV7C),SEEK_SET);
    ///lire et mettre

    fwrite(buffer,sizeof(BlocLOV7C),1,f->fichier);
}


int ouvrir_LOV7C(FichierLOV7C **f,char *nom_fichier,char mode)
{
    ///pour eviter le NULL
    *f=malloc(sizeof(FichierLOV7C));
    ///connaitre le type
    char s[4];
    if (mode=='A' || mode=='a')
        sprintf(s,"rb+");///ouvir un ancien fichier en mode lecture ecriture position du curseur au debut du fichier binaire
    else if (mode=='N' || mode=='n') {
        sprintf(s, "wb+");///creer un nouveau fichier en mode lecture ecriture position du curseur
    }
    else return 0;
    (*f)->fichier=fopen(nom_fichier,s);///ouvrir le fichier en c
    if ((*f)->fichier==0)
        return 0;
    if (mode=='A' || mode=='a')///si le mode est ancien
    {
        ///charger l'entete
        fread(&((*f)->entete),sizeof(EnteteLOV7C),1,(*f)->fichier);
    }
    else {
        ///initialiser notre entete
        (*f)->entete.nb_car_ins=0;
        (*f)->entete.nb_car_suppr=0;
        (*f)->entete.nb_bloc=0;
        (*f)->entete.tete=sizeof(EnteteLOV7C);
    }

    return 1;
}

void fermer_FichierLOV7C(FichierLOV7C*f)
{

    ///sauvgarder l'entete
    rewind(f->fichier);///aller au debut du fichier
    fwrite(&(f->entete),sizeof(f->entete),1,f->fichier);
    ///fermer le fichier
    fclose(f->fichier);
    free(f);

}


int entete(FichierLOV7C *f, int i)// i represente la ieme caracteristique dans la tete
{   switch(i) {
        case 1:
            return (f->entete.tete);
        case 2:
            return (f->entete.nb_car_ins);
        case 3:
            return (f->entete.nb_car_suppr);
        case 4:
            return (f->entete.nb_bloc);
        default:
            return 0;
    }
}


void affecte_entete(FichierLOV7C **f, int i,int valeur){
    switch(i) {
        case 1:
            (*f)->entete.tete=valeur;
        case 2:
            (*f)->entete.nb_car_ins=valeur;
        case 3:
            (*f)->entete.nb_car_suppr=valeur;
        case 4:
            (*f)->entete.nb_bloc=valeur;
    }
}

BlocLOV7C *AllocBloc(){
    return malloc(sizeof(BlocLOV7C));
}



/******************************************************************************************************************************/


                                /*********    MACHINE ABSTRAITE TOVC          ***********/



/// structure de l'entête ------------------------------------------------------------------------------------------------------
typedef struct Entete_TOVC
{
    int nbblocs;
}   Entete_TOVC ;
///-----------------------------------------------------------------------------------------------------------------------------

/// structure du bloc ----------------------------------------------------------------------------------------------------------
typedef struct Tbloc_TOVC
{
    char tableau[TAILLE_TAB_BLOC+2];  /// puisque taille variable => (bloc = tableau de caractère)
}   Tbloc_TOVC ;
///-----------------------------------------------------------------------------------------------------------------------------

/// structure du fichier de type TOVC ------------------------------------------------------------------------------------------
typedef struct TOVC
{
    FILE * f ;
    Entete_TOVC entete ;
}   TOVC ;
///-----------------------------------------------------------------------------------------------------------------------------


/// Entete : retourne la caractèristique i du fichier TOVC -----------------------------------------------------------------------------
int entete_TOVC (TOVC * fichier, int i)
{
    if (i ==1) {return(fichier->entete.nbblocs);}
    else {return -1;}
}
///-------------------------------------------------------------------------------------------------------------------------------------


/// Aff-Entete : permet d'affecter val à la caractéristique i choisie ------------------------------------------------------------------
void Aff_Entete_TOVC (TOVC * fichier, int i, int val)
{
    if (i == 1) {fichier->entete.nbblocs = val;}
}
///-------------------------------------------------------------------------------------------------------------------------------------


/// permet d'ouvrir un fichier un renvoyant un pointeur de type TOVC -------------------------------------------------------------------
TOVC * Ouvrir_TOVC (char nom_fichier[30], char mode_ouverture)
{
    TOVC * fichier = malloc(sizeof(TOVC));

    /// si on ouvre en mode nouveau
    if ((mode_ouverture == 'n')||(mode_ouverture == 'N'))
    {
        fichier->f = fopen(nom_fichier,"wb+") ;
        if (fichier->f != NULL)
        {
            /// initialisation de l'entête :
            fichier->entete.nbblocs=0;

            rewind(fichier->f);    ///positionnement au début du fichier
            fwrite(&(fichier->entete),sizeof(Entete_TOVC),1,fichier->f);///pourqoui il n'a pas fait directement size_of(entete)
        }
        else {printf("fichier mal cree\n");}
    }

        /// si on ouvre en mode ancien
    else
    {
        if ((mode_ouverture == 'a')||(mode_ouverture == 'A'))
        {
            fichier->f = fopen(nom_fichier,"rb+");
            if (fichier->f!= NULL)
            {
                rewind(fichier->f);
                fread(&(fichier->entete),sizeof(Entete_TOVC),1,fichier->f);
            }
            else {printf(" fichier mal ouvert, creer le fichier avant de continuer\n");}
        }
    }

    /// rewind(fichier->f);
    return(fichier);
}
///-------------------------------------------------------------------------------------------------------------------------------------


/// permet de fermer un fichier de type TOVC -------------------------------------------------------------------------------------------
void Fermer_TOVC (TOVC * fichier)
{
    rewind(fichier->f);
    fwrite(&(fichier->entete),sizeof(Entete_TOVC),1,fichier->f);
    fclose(fichier->f);
    free(fichier);
}
///-------------------------------------------------------------------------------------------------------------------------------------


/// permet d'écrire le Tbloc buf au i ème bloc du fichier de type TOVC -----------------------------------------------------------------
void ecriredirTOVC(TOVC * fichier, int i,Tbloc_TOVC *buf)
{
    /// on se positionne au début du i ème bloc puis on écrit dans fichier->f
    fseek(fichier->f,sizeof(Entete_TOVC)+(sizeof(Tbloc_TOVC)*(i-1)),SEEK_SET);
    fwrite(buf,sizeof(Tbloc_TOVC),1,fichier->f);
}

///-------------------------------------------------------------------------------------------------------------------------------------


/// permet de lire le i ème bloc du fichier TOVC pointé par fichier dans le Tbloc buf --------------------------------------------------
void liredirTOVC(TOVC * fichier, int i, Tbloc_TOVC * buf)
{
    /// on se jitionne au début du i ème bloc puis on le lit dans buf
    fseek(fichier->f,sizeof(Entete_TOVC)+(sizeof(Tbloc_TOVC)*(i-1)),SEEK_SET);
    fread(buf,sizeof(Tbloc_TOVC),1,fichier->f);
}
///-------------------------------------------------------------------------------------------------------------------------------------


/// permet d'allouer un nouveau bloc ---------------------------------------------------------------------------------------------------
void Alloc_bloc_TOVC(TOVC * fichier) {Aff_Entete_TOVC(fichier,1,entete_TOVC(fichier,1)+1);}
///-------------------------------------------------------------------------------------------------------------------------------------















                                          /*********    MACHINE ABSTRAITE TOF          ***********/


typedef struct EnteteTOF{
  int nb_bloc;
} EnteteTOF;


typedef struct TOF{
    FILE *fichierr;
    EnteteTOF entete;
} TOF;

typedef struct enregTOF {
    int id;
    char titre [taille_titre+1];
    char auteur [taille_aut+1];
    int annee ;
    int dispo;
}enregTOF;


typedef struct TblocTOF
{
    enregTOF tab[NbMaxArt];
    int nb_enreg;
} TblocTOF;




/*--------------Fonction d'ouverture d'un fichier--------------*/
int ouvrir_TOF(TOF **f,char *chemin,char mode)
{
    *f = malloc(sizeof(TOF));
    char s[3];

    if((mode == 'A') || (mode == 'a')) sprintf(s,"rb+");
    else if ((mode == 'N') || (mode == 'n')) sprintf(s,"wb+");
    else return 1;

    (*f)->fichierr = fopen(chemin, s);
    if((*f)->fichierr == NULL)   {printf("IMPOSSIBLE D'OUVRIR LE FICHIER ! ");}

    if((mode == 'A') || (mode == 'a'))
    {
        fread(&((*f)->entete), sizeof(EnteteTOF), 1, (*f)->fichierr);
    }
    else if ((mode == 'N') || (mode == 'n'))
    {
        (*f)->entete.nb_bloc = 0;
    }
    return 0;
}
/*--------------------------------------------------------------*/

/*--------------Fonction de fermeture d'un fichier--------------*/
void fermer_TOF(TOF *fich)
{
    //Sauvegarde de l'entête
    rewind((fich)->fichierr);
    fwrite(&(fich)->entete, sizeof(EnteteTOF), 1, (fich)->fichierr);
    fclose((fich)->fichierr);
    free(fich);
}
/*--------------------------------------------------------------*/


/*-----------------Fonction de lecture d'un bloc----------------*/
void lireDirTOF(TOF *f,int Num_Bloc,TblocTOF *buffer)
{
    if(Num_Bloc <= (f->entete).nb_bloc)
    {

        fseek(f->fichierr, sizeof(EnteteTOF) + (Num_Bloc-1) * sizeof(TblocTOF),SEEK_SET);
        fread(buffer,  sizeof(TblocTOF),1,f->fichierr);
    }
}
/*--------------------------------------------------------------*/

/*-----------------Fonction d'écriture d'un bloc----------------*/
void ecrireDirTOF(TOF *f,int Num_Bloc,TblocTOF *buffer)
{
    if(Num_Bloc <= (f->entete).nb_bloc)
    {
        fseek(f->fichierr, sizeof(EnteteTOF) + (Num_Bloc-1) * sizeof(TblocTOF),SEEK_SET);
        fwrite(buffer, 1, sizeof(TblocTOF), f->fichierr);
    }
}


/*----------------Fonction de lecture de l'entête---------------*/
/* lorsque i=1 ====> lecture de l'adresse du dernier bloc
   lorsque i=2 ====> lecture du nombre d'enregistrement*/


int enteteTOF(TOF *f,int i){
    if(i == 1) return (f->entete).nb_bloc;
    else return -1;
}
/*--------------------------------------------------------------*/


/*-------------Fonction de modification de l'entête-------------*/
/* lorsque i=1 ====> modification de l'adresse du dernier bloc
   lorsque i=2 ====> modification du nombre d'enregistrement*/
void aff_enteteTOF(TOF *f, int i, int val)
{
    if(i == 1) (f->entete).nb_bloc = val;
    else {printf ("caracteristique inexistante ! ");}
}
/*--------------------------------------------------------------*/

/*--------------------L'allocation d'un bloc--------------------*/
int alloc_blocTOF(TOF *f)
{
    aff_enteteTOF(f,1,enteteTOF(f,1)+1);
    return enteteTOF(f,1);
}

             /**********************   definition de la structire tableau index *************/
typedef struct engindex{
    int id;
    int numart;
    int numbloc;
}   engindex ;

typedef struct index{
    engindex Tabindex[NbMaxArt+1];
    int nb_articles;
}index;




#endif //TP1_MACHINE_ABS_H
