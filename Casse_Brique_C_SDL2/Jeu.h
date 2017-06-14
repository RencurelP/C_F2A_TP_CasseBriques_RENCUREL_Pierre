#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED
//Fichier .h contenant tout les prototypes de fonctions et procedures ainsi que les types utilises dans le programme
#include "References.h" //Appel du fichier reference

typedef struct game{


     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_ptexture;
     SDL_Surface *g_psurface;


}game;

typedef struct gameState{

    int g_bRunning;
    int left;
    int right;


}gameState;

typedef struct coordonnees{ //Structure gérant la position de la raquette

    double x;
    double y;

}coordonnees;


typedef struct player{  //Structure permettant de gérer les scores des joueurs ainsi que récupérer leur noms pour stocker l'information dans un fichier dans une logique e "High score"

    char Name;
    int score;

}player;//Implementation uniquement si il reste du temps

typedef struct defcircle{   //Structure gérant les coordonnées de la balle ainsi que sa vitesse de déplacement

    float BallX;//oordonnees en X de la balle (bord gauche)
    float BallY;//Coordonnees en Y de la balle (Haut)
    int Launch;//Etat de la balle (verification via un entier)
    int SpeedX;//Vitesse en X de la balle (sera ajouter a sa position pour la deplacer
    int SpeedY;//Vitesse en Y de la balle (sera ajouter a sa position pour la deplacer

}defcircle;


typedef struct Brick{   //Structure gérant les coordonnees des briques et leur état

    int State;  //Etat permettant de détruire la brique
    int BrickX; //Coordonnees X de la brique
    int BrickY; //Coordonnees Y de la brique
    int BrickW; //Largeur de la brique
    int BrickH; //Hauteur de la brique
    int Color; //Couleur de la brique


}Brick;



//prototypes
int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame);  //Initialisation de l'affichage
void destroy(game *myGame); //Destruction de l'instance du jeu
void handleEvents(gameState *state, coordonnees *dep, defcircle *DefCercle); //Gestion des inputs
void delay(unsigned int frameLimit);    //Gestion du framerate

void SetupTerrain(game *myGame, gameState state, coordonnees *dep, defcircle *DefCercle, Brick AfficheBrique[10][10]); //Affichage du jeu
void destroyTexture(game *myGame);//destruction de la texture
void MouvRaquette(gameState *state, coordonnees *dep);//Mouvement de la raquette selon son etat
void MouvBall(defcircle *DefCercle, coordonnees *dep);//Mouvement de la balle et gestion des collisions
void FillTable(Brick *AfficheBrique[10][10]); //Création des briques

#endif
