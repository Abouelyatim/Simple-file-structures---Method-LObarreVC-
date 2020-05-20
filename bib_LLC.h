#ifndef _bib_LLC_H_
#define _bib_LLC_H_


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// definir la structure d'un maillon
typedef struct maillon
{
	int val;
	struct maillon *suiv;
}maillon;

/*---------------------------------------------------------------------------------------------------------------------*/
/*                    Procédures d'implémentation du modèle sur les listes linéaires chaînées                          */
/*--------------------------------------------------------- -----------------------------------------------------------*/

void Allouerr( maillon **p)
{
    (*p)=malloc(sizeof( maillon));
}
//-----------------------------------------------------------------------------------------------------------
void Libererr(maillon *p)
{
    free (p);
}
//------------------------------------------------------------------------------------------------------------
void Aff_Val(maillon *P, int v)
{
   P->val =v;
}
//--------------------------------------------------------------------------------------------------------------
void Aff_Adr(maillon *P,  maillon *Q)
{
    P->suiv = Q;
}
//--------------------------------------------------------------------------------------------------------------
maillon *Suivant( maillon *P)
{
    return( P->suiv );
}
//--------------------------------------------------------------------------------------------------------------------
int Valeur( maillon *P)
{
     return( P->val) ;
}


  void afficher_llc( maillon *tete)    //pour afichier une LLC
{
   maillon *P,*q ;
    P=q=tete;
    printf("La liste des nombres est : \n");
    printf("\n");
    while (P!= NULL) { printf(" -----   ");  P =Suivant(P) ;}

    printf("\n") ;
    P=q;
	while (P!= NULL)
    {
		printf("|  %d  |--",Valeur(P)) ;
		P = Suivant(P);
    } printf("\n") ;
    P=q;
    while (P!= NULL) { printf(" -----   "); P =Suivant(P) ; }
    printf("\n") ;
    printf("\n");
}

#endif
