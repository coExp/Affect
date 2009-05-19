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
struct Chainon
{
	int nbr ;
	int rc  ;
	struct Chainon * next;
};
struct Chainon * AddChainon ( struct Chainon * L, int nbr, int rc );
struct Chainon * PopChainon( struct Chainon * L );
struct Chainon * Marque_Ligne( struct Chainon * List, int l );
struct Chainon * Marque_Colonne( struct Chainon * L, int c );
// ---------------------------------------------------------------------------
// Mark the matrix
void Marque_Matrice ( void )
{
	int i, j;
	struct Chainon * Liste;		
	Init_Matrice_ligne( L_RIEN );
	Init_Matrice_colonne();

	for ( i = 0 ; i < NBR_SOMMET ; i++ )
	{
		for ( j = 0 ; j < NBR_SOMMET ; j++ )
		{
Teste:
			if ( Matrice_zero[ i ][ j ] == MARKE )
			{
				Matrice_ligne[ i ] = L_RIEN ;
				i++;
				j = 0 ;
				if ( i >= NBR_SOMMET )
					return;
				goto Teste;
			}			
		}		 
		Liste = AddChainon( NULL, i, ROW ) ;
	}

	while( Liste )
	{
		if ( Liste->rc == ROW )
			Liste = Marque_Ligne( Liste, Liste->nbr ); //Matrice_ligne[ i ] = L_MARKE ;
		else
			Liste = Marque_Colonne( Liste, Liste->nbr ); //Matrice_ligne[ i ] = L_MARKE ;

		Liste = PopChainon( Liste ) ;
	}
}

// ---------------------------------------------------------------------------
struct Chainon * AddChainon ( struct Chainon * L, int nbr, int rc )
{
	struct Chainon * C = malloc ( sizeof(struct Chainon) );
	C->nbr = nbr;
	C->rc = rc;
	C->next = NULL;
	if ( L == NULL )
		return C;

	while( L->next != NULL )
		L->next = L->next ;

	L->next = C;
	return L;
}
// ---------------------------------------------------------------------------
struct Chainon * PushChainon( struct Chainon * L, int nbr, int rc )
{
	struct Chainon * C = malloc( sizeof(struct Chainon) );
	C->nbr = nbr;
	C->rc = rc ;
	C->next = L ;
	return C;
}
// ---------------------------------------------------------------------------
struct Chainon * PopChainon( struct Chainon * L )
{
	struct Chainon * Next = L->next ;
	free( L );	
	return Next;
}
// ---------------------------------------------------------------------------
struct Chainon * Marque_Ligne( struct Chainon * List, int l )
{
	int j;
//	struct Chainon * L = NULL;

/* DEBUG */ printf("\nMarque Ligne   \t%d", l+1 );

	Matrice_ligne[ l ] = L_MARKE ;
	for ( j = 0 ; j < NBR_SOMMET ; j++ )
		if ( Matrice_zero[ l ][ j ] == BARRE  )
			List = AddChainon( List, j, COL );

	return List;
/*	while( L )
	{
		if ( Matrice_colonne[ L->key ] == C_RIEN )
			Marque_Colonne( L->key );	
		L = PopChainon( L );
	}*/
}

// ---------------------------------------------------------------------------
struct Chainon * Marque_Colonne( struct Chainon * L, int c )
{
	int i;
//	struct Chainon * L = NULL;
/* DEBUG */ printf("\nMarque Colonne \t%d", c+1 );
	Matrice_colonne[ c ] = C_MARKE ;
	for ( i = 0 ; i < NBR_SOMMET ; i++ )
		if ( Matrice_zero[ i ][ c ] == MARKE  )
			L = AddChainon( L, i, ROW );
	
	return L;
/*	while( L )
	{	
		if ( Matrice_ligne[ L->key ] == L_RIEN )
			Marque_Ligne( L->key );	
		L = PopChainon( L );
	}*/
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
