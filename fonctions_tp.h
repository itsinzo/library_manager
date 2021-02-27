#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "machine_abs.h"
#include "affichage.h"

#ifndef TP1_QUESTION_01_H
#define TP1_QUESTION_01_H


bool verifie_existence( int id , int tab_id[NbMaxArt],int num_article){  //fonction qui verifie l'existence de l'id dans la table tab_id et retourne 1 si existence, 0 si non existence
    int i;
    for (i = 0; i < num_article; i++) {
        if (id == tab_id[i]) return 1;
    }
    return 0 ;
}
int choisir_id(int tab_id[NbMaxArt], int num_article) {
    int id;
    id = rand() % ((num_article+1)*100);
    while ((verifie_existence(id,tab_id,num_article)==1) || ( id < tab_id[num_article-1])){
        id = rand() % ((num_article+1)*100);
    }
    return id;
}

// fonction qui remplit la case id d'un article en recevant l'adresse du bloc et le num de la case à partir de laquelle elle remplit
void remplir_id (BlocLOV7C *buffer,int j, int tab_id[NbMaxArt], int num_article){
    int id; char try[4];
    id = choisir_id(tab_id, num_article);
    tab_id[num_article]= id ;
    itoa(id,try,10);
    buffer->tab[j] = try[0];
    buffer->tab[j+1] = try[1];
    buffer->tab[j+2] = try[2];
    buffer->tab[j+3] = try[3];
}


void remplir_titre (BlocLOV7C *buffer, int j ){
    char *string="abcdefghijklmnopqrstuvwxyz";
    int i=rand()%26;
    int tabscroller;
    for(tabscroller=j;tabscroller< j + taille_titre;tabscroller++){
        buffer->tab[tabscroller]=string[i];
        i=rand()%26;
    }//for
}

void remplir_auteur (BlocLOV7C *buffer, int j ){
    char *string="abcdefghijklmnopqrstuvwxyz";
    int i=rand()%26;
    int tabscroller;
    for(tabscroller=j;tabscroller< j + taille_aut;tabscroller++){
        buffer->tab[tabscroller]=string[i];
        i=rand()%26;
    }//for
}

void remplir_annee (BlocLOV7C *buffer, int j){        //annee de publication
    int annee; char try[4];
    annee=(rand() % 72) + 1950;
    itoa(annee,try,10);
    buffer->tab[j] = try[0];
    buffer->tab[j+1] = try[1];
    buffer->tab[j+2] = try[2];
    buffer->tab[j+3] = try[3];
}

void remplir_type (BlocLOV7C *buffer, int j){
    int rando = (rand()%4)+1;
    switch(rando) {
        case 1:{
            buffer->tab[j] ='T';
            buffer->tab[j+1] ='i'; // Texte imprimé
            break;
            }
        case 2: {
            buffer->tab[j] = 'D';
            buffer->tab[j + 1] = 'E'; // document electronique
            break;
        }
        case 3: {
            buffer->tab[j] = 'A';
            buffer->tab[j + 1] = 'R';// article
            break;
        }
        case 4: {
            buffer->tab[j] = 'P';
            buffer->tab[j + 1] = 'E'; // periodique
            break;
        }
    }
}

void remplir_dispo (BlocLOV7C *buffer, int j){      // marquer la disponibilité de l'article ( 1 pour dispo, 0 pour non dispo)
    int i=rand()%2;char try[1];
    itoa(i,try,10);
    buffer->tab[j]=try[0];
}

void remplir_cote (BlocLOV7C *buffer, int j){        //remplis la cote
    int cote; char try[4];
    cote=(rand() % 9999);
    itoa(cote,try,10);
    buffer->tab[j] = try[0];
    buffer->tab[j+1] = try[1];
    buffer->tab[j+2] = try[2];
    buffer->tab[j+3] = try[3];
}

void remplir_resume (BlocLOV7C *buffer, int j, int taille_resume){
    char *string="abcdefghijklmnopqrstuvwxyz";
    int i=rand()%26;
    int tabscroller;
    for(tabscroller=j;tabscroller< (j + taille_resume);tabscroller++){
        buffer->tab[tabscroller]=string[i];
        i=rand()%26;
    }//for
}


void creer_fichier(FichierLOV7C *f) {
    int cout=0;
    BlocLOV7C *buffer, *bufferfree;
    int nb_articles, i, j = 0, num_bloc = 1, taille_article;
    int tab_id[NbMaxArt];

    buffer = AllocBloc();
    affecte_entete(&f,1,num_bloc);
    nb_articles = (rand() % (NbMaxArt - 1)) + 1;
    printf("Nombres d'articles dans le fichier cree est:%d\n", nb_articles);
    for (i = 0; i < nb_articles; i++) {
        taille_article = rand() % 100 + 101;
        if ((j + taille_article) <= TAILLE_TAB_BLOC) {
            buffer->tab[j] = '-';
            j++;
            affecte_entete(&f,2,entete(f,2)+taille_article+1);
            remplir_id(buffer, j, tab_id, i);
            j = j + 4;                      // champ id sur 4 caracteres
            remplir_titre(buffer, j);
            j = j + taille_titre;                     // champ titre sur 30 caracteres
            remplir_auteur(buffer, j);
            j = j + taille_aut;                     // champ auteur sur 59 caracteres
            remplir_dispo(buffer, j);
            j++;
            remplir_annee(buffer, j);
            j = j + 4;
            remplir_cote(buffer,j);
            j =j+4;                                     // champ cote sur 4 caracteres
            remplir_type(buffer, j);
            j = j + 2;                     // champ type sur 2 caracteres
            remplir_resume(buffer, j, (taille_article - 100));
            j = j +(taille_article - 100);  // champ resume sur taille_article-100  caracteres  ( compris entre 0 et 100)
        }//if
        else {
            buffer->tab[j] = '~';
            affecte_entete(&f,2,entete(f,2)+taille_article+2);
            buffer->suiv = AllocBloc();
            ecrireDirLOV7C(f, num_bloc, buffer); cout++;
            num_bloc++;
            bufferfree = buffer;
            buffer = buffer->suiv;
            free(bufferfree);
            j = 0;
            buffer->tab[j] = '-';
            j++;
            remplir_id(buffer, j, tab_id, i);
            j = j + 4;                      // ch²amp id sur 4 caracteres
            remplir_titre(buffer, j);
            j = j + taille_titre;                     // champ titre sur 30 caracteres
            remplir_auteur(buffer, j);
            j = j + taille_aut;                       // champ auteur sur 59 caracteres
            remplir_dispo(buffer, j);
            j++;                                      // champ dispo sur 1 car
            remplir_annee(buffer, j);
            j = j + 4;                                // champ annee sur 4 car
            remplir_cote(buffer,j);
            j =j+4;                                   // champ cote sur 4 caracteres
            remplir_type(buffer, j);
            j = j + 2;                               // champ type sur 2 caracteres
            remplir_resume(buffer, j, (taille_article - 100));
            j = j +(taille_article - 100);  // champ resume sur taille_article-100  caracteres  ( compris entre 0 et 100)
        }//else
    }//for
    buffer->tab[j] = '~';
    affecte_entete(&f,4,num_bloc);
    buffer->suiv = NULL;
    ecrireDirLOV7C(f, num_bloc, buffer);cout++;
    Color(0,14);printf("**** Le cout de la creation du fichier ouvrages 'C1' est: %d *****",cout);Color(15,0);printf("\n");
}

void afficher_bloc (BlocLOV7C *buffer){
    int i=0,j,num_art=0;
    while ((i < TAILLE_TAB_BLOC) && (buffer->tab[i] != '~')) {
        if (buffer->tab[i] == '-') {
            i++;
            num_art++;
            printf("\n\n                              ~~~~   article numero:%d  ~~~~\n\n", num_art);
            printf("ID:%c%c%c%c\n",buffer->tab[i],buffer->tab[i+1],buffer->tab[i+2],buffer->tab[i+3]);
            i=i+4;
            printf("titre:");
            for (j=0;j<taille_titre;j++){
                printf("%c",buffer->tab[i+j]);
            }
            i=i+taille_titre;
            printf("\nauteurs:");
            for (j=0;j<taille_aut;j++){
                printf("%c",buffer->tab[i+j]);
            }
            i=i+taille_aut;
            if ((buffer->tab[i])=='1'){
                printf("\nL'article est Disponible\n");}
            else {printf("\nL'article est Non Disponible\n");}
            i++;
            printf("L'annee de distribution:%c%c%c%c\n",buffer->tab[i],buffer->tab[i+1],buffer->tab[i+2],buffer->tab[i+3]);
            i=i+4;
            printf("La cote:%c%c%c%c\n",buffer->tab[i],buffer->tab[i+1],buffer->tab[i+2],buffer->tab[i+3]);
            i=i+4;
            printf("Le type:%c%c\n",buffer->tab[i],buffer->tab[i+1]);
            i=i+2;
            printf("Resume:");
        }
        printf("%c",buffer->tab[i]); i++;
    }
    printf("\n\n\n");
}

void afficher_fichier(FichierLOV7C *f){  //affiche tous les articles dans le fichier f

    BlocLOV7C *buffer;int j;
    buffer=AllocBloc();
    printf("\n\n");
    for (j=1;j<(entete(f,4))+1;j++) {
        printf("\n             ");Color(0,14);printf("BLOC NUM:   %d\n",j);Color(15,0);
        lireDirLOV7C(f,j, &buffer);
        afficher_bloc(buffer);
    }
    printf("\n\n");
}



int rechercher(FichierLOV7C *f,int id,BlocLOV7C **blocadr,int *numart, int *numbloc) {//retourne dans numart le numero de l'article recherché si trouve et dans blocadr l'adresse de son bloc, retourne 0 dans les 2champs sinon

    int cout=0;
    BlocLOV7C *buffer;
    int i, j=1, num_art = 0;bool trouv=0; char inter[4];
    buffer = AllocBloc();
    while ((j <= entete(f, 4)) && (trouv==0)) {
        lireDirLOV7C(f, j, &buffer); cout++;
        i = 0;
        num_art = 0;
        while ((i < TAILLE_TAB_BLOC) && (buffer->tab[i] != '~') && (trouv == 0)) {
            if (buffer->tab[i] == '-') {
                num_art++;
                i++;
                inter[0] = buffer->tab[i];
                inter[1] = buffer->tab[i + 1];
                inter[2] = buffer->tab[i + 2];
                inter[3] = buffer->tab[i + 3];
                if (id == atoi(inter)) {
                    trouv = 1;
                    *numart = num_art;
                    *numbloc = j;
                    lireDirLOV7C(f, j - 1, &buffer); cout++;
                    *blocadr = &*buffer->suiv;
                }
            }
            i++;
        }
        j++;
    }
    return cout;
}






void insertion(FichierLOV7C *f) {
    int cout=0;
    BlocLOV7C *buffer,*bufferfree;
    int j, i=1, num_art = 0,taille_article;char inter[4]; int tab_id [NbMaxArt];
    buffer = AllocBloc();
    while ((i <= entete(f, 4)) ) {
        lireDirLOV7C(f, i, &buffer);cout++;
        j = 0;
        while ((j < TAILLE_TAB_BLOC) && (buffer->tab[j] != '~') ) {
            if (buffer->tab[j] == '-') {
                j++;
                inter[0]=buffer->tab[j];
                inter[1]=buffer->tab[j+1];
                inter[2]=buffer->tab[j+2];
                inter[3]=buffer->tab[j+3];
                tab_id[num_art]=atoi(inter);
                num_art++;
            }
            j++;
        }
        i++;
    }
    taille_article = rand() % 100 + 101;
    if ((j + taille_article) <= TAILLE_TAB_BLOC) {
        affecte_entete(&f,2,entete(f,2)+taille_article+1);
        buffer->tab[j] = '-';
        j++;
        remplir_id(buffer, j, tab_id, num_art+1);
        j = j + 4;                      // champ id sur 4 caracteres
        remplir_titre(buffer, j);
        j = j + taille_titre;                     // champ titre sur 30 caracteres
        remplir_auteur(buffer, j);
        j = j + taille_aut;                     // champ auteur sur 59 caracteres
        remplir_dispo(buffer, j);
        j++;
        remplir_annee(buffer, j);
        j = j + 4;
        remplir_cote(buffer,j);
        j =j+4;                                     // champ cote sur 4 caracteres
        remplir_type(buffer,j);
        j = j + 2;                     // champ type sur 2 caracteres
        remplir_resume(buffer, j, (taille_article - 100));
        j = j +(taille_article - 100);  // champ resume sur taille_article-100  caracteres  ( compris entre 0 et 100)
    }//if
    else {
        buffer->tab[j] = '~';
        affecte_entete(&f,2,entete(f,2)+taille_article+2);
        buffer->suiv = AllocBloc();
        ecrireDirLOV7C(f, i-1, buffer);
        i++;
        bufferfree = buffer;
        buffer = buffer->suiv;
        free(bufferfree);
        j = 0;
        buffer->tab[j] = '-';
        j++;
        remplir_id(buffer, j, tab_id, i);
        j = j + 4;                      // ch²amp id sur 4 caracteres
        remplir_titre(buffer, j);
        j = j + taille_titre;                     // champ titre sur 30 caracteres
        remplir_auteur(buffer, j);
        j = j + taille_aut;                       // champ auteur sur 59 caracteres
        remplir_dispo(buffer, j);
        j++;                                      // champ dispo sur 1 car
        remplir_annee(buffer, j);
        j = j + 4;                                // champ annee sur 4 car
        remplir_cote(buffer,j);
        j =j+4;                                   // champ cote sur 4 caracteres
        remplir_type(buffer, j);
        j = j + 2;                               // champ type sur 2 caracteres
        remplir_resume(buffer, j, (taille_article - 100));
        j = j +(taille_article - 100);  // champ resume sur taille_article-100  caracteres  ( compris entre 0 et 100)
    }//else
    buffer->tab[j] = '~';
    affecte_entete(&f,4,i-1);
    buffer->suiv = NULL;
    ecrireDirLOV7C(f, i-1, buffer);cout++;
    Color (0,2); printf("Insertion Reussite");Color(15,0); printf("\n");
    Color(0,14);printf("**** Le cout de l'insertion 'C3' est: %d *****",cout);Color(15,0);printf("\n");
}



void modif_etat(FichierLOV7C *f,int id) {
    int cout=0;
    int num_art,numbloc=0,i=0,j=0;BlocLOV7C *buffer;
    //buffer=AllocBloc();
    cout=cout + rechercher(f,id,&buffer,&num_art,&numbloc);
    if ((numbloc==0)||(num_art==0)){
        Color(0,12);
        printf("MODIFICATION IMPOSSIBLE, ARTICLE NON TROUVE");
        Color(15,0);
        printf("\n");
    }
    else {
        Color(0,2);
        printf("modification possible, RESULTAT: ");
        Color(15,0);
        printf("\n");
        buffer = AllocBloc();
        lireDirLOV7C(f, numbloc, &buffer);
        cout++;
        while (i < num_art) {
            if (buffer->tab[j] == '-') {
                i++;
            }
            j++;
        }
        if ((buffer)->tab[j + 4 + taille_titre + taille_aut] == '1')
            (buffer)->tab[j + 4 + taille_titre + taille_aut] = '0';
        else (buffer)->tab[j + 4 + taille_titre + taille_aut] = '1';
        ecrireDirLOV7C(f, numbloc, buffer);
        cout++;
        afficher_bloc(buffer);
        Color(0,14);printf("**** Le cout de la modification d'etat d'un article 'C4' est: %d *****", cout);Color(15,0);printf("\n");
    }
}


void recherche_periodique(TOF *f){
    int cout=0;
    TblocTOF *buf=malloc(sizeof(TblocTOF)); int i=1,cle; bool trouv=0;

    printf("Recherche periodique initialisee ,  entrez la cle de l element a rechercher");
    scanf("%d",&cle);

    while((i<= enteteTOF(f,1)) && trouv==0){
        lireDirTOF(f,i,buf);cout++;
        for(int j=0;j< NbMaxArt;j++){
            if (buf->tab[j].id == cle ) {
                trouv=1;
                printf("\n");
                Color(15,2);printf("  ***ARTICLE TROUVE***");Color(15,0);
                printf("\nID:%d\nTitre:%s\nAuteurs:%s\ndisponibilite:%d\nAnnee:%d\n",buf->tab[j].id,buf->tab[j].titre,buf->tab[j].auteur,buf->tab[j].dispo,buf->tab[j].annee);
            }
        }
        i++;
    }
    if (trouv==0){
        printf("\n");
        Color(15,12);printf ("Element non trouve");Color(15,0);
        printf("\n");
    }
    Color(0,14);printf("**** Le cout de recherche d'un periodique et affichage de tous ces champs 'C6' est: %d *****",cout);Color(15,0);printf("\n");
}


void afficher_periodique(TOF *fich){
    TblocTOF *bufferTOF=malloc(sizeof(TblocTOF));
    for (int i=1;i<=enteteTOF(fich,1);i++) {
        lireDirTOF(fich,i,bufferTOF);
        printf("\n\n     ******BLOC NUM %d******\n\n",i);
        for(int j=0;j<(bufferTOF->nb_enreg);j++){
            printf("\nARTICLE NUM %d\n",j+1);
            printf("ID:%d\n",(bufferTOF->tab[j]).id);
            printf("titre:%s\n",(bufferTOF->tab[j]).titre);
            printf("auteur(s):%s\n",(bufferTOF->tab[j]).auteur);
            printf("disponibilitee:%d\n",(bufferTOF->tab[j]).dispo);
            printf("annee de production:%d\n",(bufferTOF->tab[j]).annee);
        }
    }
}



void cree_periodique(FichierLOV7C *f) {
    int cout=0;
    TOF *fich;BlocLOV7C *bufferLOV7C; TblocTOF *bufferTOF;int i,j,numart,k;char mediateur_id_annee[5];

    bufferTOF=malloc(sizeof(TblocTOF));
    ouvrir_TOF(&fich,"periodique.bin",'n');
    bufferLOV7C=AllocBloc();
    for(i=1;i<=entete(f,4);i++) {
        numart=0;
        j=0;
        lireDirLOV7C(f,i,&bufferLOV7C);cout++;
        while (bufferLOV7C->tab[j] != '~'){
            if (bufferLOV7C->tab[j] == '-'){
                j++;
                numart++;
                for (k=0;k<4;k++){
                    mediateur_id_annee[k]=bufferLOV7C->tab[j+k];
                }
                (bufferTOF->tab[numart-1]).id=atoi(mediateur_id_annee);
                j=j+4;
                for (k=0;k<taille_titre;k++){
                    (bufferTOF->tab[numart-1]).titre[k]=bufferLOV7C->tab[j+k];
                }
                (bufferTOF->tab[numart-1]).titre[k]='\0';
                j=j+taille_titre;
                for (k=0;k<taille_aut;k++){
                    (bufferTOF->tab[numart-1]).auteur[k]=bufferLOV7C->tab[j+k];
                }
                (bufferTOF->tab[numart-1]).auteur[k]='\0';
                j=j+taille_aut;
                if(bufferLOV7C->tab[j]=='0'){
                    (bufferTOF->tab[numart-1]).dispo=0;
                }
                else {
                    (bufferTOF->tab[numart-1]).dispo=1;
                }
                j++;
                for (k=0;k<4;k++){
                    mediateur_id_annee[k]=bufferLOV7C->tab[j+k];
                }
                (bufferTOF->tab[numart-1]).annee=atoi(mediateur_id_annee);
                j=j+10; // 4 pour annee 2 pour type et 4 pour cote
            }
            else{
                j++;
            }
        }//while
        aff_enteteTOF(fich,1,enteteTOF(fich,1)+1);
        bufferTOF->nb_enreg=numart;
        ecrireDirTOF(fich,i,bufferTOF);cout++;
    }
    Color(0,2);
    printf("creation du fichier periodique reussite");
    Color(15,0); printf("\n");
    printf("voulez vous afficher le fichier periodique cree ? ( 1 pour 'oui' , autre boutton pour 'non') :");
    scanf("%d",&k);
    if(k==1) afficher_periodique(fich);
    printf("\nfermeture du fichier...\n");
    fermer_TOF(fich);
    Color(0,14);printf("**** Le cout de la creation du fichier periodique 'C54' est: %d *****",cout);Color(15,0);printf("\n");
}


void afficher_livres(TOVC *f){
    Tbloc_TOVC *buf=malloc(sizeof(Tbloc_TOVC));
    for (int i=1;i<= entete_TOVC(f,1);i++){
        liredirTOVC(f,i,buf);
        printf("\n\nBLOC NUM : %d\n\n",i);
        for (int j=0;j<TAILLE_TAB_BLOC+2;j++){
            printf("%c",buf->tableau[j]);
        }
    }

}

void cree_livres(FichierLOV7C *f){
    int cout=0;
    TOVC *fich;BlocLOV7C *bufferLOV7C;Tbloc_TOVC *bufferTOVC;int i,j,TOVCscroller=0,taille_art,nbblocTOVC=1,save;
    char tab_inter[TAILLE_TAB_BLOC+2];


    bufferTOVC=malloc(sizeof(Tbloc_TOVC));
    fich=Ouvrir_TOVC("Livres.bin",'n');
    bufferLOV7C=AllocBloc();
    for(i=1;i<=entete(f,4);i++) {
        j=0;
        lireDirLOV7C(f,i,&bufferLOV7C);cout++;
        while (bufferLOV7C->tab[j] != '~'){
            if((bufferLOV7C->tab[j])=='-'){
                taille_art=101;
                j=j+101;
                while (((bufferLOV7C->tab[j])!='-')&&((bufferLOV7C->tab[j])!='~')){ //parcour du tableau pour calculer la taille de l'article actuel
                    taille_art++;
                    j++;
                }
                j=j-taille_art;
                taille_art=taille_art-3;
                if(TOVCscroller+taille_art<TAILLE_TAB_BLOC){ //inserer dans le bloc actuel du fich TOVC
                    for(int k=0;k<(14+taille_titre+taille_aut);k++){
                        bufferTOVC->tableau[TOVCscroller+k]=bufferLOV7C->tab[j+k];
                    }
                    TOVCscroller=TOVCscroller+(14+taille_titre+taille_aut);
                    j=j+14+taille_titre+taille_aut+2;
                    for(int k=0;k<taille_art-(14+taille_titre+taille_aut)+1;k++){
                        bufferTOVC->tableau[TOVCscroller+k]=bufferLOV7C->tab[j+k];
                    }
                    TOVCscroller=TOVCscroller+taille_art-(14+taille_titre+taille_aut);
                }
                else {   //inserer en cheuvauchement dans un nouveau bloc
                    for(int k=0;k<(14+taille_titre+taille_aut);k++){ //utilisation d'une tab intermediaire pour faciliter le remplissage en cas de cheuvauchement
                        tab_inter[k]=bufferLOV7C->tab[j+k];
                    }
                    j=j+14+taille_titre+taille_aut+2;
                    for(int k=0;k<taille_art-(14+taille_titre+taille_aut)+1;k++){
                        tab_inter[k+(14+taille_titre+taille_aut)]=bufferLOV7C->tab[j+k];
                    }
                    for (int k=0;k<(TAILLE_TAB_BLOC-TOVCscroller);k++){   //remplissage de l'espace vide restant dans l'ancien bloc
                        bufferTOVC->tableau[TOVCscroller+k]=tab_inter[k];
                    }
                    save=(TAILLE_TAB_BLOC-TOVCscroller);
                    ecriredirTOVC(fich,nbblocTOVC,bufferTOVC);cout++;
                    nbblocTOVC++;
                    TOVCscroller=0;
                    for(int k=0;k<TAILLE_TAB_BLOC;k++){  // vider le buffer deja utilisee pour pouvoir l'utiliser une autre fois
                        bufferTOVC->tableau[k]='\0';
                    }
                    for (int k=0;k<(taille_art-save)+1;k++){ //remplir le reste dans le nouveau bloc
                        bufferTOVC->tableau[TOVCscroller+k]=tab_inter[k+save];
                    }
                    TOVCscroller=TOVCscroller+(taille_art-save);
                }
            }
            else j++;
        }
    }
    ecriredirTOVC(fich,nbblocTOVC,bufferTOVC);cout++;
    Aff_Entete_TOVC(fich,1,nbblocTOVC);
    Color(0,2);
    printf("Creation du fichier livres reussite\n");
    Color(15,0);
    printf("Voulez vous afficher le fichier livres cree ? ( 1 pour 'oui' , autre boutton pour 'non') :");
    scanf("%d",&i);
    if(i==1) afficher_livres(fich);
    printf("\nfermeture du fichier...\n");
    Fermer_TOVC(fich);
    Color(0,14);printf("**** Le cout de la creation du fichier livres 'C51' est: %d *****",cout);Color(15,0);printf("\n");
}

void creer_index (TOF *f) {
    int cout = 0;
    TblocTOF *buf;
    index *bufindex;
    int tabscroller = 0, j, i;
    FILE *f_index = NULL;

    buf = malloc(sizeof(TblocTOF));
    bufindex = malloc(sizeof(index));
    for (i = 1; i <= enteteTOF(f, 1); i++) {
        lireDirTOF(f, i, buf);
        cout++;
        for (j = 0; j < (buf->nb_enreg); j++) {
            bufindex->Tabindex[tabscroller].id = buf->tab[j].id;
            bufindex->Tabindex[tabscroller].numbloc = i;
            bufindex->Tabindex[tabscroller].numart = j + 1;
            tabscroller++;
        }
    }
    bufindex->nb_articles = tabscroller;
    f_index = fopen("index_Periodiques.bin", "wb+");
    cout++;
    if (f_index == NULL) printf("erreur, on peut pas ecrire dans index.periodiques.bin");
    else{
        fwrite(bufindex, sizeof(index), 1, f_index);cout++;
    }
    fclose(f_index);
    printf("Fichier index a ete cree avec succes\n");
    Color(0,14);printf("\n**** Le cout de la creation de ce fichier index 'C7' est: %d *****",cout);Color(15,0);printf("\n");
}


void recherche_periodique_index(TOF *f){
    int cout=0;
    TblocTOF *buf;index *bufindex;FILE *f_index = NULL; int cle,j,trouv=0;
    buf = malloc(sizeof(TblocTOF));
    bufindex = malloc(sizeof(index));

    f_index = fopen("index_Periodiques.bin", "rb+");cout++;
    if (f_index == NULL) printf("erreur dans l'ouverture du fichier index periodiques");
    else {
        fread(bufindex,sizeof(index),1,f_index);
        printf("\nRecherche periodique a partir de l'index initialisee , entrez la cle de l element a rechercher");
        scanf("%d",&cle);
        for(int i=0;i< bufindex->nb_articles;i++){
            if (bufindex->Tabindex[i].id==cle){
                trouv=1;
                lireDirTOF(f, bufindex->Tabindex[i].numbloc, buf);cout++;
                j=(bufindex->Tabindex[i].numart)-1;
                Color(15,2);printf("   ***ARTICLE TROUVE***"); Color(15,0);
                printf("\nID:%d\nTitre:%s\nAuteurs:%s\ndisponibilite:%d\nAnnee:%d\n",buf->tab[j].id,buf->tab[j].titre,buf->tab[j].auteur,buf->tab[j].dispo,buf->tab[j].annee);
            }
        }
        if(trouv==0) {
                Color(0,12);printf("Element non trouve");Color(15,0);printf("\n");
        }
    }
    Color(0,14);printf("**** Le cout de la recherche d'un periodique a partir de l'index et affichage de tous ses champs 'C8' est: %d *****",cout);Color(15,0);printf("\n");
}

#endif //TP1_QUESTION_01_H
