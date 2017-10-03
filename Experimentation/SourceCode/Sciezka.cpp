#define NWKS (NWK * 10)

typedef struct { int w, z; } wezel; // z : la position de l'appel dans l'itinéraire , w : position reel du noeud dans l'itinéraire
// Itinéraire
class Sciezka {                     public: wezel s[NWKS]; // liste de noeud (chaque noeud posséde un temps de ramassage et un cout)
					    int ns; // indice du noeud

					    void Zeruj(void) { ns = 0; }; // Initialisation
					    int DopiszWezel(int w, int z) { s[ns].w = w; s[ns].z = z; ns ++; return ns; }; // Ajouter noeud
/* w pour calculer les distance et delais et z pour les informations relatives  l'appel */
};
