class Gospodarstwo {        private:        int stan[NS]; // liste d'equipement du ménage
				            int krawedz, numer; // indice de l'Arc qui porte la maison et numero de la maison dans la rue 

			    public:         Gospodarstwo(void);
					    void NadajNumer(int k, int n) { krawedz = k; numer = n; }; // Donner votre numéro
					    void PodajNumer(int *k, int *n) { *k = krawedz; *n = numer; }; // Entrer votre numéro
				            int CzyBylo(int s) { return stan[s]; }; // Existe t-il?
					    void Zapisz(int s) { stan[s] = TRUE; }; // Sauvegarder
					};


Gospodarstwo::Gospodarstwo(void)
{
  int i;

  krawedz = 0;
  for (i=0; i<NS; i++) stan[i] = FALSE;

}

