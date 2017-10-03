#define NPOJ 10
#define NZGL 1000


class Osobnik { 
                               private:          int t[NPOJ][NZGL]; // Squelette d'un individu
						 int n[NPOJ]; // nombre d'appelde chaque véhicule : n[10] = {8, 7, 5, 9, 7, 3, 5, 6, 2, 4}
						 int npoj, npoj0; // Nombre de voiture
						 int p[NPOJ];
						 double kDr, kD0, kVaT, kVmT; // Valeur des variables d'un individu (Delai , Distance).

				public:         Osobnik(void); 
						int Mutacja(void);
						int Mutacja0(void);
						int Mutacja1(void);
						int Mutacja2(void);
					        int Mutacja3(void);
						int Mutacja4(void);
						int Mutacja5(void);
						int Cross(Osobnik &o);
						int OdtworzSciezke(int k, Graf &g, int baza, Zgloszenie *Z, int *V, Pojazd *pp, Sciezka *ps);
						int Inicjuj(int np, int nz, int np0);
						int Przepisz(int k, int *z);
						int DopiszZglosz(int k, int z);
						void operator = (Osobnik &o);
						double Ocena(double d0, double *D0, Graf &g, Zgloszenie *z, int nz, double vp, double czas_r, double czas_d, int *V, int baza, Pojazd *pp);
						double Oblicz(double Dr, double D0, double D, double VaT, double VmT);
						double DrogaRef(int np, Graf &g, int baza, Zgloszenie *Z, int nz, int *V, Pojazd *pp, int *oz);
						void _OX(int *d, int *s, int n, int k, int *o);
						int OX(int *p);
						int Zapisz(FILE *f);	
						int Wynik(double fobj, double D0, FILE *f, int nl);
						int ZapiszSzciezke(char *zadanie, int nz, Graf &g, int baza, Zgloszenie *Z, int *V, Pojazd *pp);
					
			};



Osobnik::Osobnik(void)
{ 
  int i; 

  for (i=0; i<NPOJ; i++) n[i] = 0;
   
}




int Osobnik::DopiszZglosz(int k, int z) // Ajouter un nouvel appel de reprise (k : indice voiture , z : nouvel appel)
{

   if (n[k] < NZGL)
   { t[k][n[k]] = z;
     (n[k]) ++;

	 return TRUE;
   } 
   else return FALSE;

}



int Osobnik::ZapiszSzciezke(char *zadanie, int nz, Graf &g, int baza, Zgloszenie *Z, int *V, Pojazd *pp) // Sauvegarder un Itinéraire
{
  char nazwa[DLGNAZWY];
  FILE *f;
  int i, j;
  Sciezka *ps = new Sciezka;

  strcpy(nazwa, zadanie);
  strcat(nazwa, "~.sc");

  f = fopen(nazwa, "wt");
  if (f == NULL) return FALSE;

  fprintf(f, "%d\t%d\n", npoj, nz);

  for (i=0; i<npoj; i++) // Pour chaque véhicule 
  { OdtworzSciezke(i, g, baza, Z, V, pp, ps); // Calcule de l'itinéraire de chaque véhicule
	
    fprintf(f, "%d\t%d\n", i, ps->ns); // ps->ns peut etre noeud indice du noeud final

    for (j=0; j<ps->ns; j++) fprintf(f, "%d\t%d\t%d\n", i, j, ps->s[j].w); // i = n° de la voiture, j = indice du noeud, s[] = liste des noeuds
  }

  fclose(f);

  delete ps;


  return TRUE;
}


int Osobnik::OdtworzSciezke(int k, Graf &g, int baza, Zgloszenie *Z, int *V, Pojazd *pp, Sciezka *ps) // Reconstruction de l'itinéraire
{
  int w0, w1;
  int s[NWK], ds;
  int i, j;
  double pi;
  int ok;
  int idz;
  Pojazd *pk = pp + p[k];
 
  ps->Zeruj(); // Initialisation de l'itinéraire

  pk->Oproznij(); // Vidage du conteneur

  w0 = baza; // Initialisation du noeud de base
  ps->DopiszWezel(w0, -2); // Ajout d'un noeud a la liste de noeud de l'itinéraire
   

  ok = TRUE;
  for (i=0; i<n[k] && ok; i++) // On parcourt la liste des appels d'un véhicule
  { 
        idz = t[k][i]; // on recupere l'id du noeud suivant
	w1 = V[idz]; // On recupere le noeud suivant
        pi = Z[idz].CzytajO(); // On recupère le volume de l'equipement du noeud suivant

	if (!pk->Zaladuj(pi)) // Si le véhicule est plein 
	{ 
          ds = g.Dijkstra(w0, baza, s); // On part du dernier noeud visité (car wo = w1) a la base
	  if (ds < 0) ok = FALSE;
	  else 
	  for (j=1; j<ds; j++) ps->DopiszWezel(s[j], (j == ds - 1? -2: -1));

	  pk->Oproznij(); // On vide le conteneur

	  ds = g.Dijkstra(baza, w1, s); // On part de la base au dernier noeud visité + 1
	  if (ds < 0) ok = FALSE;
	  else 
	  for (j=1; j<ds; j++) ps->DopiszWezel(s[j], (j == ds - 1? idz: -1));

	  pk->Zaladuj(pi); // On ajoute l'equipement au camion
	}
	else // Si le véhicule n'est pas plein
	{ ds = g.Dijkstra(w0, w1, s); // On part du dernier noeud visité au noeud suivant
	  if (ds < 0) ok = FALSE;
	  else 
	  for (j=1; j<ds; j++) ps->DopiszWezel(s[j], (j == ds - 1? idz: -1));
	}
	
	w0 = w1; // Le noeud suivant devient le noeud de base
  }

  if (ok) // Si tout s'est bien passé
  { ds = g.Dijkstra(w0, baza, s); // On part du dernier noeud du parcourt a la base
    if (ds < 0) ok = FALSE;
    else 
    for (j=1; j<ds; j++) ps->DopiszWezel(s[j], (j == ds - 1? -2 :-1));
  }


  return ok;

}



int Osobnik::Zapisz(FILE *f) // Sauvegarder 
{
  int i, j;

  fprintf(f, "\t%lg\t%lg\t%lg\t%lg\t%d", kVaT, kVmT, kD0, kDr, npoj);

  for (i=0; i<npoj; i++)
  { fprintf(f, "\t%d\t%d:", p[i], n[i]);

	for (j=0; j<n[i]; j++) fprintf(f, " %d", t[i][j]);
	fprintf(f, " ;");
  }
  
  return TRUE;
}
 

int Osobnik::Wynik(double fobj, double D0, FILE *f, int nl) // Sauvegarder les résultats
{
 
  if (!(f == NULL))
	 fprintf(f, "%lg\t%5.0f\t%5.0f\t%10.0f%c", fobj, kVaT * 60, kVmT * 60, kD0 * D0, (nl? '\n':'\t'));

  return TRUE;
}


/**************************************************************** Evaluation *****************************************************************/
double Osobnik::Ocena(double Dr, double *D0, Graf &g, Zgloszenie *z, int nz, double vp, double czas_r, double czas_d, int *V, int baza, Pojazd *pp) // Algorithme de calcul des delais et distances de chaque individu
{
  double D, Dj;
  int i, j, j0, w, idz, jest;
  double dT;
  double T, Tz, Tp, Ts;
  double VaT, VmT, VT;
  int s[NWK], ds;
  Sciezka *ps = new Sciezka;
  double kx, o1;

  D = 0;
  VaT = 0;
  VmT = 0;

  for (i=0; i<npoj; i++)
  { OdtworzSciezke(i, g, baza, z, V, pp, ps); // j'enregistre l'itinéraire de chaque véhicule dans ps
    kx = pp[p[i]].koszt; // j'enregistre son cout

	jest = FALSE;
        j = 0;
        ds = 0;
	while (j < ps->ns && !jest)
	{ w = ps->s[j].w;
	  idz = ps->s[j].z; // le 9eme appel est a la position 2 : 9 = ps->s[j].2
	  s[ds] = w;
          ds ++;

	  if (idz < 0) j ++;
	  else jest = TRUE; // Il y'a un appel, on sort de la boucle
	}

	if (jest)
	{ T = z[idz].CzytajT(); // heure de reprise
	  Ts = (z[idz].CzytajR() == 0? czas_d: 0); // L'equipement est il grand? oui alors on ajoute le temps de chargement
	  T += Ts;
	  
	  D += g.Dlugosc(s, ds) * kx; // les 2 premiers noeuds

	  j0 = j;
	  j ++;
          ds = 0;

	  while (j < ps->ns)
	  {     w = ps->s[j].w;
	        idz =ps->s[j].z;
		s[ds] = w;
		ds ++;

		if (!(idz < 0))
		{ Tz = z[idz].CzytajT(); // heure de reprise
		  Ts = (z[idz].CzytajR() == 0? czas_d: 0); // L'equipement est il grand? oui alors on ajoute le temps de chargement
		  
                  Dj = g.Dlugosc(s, ds); // distance entre deux noeuds 
		  D += Dj * kx;
		  ds = 0;

                  dT = (Dj / 1000) / vp; // temps effectué entre deux noeuds
                  Tp = T + dT;
                  // T = Tp + Ts;

		  if (Tp > Tz) // si le delai a été traversé
		  { 
                    VT = Tp - Tz; // Di

		    VaT = VaT + VT; // Dmoy = Dmoy + Di
			if (VT > VmT) VmT = VT; // Dmax = Di
			
			T = Tp + Ts;
		  }
		  else T = Max(Tz, Tp + Ts);

		  j0 = j;
		}
		else
        if (idz == -2) T = T + czas_r; // Si le vehicule est plein on rentre a la base pour decharger le véhicule

		j ++; // On passe au noeud suivant
	
	  }

	  D += g.Dlugosc(s, ds) * kx; // les 2 derniers noeuds

	}

  }

  VaT = VaT / nz; // moyenne des delais de tous les noeuds de l'itinéraire

  if (!(*D0 > 0)) *D0 = D;

  delete ps;

  o1 = Oblicz(Dr, *D0, D, VaT, VmT); // Appel de la fonction ci-dessous

  return o1; // retourne la valeur de la fonction objective
}




double Osobnik::Oblicz(double Dr, double D0, double D, double VaT, double VmT) // Evaluation de l'individu
{
   Fuzzy f;
   
   // void Kryterium(double x, int duza, int k) { tx[k] = x; FN(duza, k); };

   f.Kryterium(Min(f.FF(0, 0, 0.3, 1, VaT), f.FF(0, 0, 0.8, 2, D / D0)), TRUE, 0); // 1ère règle (LeftFuzzy , value)
   f.Kryterium(f.FF(0.5, 2, _INF, _INF, VmT), FALSE, 1); // 2ème règle (RightFuzzy , value)
   f.Kryterium(f.FF(2, 10, _INF, _INF, D / Dr), FALSE, 2); // 3ème règle (RightFuzzy , value)
 
   kVaT = VaT;
   kVmT = VmT;
   kDr = D / Dr;
   kD0 = D / D0;

   return f.FC(3); // Fuzzification + Defuzzification
}
/*******************************************************************************************************************************************/


/***************************************************************** Mutation ****************************************************************/
int Osobnik::Mutacja(void)
{
  int typm = (npoj0 > 1? LosujN(6): LosujN(2));
  int ok;
  
  ok = TRUE;
  switch (typm)
  { case 0: ok = Mutacja0(); break;
    case 1: ok = Mutacja1(); break;
    case 2: ok = Mutacja2(); break;
    case 3: ok = Mutacja3(); break;
    case 4: ok = Mutacja4(); break; 
    case 5: ok = Mutacja5(); break;
  }

  return ok;
}

// Permutation de deux noeuds d'un véhicule choisi au hazard (x...y -> y...x)
int Osobnik::Mutacja0(void)
{
  int ip = LosujN(npoj);
  int i1, i2, x;
  int *z = t[ip];

  i1 = LosujN(n[ip]);
  i2 = LosujN(n[ip]);


  if (i1 == i2) return TRUE;
 
  // Permutation
  x = z[i1];
  z[i1] = z[i2];
  z[i2] = x;


  return TRUE;

}

// modification de la séquence des appels de rammasage sélectionnée au hasard pour véhicule choisi au hasard (abcd -> dcba)
int Osobnik::Mutacja1(void)
{
  int ip = LosujN(npoj); // On recupere l'indice du véhicule choisi au hazard
  int *z = t[ip]; 
  int i1, i2, i, x, nn;
  
  // Selection de la position de l'appel
  i1 = LosujN(n[ip]); // i1 = a
  i2 = LosujN(n[ip]); // i2 = d

  if (i1 == i2) return TRUE;
 
  // Permutation si d < a
  if (i2 < i1)
  { x = i2;
    i2 = i1;
    i1 = x;
  }

  nn = (i2 - i1 + 1) / 2; // nn = 2

  // permutation abcd -> dcba
  for (i=0; i<nn; i++)
  { x = z[i1 + i];
    z[i1 + i] = z[i2 - i];
    z[i2 - i] = x;
  }


  return TRUE;

}

// échange de séquence sélectionnée au hasard d'appels entre une paire de véhicule choisi au hasard
int Osobnik::Mutacja2(void)
{
  // Selection au hazard des 2 véhicules
  int ip1 = LosujN(npoj);
  int ip2 = LosujN(npoj);

  int i11, i12, i21, i22;
  int *z1 = t[ip1], *z2 = t[ip2];
  int z1x[NZGL], z2x[NZGL], nk1x, nk2x;
  int i, x;
 
  if (ip1 == ip2) return TRUE;


  // Selection de la position de l'appel e et h de la sequence e...h du vehicule 1
  i11 = LosujN(n[ip1]); // e
  i12 = LosujN(n[ip1]); // h
  // Permutation si h < e
  if (i11 > i12)
  { x = i11;
    i11 = i12;
    i12 = x;
  }
  
  // Selection de la position de l'appel x et z de la sequence x...z du vehicule 2
  i21 = LosujN(n[ip2]);
  i22 = LosujN(n[ip2]);
  // Permutation si z < x
  if (i21 > i22)
  { x = i21;
    i21 = i22;
    i22 = x;
  }


  nk1x = 0;
  for (i=0; i<i11; i++) z1x[nk1x ++] = z1[i]; // z1x = abcd
  for (i=i21; i<i22; i++) z1x[nk1x ++] = z2[i]; // z1x = abcdxyz
  for (i=i12; i<n[ip1]; i++) z1x[nk1x ++] = z1[i]; // z1x = abcdxyzijkl...

  nk2x = 0;
  for (i=0; i<i21; i++) z2x[nk2x ++] = z2[i]; // z2x = tuvw
  for (i=i11; i<i12; i++) z2x[nk2x ++] = z1[i]; // z2x = tuvwabcd
  for (i=i22; i<n[ip2]; i++) z2x[nk2x ++] = z2[i]; // z2x = tuvwabcd...

 
  for (i=0; i<nk1x; i++) z1[i] = z1x[i]; // puis on met a jour la liste d'appel du vehicule 1 ainsi que son nombre d'appel
  n[ip1] = nk1x;

  for (i=0; i<nk2x; i++) z2[i] = z2x[i]; // puis on met a jour la liste d'appel du vehicule 1 ainsi que son nombre d'appel
  n[ip2] = nk2x;

   
  return TRUE;

}

// Mutation 3 - remplacement des véhicules
int Osobnik::Mutacja3(void)
{
  int ip1, ip2;
  int i;

  // Selection au hazard des 2 véhicules
  ip1 = LosujN(npoj0);
  ip2 = LosujN(npoj0);


  if (ip1 == ip2) return TRUE;

  // permutation des 2 véhicules
  for (i=0; i<npoj; i++)
  { if (p[i] == ip1) p[i] = ip2;
    else
    if (p[i] == ip2) p[i] = ip1;
  }

  return TRUE;

}

// Mutation 4 - augmenter le nombre de véhicules
int Osobnik::Mutacja4(void)
{
  int ip1, ip2, ix;
  int i, k;
  int tx[NPOJ];


  if (!(npoj < npoj0)) return TRUE;

  ip1 = LosujN(npoj);
  ip2 = npoj;

  if (n[ip1] < 2) return TRUE;

  ix = LosujN(n[ip1] - 2) + 1;

  k = 0;
  for (i=ix; i<n[ip1]; i++)
  { t[ip2][k] = t[ip1][i];
    k ++;
  }

  n[ip1] = ix;
  n[ip2] = k;

  for (i=0; i<npoj0; i++) tx[i] = FALSE;
  for (i=0; i<npoj; i++) tx[p[i]] = TRUE;

  p[ip2] = LosujT(tx, npoj0);
   
  npoj ++;
	
  return TRUE;

}

// Mutation 5 - la réduction du nombre de véhicules
int Osobnik::Mutacja5(void)
{
  int ip1, ip2;
	
  if (!(npoj > 1)) return TRUE;


  ip1 = LosujN(npoj - 1);
  ip2 = npoj - 1;
  
  Przepisz(ip2, t[ip1] + n[ip1]);

  n[ip1] += n[ip2];

  n[ip2] = 0;
  p[ip2] = 0;

  npoj --;
	
  return TRUE;

}
/*******************************************************************************************************************************************/ 

/***************************************************************** Crossover ***************************************************************/
int Osobnik::Cross(Osobnik &o)
{
  int z[NZGL], zo[NZGL]; // liste des appels de reprise.
  int i, n0;
  
  n0 = 0;
  for (i=0; i<npoj; i++) n0 += Przepisz(i, z + n0); 
  /* On retourne le nombre d'appels de chaque véhicule apres avoir recopier tous les indices des appels du véhicule dans *z 
   * Si le véh0 a 3 appels n[0] = 3 alors z[3] = {3, 6, 9} (appel 3, 6, 9) --> 1ere tour de boucle
   * A la fin n0 = le nombre d'appels de tous les véhicules et z = liste de tous les indices des appels des véhicules */
    
  n0 = 0;
  for (i=0; i<o.npoj; i++) n0 += o.Przepisz(i, zo + n0); // De meme pour l'individu passer
  

  if (!OX(zo)) return FALSE; // Passe en paramètre zo a l'autre individu
  if (!o.OX(z)) return FALSE; // De meme
  
  return TRUE;
}

int Osobnik::OX(int *p)
{
  int i;
  int k;
  int z[NZGL];
  double px;

  if (npoj > 1) px = 1.0;
  else px = 0.5;

  i = LosujN(npoj); // indice d'un vehicule choisi au hazard
  k = (int)((1.0 - px) * n[i]) + LosujN(px * n[i]); // point de rupture pour le crossover

  if (!(k > 0)) k = 1;
  else
  if (!(k < n[i] - 1)) k = n[i] - 2;

  Przepisz(i, z); // On recopie tous les indices des appels du véhicule i dans *z 

  _OX(t[i], z, n[i], k, p); // Appel de la fonction ci-dessous
  

  return TRUE;
    
}

  
void Osobnik::_OX(int *d, int *s, int n, int k, int *o) 
// d : reference sur ligne de la matrice (individu), n : nombre d'appel du véhicule, s : liste des indices des appels du véhicule i
// k = [1...n-1], o : liste de tous les indices des appels des véhicules de l'autre individu
{
  int i, j, p, q;
  ox tt[NZGL]; // { int k, x; } ox
 

  for (i=0; i<k; i++) d[i] = s[i]; // On recopie les indices jusqu'au point de rupture

  for (i=k; i<n; i++)
  { p = i - k; // p = [0...n-k]
    tt[p].x = s[i];

	q = -1;
	for (j=0; j<n && q<0; j++)
		if (s[i] == o[j]) q = j;

    tt[p].k = q;
  }

  qsort(tt, n - k, sizeof(ox), oxcmp); // on trie 


  for (i=k; i<n; i++) d[i] = tt[i - k].x; // On complete ensuite

 
}


int Osobnik::Przepisz(int k, int *z) // Recopier ( k : indice de la voiture , z : liste des appels du véhicules)
{ 

  int i;

  for (i=0; i<n[k]; i++) z[i] = t[k][i]; // On recopie les appels de reprise d'un véhicule dans *z 
  
  return n[k];
}
/*******************************************************************************************************************************************/  



void Osobnik::operator = (Osobnik &o) // Comparaison entre deux individus.
{
  int i, j;

  npoj = o.npoj;
  npoj0 = o.npoj0;

  kVaT = o.kVaT;
  kVmT = o.kVmT;
  kD0 = o.kD0;
  kDr = o.kDr;
  
  for (i=0; i<npoj; i++)
  { n[i] = o.n[i];
    p[i] = o.p[i];
   	for (j=0; j<n[i]; j++) t[i][j] = o.t[i][j];
  }
  
}


int Osobnik::Inicjuj(int np, int nz, int np0) // Initialisation de l'individu
{
  int i, j;
  int ok;
  int tx[NPOJ];
 
  npoj0 = np;

  for (i=0; i<np; i++) tx[i] = FALSE;

  if (np0 > 0) npoj = np0;
  else npoj = LosujN(np) + 1;

  ok = TRUE;
  for (i=0; i<npoj && ok; i++)
  { if (np0 > 0) p[i] = i;
	else p[i] = LosujT(tx, np);

    for (j=i; j<nz && ok; j+=npoj) ok = DopiszZglosz(i, j);
  }
  
	  
  return ok;
}


double Osobnik::DrogaRef(int np, Graf &g, int baza, Zgloszenie *Z, int nz, int *V, Pojazd *pp, int *oz) // Reference de l'itinéaire.
{
  int i;
  int ok;
  int iz;
  double D0 = 0;
  int ip, nzp;

  npoj = np;
  for (i=0; i<npoj; i++) p[i] = i; // indicér les véhicules


  nzp = nz / npoj; // Moyenne des appels par véhicules
  ip = 0; // 1er véhicule

  ok = TRUE;
  for (i=0; i<nz && ok; i++) //pour chaque appel
  { 
    iz = oz[i]; // appel de reprise
	
    ok = DopiszZglosz(ip, iz); // ip :vehicule et iz : appel de reprise (ajouer appel de reprise)
  

	if (ok) // si l'appel a bien été ajouté...
	if (!(i < (ip + 1) * nzp) && ip < npoj - 1 || i == nz - 1) ip ++; 
       // On passe au véhicule suivant si c'est pas ((le dernier vehicule ou le dernier appel) et le premier vehicule a atteint la moy d'appel)
	    
  }
   
  if (!ok) return -_INF; // Au cas où l'ajout n'a pas été fait.

  Ocena(1.0, &D0, g, Z, nz, 1.0, 0, 0, V, baza, pp); // evaluation
// (double Dr, double *D0, Graf &g, Zgloszenie *z, int nz, double vp, double czas_r, double czas_d, int *V, int baza, Pojazd *pp)

  return D0; // On retourne la distance heuristique calculer apres l'evaluation
}
