class Zgloszenie { 

                                               private: int gospod;
							int dzien; // Jour
							double godzina; // heure
							int sprzet; // équipement à récupérer
							double objetosc; // Volume
							double masa; // Masse
							int typ; // type

				 
                                          public:
/*------------------------------------------------- Constructeur ----------------------------------------------------------------------------*/
                                                  void Zapisz(int g, int d, double c, int s, double o, double m, int t) { gospod = g; dzien = d; godzina = c; sprzet = s; objetosc = o; masa = m; typ = t;};   
						  void Czytaj(int *g, int *d, double *c, int *s, double *o, double *m, int *t) { *g = gospod; *d = dzien; *c = godzina; *s = sprzet; *o = objetosc; *m = masa; *t = typ; }; 
/*-------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------- Getter(lire) ----------------------------------------------------------------------------*/
  						  int CzytajG(void) { return gospod; };
						  double CzytajT(void) { return godzina; };
						  int CzytajR(void) { return typ; };
						  double CzytajO(void) { return objetosc; };
/*-------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------- Lire Fichier ----------------------------------------------------------------------------*/
						  int FZapisz(FILE *f) { return (fprintf(f, "%d\t%d\t%5.2f\t%d\t%lg\t%lg\t%d\n", gospod, dzien, godzina, sprzet, objetosc, masa, typ) == 7); };
						  int FCzytaj(FILE *f) { return (fscanf(f, "%d\t%d\t%lg\t%d\t%lg\t%lg\t%d\n", &gospod, &dzien, &godzina, &sprzet, &objetosc, &masa, &typ) == 7); };
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
