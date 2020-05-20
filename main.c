#include <stdio.h>
#include <stdlib.h>
#include"bib.h"
#include"conio.h"
#include"conio2.h"


/*
*author: par Ibrahim Abouelyatim Berkane*
*version: 1
* (^.^)IAB Dev
*/


int nb_ecdire;
int nb_liredire;

void main()
{

    int menu1;
    char* E1[] =    {
                      "Creation du fichier",
                      "Chargement initial aleatoire",
                      "Insertion d un nouvel enregistrement",
                      "Requete a intervalle d_ages",
                      "Modification region militaire d un enregistrement",
                      "Supprimer un enregistrement par matricule",
                      "Supprimer tous enregistrement relatifs a une force armee",
                      "Afficher tous les enregistrements appartenant a une categorie de grades",
                      "Afficher les 3 premier blocs",
                      "Le nombre de LireDir et EcrireDir",
                      "EXIT"
                     };

	system("mode con  cols=168");
	menu:
	textbackground(GREEN);
	textcolor(BLACK);
	system("cls");
	AffichezCadre(39,19,90 ,19,219,BLACK);
	menu1=Menu(E1,11,40,20,88,17,WHITE,LIGHTGRAY);

	switch (menu1)
         {
            case 1:
            	choix_creation_fich();
                goto menu ;
                break;

            case 2:

                choix_inser_alleatoire();
                goto menu ;
                break;

            case 3:
                choix_inser_new_enreg();
                goto menu ;
                break;

            case 4:
                choix_requet_intervalle();
                goto menu ;
                break;

            case 5:
                choix_modif_region();
                goto menu ;
                break;

            case 6:
                choix_supr_enreg();
                goto menu ;
                break;

            case 7:
                choix_supr_relatif_force();
                goto menu ;
                break;

            case 8:
                choix_affich_tous_de_grade();
                goto menu ;
                break;

            case 9:
                choix_affich_3premier_bloc();
                goto menu ;
                break;

            case 10:
                textbackground(WHITE);
                textcolor(BLACK);
                system("cls");

                gotoxy(5,6);
                printf("Le nombre de LireDir: %d",nb_liredire);
                gotoxy(5,8);
                printf("Le nombre de EcrireDir: %d\n",nb_ecdire);
                textcolor(WHITE);
                system("pause");
                textcolor(BLACK);
                goto menu ;
                break;

            case 11:
                exit(1);
         }
}



/*
void main()
{
    system("mode con  cols=168");
    int pos;
    L7OVC *fichier;
    Buffer buff;
     char taille[4];
    char nom_physique[20];
    char modee;
    int n,i;
    printf("\t veuillez iontroduire le nom du fichier que vous voulez créer\n\n");

                            lire(nom_physique, 20);
                          // modif_region(nom_physique,598160,6);
                           // supr_enreg(nom_physique,598160);
                          // supr_enreg_relatif_force(nom_physique,3);
                         printf("\t veuillez introduire le mode d'ouverture du fichier\n\n");

                            scanf("%c",&modee);


                            fichier=ouvrir(nom_physique,modee);
                            if(fichier!=NULL)
                            {
                               printf("\n\t\t ouverture reussie\n");
                            }
                            else
                            {
                                printf("echec lors de l'ouverture\n\n");
                            }

//affich_grade_donner(fichier,1);
/*
                            printf("entrer la region:");
                            int region;
                            scanf("%d",&region);
                            printf("entrer l'age1:");
                            int age1;
                            scanf("%d",&age1);
                            printf("entrer l'age2:");
                            int age2;
                            scanf("%d",&age2);
               system("cls");
             rech_region_date(fichier,region,age1,age2);


*/

           /*          printf("veuillez introduire le nombre d'enregistrement que vous voulez insererdes la première fois\n\n");

                            scanf("%d",&n);

/*
                       for(i=0;i<n;i++)
                       {
                          if(  inser_nouvel_enreg(fichier)==0)
                        {
                            textcolor(BLACK);
                            textbackground(WHITE);
                            system("cls");
                           // gotoxy(50,2);
                           // cputsxyTemp(50,2,"succse",50);
                        }

                        printf("******");
                       }*/




                            //chargement_initiale(fichier,n);



                          /* taille[0]=buff.tab[0];
                           taille[1]=buff.tab[1];
                           taille[2]=buff.tab[2];
                           taille[3]='\0';*/
  ///*
    //printf("nb bloc %d tete %d queue %d indice lib %d \n\n",entete(fichier,1),entete(fichier,2),entete(fichier,3),entete(fichier,4));
                        /*   for(pos=1;pos<=2;pos++)
                           {printf("**");
                               liredir(fichier, pos , &buff);
                               printf("%s\n\n\n",buff.tab);
                           }*/


   //  */
  //         affich_enreg_par_enreg(fichier);

               //lire index
/*
             index *arrayy=malloc(sizeof(index)*entete(fichier,6));
             index_date *arrayyy=malloc(sizeof(index_date)*entete(fichier,6));
               FILE *fic=fopen("index_matr","rb+");
               FILE *ficc=fopen("index_date","rb+");

  //             fread(arrayy,sizeof(index),entete(fichier,6),fic);
//       printf("**%d**",arrayy[entete(fichier,6)-2].cle);

               fread(arrayy,sizeof(index),entete(fichier,6),fic);
               fread(arrayyy,sizeof(index_date),entete(fichier,6),ficc);


 for(i=0;i<entete(fichier,6);i++)
    {
        printf("%04d)%04d*%04d*%04d\n",i,arrayy[i].bloc,arrayy[i].pos,arrayy[i].cle);
    }

for(i=0;i<entete(fichier,6);i++)
    {
        printf("%04d)%04d*%04d\n",i,arrayyy[i].annee,arrayyy[i].cle);
    }*/
           //recherche
/*int cle=742800;int bloc;int position;
if(recherch(fichier,cle,&bloc,&position)==1)
{
    printf("%d*%d",bloc,position);
}

*/
//fermer(fichier);


 /*

int i;
index tab[1000];
for(i=0;i<1000;i++)
     {
         tab[i].bloc=0;
         tab[i].cle=0;
         tab[i].pos=0;
     }
for(i=0;i<1000;i++)
{
    inser_index(rand(),tab,i,i,i);
}
int test;
FILE *fic=fopen("index.txt","a+");


        for(test=0;test<1000;test++)
            {
                fprintf(fic,"%d)--%d*%d*%d--\n",test,tab[test].bloc,tab[test].pos,tab[test].cle);
            }*/

 /* char anne[5]="\0";
  textbackground(BLUE);
  gotoxy(8,10);
  printf("19");
            Input_Number(anne,10,10,2);
printf("%s",anne);
*/
//}












