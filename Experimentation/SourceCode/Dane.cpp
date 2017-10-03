#define NS 50

class Dane { 
                         private:	 Sprzet s[NS]; // liste d'équipements 
				         int ns; // Nombre d'equipements

			 public:         int FCzytaj(char *zadanie); // Lire Fichier
					 double ObliczStrumien(int typs, int typg); // Calculer le flux
					 double StrumienJedn(int ids, int typg); // Un flux
                      /*Nombre d'appels*/void LiczbaZgloszen(int ng, int typg, double *Z, double xdj, double xdw, double xpj, double xpw);
					 void TypSprzetu(int *T, int *D, double *O); // Type d'equipement.
					
                                         double Objetosc(Sprzet *ps) { return (ps->s / 100.0) * (ps->g / 100.0) * (ps->w / 100.0); };// volume
					 double Masa(int i) { return s[i].m; }; // indice de l'equipement.
					 double Masa(Sprzet *ps) { return ps->m; }; // equipement.
					 int Typ(int i) { return s[i].typ; }; // indice de l'equipement. 
					 Dane(void); // Constructeur.
			};


Dane::Dane(void)
{
  ns = 0;
}


void Dane::TypSprzetu(int *T, int *D, double *O) // Determine le type d'equipement
{
   int i;

   for (i=0; i<ns; i++) // Pour chaque equipement
   { T[i] = !(s[i].typ == 2);
     D[i] = (s[i].typ == 0); // grand ou pas
     O[i] = Objetosc(s + i); // volume
   }

}

double Dane::StrumienJedn(int ids, int typg) // ids : indice de l'equipement et typg : type de ménage
{
  Sprzet *ps;
  
  ps = s + ids;

  return Objetosc(ps) * ((typg == GOSPJDN? ps->pj: ps->pw) / 100.0) * ps->zr; // volume * incidence * temps de vie (equipement) 

}

double Dane::ObliczStrumien(int typs, int typg) // typs : type d'equipement et typg : type de menage
{
  int i;
  double str = 0.0;
 
  for (i=0; i<ns; i++)
   if (s[i].typ == typs) str = str + StrumienJedn(i, typg); // Calcule du flux d'un type d'equipement
  

  return str;

}


void Dane::LiczbaZgloszen(int ng, int typg, double *Z, double xdj, double xdw, double xpj, double xpw)
{
  int i;
  Sprzet *ps;
  double xx; // type d'equipement
  double pp;

  for (i=0; i<ns; i++)  // pour chaque équipement
  { ps = s + i; // On parcourt la liste d'equipements

    if (typg == GOSPJDN) // bâtiment de plusieurs étages ou maison individuelle? determine incidence de l'appareil (pj ou pw)
	{ pp = ps->pj;
	  xx = (ps->typ == 0? xdj: xpj);  // equipement grand ou petit	
	}
    else
	{ pp = ps->pw;
	  xx = (ps->typ == 0? xdw: xpw); // equipement grand ou petit
	}
	
    Z[i] += ng * (pp / 100.0) * ps->zr / xx; // Somme pour tous les equipements d'une rue
  }

}

int Dane::FCzytaj(char *zadanie) // Lecture des données 
{
  FILE *f;
  char nazwa[DLGNAZWY];
  int i, ids;
  Sprzet *ps;
  
  strcpy(nazwa, zadanie);
  strcat(nazwa, ".dev");


  f = fopen(nazwa, "rt");
  if (f == NULL) return FALSE;

  fscanf(f, "%d\n", &ns);

  if (ns > NS)
  { fclose(f);
    return FALSE;
  }

  for (i=0; i<ns; i++) 
  { ps = s + i;
    fscanf(f, "%d\t%s\t%lg\t%lg\t%lg\t%lg\t%lg\t%lg\t%lg\t%d\n", &ids, ps->nazwa, &(ps->s), &(ps->g), &(ps->w), &(ps->m), &(ps->pj), &(ps->pw), &(ps->zr), &(ps->typ));
  }

  fclose(f);

  return TRUE;
}

typedef struct {                 char nazwa[50];
				 double s, g, w; // dimension
				 double m; // masse 
				 double pj, pw; // incidence
				 double zr; // temps de vie de l'equipement
            /*type d'equipement*/int typ; } Sprzet; // équipement

