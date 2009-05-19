/******************************************************************/
/**                                                              **/
/**   GRAPH.H                                                    **/
/**                                                              **/
/******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

#include "couleurs.h"


#ifndef PROF
	#define ROW		0
	#define COL		1

	#define RIEN 	256	// Matrice : avec autre chose que zero
	#define ZERO 	2	// 	Zero ni marke, ni barre
	#define BARRE 	4	//	Zero BArré
	#define MARKE	8	//	zero Marqué

	#define L_RIEN  16	// Matrices ligne : Pas marquée
	#define L_MARKE 32	//	ligne marqué

	#define C_RIEN  32	// Matrices colone : Pas marquée
	#define C_MARKE 16	//	colonne marquée

	#define FICH 4      // Flag pour fichier
	#define VERB 8      // Flag pour verbose
	#define PAS_NOM 16	// Flag pour ne pas lire les noms des lignes et colonnes
	#define INVERT 32	// Invert the matrix : we don't Minimize cost BUT maximize gain

	int OPTIONS ;           // Flag pour les options
	int NBR_SOMMET ;        // Nombre de sommet dans le Graph
	float ** Matrice ;      // Matrice representant le Graph avec conso max
	float ** Matrice_svg ;  // Matrice representant le Graph avec conso max
	int ** Matrice_zero ; 	// Matrice des consomations en cour
	int * Matrice_ligne ;   // Matrice des consomations en cour
	int * Matrice_colonne;  // Matrice des consomations en cour
	char ** Nom_ligne ;     // Matrice avec les noms de chaques Sommets
	char ** Nom_colonne ;   // Matrice avec les noms de chaques Sommets
#else
	extern int OPTIONS ;           // Flag pour les options
	extern int NBR_SOMMET ;        // Nombre de sommet dans le Graph
	extern float ** Matrice ;      // Matrice representant le Graph avec conso max
	extern float ** Matrice_svg ;  // Matrice representant le Graph avec conso max
	extern float ** Matrice_zero ; // Matrice des consomations en cour
	extern char ** Nom_ligne ;     // Matrice avec les noms de chaques Sommets
	extern char ** Nom_colonne ;   // Matrice avec les noms de chaques Sommets
#endif

// ---------------------------------------------
int ChargeFichier ( char * fichier );

// Fonctions d'affichage -----------------------
void ExitError ( char * Mess );
void ShowHelp( char *Name );
void ShowExample(char * Name );
void AfficheTotal( void ) ;
void AfficheConso( void ) ;	
void AfficheParcour( void );
void AfficheMatrice( void );
void AfficheZero( void );
float AfficheRelations(void );

// ----------------------------------------------
void LitOptions  ( int argc, char ** argv ) ;

// fonction.c -----------------------------------------------
int VerifyNbr ( char * Nbr );
void Init_Matrice_colonne ( void ) ;
void Init_Matrice_ligne ( int Marquage ) ;
void Copie_zero ( void );
void Bare_colonne( int colonne );
void Bare_ligne( int ligne );
void Invert ( void );
void Decharge ( void );
//void Marque_Ligne( int l );
//void Marque_Colonne( int c );
void Marque_Matrice( void );
