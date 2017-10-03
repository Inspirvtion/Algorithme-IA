#define NKR 1000
#define NWK 1000

class H { private: int ne; 
          public: int t[NWK];
					
				  H(int n) { ne = 0; };
				  ~H(void) { ne = 0; };
				  int Wybierz(double *d);
				  void Zapisz(int a);
				
			};

int H::Wybierz(double *d) // Selectionner
{
   int i, k, w;
   double mx;

   if (!(ne > 0)) return -1;

   for (i=0; i<ne; i++)
	   if (i == 0) 
	   { mx = d[t[i]];
             k = i;
	   }
	   else
           if (d[t[i]] < mx)
	   { mx = d[t[i]];
	     k = i;
	   }
   
    w = t[k]; // noeud selectionné
   
    // On supprime le noeud selectionné de la liste des noeuds
    t[k] = t[ne - 1];
    ne --;
    
	if (mx < _INF) return w;
	else return -1;

}

void H::Zapisz(int a) // Sauvegarder
{
  t[ne] = a;

  ne++;
}
