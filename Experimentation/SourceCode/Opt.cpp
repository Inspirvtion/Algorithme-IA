class Opt {                     
/*------------------------------------------------- Attributs -------------------------------------------------------------------------------*/
                                private: Graf g, gx;
					 char zadanie[DLGNAZWY]; // Tache
					 Gospodarstwo *gosp; // Reference sur un noeud
					 int ngosp; // Nombre de noeud (Pas sur)
					 Zgloszenie zgl[NZGL]; // Tableau d'appel de reprise
					 int WZ[NZGL]; // id sommet pour la notification (Appel de reprise)
					 int nzgl; // Nombre de reprise
					 int baza; // noeud de base
					 int npok, nosob, elita, npoj; //N. de test, Nombre d'individus, Nombre d'Elites, Nombre de Véhicules
					 Pojazd p[NPOJ]; // tableau de Véhicule
					 double vpoj; // vitesse du véhicule
					 double pm, px; // Probabilité mutation, Probabilité crossover
					 Osobnik *pok, *pot; // Reference sur un undividu (Solution)
					 double czas_r, czas_d; // Temps de chargement et dechargement
					 int nmut; // Nombre de mutation initial
					 int nopt; // Nombre d'optimisation (generation)
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ Methodes ---------------------------------------------------------------------------------*/
			 public: Opt(char *nazwa);
							  ~Opt(void);
							  int CzytajDane(void); // Lire les Données
							  int Czytaj(void); // Lire
							  int CzytajZgloszenia(void); // Lire les appels de reprise
							  int ModyfikujGraf(void); // Modifier le Graf
							  int Optymalizacja(FILE *fx, int mb, int no); // Optimisation
							  int Optymalizacja(void); // Optimisation
							  void SortujZgloszenia(int *z); // Trier les appels de reprise
							  int LiczbaPoj0(void); // Capacité (Nombre)
							  double DrogaRef(void); // Réference de la route
							  void Czekanie(int no, int k); // Attente
							  int Parametry(void); // Parametre
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
			};


Opt::Opt(char *nazwa) // Constructeur
{
	strcpy(zadanie, nazwa);
	
	gosp = NULL;
	ngosp = 0;

	pot = NULL;
	pok = NULL;
	npok = 0;
	nosob = 0;
	elita = 0;
	nmut = 0;

	nzgl = 0;
	nopt = 1;

	srand((unsigned)time(NULL));

}



Opt::~Opt(void) // Destructeur
{
  delete []gosp;
  gosp = NULL;
  ngosp = 0;

  nzgl = 0;

  delete []pok;
  pok = NULL;

  delete []pot;
  pot = NULL;

  nosob = 0;

}


int Opt::LiczbaPoj0(void) // Calcul de la capacité.
{
  double obj = 0;
  int i;
  int np;
  double poj = 0;

  for (i=0; i<nzgl; i++) obj += zgl[i].CzytajO(); // Somme des volumes des équipements à ramasser.

  for (i=0; i<npoj; i++) poj += p[i].poj; // Somme volumique des conteneurs des véhicules.

  poj = poj / npoj; // Moyenne volumique des conteneurs des véhicules.

  np = (int)(obj / poj);


  if (np > npoj) np = npoj;
  else
  if (!(np > 0)) np = 1;

  return np; // retourne la capacité.
}


int Opt::Parametry(void) // Configurer les paramètres.
{
  int c;

  printf("Zadanie: %s, L. przeb: %d\n", zadanie, nopt);
  
  printf("L. pok. %d, L. osob %d, Elita %d\n", npok, nosob, elita);
 

  printf("L. poj. %d, Pr. poj. %lg\n", npoj, vpoj);

  printf("P. mut. %lg, P. krzyz. %lg\n", pm, px);

  printf("L. mut. pocz. %d\n", nmut);

  printf("Cz. roz. %lg, Cz. za³. %lg\n", czas_r, czas_d);

  printf("Start y/n?"); // Lancez l'optimisation ?

  do
  { c = _getch();
    c = toupper(c);
  } while (!(c == 'Y' || c == 'N'));

  return c == 'Y';
}


double Opt::DrogaRef(void)
{
  int z[NZGL]; // Tableau d'appel de reprise
  Osobnik o; // Individu (Solution)


  SortujZgloszenia(z);

  return o.DrogaRef(npoj, gx, baza, zgl, nzgl, WZ, p, z);


}

void Opt::SortujZgloszenia(int *z)
{
  int i, j, t;
  int mxg, imxg;
  int ig;

  for (i=0; i<nzgl; i++) z[i] = i; // Initialisition --> 1er appel = 0, 2eme appel = 1...

  for (i=0; i<nzgl; i++)
  { mxg = -1;
    imxg = -1;

    for (j=i; j<nzgl; j++)
	{ ig = zgl[z[j]].CzytajG();
	  if (ig > mxg)
	  { mxg = ig;
	    imxg = j;
	  }
	}

    t = z[i];
	z[i] = z[imxg];
	z[imxg] = t;
  }
}

int Opt::Optymalizacja(void)
{
  FILE *f;
  char nazwa[DLGNAZWY]; 
  int i, ok;
  
  strcpy(nazwa, zadanie);
  strcat(nazwa, ".txt");
  
  f = fopen(nazwa, "wt");
  if (f == NULL) return FALSE;
  

  ok = TRUE;
  for (i=0; i<nopt && ok; i++) if (!Optymalizacja(f, i == 0, i)) ok = FALSE; // Appel de la fonction ci-dessous.

  fclose(f);

  return ok;

}  


int Opt::Optymalizacja(FILE *fx, int mb, int no) // Fonction d'optimasation d'une génération
{
  Osobnik oi, oj, o0;
  int k, i, j;
  double *ocena, ocena0;
  int *e;
  double d0, dr;
  FILE *f;
  char nazwa[DLGNAZWY]; 
	
  if (mb)
   if (!Parametry()) return FALSE;

  pok = new Osobnik[nosob];
  pot = new Osobnik[nosob];

  if (pok == NULL || pot == NULL) return FALSE;

  ocena = new double[nosob];
  if (ocena == NULL) return FALSE;

  e = new int[nosob];
  if (e == NULL) 
  { delete []ocena;
    return FALSE;
  }

  sprintf(nazwa, "%s~%d", zadanie, no);
  strcat(nazwa, ".out");


  f = fopen(nazwa, "wt");
  if (f == NULL) 
  { delete []e;
    delete []ocena;
	return FALSE;
  }
 
  dr = DrogaRef();
  d0 = 0;
  
  if (!o0.Inicjuj(npoj, nzgl, LiczbaPoj0())) return FALSE;
  ocena0 = o0.Ocena(dr, &d0, gx, zgl, nzgl, vpoj, czas_r, czas_d, WZ, baza, p);
  
  o0.Wynik(ocena0, d0, fx, FALSE);

  fprintf(f, "%lg\t%lg\n", d0, dr);

  for (i=0; i<nosob; i++) 
  { if (!(i > elita)) pok[i] = o0;
    else 
	{ pok[i].Inicjuj(npoj, nzgl, 0);
	  for (j=0; j<nmut; j++) 
	     if (!pok[i].Mutacja()) pok[i] = o0;
	}
        
  }

  for (k=0; k<npok; k++)
  { Czekanie(no, k);

	for (i=0; i<nosob; i++) 
     if (Losuj(pm)) 
	 { oi = pok[i];
	   if (!pok[i].Mutacja()) pok[i] = oi;
	 }

    for (i=0; i<nosob / 2; i++) 
     if (Losuj(px))
	 { j = LosujN(nosob);
	   if (!(i == j)) 
	   { oi = pok[i];
	     oj = pok[j];

		 if (!pok[i].Cross(pok[j]))
		 { pok[i] = oi;
		   pok[j] = oj;
		 }
	   }
	 }

	for (i=0; i<nosob; i++) ocena[i] = pok[i].Ocena(dr, &d0, gx, zgl, nzgl, vpoj, czas_r, czas_d, WZ, baza, p);

    Najlepsi(e, Max(elita, 1), ocena, nosob);



	if (ocena[e[0]] > ocena0)
	{ o0 = pok[e[0]];
	  ocena0 = ocena[e[0]];
	}

	for (i=0; i<elita; i++) pot[i] = pok[e[i]];

	for (i=elita; i<nosob; i++) pot[i] = pok[Ruletka(ocena, nosob)];

	for (i=0; i<nosob; i++) pok[i] = pot[i];

	fprintf(f, "%d\t%10lg", k, ocena0);
    o0.Zapisz(f);
	fprintf(f, "\n");
  }


  fclose(f);

  o0.Wynik(ocena0, d0, fx, TRUE);

  o0.ZapiszSzciezke(zadanie, nzgl, gx, baza, zgl, WZ, p);


  delete []e;
  delete []ocena;

  return TRUE;

}
/*---------------------------------------  Lire toutes les données nécéssaire à l'optimisation ----------------------------------------------*/

int Opt::CzytajDane(void) // Executer les 3 fonctions et s'assurer que toutes les 3 ont été bien éxécutées.
{

  if (!g.FCzytaj(zadanie)) return FALSE;

  if (!CzytajZgloszenia()) return FALSE;

  if (Czytaj()) return FALSE;


  return TRUE;

}


int Opt::CzytajZgloszenia(void)
{
  FILE *f;
  char nazwa[DLGNAZWY];
  int stop, ok;
 

  strcpy(nazwa, zadanie);
  strcat(nazwa, ".zdz");


  f = fopen(nazwa, "rt");
  if (f == NULL) return FALSE;

  nzgl = 0;
  stop = FALSE;
  ok = TRUE;
  while (!stop && ok)
    if (zgl[nzgl].FCzytaj(f)) nzgl ++; // On recupere les parametres de chaque appel de reprise.
	else stop = TRUE;

  fclose(f);

  return ok;
}


void Opt::Czekanie(int no, int k) // Affichage (run : , step : ).
{

  printf("run %d, step %d\n", no, k);
}


int Opt::Czytaj(void)
{
  FILE *f;
  char nazwa[DLGNAZWY];
  int i;
  int ok;
 

  strcpy(nazwa, zadanie);
  strcat(nazwa, ".par");


  f = fopen(nazwa, "rt");
  if (f == NULL) return FALSE;

  ok = TRUE;

  fscanf(f, "%d\n", &nopt);
  fscanf(f, "%d\t%d\t%d\n", &npok, &nosob, &elita);
  fscanf(f, "%d\t%lg\t%d\n", &npoj, &vpoj, &baza);
  fscanf(f, "%lg\t%lg\n", &pm, &px);
  fscanf(f, "%d\n", &nmut);
  fscanf(f, "%lg\t%lg\n", &czas_r, &czas_d);



  if (npoj > NPOJ) ok = FALSE;

  for (i=0; i<npoj && ok; i++) if (!p[i].FCzytaj(f)) ok = FALSE;


  fclose(f);

  return ok;


}
/*-------------------------------------------------------------------------------------------------------------------------------------------*/


int Opt::ModyfikujGraf(void)
{
  int i;
  int z[NZGL];
  int kr, num;
  int w, ok;
  int ig;
 

  gx = g;

  gosp = g.Numeracja(&ngosp);
  if (gosp == NULL) return FALSE;

  for (i=0; i<nzgl; i++) z[i] = i;
 
  SortujZgloszenia(z);

   
  ok = TRUE;
  for (i=0; i<nzgl && ok; i++)
  { ig = zgl[z[i]].CzytajG();
    gosp[ig].PodajNumer(&kr, &num);

	w = gx.DodajWierzcholek(kr, num);

	if (w < 0) ok = FALSE;
	else WZ[z[i]] = w;
	 
  }

  if (ok) ok = gx.FZapisz(zadanie, WZ, nzgl);

  return ok;
}

