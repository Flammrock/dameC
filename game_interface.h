/*********************************************************************************\
* Copyright (c) 2020 - Flammrock                                                  *
*                                                                                 *
* Permission is hereby granted, free of charge, to any person obtaining a copy    *
* of this software and associated documentation files (the "Software"), to deal   *
* in the Software without restriction, including without limitation the rights    *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       *
* copies of the Software, and to permit persons to whom the Software is           *
* furnished to do so, subject to the following conditions:                        *
*                                                                                 *
* The above copyright notice and this permission notice shall be included in all  *
* copies or substantial portions of the Software.                                 *
*                                                                                 *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
* SOFTWARE.                                                                       *
\*********************************************************************************/



/**
 * game_interface.h
 *
 */
 
#ifndef CHECKERS_GAMEINTERFACE_H
#define CHECKERS_GAMEINTERFACE_H




/**********************\
*                      *
*       INCLUDE        *
*                      *
\**********************/

#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"
#include "bucket_explorer.h"
#include "colors.h"
#include "display.h"
#include "menu.h"
#include "loader.h"
#include "splashscreen.h"
#include "game.h"







/**********************\
*                      *
*       FUNCTIONS      *
*                      *
\**********************/


/**
 * _fgets : permet de récupérer une ligne entière
 * (même fonctionnement que fgets)
 *
 * @param {char*} s - chaîne où l'on mettra la ligne récupérer dans le stream
 * @param {int} l - taille de la chaîne s
 * @param {FILE*} stream - stream où l'on veut récupérer une ligne
 */
void _fgets(char *s, int l, FILE *stream) {
  int c;
  int i = 0;
  while ((c = getc(stream)) != '\n' && i+1 < l) s[i++] = c;
  s[i] = '\0';
}

joueur tempPlayer[2]; // On met la saisi dans des variables globales temporaires
int tempPlayerValid[2]; // Permet de savoir si les info des joueurs sont valides
int tempPlayerinit = 0; // 0 => valeur de tempPlayerValid indéterminé, 1 => valeur de tempPlayerValid déterminé

/**
 * menu_set_player : permet d'appliquer la saisie des joueurs dans un jeu
 *
 * @param {jeu*} g - le jeu
 */
void menu_set_player(jeu*g) {
  g->tabJ[0] = tempPlayer[0];
  g->tabJ[1] = tempPlayer[1];
}

/**
 * (utilisé par menu.h comme pointeur fonction (@see menu.h))
 * menu_get_player : permet de faire la saisi d'un joueur
 *
 * @Event {menu_event} (fonction appelé lors du choix d'un item dans le menu) (@see menu.h) (les arguments sont décrits dans menu.h et ce qui est retourné)
 *
 * @param {int} i - 0 => joueur 1, 1 => joueur 2
 */
menu_message menu_get_player(menu* m, menu_item* item, int i) {
  resetTerm();

  // Comme i correspond à la position de l'item, il se peut qu'il y est des items avant
  // On cherche donc à savoir si le joueur veut saisir le joueur 1 ou le joueur 2
  int decal = 0;
  int decals = 0;
  for (int k = 0; k < m->length; k++) {
    if (m->items[k]->is_separator||m->items[k]->is_hide) decals++;
    if (m->items[k]->_fn!=NULL) {
      decal = k;
      break;
    }
  }
  i -= decal; // On enlève le nombre d'items qui sont avant l'item qui permet de saisir le joueur 1
  i += decals; // On rajoute le nombre d'items qui ne sont pas sélectionnables


  // On initialise tempPlayerValid
  if (!tempPlayerinit) {
    tempPlayerValid[0] = 0;
    tempPlayerValid[1] = 0;
    tempPlayerinit = 1;
  }

  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);

  resetTerm();

  // On alloue de la mémoire pour le nom
  int test = tempPlayer[i].nom==NULL;
  if (tempPlayer[i].nom==NULL) {
    tempPlayer[i].nom = malloc(200*sizeof(char));
  }

  // Saisie interactive :

  int couleur = -1;
  int type = -1;
  for (int j = 0; j < w/2-7; j++) printf(" ");
  printf("Saisie Joueur %s%d"RESET" :\n",i==0?BRED:BBLU,i+1);
  if (test) {
    for (int j = 0; j < w/2-7; j++) printf(" ");
    printf(RESET"Nom. . . . .: ""\e[2m""[VIDE]\n"RESET);
    for (int j = 0; j < w/2-7; j++) printf(" ");
    printf(RESET"Couleur. . .: ""\e[2m""[VIDE]\n"RESET);
    for (int j = 0; j < w/2-7; j++) printf(" ");
    printf(RESET"Type . . . .: ""\e[2m""[VIDE]\n"RESET);
  } else {
    for (int j = 0; j < w/2-7; j++) printf(" ");
    printf(RESET"Nom. . . . .: ""\e[2m""%s\n"RESET,tempPlayer[i].nom);
    for (int j = 0; j < w/2-7; j++) printf(" ");
    printf(RESET"Couleur. . .: ""\e[2m""%d\n"RESET,tempPlayer[i].coul==BLANC?0:1);
    for (int j = 0; j < w/2-7; j++) printf(" ");
    printf(RESET"Type . . . .: ""\e[2m""%d\n"RESET,tempPlayer[i].type==Humain?0:1);
  }
  printf("\e[F\e[F\e[F");
  for (int j = 0; j < w/2-7; j++) printf(" ");
  printf(RESET"Nom. . . . .: ""\e[2m""\e[K");
  _fgets(tempPlayer[i].nom, 200, stdin);
  int ct = -1;
  if (tempPlayer[(i+1)%2].nom!=NULL) {
    ct = tempPlayer[(i+1)%2].coul==BLANC?1:0;
  }
  do {
    for (int j = 0; j < w/2-7; j++) printf(" ");
    printf(RESET"Couleur. . .: [0 pour blanc ou 1 pour noir] ""\e[2m""\e[K");
    scanf("%d",&couleur);
    clearstdin();
    printf("\e[F");
  } while(couleur!=0&&couleur!=1);
  if (ct!=-1&&ct!=couleur) {
    m->items[decal+(i+1)%2]->name[0] = '\0';
    strcat(m->items[decal+(i+1)%2]->name,i!=0?"Saisie du Joueur 1 "RESET"["YEL"VIDE"RESET"]":"Saisie du Joueur 2 "RESET"["YEL"VIDE"RESET"]");
    tempPlayerValid[(i+1)%2] = 0;
  }
  tempPlayer[i].coul = couleur?NOIR:BLANC;
  printf("\n\e[F");
  for (int j = 0; j < w/2-7; j++) printf(" ");
  printf(RESET"\e[2KCouleur. . .: ""\e[2m""%d\n",couleur);
  do {
    for (int j = 0; j < w/2-7; j++) printf(" ");
    printf(RESET"Type . . . .: ""[0 pour Humain ou 1 pour CPU] ""\e[2m""\e[K");
    scanf("%d",&type);
    clearstdin();
    printf("\e[F");
  } while(type!=0&&type!=1);
  tempPlayer[i].type = type==0?Humain:CPU;
  printf("\n\e[F");
  for (int j = 0; j < w/2-7; j++) printf(" ");
  printf(RESET"\e[2KType . . . .: ""\e[2m""%d\n",type);
  item->name[0] = '\0';
  strcat(item->name,i==0?"Saisie du Joueur 1 "RESET"["GRN" OK "RESET"]":"Saisie du Joueur 2 "RESET"["GRN" OK "RESET"]");
  tempPlayerValid[i%2] = 1;
  if (tempPlayerValid[0] && tempPlayerValid[1]) {
    m->items[0]->is_hide = 0;
  } else {
    m->items[0]->is_hide = 1;
  }

  sleepcp(500);

  resetTerm();

  printf(RESET);

  return MENU_NO_MESSAGE;
}

/**
 * (utilisé par menu.h comme pointeur fonction (@see menu.h))
 * menu_savemanager : permet d'afficher le sous-menu qui permet de charger une sauvegarde
 *
 * @Event {menu_event} (fonction appelé lors du choix d'un item dans le menu) (@see menu.h) (les arguments sont décrits dans menu.h et ce qui est retourné)
 *
 * @param {int} i - 0 => joueur 1, 1 => joueur 2
 */
menu_message menu_savemanager(menu* m, menu_item* item, int i) {
  resetTerm();
  int isloaded = savemanager_menu(game);

  // Si le joueur à charger une partie, on quitte le menu
  if (isloaded) {
    print_loader(200+rand()%500); // Juste de la déco
    resetTerm();
    return 1;
  }

  // Sinon on reste dans le menu
  resetTerm();
  return MENU_NO_MESSAGE;
}

/**
 * saisieMovForced : on demande de joueur de choisir le mouvement qu'il veut faire parmi les mouvements qu'il est obligé de faire
 *
 * @param {jeu*} g - le jeu
 * @param {position} p - la position de la pièce que le joueur est forcé de jouer
 * @param {position***|bucket_array} ba - le tableau qui contient la liste des coups possibles depuis cette position
 * @param {int} size - la taille du tableau ba
 *
 * @return {position**|bucket_array_item} retourne un mouvement (un tableau qui contient des pointeurs qui pointent vers des positions)
 */
bucket_array_item saisieMovForced(jeu* g,position p,bucket_array ba, int size) {

  resetTerm();
  undoHighlight(g);
  print_damier(g);

  char *colorcurrent = malloc((strlen(g->jCourant%2==0?BRED:BBLU)+1)*sizeof(char));
  colorcurrent[0] = '\0';
  strcat(colorcurrent,g->jCourant%2==0?BRED:BBLU);

  char *colorcurrentb = malloc((strlen(g->jCourant%2==0?RED:BLU)+1)*sizeof(char));
  colorcurrentb[0] = '\0';
  strcat(colorcurrentb,g->jCourant%2==0?RED:BLU);

  hightlight_bucket_array(g,ba,size);
  doSelect(g, p);

  resetTerm();
  print_damier(g);

  center_text_damier(g);
  printf("==== %sCHOIX DU MOUVEMENT"RESET" =====\n",colorcurrent);
  center_text_damier(g);
  printf("  \e[3mSaisissez \"1\" pour choisir le mouvement (1)\n  Saisissez \"2\" pour choisir le mouvement (2), etc..\e[0m\n\n\n");
  for (int i = 0; i < size; i++) printf("\n");
  center_text_damier(g);
  printf("==============================\n");

  int choice_move = 0;
  int errk = 0;
  while (1) {

    if (errk) {
	    center_text_damier(g);
      printf("%s\e[2K\e[F\e[F  CE MOUVEMENT N'EXISTE PAS"RESET"\n\n",colorcurrent);
    } else {
      //printf("\e[2K\n\n");
    }

    for (int i = 0; i < size+3; i++) printf("\e[F");

    for (int i = 0; i < size; i++) {
	  center_text_damier(g);
      printf("\e[2K - "BGRN"Mouvement"RESET" "GRN"(%d)"RESET"\n",i+1);
    }
    
	  center_text_damier(g);
    printf("\e[2K  \e[1mSaisissez le numéro du mouvement : %s\e[0m",colorcurrentb);
    scanf("%d",&choice_move);
    fflush(stdin);
    clearstdin();

    if (choice_move > 0 && choice_move <= size) {
      break;
    }

    errk = 1;

    printf("\n\n");
  }
  
  resetTerm();
  undoHighlight(g);
  print_damier(g);

  free(colorcurrent);
  free(colorcurrentb);

	return free_bucket_array_expectone(ba, size, choice_move-1);
}

/**
 * saisieMov : permet de faire saisir un mouvement par le joueur
 *   - le joueur choisit une pièce à bouger
 *   - le joueur choisit ensuite le mouvement à faire parmi la liste de mouvement disponible depuis la poèce qu'il a choisi
 *
 * @param {jeu*} g - le jeu
 *
 * @return {bucket_array_item} retourne un bucket_array_item (un tableau de pointeurs qui pointent vers des positions) (correspond à un mouvement)
 */
bucket_array_item saisieMov(jeu* g){

  resetTerm();
  undoHighlight(g);
  print_damier(g);

  char *colorcurrent = malloc((strlen(g->jCourant%2==0?BRED:BBLU)+1)*sizeof(char));
  colorcurrent[0] = '\0';
  strcat(colorcurrent,g->jCourant%2==0?BRED:BBLU);

  char *colorcurrentb = malloc((strlen(g->jCourant%2==0?RED:BLU)+1)*sizeof(char));
  colorcurrentb[0] = '\0';
  strcat(colorcurrentb,g->jCourant%2==0?RED:BLU);

  center_text_damier(g);
  printf("====== %sCHOIX DE LA PIECE"RESET" =====\n\n\n\n",colorcurrent);
  center_text_damier(g);
  printf("==============================\n");
  int x = -1, y = -1;
  int k = 0;
  bucket_array ba = NULL;
  int size;
  int iscatch;
  while (1) {
    printf("\e[F\e[F\e[F\e[F");
    if (k) {
	  center_text_damier(g);
      printf("\n\n\e[2K  %sAUCUN COUP DISPONIBLE DEPUIS CETTE POSITION\e[F\e[F"RESET,colorcurrent);
    }
	center_text_damier(g);
    printf("\e[2K  \e[3mSaisissez \"3 5\" pour choisir la pièce en ligne 3, colonne 5\e[0m\n");
	center_text_damier(g);
    printf("\e[2K  \e[1mSaisissez les coordonnées : %s\e[0m",colorcurrentb);
    scanf("%d %d",&y,&x);
    fflush(stdin);
    clearstdin();
    printf(RESET"\n\n");

    
    //ba = getMoveSansPrise(g, (position){y,x}, &size);
    ba = getMove(g,(position){y,x}, &size, &iscatch);

    
    if (ba != NULL) {
      break;
    }

    k++;

  }
  
  hightlight_bucket_array(g,ba,size);
  doSelect(g, (position){y,x});

  resetTerm();
  print_damier(g);

  center_text_damier(g);
  printf("==== %sCHOIX DU MOUVEMENT"RESET" =====\n",colorcurrent);
  center_text_damier(g);
  printf("  \e[3mSaisissez \"1\" pour choisir le mouvement (1)\n");
  center_text_damier(g);
  printf("  Saisissez \"2\" pour choisir le mouvement (2), etc..\e[0m\n\n\n");
  for (int i = 0; i < size; i++) printf("\n");
  center_text_damier(g);
  printf("==============================\n");

  int choice_move = 0;
  int errk = 0;
  while (1) {

    if (errk) {
	  center_text_damier(g);
      printf("%s\e[2K\e[F\e[F  CE MOUVEMENT N'EXISTE PAS"RESET"\n\n",colorcurrent);
    } else {
      //printf("\e[2K\n\n");
    }

    for (int i = 0; i < size+3; i++) printf("\e[F");

    for (int i = 0; i < size; i++) {
	  center_text_damier(g);
      printf("\e[2K - "BGRN"Mouvement"RESET" "GRN"(%d)"RESET"\n",i+1);
    }
    
	center_text_damier(g);
    printf("\e[2K  \e[1mSaisissez le numéro du mouvement : %s\e[0m",colorcurrentb);
    scanf("%d",&choice_move);
    fflush(stdin);
    clearstdin();

    if (choice_move > 0 && choice_move <= size) {
      break;
    }

    errk = 1;

    printf("\n\n");
  }
  
  //resetTerm();
  undoHighlight(g);
  //print_damier(g);

  free(colorcurrent);
  free(colorcurrentb);

  return free_bucket_array_expectone(ba, size, choice_move-1);
}

/**
 * saisiePiece : permet de choisir une pièce parmi une liste de pièces
 *
 * @TODO renommer la variable move qui n'est pas intuitive
 *
 * @param {jeu*} g - le jeu
 * @param {position*} move - la liste des pièces
 * @param {int} size - la taille du tableau move
 *
 * @return {position} retourne la position de la pièce choisie
 */
position saisiePiece(jeu*g,position* move,int size) {

  char *colorcurrent = malloc((strlen(g->jCourant%2==0?BRED:BBLU)+1)*sizeof(char));
  colorcurrent[0] = '\0';
  strcat(colorcurrent,g->jCourant%2==0?BRED:BBLU);

  char *colorcurrentb = malloc((strlen(g->jCourant%2==0?RED:BLU)+1)*sizeof(char));
  colorcurrentb[0] = '\0';
  strcat(colorcurrentb,g->jCourant%2==0?RED:BLU);

  doSelectMultiple(g,move,size);
  resetTerm();
  print_damier(g);

  center_text_damier(g);
  printf("===== %sCHOIX DE LA PIECE"RESET" =====\n",colorcurrent);
  center_text_damier(g);
  printf("  \e[3mSaisissez \"1\" pour choisir la pièce (1)\n");
  center_text_damier(g);
  printf("  Saisissez \"2\" pour choisir la pièce (2), etc..\e[0m\n\n\n");
  for (int i = 0; i < size; i++) printf("\n");
  center_text_damier(g);
  printf("==============================\n");

  int choice_piece = 0;
  int errk = 0;
  while (1) {

    if (errk) {
	    center_text_damier(g);
      printf("%s\e[2K\e[F\e[F  CETTE PIECE N'EXISTE PAS"RESET"\n\n",colorcurrent);
    } else {
      //printf("\e[2K\n\n");
    }

    for (int i = 0; i < size+3; i++) printf("\e[F");

    for (int i = 0; i < size; i++) {
	  center_text_damier(g);
      printf("\e[2K - "BGRN"Pièce"RESET" "GRN"(%d)"RESET"\n",i+1);
    }

	  center_text_damier(g);
    printf("\e[2K  \e[1mSaisissez le numéro de la pièce : %s\e[0m",colorcurrentb);
    scanf("%d",&choice_piece);
    fflush(stdin);
    clearstdin();

    if (choice_piece > 0 && choice_piece <= size) {
      break;
    }

    errk = 1;

    printf("\n\n");
  }

  free(colorcurrent);
  free(colorcurrentb);

  return move[choice_piece-1];
}


void menu_show_choice_game() {
  menu* m = menu_create();
  menu_item* item1 = menu_create_item("JOUER UN COUP");
  menu_item* item2 = menu_create_item("ABANDONNER");
  menu_item* item3 = menu_create_item("DEMANDER L'EGALITE");
  menu_item* item4 = menu_create_item("SAUVEGARDER LA PARTIE");
  menu_item* item5 = menu_create_item("QUITTER LA PARTIE SANS SAUVEGARDER");
  menu_item* item6 = menu_create_item("QUITTER ET SAUVEGARDER");
  menu_insert_item(m, item1);
  menu_insert_item(m, item2);
  menu_insert_item(m, item3);
  menu_insert_item(m, item4);
  menu_insert_item(m, item5);
  menu_insert_item(m, item6);

  menu_item_choice choice = print_menu(m,0,8,2,BBLU,BRED);

  free_menu(m);
}

bucket_array_item get_stdin_move(jeu*g,couleur c, int* iscatch) {

  bucket_array_item move = NULL;

  int testsizebestmove = 0;
  position* testpbestmove = getBestMoveColor(g,c,&testsizebestmove);

  int iscatchbasize = 0;

  // On force le joueur à faire un mange (ou une rafle) s'il y en a une
  if (testsizebestmove > 0) {
    position pchoose = testsizebestmove > 1 ? saisiePiece(g,testpbestmove,testsizebestmove) : testpbestmove[0];
    int basize = 0;
    bucket_array ba = getMove(g, pchoose, &basize, &iscatchbasize);
    move = saisieMovForced(g,pchoose,ba,basize);
  }
  
  // Sinon, on lui laisse le choix
  else {
    move = saisieMov(g);
  }

  if (testpbestmove!=NULL) free(testpbestmove);

  *iscatch = iscatchbasize;

  return move;
}

/*
 * joueTour : permet de se faire dérouler une partie
 *
 * @param {jeu*} g - le jeu où se déroule la partie
 *
 * @return {int} {not used}
 */
int joueTour(jeu*g){
	
  couleur c;
  int isGameOver = 0;
  int isGameEquality = 0;
  
  resetTerm();
  print_damier(g);

  while(1) {

    c = g->jCourant%2==0?BLANC:NOIR;

    isGameOver = gameOver(g,c);
    if (isGameOver) {
      break;
    }

    bucket_array_item move = NULL;
    int iscatchbasize = 0;
	
    // Si le joueur est Humain, on lui affiche un menu :
    if (g->tabJ[g->jCourant].type==Humain) {
      menu_show_choice_game();
      move = get_stdin_move(g,c,&iscatchbasize); // On recupère le mouvement que le joueur veut faire
    }
    
    // Sinon le joueur n'est pas Humain :
    else {
      move = get_random_move(g,c,&iscatchbasize); // On recupère un mouvement aléatoire
    }



    doMove(g,move);

    isGameEquality = gameEquality(g,getTypePiece(g,(position){move[0]->lig,move[0]->col}),iscatchbasize);
    if (isGameEquality>0) {
      break;
    }

    free_bucket_array_item(move);

    promotion(g);

    resetTerm();
    print_damier(g);

    g->jCourant = 1-g->jCourant;

  }
  if(isGameOver == 1){
    print_splashscreen_GO(c==BLANC?1:0);
    afficheGameOver(g,c);
  }else if(isGameEquality > 0){
    affichageEquality(g,isGameEquality);
  } 
  
  return 0;

}

/**
 * (utilisé par menu.h comme pointeur fonction (@see menu.h))
 * menu_credits : permet d'afficher les crédits
 *
 * @Event {menu_event} (fonction appelé lors du choix d'un item dans le menu) (@see menu.h) (les arguments sont décrits dans menu.h et ce qui est retourné)
 */
menu_message menu_credits(menu* m, menu_item* item, int i) {
  resetTerm();
  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);
  for (int i = 0; i < h/2; i++) printf("\n");
  for (int i = 0; i < w/2-19; i++) printf(" ");
  printf("Créé par "BRED"BRIOT Lemmy"RESET" et "BBLU"DUCHEMIN Maêl\n"RESET);
  getch();
  resetTerm();
  return MENU_NO_MESSAGE;
}

/**
 * menu_show : permet d'afficher le menu
 */
void menu_show() {

  //================ MENU SAISI JOUEUR ===============
  menu* mcreateplayer = menu_create();
  menu_item* playerlaunchgame = menu_create_item("LANCER LA PARTIE");
  playerlaunchgame->is_hide = 1;
  menu_item* player1 = menu_create_item("Saisie du Joueur 1 "RESET"["YEL"VIDE"RESET"]");
  menu_item* player2 = menu_create_item("Saisie du Joueur 2 "RESET"["YEL"VIDE"RESET"]");
  player1->length = 25;
  player2->length = 25;
  player1->_fn = menu_get_player;
  player2->_fn = menu_get_player;
  menu_insert_item(mcreateplayer, playerlaunchgame);
  menu_insert_item(mcreateplayer, player1);
  menu_insert_item(mcreateplayer, player2);
  menu_insert_separator(mcreateplayer);
  menu_insert_item(mcreateplayer, menu_create_back("RETOUR"));
  //============= End: MENU SAISI JOUEUR ==============


  //================ SOUS-MENU "JOUER" ================
  menu* m2 = menu_create();
  menu_insert_item(m2, menu_create_item_submenu("NOUVELLE PARTIE",mcreateplayer));
  menu_item* item_savemanager = menu_create_item("CHARGER UNE PARTIE");
  item_savemanager->_fn = menu_savemanager;
  menu_insert_item(m2, item_savemanager);
  menu_insert_separator(m2);
  menu_insert_item(m2, menu_create_back("RETOUR"));
  //============== End: SOUS-MENU "JOUER" =============



  //================ SOUS-MENU "OPTION" ===============
  menu* m3 = menu_create();
  menu_item* option1 = menu_create_option("NOMBRE DE LIGNES", (void*)&nb_lin, MENU_INT);
  menu_item* option2 = menu_create_option("NOMBRE DE COLONNES", (void*)&nb_col, MENU_INT);
  menu_item* option3 = menu_create_option("NOMBRE DE LIGNES DE PIONS", (void*)&nb_plin, MENU_INT);
  menu_insert_item(m3, option1);
  menu_insert_item(m3, option2);
  menu_insert_item(m3, option3);
  menu_insert_separator(m3);
  menu_insert_item(m3, menu_create_back("RETOUR"));
  //============= End: SOUS-MENU "OPTION" =============



  //================== MENU PRINCIPAL =================
  menu* m = menu_create();
  menu_item* item1 = menu_create_item("JOUER");
  item1->submenu = m2;
  menu_item* item2 = menu_create_item("OPTIONS");
  item2->submenu = m3;
  menu_item* item3 = menu_create_item("CREDITS");
  item3->_fn = menu_credits;
  menu_item* item4 = menu_create_item("QUITTER");
  menu_insert_item(m, item1);
  menu_insert_item(m, item2);
  menu_insert_item(m, item3);
  menu_insert_item(m, item4);
  //=============== End: MENU PRINCIPAL ===============

  int index = 0;

  while(1){

    // On rend invisible le curseur (pour que ce soit un peu plus jolie)
    display_hide_cursor();

    // On attend que l'utilisateur fasse un choix
    menu_item_choice choice = print_menu(m,index,8,2,BBLU,BRED);

    // On réaffiche le curseur (on veut que le curseur soit visible pour la saisi au clavier)
    display_show_cursor();
    
    // Si le joueur lance une nouvelle partie :
    if (choice.m==mcreateplayer&&choice.index==0) {

      // On initialise le jeu
      init(game, nb_lin, nb_col, nb_plin);

      // On met dans la variable "game" le nom des joueurs
      menu_set_player(game);

      // On lance la partie
      joueTour(game);

    }

    // Sinon il charge une partie sauvegarder
    else if (choice.m==m2&&choice.index==1) {

      // La sauvegade a déjà été chargé (dans la fonction menu_savemanager qui se trouve dans ce fichier)


      // On a juste a lancé la partie
      joueTour(game);

    }

    // Sinon il quitte le jeu :'(
    else if (choice.m==m&&choice.index==3) {
      break;
    }

  }
  
  // On libère entièrement tout le menu
  free_menu(m);
}

/**
 * game_run : permet de lancer le jeu
 *
 * @alias menu_show
 */
void game_run() {
  menu_show();
}

#endif