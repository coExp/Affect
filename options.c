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

	// FICH : Si un fichier bien entre
	if ( ! ( OPTIONS & FICH ) )
	{
		ShowHelp ( argv[0] ) ;
		exit ( EXIT_FAILURE ) ;
	}

	// Chager le fichier avec les graph des conso et max
	if ( ChargeFichier ( NomFichier ) )
		exit ( EXIT_FAILURE ) ; 
}
