// ---------------------------------------------------------------------------
// Mark the matrix
void Marque_Matrice_Essai ( void )
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

	Matrice_ligne[ l ] = L_MARKE ;
	for ( j = 0 ; j < NBR_SOMMET ; j++ )
		if ( Matrice_zero[ l ][ j ] == BARRE  )
			List = AddChainon( List, j, COL );

	return List;
}

// ---------------------------------------------------------------------------
struct Chainon * Marque_Colonne( struct Chainon * L, int c )
{
	int i;

	Matrice_colonne[ c ] = C_MARKE ;
	for ( i = 0 ; i < NBR_SOMMET ; i++ )
		if ( Matrice_zero[ i ][ c ] == MARKE  )
			L = AddChainon( L, i, ROW );
	
	return L;
}