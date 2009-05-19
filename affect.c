/******************************************************************/
/**                                                              **/
/**   --- AFFECT.C ---                                           **/
/**                                                              **/
/******************************************************************/

#include "affect.h"

// Load file and matrix in memory ---------------------------------------
int ChargeFichier ( char * fichier )
{
	FILE *ptr;		// Pointer on the File
	size_t s;		// Size of name
	int i, j ;		
	char *tmp = (char *) calloc ( 1, 50 ) ;	// Tempory name of sommet 
	char * tmpNom ;	// Pointeur vers la vraie future zone memoire du nom

	if ( ( ptr = fopen ( fichier, "r+" ) ) == NULL )
	{	
		fprintf ( stderr, "Error: Can't open file\n" ) ;
		return 2 ;	
	}

	// Lit le nombre de Sommets
	fscanf( ptr, "%d", &NBR_SOMMET );
	if ( NBR_SOMMET <= 0 )
	    return 3;

	// Alloue la memoire pour la matrice
	Matrice = (float **) malloc ( sizeof ( float * ) * NBR_SOMMET );
	for( i = 0 ; i < NBR_SOMMET ; i++ )
		Matrice [ i ] = malloc ( sizeof ( float ) * NBR_SOMMET);

	//if ( OPTIONS & INVERT )
	//{
		// allocate memory for temporary matrix 
		Matrice_svg = (float **) malloc ( sizeof ( float * ) * NBR_SOMMET );
		for( i = 0 ; i < NBR_SOMMET ; i++ )
			Matrice_svg [ i ] = malloc ( sizeof ( float ) * NBR_SOMMET);
	//}

	// Alloue la memoire pour checker les zeros
	Matrice_zero = ( int **) malloc ( sizeof ( int * ) * NBR_SOMMET );
	for( i = 0 ; i < NBR_SOMMET ; i++ )
		Matrice_zero [ i ] = malloc ( sizeof ( int ) * NBR_SOMMET);
	Matrice_ligne = ( int * ) malloc ( sizeof( int ) * NBR_SOMMET ) ;
	Matrice_colonne = ( int * ) malloc ( sizeof( int ) * NBR_SOMMET ) ;

	// Alloue memoire pour les noms des sommets
	Nom_colonne = (char **) malloc ( sizeof ( char * ) * NBR_SOMMET );
	Nom_ligne = (char **) malloc ( sizeof ( char * ) * NBR_SOMMET );

	if ( ! (OPTIONS & PAS_NOM ) )
	{
	// Read ROW names : Lit le nom des lignes -----------------------------
		for ( i = 0 ; i < NBR_SOMMET ; i++ )
		{
			fscanf ( ptr, "%49s",  tmp ) ;	
			s = strlen ( tmp ) ;
	      	tmpNom = (char *) calloc ( 1, s + 3 ) ;
			memcpy ( tmpNom, tmp, s ) ;
			tmpNom [ (int) s ] = '\0' ;
	      	Nom_ligne [ i ] = tmpNom ;
		}
	// Read COLUMN names : Lit le nom des colonnes ------------------------
		for ( i = 0 ; i < NBR_SOMMET ; i++ )
		{
			fscanf ( ptr, "%49s",  tmp ) ;	
			s = strlen ( tmp ) ;
	      	tmpNom = (char *) calloc ( 1, s + 3 ) ;
			memcpy ( tmpNom, tmp, s ) ;
			tmpNom [ (int) s ] = '\0' ;
	      	Nom_colonne [ i ] = tmpNom ;
		}
	}

	// Lit les affectation entre chaque ligne / colone --------------
	for( i = 0 ; i < NBR_SOMMET ; i++ )
	{
		for ( j = 0 ; j < NBR_SOMMET ; j++ ) 	
		{				
			if ( EOF == fscanf( ptr, "%s", tmp) )
				ExitError( "End of File unexpected ; Try with option '-n' to not load row's and column's names" );

			if ( VerifyNbr( tmp ) ) 
				ExitError( "Char unexpected ; Expected number ; Try without option '-u' to load row's and column's names" );
			
			Matrice[ i ][ j ] = atoi( tmp );

			//if ( OPTIONS & INVERT )
				Matrice_svg[ i ][ j ] = atoi( tmp );
		}
	}

	free ( tmp ) ;
	fclose ( ptr ) ;
	return 0 ;	// Ok, tout c'est bien passé 
}

// ----------------------------------------------------------------------------
// Reduit la matrice horitalement ---------------------------------------------
float Reduit_Horizont ( void ) 
{
	int i, j ;
	float Min ;
	float Sous_Total = 0 ;	// Totale soustrait a chaque ligne de la matrice

	// Pour toutes les lignes
	for ( i = 0; i < NBR_SOMMET ; i++ )
	{
		Min = Matrice [ i ][ 0 ] ;
		// Recherche la Minimun de chaque ligne
		for ( j = 1 ; j < NBR_SOMMET ; j++ )
			if ( Matrice [ i ][ j ] < Min )
				Min = Matrice [ i ] [ j ] ;
		// Soustrait Min de chaque Ligne
		if ( Min != 0 ) 
		{
			for ( j = 0 ; j < NBR_SOMMET ; j++ )
				Matrice [ i ][ j ] -= Min ;
		}
		// Ajoute se qu on a soustrait
		Sous_Total += Min ;
	}
	return Sous_Total ;
}

// ---------------------------------------------------------------------------
// Reduit la matrice verticalement -------------------------------------------
float Reduit_Vertical ( void )
{
	int i, j ;
	float Min ;
	float Sous_Total = 0 ;	// Totale soustrait a chaque ligne de la matrice

	// Pour toutes les colonnes
	for ( j = 0 ; j < NBR_SOMMET ; j++ )
	{
		Min = Matrice [ 0 ][ j ] ;
		// Recherche la Minimun de chaque colonnes
		for ( i = 1 ; i < NBR_SOMMET ; i++ )
			if ( Matrice [ i ][ j ] < Min )
				Min = Matrice [ i ] [ j ] ;
		// Soustrait Min de chaque colonnes
		if ( Min != 0 ) 
		{
			for ( i = 0 ; i < NBR_SOMMET ; i++ )
				Matrice [ i ][ j ] -= Min ;
		}
		// Ajoute se qu on a soustrait
		Sous_Total += Min ;
	}
	return Sous_Total ;
}

// -------------------------------------------------------------------
// Marque Ligne et colonne de matrice pour ensuite la reduire --------
void Marque_Matrice_old ( void )
{
	int i, j ;
	int Changement ;

	// Init Matrice ligne et colonne
	Init_Matrice_ligne ( L_MARKE ) ;
	Init_Matrice_colonne () ;
	
	// Chek une premiere fois les lignes non marqué --------------
	for ( i = 0 ; i < NBR_SOMMET ; i ++ )
	{
		for ( j = 0 ; j < NBR_SOMMET ; j ++ )
		{
			if ( Matrice_zero[ i ][ j ] == MARKE )			
			{
				Matrice_ligne [ i ] = L_RIEN ;				
				break ;		
			}
		}
	}
	// ------------------------------------------------------------
	do 
	{	
		Changement = 0 ; // Regarde si y a eu un changement

		// Pour chaque ligne marqué, marque colonne avec zero BARRE
		for ( i = 0 ; i < NBR_SOMMET ; i ++ )
		{
			if ( Matrice_ligne [ i ] == L_MARKE )
			{
				// Verifie les zero marqué
				for ( j = 0 ; j < NBR_SOMMET ; j++ )
				{
					if ( ( Matrice_zero [ i ][ j ] == BARRE ) && ( Matrice_colonne[ j ] != C_MARKE ) )
					{
						Matrice_colonne [ j ] = C_MARKE ;
						Changement = 1 ;
					}
				}
			}
		}
		// Pour chaque colonne marque, marquer ligne avec zero MARKE
		for ( j = 0 ; j < NBR_SOMMET ; j ++ )
		{
			if ( Matrice_colonne [ j ] == C_MARKE )
			{
				// Verifie les zero marqué
				for ( i = 0 ; i < NBR_SOMMET ; i++ )
				{
					if ( ( Matrice_zero [ i ][ j ] == MARKE ) && ( Matrice_ligne[ i ] != L_MARKE ) )
					{
						Matrice_ligne [ i ] = L_MARKE ;
						Changement = 1 ;
					}
				}
			}
		}

	} while ( Changement );
}

// ----------------------------------------------------------------------------
// Marque les zeros de la matrice ---------------------------------------------
int Marque_zero ( void )
{
	int n, i, j, min, ligne, nbr_zero ;

	// Copie les zero dans la Matrice TEMOIN --------------------
	Copie_zero () ;
	Init_Matrice_ligne ( L_RIEN ) ;
	Init_Matrice_colonne () ;

	// Essaye de marque 1 zero par ligne ------------------------
	for ( n = 0 ; n < NBR_SOMMET ; n++ )
	{
		min = NBR_SOMMET + 1 ; 
		ligne = -1 ; 

		// Recherche la Ligne avec le moins de Zero NON- barre
		for ( i = 0 ; i < NBR_SOMMET ; i++ )
		{
			if ( Matrice_ligne[ i ] != L_MARKE )
			{
				// Recherche le nbr de zero sur cette ligne
				nbr_zero = 0 ;
				for ( j = 0 ; j < NBR_SOMMET ; j++ )			
					if ( Matrice_zero[ i ][ j ] == ZERO )
						nbr_zero ++ ;
			
				// Si ligne avec moins de zero on enregistre
				if ( ( min > nbr_zero ) && ( nbr_zero > 0 ) )
				{
					min = nbr_zero ;
					ligne = i ;
				}
			}
		}

		if ( ligne == -1 )
			break ;

		// Marque la ligne trouvée
		for ( j = 0 ; j < NBR_SOMMET ; j ++ )
		{
			if ( Matrice_zero[ ligne ][ j ] == ZERO ) 
			{
				Matrice_zero [ ligne ][ j ] = MARKE ;
				Matrice_ligne [ ligne ] = L_MARKE ;
				Bare_ligne ( ligne ) ;
				Bare_colonne ( j ) ;
			}
		}
	}

	// Veriffie si chque ligne est maque
	for ( n = 0 ; n < NBR_SOMMET ; n++ )
		if ( Matrice_ligne[ n ] == L_RIEN )
			return 0 ;	// Une ligne non marke est trouvée

	return 1 ;	// Toutes les lignes sont marqué : c'est finit.
}

// ----------------------------------------------------------------------------
// Reduit la matrice sur les ligne MARKE et les colonne NON MARKE -------------
float Reduit_reste ( void )
{
	int i, j ;
	int Chek = 0 ; 	// Pour initialisae min 
	float min ;

	// RECHERCHE LE MIN DE S QUI RESTE ------------------------------------
	// Pour chaque ligne MARKÉ
	for ( i = 0 ; i < NBR_SOMMET ; i++ )
	{
		if ( Matrice_ligne [ i ] == L_MARKE  )
		{
			// Pour chaque colonne NON MARKE
			for ( j = 0 ; j < NBR_SOMMET ; j++ )
			{
				if ( Matrice_colonne [ j ] != C_MARKE )
				{
					if ( Chek == 0 ) 
					{
						min = Matrice[ i ][ j ] ;
						Chek = 1 ;
					}
					if ( min > Matrice[ i ][ j ] )
						min = Matrice[ i ][ j ] ;				
				}
			}
		}
	}

	// Appliqu le changement ---------------------------------------------	
	for ( i = 0 ; i < NBR_SOMMET ; i++ )
	{
		// Pour chaque colonne NON MARKE
		for ( j = 0 ; j < NBR_SOMMET ; j++ )
		{
			if ( ( Matrice_colonne[ j ] != C_MARKE ) && ( Matrice_ligne[ i ] == L_MARKE  ) )
				Matrice[ i ][ j ] -= min ;
			else if ( ( Matrice_colonne[ j ] == C_MARKE ) && ( Matrice_ligne[ i ] != L_MARKE ) )
				Matrice[ i ][ j ] += min ;
		}
	}

	return min ;
}

// ----------------------------------------------------------------------------
int main ( int agc, char ** argv ) 
{
	float tmp ;
	int NbrIteration = 1 ;
	float Affect = 0 ;

	LitOptions ( agc, argv ) ;  	// Charge le Graph contenu ds le fichier

	/* Invert the matrix */
	if ( OPTIONS & INVERT )
	{
		Invert();
	}

	/* 1 */ 
	if ( OPTIONS & VERB ) 
	{	
		printf ( "\033[1;33m\n0 : Matrice en entrée" );
		AfficheMatrice () ;
	}

	/* 2 */
	tmp = Reduit_Horizont( ) ;
	Affect += tmp ;
	if ( OPTIONS & VERB ) 
	{	
		printf ( "\033[1;33m\n1 : Reduit horizontalement de %.2f", tmp );
		AfficheMatrice () ;
	}

	/* 3 */
	tmp = Reduit_Vertical( ) ;
	Affect += tmp ;
	if ( OPTIONS & VERB ) 
	{	
		printf ( "\033[1;33m\n2 : Reduit vertivalement de %.2f", tmp );
		AfficheMatrice () ;
	}


	while (  ! Marque_zero()  )  
	{	
		Marque_Matrice () ;

		/* 4 */
		if ( OPTIONS & VERB ) 
		{	
			printf ( "\033[1;33m\n3.%d : Marquage de la matrice", NbrIteration  );
			AfficheZero ( ) ;
		}

		if ( (tmp = Reduit_reste () ) == 0 )
		{
			if ( OPTIONS & VERB )
				printf( "\n\n\033[1;31m" );
			
			printf ( "No solution\n" ) ;
			
			if ( OPTIONS & VERB )
				printf( "\033[0;m" );

			exit( EXIT_FAILURE );
		}
		
		if ( OPTIONS & VERB ) 		
			printf ( "\033[1;33m\n3.%d : Gain de %f\n", NbrIteration, tmp ) ;

		Affect += tmp ;
		NbrIteration ++ ;
	}

	if ( OPTIONS & VERB ) 
	{	
		CYAN 
		printf( "\n\n   ---------------------------------" );
		printf(   "\n----         RESULTAT FINAL        ----\n\n" );
		DEFAULT_COLOR
		Marque_Matrice () ;
		AfficheZero () ;
		printf ( "\n\n" ) ;
	}

	tmp = AfficheRelations() ;
	
	if ( OPTIONS & INVERT )
		printf ( "Maximun: %f\t Average: %f\n", tmp, tmp/NBR_SOMMET  ) ;
	else
		printf ( "Minimun: %f\t Debug: %f\n", Affect, tmp ) ;

	Decharge () ;

	return 0;
}



