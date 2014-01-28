/*
  Last changed Time-stamp: <2006-05-29 23:21:45 xtof>
  $Id: maxmatch.c,v 1.2 2006/08/04 23:49:23 Kinwalker Exp $
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "fold_vars.h"
#include "pair_mat.h"
#include "maxmatch.h"

#define  PUBLIC
#define  PRIVATE   static

PRIVATE short ***mu;
PRIVATE short  **aa;
PRIVATE short  **pieces;

PRIVATE int minimum_loopsize  = 3;
PRIVATE int minimum_stacksize = 2;

PRIVATE void get_mm_arrays(int length);
PRIVATE int  NJ_energy(int i, int j);

PUBLIC double
mm_get_ratio(int i, int j)
{
  if (j-i+1 <= 0) return 0.0;
  return ((double)mu[i][j][0] / (double)(j-i+1));
}

PUBLIC double
mm_get_energy(int i, int j)
{
  return ((double)mu[i][j][0]);
}

char *
mm_backtrack_structure(int first, int last)
{
  int   i, k, n1, n2, mm;
  int   max = 0;
  int   nbp = 0, npieces;
  int   s, m;
  char* structure = NULL;
  /* init */

  m = minimum_loopsize;
  s = minimum_stacksize;
  structure = (char*)space((last-first+3)*sizeof(char));
  
  if ( s > 1 ) {
    /* BACKTRACKING */    
    npieces = 1;
    nbp = 0;
    pieces[1][1] = first;
    pieces[1][2] = last;

    base_pair[0].i = max;
 
    while ( npieces > 0) { 
 
      n1 = pieces[npieces][1];
      n2 = pieces[npieces][2];
      npieces--;
    
      if ( mu[n1][n2][0] >= mu[n1][n2][s] ) {     /* n1 ./. n2 */
        k = aa[n1][n2];
        if ( k > 0 ) {   /* pb k...n2 */

	  if( ((k-1)-n1) > m ) {         /*non-vanishing 1st part*/
	    npieces++ ;
	    pieces[npieces][1] = n1;
	    pieces[npieces][2] = k-1;
	  }
	  n1 = k;
	  mm = mu[n1][n2][s];
	  while ( mu[n1][n2][s] == mm ) {
	    nbp++;
	    mm--;
	    base_pair[nbp].i = n1;
	    base_pair[nbp].j = n2;
	    n1++;
	    n2--;
	  }
	  for ( i=s-1; i >=1; i--) {
	    nbp++;
	    base_pair[nbp].i = n1;
	    base_pair[nbp].j = n2;
	    n1++;
	    n2--;
	  }
          
	  if( (n2-n1) > m ) {
	    npieces++;
	    pieces[npieces][1] = n1;
	    pieces[npieces][2] = n2;
	  }
        }
        else {
	  if( ((n2-1)-n1) > m ) { 
	    npieces++;
	    pieces[npieces][1] = n1;    
	    pieces[npieces][2] = n2-1;
	  }
        }
      }
      else {                                    /* n1...n2 */
        mm = mu[n1][n2][s];
        while ( mu[n1][n2][s] == mm ) {
	  nbp++;
	  mm--;
	  base_pair[nbp].i = n1;
	  base_pair[nbp].j = n2;
	  n1++;
	  n2--;
        }
        for ( i=s-1; i >=1; i-- ) {
	  nbp++;
	  base_pair[nbp].i = n1;
	  base_pair[nbp].j = n2;
	  n1++;
	  n2--;
        }
        if( (n2-n1) > m ) {
	  npieces++;
	  pieces[npieces][1] = n1;
	  pieces[npieces][2] = n2;
        }
      }

    }   /* REPEAT UNTIL npieces==0 */
  } /* END OF THE s>1 BLOCK */

  if ( s == 1 ) {
        base_pair[0].i = max;
 
    npieces = 1;
    nbp = 0;
    pieces[1][1] = first;
    pieces[1][2] = last;
    while ( npieces > 0 ) { 

      n1 = pieces[npieces][1];
      n2 = pieces[npieces][2];
      npieces--;
      k = aa[n1][n2];

      if ( k>0 ) {    /* stack von k...n2 */
	nbp++;
	base_pair[nbp].i = k;
	base_pair[nbp].j = n2;   
	if( (k-n1) > (m+1) ) {
	  npieces++;
	  pieces[npieces][1] = n1;
	  pieces[npieces][2] = k-1;
	}  
	if ( ((n2-1)-(k+1)) > m ) {
	  npieces++;
	  pieces[npieces][1] = k+1;
	  pieces[npieces][2] = n2-1;
	}
      }
      else {
	if ( (n2-n1) > (m+1) ) { 
	  npieces++;
	  pieces[npieces][1] = n1;    /* eigentlich unnoetig */
	  pieces[npieces][2] = n2-1;
	}
      }
    } /* repeat until npieces = 0 */

  }  /* END OF s==1 */

  for ( i=0; i<last-first+1; i++ ) structure[i]='.';
  structure[last-first+1]='\0';
    
  for ( i=1; i<= nbp; i++ ) {
    n1 = base_pair[i].i-first;
    n2 = base_pair[i].j-first;
    structure[n1] = '(';
    structure[n2] = ')';     
  }
  return (structure);
}


/* ------------------------------------------------------------------------- */
PUBLIC  short***
get_mu_array(void)
{
  return (mu);
}

/* ------------------------------------------------------------------------- */
PUBLIC void
mm_fill_arrays(char *string, char *structure)
{
  int    i,j,k,p,t,n;
  int    max, kmax;
  int    m,s;
  short *sq;

  /* init */

  m = minimum_loopsize;
  s = minimum_stacksize;

  n = strlen(string);
  for ( i=0; i<=n; i++ ) {
    for ( j=0; j<=n; j++ ) {
      aa[i][j] = 0;
      for (t=0; t<=s; t++ ) {
	mu[i][j][t] = 0;
      }
    }
  }
	
  n  = strlen(string);
  sq = (short *) space((unsigned) (n+1)*sizeof(short));

  for ( i=1; i<=n; i++ )
    sq[i] = encode_char(string[i-1]);

  if ( s > 1 ) { /* case: minimal stack size > 1 */
    /* interate */
    for ( p = m+1; p<=n; p++ ) {
      for ( i=1; i<= n-p; i++ ) {
	j = i + p;
	/* no terminal match */
	max = 0; kmax = 0;
	for ( k=i; k<j-m; k++ ) {
	  if( (mu[k][j][s] > 0)  ) {
	    if( (mu[i][k-1][0] + mu[k][j][s]) >= max){
	      max  = mu[i][k-1][0] + mu[k][j][s];
	      kmax = k;
	    }
	    if( (mu[i][k-1][s] + mu[k][j][s]) >= max){
	      max  = mu[i][k-1][s]+mu[k][j][s];
	      kmax = k;
	    }
	  }
	  if ( mu[i][j-1][0] > max ){
	    max  = mu[i][j-1][0];
	    kmax = 0;
	  }
	  if ( mu[i][j-1][s] > max ){
	    max  =  mu[i][j-1][s];
	    kmax = 0;
	  }
	}
	mu[i][j][0] = max;
	aa[i][j]    = kmax;

	/* s or more terminal base pairs */
	max = 0;
	if ( pair[sq[i]][sq[j]] ) {
	  if ( mu[i+1][j-1][s-1] > 0 ) { 
	    max = mu[i+1][j-1][s-1];
	    if ( mu[i+1][j-1][s] > max ) max =  mu[i+1][j-1][s];
	    max = max + NJ_energy(sq[i], sq[j]);
	  }
	}
	mu[i][j][s] = max;

	/* exactly one terminal base pair */
	max = 0;
	if ( pair[sq[i]][sq[j]] ) {
	  max = mu[i+1][j-1][0] + 1;
	}
	mu[i][j][1] = max;

	/* any other number of base pairs */
 	for( t=2; t <= s-1; t++) {
	  max = 0;
	  if ( (pair[sq[i]][sq[j]]) && (mu[i+1][j-1][t-1] > 0) ) {
	    max = mu[i+1][j-1][t-1] + NJ_energy(sq[i], sq[j]);
	  }
	  mu[i][j][t] = max;
	}
      }
    } 
    max = mu[1][n][0];
    if ( mu[1][n][s] >= max ) max = mu[1][n][s];
  }

  if ( s == 1 ) { /* case: minimal stack size == 1 */     
    /* iterate */
    for ( p=m+1; p<=n; p++ ) {
      for( i=1; i<= n-p; i++ ) {
	j = i + p;
	max = mu[i][j-1][0]; kmax = 0;    /* no additional bracket */
	for ( k=i; k<j-m; k++ ) {
	  if ( pair[sq[k]][sq[j]] ) {
	    if( (mu[i][k-1][0] + mu[k+1][j-1][0]+1) > max ) { 
	      max  = mu[i][k-1][0] + mu[k+1][j-1][0] + NJ_energy(sq[k], sq[j]);
	      kmax = k;
	    }
	  }
	}
	mu[i][j][0] = max;
	aa[i][j] = kmax;
      }
    }
    max = mu[1][n][0];
  }
  /* mm_print_matrix(mu, string); */
}

/* ------------------------------------------------------------------------- */

PUBLIC float max_matching(char *string, char *structure) 
{
  int    i,j,k,p,t,n1,n2,mm,n;
  int    max=0, kmax;
  int    nbp=0, npieces;
  int    m,s;
  short *sq;


  /* init */

  m = minimum_loopsize;
  s = minimum_stacksize;

  n = strlen(string);
  for ( i=0; i<=n; i++ ) {
    for ( j=0; j<=n; j++ ) {
      aa[i][j] = 0;
      for (t=0; t<=s; t++ ) {
	mu[i][j][t] = 0;
      }
    }
  }
	
  n = strlen(string);
  sq = (short *) space((unsigned) (n+1)*sizeof(short));

  /* encode sequence numerically */
  for ( i=1; i<=n; i++ )
    sq[i] = encode_char(string[i-1]);

  /* interate */

  if ( s > 1 ) {

    for ( p = m+1; p<=n; p++ ) {
      for ( i=1; i<= n-p; i++ ) {
	j=i+p;
            
	/* no terminal match */

	max = 0; kmax = 0;
	for ( k=i; k<j-m; k++ ) {
	  if( (mu[k][j][s] > 0)  ) {
	    if( (mu[i][k-1][0] + mu[k][j][s]) >= max){
	      max  = mu[i][k-1][0] + mu[k][j][s];
	      kmax = k;
	    }
	    if( (mu[i][k-1][s] + mu[k][j][s]) >= max){
	      max  = mu[i][k-1][s] + mu[k][j][s];
	      kmax = k;
	    }
	  }
	  if ( mu[i][j-1][0] > max ){
	    max  =  mu[i][j-1][0];
	    kmax = 0;
	  }
	  if ( mu[i][j-1][s] > max ){
	    max  =  mu[i][j-1][s];
	    kmax = 0;
	  }
	}
	mu[i][j][0] = max;
	aa[i][j]    = kmax;

	/* s or more terminal base pairs */

	max = 0;
	if ( pair[sq[i]][sq[j]] ) {
	  if ( mu[i+1][j-1][s-1] > 0 ) { 
	    max = mu[i+1][j-1][s-1];
	    if ( mu[i+1][j-1][s] > max ) max = mu[i+1][j-1][s];
	    max = max + NJ_energy(sq[i], sq[j]);
	  }
	}
	mu[i][j][s] = max;

	/* exactly one terminal base pair */

	max = 0;
	if ( pair[sq[i]][sq[j]] ) {
	  max = mu[i+1][j-1][0] + NJ_energy(sq[i], sq[j]);
	}
	mu[i][j][1] = max;

	/* any other number of base pairs */
 
	for( t=2; t <= s-1; t++) {
	  max = 0;
	  if ( (pair[sq[i]][sq[j]]) && (mu[i+1][j-1][t-1] > 0) ) {
	    max = mu[i+1][j-1][t-1] + NJ_energy(sq[i], sq[j]);
	  }
	  mu[i][j][t] = max;
	}
      }
    } 
    max = mu[1][n][0];
    if ( mu[1][n][s] >= max ) max = mu[1][n][s];

    /* BACKTRACKING */    

    npieces = 1;
    nbp = 0;
    pieces[1][1] = 1;
    pieces[1][2] = n;

    base_pair[0].i = max;
 
    while ( npieces > 0) { 
 
      n1 = pieces[npieces][1];
      n2 = pieces[npieces][2];
      npieces--;
    
      if ( mu[n1][n2][0] >= mu[n1][n2][s] ) {     /* n1 ./. n2 */
        k = aa[n1][n2];
        if ( k > 0 ) {   /* pb k...n2 */

	  if( ((k-1)-n1) > m ) {         /*non-vanishing 1st part*/
	    npieces++ ;
	    pieces[npieces][1] = n1;
	    pieces[npieces][2] = k-1;
	  }
	  n1 = k;
	  mm = mu[n1][n2][s];
	  while ( mu[n1][n2][s] == mm ) {
	    nbp++;
	    mm--;
	    base_pair[nbp].i = n1;
	    base_pair[nbp].j = n2;
	    n1++;
	    n2--;
	  }
	  for ( i=s-1; i >=1; i--) {
	    nbp++;
	    base_pair[nbp].i = n1;
	    base_pair[nbp].j = n2;
	    n1++;
	    n2--;
	  }
          
	  if( (n2-n1) > m ) {
	    npieces++;
	    pieces[npieces][1] = n1;
	    pieces[npieces][2] = n2;
	  }
        }
        else {
	  if( ((n2-1)-n1) > m ) { 
	    npieces++;
	    pieces[npieces][1] = n1;    
	    pieces[npieces][2] = n2-1;
	  }
        }
      }
      else {                                    /* n1...n2 */
        mm = mu[n1][n2][s];
        while ( mu[n1][n2][s] == mm ) {
	  nbp++;
	  mm--;
	  base_pair[nbp].i = n1;
	  base_pair[nbp].j = n2;
	  n1++;
	  n2--;
        }
        for ( i=s-1; i >=1; i-- ) {
	  nbp++;
	  base_pair[nbp].i = n1;
	  base_pair[nbp].j = n2;
	  n1++;
	  n2--;
        }
        if( (n2-n1) > m ) {
	  npieces++;
	  pieces[npieces][1] = n1;
	  pieces[npieces][2] = n2;
        }
      }

    }   /* REPEAT UNTIL npieces==0 */

  } /* END OF THE s>1 BLOCK */

  if ( s == 1 ) {     
    /* iterate */
    for ( p=m+1; p<=n; p++ ) {
      for( i=1; i<= n-p; i++ ) {
	j=i+p;
	max = mu[i][j-1][0]; kmax = 0;    /* no additional bracket */
	for ( k=i; k<j-m; k++ ) {
	  if ( pair[sq[k]][sq[j]] ) {
	    if( (mu[i][k-1][0] + mu[k+1][j-1][0]
		 + NJ_energy(sq[k], sq[j])) > max ) { 
		  max  = mu[i][k-1][0] + mu[k+1][j-1][0]
		    + NJ_energy(sq[k], sq[j]);
	      kmax = k;
	    }
	  }
	}
	mu[i][j][0] = max;
	aa[i][j] = kmax;
      }
    }
    max = mu[1][n][0];

    /* mm_print_matrix(mu, string); */

    /* BACKTRACKING */

    base_pair[0].i = max;
 
    npieces = 1;
    nbp = 0;
    pieces[1][1] = 1;
    pieces[1][2] = n;
    while ( npieces > 0 ) { 

      n1 = pieces[npieces][1];
      n2 = pieces[npieces][2];
      npieces--;
      k = aa[n1][n2];

      if ( k>0 ) {    /* stack von k...n2 */
	nbp++;
	base_pair[nbp].i = k;
	base_pair[nbp].j = n2;   
	if( (k-n1) > (m+1) ) {
	  npieces++;
	  pieces[npieces][1] = n1;
	  pieces[npieces][2] = k-1;
	}  
	if ( ((n2-1)-(k+1)) > m ) {
	  npieces++;
	  pieces[npieces][1] = k+1;
	  pieces[npieces][2] = n2-1;
	}
      }
      else {
	if ( (n2-n1) > (m+1) ) { 
	  npieces++;
	  pieces[npieces][1] = n1;    /* eigentlich unnoetig */
	  pieces[npieces][2] = n2-1;
	}
      }
    } /* repeat until npieces = 0 */

  }  /* END OF s==1 */

  for ( i=0; i<n; i++ ) structure[i]='.';
  structure[n]='\0';
    
  for ( i=1; i<= nbp; i++ ) {
    n1 = base_pair[i].i;
    n2 = base_pair[i].j;
    structure[n1-1] = '(';
    structure[n2-1] = ')';     
  }

  return (float)max;
}

/* ------------------------------------------------------------------------- */

PUBLIC void
init_maxmat(int length, int minls, int minss)
{
  if ( minls > -1 ) minimum_loopsize = minls;
  if ( minss > -1 ) minimum_stacksize = minss;
  
  make_pair_matrix();
  get_mm_arrays(length);
}   

/* ------------------------------------------------------------------------- */

PUBLIC void
update_mm_params(void)
{
  make_pair_matrix();
}

/* ------------------------------------------------------------------------- */

PRIVATE int
NJ_energy(int i, int j)
{ /* Nussinov Jacobson energy model */
  int A, U, G, C;
  A = encode_char('A');
  U = encode_char('U');
  G = encode_char('G');
  C = encode_char('C');

  if ((i == G && j == C) || (i == C && j == G)) return (3); /* GC CG */
  if ((i == A && j == U) || (i == U && j == A)) return (2); /* AU UA */
  if ((i == G && j == U) || (i == U && j == G)) return (1); /* GU UG */
  return (0); /* unknown base pair */
}

/* ------------------------------------------------------------------------- */

PRIVATE void get_mm_arrays(int length)
{
  int i, j;
  int s;
  s = minimum_stacksize;

  mu = (short ***)space((unsigned) (length+1)*sizeof(short **));
  for ( i=0; i<=length; i++ ) {
    mu[i] = (short **) space((unsigned) (length+1)*sizeof(short *));
    for ( j=0; j<=length; j++ ) {
      if ( s>1 ) mu[i][j] = (short *)space((unsigned) (s+1)*sizeof(short));
      else       mu[i][j] = (short *)space((unsigned) 1*sizeof(short));
    }
  }

  aa = (short **)space((unsigned) (length+1)*sizeof(short *));
  for ( i=0; i<=length; i++ )
    aa[i]=(short *) space((unsigned) (length+1)*sizeof(short ));

  pieces = (short **)space((unsigned) (length+1)*sizeof(short *));
  base_pair = (struct bond *) space((1+length/2)*sizeof(struct bond));

  for ( i=0; i<=length; i++ ) {
    pieces[i] = (short *) space((unsigned) (length+1)*sizeof(short));
    pieces[i]--;
  }
}

/* ------------------------------------------------------------------------- */

PUBLIC void free_mm_arrays(int length)
{
  int i,j;

  for ( i=0; i<=length; i++ ) {
    for ( j=0; j<=length; j++ )
      free((char *)mu[i][j]);
    free((char *)mu[i]);
    free((char *)aa[i]);
    free((char *)(pieces[i]+1));
  }
  free((char *)base_pair);
  free((char *)mu);
  free((char *)aa);
  free((char *)pieces);
}

/* ------------------------------------------------------------------------- */

PUBLIC void
mm_print_matrix(short ***m, char *string) {
  int i,j,l;
  l = strlen(string);
  printf("   ");
  for (j=0; j<l; j++)
    printf("%4c", string[j]);
  printf("\n   ");
  for (j=0; j<l; j++)
    printf("%4d", j+1);
  printf("\n   ");
  for (j=0; j<4*l+1; j++) printf("-");
  printf("\n");
  for (i=0; i<l; i++) {
    printf("%2d ", i+1);
    for (j=0; j<i; j++)
      printf("    ");
    for (; j<l; j++)
      printf("%4d", m[i+1][j+1][0]);
    printf("%4c\n", string[i]);
  }
}
