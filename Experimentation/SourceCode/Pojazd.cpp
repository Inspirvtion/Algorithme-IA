class Pojazd { 
                                   public: double w, s, d; // Longueur, Largeur, Hauteur.
					   double koszt; // Cout du véhicule.
					   double poj; // Volume total du conteneur.
					   double lad; // Volume utilisé.


					   int FCzytaj(FILE *f);
					   Pojazd(void) { lad = 0; };
					   int Zaladuj(double p); 
					   void Oproznij(void); 
};


int Pojazd::Zaladuj(double p) // Ajouter une charge au conteneur.
{
  if (lad + p > poj) return FALSE;
  else
  { lad += p;
    return TRUE;
  }
}

void Pojazd::Oproznij(void) // Vider le conteneur.
{

  lad = 0;

}

int Pojazd::FCzytaj(FILE *f) // Lire le fichier en vue de calculer le volume du conteneur.
{
  int ok;

  ok = (fscanf(f, "%lg\t%lg\t%lg\t%lg\n", &w, &s, &d, &koszt) == 4);

  if (ok) poj = w * s * d;
  else poj = 0;

  return ok;

}
