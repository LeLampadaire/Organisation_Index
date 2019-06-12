#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define DIM 20
#define NOMBREJEU 25
#define DATE_ANNEE_ACTUELLE 2019

typedef struct{
    short jour;
    short mois;
    short annee;
}DATE_ACHAT;

typedef struct{
    long IdJeu;
    char Nom[NOMBREJEU];
    char Interieur;
    char Exterieur;
    short AgeMin;
    short AgeMax;
    short NbJoueursMin;
    short NbJoueursMax;
    DATE_ACHAT DateAchat;
    short DureeGarantie;
}JEUX;

typedef struct{
    long IdJeu;
    char Nom[NOMBREJEU];
    long Position;
}INDEX;

void INIT(INDEX *, JEUX *, short *); // Initialisation de l'index
void TRI_INDEX(INDEX *, short *); // Tri de l'index
void AFFICHAGE(INDEX *, JEUX *, short *); // Affichage
void RECHERCHE(INDEX *, JEUX *, short *); // Recherche d'un JEUX à partir de l'index puis l'afficher
void AJOUT(INDEX *, JEUX *, short *); // Ajout d'un jeu à la première place libre
DATE_ACHAT VERIFICATION_DATE(char *); // Vérification de la date
void SUPPRESSION(INDEX *, JEUX *, short *); // Supprimer un jeu
short MODIFICATION(INDEX *, JEUX *, short *); // Modification de une ou plusieurs données d'un jeu

// Vérification
short VERIF_IDJEU(INDEX *, JEUX *, short *);
void VERIF_NOM(INDEX *, JEUX *, short *);
void VERIF_INTERIEUR(JEUX *);
void VERIF_EXTERIEUR(JEUX *);
void VERIF_AGEMIN(JEUX *);
void VERIF_AGEMAX(JEUX *);
void VERIF_NBJOUEURMIN(JEUX *);
void VERIF_NBJOUEURMAX(JEUX *);
void VERIF_GARANTIE(JEUX *);

int main(void){
    JEUX vec[DIM] = {
        {42,"Monopoly",'T','F',8,99,2,8,{14,9,2012},3},
        {17,"Hereos",'T','T',6,65,2,12,{5,5,2017},1},
        {67,"Volley",'F','T',6,40,2,6,{11,3,2019},2},
        {81,"Poker",'F','T',6,99,4,12,{1,6,1995},4},
        {12,"UNOO",'F','T',6,99,2,4,{25,10,1998},7} };
    INDEX vec_index[DIM];
    short choix=0, dimension=0, erreur=0;

    // Dimension = 5 -> On a initialisé les 5 premiers jeux
    dimension = 5;

    INIT(&vec_index[0], &vec[0], &dimension);

    do{
        do{
            printf("Menu :\n");
            printf("\t1 - Recherche\n");
            printf("\t2 - Ajout\n");
            printf("\t3 - Suppression\n");
            printf("\t4 - Modification\n");
            printf("\t5 - Affichage\n");
            printf("\t6 - Quitter\n");
            printf("Votre choix : ");
            fflush(stdin);
            scanf("%hd", &choix);
            system("cls");
        }while(choix < 1 && choix > 5);

        switch(choix){
            case 1:
                RECHERCHE(&vec_index[0], &vec[0], &dimension);

                break;
            case 2:
                AJOUT(&vec_index[0], &vec[0], &dimension);
                TRI_INDEX(&vec_index[0], &dimension);

                break;
            case 3:
                SUPPRESSION(&vec_index[0], &vec[0], &dimension);
                TRI_INDEX(&vec_index[0], &dimension);

                break;
            case 4:
                do{
                    erreur = MODIFICATION(&vec_index[0], &vec[0], &dimension);
                }while(erreur == 1);
                TRI_INDEX(&vec_index[0], &dimension);

                break;
            case 5:
                printf("AFFICHAGE\n");
                AFFICHAGE(&vec_index[0], &vec[0], &dimension);

                break;
            default:
                if(choix != 6){
                    printf("[Erreur] Choix inexistant\n");
                }
        }

        if(choix != 6){
            printf("\n- Voulez-vous reprendre au menu (1: Oui / 0: Non) : ");
            fflush(stdin);
            scanf("%hd", &choix);
            system("cls");
        }

    }while(choix == 1);

    printf("Fin du programme.\n");

    return 0;
}

/*************************************************************************************************************************/
/* INPUT : Récupère la structure JEUX dans le pointeur j et la structure INDEX dans le pointeur i                        */
/* PROCESS : Copie les données IdJeu, Nom de la structure JEUX vers la structure INDEX + augmente le compteur = position */
/* OUTPUT : /                                                                                                            */
/*************************************************************************************************************************/
void INIT(INDEX *i, JEUX *j, short *dimension){
    long cpt=0;

    for(cpt=0;cpt<*dimension;cpt++,j++,i++){
        i->IdJeu = j->IdJeu;
        strcpy(i->Nom, j->Nom);
        i->Position = cpt;
    }
}

/***********************************************************************/
/* INPUT : Récupère la structure INDEX dans le pointeur i              */
/* PROCESS : Fait un tri bulle à partir des noms de la structure INDEX */
/* OUTPUT : /                                                          */
/***********************************************************************/
void TRI_INDEX(INDEX *i, short *dimension){
    short cpt=0, NBR_JEUX_Tempo=0;
    INDEX tempo, *iTempo;

    iTempo = i;
    NBR_JEUX_Tempo = *dimension;

    while(NBR_JEUX_Tempo>1){
        cpt=0;
        i = iTempo;
        while(cpt<NBR_JEUX_Tempo-1){
            if( stricmp(i->Nom, (i+1)->Nom) > 0 ){
                tempo = *i;
                *i = *(i+1);
                *(i+1) = tempo;
            }
            cpt++;
            i++;
        }
        NBR_JEUX_Tempo--;
    }
}

/***********************************************************/
/* INPUT : Récupère la structure INDEX dans le pointeur i  */
/* PROCESS : Affichage de la structure INDEX               */
/* OUTPUT : /                                              */
/***********************************************************/
void AFFICHAGE(INDEX *i, JEUX *j, short *dimension){
    short cpt=0;
    JEUX *jTempo;
    INDEX *iTempo;

    jTempo = j;
    iTempo = i;

    i = iTempo;
    for(cpt=0;cpt<*dimension;cpt++,i++){
        if(i->IdJeu != -1){
            j = jTempo + i->Position;
            printf("\nIdJeu : [%ld]\n",j->IdJeu);
            printf("Nom du jeu : %s\n",j->Nom);
            printf("Interieur(T=Oui/F=Non) : %c\n",j->Interieur);
            printf("Exterieur(T=Oui/F=Non) : %c\n",j->Exterieur);
            printf("Age minimum : %hd\n",j->AgeMin);
            printf("Age maximum : %hd\n",j->AgeMax);
            printf("Nombre de joueurs minimum : %hd\n",j->NbJoueursMin);
            printf("Nombre de joueurs maximum : %hd\n",j->NbJoueursMax);
            printf("Date d'achat : %hd/%hd/%hd\n",j->DateAchat.jour, j->DateAchat.mois, j->DateAchat.annee);
            printf("Garantie : %hd ans.\n",j->DureeGarantie);
            printf("\n");
        }
    }
}

/***************************************************************************************************************************/
/* INPUT : Récupère la structure INDEX dans le pointeur i et  la structure JEUX dans le pointeur j                         */
/* PROCESS : Recherche d'un jeux à partir de l'index et afficher toute ses caractéristiques à partir de la structure JEUX  */
/* OUTPUT : /                                                                                                              */
/***************************************************************************************************************************/
void RECHERCHE(INDEX *i, JEUX *j, short *dimension){
    short cpt=0, choix=0, posTempo=0;
    long rechercheID=0;
    char rechercheNom[NOMBREJEU];
    INDEX *iTempo;

    iTempo = i;

    do{
        printf("Choisir entre la recherche avec l'ID ou avec le nom du jeu :\n");
        printf("\t 1 - ID\n");
        printf("\t 2 - Nom\n");
        printf("Votre choix : ");
        fflush(stdin);
        scanf("%hd", &choix);
        system("cls");
    }while(choix != 1 && choix != 2);

    for(cpt=0;cpt<*dimension;cpt++,i++){
        if(i->IdJeu != -1){
            printf("[%ld] : %s\n",i->IdJeu, i->Nom);
        }
    }

    i = iTempo;

    if(choix == 1){
        printf("L'ID : ");
        fflush(stdin);
        scanf("%ld", &rechercheID);

        for(cpt=0;cpt<*dimension;cpt++,i++){
            if(rechercheID == i->IdJeu){
                posTempo = i->Position;
                break;
            }
        }
    }else{
        printf("Le nom : ");
        fflush(stdin);
        gets(rechercheNom);

        for(cpt=0;cpt<*dimension;cpt++,i++){
            if(stricmp(i->Nom, rechercheNom) == 0 && i->IdJeu != -1){
                posTempo = i->Position;
                break;
            }
        }
    }

    if(cpt == *dimension){
            printf("- Aucun jeu n'a ete trouve.\n");
        }else{
            j = j + posTempo;
            printf("\n- Le jeux :\n");
            printf("[%ld] %s\n",j->IdJeu, j->Nom);
            printf("Interieur : %c\n",j->Interieur);
            printf("Exterieur : %c\n",j->Exterieur);
            printf("Age minimum : %hd\n",j->AgeMin);
            printf("Age maximum : %hd\n",j->AgeMax);
            printf("Nombre de joueur minimum : %hd\n",j->NbJoueursMin);
            printf("Nombre de joueur maximum : %hd\n",j->NbJoueursMax);
            printf("Date d'achat : %hd/%hd/%hd\n",j->DateAchat.jour ,j->DateAchat.mois ,j->DateAchat.annee);
            printf("Duree de garantie : %hd\n",j->DureeGarantie);
        }
}

/***************************************************************************************************/
/* INPUT : Récupère la structure INDEX dans le pointeur i et  la structure JEUX dans le pointeur j */
/* PROCESS : Ajout un jeu à la première place disponible                                           */
/* OUTPUT : /                                                                                      */
/***************************************************************************************************/
void AJOUT(INDEX *i, JEUX *j, short *dimension){
    short cpt=0, test=0;
    INDEX *iTempo;
    char date[11];

    iTempo = i;

    // On vérifie d'abord qu'une ligne ne soit pas supprimé
    for(cpt=0;cpt<*dimension;cpt++,i++){
        if(i->IdJeu == -1){
            break;
        }
    }

    if(cpt != *dimension){
        j = j + i->Position;
    }else{
        j = j + cpt;
        if(*dimension != DIM){
            (*dimension)++;
        }else{
            test = 1;
        }
    }

    if(test != 1){
        test = 0;
        do{
            if(test == 1){
                printf("[Erreur] L'ID du jeu existe deja.\n");
            }
            i = iTempo;
            test = VERIF_IDJEU(i, j, dimension);
        }while(test == 1);

        VERIF_NOM(i, j, dimension);

        do{
            VERIF_INTERIEUR(j);
            VERIF_EXTERIEUR(j);
        }while( (j->Interieur == 'F' && j->Exterieur == 'F') || (j->Interieur == 'f' && j->Exterieur == 'f') || (j->Interieur == 'F' && j->Exterieur == 'f') || (j->Interieur == 'f' && j->Exterieur == 'F') );

        do{
            VERIF_AGEMIN(j);
            VERIF_AGEMAX(j);
        }while(j->AgeMin > j->AgeMax);

        do{
            VERIF_NBJOUEURMIN(j);
            VERIF_NBJOUEURMAX(j);
        }while(j->NbJoueursMin > j->NbJoueursMax);

        do{
            printf("Date d'achat (jj/mm/yyyy) : ");
            fflush(stdin);
            gets(date);
            j->DateAchat = VERIFICATION_DATE(&date[0]);
        }while( stricmp(date,"-1") == 0 );

        VERIF_GARANTIE(j);

        i->IdJeu = j->IdJeu;
        strcpy(i->Nom, j->Nom);
        i->Position = cpt;

    }else{
        printf("[Erreur] Il y a trop de jeu.\n");
    }
}

/******************************************************************************************/
/* INPUT : Récupère la structure JEUX et la date                                          */
/* PROCESS : Vérifie si la date est correct puis envoi les données dans la structure DATE */
/* OUTPUT : Renvoi la réponse de la vérification                                          */
/******************************************************************************************/
DATE_ACHAT VERIFICATION_DATE(char *DateDonnee){
    char date[11];
	char *pTempo, TempoDate[11];
	short test=0;

	strcpy(date, DateDonnee);
	pTempo = &date[0];

	DATE_ACHAT DateTempo;

	if ( (date[1] == '-' || date[1] == '/') && (date[3] == '-' || date[3] == '/') ){
		date[1] = '\0';
		strcpy(TempoDate, pTempo);
		DateTempo.jour = atoi(TempoDate);

		date[3] = '\0';
		pTempo = &date[2];
		strcpy(TempoDate, pTempo);
		DateTempo.mois = atoi(TempoDate);

		pTempo = &date[4];
	}else if( (date[1] == '-' || date[1] == '/') && (date[4] == '-' || date[4] == '/') ){
		date[1] = '\0';
		strcpy(TempoDate, pTempo);
		DateTempo.jour = atoi(TempoDate);

		date[4] = '\0';
		pTempo = &date[2];
		strcpy(TempoDate, pTempo);
		DateTempo.mois = atoi(TempoDate);

		pTempo = &date[5];
	}else if( (date[2] == '-' || date[2] == '/') && (date[4] == '-' || date[4] == '/') ){
		date[2] = '\0';
		strcpy(TempoDate, pTempo);
		DateTempo.jour = atoi(TempoDate);

		date[4] = '\0';
		pTempo = &date[3];
		strcpy(TempoDate, pTempo);
		DateTempo.mois = atoi(TempoDate);

		pTempo = &date[5];
	}else if( (date[2] == '-' || date[2] == '/') && (date[5] == '-' || date[5] == '/') ){
		date[2] = '\0';
		strcpy(TempoDate, pTempo);
		DateTempo.jour = atoi(TempoDate);

		date[5] = '\0';
		pTempo = &date[3];
		strcpy(TempoDate, pTempo);
		DateTempo.mois = atoi(TempoDate);

		pTempo = &date[6];
	}else{
        test = 1;
	}


	if (test == 1) {
		printf("/!\\ Date invalide.\n");
	}else {
	    // On récupère l'année ici pour éviter de faire une condition en plus ou de devoir le répéter 3 fois
        strcpy(TempoDate, pTempo);
        DateTempo.annee = atoi(TempoDate);

		if (DateTempo.annee <= DATE_ANNEE_ACTUELLE && DateTempo.annee >= 1900) {
			if (DateTempo.mois == 2) {
				if ( (DateTempo.jour <= 28 && DateTempo.jour >= 1) || (DateTempo.jour == 29 && (DateTempo.annee % 400 == 0 || (DateTempo.annee % 4 == 0 && DateTempo.annee % 100 != 0) ) ) ) {
					test = 0;
				} else {
				    printf("/!\\ Le jour du mois de fevrier n'est pas correct.\n");
					test = 1;
				}
			} else if (DateTempo.mois == 4 || DateTempo.mois == 6 || DateTempo.mois == 9 || DateTempo.mois == 11 || DateTempo.mois == 12) {
				if (DateTempo.jour <= 30 && DateTempo.jour >= 1) {
                    test = 0;
				} else {
				    printf("/!\\ Le jour n'est pas correct.\n");
					test = 1;
				}
			} else if (DateTempo.mois == 1 || DateTempo.mois == 3 || DateTempo.mois == 5 || DateTempo.mois == 7 || DateTempo.mois == 8 || DateTempo.mois == 10 || DateTempo.mois == 12) {
				if (DateTempo.jour <= 31 && DateTempo.jour >= 1) {
                    test = 0;
				} else {
				    printf("/!\\ Le jour n'est pas correct.\n");
					test = 1;
				}
			}else{
                printf("/!\\ Le mois n'est pas correct.\n");
                test = 1;
			}
		}else{
			printf("/!\\ L'annee n'est pas correcte.\n");
			test = 1;
		}
	}

	if(test == 1){
        strcpy(DateDonnee, "-1");
	}

	return DateTempo;
}

/***************************************************************************************************/
/* INPUT : Récupère la structure INDEX dans le pointeur i et  la structure JEUX dans le pointeur j */
/* PROCESS : Supprime un jeu de façon logique                                                      */
/* OUTPUT : /                                                                                      */
/***************************************************************************************************/
void SUPPRESSION(INDEX *i, JEUX *j, short *dimension){
    short choix=0;
    long rechercheID=0, cpt=0;
    char rechercheNom[NOMBREJEU];
    INDEX *iTempo;

    iTempo = i;

    do{
        printf("Choisir entre la suppression avec l'ID ou avec le nom du jeu :\n");
        printf("\t 1 - ID\n");
        printf("\t 2 - Nom\n");
        printf("Votre choix : ");
        fflush(stdin);
        scanf("%hd", &choix);
        system("cls");
    }while(choix != 1 && choix != 2);

    printf("SUPPRESSION\n");
    for(cpt=0;cpt<*dimension;cpt++,i++){
        if(i->IdJeu != -1){
            printf("[%ld] : %s\n",i->IdJeu, i->Nom);
        }
    }

    i = iTempo;

    if(choix == 1){
        printf("L'ID : ");
        fflush(stdin);
        scanf("%ld", &rechercheID);

        for(cpt=0;cpt<*dimension;cpt++,i++){
            if(i->IdJeu == rechercheID){
                j = j + i->Position;

                j->IdJeu = -1;

                i->IdJeu = -1;
                printf("\nLe jeu a bien ete supprime.");
                break;
            }
        }
    }else{
        printf("Le nom : ");
        fflush(stdin);
        gets(rechercheNom);

        for(cpt=0;cpt<*dimension;cpt++,i++){
            if( stricmp(i->Nom, rechercheNom) == 0 ){
                j = j + i->Position;

                j->IdJeu = -1;

                i->IdJeu = -1;
                printf("\nLe jeu a bien ete supprime.");
                break;
            }
        }
    }

    if(cpt == *dimension){
        printf("\n- [Erreur] L'ID n'existe pas.\n");
    }

}

/***************************************************************************************************/
/* INPUT : Récupère la structure INDEX dans le pointeur i et  la structure JEUX dans le pointeur j */
/* PROCESS : Modifie une ou plusieurs données d'un jeu                                             */
/* OUTPUT : /                                                                                      */
/***************************************************************************************************/
short MODIFICATION(INDEX *i, JEUX *j, short *dimension){
    short choix=0, ModifJeu=0, test=0, cpt=0, erreur=0;
    INDEX *iTempo;
    char ModifJeuChar[NOMBREJEU], date[11];

    iTempo = i;

    do{
        printf("Choisir entre la suppression avec l'ID ou avec le nom du jeu :\n");
        printf("\t 1 - ID\n");
        printf("\t 2 - Nom\n");
        printf("Votre choix : ");
        fflush(stdin);
        scanf("%hd", &choix);
        system("cls");
    }while(choix != 1 && choix != 2);

    printf("MODIFICATION\n");
    for(cpt=0;cpt<*dimension;cpt++,i++){
        if(i->IdJeu != -1){
            printf("[%ld] : %s\n",i->IdJeu, i->Nom);
        }
    }

    i = iTempo;

    if(choix == 1){
        printf("L'ID : ");
        fflush(stdin);
        scanf("%hd", &ModifJeu);

        for(cpt=0;cpt<*dimension;cpt++,i++){
            if(i->IdJeu == ModifJeu){
                j = j + i->Position;
                break;
            }
        }

    }else{
        printf("Le nom : ");
        fflush(stdin);
        gets(ModifJeuChar);

        for(cpt=0;cpt<*dimension;cpt++,i++){
            if( stricmp(i->Nom, ModifJeuChar) == 0 && i->IdJeu != -1){
                j = j + i->Position;
                break;
            }
        }
    }

    if(cpt != *dimension){
        do{
            system("cls");
            printf("Quelle donnee voulez-vous modifier ?\n");
            printf("\t 1 - IdJeu\n");
            printf("\t 2 - Nom\n");
            printf("\t 3 - Interieur\n");
            printf("\t 4 - Exterieur\n");
            printf("\t 5 - Age minimum\n");
            printf("\t 6 - Age maximum\n");
            printf("\t 7 - Nombres joueurs minimum\n");
            printf("\t 8 - Nombres joueurs maximum\n");
            printf("\t 9 - Date d'achat\n");
            printf("\t 10 - Duree de garantie\n");
            printf("Votre choix : ");
            fflush(stdin);
            scanf("%hd",&choix);
            system("cls");

            i = iTempo;

            switch(choix){
                case 1:
                    test = 0;
                    do{
                        if(test == 1){
                            printf("[Erreur] L'ID du jeu existe deja.\n");
                        }
                        test = VERIF_IDJEU(i, j, dimension);
                    }while(test == 1);

                    break;
                case 2:
                    VERIF_NOM(i, j, dimension);

                    break;
                case 3:
                    do{
                        VERIF_INTERIEUR(j);
                    }while( (j->Interieur == 'F' && j->Exterieur == 'F') || (j->Interieur == 'f' && j->Exterieur == 'f') || (j->Interieur == 'F' && j->Exterieur == 'f') || (j->Interieur == 'f' && j->Exterieur == 'F') );

                    break;
                case 4:
                    do{
                        VERIF_EXTERIEUR(j);
                    }while( (j->Interieur == 'F' && j->Exterieur == 'F') || (j->Interieur == 'f' && j->Exterieur == 'f') || (j->Interieur == 'F' && j->Exterieur == 'f') || (j->Interieur == 'f' && j->Exterieur == 'F') );

                    break;
                case 5:
                    do{
                        VERIF_AGEMIN(j);
                    }while(j->AgeMin > j->AgeMax);

                    break;
                case 6:
                    do{
                        VERIF_AGEMAX(j);
                    }while(j->AgeMin > j->AgeMax);

                    break;
                case 7:
                    do{
                        VERIF_NBJOUEURMIN(j);
                    }while(j->NbJoueursMin > j->NbJoueursMax);

                    break;
                case 8:
                    do{
                        VERIF_NBJOUEURMAX(j);
                    }while(j->NbJoueursMin > j->NbJoueursMax);

                    break;
                case 9:
                    do{
                        printf("Date d'achat (jj/mm/yyyy) : ");
                        fflush(stdin);
                        gets(date);
                        j->DateAchat = VERIFICATION_DATE(&date[0]);
                    }while( stricmp(date,"-1") == 0 );

                    break;
                case 10:
                    VERIF_GARANTIE(j);

                    break;
                default:
                    printf("[Erreur] La donnee choisie n'existe pas.\n");
                    choix = -1;
                    break;
            }

            if(choix != -1){
                printf("\nVoulez-vous refaire une modification sur le meme jeu (1: Oui / 2: Non) :");
                fflush(stdin);
                scanf("%hd",&choix);
                system("cls");
            }

        }while(choix == 1);

    }else{
        printf("\n- [Erreur] Le jeu n'esxiste pas.\n");
        erreur = 1;
    }

    return erreur;

}

/**************************************************************************************************************************************/
/* INPUT : Récupère la structure INDEX dans le pointeur i, la structure JEUX dans le pointeur j et l'adresse de la variable dimension */
/* PROCESS : Vérifie l'ID du jeu, renvoi 1 si l'ID a été trouvé                                                                       */
/* OUTPUT : /                                                                                                                         */
/**************************************************************************************************************************************/
short VERIF_IDJEU(INDEX *i, JEUX *j, short *dimension){
    short cpt=0, test=0;
    long IdJeuTempo=0;

    printf("ID : ");
    fflush(stdin);
    scanf("%ld",&IdJeuTempo);

    for(cpt=0;cpt<*dimension;cpt++,i++){
        if(i->IdJeu == IdJeuTempo){
            break;
        }
    }

    if(cpt == *dimension){
        j->IdJeu = IdJeuTempo;
    }else{
        test = 1;
    }

    return test;
}

/*********************************************************************/
/* INPUT : Récupère la structure JEUX dans le pointeur j             */
/* PROCESS : Vérifie si le nom ne dépasse pas la longueur du vecteur */
/* OUTPUT : /                                                        */
/*********************************************************************/
void VERIF_NOM(INDEX *i, JEUX *j, short *dimension){
    short lenNom=0, test=0, cpt=0;
    char NomTempo[NOMBREJEU];
    INDEX *iTempo;

    iTempo = i;

    do{
        test = 0;
        do{
            printf("Nom : ");
            fflush(stdin);
            gets(NomTempo);
            lenNom = strlen(NomTempo);
        }while(lenNom > NOMBREJEU);

        i = iTempo;

        for(cpt=0;cpt<*dimension;cpt++,i++){
            if( stricmp(i->Nom, NomTempo) == 0 ){
                break;
            }
        }

        if(cpt == *dimension){
            strcpy(i->Nom, NomTempo);
            strcpy(j->Nom, i->Nom);
        }else{
            printf("[Erreur] Le nom du jeu existe deja.\n");
            test = 1;
        }
    }while(test == 1);
}

/*************************************************************************/
/* INPUT : Récupère la structure JEUX dans le pointeur j                 */
/* PROCESS : Vérifie que la lettre entrée soit égale soit à T, t, f ou F */
/* OUTPUT : /                                                            */
/*************************************************************************/
void VERIF_INTERIEUR(JEUX *j){
    do{
        printf("Ce joue-t-il a l'interieur (T=Oui/F=Non) : ");
        fflush(stdin);
        j->Interieur = getchar();
    }while(j->Interieur != 'T' && j->Interieur != 'F' && j->Interieur != 't' && j->Interieur != 'f');
}

/*************************************************************************/
/* INPUT : Récupère la structure JEUX dans le pointeur j                 */
/* PROCESS : Vérifie que la lettre entrée soit égale soit à T, t, f ou F */
/* OUTPUT : /                                                            */
/*************************************************************************/
void VERIF_EXTERIEUR(JEUX *j){
    do{
        printf("Ce joue-t-il a l'exterieur (T=Oui/F=Non) : ");
        fflush(stdin);
        j->Exterieur = getchar();
    }while(j->Exterieur != 'T' && j->Exterieur != 'F' && j->Exterieur != 't' && j->Exterieur != 'f');
}

/*********************************************************/
/* INPUT : Récupère la structure JEUX dans le pointeur j */
/* PROCESS : Vérifie que l'âge soit positif ou nul       */
/* OUTPUT : /                                            */
/*********************************************************/
void VERIF_AGEMIN(JEUX *j){
    do{
        printf("Age minimum : ");
        fflush(stdin);
        scanf("%hd",&(j->AgeMin));
    }while(j->AgeMin < 0);
}

/**************************************************************/
/* INPUT : Récupère la structure JEUX dans le pointeur j      */
/* PROCESS : Vérifie que l'âge ne soit pas supérieur à 99 ans */
/* OUTPUT : /                                                 */
/**************************************************************/
void VERIF_AGEMAX(JEUX *j){
    do{
        printf("Age maximum : ");
        fflush(stdin);
        scanf("%hd",&(j->AgeMax));
    }while(j->AgeMax > 99);
}

/****************************************************************/
/* INPUT : Récupère la structure JEUX dans le pointeur j        */
/* PROCESS : Vérifie que le nombre de joueur soit supérieur à 0 */
/* OUTPUT : /                                                   */
/****************************************************************/
void VERIF_NBJOUEURMIN(JEUX *j){
    do{
        printf("Nombre de joueurs minimum : ");
        fflush(stdin);
        scanf("%hd",&(j->NbJoueursMin));
    }while(j->NbJoueursMin <= 0);
}

/****************************************************************************/
/* INPUT : Récupère la structure JEUX dans le pointeur j                    */
/* PROCESS : Vérifie que le nombre de joueur ne soit pas supérieur à 99 ans */
/* OUTPUT : /                                                               */
/****************************************************************************/
void VERIF_NBJOUEURMAX(JEUX *j){
    do{
        printf("Nombre de joueurs maximum : ");
        fflush(stdin);
        scanf("%hd",&(j->NbJoueursMax));
    }while(j->NbJoueursMax > 100);
}

/*********************************************************/
/* INPUT : Récupère la structure JEUX dans le pointeur j */
/* PROCESS : Vérifie que la garantie soit supérieur à 0  */
/* OUTPUT : /                                            */
/*********************************************************/
void VERIF_GARANTIE(JEUX *j){
    do{
        printf("Duree de garantie : ");
        fflush(stdin);
        scanf("%hd",&(j->DureeGarantie));
    }while(j->DureeGarantie < 0);
}
