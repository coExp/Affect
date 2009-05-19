/******************************************************************/
/**																 **/
/**   AFFICHE.C                                                  **/
/**																 **/
/******************************************************************/

#include "affect.h"

// Affiche un message d'aide -------------------------------------------------
void ShowHelp ( char *Name )
{
	printf ( "Usage: %s [OPTIONS] -f FILE", Name );
	printf ( "\nLinear assignment of matrix in FILE. Assign 1 row with 1 column AND minimize costs " ) ;
	printf ( "\nExample: %s -nf Ex.txt", Name );
	printf ( "\n\t-f foo.txt : load the matrix of FILE foo.txt" ) ;
	printf ( "\n\t-v : mode verbose, bavard" ) ;
	printf ( "\n\t-n : do not read name" ) ;
	printf ( "\n\t-i : invert the matrix, maximize gain/score" ) ;
	printf ( "\n\t-h : this message" ) ;
	printf ( "\n\t-e : show example of FILE" ) ;
	printf ( "\n" ) ;
}

// Print an error message et exit --------------------------------------------
void ExitError ( char * Mess )
{
	fprintf ( stderr, "%s\n", Mess );
	exit ( EXIT_FAILURE );
}


// Show a short example of FILE ----------------------------------------------
void ShowExample ( char *Name )
{
//	printf ( "\nn\nName_Rows...\nName_Columns\n...\n" );
	printf ( "\n[%s -nf FILE] : \n\n", Name );
	printf ( "4\n15 42 98 52\n12 45 89 325\n10 455 87 32\n45 42 98 52\n\n" );
	printf ( "[%s -f FILE] : \n\n", Name );
	printf ( "3\nR1 R2 R3\nC1 C2 C3\n88 108 100\n89 105 106\n90 107 98\n\n" );
}

// Affiche les relation trouvé pour affectation lineaire ---------------------
float AfficheRelations ( void ) 
{
	int i, j ;
	float max = 0 ;	

	for ( i = 0 ; i < NBR_SOMMET ; i++ )
		for ( j = 0 ; j < NBR_SOMMET ; j++ )
			if ( Matrice_zero [ i ][ j ] == MARKE )
			{
				if ( OPTIONS & PAS_NOM )
					printf( "L%d C%d\n",  i+1 , j+1  ) ;
				else 
					printf( "%s %s\n", Nom_ligne [ i ], Nom_colonne [ j ] ) ;

				max += Matrice_svg[ i ][ j ];
			}
	return max;
}

// Affiche la matrice avec les Zero MARKE et BARRE
void AfficheZero ( void )
{
	int i, j, tmp, ligne_vide ;
	printf( "\n\t" );
	for ( j = 0 ; j < NBR_SOMMET ; j++ )
	{
		if ( Matrice_colonne[ j ] == C_MARKE  )
			printf( "   @  " );
		else
			printf( "      " );
	}
		
	for ( i = 0 ; i < NBR_SOMMET ; i++ )
	{
		printf ( "\n\t" ) ;
		ligne_vide = 0 ;
		for ( j = 0 ; j < NBR_SOMMET ; j ++ )
		{

			if ( ( Matrice_colonne[ j ] == C_MARKE ) || ( Matrice_ligne[ i ] != L_MARKE ) )				
				tmp = C_MARKE ;
			else 
				tmp = C_RIEN ;

			switch (  Matrice_zero [ i ][ j ] | tmp  )
			{
				case ZERO | C_RIEN :	// Erreur 
				case MARKE | C_RIEN :
				case BARRE | C_RIEN :
					printf ( "\033[1;46;30m" ) ;	
					break ;

				case BARRE | C_MARKE :
					printf ( "\033[1;42;31m" ) ;	
					break ;				
	
				case RIEN | C_MARKE :
					printf ( "\033[1;42;33m" ) ;	
					break ;					

				case MARKE | C_MARKE :
					printf ( "\033[1;41;33m" ) ;	
					break ;

				case RIEN | C_RIEN :
					printf ( "\033[0;m" ) ;
					break ;
			}

			printf ( " %3.0f ", Matrice[ i ][ j ] ) ;
			printf ( "\033[0;m" ) ;

			printf ( " " ) ;

			if ( Matrice_zero[ i ][ j ] == MARKE )
				ligne_vide = 1 ;
		}
		
		if ( Matrice_ligne[ i ] == L_MARKE  )
			printf( " @ " );
		else
			printf( "   " );

		if ( ligne_vide == 0 )
			printf ( "\033[0;33m  <- Non Marquée\033[0;m" ) ;
	}
}

void AfficheMatrice ( void )
{
	int i, j ;
	
	printf ( "\033[0;m\n\t" ) ;
	for ( i = 0 ; i < NBR_SOMMET ; i++ )
	{
		for ( j = 0 ; j < NBR_SOMMET ; j ++ )
			printf ( "%2.0f\t", Matrice[ i ][ j ]  ) ;
		printf ( "\n\t" ) ;
	}
}

