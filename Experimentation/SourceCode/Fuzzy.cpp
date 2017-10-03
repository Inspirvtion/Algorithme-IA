#define NKRYT 5

class Fuzzy { private: double tk[NKRYT][4]; // 
                       double tx[NKRYT]; // Tableau des degrés d'appartenance des prémisses de chaque règle.

	      public:  double FF(double x1, double x2, double x3, double x4, double x);
		       void FN(int duza, int k); // duza = grand
		       void Kryterium(double x, int duza, int k) { tx[k] = x; FN(duza, k); }; //Dégré d'appartenance et fonction d'appartenance
		       double FC(int nk);   
			};

void Fuzzy::FN(int duza, int k) // Fonction Evaluation (Sortie) : Création de la fonction d'appartenance de la sortie.
{ 
  if (duza) // Quand la fonction d'evaluation est "High" (Right)
  { tk[k][0] = 0.3;
    tk[k][1] = 0.9;
    tk[k][2] = _INF;
    tk[k][3] = _INF;
  }
  else // Quand la fonction d'evaluation est "Low" (Left)
  { tk[k][0] = 0.0;
    tk[k][1] = 0.0;
    tk[k][2] = 0.2;
    tk[k][3] = 0.7;
  }
}

double Fuzzy::FF(double x1, double x2, double x3, double x4, double x) // Calcul du dégrée d'appartenance.
{
  if (x >= x2 && x <= x3) return 1.0;
  else
  if (x >= x1 && x < x2) // RighFuzzy
	  if (x2 > x1) return (x - x1) / (x2 - x1);
	  else return 0.0;
  else 
  if (x > x3 && x <= x4) // LeftFuzzy
	  if (x4 > x3) return (x4 - x) / (x4 - x3);
	  else return 0.0;
  else return 0.0;
}

double Fuzzy::FC(int nk)
{
  int i, j, nn;
  double dx, xsr, x, p0;
  double w, max;

  nn = 100;
  dx = 1.0 / nn; // dx = 0.01
	
  xsr = 0;
  p0 = 0;

  for (i=0; i<nn; i++)
  { x = i * dx;   // x = [0 : 0.99] 100 valeurs = 0.00, 0.01, 0.02...0.99
    max = 0;
 
	for (j=0; j<nk; j++) // Fuzzification
	{ w = FF(tk[j][0], tk[j][1], tk[j][2], tk[j][3], x); // Pour chaque regle on calcul le degré d'appartenance de la sortie.

	  if (w > tx[j]) w = tx[j];
          if (w > max) max = w; // On recupere le dégré le plus grand des 3 règles.
        } 
  
    xsr = xsr + max * x;
    p0 = p0 + max;
  }


  if (p0 > 0) return xsr / p0; // Defuzzication (Moyenne)
  else return 0;
}

