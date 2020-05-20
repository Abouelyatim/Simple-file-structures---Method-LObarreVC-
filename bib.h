#ifndef _bib_
#define _bib_

#define max 1024

//*******************************************declaration des types ********************************************//
typedef struct Tbloc
{
    char tab[max];  // le tableau de caractère
    int suivant;    // le numero du bloc suivant dans la liste
}Tbloc;

typedef struct Tbloc Buffer;  // declaration du tampon


typedef struct Entete        // structure de l'entete du fichier en mémoire centrale
{
    int nbbloc;
    int tete;
    int queue;
    int indice_libre;   // l'indice libre dans le bloc de la queue
    int nb_car_sup;
    int nb_enreg;

}Entete;

typedef struct L7OVC
{
    FILE *fich;
    Entete entete;

}L7OVC;

typedef struct index
{
    int cle;
    int pos;
    int bloc;
}index;

typedef struct index_date
{
    int cle;
    int annee;
}index_date;
//*****************************************************traitement fichier
void liredir(L7OVC *fichier, int i , Buffer *buf);
void ecriredir(L7OVC *fichier, int i, Buffer *buf);
int entete(L7OVC *fichier, int i);
void aff_entete(L7OVC *fichier,int i , int valeur);
L7OVC *ouvrir(char nom_physique[], char mode);
void fermer(L7OVC *fichier);
void  alloc_bloc(L7OVC *fichier);
//*****************************************************

//*****************************************************chargement initiale
//void inserttion(L7OVC*fichier,char*enreg,int inser,index*tab,Buffer *buf);
void chargement_initiale(L7OVC *fichier,int n);
//void ecrire_chaine(L7OVC *fichier,int n , int *i, int *j, char *enreg,int *cpt,Buffer *buf,int insser,index*tab);
int recherch(L7OVC *fichier,int cle,int*bloc,int*position);
//*****************************************************

//*****************************************************generation aleatoire
char*genere_enreg();
char*genere_date();
int maxjour(int y,int m);
char*genere_sang();
char*genere_region();
char*genere_force();
char*genere_grade();
char*genere_wilaya();
char* genere_nom_prenom();
char* genere_matricule();
void affich_enreg_par_enreg(L7OVC *fichier);
void lire_enreg(char*tab,int *matrr,char date[9],char eff[2],char nom[32],char prenom[32],char wilaya[22],char force[48],char grade[31],char region[26],char sang[5]);
//*****************************************************

void lire(char *chaine, int longueur);

#endif
