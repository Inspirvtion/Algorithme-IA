class Test { private: Graf g;
					  Dane d;
					  char zadanie[DLGNAZWY];
					  double sdj, sdw, spj, spw;
					  int godz_od, godz_do;
					  int ile_dni;
					  double xdj, xdw, xpj, xpw;
					  double efc;
					  double Z[NS], O[NS];
					  int T[NS], D[NS];
					  Gospodarstwo *gosp;
					  int ngosp;
					  Zgloszenie *zgl;
					  int nzgl;
					  double xs;

					  public: Test(char *nazwa);
							  ~Test(void);
							  int CzytajDane(void);
							  int Czytaj(void);
							  double ObliczStrumien(int typs);
							  void Kalibracja(void);
							  void LiczbaZgloszen(void);
							  int Numeracja(void);
							  void GenerujZgloszenia(void);
							  void NoweZgloszenie(int idg, int ids, double odj, double masa, int typ);
							  int ZapiszZgloszenia(void);
							  
			};

Test::Test(char *nazwa)
{
	strcpy(zadanie, nazwa);
	sdj = 0.0;
	sdw = 0.0;
	spj = 0.0;
	spw = 0.0;

	xdj = 1.0;
	xdw = 1.0;
	xpj = 1.0;
	xpw = 1.0;

	int i;

	for (i=0; i<NS; i++) 
	{ Z[i] = 0;
	  T[i] = FALSE;
	}

	gosp = NULL;
	ngosp = 0;

    zgl = NULL;
	nzgl = 0;

	srand((unsigned)time(NULL));

	xs = 0;
}




Test::~Test(void)
{
  delete []gosp;
  gosp = NULL;
  ngosp = 0;

  delete []zgl;
  zgl = NULL;
  nzgl = 0;


}

int Test::ZapiszZgloszenia(void)
{
  FILE *f;
  int i;
  char nazwa[DLGNAZWY];

 

  strcpy(nazwa, zadanie);
  strcat(nazwa, ".zgl");


  f = fopen(nazwa, "wt");
  if (f == NULL) return FALSE;

  for (i=0; i<nzgl; i++) zgl[i].FZapisz(f);

  fclose(f);


  return TRUE;




}

int Test::Numeracja(void)
{
   gosp = g.Numeracja(&ngosp);

   return (gosp == NULL? FALSE: TRUE);
}



void Test::GenerujZgloszenia(void)
{

  int i, j, ns, idg;
  int gotowe;
  int licz;

  for (i=0; i<NS; i++)
	  if (T[i])
	  { ns = (int)(Z[i] * efc + 0.5);
	    
	    for (j=0; j<ns; j++)
		{ gotowe = FALSE;
		  licz = 0;

		  while (!gotowe)
		  { idg = LosujN(ngosp);
		    licz ++;

		    if (!gosp[idg].CzyBylo(i) || !(licz < ngosp))
			{ gotowe = TRUE;
			  gosp[idg].Zapisz(i);

			  NoweZgloszenie(idg, i, O[i], d.Masa(i), d.Typ(i));

			  if (D[i]) xs += O[i];
			}
		  }
		}
	  }

   xs = xs / ngosp;

}


void Test::NoweZgloszenie(int idg, int ids, double obj, double masa, int typ)
{
  
  Zgloszenie *z;
  double czas;

  z = new Zgloszenie[nzgl + 1];

  memmove(z, zgl, nzgl * sizeof(Zgloszenie));

  czas = 1.0 * (godz_od + LosujN(godz_do - godz_od)) + LosujN(4) * 0.15;
  z[nzgl].Zapisz(idg, LosujN(ile_dni) + 1, czas, ids, obj, masa, typ);

  nzgl ++;

  delete []zgl;

  zgl = z;
}

void Test::LiczbaZgloszen(void)
{

  g.LiczbaZgloszen(d, Z, xdj, xdw, xpj, xpw);

}

void Test::Kalibracja(void)
{
   double str;

   str = d.ObliczStrumien(0, GOSPJDN);
   xdj = str / sdj;

   str = d.ObliczStrumien(0, GOSPWLR);
   xdw = str / sdw;

   str = d.ObliczStrumien(1, GOSPJDN) + d.ObliczStrumien(2, GOSPJDN);
   xpj = str / spj;

   str = d.ObliczStrumien(1, GOSPWLR) + d.ObliczStrumien(2, GOSPWLR);
   xpw = str / spw;

}

double Test::ObliczStrumien(int typs)
{

  return g.ObliczStrumien(d, typs);

}

int Test::CzytajDane(void)
{
  if (!d.FCzytaj(zadanie)) return FALSE;

  if (!g.FCzytaj(zadanie)) return FALSE;

  if (!Czytaj()) return FALSE;

  d.TypSprzetu(T, D, O);

  return TRUE;
}

int Test::Czytaj(void)
{
  FILE *f;
  char nazwa[DLGNAZWY];
 

  strcpy(nazwa, zadanie);
  strcat(nazwa, ".dat");


  f = fopen(nazwa, "rt");
  if (f == NULL) return FALSE;


  fscanf(f, "%lg\t%lg\n", &sdj, &sdw);
  fscanf(f, "%lg\t%lg\n", &spj, &spw);

  fscanf(f, "%lg\n", &efc);
  fscanf(f, "%d\t%d\n", &godz_od, &godz_do);
  fscanf(f, "%d\n", &ile_dni);

  fclose(f);

  return TRUE;
}

