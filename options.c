#include "affect.h"

// Lit les options entrées en arguments -------------------------------------
void LitOptions  ( int argc, char ** argv )
{
	int opt ;
	char * NomFichier;
	OPTIONS = 0 ;

	while ( ( opt = getopt(argc, argv, "ievf:hn") ) != -1 ) 
	{
		switch ( opt ) 
		{
			case 'v' :
				OPTIONS |= VERB ; 
				break;
			case 'i' :
				OPTIONS |= INVERT ; 
				break;
			case 'n' :
				OPTIONS |= PAS_NOM ; 
				break;
			case 'f' :
				NomFichier = optarg ;
				OPTIONS |= FICH ;
				break;
			case 'h' :
				ShowHelp( argv[0] ) ;
				exit ( EXIT_SUCCESS ) ;
				break;
			case 'e' :
				ShowHelp( argv[0] ) ;
				ShowExample( argv[0] ) ;
				exit ( EXIT_SUCCESS ) ;
				break;
			default: 	
				fprintf ( stderr, "\nError: unkonwn argument" ) ;
				ShowHelp ( argv[0] ) ;
				exit ( EXIT_FAILURE ) ;
		}
	}

	if ( ! ( OPTIONS & FICH ) )	{
		ShowHelp ( argv[0] ) ;
		exit ( EXIT_FAILURE ) ;
	}

	if ( loadFile ( NomFichier ) )
		exit ( EXIT_FAILURE ) ; 
}

// Load file and matrix in memory ---------------------------------------
int loadFile ( char * fichier )
{
	FILE *ptr;		// Pointer on the File
	size_t s;		// Size of name
	int i, j ;		
	char *tmp = (char *) calloc ( 1, 50 ) ;	// Tempory name of sommet 
	char *tmpNom ;	// Pointeur vers la vraie future zone memoire du nom

	if ( ( ptr = fopen ( fichier, "r+" ) ) == NULL )	{	
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
