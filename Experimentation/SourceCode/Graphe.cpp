typedef struct	{ double x, y; } VX;

typedef struct  {                 int a, b; // noeud de depart et d'arrivé
				  double d; // distance de l'arc
				  int ng; // nombre de ménage de la rue
				  int typ; } Krawedz; // type de ménage


class Graf { private:                     Krawedz k[NKR]; // arc qui represente une rue
					  VX v[NWK]; // nombre de noeuds
   					  int nk, nw; // nombre d'arcs

			 public:         int FCzytaj(char *zadanie);
					 int FZapisz(char *zadanie, int *wz, int nz);
					 double ObliczStrumien(Dane &s, int typs);
					 void LiczbaZgloszen(Dane &s, double *Z, double xdj, double xdw, double xpj, double xpw);
					 int Dijkstra(int w0, int w1, int *s);
					 Graf(void); 
					 Gospodarstwo *Numeracja(int *ngosp);
					 void operator = (Graf &a);
					 int DodajWierzcholek(int kr, int num);
					 double Dlugosc(int *s, int n);

			};

Graf::Graf(void) // Constructeur
{ 
  int i;

  nk = 0;
  nw = 0;

  for (i=0; i<NWK; i++)
  { v[i].x = 0;
    v[i].y = 0;
  }
}


double Graf::Dlugosc(int *s, int n) // Longueur d'un arc
{
  double d = 0, di;
  int i, j;
  int a, b, jest;

  for (i=0; i<n-1; i++)
  { a = s[i]; // noeud de départ
    b = s[i + 1]; // noeud d'arrivé

	di = 0;
	jest = FALSE;
	for (j=0; j<nk && !jest; j++)
		if (k[j].a == a && k[j].b == b || k[j].a == b && k[j].b == a) // le noeud de départ et d'arrivé forment ils un arc ? 
		{ jest = TRUE;
		  di = k[j].d;
		}

    d += di;
  }

  return d;

}

Gospodarstwo *Graf::Numeracja(int *ngosp) // Numerotage et Parametre : nombre de ménage
{
  Gospodarstwo *g;
  int i, j, n;


  *ngosp = 0;
  for (i=0; i<nk; i++) (*ngosp) += k[i].ng; // ngosp = somme des ménages de tous les arcs du graphe  

  g = new Gospodarstwo[*ngosp]; // tableau de ménage

  if (g == NULL) return NULL;

  n = 0;
  for (i=0; i<nk; i++) // pour chaque arc
	  for (j=0; j<k[i].ng; j++) // pour chaque ménage 
	  { g[n].NadajNumer(i, j); // i : arc , j : numero du menage
	    n ++;
	  }
	    

  return g;
}

void Graf::operator = (Graf &a) // Comparaison entre deux graphes
{
  int i;
	
  nk = a.nk;
  nw = a.nw;

  for (i=0; i<nk; i++) memmove(k + i, a.k + i, sizeof(Krawedz)); // meme nombre d'arc
  for (i=0; i<nw; i++) memmove(v + i, a.v + i, sizeof(VX)); // meme nombre de noeuds
}


double Graf::ObliczStrumien(Dane &s, int typs) // calculer le flux
{
  double str = 0.0;
  int i;

  for (i=0; i<nk; i++) str = str + k[i].ng * s.ObliczStrumien(typs, k[i].typ); // Pour chaque arc du graphe

  return str;
}

void Graf::LiczbaZgloszen(Dane &s, double *Z, double xdj, double xdw, double xpj, double xpw) // Nombre de demandes
{

  int i;
  

  for (i=0; i<nk; i++) s.LiczbaZgloszen(k[i].ng, k[i].typ, Z, xdj, xdw, xpj, xpw); // Pour chaque arc (rue) du graphe

}


int Graf::FCzytaj(char *zadanie) // Lecture des données
{
  FILE *f;
  char nazwa[DLGNAZWY];
  int i, x, nw0;
  Krawedz *pk;

  strcpy(nazwa, zadanie);
  strcat(nazwa, ".grf");


  f = fopen(nazwa, "rt");
  if (f == NULL) return FALSE;

  fscanf(f, "%d\n", &nk);

  if (nk > NKR)
  { fclose(f);
    return FALSE;
  }

  for (i=0; i<nk; i++) // Pour chaque Arc
  { pk = k + i;
    fscanf(f, "%d\t%d\t%d\t%lg\t%d\t%d\n", &x, &(pk->a), &(pk->b), &(pk->d), &(pk->ng), &(pk->typ));

	if (pk->a > nw) nw = pk->a; // dernier noeud
        if (pk->b > nw) nw = pk->b; // dernier noeud
  }

  nw ++; // On incremente le nombre de noeud

  fclose(f);

  strcpy(nazwa, zadanie);
  strcat(nazwa, ".grv");

  f = fopen(nazwa, "rt");
  if (!(f == NULL))
  { fscanf(f, "%d\n", &nw0);

    for (i=0; i<nw0; i++) fscanf(f, "%d\t%lg\t%lg\n", &x, &(v[i].x), &(v[i].y)); // Pour chaque noeud

    fclose(f);

	if (nw0 > nw) nw = nw0;
  }


  return TRUE;
}


int Graf::FZapisz(char *zadanie, int *wz, int nz) // Sauvegarder
{
  FILE *f;
  char nazwa[DLGNAZWY];
  int i;
  Krawedz *pk;

 
  strcpy(nazwa, zadanie);
  strcat(nazwa, "~.grf");


  f = fopen(nazwa, "wt");
  if (f == NULL) return FALSE;

  fprintf(f, "%d\n", nk);



  for (i=0; i<nk; i++) 
  { pk = k + i;
    fprintf(f, "%d\t%d\t%d\t%lg\t%d\t%d\n", i, pk->a, pk->b, pk->d, pk->ng, pk->typ);

  }

  fclose(f);


  strcpy(nazwa, zadanie);
  strcat(nazwa, "~.grv");

  f = fopen(nazwa, "wt");
  if (f == NULL) return FALSE;

  fprintf(f, "%d\n", nw);

  for (i=0; i<nw; i++) fprintf(f, "%d\t%lg\t%lg\n", i, v[i].x, v[i].y);

  fclose(f);

  return TRUE;
}


int Graf::DodajWierzcholek(int kr, int num) // Ajouter un ménage a une rue. kr : indice de la rue et num : numero du menage dans la rue
{
   Krawedz *pk0 = k + kr, *pk1; // pko : arc sur le quel on veut ajouter la maison et pk1 : arc créé apres l'ajout de la maison
   int w1; // noeud ajouté


   if (!(nk < NKR)) return -1;
   if (!(nw < NWK)) return -1;

   nk ++; // On incremente le nombre de rue
   nw ++; //De meme

   w1 = nw - 1; // Initialisation

   v[w1].x = (v[pk0->a].x + v[pk0->b].x) / 2;
   v[w1].y = (v[pk0->a].y + v[pk0->b].y) / 2;
 
   pk1 = k + nk - 1; // initialisation

   
   //Nouvelle arc apres l'ajout
   pk1->b = pk0->b; // la derniere maison de la rue pko devient la derniere maison de la nouvelle rue pk1
   pk1->a = w1; // la premiere maison de la rue pk1 est la maison ajouté 
   pk1->ng = pk0->ng - num; // nbre de maison de la nvelle rue = nbre de maison de la rue pko - la position de la maison dans la rue pko
   pk1->d = ((pk1->ng + 1) * pk0->d) / (pk0->ng + 1); // Dpk1 = (NbreMaisonPk1 / NbreMaisonPk0) * Dpk0
   pk1->typ = pk0->typ; // meme type de rue

   // Arc sur le quel l'on ajout la maison
   pk0->b = w1; // la derniere maison de la rue pk0 devient la nouvelle maison ajoutée
   pk0->ng = num; // le nombre de maison egale le numero de la maison ajouté
   pk0->d = pk0->d - pk1->d; // Dpk0 = Dpk0 - Dpk1

   return w1;
 }

int Graf::Dijkstra(int w0, int w1, int *s) // Algorithme de Dijkstra (wo : noeud de depart, w1 : noeud final)
{
  double d[NWK]; // Liste des couts des noeuds
  int p[NWK]; // liste des precursseurs des noeuds
  int ok, stop;
  int x, y, i;
  double dxy;
  H t(nw);
  int n;
   
  // Initialisation
  for (i=0; i<nw; i++) 
  { if (i == w0) d[i] = 0; // distance du noeud de depart = 0
	else d[i] = _INF; // distance des autres noeuds = + INF

	t.Zapisz(i); // On sauvegarde tous les noeuds dans t
  }

  stop = FALSE;
  while (!stop)
  { x = t.Wybierz(d);
	  
    if (x < 0) // Si l'on ne trouve aucun noeud on arrete la boucle
	{ stop = TRUE;
	  ok = FALSE;
	}
    else // Si l'on a atteint le dernier noeud on arrete la boucle
      if (x == w1)
	{ stop = TRUE;
	  ok = TRUE;
	}
    else	
	for (i=0; i<nk; i++) // On parcourt tous les arcs
	{ dxy = 0;
	  if (x == k[i].a) // Si x est le noeud de depart de l'arc k[i] (grace x on peut determiner l'arc où il se trouve et...)
	  { y = k[i].b;
	    dxy = k[i].d; // dxy : distance l'arc
	  }
	  else
	  if (x == k[i].b) // Si y est le noeud de depart de l'arc k[i] (...s'il est le 1er ou le dernier noeud)
	  { y = k[i].a;
	    dxy = k[i].d; // dxy : distance l'arc
	  }
		   
	  if (dxy > 0)
	   if (d[x] + dxy < d[y]) 
	   { d[y] = d[x] + dxy; // Distance du noeud Y = Distance du noeud X + Distance de l'arc (XY)
		 p[y] = x; // le noeud X est le precursseur du noeud Y de l'arc (XY).
	   }
	}
  }

  if (ok) // On a parcourue tous les noeuds
  { n = 1;
    x = w1; // On initialise x au dernier noeud

	while (!(x == w0)) // Determine le nombre de noeuds qui constitue le plus court chemin.
	{ n++;
	  x = p[x];
	}
    
	i = n - 1; // i : nombre de noeuds
        x = w1; // On reinitialise x au dernier noeud

	while (!(x == w0)) // Tant que x n'est pas egale a w0
	{ s[i] = x;
 
	  i--;
	  x = p[x];
	}
   
	s[0] = w0; // s[n] = {wo,...,w1} liste de tous les noeuds parcourue et constituant le plus court chemin

	return n; // nombre de noeuds qui constitue le plus court chemin
  }
  else return -1;

}

