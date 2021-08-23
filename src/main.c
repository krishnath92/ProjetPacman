   /********************
   * **********************************************************/
/* MAIN.c                                                                     */
/****************************************************
 * **************************/
#include "./main.h"
#include "../lib/libgraphique.h"
#define fenetre_pixel 25

/******************************************************************************/
/* MAIN                                                                       */
/******************************************************************************/
int main(int argc, char **argv)
/* argc indique le nombre d'arguments,
   argv est un tableau de mots contenant ces arguments. */
    {

	
    Partie p;      // la structure contenant les données d'une partie
    int i,j;
    /** Traitement des arguments **************************************************/
    if(argc!=2) // Si le nombre d'arguments est différent de 2
        {
        printf("Usage: %s fichier_plan\n", argv[0]); // mode d'emploi et sortie
        return 0;
        }
    /******************************************************************************/

    /* Chargement du plan à partir du fichier fourni en paramètre                 */
    printf("Chargement du plan...\n");
    p = charge_plan(argv[1]);
    /* Si problème lors du chargement du plan...                                  */
    if(p.plateau == NULL)
        return 1;

#if DEBUG==1 //Ceci est un drapeau à activer/désactiver dans main.h
    /* Affichage du plan lu                                                       */
    printf("Affichage du plan...\n");
    for(i=0;i!=p.L;i++)
        {
        for(j=0;j!=p.C;j++)
            printf("%c",p.plateau[i][j]);
        printf("\n");
        }
#endif
     
/******************************************************************************/
/* A PARTIR D'ICI...C'EST A VOUS DE JOUER!                                    */
/******************************************************************************/
	p.boucle = 20;//elle permet de garder le pacman boster pendant 20 cases 
	p.ralentissement_fantomes = 0;//permet de ralentir le fantome selon le niv choisis
	p.compteur_point = 0;//fonction qui va compter les points(score)
	int menu;
	system("clear");//pour que rien ne soit afficher dans l'invite de comande 
	menu = menu_jeu();//recoit de la fonction un choix 
	system("clear");
	if (menu == 1)//si le choix est 1 alors dessiner le jeu 
    {   
      system("clear");
      presentation_niveaux();//presente les niveaux disponibles
      int niveaux;
	  niveaux = demande_classique(1,3);//choix du niveaux
      jouer(p,niveaux);//fonction qui va permettre de jouer 
    }
	actualiser();	 
	attendre_clic() ;
    return 0 ;
    }
//fonction qui va permettre de jouer
Partie jouer(Partie p,int niveaux)
{
		int quel_plateau = 0;
	    int hauteur_fenetre = p.L*fenetre_pixel;//hauteur de la fenetre selon p.L
        int largeur_fenetre = p.C*fenetre_pixel;//largeur de la fenetre selon p.C
        p = vie_selon_niv(p,niveaux);//quantité de vie selon le niv
        ouvrir_fenetre(largeur_fenetre,hauteur_fenetre);//ouvre une fenetre 
        graphique(p, hauteur_fenetre, largeur_fenetre);//fait le plateau de jeu
        fantome(p);//dessine les fantomes
        pacman_normal(p);//dessine le pacman normal
        //boucle infini
		while(1)
		{
			p = boucle_principale_jeu(p,niveaux);//permet de faire la boucle du jeu
			if (p.nppoint == p.compteur_point)//si le score = aux nombre de point
			{
				p = changement_map (p,niveaux,largeur_fenetre,hauteur_fenetre, quel_plateau);//changer de map
				quel_plateau++;//pour incrémenter la map
			}
		}
		return p;
	}	
Partie boucle_principale_jeu(Partie p,int niveaux)
{
	int touche;
	int touche_temporaire = attendre_touche_duree(125);
	if(touche_temporaire != 0)
		touche = touche_temporaire;
	system("clear");
	printf("Il reste %d de vie à pacman\n", p.nbvie+1);//affihe vie restant dans l'invite
	//affiche le score et les points restant dans l'invite
	printf ("VOTRE SCORE %d, IL VOUS RESTE %d POINT POUR PASSER AUX NIVEAUX SUIVANTS \n", p.compteur_point,p.nppoint - p.compteur_point);
	p = deplacement_pacman_fantome(touche, p,niveaux);//envoie a la partie le deplacement de pacman et fantome
	p.ralentissement_fantomes++;//incremente puis le divise selon le niveaux choisis ce qui va permettre le ralentissement
	return p;
}
Partie vie_selon_niv(Partie p, int niveaux)//vie selon niv
{
	if(niveaux == 1)
	    p.nbvie = 6;
	if(niveaux == 2)
	    p.nbvie = 4;
	if(niveaux == 3)
	    p.nbvie = 2;
	return p;
 }
int demande_classique(int a, int b)//fonction qui recoit le choix de l'utilisateur entre 1 et 3
{
    int choix;
   	do
    	{
        	printf("\nchoisissez entre %d et %d : ", a, b);
          	scanf("%d%*[1-3]",&choix);//envoie ce que l'utilisateur a choisis dans l'adresse choix
		    scanf("%*[^\n]");
		    getchar();//attent que l'utilisateur entre un caractère

		    printf("\n");
    	}while (choix < a || choix > b);// tant que le choix est entre 1 et 3 demander
    return choix;//retourne le choix 
}
void presentation_menu()
{
	    printf("\n\t\t\t*****************\n");
		printf("\t\t\t*   Bienvenue   *\n");
		printf("\t\t\t*  dans le jeu  *\n");
		printf("\t\t\t*     Pacman    *\n");
		printf("\t\t\t*****************\n");
	       	printf("\n\n MENU PRINCIPAL \n\n");
	        printf(" 1. Jouer\n");
	        printf(" 2. règles\n");
	       	printf(" 3. Quitter\n");	
}
void presentation_niveaux()
{
	printf("\n\n CHOISISSEZ UN NIVEAU \n\n");
	printf(" 1. Facile\n");
	printf(" 2. Moyen\n");
    printf(" 3. Difficile\n");
}	
int menu_jeu()//fonction qui va agir selon les choix 
{
	FILE *f;
	int menu;
	char c;
	do
	{
		presentation_menu();
		menu = demande_classique(1,3);//recupère le choix 
		if(menu == 2)//ouvrir le fichier contenant les règles du jeu 
		{
			f = fopen("regles.txt","r");
			system("clear");
			while(1) //boucle infini pour lire les caractères dans le texte
			{
      				c = fgetc(f);
      				if(feof(f)) //si il y'a plus rien a lire on arrete la lecture
         				break ;
      				printf("%c", c);			
  			} 
   			fclose(f);//fermer le fichier
   			getchar();//attente que l'utilisateur entre a caractère 
		}
	}while (menu == 2);
    if (menu == 3)//sion veut quiter on ferme la fenetre
	{
		system("clear");
		fermer_fenetre();
	}
	return menu;//envoie le menu choisis 
}
void graphique(Partie p, int hauteur_fenetre, int largeur_fenetre)//dessine le plateaux
{
	int a,b;
	Point dessin;
	dessin.y=0;
    for(a=0;a<=p.C+(fenetre_pixel/5);a++)//incrémentation colonne
    {   
        dessin.x=0;
        for(b=0;b<=p.L;b++)//incrémentation ligne 
	     {
	        detection_de_caractere(p,a, b,dessin,largeur_fenetre,hauteur_fenetre);//dessine selon les caracteres
	        dessin.x += fenetre_pixel;//lit les lignes selon les pixel donné
        }
       dessin.y += fenetre_pixel;//lit les colonnes selon les pixel donné
	}
	actualiser();
}
void point_et_bonus (Point dessin,int largeur_fenetre,int hauteur_fenetre, int taille)//pour dessine point et bonus
{
	Point dessin_rond;//pôur dessiné les points ronds 
    dessin_rond.x = dessin.x + (fenetre_pixel/2);//calcule pour que le point du cercle soit au milieu 
    dessin_rond.y = dessin.y + (fenetre_pixel/2);
	dessiner_rectangle(dessin,hauteur_fenetre,largeur_fenetre,noir);//dessiner le fond en noir
    dessiner_disque(dessin_rond,(fenetre_pixel/taille),white);//dessiner les petits points par dessus
}
void point (Point dessin,int largeur_fenetre,int hauteur_fenetre)//dessine point
{
	point_et_bonus (dessin,largeur_fenetre,hauteur_fenetre, 8);
}
void bonus(Point dessin, int largeur_fenetre,int hauteur_fenetre)//dessine bonus
{
	point_et_bonus (dessin,largeur_fenetre,hauteur_fenetre, 4);			
}	
void couleur_fantome(Partie p,Couleur couleur, int nombre_fantome )//va permettre de dessiner les fantomes avec la couleur et son nombre
{
	 Point dessin_rond;
     dessin_rond.x = p.fantomes[nombre_fantome].c * fenetre_pixel;//dessine un carré noir
	 dessin_rond.y = p.fantomes[nombre_fantome].l * fenetre_pixel;
	 dessiner_rectangle(dessin_rond, fenetre_pixel, fenetre_pixel, noir);
	 dessin_rond.x = dessin_rond.x + (fenetre_pixel/2);
	 dessin_rond.y = dessin_rond.y + (fenetre_pixel/2);	 
     dessiner_disque(dessin_rond,(fenetre_pixel/2)-3,couleur);//dessine par dessus le carré noir un rond avec la couleur 
}
void fantome(Partie p)//fonction qui dessine les fantomes selon les couleurs 
{
	 couleur_fantome(p,magenta,0);
     couleur_fantome(p,orange,1);
     couleur_fantome(p,lightblue,2);
     couleur_fantome(p,rouge,3);
     actualiser();
}	
void fantome_lors_du_bonus(Partie p,int i)//dessine les fantomes lors du bonus
{
	 Point dessin_rond;
	 dessin_rond.x = p.fantomes[i].c * fenetre_pixel + (fenetre_pixel/2);//pour dessiner a partir du milieu
	 dessin_rond.y = p.fantomes[i].l * fenetre_pixel + (fenetre_pixel/2);
	 dessiner_disque(dessin_rond,(fenetre_pixel/2)-3,bleu);//dessine tout les fantomes en bleu
}
void dessiner_pacman(Partie p,Couleur couleur)//fonction qui dessine pacman selon couleur
{
	 Point dessin_rond;
	 dessin_rond.x = p.pacman.c * fenetre_pixel + (fenetre_pixel/2);
	 dessin_rond.y = p.pacman.l * fenetre_pixel + (fenetre_pixel/2);
     dessiner_disque(dessin_rond,(fenetre_pixel/2)-3,couleur);//permettre de distinguer pacman normal et boster
     actualiser();
}
void pacman_normal(Partie p)//dessine pacman normal en jaune avec la fct au dessus
{
	   dessiner_pacman(p,jaune);
	   actualiser();   
}
void pacman_boster(Partie p)//dessine pacman boster en blanc 
{
	   dessiner_pacman(p,white);
	   p.plateau[p.pacman.l][p.pacman.c] = 'B';//change le caractere pour le differencié du pacman normal
	   actualiser();   
}
void dessiner_passage(Point dessin, int hauteur_fenetre, int largeur_fenetre)//fonction qui dessine les passages 
{
	dessiner_rectangle(dessin,hauteur_fenetre,largeur_fenetre,noir);//dessine le rectangle 
}
void dessiner_mur(Point dessin, int hauteur_fenetre, int largeur_fenetre)//fonction qui dessine les murs 
{
	dessiner_rectangle(dessin,hauteur_fenetre,largeur_fenetre,bleu);	//dessine un rectangle bleu 
}
//fonction qui permet de detecter les caractères et puis les dessiner à l'aide d'autre fonction 			 
void detection_de_caractere(Partie p,int a, int b,Point dessin,int largeur_fenetre, int hauteur_fenetre)
{
	if((p.plateau[a][b]) == '*')//dessine les murs si il repere *
	  dessiner_mur(dessin,hauteur_fenetre, largeur_fenetre);
    else if((p.plateau[a][b]) == '.')
	  point(dessin,largeur_fenetre,hauteur_fenetre);//dessine des petits point si il detecte .	
    else if((p.plateau[a][b]) == 'B')
      bonus(dessin,largeur_fenetre,hauteur_fenetre);//si on repere B dessiner un cercle blanc pour bonus	        
    else if((p.plateau[a][b]) == ' ')
	  dessiner_passage(dessin,largeur_fenetre,hauteur_fenetre);//si on repere' 'dessiner un rectangle noir pour les passages
}
Partie carre_noir_de_pacman(Partie p, Point dessin_rond)//dessine rectangle noir lors du deplacement de pacman
{
    dessin_rond.x = p.pacman.c * fenetre_pixel;
    dessin_rond.y = p.pacman.l * fenetre_pixel;
    dessiner_rectangle(dessin_rond, fenetre_pixel, fenetre_pixel, noir);
    //change le caractere en ' ' pour que par la suite les fantomes ne confonde pas 
    p.plateau[p.pacman.l][p.pacman.c] = ' ';
    return p;
}
Partie deplacement_de_pacman_a_gauche(Partie p, Point dessin_rond,int touche)
{
    if (p.pacman.c == 0)//si le pacman est tout a gauch
	     p = dessiner_deplacement_tunnel_pacman(p,(p.C-1));//alors le faire venir a droite	
	else if(p.plateau[p.pacman.l][p.pacman.c-1] != '*')//si il y'a pas de mur alors 
	{
		 if (p.plateau[p.pacman.l][p.pacman.c-1] == '.')//si il y'a un point
			 p.compteur_point ++;//compter le point
			 
		 p = carre_noir_de_pacman(p,dessin_rond);//dessine le carré pour chaque deplacement
         p.pacman.c -= 1;//fait deplacer la colonne de pacman de -1 
	}
    return p;
}
Partie deplacement_de_pacman_a_droite(Partie p, Point dessin_rond,int touche)
{
	 if (p.pacman.c == (p.C-1))//deplacement de droite a gauche pour le tunel
	   p = dessiner_deplacement_tunnel_pacman(p, 0);
	 else if(p.plateau[p.pacman.l][p.pacman.c+1] != '*')//si pas de mur alors 
	 {
		if (p.plateau[p.pacman.l][p.pacman.c+1] == '.')//si il est sur un point
		    p.compteur_point ++;//compte le point
		    
	    p = carre_noir_de_pacman(p,dessin_rond);
	    p.pacman.c += 1;//fait deplacer la ligne de pacman de -1 	
	 }
    return p;
}
Partie deplacement_de_pacman_en_bas(Partie p, Point dessin_rond,int touche)
{
    if(p.plateau[p.pacman.l+1][p.pacman.c] != '*')
	{
	    if (p.plateau[p.pacman.l+1][p.pacman.c] == '.')
		   p.compteur_point ++;
		p = carre_noir_de_pacman(p,dessin_rond);
        p.pacman.l += 1;//fait deplacer la ligne de pacman de +1 
	}
    return p;
}
Partie deplacement_de_pacman_en_haut(Partie p, Point dessin_rond,int touche)
{
    if(p.plateau[p.pacman.l-1][p.pacman.c] != '*')
	{
		if (p.plateau[p.pacman.l-1][p.pacman.c] == '.')
			p.compteur_point ++;
		p = carre_noir_de_pacman(p,dessin_rond);
		p.pacman.l -= 1; //fait deplacer la ligne de pacman de -1 
	}
    return p;
}
Partie deplacement_pacman(int touche, Partie p)//reunit tout les deplacements
{       //selon la touche fais apelle a sa fonction
		Point dessin_rond;
		if(touche == SDLK_LEFT)
			  p =deplacement_de_pacman_a_gauche(p,dessin_rond,touche);
		if(touche == SDLK_DOWN)
			  p = deplacement_de_pacman_en_bas(p,dessin_rond,touche);
		if(touche == SDLK_UP)
			  p= deplacement_de_pacman_en_haut(p,dessin_rond,touche);
	    if(touche == SDLK_RIGHT)
			  p = deplacement_de_pacman_a_droite(p,dessin_rond,touche);
		return p;
} 		
Partie deplacement_pacman_fantome(int touche, Partie p,int niveaux)
{
	if (p.plateau[p.pacman.l][p.pacman.c] != 'B')//si pacman est different lorsqu'il est boster
	{
		p = deplacement_pacman(touche,p);//deplacement pacman normal
		pacman_normal(p);//dessiner pacman normal
	}
	else if (p.plateau[p.pacman.l][p.pacman.c] == 'B')//si pacman boster
	{
		if (p.boucle > 0)//boucle est de 20 donc il va etre en boste pendant 20 cases
		{   
			p = deplacement_pacman(touche,p);
			pacman_boster(p);
			p.boucle --;//incremente a chaque deplacement
	    }
	    else if (p.boucle == 0)//si il n'est plus en boost
	    {
			p.plateau[p.pacman.l][p.pacman.c] = 'P';//alors remettre le caractere de pacman
			p.boucle +=20;//ajouter 20 pour que lors du prochain bonus il puisse refaire 20 cases
	    }
	}
	p = ralentissement_fantome(p,niveaux);//ralentir pacman selon niv 
	actualiser();
    return p;
}
Partie ralentissement_fantome(Partie p, int niveaux)
{
	   Point dessin_fantome;
	   if(niveaux == 1)//si le niv choisis = 1 
	   {
		 if (p.ralentissement_fantomes%4 == 0)//lors que pacman avance de 4 case les fantomes de 1
			p = deplacement_fantome(dessin_fantome,p);
	   }
	   if(niveaux == 2)// si niv 2 
	   {
		 if (p.ralentissement_fantomes%2 == 0)//lors que pacman avance de 2 case les fantomes de 1
			p = deplacement_fantome(dessin_fantome,p);
	   }
	   if(niveaux == 3)// si niv 3 
	   {
		 if (p.ralentissement_fantomes%1 == 0)//avance tout les 2 a la meme vitesse
			p = deplacement_fantome(dessin_fantome,p);
	   }
	   return p;
}
Partie redessine_le_dep_fantome_pour_point_et_bonus(Partie p, Point dessin_fantome, int i,int taille)
{
	dessin_fantome.x = p.fantomes[i].c * fenetre_pixel + (fenetre_pixel/2);
    dessin_fantome.y = p.fantomes[i].l * fenetre_pixel + (fenetre_pixel/2);
    dessiner_disque(dessin_fantome,(fenetre_pixel/taille),white);
    return p; 
}
//redessine les caractères lors du deplacement de fantome
Partie redessine_le_deplacement_de_fantome(Partie p, Point dessin_fantome,int i)
{
	dessin_fantome.x = p.fantomes[i].c * fenetre_pixel;
	dessin_fantome.y = p.fantomes[i].l * fenetre_pixel;
	dessiner_rectangle(dessin_fantome, fenetre_pixel, fenetre_pixel, noir);//carré noir pour chaque deplacement
	if (p.plateau[p.fantomes[i].l][p.fantomes[i].c] == '.')//si le fantome est sur un point
	{
		  p= redessine_le_dep_fantome_pour_point_et_bonus(p,dessin_fantome,i,8);//dessiner le point
		  p.plateau[p.fantomes[i].l][p.fantomes[i].c] = '.';//remettre son caractere pour que pacman puissent detecter les points
    }
	else if (p.plateau[p.fantomes[i].l][p.fantomes[i].c] == 'B')//si le f est sur un bonus
	{
		  p= redessine_le_dep_fantome_pour_point_et_bonus(p,dessin_fantome,i,4);//dessiner le bonus
		  p.plateau[p.fantomes[i].l][p.fantomes[i].c] = 'B';//remettre son caractere 
	}
	else if (p.plateau[p.fantomes[i].l][p.fantomes[i].c] == ' ')//si f est sur le passage
	{
		  dessin_fantome.x = p.fantomes[i].c * fenetre_pixel;
		  dessin_fantome.y = p.fantomes[i].l * fenetre_pixel;
		  dessiner_rectangle(dessin_fantome, fenetre_pixel, fenetre_pixel, noir);//dessiner carré noir
		  p.plateau[p.fantomes[i].l][p.fantomes[i].c] = ' ';//remettre soncaractere 
	}
	return p;
}
Partie deplacement_fantome_gauche(Point dessin_fantome,Partie p,int i)
{ 
     if (p.fantomes[i].c == 0)//si fantome a gauche du tunel
			p = dessiner_deplacement_tunnel_fantomes(p,(p.C-1), i);//fonction qui fait passer a droite 
	 else
			p.fantomes[i].c -= 1;// sinon faire avancer le fantome a gauche
	  
	 return p;
}
Partie deplacement_fantome_droite(Point dessin_fantome,Partie p,int i)
{ 
	  if (p.fantomes[i].c == (p.C-1))//si fantome a droite du tunel
		 p = dessiner_deplacement_tunnel_fantomes(p, 0, i);//fonction qui fait passer a gauche 
	  else
		p.fantomes[i].c += 1;//sinon faire avancer a droite 
	  return p;
}
Partie deplacement_fantome_bas(Point dessin_fantome,Partie p,int i)
{
	p.fantomes[i].l += 1;//faire avancer en bas 
	return p;
}
Partie deplacement_fantome_haut(Point dessin_fantome,Partie p,int i)
{
	p.fantomes[i].l -= 1;//faire avancer en bas 
	return p;
}
Partie deplacement_fantome_normal(Point dessin_fantome,Partie p, verification dire, int i,info inf)
{  
	  dire = verification_direction(p, dire, i);
	  inf = information_direction(p,dire,inf);//envoie les info si les fantomes ne sont pas l'un sur l'autre
	  //si la colonne pacman est inférieur à celle du fantome et qu'il y'a pas de mur ni fantome sur lui
	  if((p.pacman.c < p.fantomes[i].c)&&(p.plateau[p.fantomes[i].l][p.fantomes[i].c-1] != '*') && (inf.deplacement_gauche == 0))
          p = deplacement_fantome_gauche(dessin_fantome,p,i);
	  //si la colonne pacman est supérieur à celle du fantome et qu'il y'a pas de mur ni fantome sur lui
	  else if((p.pacman.c > p.fantomes[i].c) && (p.plateau[p.fantomes[i].l][p.fantomes[i].c+1] != '*') && (inf.deplacement_droite == 0))
		  p = deplacement_fantome_droite(dessin_fantome,p,i);
	   //si la ligne pacman est supérieur à celle du fantome et qu'il y'a pas de mur ni fantome sur lui
	  else if((p.pacman.l > p.fantomes[i].l) && (p.plateau[p.fantomes[i].l+1][p.fantomes[i].c] != '*') && (inf.deplacement_bas == 0))
		  p = deplacement_fantome_bas(dessin_fantome,p,i);
	  //si la ligne pacman est inférieur à celle du fantome et qu'il y'a pas de mur ni fantome sur lui
	  else if((p.pacman.l < p.fantomes[i].l) && (p.plateau[p.fantomes[i].l-1][p.fantomes[i].c] != '*') && (inf.deplacement_haut == 0))
	      p = deplacement_fantome_haut(dessin_fantome,p,i);
	  fantome(p);//dessiner les fantomes
	  p = victoire_des_fantomes(p);//si 0 vie = victoire fantomes
	  return p;
}
Partie deplacement_fantome_peur(Point dessin_fantome, Partie p, verification dire, int i,info inf)
{  
	  dire = verification_direction(p, dire, i);
	  inf = information_direction(p,dire,inf);
	  //si la colonne pacman est inférieur à celle du fantome et qu'il y'a pas de mur ni fantome sur lui
	  if((p.pacman.c < p.fantomes[i].c)&&(p.plateau[p.fantomes[i].l][p.fantomes[i].c+1] != '*') && (inf.deplacement_droite == 0))
		 p = deplacement_fantome_droite(dessin_fantome,p,i);//mouv inverse pour chaque cas 
	  //si la colonne pacman est superieur à celle du fantome et qu'il y'a pas de mur ni fantome sur lui
	  else if((p.pacman.c > p.fantomes[i].c)&&(p.plateau[p.fantomes[i].l][p.fantomes[i].c-1] != '*') && (inf.deplacement_gauche == 0))
         p = deplacement_fantome_gauche(dessin_fantome,p,i);
	  //si la ligne pacman est supérieur à celle du fantome et qu'il y'a pas de mur ni fantome sur lui
	  else if((p.pacman.l > p.fantomes[i].l)&&(p.plateau[p.fantomes[i].l-1][p.fantomes[i].c] != '*') && (inf.deplacement_haut == 0))
		 p = deplacement_fantome_haut(dessin_fantome,p,i);
	  //si la ligne pacman est supérieur à celle du fantome et qu'il y'a pas de mur ni fantome sur lui
	  else if((p.pacman.l < p.fantomes[i].l)&&(p.plateau[p.fantomes[i].l+1][p.fantomes[i].c] != '*') && (inf.deplacement_bas == 0))
		 p = deplacement_fantome_bas(dessin_fantome,p,i);
	 fantome_lors_du_bonus(p,i);//dessine les fantomes lors que pacman est boster
     p = reanimation_fantome(p);//reanime les fantomes 
     return p;
}
Partie deplacement_fantome(Point dessin_fantome,Partie p)
{
	verification dire;
	info inf;
	int i;
	for (i = 0; i<NBFANTOMES; i++)//pour chaque fantome
	{
	  if (p.plateau[p.pacman.l][p.pacman.c] != 'B')//si pacman pas boster faire avancer les fantomes vers lui
	  {   
		 p =redessine_le_deplacement_de_fantome(p, dessin_fantome,i);//redessine le deplacement de fantome selon leur caractère    
         p = deplacement_fantome_normal(dessin_fantome,p,dire,i,inf);   
      }
	 else //si pacman boster eloigner les fantomes de lui
	  { 
		 p =redessine_le_deplacement_de_fantome(p, dessin_fantome,i);
		 p = deplacement_fantome_peur(dessin_fantome,p,dire,i,inf);     
	  }
	  actualiser();
   }
   return p;
}
verification verification_direction(Partie p, verification dire, int i )
{
	dire.bas.c = p.fantomes[i].c;
	dire.bas.l = p.fantomes[i].l+1;
	dire.haut.c = p.fantomes[i].c;
	dire.haut.l = p.fantomes[i].l-1;
	dire.droite.c = p.fantomes[i].c+1;
	dire.droite.l = p.fantomes[i].l;
	dire.gauche.c = p.fantomes[i].c-1;
	dire.gauche.l = p.fantomes[i].l;
	return dire;
}
info information_direction (Partie p, verification dire,info inf)
{
	//si les fantomes se rencontre renvoie +1 sinon 0 
    inf.deplacement_bas = rencontre_fantome(dire.bas, p);
	inf.deplacement_haut = rencontre_fantome(dire.haut, p);
	inf.deplacement_droite = rencontre_fantome(dire.droite, p);
	inf.deplacement_gauche = rencontre_fantome(dire.gauche, p); 
	return inf;
} 
int rencontre_fantome(Pos direction, Partie p) // cette fonction fait en sorte de retourner l'information que dans la direction où veut aller un autre fantomes il n'y a pas un autre fantome
{
	int i, information;
	information = 0;
	for (i = 0; i<NBFANTOMES; i++)//pour chaque fantome
		if( (direction.l == p.fantomes[i].l) && (direction.c == p.fantomes[i].c) )//si fantome un sur l'autre alors 
			information++;
	return information;
}
Partie victoire_des_fantomes(Partie p)           // si les fantome mangent pacmant on enlève une vie à pacman
{
	if(((p.pacman.c == p.fantomes[1].c)&&(p.pacman.l == p.fantomes[1].l))//si un des fantomes est sur pacman 
	||((p.pacman.c == p.fantomes[2].c)&&(p.pacman.l == p.fantomes[2].l))
	||((p.pacman.c == p.fantomes[3].c)&&(p.pacman.l == p.fantomes[3].l))
	||((p.pacman.c == p.fantomes[0].c)&&(p.pacman.l == p.fantomes[0].l)))
	{
		if(p.nbvie > 0)//si sa vie est >0
		{
			p.pacman.c = 10;
			p.pacman.l = 20;//coordonner de pacman
			dessiner_pacman(p,yellow);//redessiner sur les coordoner 
			actualiser(); 
			p.nbvie --;//avec une vie en moi
		}
		else if(p.nbvie == 0)//si plus de vie 
		{
			system("clear");
			printf("Victoire des fantomes ^'!!\nVous avez perdu ^' !!\n");//afficher victoire de fantome
			fermer_fenetre();
		}	
	}
	return p;
}
Partie reanimation_fantome(Partie p)//lorsque pacman boster les mangent 
{
	p = redessiner_fantome_reanimation(p, 0, 7, 10);
	p = redessiner_fantome_reanimation(p, 1, 13, 10);
	p = redessiner_fantome_reanimation(p, 2, 8, 15);
	p = redessiner_fantome_reanimation(p, 3, 12, 15);
	pacman_boster(p);
	actualiser();
    return p;
}
Partie redessiner_fantome_reanimation(Partie p, int a, int b, int c)   //une fois que pacman mangent un fantome, ce fantome est remis a sa pos initial
{
	if((p.pacman.c == p.fantomes[a].c)&&(p.pacman.l == p.fantomes[a].l))
	{
		p.fantomes[a].c = b;
		p.fantomes[a].l = c;//redéfinition des coordonnée d'un fantome
		couleur_fantome(p,bleu,a);
	}
	return p;
}
Partie dessiner_deplacement_tunnel_pacman(Partie p,int loc_deplacement)	//reddefinition des emplacement et dessins de pacman et après qu'ils ont utiliser un tunnel
{
	Point dessiner_tunnel;				// le pacman est deplacer à la même ligne et à la colone oposée d'où il était avant d'emprunter le tunnel
	dessiner_tunnel.x = p.pacman.c * fenetre_pixel;
	dessiner_tunnel.y = p.pacman.l * fenetre_pixel;
	dessiner_rectangle(dessiner_tunnel, fenetre_pixel, fenetre_pixel, noir);
	dessiner_tunnel.x = loc_deplacement * fenetre_pixel + (fenetre_pixel/2);
	dessiner_tunnel.y = p.pacman.l * fenetre_pixel + (fenetre_pixel/2);
	dessiner_disque(dessiner_tunnel,(fenetre_pixel/2)-3,yellow);
	p.plateau[p.pacman.l][p.pacman.c] = ' ';
	p.pacman.c = loc_deplacement;
	return p;
}
Partie dessiner_deplacement_tunnel_fantomes(Partie p,int loc_deplacement, int i) //redefinition des emplacement et dessins des fantomes et après qu'ils ont utiliser un tunnel
{
	Point dessiner_tunnel;  	// le fantome est deplacer à la même ligne et à la colone oposée d'où il était avant d'emprunter le tunnel
	dessiner_tunnel.x = p.fantomes[i].c * fenetre_pixel;
	dessiner_tunnel.y = p.fantomes[i].l * fenetre_pixel;
	dessiner_rectangle(dessiner_tunnel, fenetre_pixel, fenetre_pixel, noir);
	dessiner_tunnel.x = loc_deplacement * fenetre_pixel + (fenetre_pixel/2);
	dessiner_tunnel.y = p.fantomes[i].l * fenetre_pixel + (fenetre_pixel/2);
	dessiner_disque(dessiner_tunnel,(fenetre_pixel/2)-3,yellow);
	p.plateau[p.fantomes[i].l][p.fantomes[i].c] = ' ';
	p.fantomes[i].c = loc_deplacement;
	return p;
}
Partie changement_map (Partie p,int niveaux, int largeur_fenetre, int hauteur_fenetre, int i)
{
	   if(i == 0)
		{
		   p = charge_plan("data/plateau_1.txt");		//charge le deuxième plateau plateau
	       p = reinitialisation_partie(p,niveaux,largeur_fenetre,hauteur_fenetre);
	    }
	    if (i == 1)		//charge le troisième plateau
	    {
	       p = charge_plan("data/plateau_2.txt");
	       p = reinitialisation_partie(p,niveaux,largeur_fenetre,hauteur_fenetre);
	    }
	    if (i==2)//fin du jeu si on a fini les trois plateau et que pacman a encore toute ces vies
	    {
			fermer_fenetre();
			printf("Vous avez reussis tout les niveaux!");
	    }
       return p;
}
Partie reinitialisation_partie(Partie p,int niveaux, int largeur_fenetre, int hauteur_fenetre)
{	    
		p = vie_selon_niv(p,niveaux);//vie selon le niv 
		p.compteur_point = 0;
		p.boucle = 20;
		graphique(p, hauteur_fenetre, largeur_fenetre);//fait le plateau de jeu
		fantome(p);//dessine les fantomes
		pacman_normal(p);//dessine le pacman normal
		return p;
}


