/******************************************************************************/
/* MAIN.h                                                                     */
/******************************************************************************/
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include "./partie.h" // on a besoin de connaître la structure Partie
#include "../lib/libgraphique.h"

#define DEBUG 1


/******************************************************************************/
/*FONCTION*/

//fonction qui presente le menu 
void presentation_menu();
//fonction qui prend en compte le choix de l'utilisateur
int demande_classique(int a , int b);
//fonction menu jeu selon le choix de l'utilisateur 
int menu_jeu();
//fonction qui presente les niveaux dans la commande
void presentation_niveaux();
//fonction qui detecte les caracteres 
void detection_de_caractere(Partie p,int a, int b,Point dessin,int largeur_fenetre, int hauteur_fenetre);
//fonction qui sert a dessiner le plateau 
void graphique(Partie p, int hauteur_fenetre, int largeur_fenetre);
//fonction qui va permettre de dessiner point et bonus 
void point_et_bonus (Point dessin, int largeur_fenetre,int hauteur_fenetre, int taille);
//fonction qui dessine les petits point avec la fonction point_et_bonus
void point (Point dessin,int largeur_fenetre,int hauteur_fenetre);
//fonction qui dessine les bonus avec la fonction point_et_bonus
void bonus(Point dessin,int largeur_fenetre,int hauteur_fenetre);
//fonction qui dessine les passages 
void dessiner_passage(Point dessin, int hauteur_fenetre, int largeur_fenetre);
//fonction qui dessine un mur 
void dessiner_mur(Point dessin, int hauteur_fenetre, int largeur_fenetre);
//fonction qui dessine les fantomes 
void fantome(Partie p);
//fonction qui change la couleur de chaque fantome 
void couleur_fantome(Partie p,Couleur couleur, int nombre_fantome );
//fonction qui dessine les fantomes lors du bonus 
void fantome_lors_du_bonus(Partie p,int i);
//fonction qui dessine le pacman normal
void pacman_normal(Partie p);
//fonction qui dessine le pacman boster
void pacman_boster(Partie p );
//fonction qui va permettre de dessiner pacman boster et normal
void dessiner_pacman(Partie p,Couleur couleur);
//fonction qui va permettre de jouer
Partie jouer(Partie p,int niveaux);
//fonction qui permet à faire la boucle principal du jeu 
Partie boucle_principale_jeu(Partie p,int niveaux);
//fonction qui dessine les carre noir quand pacman se deplace
Partie carre_noir_de_pacman(Partie p, Point dessin_rond);
//fonction qui s'occupe du deplacement de pacman en haut
Partie deplacement_de_pacman_en_haut(Partie p, Point dessin_rond,int touche);
//fonction qui s'occupe du deplacement de pacman en bas
Partie deplacement_de_pacman_en_bas(Partie p, Point dessin_rond,int touche);
//fonction qui s'occupe du deplacement de pacman a droite
Partie deplacement_de_pacman_a_droite(Partie p, Point dessin_rond,int touche);
//fonction qui s'occupe du deplacement de pacman a gauche 
Partie deplacement_de_pacman_a_gauche( Partie p, Point dessin_rond,int touche);	
//fonction qui s'occupe du deplacement de pacman dans toute les directions 
Partie deplacement_pacman(int touche, Partie p);
//fonction qui s'occupe des deplacements vers le haut des fantomes normal 
Partie deplacement_fantome_haut(Point dessin_fantome, Partie p,int i);
//fonction qui s'occupe des deplacements vers le bas des fantomes normal 
Partie deplacement_fantome_bas(Point dessin_fantome, Partie p, int i);
//fonction qui s'occupe des deplacements vers la droite des fantomes normal 
Partie deplacement_fantome_droite(Point dessin_fantome, Partie p, int i);
//fonction qui s'occupe des deplacements vers la gauche des fantomes normal 
Partie deplacement_fantome_gauche(Point dessin_fantome,Partie p,int i);
//fonction qui s'occupe des deplacement global des fantomes 
Partie deplacement_fantome(Point dessin_fantome, Partie p);
//fonction qui s'occupe des deplacement des fantomes qui ont peur de pacman
Partie deplacement_fantome_peur(Point dessin_fantome, Partie p, verification dire, int i,info inf);
//fonction qui s'occupe des deplacement des fantomes normal
Partie deplacement_fantome_normal(Point dessin_fantome, Partie p, verification dire, int i,info inf);
//fonction qui va permettre d'eviter qu'un fantome se met sur l'autre 
int rencontre_fantome(Pos direction, Partie p);
//les variables qui vont faire en sorte qu'un fantome ne se met pas sur l'autre 
info information_direction (Partie p, verification dire,info inf);
//fonction qui va permettre de dessiner bonus et point lors du deplacement
Partie redessine_le_dep_fantome_pour_point_et_bonus(Partie p, Point dessin_fantome, int i,int taille);
//fonction qui s'occupe de redessiner le deplacement de fantome
Partie redessine_le_deplacement_de_fantome( Partie p, Point dessin_fantome,int i);
//fonction qui va mettre en lien les deplacement fantomes et pacman
Partie deplacement_pacman_fantome(int touche, Partie p,int niveaux);
//fonction qui gere le deplacemt quand pacman utilise un tunnel
Partie dessiner_deplacement_tunnel_pacman(Partie p,int loc_deplacement);
//fonction qui gere le deplacemt quand un fantome utilise un tunnel
Partie dessiner_deplacement_tunnel_fantomes(Partie p,int loc_deplacement, int i);
//fonction lors de la victoire des fantomes 
Partie victoire_des_fantomes(Partie p);
// fonction qui va reanimer les fantomes lorsque pacman boster les mangent
Partie reanimation_fantome(Partie p);
//fonction qui va changer de map
Partie changement_map (Partie p,int niveaux, int largeur_fenetre, int hauteur_fenetre, int i);
//fonction qui va ajuster les vie selon le niv
Partie vie_selon_niv(Partie p, int niveaux);
//fonction qui va ralentir les fantomes selon le niveaux 
Partie ralentissement_fantome(Partie p, int niveaux);
Partie reinitialisation_partie(Partie p,int niveaux, int largeur_fenetre, int hauteur_fenetre);
//création de pos pour vérifier que les fantomes ne s'entrechoquent pas
verification verification_direction(Partie p, verification dire, int i);
//replacer et redessiner les fantomes aprés qu'ils ont étaient bouger par pacman
Partie redessiner_fantome_reanimation(Partie p, int a, int b, int c);

#endif
