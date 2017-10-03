/*********************************************************** Tirage au sort ******************************************************************/
int LosujN(int n)
{
   int x; 

   if (!(n > 0)) return 0;

   x = (int)(LosujX() * n);

   if (x < n) return x;
   else return n - 1;
 
}

int Losuj(double p)
{
   if (LosujX() <= p) return TRUE;
   else return FALSE;

}

int LosujT(int *t, int n)
{
  int gotowe = FALSE;
  int k;
 
  while (!gotowe)
  { k = LosujN(n);

	if (!t[k])
	{ gotowe = TRUE;
      t[k] = TRUE;
	}
  }

  return k;
}
/*********************************************************************************************************************************************/

/*************************************************************** Roulette ********************************************************************/
int Ruletka(double *d, int n)
{
  double suma, x;
  int i;
  int stop;
	
  suma = 0;
  for (i=0; i<n; i++) suma += d[i];
 

  if (!(suma > 0)) return LosujN(n);

  x = LosujX() * suma;

  i = 0;
  suma = 0;
  stop = FALSE;

  while (!stop)
  { suma += d[i]; 
    
    if (suma >= x) stop = TRUE;
	else 
	{ i++;
	  if (!(i<n)) stop = TRUE;
	}
	
  }
   
  if (i < n) return i;
  else return n - 1;
}
/*********************************************************************************************************************************************/

void Najlepsi(int *e, int ne, double *d, int n)
{
   int i, j, k, t;
   double m;

   for (i=0; i<n; i++) e[i] = i;

   for (i=0; i<ne; i++)
   { k = -1;
     m = -_INF;

	 for (j=i; j<n; j++)
		 if (k < 0 || d[e[j]] > m)
		 { k = j;
		   m = d[e[j]];
		 }

     t = e[i];
	 e[i] = e[k];
	 e[k] = t;
   }
}


int CzyNaLiscie(int x, int *t, int n)
{
   int i;
   int k = -1;

   for (i=0; i<n && k<0; i++)
	   if (x == t[i]) k = i;


   return k;

}


typedef struct { int k, x; } ox;

int oxcmp(const void *a, const void *b)
{
  ox *pa = (ox *)a;
  ox *pb = (ox *)b;

  if (pa->k < pb->k) return -1;
  else
  if (pa->k > pb->k) return 1;
  else return 0;
	
}

int icmp(const void *a, const void *b)
{
  int *pa = (int *)a;
  int *pb = (int *)b;

  if (*pa < *pb) return -1;
  else
  if (*pa > *pb) return 1;
  else return 0;
	
}

