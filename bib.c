#include <stdio.h>
#include <stdlib.h>
#include"bib.h"
#include"conio.h"
#include"conio2.h"
#include"input.h"
#include"string++.h"
#include"bib_LLC.h"
int nb_ecdire;
int nb_liredire;
//*********************************** les modeles pour manupiler le fichier*******************************************************//

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

void liredir(L7OVC *fichier, int i , Buffer *buf)
{

 fseek(fichier->fich,(sizeof(Entete)+sizeof(Tbloc)*(i-1)),SEEK_SET); // positionnement au debut du bloc numero i
 fread(buf,sizeof(Buffer),1,fichier->fich);                         //lecture d'un bloc de caractère correspondant a la taille du bloc dans le buffer
 rewind(fichier->fich);                                            // repositionnement endebut de fichier

}
//-----------------------------------------------------------------------------------------

void ecriredir(L7OVC *fichier, int i, Buffer *buf)
{

     fseek(fichier->fich,sizeof(Entete)+sizeof(Tbloc)*(i-1),SEEK_SET); // positionnement au debut du bloc numero i
     fwrite(buf,sizeof(Buffer),1,fichier->fich);                       //ecriture du contenu du buffer dans le bloc numero i du fichier
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

int entete(L7OVC *fichier, int i)
{

    switch(i)
    {
        case 1:
        {
            return(fichier->entete.nbbloc);
        }break;
         case 2:
        {
            return(fichier->entete.tete);
        }break;
         case 3:
        {
            return(fichier->entete.queue);
        }break;
         case 4:
        {
            return(fichier->entete.indice_libre);
        }break;
         case 5:
        {
            return(fichier->entete.nb_car_sup);
        }break;
        case 6:
        {
            return(fichier->entete.nb_enreg);
        }break;

    };
}
//-----------------------------------------------------------------------------------------
void aff_entete(L7OVC *fichier,int i , int valeur)
{

    switch(i)
    {
        case 1:
        {
            fichier->entete.nbbloc=valeur; // nombre total de bloc alloués dans le fichier
        }break;
         case 2:
        {
            fichier->entete.tete=valeur;  // numero du bloc representatnt la tete du fichier
        }break;
         case 3:
        {
            fichier->entete.queue=valeur;  // numero du bloc representatnt la tete duf icheir
        }break;
         case 4:
        {
            fichier->entete.indice_libre=valeur; // la postion libre dans le bloc de queue
        }break;
         case 5:
        {
            fichier->entete.nb_car_sup=valeur;  // nombre de caractères suprimé depuis la
                                                  //création du ficher afin de lancer la réorganiosation
        }break;

        case 6:
        {
            fichier->entete.nb_enreg=valeur;
        }break;

    };
}
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
L7OVC *ouvrir(char nom_physique[], char mode)
{

    L7OVC *fichier=malloc(sizeof(L7OVC));                      // allocation  de la structure
    Buffer buf;
    if( (mode=='A') ||  (mode == 'a') )                        // mode ancien
    {
        fichier->fich=fopen(nom_physique,"rb+");               // ouverture du fichier en mode binaire lecture et ecriture
        fread(&(fichier->entete),sizeof(Entete),1,fichier->fich);  // chargement de l'entete enregistrée en debut de fichier
    }
    else
    {
        if( (mode=='N') ||  (mode == 'n') )                   // mode nouveau
        {
             fichier->fich=fopen(nom_physique,"wb+");         // ouverture du fichier en mode binaire  ecriture
             aff_entete(fichier,1,0);                         // mise a zeo du nombre de bloc si entete(fichier,1)=0 alors le fichier est vide
             aff_entete(fichier,2,1);                         // mettre tete au premier bloc
             aff_entete(fichier,3,1);                         // de meme pour la queue puisque au debut tete est queue sont confondues
             aff_entete(fichier,4,0);                         // le premier caractère du ficheir correspond a laposition libre puisqu'il est nouveau
             aff_entete(fichier,5,0);                         // aucun caractère n'a encore été supprimé
             fwrite(&(fichier->entete),sizeof(Entete),1,fichier->fich);// enregistrement de l'entete dans le fichier
             buf.suivant=-1;                                 // le suivant du premier bloc a NULL
             sprintf(buf.tab,"%s","");                       // initialisation du buffer a chaine vide
             ecriredir(fichier,1,&buf);
             nb_ecdire++;                     // ecriture du premier bloc dans le fichier


        }
        else                                                // format d'ouverture incorrecte
        {
            printf("format d'ouverture impossible");
        }
    }
    return(fichier);                                       // renvoyer la structure créée
}
//-----------------------------------------------------------------------------------------

void fermer(L7OVC *fichier)  // procedure de fermeture du fichier
{
    rewind(fichier->fich); // repositionnement du curseur en debut de fichier
    fwrite(&(fichier->entete),sizeof(Entete),1,fichier->fich); // sauvegarde de la dernière version de l'entete de la strucuture L7OVC
                                                                      //dans le fichier
    rewind(fichier->fich);// repositionnement du curseur en debut de fichier
    fclose(fichier->fich);// fermeture du fichier

}
//-----------------------------------------------------------------------------------------
void  alloc_bloc(L7OVC *fichier)
{

        Buffer *buf=malloc(sizeof(Buffer));       // allocation du Buffer
        liredir(fichier,entete(fichier,3),buf);   // lecture du bloc correspondant a la queue
        nb_liredire++;
        buf->suivant=entete(fichier,1)+1;         // mise a jour dui suvant de la queue au bloc correspondant a la nouvelle queue
        ecriredir(fichier,entete(fichier,3),buf);
        nb_ecdire++; // ecriture du bloc de queue dans le fichier
        aff_entete(fichier,3,entete(fichier,1)+1);// mise a jour du numero du bloc correspondant a la nouvelle queue dan sl'entete
        buf->suivant=-1;                         // mise a jour du suivant a nill
        sprintf(buf->tab,"%s","");               // vider la chaine du buffer
        ecriredir(fichier,entete(fichier,3),buf);// ecriture du buffer dans le bloc representatnt la nouvelle queue
        nb_ecdire++;
        aff_entete(fichier,1,entete(fichier,1)+1);// incrémentation du nombre de bloc alloués
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

//*********************************************module generation aleatoire***************************************************

char* genere_matricule()//generer un matricule aleatoire
{
    char tab[7];
	int i;

	do
	{
		i=rand()*31;

	}while( i<111111 || i>999999 );//generer un nombre de 6 positions
	//tab=malloc(sizeof(char)*7);
	itoa(i,tab,10);//convert entier to an char
	tab[6]='\0';

	return tab;

}
//-----------------------------------------------------------------------------------------
char* genere_nom_prenom()//generer un nom aleatoire
{
	char nom[34];
	char Alpha[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','K','R','S','T','U','V','W','X','Y','Z'};
	char Num[10]={'0','1','2','3','4','5','6','7','8','9'};

	int taille,i,save=0;

	do
	{
		taille=rand();

	}while( taille<4 || taille>30 );//pour la taille du nom
   // printf("**%d**",taille);

	//printf("(%d)",sizeof(char)*(taille+3));
	nom[taille+3]='\0';
//	printf("/*/%d/*/",strlen(nom));
	for(i=3;i<taille+3;i++)
	{
		do
	  {
		save=rand();

      }while( save<0 || save>25 );
     // printf("(%d)",save);
      nom[i]=Alpha[save];
     //printf("(%c)",nom[i]);

	}
//	printf("/*/%d/*/",strlen(nom));
	i=2;
	save=0;
//	taille=taille+1;
	while(i>=0)//ecrire la taille dans les 3 premiere potition
	{
		save=taille % 10;
		nom[i]=Num[save];

		taille=taille/10;
		i--;
	}

//	printf("%d/",strlen(nom));
//	printf("%s\n",nom);
	return nom;


}
//-----------------------------------------------------------------------------------------
char*genere_wilaya()//generer wilaya aliatoire
{
	char Wilaya_naissance[48][20]={"Adrar","Chlef","Laghouat","Oum El_Bouaghi","Batna","Bejaia","Biskra","Bechar","Blida","Bouira","Tamenrasset","Tebessa","Telemcen","Tiaret","Tizi_Ouzou","Alger","Djelfa","Jijel","Setif","Saida","Sakikda","Sidi_Bel_Abbas","Annaba","Guelma","Constantine","Medea","Mostaganem","M'sila","Mascara","Ouargla","Oran","El_Bayedh","Illizi","Bordj Bou_Arreridj","Boumerdès","El_Taref","Tindouf","Tissemsilt","El_Oued","Khenchela","Souk_Ahras","Tipaza","Mila","Ain_Defla","Naama","Ain_Temouchent","Ghardaia","Relizane"};
	char tab[21];
	char Num[10]={'0','1','2','3','4','5','6','7','8','9'};
	int i,ii,save,taille;
   for(i=0;i<21;i++)
	{
		tab[i]='\0';
	}
	do
	{
		ii=rand();

	}while( ii<0 || ii>47 );
	//printf("/%d/",ii);
	//tab=malloc(sizeof(char)*(strlen(Wilaya_naissance[ii])+4));
//	printf("(%d)",strlen(Wilaya_naissance[ii])+4);
taille=strlen(Wilaya_naissance[ii]);
 // printf("*%d*",taille);
     i=2;
     save=0;
	while(i>=0)
	{
		save=taille % 10;
		tab[i]=Num[save];

		taille=taille/10;
		i--;
	}
//printf("**%s**",tab);
   strcat(tab,Wilaya_naissance[ii]);
//printf("**%s**",tab);
	tab[strlen(Wilaya_naissance[ii])+3]='\0';

	return tab;


}
//-----------------------------------------------------------------------------------------
char*genere_grade()
{
	char Grade[18][30]={"General_de_corps_d_armee","General-Major","General","Colonel","Lieutenant-colonel","Commandant","Capitaine","Lieutenant","Sous-lieutenant","Aspirant","Adjudant-chef","Adjudant","Sergentchef","Sergent","Caporal-chef","Caporal","Djoundi"};
	char tab[31];
	char Num[10]={'0','1','2','3','4','5','6','7','8','9'};
	int i,ii,save,taille;
    for(i=0;i<31;i++)
	{
		tab[i]='\0';
	}
	do
	{
		ii=rand();

	}while( ii<0 || ii>16 );
//	printf("/%d/",ii);
	//tab=malloc(sizeof(char)*(strlen(Grade[ii])+4));
//	printf("(%d)",strlen(Wilaya_naissance[ii])+4);
taille=strlen(Grade[ii]);
  //printf("%d*",taille);
     i=2;
     save=0;
	while(i>=0)
	{
		save=taille % 10;
		tab[i]=Num[save];

		taille=taille/10;
		i--;
	}

   strcat(tab,Grade[ii]);
//printf("**%s**",tab);
	tab[strlen(Grade[ii])+3]='\0';

	return tab;
}
//-----------------------------------------------------------------------------------------
char*genere_force()
{

	char Force_armee[8][47]={"Armee_de_terre","Armee_de_l_air","Marine_nationale","Defense_aerienne_du_territoire","Gendarmerie_nationale","Garde_republicaine","Departement_du_renseignement_et_de_la_securite","Sante_militaire"};
	char tab[48];
	char Num[10]={'0','1','2','3','4','5','6','7','8','9'};
	int i,ii,save,taille;
    for(i=0;i<48;i++)
	{
		tab[i]='\0';
	}
	do
	{
		ii=rand();

	}while( ii<0 || ii>7 );
//	printf("/%d/",ii);
	//tab=malloc(sizeof(char)*(strlen(Force_armee[ii])+4));
//	printf("(%d)",strlen(Wilaya_naissance[ii])+4);
taille=strlen(Force_armee[ii]);
  //printf("%d*",taille);
     i=2;
     save=0;
	while(i>=0)
	{
		save=taille % 10;
		tab[i]=Num[save];

		taille=taille/10;
		i--;
	}

   strcat(tab,Force_armee[ii]);
//printf("**%s**",tab);
	tab[strlen(Force_armee[ii])+3]='\0';

	return tab;
}
//-----------------------------------------------------------------------------------------
char*genere_region()
{

    char Region_militaire[7][25]={"1RM-Blida","2RM-Oran","3RM-Bechar","4RM-Ouargla","5RM-Constantine","6RM-Tamanrasset"};
	char tab[26];
	char Num[10]={'0','1','2','3','4','5','6','7','8','9'};
	int i,ii,save,taille;

	for(i=0;i<26;i++)
	{
		tab[i]='\0';
	}

	do
	{
		ii=rand();

	}while( ii<0 || ii>5 );
//	printf("/%d/",ii);
	//tab=malloc(sizeof(char)*(strlen(Region_militaire[ii])+4));
//	printf("(%d)",strlen(Wilaya_naissance[ii])+4);
taille=strlen(Region_militaire[ii]);
  //printf("%d*",taille);
     i=2;
     save=0;
	while(i>=0)
	{
		save=taille % 10;
		tab[i]=Num[save];

		taille=taille/10;
		i--;
	}

   strcat(tab,Region_militaire[ii]);
//printf("**%s**",tab);
	tab[strlen(Region_militaire[ii])+3]='\0';

	return tab;
}
//-----------------------------------------------------------------------------------------
char*genere_sang()
{
		char Num[10]={'0','1','2','3','4','5','6','7','8','9'};
    char Groupe_Sanguin[8][5]={"O+","A+","B+","O-","A-","AB+","B-","AB-"};
	char tab[6];

	int ii,save,taille,i;

	for(i=0;i<6;i++)
	{
		tab[i]='\0';
	}

	do
	{
		ii=rand();

	}while( ii<0 || ii>7 );
//	printf("/%d/",ii);
	//tab=malloc(sizeof(char)*(strlen(Groupe_Sanguin[ii])+4));
//	printf("(%d)",strlen(Wilaya_naissance[ii])+4);
taille=strlen(Groupe_Sanguin[ii]);
  //printf("%d*",taille);
     i=2;
     save=0;
	while(i>=0)
	{
		save=taille % 10;
		tab[i]=Num[save];

		taille=taille/10;
		i--;
	}

   strcat(tab,Groupe_Sanguin[ii]);
//printf("**%s**",tab);
	tab[strlen(Groupe_Sanguin[ii])+3]='\0';

	return tab;
}
//-----------------------------------------------------------------------------------------

int maxjour(int y,int m){

    /** DONNEZ LE MAXIMAN JOUR DE LA MOIS
     ** IL PRENDS COMPS LE ANNES DES 29 ET 28 FIVIRIER
     **/

     switch (m) {

     case 1: return 31;
     case 2: return ((y%4)==0) ? 29 : 28 ;
     case 3: return 31;
     case 4: return 30;
     case 5: return 31;
     case 6: return 30;
     case 7: return 31;
     case 8: return 31;
     case 9: return 30;
     case 10: return 31;
     case 11: return 30;
     case 12: return 31;

     }
}
//-----------------------------------------------------------------------------------------
char*genere_date()
{
	char tab[8];
	int y,m,j;

	do
	{
		y=rand();

	}while( y<1940 || y>1999 );
	do
	{
		m=rand();

	}while( m<1 || m>12 );
	do
	{
		j=rand();

	}while( j<1 || j>maxjour(y,m));


	itoa(y*10000+m*100+j,tab,10);
	return tab;

}
//-----------------------------------------------------------------------------------------
char*genere_enreg()
{
	int i,taille,save;
	char tab[500];
	char enreg[500];
	char Num[10]={'0','1','2','3','4','5','6','7','8','9'};

	for(i=0;i<500;i++)
	{
		tab[i]='\0';
	}


		strcat(tab,genere_matricule());

		tab[6]='0';

		strcat(tab,genere_date());

		strcat(tab,genere_nom_prenom());

		strcat(tab,genere_nom_prenom());
		strcat(tab,genere_wilaya());
		strcat(tab,genere_force());
		strcat(tab,genere_grade());
		strcat(tab,genere_region());
		strcat(tab,genere_sang());


		for(i=0;i<500;i++)
	{
		enreg[i]='\0';
	}

        taille=strlen(tab);
		i=2;
        save=0;
     	while(i>=0)
    	{
	    	save=taille % 10;
		    enreg[i]=Num[save];
	    	taille=taille/10;
	    	i--;
    	}


	strcat(enreg,tab);
	//printf("**%s**",enreg);

	return enreg;
}

//-----------------------------------------------------------------------------------------
void lire_enreg(char*tab,int *matrr,char date[9],char eff[2],char nom[32],char prenom[32],char wilaya[22],char force[48],char grade[31],char region[26],char sang[5])
{//decomposer l'enreg en champ et les mettre en sortie

    // FILE *fic=fopen("contennue.txt","a+");
	int save_prenom,taille_nom,taille_prenom,save_wilaya,taille_wilaya,taille_force,save_force,i,taille_grade,save_grade,taille_region,save_region,taille_sang,save_sang;
	char taille[3];

	char matr[7];


	/*taille[0]=tab[0];
	taille[1]=tab[1];
	taille[2]=tab[2];
	printf("la taille de l'enreg: %d\n",atoi(taille));//trouver la taille de l'enreg*/

	matr[0]=tab[0];
	matr[1]=tab[1];
	matr[2]=tab[2];
	matr[3]=tab[3];
	matr[4]=tab[4];
	matr[5]=tab[5];
	matr[6]='\0';
	*matrr=atoi(matr);//recuperration du matricule

	//printf("le matricule: %d\n",atoi(matr));
	//fprintf(fic,"le matricule: %d\n",atoi(matr));

    eff[0]=tab[6];
    eff[1]='\0';//recuperation du champ effacement logique

	//printf("effacer: %c\n",tab[6]);

	date[0]=tab[7];
	date[1]=tab[8];
	date[2]=tab[9];
	date[3]=tab[10];
	date[4]=tab[11];
	date[5]=tab[12];
	date[6]=tab[13];
	date[7]=tab[14];
	date[8]='\0';//recupuration de la date

	//printf("date: %s\n",date);

	taille[0]=tab[15];
	taille[1]=tab[16];
	taille[2]=tab[17];
	taille_nom=atoi(taille);//taille du nom

	//char*nom=malloc(sizeof(char)*(taille_nom+1));
	nom[taille_nom]='\0';
	for(i=0;i<taille_nom;i++)
	{
		nom[i]=tab[18+i];
	}
	//printf("nom: %s\n",nom);

	save_prenom=18+taille_nom;//recuperer la position du debut du champ
	//printf("**%d**",save_prenom);
	taille[0]=tab[save_prenom];//recuperer la taille
	taille[1]=tab[save_prenom+1];
	taille[2]=tab[save_prenom+2];
//	printf("**%s**",taille);
	taille_prenom=atoi(taille);
	//printf("**%d**",taille_prenom);

	//char*prenom=malloc(sizeof(char)*(taille_prenom+1));
	prenom[taille_prenom]='\0';
    for(i=0;i<taille_prenom;i++)
	{
		prenom[i]=tab[save_prenom+3+i];
	}
	//printf("prenom: %s\n",prenom);

	save_wilaya=save_prenom+3+taille_prenom;//recuperer la position du debut du champ
//	printf("**%d**",save_wilaya);
//	printf("**%c**",tab [save_wilaya]);
	taille[0]=tab[save_wilaya];//recuperer la taille
	taille[1]=tab[save_wilaya+1];
	taille[2]=tab[save_wilaya+2];
//	printf("**%s**",taille);
	taille_wilaya=atoi(taille);
//	printf("**%d**",taille_wilaya);

	//char*wilaya=malloc(sizeof(char)*(taille_wilaya+1));
	wilaya[taille_wilaya]='\0';
    for(i=0;i<taille_wilaya;i++)
	{
		wilaya[i]=tab[save_wilaya+3+i];
	}
	//printf("wilaya: %s\n",wilaya);

	save_force=save_wilaya+3+taille_wilaya;//recuperer la position du debut du champ
//	printf("**%d**",save_wilaya);
//	printf("**%c**",tab [save_wilaya]);
	taille[0]=tab[save_force];//recuperer la taille
	taille[1]=tab[save_force+1];
	taille[2]=tab[save_force+2];
//	printf("**%s**",taille);
	taille_force=atoi(taille);
//	printf("**%d**",taille_wilaya);

	//char*force=malloc(sizeof(char)*(taille_force+1));
	force[taille_force]='\0';
    for(i=0;i<taille_force;i++)
	{
		force[i]=tab[save_force+3+i];
	}
	//printf("force: %s\n",force);

	save_grade=save_force+3+taille_force;//recuperer la position du debut du champ
//	printf("**%d**",save_wilaya);
//	printf("**%c**",tab [save_wilaya]);
	taille[0]=tab[save_grade];//recuperer la taille
	taille[1]=tab[save_grade+1];
	taille[2]=tab[save_grade+2];
//	printf("**%s**",taille);
	taille_grade=atoi(taille);
//	printf("**%d**",taille_wilaya);

	//char*grade=malloc(sizeof(char)*(taille_grade+1));
	grade[taille_grade]='\0';
    for(i=0;i<taille_grade;i++)
	{
		grade[i]=tab[save_grade+3+i];
	}
	//printf("grade: %s\n",grade);

	save_region=save_grade+3+taille_grade;//recuperer la position du debut du champ
//	printf("**%d**",save_wilaya);
//	printf("**%c**",tab [save_wilaya]);
	taille[0]=tab[save_region];//recuperer la taille
	taille[1]=tab[save_region+1];
	taille[2]=tab[save_region+2];
//	printf("**%s**",taille);
	taille_region=atoi(taille);
//	printf("**%d**",taille_wilaya);

	//char*region=malloc(sizeof(char)*(taille_region+1));
	region[taille_region]='\0';
    for(i=0;i<taille_region;i++)
	{
		region[i]=tab[save_region+3+i];
	}
	//printf("region: %s\n",region);

	save_sang=save_region+3+taille_region;//recuperer la position du debut du champ
//	printf("**%d**",save_wilaya);
//	printf("**%c**",tab [save_wilaya]);
	taille[0]=tab[save_sang];//recuperer la taille
	taille[1]=tab[save_sang+1];
	taille[2]=tab[save_sang+2];
//	printf("**%s**",taille);
	taille_sang=atoi(taille);
//	printf("**%d**",taille_wilaya);

//	char*sang=malloc(sizeof(char)*(taille_sang+1));
	sang[taille_sang]='\0';
    for(i=0;i<taille_sang;i++)
	{
		sang[i]=tab[save_sang+3+i];
	}
	//printf("sang: %s\n",sang);
	//printf("------------------------------------------------------------------\n");
}
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

void lire(char *chaine, int longueur)
{
     char *positionEntree = NULL;
      // On lit le texte saisi au clavier
     if (fgets(chaine, longueur, stdin) != NULL) // Pas d'erreur de saisie ?
    {
        positionEntree = strchr(chaine, '\n'); // On recherche l'"Entrée"
        if (positionEntree != NULL) // Si on a trouvé le retour à la ligne
        {
             *positionEntree = '\0'; // On remplace ce caractère par \0
        }
    }
}





void rech_index_initiale(int cle,int nb_enreg_inser,index*tab,int*trouv,int*i)
{
    int inf,sup,millieu;

    inf=0;
    *trouv=0;
    sup=nb_enreg_inser-1;
    while(*trouv==0 && inf<=sup)
    {
        millieu=(sup+inf)/2;
        if(cle<tab[millieu].cle)
        {
            sup=millieu-1;
        }
        else
        {
            if(cle>tab[millieu].cle)
            {
                inf=millieu+1;
            }
            else
            {
                //printf("+%d+",cle);
                *trouv=1;
                *i=millieu;
            }
        }
    }
    if(*trouv==0)
    {
        *i=inf;

    }
}

void inser_index(int cle,index*tab,int bloc,int pos,int nb_enreg_inser)
{
    int ii,k,test;
    int trouvv=0;
    if(nb_enreg_inser==0)
    {

        tab[0].cle=cle;
        tab[0].pos=pos;
        tab[0].bloc=bloc;

    }
    else
    {
        rech_index_initiale(cle,nb_enreg_inser,tab,&trouvv,&ii);
//printf("++%d++  ",ii);

        if(trouvv==0)
        {
            k=nb_enreg_inser-1;

           // printf("++%d++  ",k);
            while(k>=ii)
            {
                tab[k+1].cle=tab[k].cle;
                tab[k+1].bloc=tab[k].bloc;
                tab[k+1].pos=tab[k].pos;

                k--;
            }
            tab[ii].cle=cle;
            tab[ii].bloc=bloc;
            tab[ii].pos=pos;
           /* gotoxy(1,5);
            printf("%d**\n",tab[ii].cle);*/

        }
        else
        {printf("double**%d**",nb_enreg_inser);system("pause");}
    }
}

void ecrire_chaine(L7OVC *fichier,int n , int *i, int *j, char *enreg,int *cpt,Buffer *buf,int insser,index*tab,index_date*tabbb,index_date*tabbbb)
{
     char cle[7];
     char datee[5];
     int matrrr;

    int k=0;
    (*cpt)=0;     // nombre de bloc ajoutés
    for(k=0;k<n;k++)   // k pourn le deplacement dans la chaine
    {
        if(k==0)
            {
                //printf("**%d|%d**",*j,*i);
                cle[0]=enreg[3];
                cle[1]=enreg[4];
                cle[2]=enreg[5];
                cle[3]=enreg[6];
                cle[4]=enreg[7];
                cle[5]=enreg[8];
                cle[6]='\0';

                //printf("%s**\n",cle);
                matrrr=atoi(cle);

                inser_index(matrrr,tab,*i,*j,insser);

                datee[0]=enreg[10];
                datee[1]=enreg[11];
                datee[2]=enreg[12];
                datee[3]=enreg[13];
                datee[4]='\0';
                tabbb[insser].annee=atoi(datee);
                tabbb[insser].cle=matrrr;

                tabbbb[insser].cle=matrrr;
                char errore[49]={"\0"};
                char eror_enreg[200]={"\0"};
                int ero;

                for(ero=0;ero<=strlen(enreg)-3;ero++)
                {
                    eror_enreg[ero]=enreg[ero+3];

                }

                char saveregion[26];
                char numregion[2];
                lire_enreg(eror_enreg,&ero,errore,errore,errore,errore,errore,errore,errore,saveregion,errore);

                numregion[0]=saveregion[0];
                numregion[1]='\0';
                //printf("****%s*****",enreg);
                tabbbb[insser].annee=atoi(numregion);




            }//pour sauvgarder la premiere pisition et num du bloc por l'index*/
        if((*j)<=1022)  //si je suis toujours dans le bloc conserné
        {
            buf->tab[*j]=enreg[k]; // affectation des caractères de la chaine dans le buffer un a un
            (*j)++;                  // deplacement da,s le buffer
        }
        else                          // si la chaine a inserer depasse le buffer
         {
            if((*j)==1023){buf->tab[1023]='\0';}
            ecriredir(fichier,*i,buf);  // ecriture du precedent buffer dans le fichier
nb_ecdire++;
            alloc_bloc(fichier);        // alocation d'un nouveau bloc afin de recevoir le reste de la chaine
           // printf("%s\n",*buf.tab);
            buf->tab[0]=enreg[k];      // ecrtiture du kiem caractère de la chaine dans la position 0
            (*j)=1;                     // passage a la position 1
            (*i)=entete(fichier,3);     // deplacement dans les bloc du ficher
            (*cpt)++;                   // incrementation du nombre de bloc alloues
        }
    }
    buf->tab[*j]='\0';                  // fin de la chaine

}



void inserttion(L7OVC*fichier,char*enreg,int inser,index*tab,Buffer *buf,index_date*tabbb,index_date*tabbbb)
{//printf("********");
    int i,j,cpt; // declaration des variables



    if(entete(fichier,1)==0) // si le fichier esst vide on insere dans le premier bloc
    {
        aff_entete(fichier,1,1);
    }
        cpt=0;
        i=entete(fichier,3); // positionnelent dans le queue
        j=entete(fichier,4);  // positionnnement a la position libre dans le bloc de queue
        liredir(fichier,i,buf); // lecture du  bloc
nb_liredire++;
        ecrire_chaine(fichier,strlen(enreg),&i,&j,enreg,&cpt,buf,inser,tab,tabbb,tabbbb); // ecriture de la chaine construite


        ecriredir(fichier,i,buf); // ecriture du dernier bloc en cas de chevauchement
        nb_ecdire++;
        aff_entete(fichier,3,i); // mise a jour de la quee
        aff_entete(fichier,4,j); // mise a jour de la position libre



}

void chargement_initiale(L7OVC *fichier,int n) // procedure de création du ficher
{
    //printf("%d",n);
   // printf("%s",genere_enreg());
int m;
Buffer buf;
    int k,i,nb=0;
    char tab[500];
     char matrr[7];
     int matrrr,pass;
     index *tabb=malloc(sizeof(index)*n);
     index_date*tabbb=malloc(sizeof(index_date)*n);
     index_date*tabbbb=malloc(sizeof(index_date)*n);

     int *cless=malloc(sizeof(int)*n);
     for(i=0;i<n;i++)
     {
         cless[i]=0;
     }
     for(i=0;i<n;i++)
     {
         tabb[i].bloc=0;
         tabb[i].cle=0;
         tabb[i].pos=0;
     }
    for(i=0;i<500;i++)
	{
		tab[i]='\0';
	}//initialisation

    aff_entete(fichier,1,1);
    //for(k=0;k<n;k++)
    k=0;
while(k!=n)
    {
        gotoxy(1,1);
        printf("%d",nb);

        strcpy(tab,genere_enreg());
        matrr[0]=tab[3];
	    matrr[1]=tab[4];
	    matrr[2]=tab[5];
	    matrr[3]=tab[6];
	    matrr[4]=tab[7];
	    matrr[5]=tab[8];
	    matrr[6]='\0';
        matrrr=atoi(matrr);

        //printf("%d**\n",cless[k]);
        if(k==0)
        {
            cless[k]=matrrr;
            inserttion(fichier,tab,nb,tabb,&buf,tabbb,tabbbb);
k++;
            nb++;
        }
        else
        {
            pass=rech_cle(cless,n,matrrr);
          //  if(pass==1){printf("double matr");system("pause");}
            cless[k]=matrrr;
            if( pass==0)
            {
                inserttion(fichier,tab,nb,tabb,&buf,tabbb,tabbbb);
                //printf("%s\n\n\n",buf.tab);
                nb++;k++;srand(time(NULL));
            }
        }
    }

aff_entete(fichier,6,nb+1);
FILE*fic=fopen("index_matr","wb+");
FILE*ficc=fopen("index_date","wb+");
FILE*ficcc=fopen("index_region","wb+");

 fwrite(tabb,sizeof(index)*nb,1,fic);
 fwrite(tabbb,sizeof(index_date)*nb,1,ficc);
 fwrite(tabbbb,sizeof(index_date)*nb,1,ficcc);
 fclose(fic); fclose(ficc); fclose(ficcc);
 /*
    for(i=0;i<nb;i++)
    {
        fprintf(fic,"%04d)%04d*%04d*%04d\n",i,tabb[i].bloc,tabb[i].pos,tabb[i].cle);
    }*/
//printf("nb enreg: %d",entete(fichier,6));

}

int rech_cle(int*tab,int n,int cle)
{
    int i=0;
    int trouv=0;
    while(i<n && trouv==0)
    {
        if(tab[i]==cle)
        {
            trouv=1;
        }

        i++;
    }
    return trouv;
}











int recherch(L7OVC *fichier,int cle,int*bloc,int*position)//recherche sans index
{//system("pause");
    int trouv=0;
    int pos,i,j,suivant,k,ii,stop;
    Buffer buf;
    char taille[4];
    char tab[500];

    if(entete(fichier,1)!=0)
    {
        pos=0;j=0;i=1;
        stop=0;
        liredir(fichier,i,&buf);
        nb_liredire++;
       // printf("%s\n",buf.tab);
      // system("pause");
        while(stop==0 && trouv==0)
        {
            if(j==entete(fichier,4) && i==entete(fichier,3) )
            {
                stop=1;
                break;
            }
            for(ii=0;ii<499;ii++)
	        {
	        	tab[ii]='\0';
	        }
            if(pos<=max-4)
            {
                taille[0]=buf.tab[pos];

                taille[1]=buf.tab[pos+1];

                taille[2]=buf.tab[pos+2];
                taille[3]='\0';


            }
            else
            {
                if(pos==max-3)
                {
                    taille[0]=buf.tab[pos];
                    taille[1]=buf.tab[pos+1];
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    taille[2]=buf.tab[0];
                    taille[3]='\0';
                    i++;
                    pos=-2;
                }
                else
                {
                    if(pos==max-2)
                    {
                        taille[0]=buf.tab[pos];
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[1]=buf.tab[0];
                        taille[2]=buf.tab[1];
                        taille[3]='\0';
                        pos=-1;
                    }
                    else
                    {
                        pos=0;
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[0]=buf.tab[pos];
                        taille[1]=buf.tab[pos+1];
                        taille[2]=buf.tab[pos+2];
                        taille[3]='\0';
                    }

                }
            }
            //printf("**%d**\n",atoi(taille));
            j=pos+3;
            for(k=0;k<=atoi(taille)-1;k++)
            {
                if(k==0)
                    {
                        *bloc=i;
                        *position=pos;

                }
                if(j<=1022)
                {
                    tab[k]=buf.tab[j];
                    j++;
                    pos=j;
                }
                else
                {
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    i++;
                    tab[k]=buf.tab[0];
                    j=1;
                    pos=j;
                }
            }
           // system("pause");
            //printf("%s\n\n\n",tab);


           int matr;
           char date[9],eff[2],nom[32],prenom[32],wilaya[22],force[48],grade[31],region[26],sang[5];
           lire_enreg(tab,&matr,date,eff,nom,prenom,wilaya,force,grade,region,sang);

           if(matr==cle)
           {
               trouv=1;

           }

        }
    }
    return trouv;
}












void affich_enreg_par_enreg(L7OVC *fichier)//afficher tous le fichier enreg par enreg sans index
{//system("pause");
    int pos,i,j,suivant,k,ii,stop,num;//FILE*fic=fopen("indexx.txt","a+");
    Buffer buf;
    char taille[4];
    char tab[500];
num=1;
    if(entete(fichier,1)!=0)
    {
        pos=0;j=0;i=1;
        stop=0;
        liredir(fichier,i,&buf);
        nb_liredire++;
       // printf("%s\n",buf.tab);
      // system("pause");
        while(stop==0)
        {
            if(j==entete(fichier,4) && i==entete(fichier,3) )//si on arrive a la fin du fichier la derniere pos dans le dernier bloc
            {
                stop=1;
                break;
            }
            for(ii=0;ii<499;ii++)
	        {
	        	tab[ii]='\0';
	        }//initialisation
            if(pos<=max-4)//si ona (...xxx)xxx designe la taille
            {
                taille[0]=buf.tab[pos];

                taille[1]=buf.tab[pos+1];

                taille[2]=buf.tab[pos+2];
                taille[3]='\0';


            }
            else
            {
                if(pos==max-3)//si(..xx)
                {
                    taille[0]=buf.tab[pos];
                    taille[1]=buf.tab[pos+1];
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    taille[2]=buf.tab[0];
                    taille[3]='\0';
                    i++;
                    pos=-2;
                }
                else
                {
                    if(pos==max-2)//(...x)
                    {
                        taille[0]=buf.tab[pos];
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[1]=buf.tab[0];
                        taille[2]=buf.tab[1];
                        taille[3]='\0';
                        pos=-1;
                    }
                    else//(xxx...)
                    {
                        pos=0;
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[0]=buf.tab[pos];
                        taille[1]=buf.tab[pos+1];
                        taille[2]=buf.tab[pos+2];
                        taille[3]='\0';
                    }

                }
            }
            //printf("**%d**\n",atoi(taille));
            j=pos+3;
            for(k=0;k<=atoi(taille)-1;k++)
            {
                if(k==0){printf("potition dans le bloc: %d\n",pos);printf("num du bloc: %d\n",i);}
                if(j<=1022)
                {
                    tab[k]=buf.tab[j];
                    j++;
                    pos=j;
                }
                else
                {
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    i++;
                    tab[k]=buf.tab[0];
                    j=1;
                    pos=j;
                }
            }
           // system("pause");
            //printf("%s\n\n\n",tab);
            printf("%d--\n",num);
           num++;
           int matrrr;
           char date[9],eff[2],nom[32],prenom[32],wilaya[22],force[48],grade[31],region[26],sang[5];
           lire_enreg(tab,&matrrr,date,eff,nom,prenom,wilaya,force,grade,region,sang);

           printf("matricule: %d\n",matrrr);


        //fprintf(fic,"%d)%d*\n",num,matrrr);

           printf("eff: %s\n",eff);
           printf("date: %s\n",date);
           printf("nom: %s\n",nom);
           printf("prenom: %s\n",prenom);
           printf("wilaya: %s\n",wilaya);
           printf("force: %s\n",force);
           printf("grade: %s\n",grade);
           printf("region: %s\n",region);
           printf("sang: %s\n",sang);
           printf("---------------------------------------------------\n");
           // system("pause");
        }
    }
}





void affich_enreg_par_position(L7OVC *fichier,int num_bloc,int pos_dans_bloc,int nb)//afficher un anreg a partir de sa position
{


    int pos,i,j,suivant,k,ii,stop;//FILE*fic=fopen("indexx.txt","a+");
    Buffer buf;
    char taille[4];
    char tab[500];

    if(entete(fichier,1)!=0)
    {
        pos=pos_dans_bloc;
        j=pos_dans_bloc;
        i=num_bloc;
        stop=0;
        liredir(fichier,i,&buf);
        nb_liredire++;
       // printf("%s\n",buf.tab);
      // system("pause");

            for(ii=0;ii<499;ii++)
	        {
	        	tab[ii]='\0';
	        }//initialisation

            if(pos<=max-4)//si ona (...xxx)xxx designe la taille
            {
                taille[0]=buf.tab[pos];

                taille[1]=buf.tab[pos+1];

                taille[2]=buf.tab[pos+2];
                taille[3]='\0';


            }
            else
            {
                if(pos==max-3)//si(..xx)
                {
                    taille[0]=buf.tab[pos];
                    taille[1]=buf.tab[pos+1];
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    taille[2]=buf.tab[0];
                    taille[3]='\0';
                    i++;
                    pos=-2;
                }
                else
                {
                    if(pos==max-2)//(...x)
                    {
                        taille[0]=buf.tab[pos];
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[1]=buf.tab[0];
                        taille[2]=buf.tab[1];
                        taille[3]='\0';
                        pos=-1;
                    }
                    else//(xxx...)
                    {
                        pos=0;
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[0]=buf.tab[pos];
                        taille[1]=buf.tab[pos+1];
                        taille[2]=buf.tab[pos+2];
                        taille[3]='\0';
                    }

                }
            }
            //printf("**%d**\n",atoi(taille));
            j=pos+3;
            for(k=0;k<=atoi(taille)-1;k++)
            {
                if(k==0){printf("\t\t\t\t\t\t\t\t\tpotition dans le bloc: %d\n",pos);printf("\t\t\t\t\t\t\t\t\tnum du bloc: %d\n\n",i);}
                if(j<=1022)
                {
                    tab[k]=buf.tab[j];
                    j++;
                    pos=j;
                }
                else
                {
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    i++;
                    tab[k]=buf.tab[0];
                    j=1;
                    pos=j;
                }
            }
           // system("pause");
            //printf("%s\n\n\n",tab);

           int matrrr;
           char date[9],eff[2],nom[32],prenom[32],wilaya[22],force[48],grade[31],region[26],sang[5];
           lire_enreg(tab,&matrrr,date,eff,nom,prenom,wilaya,force,grade,region,sang);

           printf("\t\t\t\t\t\t\t\t\tMatricule: %d\n\n",matrrr);


        //fprintf(fic,"%d)%d*\n",num,matrrr);

          // printf("eff: %s\n",eff);
           printf("\t\t\t\t\t\t\t\t\tDate_Naissance: %s\n\n",date);
           printf("\t\t\t\t\t\t\t\t\tNom: %s\n\n",nom);
           printf("\t\t\t\t\t\t\t\t\tPrenom: %s\n\n",prenom);
           printf("\t\t\t\t\t\t\t\t\tWilaya: %s\n\n",wilaya);
           printf("\t\t\t\t\t\t\t\t\tForce_Armée: %s\n\n",force);
           printf("\t\t\t\t\t\t\t\t\tGrade: %s\n\n",grade);
           printf("\t\t\t\t\t\t\t\t\tRégion_militaire: %s\n\n",region);
           printf("\t\t\t\t\t\t\t\t\tGroupe_Sanguin: %s\n\n",sang);
           printf("_______________________________________________________________________________________________________________________________________________________________________\n");
           // system("pause");

    }
}



void AffichezCadre(int x,int y,int Longueur ,int Hauteur,int ch,int color){

    /** Affichez un cadre avec cractère ch debutera par la postion (x,y)
     ** se termenra sur la postion (x+longueur-1,y+Hauteur-1)
     ** color est le color de cadre
     **/
     int colorText= 0 ;
     int colorbackground = 14;
     int i;
     if (ch==219) textbackground(color);
     textcolor(color);
     gotoxy(x,y);
     for (i=0;i<Longueur;i++) printf("%c",ch);
     gotoxy(x,y+Hauteur-1);
     for (i=0;i<Longueur;i++) printf("%c",ch);
     for (i=1;i<Hauteur-1;i++) {
        gotoxy(x,y+i);
        printf("%c",ch);
        gotoxy(x+Longueur-1,y+i);
        printf("%c",ch);
    }
     if (ch==219) textbackground(colorbackground);
     textcolor(colorText); /// return le color du text comme aupervant

     printf("\n");printf("\n");printf("\n");
}

void choix_creation_fich()
{
    char mode='n';
    char nom_fichier[30]={"\0"};
    L7OVC*fichier;
    char c;
    int NumbreInput=1;
    textbackground(WHITE);
    system("cls");
    textbackground(WHITE);
    textcolor(BLACK);
    cputsxy(66,5,"Nom du fichier");
    textbackground(BLACK);
    DrawRectangle(66,6,46,3);
    textbackground(BLACK);
    textcolor(WHITE);
    AfficheButton("Enregistrer",79,20,20,3);

    do{
        if (NumbreInput>0 && NumbreInput<2){


                if(NumbreInput==1)
                {
                    c=Input_Text(nom_fichier,66,8,29);
                }

                if (c==80 || c==81 || c==9 || c==13 )
               {
                  NumbreInput++;

                }
            else
                if (c==72 || c==73)
                {
                  NumbreInput--;
                  if (NumbreInput==0) NumbreInput=2;

                }
        }else
        {
            if (NumbreInput==2)
            {

            textbackground(BLUE);
            textcolor(WHITE);
            AfficheButton("Enregistrer",79,20,20,3);
            textcolor(WHITE);
            textbackground(BLACK);

            do{
                c=SaiserCntrl();
            }while(c==75 || c==77 ); // car here touch left and right ne fait rien
            if (c==13){


                    if (!(  nom_fichier[0])) {
                    // This is input vide

                    char bl[28]= {"                           "};

                    textcolor(RED);
                   textbackground(YELLOW);

                  AfficheButton("Le nom du fichier est vide",26,25,28,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                 // cputsxych(30,25,videe);
                  //Sleep(1500);
                  char cc;
                  cc=SaiserCntrl();
                  if(cc==72 || cc==73 || cc==80 || cc==81 || cc==9 || cc==75 || cc==77 || cc==13 )
                  {
                      cputsxych(26,25,bl);
                  cputsxych(26,26,bl);
                  cputsxych(26,27,bl);
                  textbackground(WHITE);
                gotoxy(53,25);
                printf("%c",219);
                gotoxy(53,26);
                printf("%c",219);
                gotoxy(53,27);
                printf("%c",219);
                textbackground(BLACK);

                  }
                    }
                    else
                    {
                        fichier=ouvrir(nom_fichier,mode);
                        chargement_initiale(fichier,1);
                         fermer(fichier);
                         return 0;

                    }


            }
            else
                  {
                    if (c==80 || c==81 || c==9 )
                    {
                        NumbreInput=1;
                        textbackground(BLACK);
                        textcolor(WHITE);
                        AfficheButton("Enregistrer",79,20,20,3);
                    }
                    else
                   {
                      if (c==72 || c==73)
                      {
                          NumbreInput=1;
                          textbackground(BLACK);
                          textcolor(WHITE);
                          AfficheButton("Enregistrer",79,20,20,3);
                      }
                }
        }

        }
        }
    }while(1);

}


int Nom_Fichier(char **nom){
    // affiche liste des fichiers et l'utilisatuer choiser un nom fichier pour l'utiliser
          char **NFile;
          int i,j,taille;
          File_Derectory_Current(&NFile,&taille);
          do{

          textbackground(WHITE);
          textcolor(BLACK);
          FrameRectangle(49,14,82,22,4);
          textcolor(BLACK);
          i=Menu(NFile,taille,50,15,80,20,WHITE,LIGHTGRAY);
          if (i!=0) {
            (*nom)=NFile[i-1];
            FILE* F=fopen(*nom,"r");
            if (F==NULL){
                textbackground(WHITE);
                system("CLS");
                textcolor(WHITE);
               // cputsxyTemp(10,2,"THIS is derecotory try again file \n\t",15);
                system("PAUSE");
                continue;
            }
            fclose(F);
          }
            break;
          }while(1);
          for (j=0;j<taille;j++) if ((j+1)!=i) free(NFile[j]);
          free(NFile);
        return i;
}



void choix_inser_alleatoire()
{
    textbackground(WHITE);
    system("cls");
    gotoxy(49,12);
    printf("Choix du fichier");
    char* nom_fic;
    int NumbreInput=1;
    char mode='a';
    char c;
    L7OVC*fichier;
    char n_enreg[7];
    if (Nom_Fichier(&nom_fic)==0) return 1;

    textbackground(WHITE);
    system("cls");
    textcolor(BLACK);
    cputsxy(66,5,"Nombre d'enregistrement a insserer");
    textbackground(BLACK);
    DrawRectangle(66,6,46,3);
    textbackground(BLACK);
    textcolor(WHITE);
    AfficheButton("Enregistrer",79,20,20,3);




     do{
        if (NumbreInput>0 && NumbreInput<2){


                if(NumbreInput==1)
                {
                    c=Input_Number(n_enreg,66,8,6);
                }

                if (c==80 || c==81 || c==9 || c==13 )
               {
                  NumbreInput++;

                }
            else
                if (c==72 || c==73)
                {
                  NumbreInput--;
                  if (NumbreInput==0) NumbreInput=12;

                }
        }else
        {
            if (NumbreInput==2)
            {

            textbackground(BLUE);
            textcolor(WHITE);
            AfficheButton("Enregistrer",79,20,20,3);
            textcolor(WHITE);
            textbackground(BLACK);

            do{
                c=SaiserCntrl();
            }while(c==75 || c==77 ); // car here touch left and right ne fait rien
            if (c==13){


                    if (!(  n_enreg[0])) {
                    // This is input vide

                    char bl[28]= {"                           "};

                    textcolor(RED);
                   textbackground(YELLOW);

                  AfficheButton("La case est vide",26,25,28,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                 // cputsxych(30,25,videe);
                  //Sleep(1500);
                  char cc;
                  cc=SaiserCntrl();
                  if(cc==72 || cc==73 || cc==80 || cc==81 || cc==9 || cc==75 || cc==77 || cc==13 )
                  {
                      cputsxych(26,25,bl);
                  cputsxych(26,26,bl);
                  cputsxych(26,27,bl);
                  textbackground(WHITE);
                gotoxy(53,25);
                printf("%c",219);
                gotoxy(53,26);
                printf("%c",219);
                gotoxy(53,27);
                printf("%c",219);
                textbackground(BLACK);

                  }
                    }
                    else
                    {
                        fichier=ouvrir(nom_fic,mode);
                         textcolor(RED);
                   textbackground(YELLOW);

                  AfficheButton("Patienter...",26,25,28,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                        chargement_initiale(fichier,atoi(n_enreg));
                         fermer(fichier);
                         return 0;

                    }


            }
            else
                  {
                    if (c==80 || c==81 || c==9 )
                    {
                        NumbreInput=1;
                        textbackground(BLACK);
                        textcolor(WHITE);
                        AfficheButton("Enregistrer",79,20,20,3);
                    }
                    else
                   {
                      if (c==72 || c==73)
                      {
                          NumbreInput=1;
                          textbackground(BLACK);
                          textcolor(WHITE);
                          AfficheButton("Enregistrer",79,20,20,3);
                      }
                }
        }

        }
        }
    }while(1);
}

void choix_inser_new_enreg()
{
    L7OVC*fichier;
    textbackground(WHITE);
    system("cls");
    gotoxy(49,12);
    printf("Choix du fichier");
    char* nom_fic;
    char mode='a';
    if (Nom_Fichier(&nom_fic)==0) return 1;

    fichier=ouvrir(nom_fic,mode);

    if(  inser_nouvel_enreg(fichier)==0)
                        {
                            textcolor(BLACK);
                            textbackground(WHITE);
                        }
}

void choix_requet_intervalle()
{
    L7OVC*fichier;
    char *Region_militaire[]={"1RM-Blida","2RM-Oran","3RM-Bechar","4RM-Ouargla","5RM-Constantine","6RM-Tamanrasset"};
    char *Age[]={"19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54","55","56","57","58","59","60","61","62","63","64","65","66","67","68","69","70","71","72","73","74","75","76","77","78"};
    textbackground(WHITE);
    system("cls");
    gotoxy(49,12);
    printf("Choix du fichier");
    char* nom_fic;
    char mode='a';
    if (Nom_Fichier(&nom_fic)==0) return 1;
    fichier=ouvrir(nom_fic,mode);

    int choix_region,choix_age1,choix_age2;
    char c;
    int NumbreInput=1;
    textbackground(WHITE);
    system("cls");
    textbackground(WHITE);
    textcolor(BLACK);
    cputsxy(66,5,"Region_militaire");
    cputsxy(66,10 ,"Age1");
    cputsxy(66,15 ,"Age2");

    textbackground(BLACK);
    DrawRectangle(66,6,46,3);
    DrawRectangle(66,11,46,3);
     DrawRectangle(66,16,46,3);
    textbackground(BLACK);
    textcolor(WHITE);
    AfficheButton("Chercher",79,20,20,3);

    do{
        if (NumbreInput>0 && NumbreInput<4){


                if(NumbreInput==1)
                {
                    choix_region=Menu(Region_militaire,6,66,7,30,1,BLACK,BLACK);
                    c=SaiserCntrl();
                    textbackground(BLACK);
                }
                else
                {
                    if(NumbreInput==2)
                    {
                        choix_age1=Menu(Age,60,66,12,30,1,BLACK,BLACK);
                        c=SaiserCntrl();
                        textbackground(BLACK);

                    }
                    else
                    {
                        if(NumbreInput==3)
                        {
                            choix_age2=Menu(Age,60,66,17,30,1,BLACK,BLACK);
                            c=SaiserCntrl();
                            textbackground(BLACK);
                        }

                    }
                }

            if (c==80 || c==81 || c==9 || c==13 )
               {
                  NumbreInput++;

                }
            else
                if (c==72 || c==73)
                {
                  NumbreInput--;
                  if (NumbreInput==0) NumbreInput=4;

                }
        }
        else if (NumbreInput==4)
            {

            textbackground(BLUE);
            textcolor(WHITE);
            AfficheButton("Chercher",79,20,20,3);
            textcolor(WHITE);
            textbackground(BLACK);

            do{
                c=SaiserCntrl();
            }while(c==75 || c==77 ); // car here touch left and right ne fait rien
            if (c==13){

            textbackground(WHITE);
            system("cls");

            textcolor(BLACK);
            rech_region_date(fichier,choix_region,choix_age1+18,choix_age2+18);
fermer(fichier);
textcolor(WHITE);
system("pause");
textcolor(BLACK);
              return 0;


                    }
                else
                  {
                    if (c==80 || c==81 || c==9 )
                    {
                        NumbreInput=1;
                        textbackground(BLACK);
                        textcolor(WHITE);
                        AfficheButton("Chercher",79,20,20,3);
                    }
                    else
                   {
                      if (c==72 || c==73)
                      {
                          NumbreInput=3;
                          textbackground(BLACK);
                          textcolor(WHITE);
                          AfficheButton("Chercher",79,20,20,3);
                      }
                }
        }


        }

	 }while(1);


}
//modif_region(nom_physique,598160,6);
void choix_modif_region()
{

int trouv,i;

    char *Region_militaire[]={"1RM-Blida","2RM-Oran","3RM-Bechar","4RM-Ouargla","5RM-Constantine","6RM-Tamanrasset"};
    textbackground(WHITE);
    system("cls");
    gotoxy(49,12);
    printf("Choix du fichier");
    char* nom_fic;
    char mode='a';
    if (Nom_Fichier(&nom_fic)==0) return 1;

    L7OVC*fichier=ouvrir(nom_fic,mode);
index *arrayy=malloc(sizeof(index)*(entete(fichier,6)+1));
    FILE *fic=fopen("index_matr","ab+");
    fread(arrayy,sizeof(index)*entete(fichier,6),1,fic);

    int choix_region;
    char c;
    int NumbreInput=1;
    textbackground(WHITE);
    system("cls");
    textbackground(WHITE);
    textcolor(BLACK);
    cputsxy(66,5,"Matricule");
    cputsxy(66,10 ,"Region militaire");

    textbackground(BLACK);
    DrawRectangle(66,6,46,3);
    DrawRectangle(66,11,46,3);

    textbackground(BLACK);
    textcolor(WHITE);
    AfficheButton("Modifier",79,20,20,3);
    char cle[7];

    do{
        if (NumbreInput>0 && NumbreInput<3){


                if(NumbreInput==1)
                {
                    c=Input_Number(cle,66,8,6);
                }
                else
                {
                    if(NumbreInput==2)
                    {
                        choix_region=Menu(Region_militaire,6,66,12,30,1,BLACK,BLACK);
                        c=SaiserCntrl();
                        textbackground(BLACK);

                    }

                }

            if (c==80 || c==81 || c==9 || c==13 )
               {
                  NumbreInput++;

                }
            else
                if (c==72 || c==73)
                {
                  NumbreInput--;
                  if (NumbreInput==0) NumbreInput=3;

                }
        }
        else if (NumbreInput==3)
            { textbackground(BLUE);
            textcolor(WHITE);
            AfficheButton("Modifier",79,20,20,3);
            textcolor(WHITE);
            textbackground(BLACK);

            do{
                c=SaiserCntrl();
            }while(c==75 || c==77 ); // car here touch left and right ne fait rien
            if (c==13){


                    if(verif_matricule(atoi(cle))==0)
                   {

                    char bl[39]=    {"                                      "};

                    textcolor(RED);
                   textbackground(YELLOW);
                  AfficheButton("Le matricule doit contenire 6 position",26,25,38,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                 // cputsxych(30,25,videe);
                    char cc=SaiserCntrl();
                  if(cc==72 || cc==73 || cc==80 || cc==81 || cc==9 || cc==75 || cc==77 || cc==13 )
                  {
                 // Sleep(1500);
                  cputsxych(26,25,bl);
                  cputsxych(26,26,bl);
                  cputsxych(26,27,bl);
                  }

                }

                else{rech_index_initiale(atoi(cle),entete(fichier,6),arrayy,&trouv,&i);

                if(trouv==0)
                {
                    char bl[39]=    {"                          "};

                    textcolor(RED);
                   textbackground(YELLOW);
                  AfficheButton("Le matricule n existe pas",26,25,25,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                 // cputsxych(30,25,videe);
                  //Sleep(1500);
                char cc=SaiserCntrl();
                  if(cc==72 || cc==73 || cc==80 || cc==81 || cc==9 || cc==75 || cc==77 || cc==13 )
                  {
                  cputsxych(26,25,bl);
                  cputsxych(26,26,bl);
                  cputsxych(26,27,bl);
                  }

                }
                else
                {

                    fermer(fichier);
                    fclose(fic);
//gotoxy(1,1);printf("%s**%d**%d",nom_fic,atoi(cle),choix_region);
//system("pause");
                    modif_region(nom_fic,atoi(cle),choix_region);
                    textcolor(BLACK);
                    return 0;

                }
                }





                    }
                else
                  {
                    if (c==80 || c==81 || c==9 )
                    {
                        NumbreInput=1;
                        textbackground(BLACK);
                        textcolor(WHITE);
                        AfficheButton("Modifier",79,20,20,3);
                    }
                    else
                   {
                      if (c==72 || c==73)
                      {
                          NumbreInput=2;
                          textbackground(BLACK);
                          textcolor(WHITE);
                          AfficheButton("Modifier",79,20,20,3);
                      }
                }
        }


        }

	 }while(1);
}


void choix_supr_enreg()
{

    int trouv,i;
    textbackground(WHITE);
    system("cls");
    gotoxy(49,12);
    printf("Choix du fichier");
    char* nom_fic;
    char mode='a';
    if (Nom_Fichier(&nom_fic)==0) return 1;

    L7OVC*fichier=ouvrir(nom_fic,mode);
index *arrayy=malloc(sizeof(index)*(entete(fichier,6)+1));
    FILE *fic=fopen("index_matr","ab+");
    fread(arrayy,sizeof(index)*entete(fichier,6),1,fic);

    char c;
    int NumbreInput=1;
    textbackground(WHITE);
    system("cls");
    textbackground(WHITE);
    textcolor(BLACK);
    cputsxy(66,5,"Matricule");

    textbackground(BLACK);
    DrawRectangle(66,6,46,3);

    textbackground(BLACK);
    textcolor(WHITE);
    AfficheButton("Supprimer",79,20,20,3);
    char cle[7];

    do{
        if (NumbreInput>0 && NumbreInput<2){


                if(NumbreInput==1)
                {
                    c=Input_Number(cle,66,8,6);
                }


            if (c==80 || c==81 || c==9 || c==13 )
               {
                  NumbreInput++;

                }
            else
                if (c==72 || c==73)
                {
                  NumbreInput--;
                  if (NumbreInput==0) NumbreInput=2;

                }
        }
        else if (NumbreInput==2)
            { textbackground(BLUE);
            textcolor(WHITE);
            AfficheButton("Supprimer",79,20,20,3);
            textcolor(WHITE);
            textbackground(BLACK);

            do{
                c=SaiserCntrl();
            }while(c==75 || c==77 ); // car here touch left and right ne fait rien
            if (c==13){


                    if(verif_matricule(atoi(cle))==0)
                   {

                    char bl[39]=    {"                                      "};

                    textcolor(RED);
                   textbackground(YELLOW);
                  AfficheButton("Le matricule doit contenire 6 position",26,25,38,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                 // cputsxych(30,25,videe);
                    char cc=SaiserCntrl();
                  if(cc==72 || cc==73 || cc==80 || cc==81 || cc==9 || cc==75 || cc==77 || cc==13 )
                  {
                 // Sleep(1500);
                  cputsxych(26,25,bl);
                  cputsxych(26,26,bl);
                  cputsxych(26,27,bl);
                  }

                }

                else{rech_index_initiale(atoi(cle),entete(fichier,6),arrayy,&trouv,&i);

                if(trouv==0)
                {
                    char bl[39]=    {"                          "};

                    textcolor(RED);
                   textbackground(YELLOW);
                  AfficheButton("Le matricule n existe pas",26,25,25,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                 // cputsxych(30,25,videe);
                  //Sleep(1500);
                char cc=SaiserCntrl();
                  if(cc==72 || cc==73 || cc==80 || cc==81 || cc==9 || cc==75 || cc==77 || cc==13 )
                  {
                  cputsxych(26,25,bl);
                  cputsxych(26,26,bl);
                  cputsxych(26,27,bl);
                  }

                }
                else
                {

                    fermer(fichier);
                    fclose(fic);
//gotoxy(1,1);printf("%s**%d**%d",nom_fic,atoi(cle),choix_region);
//system("pause");

                    supr_enreg(nom_fic,atoi(cle));
                    textcolor(BLACK);
                    return 0;

                }
                }





                    }
                else
                  {
                    if (c==80 || c==81 || c==9 )
                    {
                        NumbreInput=1;
                        textbackground(BLACK);
                        textcolor(WHITE);
                         AfficheButton("Supprimer",79,20,20,3);
                    }
                    else
                   {
                      if (c==72 || c==73)
                      {
                          NumbreInput=2;
                          textbackground(BLACK);
                          textcolor(WHITE);
                           AfficheButton("Supprimer",79,20,20,3);
                      }
                }
        }


        }

	 }while(1);

}

void choix_supr_relatif_force()
{

    int trouv,i;
    textbackground(WHITE);
    system("cls");
    gotoxy(49,12);
    printf("Choix du fichier");
    char* nom_fic;
    char mode='a';
    if (Nom_Fichier(&nom_fic)==0) return 1;

    char *Force_armee[]={"Armee_de_terre","Armee_de_l_air","Marine_nationale","Defense_aerienne_du_territoire","Gendarmerie_nationale","Garde_republicaine","Departement_du_renseignement_et_de_la_securite","Sante_militaire"};



    char c;
    int NumbreInput=1;
    textbackground(WHITE);
    system("cls");
    textbackground(WHITE);
    textcolor(BLACK);
    cputsxy(66,5,"Force armee");

    textbackground(BLACK);
    DrawRectangle(66,6,52,3);

    textbackground(BLACK);
    textcolor(WHITE);
    AfficheButton("Supprimer",79,20,20,3);
    int choix_force;
    do{
        if (NumbreInput>0 && NumbreInput<2){


                if(NumbreInput==1)
                {

                    choix_force=Menu(Force_armee,8,66,7,52,1,BLACK,BLACK);
                    c=SaiserCntrl();
                    textbackground(BLACK);
                }


            if (c==80 || c==81 || c==9 || c==13 )
               {
                  NumbreInput++;

                }
            else
                if (c==72 || c==73)
                {
                  NumbreInput--;
                  if (NumbreInput==0) NumbreInput=2;

                }
        }
        else if (NumbreInput==2)
            { textbackground(BLUE);
            textcolor(WHITE);
            AfficheButton("Supprimer",79,20,20,3);
            textcolor(WHITE);
            textbackground(BLACK);

            do{
                c=SaiserCntrl();
            }while(c==75 || c==77 ); // car here touch left and right ne fait rien
            if (c==13){

                    textcolor(RED);
                   textbackground(YELLOW);
                  AfficheButton("Patienter...",26,25,28,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                    supr_enreg_relatif_force(nom_fic,choix_force);

                    return 0;

                }


                else
                  {
                    if (c==80 || c==81 || c==9 )
                    {
                        NumbreInput=1;
                        textbackground(BLACK);
                        textcolor(WHITE);
                         AfficheButton("Supprimer",79,20,20,3);
                    }
                    else
                   {
                      if (c==72 || c==73)
                      {
                          NumbreInput=2;
                          textbackground(BLACK);
                          textcolor(WHITE);
                           AfficheButton("Supprimer",79,20,20,3);
                      }
                }
        }

            }


	 }while(1);
}

void choix_affich_tous_de_grade()
{
        int trouv,i;
    textbackground(WHITE);
    system("cls");
    gotoxy(49,12);
    printf("Choix du fichier");
    char* nom_fic;
    char mode='a';
    if (Nom_Fichier(&nom_fic)==0) return 1;
    L7OVC*fichier;
    fichier=ouvrir(nom_fic,mode);
    char *Grade[]={"Officiers-generaux","officiers-superieurs", "officiers", "sous-officiers","hommes de troupes"};



    char c;
    int NumbreInput=1;
    textbackground(WHITE);
    system("cls");
    textbackground(WHITE);
    textcolor(BLACK);
    cputsxy(73,5,"Categorie de grades");

    textbackground(BLACK);
    DrawRectangle(73,6,30,3);

    textbackground(BLACK);
    textcolor(WHITE);
    AfficheButton("Recherche",79,20,20,3);
    int choix_grade;
    do{
        if (NumbreInput>0 && NumbreInput<2){


                if(NumbreInput==1)
                {

                    choix_grade=Menu(Grade,5,73,7,30,1,BLACK,BLACK);
                    c=SaiserCntrl();
                    textbackground(BLACK);
                }


            if (c==80 || c==81 || c==9 || c==13 )
               {
                  NumbreInput++;

                }
            else
                if (c==72 || c==73)
                {
                  NumbreInput--;
                  if (NumbreInput==0) NumbreInput=2;

                }
        }
        else if (NumbreInput==2)
            { textbackground(BLUE);
            textcolor(WHITE);
            AfficheButton("Recherche",79,20,20,3);
            textcolor(WHITE);
            textbackground(BLACK);

            do{
                c=SaiserCntrl();
            }while(c==75 || c==77 ); // car here touch left and right ne fait rien
            if (c==13){


                    textbackground(WHITE);
                    system("cls");
                    textcolor(BLACK);
                    affich_grade_donner(fichier,choix_grade);
                    system("pause");


                    return 0;

                }


                else
                  {
                    if (c==80 || c==81 || c==9 )
                    {
                        NumbreInput=1;
                        textbackground(BLACK);
                        textcolor(WHITE);
                         AfficheButton("Recherche",79,20,20,3);
                    }
                    else
                   {
                      if (c==72 || c==73)
                      {
                          NumbreInput=2;
                          textbackground(BLACK);
                          textcolor(WHITE);
                           AfficheButton("Recherche",79,20,20,3);
                      }
                }
        }

            }


	 }while(1);
}

void choix_affich_3premier_bloc()
{
    textbackground(WHITE);
    system("cls");
    char* nom_fic;
    L7OVC*fichier;
    char mode='a';

    if (Nom_Fichier(&nom_fic)==0) return 1;

    fichier=ouvrir(nom_fic,mode);
    FILE*fic=fopen("tous le fichier.txt","a+");
    Buffer buf;
    int ioi=1;
    textbackground(WHITE);
    system("cls");
    printf("\n\n\n");
    printf("\t\t\tLe nombre de bloc: %d\n\n",entete(fichier,1));
    printf("\t\t\tLa tete du fichier: %d\n\n",entete(fichier,2));
    printf("\t\t\tLa queue du fichier: %d\n\n",entete(fichier,3));
    printf("\t\t\tindice libre: %d\n\n",entete(fichier,4));
    printf("\t\t\tnombre d enregistrement: %d\n\n",entete(fichier,6));

    textcolor(WHITE);
    system("pause");


    system("cls");
    while(ioi<=entete(fichier,1))
    {
        liredir(fichier,ioi,&buf);
        nb_liredire++;
        textbackground(WHITE);
        textcolor(BLACK);
        printf("\n%s\n\n\n",buf.tab);
        ioi++;
    }
     int pos,i,j,suivant,k,ii,stop,num;//FILE*fic=fopen("indexx.txt","a+");

    char taille[4];
    char tab[500];
num=1;
    if(entete(fichier,1)!=0)
    {
        pos=0;j=0;i=1;
        stop=0;
        liredir(fichier,i,&buf);
        nb_liredire++;
       // printf("%s\n",buf.tab);
      // system("pause");
        while(stop==0)
        {
            if(j==entete(fichier,4) && i==entete(fichier,3) )//si on arrive a la fin du fichier la derniere pos dans le dernier bloc
            {
                stop=1;
                break;
            }
            for(ii=0;ii<499;ii++)
	        {
	        	tab[ii]='\0';
	        }//initialisation
            if(pos<=max-4)//si ona (...xxx)xxx designe la taille
            {
                taille[0]=buf.tab[pos];

                taille[1]=buf.tab[pos+1];

                taille[2]=buf.tab[pos+2];
                taille[3]='\0';


            }
            else
            {
                if(pos==max-3)//si(..xx)
                {
                    taille[0]=buf.tab[pos];
                    taille[1]=buf.tab[pos+1];
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    taille[2]=buf.tab[0];
                    taille[3]='\0';
                    i++;
                    pos=-2;
                }
                else
                {
                    if(pos==max-2)//(...x)
                    {
                        taille[0]=buf.tab[pos];
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[1]=buf.tab[0];
                        taille[2]=buf.tab[1];
                        taille[3]='\0';
                        pos=-1;
                    }
                    else//(xxx...)
                    {
                        pos=0;
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[0]=buf.tab[pos];
                        taille[1]=buf.tab[pos+1];
                        taille[2]=buf.tab[pos+2];
                        taille[3]='\0';
                    }

                }
            }
            //printf("**%d**\n",atoi(taille));
            j=pos+3;
            for(k=0;k<=atoi(taille)-1;k++)
            {
                if(k==0){fprintf(fic,"potition dans le bloc: %d\n",pos);fprintf(fic,"num du bloc: %d\n",i);}
                if(j<=1022)
                {
                    tab[k]=buf.tab[j];
                    j++;
                    pos=j;
                }
                else
                {
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    i++;
                    tab[k]=buf.tab[0];
                    j=1;
                    pos=j;
                }
            }
           // system("pause");
            //printf("%s\n\n\n",tab);
            fprintf(fic,"%d--\n",num);
           num++;
           int matrrr;
           char date[9],eff[2],nom[32],prenom[32],wilaya[22],force[48],grade[31],region[26],sang[5];
           lire_enreg(tab,&matrrr,date,eff,nom,prenom,wilaya,force,grade,region,sang);

            fprintf(fic,"Matricule: %d\n\n",matrrr);


        //fprintf(fic,"%d)%d*\n",num,matrrr);

          // printf("eff: %s\n",eff);
           fprintf(fic,"Date_Naissance: %s\n\n",date);
           fprintf(fic,"Nom: %s\n\n",nom);
           fprintf(fic,"Prenom: %s\n\n",prenom);
           fprintf(fic,"Wilaya: %s\n\n",wilaya);
           fprintf(fic,"Force_Armée: %s\n\n",force);
           fprintf(fic,"Grade: %s\n\n",grade);
           fprintf(fic,"Région_militaire: %s\n\n",region);
           fprintf(fic,"Groupe_Sanguin: %s\n\n",sang);
           fprintf(fic,"_____________________________________________\n");

        }

        fclose(fic);
        fermer(fichier);
    }







    textcolor(WHITE);
    system("pause");
}






void cputsxych(int x,int y,char* a){
textcolor(RED);
    textbackground(WHITE);
    int i=0;
    gotoxy(x,y);
    while(a[i]!='\0'){
        printf("%c",a[i++]);
      //  Sleep(10);
    }
    textcolor(WHITE);
    textbackground(BLACK);

}



int inser_nouvel_enreg(L7OVC *fichier)
{
    system("mode con  cols=168");
    textbackground(WHITE);

    system("cls");
    index *arrayy=malloc(sizeof(index)*(entete(fichier,6)+1));
    index_date*arrayyy=malloc(sizeof(index_date)*(entete(fichier,6)+1));
    index_date*arrayyyy=malloc(sizeof(index_date)*(entete(fichier,6)+1));

    FILE *fic=fopen("index_matr","ab+");
    FILE *ficc=fopen("index_date","ab+");
    FILE *ficcc=fopen("index_region","ab+");

    fread(arrayy,sizeof(index)*entete(fichier,6),1,fic);
    fread(arrayyy,sizeof(index_date)*entete(fichier,6),1,ficc);
    fread(arrayyyy,sizeof(index_date)*entete(fichier,6),1,ficcc);

int choix_region=0,choix_force=0,choix_grade=0,choix_sang=0,choix_wilaya=0;
    char enreg1[200];
    char matrr[7]={"\0"};
    char nom[31]={"\0"};
    char prenom[31]={"\0"};
    char jour[3]={"\0"};
    char mois[3]={"\0"};
    char annee[3]={"\0"};
    char datee[9];int date;
	int matr,trouv,i;
   trouv=1;
   int NumbreInput=1;
   char c;
    textcolor(BLACK);
    char *Wilaya_naissance[]={"Adrar","Chlef","Laghouat","Oum El_Bouaghi","Batna","Bejaia","Biskra","Bechar","Blida","Bouira","Tamenrasset","Tebessa","Telemcen","Tiaret","Tizi_Ouzou","Alger","Djelfa","Jijel","Setif","Saida","Sakikda","Sidi_Bel_Abbas","Annaba","Guelma","Constantine","Medea","Mostaganem","M'sila","Mascara","Ouargla","Oran","El_Bayedh","Illizi","Bordj Bou_Arreridj","Boumerdès","El_Taref","Tindouf","Tissemsilt","El_Oued","Khenchela","Souk_Ahras","Tipaza","Mila","Ain_Defla","Naama","Ain_Temouchent","Ghardaia","Relizane"};
    char*Force_armee[]={"Armee_de_terre","Armee_de_l_air","Marine_nationale","Defense_aerienne_du_territoire","Gendarmerie_nationale","Garde_republicaine","Departement_du_renseignement_et_de_la_securite","Sante_militaire"};
    char *Groupe_Sanguin[]={"O+","A+","B+","O-","A-","AB+","B-","AB-"};
    char *Grade[]={"General_de_corps_d_armee","General-Major","General","Colonel","Lieutenant-colonel","Commandant","Capitaine","Lieutenant","Sous-lieutenant","Aspirant","Adjudant-chef","Adjudant","Sergentchef","Sergent","Caporal-chef","Caporal","Djoundi"};
    char *Region_militaire[]={"1RM-Blida","2RM-Oran","3RM-Bechar","4RM-Ouargla","5RM-Constantine","6RM-Tamanrasset"};
    cputsxy(66,5,"Matricule");
    cputsxy(66,10 ,"NOM");
    cputsxy(66,15 ,"PRENOM");
    cputsxy(66,20 ,"DATE_Naissance");
    cputsxy(66,26 ,"Wilaya_Naissance");
    cputsxy(66,32 ,"Groupe_Sanguin");
    cputsxy(66,38 ,"Grade");
    cputsxy(66,44 ,"Force_Armée");
    cputsxy(66,50 ,"Region_militaire");
     textbackground(BLACK);
     DrawRectangle(66,6,46,3);
     DrawRectangle(66,11,46,3);
     DrawRectangle(66,16,46,3);
     DrawRectangle(67,22,2,3);
     DrawRectangle(73,22,2,3);
     DrawRectangle(78,22,4,3);
     DrawRectangle(66,28,46,3);
     DrawRectangle(66,34,46,3);
     DrawRectangle(66,40,46,3);
     DrawRectangle(66,46,46,3);
     DrawRectangle(66,52,46,3);
     textcolor(WHITE);
     cputsxy(78,24 ,"19");
     textbackground(WHITE);
     textcolor(RED);
     cputsxy(66,21 ,"Jour");
     cputsxy(72,21 ,"Mois");
     cputsxy(78,21 ,"Annee");


	 textbackground(BLACK);
      textcolor(WHITE);
      AfficheButton("Enregistrer",80,57,20,3);
	 do{
        if (NumbreInput>0 && NumbreInput<12){


                if(NumbreInput==1)
                {
                    c=Input_Number(matrr,66,8,6);
                }
                else
                {
                    if(NumbreInput==2)
                    {
                        c=Input_Text(nom,66,13,30);
                    }
                    else
                    {
                        if(NumbreInput==3)
                        {
                            c=Input_Text(prenom,66,18,30);
                        }
                        else
                        {
                            if(NumbreInput==4)
                            {

                                c=Input_Number(jour,67,24,2);
                            }
                            else
                            {
                                if(NumbreInput==5)
                                {
                                    c=Input_Number(mois,73,24,2);
                                }
                                else
                                {
                                    if(NumbreInput==6)
                                    {
                                        c=Input_Number(annee,80,24,2);
                                    }
                                    else
                                    {
                                        if(NumbreInput==7)
                                        {
                                            choix_wilaya=Menu(Wilaya_naissance,48,74,29,30,1,BLACK,BLACK);
                                            c=SaiserCntrl();
                                            textbackground(BLACK);

                                        }
                                        else
                                        {
                                            if(NumbreInput==8)
                                            {
                                                choix_sang=Menu(Groupe_Sanguin,8,74,35,30,1,BLACK,BLACK);
                                                 c=SaiserCntrl();
                                                textbackground(BLACK);
                                            }
                                            else
                                            {
                                                if(NumbreInput==9)
                                                {
                                                    choix_grade=Menu(Grade,17,74,41,30,1,BLACK,BLACK);
                                                    c=SaiserCntrl();
                                                    textbackground(BLACK);
                                                }
                                                else
                                                    if (NumbreInput==10)
                                                {
                                                     choix_force=Menu(Force_armee,8,74,47,30,1,BLACK,BLACK);
                                                    c=SaiserCntrl();
                                                    textbackground(BLACK);
                                                }
                                                else
                                                    if(NumbreInput==11)
                                                {
                                                     choix_region=Menu(Region_militaire,6,74,53,30,1,BLACK,BLACK);
                                                    c=SaiserCntrl();
                                                    textbackground(BLACK);
                                                }
                                            }
                                        }
                                    }

                                }
                            }
                        }
                    }
                }


            if (c==80 || c==81 || c==9 || c==13 )
               {
                  NumbreInput++;

                }
            else
                if (c==72 || c==73)
                {
                  NumbreInput--;
                  if (NumbreInput==0) NumbreInput=12;

                }
        }
        else if (NumbreInput==12)
            {

            textbackground(BLUE);
            textcolor(WHITE);
            AfficheButton("Enregistrer",80,57,20,3);
            textcolor(WHITE);
            textbackground(BLACK);

            do{
                c=SaiserCntrl();
            }while(c==75 || c==77 ); // car here touch left and right ne fait rien
            if (c==13){
                // here est create new account
                if (!(matrr[0] && nom[0] && prenom[0] && jour[0] && mois[0] &&  annee[0])|| choix_region==0 || choix_sang==0 || choix_grade==0|| choix_force==0||choix_wilaya==0) {
                    // This is input vide

                    char bl[28]= {"                           "};

                    textcolor(RED);
                   textbackground(YELLOW);

                  AfficheButton("Il y a au moin une case vide",26,25,28,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                 // cputsxych(30,25,videe);
                  //Sleep(1500);
                  char cc;
                  cc=SaiserCntrl();
                  if(cc==72 || cc==73 || cc==80 || cc==81 || cc==9 || cc==75 || cc==77 || cc==13 )
                  {
                      cputsxych(26,25,bl);
                  cputsxych(26,26,bl);
                  cputsxych(26,27,bl);
                  textbackground(WHITE);
                gotoxy(53,25);
                printf("%c",219);
                gotoxy(53,26);
                printf("%c",219);
                gotoxy(53,27);
                printf("%c",219);
                textbackground(BLACK);

                  }



                }
               else{ if(verif_matricule(atoi(matrr))==0)
                {

                    char bl[39]=    {"                                      "};

                    textcolor(RED);
                   textbackground(YELLOW);
                  AfficheButton("Le matricule doit contenire 6 position",26,25,38,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                 // cputsxych(30,25,videe);
                    char cc=SaiserCntrl();
                  if(cc==72 || cc==73 || cc==80 || cc==81 || cc==9 || cc==75 || cc==77 || cc==13 )
                  {
                 // Sleep(1500);
                  cputsxych(26,25,bl);
                  cputsxych(26,26,bl);
                  cputsxych(26,27,bl);
                  }

                }

                else{rech_index_initiale(atoi(matrr),entete(fichier,6),arrayy,&trouv,&i);

                if(trouv==1)
                {
                    char bl[39]=    {"                    "};

                    textcolor(RED);
                   textbackground(YELLOW);
                  AfficheButton("Le matricule existe",26,25,19,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                 // cputsxych(30,25,videe);
                  //Sleep(1500);
                char cc=SaiserCntrl();
                  if(cc==72 || cc==73 || cc==80 || cc==81 || cc==9 || cc==75 || cc==77 || cc==13 )
                  {
                  cputsxych(26,25,bl);
                  cputsxych(26,26,bl);
                  cputsxych(26,27,bl);
                  }

                }
            else{
                char date_19[5]={"\0"};
                date_19[0]='1';
                date_19[1]='9';
                if(annee[0]=='\0')
                {
                    date_19[2]='0';
                }
                else
                {
                    date_19[2]=annee[0];
                }
                if(annee[1]=='\0')
                {
                    date_19[3]='0';
                }
                else
                {
                    date_19[3]=annee[1];
                }

                date_19[4]='\0';
                if(verif_annee(atoi(date_19))==0)
                {
                    char bl[39]= {"                                   "};

                    textcolor(RED);
                   textbackground(YELLOW);

                  AfficheButton("L' annee doit etre entre 1940..1999",20,25,35,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                 // cputsxych(30,25,videe);
                char cc=SaiserCntrl();
                  if(cc==72 || cc==73 || cc==80 || cc==81 || cc==9 || cc==75 || cc==77 || cc==13 )
                  {
                  //Sleep(1700);
                  cputsxych(20,25,bl);
                  cputsxych(20,26,bl);
                  cputsxych(20,27,bl);
                  }
                }

                else{if(atoi(mois)>13 || atoi(mois)==0 )
                {
                     char bl[39]= {"                                  "};

                    textcolor(RED);
                   textbackground(YELLOW);

                  AfficheButton("Le mois doit etre enter 1..12",20,25,29,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                 // cputsxych(30,25,videe);
                char cc=SaiserCntrl();
                  if(cc==72 || cc==73 || cc==80 || cc==81 || cc==9 || cc==75 || cc==77 || cc==13 )
                  {
                //  Sleep(1700);
                  cputsxych(20,25,bl);
                  cputsxych(20,26,bl);
                  cputsxych(20,27,bl);
                  }
                }
                else{if(maxjour(atoi(date_19),atoi(mois))<atoi(jour))
                {
                    char bl[39]= {"                           "};

                    textcolor(RED);
                   textbackground(YELLOW);

                  AfficheButton("Le jour est faux",20,25,16,3);
                  textcolor(WHITE);
                   textbackground(BLACK);
                 // cputsxych(30,25,videe);
                                   char cc=SaiserCntrl();
                  if(cc==72 || cc==73 || cc==80 || cc==81 || cc==9 || cc==75 || cc==77 || cc==13 )
                  {
                  //Sleep(1700);
                  cputsxych(20,25,bl);
                  cputsxych(20,26,bl);
                  cputsxych(20,27,bl);
                  }

                }
                else{




                int iii,taille;

                for(iii=0;iii<200;iii++)
	            {
	               	enreg1[iii]='\0';
                }

                strcat(enreg1,matrr);
                enreg1[6]='0';
                strcat(enreg1,date_19);
                strcat(enreg1,mois);
                strcat(enreg1,jour);

                taille=strlen(nom);

                char tailleee[4];
                iii=2;
                int save=0;
                char Num[10]={'0','1','2','3','4','5','6','7','8','9'};
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;

            	}
            	tailleee[3]='\0';
            	strcat(enreg1,tailleee);
            	strcat(enreg1,nom);



                taille=strlen(prenom);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(enreg1,tailleee);
            	strcat(enreg1,prenom);


            	taille=strlen(Wilaya_naissance[choix_wilaya-1]);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(enreg1,tailleee);
            	strcat(enreg1,Wilaya_naissance[choix_wilaya-1]);


            	taille=strlen(Force_armee[choix_force-1]);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(enreg1,tailleee);
            	strcat(enreg1,Force_armee[choix_force-1]);


            	taille=strlen(Grade[choix_grade-1]);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(enreg1,tailleee);
            	strcat(enreg1,Grade[choix_grade-1]);


            	taille=strlen(Region_militaire[choix_region-1]);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(enreg1,tailleee);
            	strcat(enreg1,Region_militaire[choix_region-1]);


            	taille=strlen(Groupe_Sanguin[choix_sang-1]);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(enreg1,tailleee);
            	strcat(enreg1,Groupe_Sanguin[choix_sang-1]);

            	char enreg[200];
            	for(iii=0;iii<200;iii++)
	            {
		          enreg[iii]='\0';
	            }

	            taille=strlen(enreg1);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(enreg,tailleee);

            	strcat(enreg,enreg1);





Buffer buf;


       inserttion(fichier,enreg,entete(fichier,6),arrayy,&buf,arrayyy,arrayyyy);
      aff_entete(fichier,6,entete(fichier,6)+1);
     // printf("**%d**",arrayy[entete(fichier,6)-2].cle);
     gotoxy(1,1);

      remove("index_matr");
      remove("index_date");
      remove("index_region");
      fclose(fic);
      fclose(ficc);
      fclose(ficcc);

      FILE *fic=fopen("index_matr","rb+");
      FILE *ficc=fopen("index_date","rb+");
      FILE *ficcc=fopen("index_region","rb+");


      arrayyy[entete(fichier,6)-1].cle=atoi(matrr);
      arrayyy[entete(fichier,6)-1].annee=atoi(date_19);

      arrayyyy[entete(fichier,6)-1].cle=atoi(matrr);
      arrayyyy[entete(fichier,6)-1].annee=choix_region;

    //  printf("**%d**\n",arrayy[entete(fichier,6)-1].cle);
       fwrite(arrayy,sizeof(index)*(entete(fichier,6)),1,fic);
       fwrite(arrayyy,sizeof(index_date)*(entete(fichier,6)),1,ficc);
       fwrite(arrayyyy,sizeof(index_date)*(entete(fichier,6)),1,ficcc);
       fclose(fic);
       fclose(ficc);
       fclose(ficcc);


/*
       arrayy[entete(fichier,6)-1].cle=0;
       fread(arrayy,sizeof(index),entete(fichier,6),fic);
       printf("**%d**",arrayy[entete(fichier,6)-1].cle);*/


fermer(fichier);
//printf("%s**",enreg);


                //c=getch();
                return 0;}}}}}}

                }
                else
                  {
                    if (c==80 || c==81 || c==9 )
                    {
                        NumbreInput=1;
                        textbackground(BLACK);
                        textcolor(WHITE);
                        AfficheButton("Enregistrer",80,57,20,3);
                    }
                    else
                   {
                      if (c==72 || c==73)
                      {
                          NumbreInput=11;
                          textbackground(BLACK);
                          textcolor(WHITE);
                          AfficheButton("Enregistrer",80,57,20,3);
                      }
                }
        }


        }

	 }while(1);
/*while(trouv==1)
   {
if(verif_matricule(matr)==1)
     {
         rech_index_initiale(matr,entete(fichier,6),arrayy,&trouv,&i);
	     if(trouv==1)
         {
             printf("matricule exisete\n");
         }
     }
     else
     {
         printf("le matr doit avoir 6 position\n");
     }

   }
	itoa(matr,matrr,10);//convert entier to an char
	matrr[6]='\0';
	printf("%s",matrr);

	printf("entrer l'anne: ");
    scanf("%d",&date);

*/
}
int verif_annee(int anne)
{
    if(anne<=1999 && anne>=1940)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int verif_matricule(int matr)
{
    if(matr/100000!=0 && matr/1000000==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



void rech_region_date(L7OVC*fichier,int region,int age1,int age2)
{
    maillon*p;
	maillon*q;
	maillon*pp;
	maillon*qq;
	maillon*tete1=NULL;
	maillon*tete2=NULL;
	maillon*tete3=NULL;

	int date1=Max(2018-age1,2018-age2);
	int date2=Min(2018-age1,2018-age2);

	int premier1=0;
	int premier2=0;

	int i,trouv=0,pos;


	index_date*tab=malloc(sizeof(index_date)*entete(fichier,6));
	index_date*tabb=malloc(sizeof(index_date)*entete(fichier,6));
	index *tabbb=malloc(sizeof(index)*entete(fichier,6));

	FILE *fic=fopen("index_region","rb+");
    FILE *ficc=fopen("index_date","rb+");
    FILE *ficcc=fopen("index_matr","rb+");

    fread(tab,sizeof(index),entete(fichier,6),fic);
    fread(tabb,sizeof(index_date),entete(fichier,6),ficc);
    fread(tabbb,sizeof(index),entete(fichier,6),ficcc);

    for(i=0;i<entete(fichier,6);i++)
    {
        if(tab[i].annee==region && premier1==1)
        {
            Allouerr(&q);
            Aff_Val(q,tab[i].cle);
            Aff_Adr(p,q);
            p=q;
        }
        if(tab[i].annee==region && premier1==0)
        {
            Allouerr(&p);
            Aff_Val(p,tab[i].cle);
            tete1=p;
            premier1=1;
        }

        if(tabb[i].annee<=date1 && tabb[i].annee>=date2 && premier2==1 )
        {
            Allouerr(&qq);
            Aff_Val(qq,tabb[i].cle);
            Aff_Adr(pp,qq);
            pp=qq;
        }

        if(tabb[i].annee<=date1 && tabb[i].annee>=date2 && premier2==0 )
        {
            Allouerr(&pp);
            Aff_Val(pp,tabb[i].cle);
            tete2=pp;
            premier2=1;
        }

    }
    Aff_Adr(p,NULL);
    Aff_Adr(pp,NULL);
    textcolor(WHITE);

gotoxy(1,1);printf("*******");

    intersection_LLC(tete1,tete2,&tete3);
    textcolor(BLACK);

    p=tete3;
int nbb=1;

    while(p!=NULL)
    {
        rech_index_initiale(Valeur(p),entete(fichier,6),tabbb,&trouv,&pos);
        if(trouv==1)
        {
            printf("\t\t\t\t\t\t\t\t\t%d-\n",nbb);
            affich_enreg_par_position(fichier,tabbb[pos].bloc,tabbb[pos].pos,nbb);
            nbb++;
        }
        p=Suivant(p);
    }
     textcolor(WHITE);
gotoxy(1,1);printf("*******");
fclose(fic);fclose(ficc);fclose(ficcc);fermer(fichier);

}

void intersection_LLC(maillon*tete1,maillon*tete2,maillon**tete3)
{
    maillon *p,*q,*z,*y;
    int premmier=0;
    *tete3=NULL;

    p=tete1;
    while(p!=NULL)
    {
        q=tete2;
        while(q!=NULL)
        {

            if(Valeur(p)==Valeur(q) && premmier==1)
            {
                Allouerr(&y);
                Aff_Val(y,Valeur(p));
                Aff_Adr(z,y);
                z=y;
            }
            if(Valeur(p)==Valeur(q) && premmier==0)
            {
                Allouerr(&z);
                Aff_Val(z,Valeur(p));
                *tete3=z;
                premmier=1;
            }
            q=Suivant(q);
        }
        p=Suivant(p);
    }
    Aff_Adr(z,NULL);
}

void supr_enreg(char nom_physique[30],int cle)
{
    int pos,i,j,suivant,k,ii,stop,num;
    Buffer buf;
    Buffer buff;
    char taille[4];
    char tab[200];
    char tabb[200]={"\0"};
    num=1;
    int nb=0;
    char nom_physiquee[10]={"aaaaaaaa"};
char mode='a';
    L7OVC *fichier=ouvrir(nom_physique,mode);;
    L7OVC *new_fichier;

    index *aray=malloc(sizeof(index)*(entete(fichier,6)-1));
    int ooo;
    for(ooo=0;ooo<(entete(fichier,6)-1);ooo++)
    {
        aray[ooo].bloc=0;
        aray[ooo].cle=0;
        aray[ooo].pos=0;
    }
    index_date*arayy=malloc(sizeof(index_date)*(entete(fichier,6)-1));
    for(ooo=0;ooo<(entete(fichier,6)-1);ooo++)
    {
        arayy[ooo].annee=0;
        arayy[ooo].cle=0;
    }
    index_date*arayyy=malloc(sizeof(index_date)*(entete(fichier,6)-1));
    for(ooo=0;ooo<(entete(fichier,6)-1);ooo++)
    {
        arayyy[ooo].annee=0;
        arayyy[ooo].cle=0;
    }



    remove("index_matr");
    remove("index_date");
    remove("index_region");

    char modee='n';




    new_fichier=ouvrir(nom_physiquee,modee);
   // chargement_initiale(new_fichier,1);

    FILE *fic=fopen("index_matr","ab+");
    FILE *ficc=fopen("index_date","ab+");
    FILE *ficcc=fopen("index_region","ab+");
if(entete(new_fichier,6)!=0)
{
    fread(aray,sizeof(index)*entete(new_fichier,6),1,fic);
    fread(arayy,sizeof(index_date)*entete(new_fichier,6),1,ficc);
    fread(arayyy,sizeof(index_date)*entete(new_fichier,6),1,ficcc);
}

    if(entete(fichier,1)!=0)
    {
        pos=0;j=0;i=1;
        stop=0;
        liredir(fichier,i,&buf);
        nb_liredire++;
       // printf("%s\n",buf.tab);
      // system("pause");
        while(stop==0)
        {
            if(j==entete(fichier,4) && i==entete(fichier,3) )//si on arrive a la fin du fichier la derniere pos dans le dernier bloc
            {
                stop=1;
                break;
            }
            for(ii=0;ii<200;ii++)
	        {
	        	tab[ii]='\0';
	        }//initialisation
	        for(ii=0;ii<200;ii++)
	        {
	        	tabb[ii]='\0';
	        }//initialisation
            if(pos<=max-4)//si ona (...xxx)xxx designe la taille
            {
                taille[0]=buf.tab[pos];

                taille[1]=buf.tab[pos+1];

                taille[2]=buf.tab[pos+2];
                taille[3]='\0';


            }
            else
            {
                if(pos==max-3)//si(..xx)
                {
                    taille[0]=buf.tab[pos];
                    taille[1]=buf.tab[pos+1];
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    taille[2]=buf.tab[0];
                    taille[3]='\0';
                    i++;
                    pos=-2;
                }
                else
                {
                    if(pos==max-2)//(...x)
                    {
                        taille[0]=buf.tab[pos];
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[1]=buf.tab[0];
                        taille[2]=buf.tab[1];
                        taille[3]='\0';
                        pos=-1;
                    }
                    else//(xxx...)
                    {
                        pos=0;
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[0]=buf.tab[pos];
                        taille[1]=buf.tab[pos+1];
                        taille[2]=buf.tab[pos+2];
                        taille[3]='\0';
                    }

                }
            }
            //printf("**%d**\n",atoi(taille));
            j=pos+3;
            for(k=0;k<=atoi(taille)-1;k++)
            {
               // if(k==0){printf("potition dans le bloc: %d\n",pos);printf("num du bloc: %d\n",i);}
                if(j<=1022)
                {
                    tab[k]=buf.tab[j];
                    j++;
                    pos=j;
                }
                else
                {
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    i++;
                    tab[k]=buf.tab[0];
                    j=1;
                    pos=j;
                }
            }
           // system("pause");
            //printf("%s\n\n\n",tab);
            //printf("%d--\n",num);
           num++;
           int matrrr;
           char date[9],eff[2],nom[32],prenom[32],wilaya[22],force[48],grade[31],region[26],sang[5];
           lire_enreg(tab,&matrrr,date,eff,nom,prenom,wilaya,force,grade,region,sang);

           strcat(tabb,taille);
           strcat(tabb,tab);

           if(matrrr!=cle)
           {

               inserttion(new_fichier,tabb,nb,aray,&buff,arayy,arayyy);
               nb++;
           }
            //system("pause");
        }
        aff_entete(new_fichier,6,nb);

/*
        for(i=0;i<entete(new_fichier,6);i++)
    {
        printf("%04d)%04d*%04d*%04d\n",i,aray[i].bloc,aray[i].pos,aray[i].cle);
    }

for(i=0;i<entete(new_fichier,6);i++)
    {
        printf("%04d)%04d*%04d\n",i,arayy[i].annee,arayy[i].cle);
    }
*/

    fwrite(aray,sizeof(index)*nb,1,fic);

    fwrite(arayy,sizeof(index_date)*nb,1,ficc);

    fwrite(arayyy,sizeof(index_date)*nb,1,ficcc);

    fclose(fic); fclose(ficc); fclose(ficcc);

    fermer(fichier);
    remove(nom_physique);

    fermer(new_fichier);
    rename(nom_physiquee,nom_physique);




    }

}

void modif_region(char nom_physique[30],int cle,int regionn)
{
    int pos,i,j,suivant,k,stop;
    Buffer buf,buff;
    char taille[4];

    int trouv,ii,poss,bloc;
    char tab[200]={"\0"};
    char tabb[200]={"\0"};
    char Region_militaire[7][25]={"1RM-Blida","2RM-Oran","3RM-Bechar","4RM-Ouargla","5RM-Constantine","6RM-Tamanrasset"};

    L7OVC *fichier;
    char mode='a';
    fichier=ouvrir(nom_physique,mode);

    index *aaray=malloc(sizeof(index)*(entete(fichier,6)));
    FILE *fic=fopen("index_matr","ab+");
    fread(aaray,sizeof(index)*entete(fichier,6),1,fic);

    rech_index_initiale(cle,entete(fichier,6),aaray,&trouv,&ii);

    if(trouv==1)
    {

        if(entete(fichier,1)!=0)
        {
            fclose(fic);
            free(aaray);
        pos=aaray[ii].pos;
        j=aaray[ii].pos;
        i=aaray[ii].bloc;
        stop=0;
        liredir(fichier,i,&buf);nb_liredire++;
       // printf("%s\n",buf.tab);
      // system("pause");

            for(ii=0;ii<200;ii++)
	        {
	        	tab[ii]='\0';
	        }//initialisation

            if(pos<=max-4)//si ona (...xxx)xxx designe la taille
            {
                taille[0]=buf.tab[pos];

                taille[1]=buf.tab[pos+1];

                taille[2]=buf.tab[pos+2];
                taille[3]='\0';


            }
            else
            {
                if(pos==max-3)//si(..xx)
                {
                    taille[0]=buf.tab[pos];
                    taille[1]=buf.tab[pos+1];
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    taille[2]=buf.tab[0];
                    taille[3]='\0';
                    i++;
                    pos=-2;
                }
                else
                {
                    if(pos==max-2)//(...x)
                    {
                        taille[0]=buf.tab[pos];
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[1]=buf.tab[0];
                        taille[2]=buf.tab[1];
                        taille[3]='\0';
                        pos=-1;
                    }
                    else//(xxx...)
                    {
                        pos=0;
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[0]=buf.tab[pos];
                        taille[1]=buf.tab[pos+1];
                        taille[2]=buf.tab[pos+2];
                        taille[3]='\0';
                    }

                }
            }
            //printf("**%d**\n",atoi(taille));
            j=pos+3;
            for(k=0;k<=atoi(taille)-1;k++)
            {
                if(j<=1022)
                {
                    tab[k]=buf.tab[j];
                    j++;
                    pos=j;
                }
                else
                {
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    i++;
                    tab[k]=buf.tab[0];
                    j=1;
                    pos=j;
                }
            }
           // system("pause");
            //printf("%s\n\n\n",tab);

           int matrrr;
           char matrrrr[7]={"\0"};
           char date[9],eff[2],nom[32],prenom[32],wilaya[22],force[48],grade[31],region[26],sang[5];
           lire_enreg(tab,&matrrr,date,eff,nom,prenom,wilaya,force,grade,region,sang);

            for(ii=0;ii<200;ii++)
	        {
	        	tab[ii]='\0';
	        }//initialisation

	        strcat(tab,itoa(matrrr,matrrrr,10));
	        strcat(tab,eff);
	        strcat(tab,date);


	        int taille=strlen(nom);

                char tailleee[4];
                int iii=2;
                int save=0;
                char Num[10]={'0','1','2','3','4','5','6','7','8','9'};
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;

            	}
            	tailleee[3]='\0';
            	strcat(tab,tailleee);
            	strcat(tab,nom);



                taille=strlen(prenom);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(tab,tailleee);
            	strcat(tab,prenom);


            	taille=strlen(wilaya);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(tab,tailleee);
            	strcat(tab,wilaya);


            	taille=strlen(force);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(tab,tailleee);
            	strcat(tab,force);


            	taille=strlen(grade);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(tab,tailleee);
            	strcat(tab,grade);


            	taille=strlen(Region_militaire[regionn-1]);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(tab,tailleee);
            	strcat(tab,Region_militaire[regionn-1]);


            	taille=strlen(sang);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(tab,tailleee);
            	strcat(tab,sang);


            	taille=strlen(tab);
                iii=2;
                save=0;
                while(iii>=0)//ecrire la taille dans les 3 premiere potition
	             {
	              	save=taille % 10;
	            	tailleee[iii]=Num[save];
	            	taille=taille/10;
	            	iii--;
            	}
            	tailleee[3]='\0';
            	strcat(tabb,tailleee);

            	strcat(tabb,tab);

                fermer(fichier);
            	supr_enreg(nom_physique,cle);



                fichier=ouvrir(nom_physique,mode);

    for(iii=0;iii<(entete(fichier,6)+1);iii++)
    {
        aaray[iii].bloc=0;
        aaray[iii].cle=0;
        aaray[iii].pos=0;
    }
    index_date*arayy=malloc(sizeof(index_date)*(entete(fichier,6)+1));

    for(iii=0;iii<(entete(fichier,6)+1);iii++)
    {
        arayy[iii].annee=0;
        arayy[iii].cle=0;
    }
    index_date*arayyy=malloc(sizeof(index_date)*(entete(fichier,6)+1));
    for(iii=0;iii<(entete(fichier,6)+1);iii++)
    {
        arayyy[iii].annee=0;
        arayyy[iii].cle=0;
    }

    FILE *fic=fopen("index_matr","ab+");
    FILE *ficc=fopen("index_date","ab+");
    FILE *ficcc=fopen("index_region","ab+");
if(entete(fichier,6)!=0)
{
    fread(aaray,sizeof(index)*entete(fichier,6),1,fic);
    fread(arayy,sizeof(index_date)*entete(fichier,6),1,ficc);
    fread(arayyy,sizeof(index_date)*entete(fichier,6),1,ficcc);
}



            	inserttion(fichier,tabb,entete(fichier,6),aaray,&buff,arayy,arayyy);

  aff_entete(fichier,6,entete(fichier,6)+1);
     // printf("**%d**",arrayy[entete(fichier,6)-2].cle);

      remove("index_matr");
      remove("index_date");
      remove("index_region");
      fclose(fic);
      fclose(ficc);
      fclose(ficcc);


      fic=fopen("index_matr","rb+");
      ficc=fopen("index_date","rb+");
      ficcc=fopen("index_region","rb+");


    //  printf("**%d**\n",arrayy[entete(fichier,6)-1].cle);
       fwrite(aaray,sizeof(index)*(entete(fichier,6)),1,fic);
       fwrite(arayy,sizeof(index_date)*(entete(fichier,6)),1,ficc);
       fwrite(arayyy,sizeof(index_date)*(entete(fichier,6)),1,ficcc);
       fclose(fic);
       fclose(ficc);
       fclose(ficcc);
fermer(fichier);

    }



    }
}




void supr_enreg_relatif_force(char nom_physique[30],int num_force)
{

    char Force_armee[8][47]={"Armee_de_terre","Armee_de_l_air","Marine_nationale","Defense_aerienne_du_territoire","Gendarmerie_nationale","Garde_republicaine","Departement_du_renseignement_et_de_la_securite","Sante_militaire"};

    int pos,i,j,suivant,k,ii,stop,num;
    Buffer buf;
    Buffer buff;
    char taille[4];
    char tab[200];
    char tabb[200]={"\0"};
    num=1;
    int nb=0;
    char nom_physiquee[10]={"aaaaaaaa"};
char mode='a';
    L7OVC *fichier=ouvrir(nom_physique,mode);;
    L7OVC *new_fichier;

    index *aray=malloc(sizeof(index)*(entete(fichier,6)-1));
    int ooo;
    for(ooo=0;ooo<(entete(fichier,6)-1);ooo++)
    {
        aray[ooo].bloc=0;
        aray[ooo].cle=0;
        aray[ooo].pos=0;
    }
    index_date*arayy=malloc(sizeof(index_date)*(entete(fichier,6)-1));
    for(ooo=0;ooo<(entete(fichier,6)-1);ooo++)
    {
        arayy[ooo].annee=0;
        arayy[ooo].cle=0;
    }
    index_date*arayyy=malloc(sizeof(index_date)*(entete(fichier,6)-1));
    for(ooo=0;ooo<(entete(fichier,6)-1);ooo++)
    {
        arayyy[ooo].annee=0;
        arayyy[ooo].cle=0;
    }



    remove("index_matr");
    remove("index_date");
    remove("index_region");

    char modee='n';




    new_fichier=ouvrir(nom_physiquee,modee);
   // chargement_initiale(new_fichier,1);

    FILE *fic=fopen("index_matr","ab+");
    FILE *ficc=fopen("index_date","ab+");
    FILE *ficcc=fopen("index_region","ab+");
if(entete(new_fichier,6)!=0)
{
    fread(aray,sizeof(index)*entete(new_fichier,6),1,fic);
    fread(arayy,sizeof(index_date)*entete(new_fichier,6),1,ficc);
    fread(arayyy,sizeof(index_date)*entete(new_fichier,6),1,ficcc);
}

    if(entete(fichier,1)!=0)
    {
        pos=0;j=0;i=1;
        stop=0;
        liredir(fichier,i,&buf);
        nb_liredire++;
       // printf("%s\n",buf.tab);
      // system("pause");
        while(stop==0)
        {
            if(j==entete(fichier,4) && i==entete(fichier,3) )//si on arrive a la fin du fichier la derniere pos dans le dernier bloc
            {
                stop=1;
                break;
            }
            for(ii=0;ii<200;ii++)
	        {
	        	tab[ii]='\0';
	        }//initialisation
	        for(ii=0;ii<200;ii++)
	        {
	        	tabb[ii]='\0';
	        }//initialisation
            if(pos<=max-4)//si ona (...xxx)xxx designe la taille
            {
                taille[0]=buf.tab[pos];

                taille[1]=buf.tab[pos+1];

                taille[2]=buf.tab[pos+2];
                taille[3]='\0';


            }
            else
            {
                if(pos==max-3)//si(..xx)
                {
                    taille[0]=buf.tab[pos];
                    taille[1]=buf.tab[pos+1];
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    taille[2]=buf.tab[0];
                    taille[3]='\0';
                    i++;
                    pos=-2;
                }
                else
                {
                    if(pos==max-2)//(...x)
                    {
                        taille[0]=buf.tab[pos];
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[1]=buf.tab[0];
                        taille[2]=buf.tab[1];
                        taille[3]='\0';
                        pos=-1;
                    }
                    else//(xxx...)
                    {
                        pos=0;
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[0]=buf.tab[pos];
                        taille[1]=buf.tab[pos+1];
                        taille[2]=buf.tab[pos+2];
                        taille[3]='\0';
                    }

                }
            }
            //printf("**%d**\n",atoi(taille));
            j=pos+3;
            for(k=0;k<=atoi(taille)-1;k++)
            {
               // if(k==0){printf("potition dans le bloc: %d\n",pos);printf("num du bloc: %d\n",i);}
                if(j<=1022)
                {
                    tab[k]=buf.tab[j];
                    j++;
                    pos=j;
                }
                else
                {
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    i++;
                    tab[k]=buf.tab[0];
                    j=1;
                    pos=j;
                }
            }
           // system("pause");
            //printf("%s\n\n\n",tab);
            //printf("%d--\n",num);
           num++;
           int matrrr;
           char date[9],eff[2],nom[32],prenom[32],wilaya[22],force[48],grade[31],region[26],sang[5];

           int oooo;
           for(oooo=0;oooo<48;oooo++)
           {
               force[oooo]='\0';
           }
           lire_enreg(tab,&matrrr,date,eff,nom,prenom,wilaya,force,grade,region,sang);

           strcat(tabb,taille);
           strcat(tabb,tab);

           if(strcmp(force,Force_armee[num_force-1])!=0)
           {

               inserttion(new_fichier,tabb,nb,aray,&buff,arayy,arayyy);
               nb++;
           }
            //system("pause");
        }




        aff_entete(new_fichier,6,nb);

/*
        for(i=0;i<entete(new_fichier,6);i++)
    {
        printf("%04d)%04d*%04d*%04d\n",i,aray[i].bloc,aray[i].pos,aray[i].cle);
    }

for(i=0;i<entete(new_fichier,6);i++)
    {
        printf("%04d)%04d*%04d\n",i,arayy[i].annee,arayy[i].cle);
    }
*/

    fwrite(aray,sizeof(index)*nb,1,fic);

    fwrite(arayy,sizeof(index_date)*nb,1,ficc);

    fwrite(arayyy,sizeof(index_date)*nb,1,ficcc);

    fclose(fic); fclose(ficc); fclose(ficcc);

    fermer(fichier);
    remove(nom_physique);

    fermer(new_fichier);
    rename(nom_physiquee,nom_physique);




    }

}

void affich_grade_donner(L7OVC *fichier,int categorie_grade)
{

    int pos,i,j,suivant,k,ii,stop,num;//FILE*fic=fopen("indexx.txt","a+");
    Buffer buf;
    char taille[4];
    char tab[500];
num=1;

    if(entete(fichier,1)!=0)
    {
        pos=0;j=0;i=1;
        stop=0;
        liredir(fichier,i,&buf);
        nb_liredire++;
       // printf("%s\n",buf.tab);
      // system("pause");
        while(stop==0)
        {
            if(j==entete(fichier,4) && i==entete(fichier,3) )//si on arrive a la fin du fichier la derniere pos dans le dernier bloc
            {
                stop=1;
                break;
            }
            for(ii=0;ii<499;ii++)
	        {
	        	tab[ii]='\0';
	        }//initialisation
            if(pos<=max-4)//si ona (...xxx)xxx designe la taille
            {
                taille[0]=buf.tab[pos];

                taille[1]=buf.tab[pos+1];

                taille[2]=buf.tab[pos+2];
                taille[3]='\0';


            }
            else
            {
                if(pos==max-3)//si(..xx)
                {
                    taille[0]=buf.tab[pos];
                    taille[1]=buf.tab[pos+1];
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    taille[2]=buf.tab[0];
                    taille[3]='\0';
                    i++;
                    pos=-2;
                }
                else
                {
                    if(pos==max-2)//(...x)
                    {
                        taille[0]=buf.tab[pos];
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[1]=buf.tab[0];
                        taille[2]=buf.tab[1];
                        taille[3]='\0';
                        pos=-1;
                    }
                    else//(xxx...)
                    {
                        pos=0;
                        suivant=buf.suivant;
                        liredir(fichier,suivant,&buf);
                        nb_liredire++;
                        i++;
                        taille[0]=buf.tab[pos];
                        taille[1]=buf.tab[pos+1];
                        taille[2]=buf.tab[pos+2];
                        taille[3]='\0';
                    }

                }
            }
            //printf("**%d**\n",atoi(taille));
            j=pos+3;
            for(k=0;k<=atoi(taille)-1;k++)
            {
               // if(k==0){printf("potition dans le bloc: %d\n",pos);printf("num du bloc: %d\n",i);}
                if(j<=1022)
                {
                    tab[k]=buf.tab[j];
                    j++;
                    pos=j;
                }
                else
                {
                    suivant=buf.suivant;
                    liredir(fichier,suivant,&buf);
                    nb_liredire++;
                    i++;
                    tab[k]=buf.tab[0];
                    j=1;
                    pos=j;
                }
            }
           // system("pause");
            //printf("%s\n\n\n",tab);
            //printf("%d--\n",num);
           num++;
           int matrrr,oooo;
           char date[9],eff[2],nom[32],prenom[32],wilaya[22],force[48],grade[31],region[26],sang[5];
           for(oooo=0;oooo<31;oooo++)
           {
               grade[oooo]='\0';
           }
           lire_enreg(tab,&matrrr,date,eff,nom,prenom,wilaya,force,grade,region,sang);

           char Grade[18][30]={"General_de_corps_d_armee","General-Major","General","Colonel","Lieutenant-colonel","Commandant","Capitaine","Lieutenant","Sous-lieutenant","Aspirant","Adjudant-chef","Adjudant","Sergentchef","Sergent","Caporal-chef","Caporal","Djoundi"};

           if(categorie_grade==1 && (strcmp(grade,Grade[0])==0||strcmp(grade,Grade[1])==0||strcmp(grade,Grade[2])==0) )
           {
               printf("\t\t\t\t\t\t\t\t\tMatricule: %d\n\n",matrrr);
           printf("\t\t\t\t\t\t\t\t\tDate_Naissance: %s\n\n",date);
           printf("\t\t\t\t\t\t\t\t\tNom: %s\n\n",nom);
           printf("\t\t\t\t\t\t\t\t\tPrenom: %s\n\n",prenom);
           printf("\t\t\t\t\t\t\t\t\tWilaya: %s\n\n",wilaya);
           printf("\t\t\t\t\t\t\t\t\tForce_Armée: %s\n\n",force);
           printf("\t\t\t\t\t\t\t\t\tGrade: %s\n\n",grade);
           printf("\t\t\t\t\t\t\t\t\tRégion_militaire: %s\n\n",region);
           printf("\t\t\t\t\t\t\t\t\tGroupe_Sanguin: %s\n\n",sang);
           printf("_______________________________________________________________________________________________________________________________________________________________________\n");

           }

           if(categorie_grade==2 && (strcmp(grade,Grade[3])==0||strcmp(grade,Grade[4])==0||strcmp(grade,Grade[5])==0) )
           {
                printf("\t\t\t\t\t\t\t\t\tMatricule: %d\n\n",matrrr);
           printf("\t\t\t\t\t\t\t\t\tDate_Naissance: %s\n\n",date);
           printf("\t\t\t\t\t\t\t\t\tNom: %s\n\n",nom);
           printf("\t\t\t\t\t\t\t\t\tPrenom: %s\n\n",prenom);
           printf("\t\t\t\t\t\t\t\t\tWilaya: %s\n\n",wilaya);
           printf("\t\t\t\t\t\t\t\t\tForce_Armée: %s\n\n",force);
           printf("\t\t\t\t\t\t\t\t\tGrade: %s\n\n",grade);
           printf("\t\t\t\t\t\t\t\t\tRégion_militaire: %s\n\n",region);
           printf("\t\t\t\t\t\t\t\t\tGroupe_Sanguin: %s\n\n",sang);
           printf("_______________________________________________________________________________________________________________________________________________________________________\n");


           }

           if(categorie_grade==3 && (strcmp(grade,Grade[6])==0||strcmp(grade,Grade[7])==0||strcmp(grade,Grade[8])==0) )
           {
                printf("\t\t\t\t\t\t\t\t\tMatricule: %d\n\n",matrrr);
           printf("\t\t\t\t\t\t\t\t\tDate_Naissance: %s\n\n",date);
           printf("\t\t\t\t\t\t\t\t\tNom: %s\n\n",nom);
           printf("\t\t\t\t\t\t\t\t\tPrenom: %s\n\n",prenom);
           printf("\t\t\t\t\t\t\t\t\tWilaya: %s\n\n",wilaya);
           printf("\t\t\t\t\t\t\t\t\tForce_Armée: %s\n\n",force);
           printf("\t\t\t\t\t\t\t\t\tGrade: %s\n\n",grade);
           printf("\t\t\t\t\t\t\t\t\tRégion_militaire: %s\n\n",region);
           printf("\t\t\t\t\t\t\t\t\tGroupe_Sanguin: %s\n\n",sang);
           printf("_______________________________________________________________________________________________________________________________________________________________________\n");


           }
           if(categorie_grade==4 && (strcmp(grade,Grade[9])==0||strcmp(grade,Grade[10])==0||strcmp(grade,Grade[11])==0)||strcmp(grade,Grade[12])==0 )
           {
                printf("\t\t\t\t\t\t\t\t\tMatricule: %d\n\n",matrrr);
           printf("\t\t\t\t\t\t\t\t\tDate_Naissance: %s\n\n",date);
           printf("\t\t\t\t\t\t\t\t\tNom: %s\n\n",nom);
           printf("\t\t\t\t\t\t\t\t\tPrenom: %s\n\n",prenom);
           printf("\t\t\t\t\t\t\t\t\tWilaya: %s\n\n",wilaya);
           printf("\t\t\t\t\t\t\t\t\tForce_Armée: %s\n\n",force);
           printf("\t\t\t\t\t\t\t\t\tGrade: %s\n\n",grade);
           printf("\t\t\t\t\t\t\t\t\tRégion_militaire: %s\n\n",region);
           printf("\t\t\t\t\t\t\t\t\tGroupe_Sanguin: %s\n\n",sang);
           printf("_______________________________________________________________________________________________________________________________________________________________________\n");


           }
           if(categorie_grade==5 && (strcmp(grade,Grade[13])==0||strcmp(grade,Grade[14])==0||strcmp(grade,Grade[15])==0||strcmp(grade,Grade[16])==0) )
           { printf("\t\t\t\t\t\t\t\t\tMatricule: %d\n\n",matrrr);
           printf("\t\t\t\t\t\t\t\t\tDate_Naissance: %s\n\n",date);
           printf("\t\t\t\t\t\t\t\t\tNom: %s\n\n",nom);
           printf("\t\t\t\t\t\t\t\t\tPrenom: %s\n\n",prenom);
           printf("\t\t\t\t\t\t\t\t\tWilaya: %s\n\n",wilaya);
           printf("\t\t\t\t\t\t\t\t\tForce_Armée: %s\n\n",force);
           printf("\t\t\t\t\t\t\t\t\tGrade: %s\n\n",grade);
           printf("\t\t\t\t\t\t\t\t\tRégion_militaire: %s\n\n",region);
           printf("\t\t\t\t\t\t\t\t\tGroupe_Sanguin: %s\n\n",sang);
           printf("_______________________________________________________________________________________________________________________________________________________________________\n");


           }

        }
    }
}
