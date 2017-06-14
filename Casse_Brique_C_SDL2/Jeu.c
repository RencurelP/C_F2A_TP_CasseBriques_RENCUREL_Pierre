//Fichier contenant toutes les procedures et fonctions completes du programme


#include "Jeu.h" //Inclusion du fichier contenant les structures et prototypes

int init(char *title, int xpos,int ypos,int height, int width,int flags,game *myGame){


    //initialize SDL

    if(SDL_Init(SDL_INIT_EVERYTHING)>=0)
    {
            //if succeeded create our window
            myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,height,width,flags);
            //if succeeded create window, create our render
            if(myGame->g_pWindow!=NULL){
                myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_ACCELERATED);
                SDL_SetRenderDrawColor(myGame->g_pRenderer, 0, 0, 0, 255);
                SDL_RenderClear(myGame->g_pRenderer);

            }


    }else{


        return 0;
    }




    return 1;


}


void delay(unsigned int frameLimit)
{
    // Gestion des 60 fps (images/seconde)
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}

void handleEvents(gameState *state, coordonnees *dep, defcircle *DefCercle){  //Gestion des inputs


    SDL_Event event;

    if(SDL_PollEvent(&event)){


            switch(event.type){
            case SDL_QUIT:
                  state->g_bRunning=0;break;
            case SDL_KEYDOWN://Verification de pression sur une touche
                            switch (event.key.keysym.sym)
                                {
                                    case SDLK_LEFT:state->left=1;state->right=0; break;//Lorsque la touche "fleche gauche" est pressee l'etat de la raquette passe a "gauche"
                                    case SDLK_RIGHT:state->right=1;state->left=0 ; break;//Lorsque la touche "fleche droite" est pressee l'etat de la raquette passe a "droite"
                                    case SDLK_UP:    ; break;
                                    case SDLK_DOWN:  ; break;


                                    case SDLK_SPACE:    //Lorsque la touche "espace" est pressee l'etat de la balle passe a "lancee
                                        if(DefCercle->Launch == 0){//Verification que l'etat ne soit pas deja setter a lance pour eviter les anomalies
                                                DefCercle->Launch = 1;//Set de l'etat
                                                DefCercle->SpeedX=0;//Maintient de la vitesse en X a 0
                                                DefCercle->SpeedY=-1;//Set de la vitesse en Y a -1 pour que la balle monte
                                                //La balle va montee a la vertical
                                        }break;
                                }
                                break;

            case SDL_KEYUP://Verification qu'une touche a ete relachee
                            switch (event.key.keysym.sym)
                                {
                                    case SDLK_LEFT:state->left=0;state->right=0; break;//Lorsque la touche "fleche gauche" est lachee l'etat de la raquette passe a "0"
                                    case SDLK_RIGHT:state->right=0;state->left=0 ; break;//Lorsque la touche "fleche droite" est lachee l'etat de la raquette passe a "0"
                                    case SDLK_UP:    ; break;
                                    case SDLK_DOWN:  ; break;

                                    //La touche "espace" n'a pas de statut "relachee" pour ne pas reset la valeur d'etat de la balle afin d'eviter les anomalies
                                }
                                break;

            default:break;

        }
    }


}

void FillTable(Brick *AfficheBrique[10][10]){ //Fonction remplissant un tableau de brique afin de gérer tout les paramètres des briques

    int i=0; //Variable gérant le remplissage horizontal du tableau
    int j=0; //Variable gérant le remplissage vertical du tableau

    for (i=0 ; i<10 ; i++){ //Boucle permettant de parcourir le tableau et de le remplir
        for (j=0 ; j<10 ; j++){
            AfficheBrique[i][j]->State = 1; //Etat de la brique passé a 1 stipulant qu'elle existe et sera affichée
            AfficheBrique[i][j]->BrickH = (24); //Set de la Hauteur de la brique
            AfficheBrique[i][j]->BrickW = (64); //Set de la largeur de la brique
            AfficheBrique[i][j]->BrickX = (i*64); //Set de la position en X de la brique
            AfficheBrique[i][j]->BrickY = (j*24); //Set de la position en Y de la brique
            AfficheBrique[i][j]->Color = rand() % 4 + 1; //Choix aléatoire de la couleur de la brique parmi 4
        }
    }

}

void MouvRaquette(gameState *state, coordonnees *dep){//Mouvement de la raquette grace a un etat

    if((state->left==1)){//Verification de l'etat gauche
            if((dep->x<16)){//Verification de collision
                dep->x+=1;//Si il y a collision alors la raquette change de direction jusqu'a ce qu'elle ne touche plus la bordure
            }else{
                dep->x+=-1;//Deplacement de la raquette de 1 picel vers la gauche
            }

    }else{
        if((state->right==1)){//Verification de l'etat droite
            if((dep->x>(SCREEN_WIDTH-144))){//Verification de collision
                dep->x-=1;//Si il y a collision alors la raquette change de direction jusqu'a ce qu'elle ne touche plus la bordure
            }else{
                dep->x+=1;//Deplacement de la raquette de 1 picel vers la droite
            }
        }
    }

}

void MouvBall(defcircle *DefCercle, coordonnees *dep){//Gestion du deplacement de la balle

    if(DefCercle->Launch==1){//Verification que la balle est lancee via son etat


        if((DefCercle->BallX>16) && (DefCercle->BallX<(SCREEN_WIDTH-16-RAYON_BALLE))){//Verification que la balle ne touche aucun bord vertical (gauche et droite)
            DefCercle->BallX += DefCercle->SpeedX;//Maintient de la vitesse actuelle
        }else{//Si la balle touche un cote
            DefCercle->SpeedX *= -1;//Inversion de la direction
            DefCercle->BallX += DefCercle->SpeedX;
        }

        if((DefCercle->BallY>0) && (DefCercle->BallY<SCREEN_HEIGHT-RAYON_BALLE)){//Verification que la balle ne touche pas un des bords horizontaux (top / bottom)
            DefCercle->BallY += DefCercle->SpeedY;//Maintient de la vitesse actuelle
        }else if(DefCercle->BallY==0){//La balle touche le haut de l'ecran
            DefCercle->SpeedY *= -1;//Inversion de la direction
            DefCercle->BallY += DefCercle->SpeedY;//Incrementation de la vitesse
        }else{//La balle touche le bas de l'ecran
            DefCercle->Launch = 0;//Reset de l'etat de lancement de la balle a 0
            DefCercle->SpeedX = 0;
            //Reset de la vitesse de la balle a 0
            DefCercle->SpeedY = 0;
            DefCercle->BallX = (dep->x+52);
            //Reset des coordonnees relatives de la balle a la raquette
            DefCercle->BallY = (SCREEN_HEIGHT-56);
        }

        if(((DefCercle->BallX<=(dep->x + ((2*(124 / 3)))-RAYON_BALLE)) && (DefCercle->BallX>=(dep->x + (124/3)))) && (DefCercle->BallY==(dep->y - RAYON_BALLE))){ //Verification que la balle se situe sur le tier central de la raquette
            DefCercle->SpeedX = 0; //Vitesse en X remise à 0
            DefCercle->SpeedY *= -1; //Inversion de la vitesse en Y
            DefCercle->BallX += DefCercle->SpeedX; //Les coordonnées X et Y de la balle sont mises a jour selon la vitesse de cette dernière
            DefCercle->BallY += DefCercle->SpeedY;
            }else if(((DefCercle->BallX>(dep->x + ((2*(124 / 3))-RAYON_BALLE)) && (DefCercle->BallX<dep->x + 124))) && (DefCercle->BallY==(dep->y - RAYON_BALLE))){ //Verification que la balle se situe sur le tier droit de la raquette
                DefCercle->SpeedX = 1;  //Vitesse en X prends la valeur 1 pour que la balle se dirige vers la droite
                DefCercle->SpeedY *= -1;
                DefCercle->BallX += DefCercle->SpeedX;
                DefCercle->BallY += DefCercle->SpeedY;
                }else if(((DefCercle->BallX>dep->x-RAYON_BALLE) && (DefCercle->BallX<(dep->x + (124/3)))) && (DefCercle->BallY==(dep->y - RAYON_BALLE))){ //Verification que la balle se situe sur le tier gauche de la raquette
                    DefCercle->SpeedX = -1; //Vitesse en X de la balle prends -1 pour qu'elle se dirige vers la gauche
                    DefCercle->SpeedY *= -1;
                    DefCercle->BallX += DefCercle->SpeedX;
                    DefCercle->BallY += DefCercle->SpeedY;
                    }

    }else{//Si la balle n'est pas lancee alors elle garde sa position relative a la raquette

        DefCercle->SpeedX = 0;
        //Maintient d'une vitesse nul pour que les coordonnees de la raquette ne s'incremente pas
        DefCercle->SpeedY = 0;
        DefCercle->BallX = (dep->x+52);//Maintient de la position X relative au centre de la raquette
        DefCercle->BallY = (SCREEN_HEIGHT-56);//Maintient de la position Y de sorte qu'elle soit au dessus de la raquette

    }


}

void destroy(game *myGame){

      //Destroy render
     if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);


    //Destroy surface
     if(myGame->g_psurface!=NULL)
         SDL_FreeSurface(myGame->g_psurface);

    //Destroy window
    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);

}


void SetupTerrain(game *myGame,gameState state,  coordonnees *dep, defcircle *DefCercle, Brick AfficheBrique[10][10]){

        SDL_Rect RaqDest;
        SDL_Rect BallDest;
        SDL_Rect BorderDest;
        SDL_Rect BriqueSource;
        SDL_Rect BriqueDest;
        int i=0;    //Variables d'incrément utilisée pour remplir le tableau de briques
        int j=0;


        myGame->g_psurface = IMG_Load("./assets/paddle.png");//Chargement de l'image png

         if(!myGame->g_psurface) {
            fprintf(stdout,"IMG_Load: %s\n", SDL_GetError());
            // handle error
        }


        if(myGame->g_psurface){

                 myGame->g_ptexture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_psurface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_psurface); // Libération de la ressource occupée par le sprite

                if(myGame->g_ptexture){

                    SDL_QueryTexture(myGame->g_ptexture,NULL,NULL,NULL,NULL);
                    //Définition du rectangle dest pour dessiner PNG
                    RaqDest.x=dep->x;//((SCREEN_WIDTH/2)-64);//debut x
                    RaqDest.y=(SCREEN_HEIGHT-32);//debut y
                    RaqDest.w=128; //Largeur
                    RaqDest.h=32; //Hauteur

                    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&RaqDest);


                }

                else{
                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                }



        }else{
            fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
        }




        myGame->g_psurface = IMG_Load("./assets/ball.png");//Chargement de l'image png

         if(!myGame->g_psurface) {
            fprintf(stdout,"IMG_Load: %s\n", SDL_GetError());
            // handle error
        }


        if(myGame->g_psurface){

                 myGame->g_ptexture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_psurface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_psurface); // Libération de la ressource occupée par le sprite

                if(myGame->g_ptexture){

                    SDL_QueryTexture(myGame->g_ptexture,NULL,NULL,NULL,NULL);
                    //Définition du rectangle dest pour dessiner PNG
                    BallDest.x=DefCercle->BallX;//((SCREEN_WIDTH/2)-64);//debut x
                    BallDest.y=DefCercle->BallY;//debut y
                    BallDest.w=RAYON_BALLE; //Largeur
                    BallDest.h=RAYON_BALLE; //Hauteur

                    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&BallDest);


                }

                else{
                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                }



        }else{
            fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
        }


        //Affectation des briques aux cases du tableau

        //for(i=0 ; i>=10 ; i++){ //Spawn des briques dans la largeur
            //for(j=0 ; j>=10 ; j++){//Spawn des briques dans la hauteur
                        myGame->g_psurface = IMG_Load("./assets/Bricks.bmp");//Chargement de l'image png

                         if(!myGame->g_psurface) {
                            fprintf(stdout,"IMG_Load: %s\n", SDL_GetError());
                            // handle error
                        }


                        if(myGame->g_psurface){

                                 myGame->g_ptexture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_psurface); // Préparation du sprite
                                 SDL_FreeSurface(myGame->g_psurface); // Libération de la ressource occupée par le sprite

                                if(myGame->g_ptexture){

                                    for(i=0 ; i<10 ; i++){ //Spawn des briques dans la largeur
                                        for(j=0 ; j<10 ; j++){//Spawn des briques dans la hauteur

                                            if((AfficheBrique[i][j].State=1)){
                                                if((AfficheBrique[i][j].Color = 1)){  //Couleur 1 = Rouge
                                                    BriqueSource.x=0;
                                                    BriqueSource.y=0;
                                                    BriqueSource.w=64;
                                                    BriqueSource.h=24;

                                                    SDL_QueryTexture(myGame->g_ptexture,NULL,NULL,NULL,NULL);

                                                    //Définition du rectangle dest pour dessiner PNG
                                                    BriqueDest.x=AfficheBrique[i][j].BrickX;//debut x
                                                    BriqueDest.y=AfficheBrique[i][j].BrickY;//debut y
                                                    BriqueDest.w=BriqueSource.w; //Largeur
                                                    BriqueDest.h=BriqueSource.h; //Hauteur

                                                    //SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,&BriqueSource,&BriqueDest);
                                                }else{
                                                    if((AfficheBrique[i][j].Color = 2)){ //Couleur 2 = Jaune
                                                        BriqueSource.x=64;
                                                        BriqueSource.y=0;
                                                        BriqueSource.w=64;
                                                        BriqueSource.h=24;

                                                        SDL_QueryTexture(myGame->g_ptexture,NULL,NULL,NULL,NULL);

                                                        //Définition du rectangle dest pour dessiner PNG
                                                        BriqueDest.x=AfficheBrique[i][j].BrickX;//debut x
                                                        BriqueDest.y=AfficheBrique[i][j].BrickY;//debut y
                                                        BriqueDest.w=BriqueSource.w; //Largeur
                                                        BriqueDest.h=BriqueSource.h; //Hauteur

                                                        //SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,&BriqueSource,&BriqueDest);
                                                    }else{
                                                        if((AfficheBrique[i][j].Color = 3)){ //Couleur 3 = Vert
                                                            BriqueSource.x=0;
                                                            BriqueSource.y=24;
                                                            BriqueSource.w=64;
                                                            BriqueSource.h=24;

                                                            SDL_QueryTexture(myGame->g_ptexture,NULL,NULL,NULL,NULL);

                                                            //Définition du rectangle dest pour dessiner PNG
                                                            BriqueDest.x=AfficheBrique[i][j].BrickX;//debut x
                                                            BriqueDest.y=AfficheBrique[i][j].BrickY;//debut y
                                                            BriqueDest.w=BriqueSource.w; //Largeur
                                                            BriqueDest.h=BriqueSource.h; //Hauteur

                                                            //SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,&BriqueSource,&BriqueDest);
                                                        }else{
                                                            if((AfficheBrique[i][j].Color = 4)){ //Couleur 4 = Bleu
                                                                BriqueSource.x=64;
                                                                BriqueSource.y=24;
                                                                BriqueSource.w=64;
                                                                BriqueSource.h=24;

                                                                SDL_QueryTexture(myGame->g_ptexture,NULL,NULL,NULL,NULL);

                                                                //Définition du rectangle dest pour dessiner PNG
                                                                BriqueDest.x=AfficheBrique[i][j].BrickX;//debut x
                                                                BriqueDest.y=AfficheBrique[i][j].BrickY;//debut y
                                                                BriqueDest.w=BriqueSource.w; //Largeur
                                                                BriqueDest.h=BriqueSource.h; //Hauteur

                                                                //SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,&BriqueSource,&BriqueDest);
                                                            }
                                                        }
                                                    }
                                                }

                                                /*//Définition du rectangle dest pour dessiner PNG
                                                BriqueDest.x=0+(64*i);//((SCREEN_WIDTH/2)-64);//debut x
                                                BriqueDest.y=0+(24*j);//debut y
                                                BriqueDest.w=BriqueSource.w; //Largeur
                                                BriqueDest.h=BriqueSource.h; //Hauteur*/
                                            }
                                        }
                                    }
                                    /*BriqueSource.x=64;
                                    BriqueSource.y=0;
                                    BriqueSource.w=64;
                                    BriqueSource.h=24;

                                    //Définition du rectangle dest pour dessiner PNG
                                    BriqueDest.x=27;//((SCREEN_WIDTH/2)-64);//debut x
                                    BriqueDest.y=44;//debut y
                                    BriqueDest.w=BriqueSource.w; //Largeur
                                    BriqueDest.h=BriqueSource.h; //Hauteur*/

                                    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,&BriqueSource,&BriqueDest);


                                }


                                else{
                                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                                }



                        }else{
                            fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
                        }
            //}

        //}




        myGame->g_psurface = IMG_Load("./assets/side.png");//Chargement de l'image png

         if(!myGame->g_psurface) {
            fprintf(stdout,"IMG_Load: %s\n", SDL_GetError());
            // handle error
        }

        if(myGame->g_psurface){

                 myGame->g_ptexture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_psurface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_psurface); // Libération de la ressource occupée par le sprite

                if(myGame->g_ptexture){


                    SDL_QueryTexture(myGame->g_ptexture,NULL,NULL,NULL,NULL);
                    //Bordure gauche
                    BorderDest.x=0; //Affectation de la coordonnee X de la bordure gauche
                    BorderDest.y=0; //Affectation de la coordonnee Y de la bordure gauche
                    BorderDest.w=16; //Affectation de la largeur de la bordure
                    BorderDest.h=SCREEN_HEIGHT; //La hauteur de la bordure dépends de la hauteur de l'écran

                    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&BorderDest);


                    SDL_QueryTexture(myGame->g_ptexture,NULL,NULL,NULL,NULL);
                    //Bordure droite
                    BorderDest.x=(SCREEN_WIDTH-16); //Affectation de la coordonnee X de la bordure droite (largeur d'écran - largeur de bordure)
                    BorderDest.y=0; //Affectation de la coordonnee Y de la bordure droite
                    BorderDest.w=16; //Affectation de la largeur de la bordure
                    BorderDest.h=SCREEN_HEIGHT; //La hauteur de la bordure dépends de la hauteur de l'écran

                    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&BorderDest);



                    SDL_RenderPresent(myGame->g_pRenderer); // Affichage


                    SDL_RenderClear(myGame->g_pRenderer);

                }

                else{
                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                }



        }else{
            fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
        }


    destroyTexture(myGame);


}


void destroyTexture(game *myGame){


    //Destroy texture
    if(myGame->g_ptexture!=NULL)
            SDL_DestroyTexture(myGame->g_ptexture);

}

