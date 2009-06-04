#include "affect.h"


// ----------------------------------------------------------------------------
// Invert the matrix ----------------------------------------------------------
void Invert ( void )
{
	int i,j ;
	float Max = Matrice[ 0 ][ 0 ] ;

	// Looking for max coef
	for ( i = 0 ; i < NBR_SOMMET ; i ++ )
	{
		for ( j = 0 ; j < NBR_SOMMET ; j ++ )
		{
			if ( Matrice[ i ][ j ] > Max )
				Max = Matrice[ i ][ j ];
		}
	}

	// Invert the matrix
	for ( i = 0 ; i < NBR_SOMMET ; i ++ )
		for ( j = 0 ; j < NBR_SOMMET ; j ++ )
			Matrice[ i ][ j ] = Max - Matrice[ i ][ j ];
}


// ----------------------------------------------------------------------------
// Marque tout les zero de la colonne -----------------------------------------
void Bare_ligne( int ligne )
{
	int j;
	for ( j = 0 ; j < NBR_SOMMET ; j ++ )
	{
		if ( Matrice_zero[ ligne ][ j ] == ZERO )
			Matrice_zero [ ligne ][ j ] = BARRE ;	
	}
}

// ----------------------------------------------------------------------------
// Marque tout les zero de la colonne -----------------------------------------
void Bare_colonne( int colonne )
{
	int i;
	for ( i = 0 ; i < NBR_SOMMET ; i ++ )
	{
		if ( Matrice_zero[ i ][ colonne ] == ZERO )
			Matrice_zero [ i ][ colonne ] = BARRE ;	
	}
}

// ----------------------------------------------------------------------------
// Copie les zeros dans la Matrice_zero, pour les marqué ----------------------
void Copie_zero ( void )
{
	int i, j ;

	for ( i = 0 ; i < NBR_SOMMET ; i++ )
	{
		Matrice_ligne [ i ] = L_RIEN ;
		for ( j = 0 ; j < NBR_SOMMET ; j++ )
		{
			if ( Matrice[ i ][ j ] != 0.0 )
				Matrice_zero[ i ][ j ] = RIEN ;
			else
				Matrice_zero[ i ][ j ] = ZERO ;
		}
	}		
}

// ------------------------------------------------------------------
// Initialise la matrice ligne --------------------------------------
void Init_Matrice_ligne ( int Marquage ) 
{
	int i ;
	for ( i = 0 ; i < NBR_SOMMET ; i++ )
		Matrice_ligne [ i ] = Marquage ;
}

// -------------------------------------------------------------------
// Initialise la matrice colone --------------------------------------
void Init_Matrice_colonne ( void ) 
{
	int j ;
	for ( j = 0 ; j < NBR_SOMMET ; j++ )
		Matrice_colonne [ j ] = C_RIEN ;
}

// ---------------------------------------------------------------------------
// Verify the number ---------------------------------------------------------
int VerifyNbr ( char * Nbr )
{
	int i, len;

	len = strlen( Nbr );
	for ( i = 0 ; i < len ; i++ )
		if ( ! isdigit( Nbr[i] ) )
			return 1 ;	// Non Ok

	return 0 ;			// OK
}


// ---------------------------------------------------------------------------
void Decharge ( void )
{
	int j ;
	free ( Matrice_colonne ) ;
	free ( Matrice_ligne ) ;
	for ( j = 0 ; j < NBR_SOMMET ; j++ )
	{
		free ( Matrice[ j ] ) ;
		free ( Matrice_zero [ j ] ) ;
		free ( Nom_colonne[ j ] ) ;
		free ( Nom_ligne[ j ] ) ;
	}
	free ( Matrice ) ;
	free ( Matrice_zero ) ;
	free ( Nom_colonne ) ;
	free ( Nom_ligne ) ;
}
