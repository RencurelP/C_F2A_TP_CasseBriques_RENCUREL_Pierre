#include "Jeu.h" //Inclusion du fichier contenant les structures et prototypes

int main(int argc, char *argv[])    //Fonction main
{


     game myGame;   //Déclarations des variables
     gameState state;

     unsigned int frameLimit = SDL_GetTicks() + 16; //Affectation du framerate
     coordonnees dep;
     dep.x=((SCREEN_WIDTH/2)-64);//Mise en place de la coordonnee de depart de la raquette (axe X)
     dep.y = (SCREEN_HEIGHT - 32);//Mise en place de la coordonnee de depart de la raquette (axe Y)


     defcircle DefCercle;
     DefCercle.SpeedX=0;//Balle immobile
     DefCercle.SpeedY=0;//Balle immobile
     DefCercle.Launch=0;//Etat de la balle "en attente"
     DefCercle.BallX=(dep.x+52);//Maintient de la balle au centre de la raquette
     DefCercle.BallY=(SCREEN_HEIGHT-56);//Maintient de la balle au dessus de la raquette


     Brick AfficheBrique [10] [10]; //Tableau de briques contenant toutes les informations relatives aux briques





     int choix=0;   //Variable permettant le choix


     state.g_bRunning=1;




        //Create texture for drawing in texture or load picture
        myGame.g_ptexture=SDL_CreateTexture(myGame.g_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH,SCREEN_HEIGHT);




        while(state.g_bRunning){



                if(choix==0){   //Lacement du jeu après consultation du joueur

                    printf("1:Lancer le Casse Brique C facile\n");
                    printf("2:Quitter l'application\n");

                    scanf("%i",&choix); //Saisie du choix par le joueur


                }
                switch (choix){//Choix permettant de soit lancé soit fermer le programme
                        //Lancement du jeu lorsque l'utilisateur presse 1
                        case 1:init("Chapter 1 setting up SDL",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame);

                                while(state.g_bRunning){    //Appel des procedures et fonctions

                                    //MouvRaquette(&state, &dep);
                                    handleEvents(&state, &dep, &DefCercle);    //Gestion des inputs
                                    FillTable(&AfficheBrique);
                                    printf("%d\n",AfficheBrique[10][10].State);
                                    printf("%d\n",AfficheBrique[10][10].BrickH);
                                    printf("%d\n",AfficheBrique[10][10].BrickW);
                                    printf("%d\n",AfficheBrique[10][10].BrickX);
                                    printf("%d\n",AfficheBrique[10][10].BrickY);
                                    printf("%d\n",AfficheBrique[10][10].Color);
                                    MouvRaquette(&state, &dep); //Mouvement de la raqutte
                                    MouvBall(&DefCercle, &dep); //Mouvement de la balle
                                    SetupTerrain(&myGame, state, &dep, &DefCercle, AfficheBrique); //Tracé des raquettes et de la balle
                                }

                                 break;

                        case 2:EXIT_SUCCESS;break;  //Sortie du programme sur succès de la commande


                        default:break;


                }

                 system ("cls");


             // Gestion des 60 fps (1000ms/60 = 16.6 -> 16
            delay(frameLimit);  //Limitation du framerate
            frameLimit = SDL_GetTicks() + 16;



        }

        destroy(&myGame);   //Destruction de l'affichage

        SDL_Quit(); //Fin de l'utilisation de la SDL


    return 0;
}

