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

#ifndef CHECKERS_BUCKET_EXPLORER_H
#define CHECKERS_BUCKET_EXPLORER_H


/**

  /!\ TOUTES LES FONCTIONS ONT ETE TESTEE /!\

      - test no memory leak     : succès
      - test résultat fonctions : succès

*/



/**********************\
*                      *
*       INCLUDE        *
*                      *
\**********************/

#include <time.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "colors.h"
#include "structures.h"
#include "game_play.h"






/**********************\
*                      *
*        STRUCT        *
*                      *
\**********************/

/**
 * structure qui permet de contenir toutes les rafles disponible depuis une position sur le plateau
 */
typedef struct bucket_explorer{

  // ,----------,          ,----------,
  // |          |          |          |
  // |  up_left |          | up_right |
  // |          |          |          |
  // '----------+----------+----------'
  //            |          |
  //            |   pos    |
  //            |position* |
  // ,----------+----------+----------,
  // |          |          |          |
  // |down_left |          |down_right|
  // |          |          |          |
  // '----------'          '----------'

  // Tableau qui contient les positions atteignables possibles lors d'une rafle
  // Un pion la seula case atteignable pour un mange valide est la case juste après
  // la pièce à capturer
  // TANDIS QUE pour une dame, toutes les cases vides après la pièce à capturer sont valides
  // (d'où un tableau qui représente les cases atteignables sur une même diagonale)

  struct bucket_explorer* up_left;
  int len_up_left;

  struct bucket_explorer* up_right;
  int len_up_right;

  struct bucket_explorer* down_left;
  int len_down_left;

  struct bucket_explorer* down_right;
  int len_down_right;

  position *pos;

} bucket_explorer;

// Pour pas se perdre avec toutes les étoiles
typedef position*** bucket_array;
typedef position** bucket_array_item;







/**********************\
*                      *
*       FUNCTIONS      *
*                      *
\**********************/

/**
 * create_bucket_explorer : permet de créer proprement un nouveau bucket_explorer
 *
 * @return {bucket_explorer*} retourne le bucket_explorer
 */
bucket_explorer* create_bucket_explorer() {
  bucket_explorer *b = (bucket_explorer*)calloc(1,sizeof(bucket_explorer));
  return b;
}


/**
 * create_array_bucket_explorer : permet de créer proprement un tableau de bucket_explorer
 *
 * @param {int} size - taille du tableau à créer
 *
 * @return {bucket_explorer*} retourne un tableau de taille size de bucket_explorer
 */
bucket_explorer* create_array_bucket_explorer(int size) {
  bucket_explorer *b = (bucket_explorer*)calloc(size,sizeof(bucket_explorer));
  return b;
}

/**
 * free_bucket_array_item : permet de libérer un bucket_array_item (position**))
 *
 * @param {bucket_array_item} p - le bucket_array_item à libérer
 */
void free_bucket_array_item(bucket_array_item p) {
  if (p==NULL) return;
  int kr = 0;
  while (p[kr]!=NULL) {
    free(p[kr]);
    kr++;
  }
  free(p);
}


/**
 * @private
 * free_bucket_explorer_ : permet de proprement libérer un bucket_explorer
 * (fonction récursive "temporaire" "privé")
 *
 * @param {bucket_explorer*} b - le bucket_explorer à libérer
 */
static void free_bucket_explorer_(bucket_explorer* b) {
  if (b != NULL) {
    if (b->pos != NULL) free(b->pos);
    
    for (int i = 0; i < b->len_up_left; i++) {
      free_bucket_explorer_(&b->up_left[i]);
    }
    for (int i = 0; i < b->len_up_right; i++) {
      free_bucket_explorer_(&b->up_right[i]);
    }
    for (int i = 0; i < b->len_down_left; i++) {
      free_bucket_explorer_(&b->down_left[i]);
    }
    for (int i = 0; i < b->len_down_right; i++) {
      free_bucket_explorer_(&b->down_right[i]);
    }
    
    free(b->up_left);
    free(b->up_right);
    free(b->down_left);
    free(b->down_right);
    
  }
}

/**
 * free_bucket_explorer : permet de proprement libérer un bucket_explorer
 *
 * @param {bucket_explorer*} b - le bucket_explorer à libérer
 */
void free_bucket_explorer(bucket_explorer* b) {
  if (b != NULL) {
    free_bucket_explorer_(b);

    free(b);
  }
}

/**
 * is_bucket_empty : permet de savoir si un bucket_explorer est vide ou pas
 * On considère qu'un bucket_explorer est vide s'il contient seulement une position ou s'il ne pointe nulle part
 *
 * @param {bucket_explorer*} b - le bucket_explorer à tester
 *
 * @return {int} retourne 1 si le bucket_explorer est video sinon 0 (le bucket_explorer contient un mange valide)
 */
int is_bucket_empty(bucket_explorer* b) {
  if (b == NULL) return 1;
  if (b->up_left==NULL && b->up_right==NULL && b->down_left==NULL && b->down_right==NULL) return 1;
  if (b->pos == NULL) return 1;
  return 0;
}

// Simple déclaration
void explore_lookat(jeu* g, bucket_explorer* b, position p, couleur c, typePiece typepiece);

/**
 * @private
 * explore_lookat_direction_pion : permet d'explorer le plateau dans toutes les directions lorsqu'il s'agit d'un pion
 *
 * @param {jeu*} g - le jeu (on doit avoir accès au plateau pour savoir si un mange est valide)
 * @param {bucket_explorer*} b - le bucket_explorer courrant (là où on se trouve récursivement)
 * @param {position} p - la position courrante
 * @param {couleur} c - la couleur du pion de départ (on doit savoir à tout instant si on peut manger ou pas)
 * @param {typePiece} typepiece - on doit aussi propager le type de la pièce car la fonction explore_lookat fonctionne aussi pour les dames
 * @param {int} dx - 1 si on se dirige à droite, -1 si on se dirige à gauche
 * @param {int} dy - 1 si on se dirige en bas, -1 si on se dirige à droite
 *
 */
static void explore_lookat_direction_pion(jeu* g, bucket_explorer* b, position p, couleur c, typePiece typepiece, int dx, int dy) {

  position q = posVoisine(p,dx,dy);
  int direction = dx < 0 && dy < 0 ? 0 : dx > 0 && dy < 0 ? 1 : dx < 0 && dy > 0 ? 2 : 3;
  position test = isMangePionValideTemp(g,p,q);

  if (isValide(g,q) && test.lig!=-1 && test.col!=-1 && getCouleur(g,test)!=c) {
    switchCapture(g,test);

    switch (direction) {
      case 0:
        b->up_left = create_bucket_explorer();
        b->len_up_left++;
        explore_lookat(g,b->up_left,q,c,typepiece);
        break;
      case 1:
        b->up_right = create_bucket_explorer();
        b->len_up_right++;
        explore_lookat(g,b->up_right,q,c,typepiece);
        break;
      case 2:
        b->down_left = create_bucket_explorer();
        b->len_down_left++;
        explore_lookat(g,b->down_left,q,c,typepiece);
        break;
      case 3:
        b->down_right = create_bucket_explorer();
        b->len_down_right++;
        explore_lookat(g,b->down_right,q,c,typepiece);
        break;
    }

  }

}


/**
 * explore_get_next_piece : permet de récupérer la pièce qu'une dame peut capturer suivant une diagonale (indiqué par dx et dy) et renvoie le nombre de cases libres après la pièce capturée
 *
 * @info Uniquement pour les dames
 *
 * @param {jeu*} g - le jeu
 * @param {position} p - la position de la dame
 * @param {couleur} c - la couleur de la dame
 * @param {int} dx - 1 si on se dirige à droite, -1 si on se dirige à gauche
 * @param {int} dy - 1 si on se dirige en bas, -1 si on se dirige à droite
 * @param {int*} size - le nombre de cases atteignables après la pièce capturée
 *
 * @return {position} retourne la position de la pièce prise si la dame peut manger sur la diagonale, sinon (position){-1,-1} est retournée
 */
position explore_get_next_piece(jeu* g, position p, couleur c, int dx, int dy, int *size) {
  
  position q = p;
  position capture; // contiendra la position de la pièce que la dame peut capturer
  int found = 0; // s'il y a une pièce à capturer
  int recursion = 0; // nombre de cases libres après la pièce capturée

  // On cherche une pièce adverse sur la diagonal
  while (1) {
    q = posVoisine(q,dx,dy); // On se déplace sur la diagonale de case en case

    // Dès qu'on croise une pièce on quitte la boucle
    if (!isValide(g,q)) break;
    if (isCapture(g,q)) break;
    if (getCouleur(g,q)==c) break;
    if (getCouleur(g,q)!=c && getTypePiece(g,q) != NOTYPE) {found = 1;break;}

   }

  // On regarde s'il y a une place de libre derrière
  if (found) {
    capture = q;
    while (1) {
      q = posVoisine(q,dx,dy); // On se déplace sur la diagonale de case en case

      // Si la case est vide alors la dame peut atteindre la case
      if (isValide(g,q) && g->plateau[q.lig*g->NB_Col+q.col] == VIDE) {
        recursion++; // On compte le nombre de cases vides
      }
      
      // Sinon la case n'est pas vide, les cases après ne sont plus atteignables
      else {
        break;
      }

    }
  }

  // Si on peut manger et qu'il y a des cases libres derrières
  if (recursion) {
    *size = recursion;
    return capture;
  }
  
  // Sinon la dame ne peut pas manger sur cette diagonale
  else {
    *size = 0;
    return (position){-1,-1};
  }

}

/**
 * explore_get_next_piece : permet de récupérer le nombre de cases libres sur la diagonale avant de rencontrer une case qui n'est pas vide, elle retourne la position de la case non-vide
 *
 * @info Uniquement pour les dames
 *
 * @param {jeu*} g - le jeu
 * @param {position} p - la position de la dame
 * @param {int} dx - 1 si on se dirige à droite, -1 si on se dirige à gauche
 * @param {int} dy - 1 si on se dirige en bas, -1 si on se dirige à droite
 * @param {int*} size - le nombre de cases vides avant de rencontrer une case non-vide
 *
 * @return {position} retourne la position de la pièce non-vide, cette fonction met dans *size le nombre de cases vides sur la diagonale avant de rencontrer une case non-vide
 */
position explore_get_next_piece_sans_prise(jeu* g, position p, int dx, int dy, int *size) {
  
  position q = p;
  int found = 0;
  // on cherche une pièce adverse sur la diagonal
  int i = 0;
  while (1) {
    q = posVoisine(q,dx,dy); // On se déplace sur la diagonale de case en case

    // Si on sort du plateau, alors on a fini de compter les cases vides sur la diagonale
    // Si p!=q alors on a trouvé au minimum une case vide
    if (!isValide(g,q) && !equals(p,q)) {found = 1;break;}

    // Si on sort du plateau et que p==q alors on a trouvé aucune case vide
    // (ce cas n'arrivera jamais)
    if (!isValide(g,q)) break;

    // Si on croise une pièce capturée (ce qui est impossible car on ne fait pas une rafle) => on quitte
    if (isCapture(g,q)) break;

    // Si on trouve une case non-vide
    if (getTypePiece(g,q) != NOTYPE) {found = 1;break;}
    i++;
  }

  if (found) {
    *size = i;
    return q;
  } else {
    *size = 0;
    return (position){-1,-1};
  }
 
}

/**
 * position_array_copy : permet de copier un bucket_array_item dans un bucket_array_item
 *
 * @param {position**|bucket_array_item} src - le tableau source
 * @param {position**|bucket_array_item} dst - le tableau de destination
 */
void position_array_copy(position **src, position **dst) {
  int i = 0;
  while (src[i]!=NULL) {
    dst[i] = malloc(sizeof(position));
    dst[i]->lig = src[i]->lig;
    dst[i]->col = src[i]->col;
    i++;
  }
  dst[i] = NULL; // les bucket_array_item fonctionnent de la même façon que les chaines de caractères, le dernier élément est NULL (ce qui permet de ne pas se trimballer partout la taille du tableau)
}

/**
 * print_bucket_array : permet d'afficher sur la console le contenu d'un bucket_array
 *
 * @param {bucket_array} ab - le tableau qui contient des bucket_array_item
 * @param {int} size - taille du tableau (contrairement aux bucket_array_item qui n'ont pas de taille, un bucket_array est un tableau classique)
 */
void print_bucket_array(bucket_array ab, int size) {
  for (int i = 0; i < size; i++) {
    int j = 0;
    printf("MOVE %d : ",i);
    while (ab[i][j]!=NULL) {
      printf(ab[i][j+1]==NULL?"%d %d":"%d %d -> ",ab[i][j]->lig,ab[i][j]->col);
      j++;
    }
    printf("\n");
  }
}

/**
 * free_bucket_array : permet de libérer proprement un bucket_array
 *
 * @param {bucket_array} ab - le tableau qui contient des bucket_array_item
 * @param {int} size - taille du tableau (contrairement aux bucket_array_item qui n'ont pas de taille, un bucket_array est un tableau classique)
 */
void free_bucket_array(bucket_array ab, int size) {
  if (ab == NULL) return;
  for (int i = 0; i < size; i++) {
    int j = 0;
    while (ab[i][j]!=NULL) {
      free(ab[i][j]);
      j++;
    }
    free(ab[i]);
  }
  free(ab);
}

/**
 * free_bucket_array_expectone : permet de libérer proprement un bucket_array sauf un bucket_array_item qui sera retourner
 *
 * @param {bucket_array} ab - le tableau qui contient des bucket_array_item
 * @param {int} size - taille du tableau (contrairement aux bucket_array_item qui n'ont pas de taille, un bucket_array est un tableau classique)
 * @param {int} index - indice de l'élément qu'on ne veut pas libérer
 *
 * @return {position**|bucket_array_item} retourne le bucket_array_item qu'on ne veut pas libérer
 */
position** free_bucket_array_expectone(bucket_array ab, int size, int index) {
  if (ab == NULL) return NULL;
  position** r = NULL;
  for (int i = 0; i < size; i++) {
    if (i==index) {
      r = ab[i];
      continue;
    }
    int j = 0;
    while (ab[i][j]!=NULL) {
      free(ab[i][j]);
      j++;
    }
    free(ab[i]);
  }
  free(ab);
  return r;
}

/**
 * @private
 * explore_lookat_direction_dame : permet d'explorer le tableau depuis le point de vue d'une dame
 *
 * @param {jeu*} g - le jeu
 * @param {bucket_explorer*} b - le bucket d'exploration
 * @param {position} p - la position courrante de où on se trouve sur le plateau
 * @param {couleur} c - la couleur de la dame
 * @param {typePiece} typepiece - le type de la pièce (on sait que c'est une dame)
 * @param {int} dx - 1 si on se dirige à droite, -1 si on se dirige à gauche
 * @param {int} dy - 1 si on se dirige en bas, -1 si on se dirige à droite
 */
static void explore_lookat_direction_dame(jeu* g, bucket_explorer* b, position p, couleur c, typePiece typepiece, int dx, int dy) {

  int size; // contiendra le nombre de cases vides après la case à capturer

  // On récupère la pièce à captuer sur cette diagonale
  position target = explore_get_next_piece(g, p, c, dx, dy, &size);

  // S'il y a bien une case à manger sur la diagonale
  if (target.lig!=-1 && target.col!=-1) {

    // Selon les règles, on ne peut pas passer plusieurs fois sur une même pièce,
    // donc si on l'a déjà capturer alors on ne peut plus passer par là
    if (isCapture(g,target)) return;

    // Pour construire correctement le bucket d'exploration, il faut savoir dans quelle direction on va suivant dx et dy (pour chaque couple de valeur, on associe un nombre)
    //   -1 -1 => 0 (up_left)
    //    1 -1 => 1 (up_right)
    //   -1  1 => 2 (down_left)
    //    1  1 => 3 (down_right)
    int direction = dx < 0 && dy < 0 ? 0 : dx > 0 && dy < 0 ? 1 : dx < 0 && dy > 0 ? 2 : 3;

    // On regarde les cases vides (donc atteignables) après la case capturée
    position q = target;

    // Comme on passe par là, on capture la pièce (pour ne pas plus tard repasser sur cette pièce) (comme explore_get_next_piece a retourné une position valide alors on sait que la pièce est de la couleur adverse)
    switchCapture(g,target);

    // On regarde toutes les cases vides après la case capturée
    while (1) {

      q = posVoisine(q,dx,dy); // On se déplace sur la diagonale de case en case

      // Si on sort du plateau alors on peut plus explorer (on ne va pas explorer sur des cases qui n'existent pas)
      if (!isValide(g,q)) break;

      // Si on trouve une case non-vide alors on ne peut pas aller plus loin
      if (g->plateau[q.lig*g->NB_Col+q.col] != VIDE) break;

      // Suivant la direction, on va constuire différemment le bucket d'exploration
      switch (direction) {

        // Si on se dirige en haut à gauche
        case 0:
          if (b->up_left == NULL) b->up_left = create_array_bucket_explorer(size);
          b->len_up_left++;

          // On explore depuis cette case vide (exploration récursive)
          explore_lookat(g,&b->up_left[b->len_up_left-1],q,c,typepiece);
          break;

        // Si on se dirige en haut à droite
        case 1:
          if (b->up_right == NULL) b->up_right = create_array_bucket_explorer(size);
          b->len_up_right++;

          // On explore depuis cette case vide (exploration récursive)
          explore_lookat(g,&b->up_right[b->len_up_right-1],q,c,typepiece);
          break;

        // Si on se dirige en bas à gauche
        case 2:
          if (b->down_left == NULL) b->down_left = create_array_bucket_explorer(size);
          b->len_down_left++;

          // On explore depuis cette case vide (exploration récursive)
          explore_lookat(g,&b->down_left[b->len_down_left-1],q,c,typepiece);
          break;

        // Si on se dirige en bas à droite
        case 3:
          if (b->down_right == NULL) b->down_right = create_array_bucket_explorer(size);
          b->len_down_right++;

          // On explore depuis cette case vide (exploration récursive)
          explore_lookat(g,&b->down_right[b->len_down_right-1],q,c,typepiece);
          break;
      }
    }
  }
}

/**
 * explore_save_state : permet de copier une variable de type jeu
 *
 * @param {jeu*} g - le jeu
 *
 * @return {jeu*} retorune un pointeur vers la copie du jeu *g
 */
jeu* explore_save_state(jeu *g) {
  jeu* g_ = calloc(1,sizeof(jeu));
  g_->NB_Col = g->NB_Col;
  g_->NB_Lig = g->NB_Lig;
  g_->jCourant = g->jCourant;
  g_->plateau = calloc(1,g->NB_Lig * g->NB_Col * sizeof (int));
  g_->plateau_s = NULL;
  g_->jeux = NULL;
  g_->NB_LigInit = g->NB_LigInit;
  for (int y = 0; y < g_->NB_Lig; y++) {
    for (int x = 0; x < g_->NB_Col; x++) {
      g_->plateau[y*g_->NB_Col+x] = g->plateau[y*g_->NB_Col+x];
    }
  }
  return g_;
}

/**
 * explore_restore_state : permet de libérer un jeu copié par la fonction explore_save_state
 *
 * @param {jeu*} g - le jeu à libérer
 */
void explore_restore_state(jeu *g) {
  if (g != NULL) {
    free(g->plateau);
    free(g);
  }
}

/**
 * explore_lookat : permet d'explorer récursivement sur le plateau du jeu afin de récupérer toutes les rafles possibles depuis une position
 *
 * @param {jeu*} g - le jeu
 * @param {bucket_explorer*} b - le bucket d'exploration
 * @param {position} p - la position courrante
 * @param {couleur} c - la couleur de la pièce d'origine (au 1er appel de la fonction)
 * @param {typePiece} typepiece - le type de la pièce d'origine (au 1er appel de la fonction)
 */
void explore_lookat(jeu* g, bucket_explorer* b, position p, couleur c, typePiece typepiece) {

  // On construit récursivement notre bucket
  b->pos = (position*)malloc(sizeof(position));
  b->pos->lig = p.lig;
  b->pos->col = p.col;

  // On ne veut pas que les différentes explorations interfèrent entre elles
  // @TODO : améliorer ça en annulant les pièces capturés pour ne pas stocker des dizaines d'états de jeu
  jeu *g1 = explore_save_state(g);
  jeu *g2 = explore_save_state(g);
  jeu *g3 = explore_save_state(g);
  jeu *g4 = explore_save_state(g);

  // S'il s'agit d'un pion, on appelle la fonction explore_lookat_direction_pion
  if (typepiece == Pion) {

    // On met -2 -2, 2 -2, -2 2 et 2 2 car la seule case atteignable lors d'une mange avec
    // un pion est la case juste après la case capturée

    explore_lookat_direction_pion(g1, b, p, c, typepiece, -2, -2);
    explore_lookat_direction_pion(g2, b, p, c, typepiece,  2, -2);
    explore_lookat_direction_pion(g3, b, p, c, typepiece, -2,  2);
    explore_lookat_direction_pion(g4, b, p, c, typepiece,  2,  2);

  }
  
  // Sinon il s'agit d'une dame, dans ce cas, on eppelle la fonction explore_lookat_direction_dame
  else if (typepiece == Dame) {

    explore_lookat_direction_dame(g1, b, p, c, typepiece, -1, -1);
    explore_lookat_direction_dame(g2, b, p, c, typepiece,  1, -1);
    explore_lookat_direction_dame(g3, b, p, c, typepiece, -1,  1);
    explore_lookat_direction_dame(g4, b, p, c, typepiece,  1,  1);
    
  }

  // On libère les états
  explore_restore_state(g1);
  explore_restore_state(g2);
  explore_restore_state(g3);
  explore_restore_state(g4);

}

/**
 * explore : permet d'explorer tout les mouvements possibles depuis une position
 * cette fonction retourne un bucket_explorer qui contient toutes les info nécessaires
 *
 * @param {jeu*} g - le jeu
 * @param {position} p - la position p pour laquelle on veut explorer les rafles possibles
 *
 * @return {bucket_explorer*} retourne un bucket d'exploration
 */
bucket_explorer* explore(jeu *g, position p) {
  bucket_explorer *b = create_bucket_explorer(); // On créé un bucket
  explore_lookat(g,b,p,getCouleur(g,p),getTypePiece(g,p)); // On commece à explorer récursivement 
  undoCapture(g); // On explore juste, on ne veut pas manger donc on annule tout
  return b;
}

/**
 * @private
 * print_all_move_ : fonction invisible en dehors du fichier grâce au mot clé "static"
 * cette fonction prend en paramètre un bucket_explorer et une chaîne
 * cette fonction ne doit jamais être appelée
 *
 * @param {bucket_explorer*} b - le bucket que l'on veut afficher sur la console
 * @param {char*} previous - on construit une chaîne récursivement
 * @param {int*} nbprint - numéro du mouvement (le nombre de feuille qu'on a rencontré +1)
 */
static void print_all_move_(bucket_explorer* b, char *previous, int *nbprint) {
  if (b != NULL) {

    char str[10];
    sprintf(str, "%d", b->pos->lig);
    char str2[10];
    sprintf(str2, "%d", b->pos->col);


    char *n = malloc(strlen(str)+strlen(str2)+2);
    n[0] = '\0';
    strcat(n,str);
    strcat(n,",");
    strcat(n,str2);
    

    char *t = NULL;

    if (previous != NULL) {
      t = malloc(strlen(previous)+strlen(n)+5);
      t[0] = '\0';
      strcat(t,previous);
      strcat(t," -> ");
      strcat(t,n);
    } else {
       t = malloc(strlen(n)+1);
      t[0] = '\0';
      strcat(t,n);
    }
   

    free(n);

    for (int i = 0; i < b->len_up_left; i++) {
      print_all_move_(&b->up_left[i],t,nbprint);
    }
    for (int i = 0; i < b->len_up_right; i++) {
      print_all_move_(&b->up_right[i],t,nbprint);
    }
    for (int i = 0; i < b->len_down_left; i++) {
      print_all_move_(&b->down_left[i],t,nbprint);
    }
    for (int i = 0; i < b->len_down_right; i++) {
      print_all_move_(&b->down_right[i],t,nbprint);
    }

    if (b->up_left==NULL && b->up_right==NULL && b->down_left==NULL && b->down_right==NULL) {
      printf("MOVE %d : %s\n",*nbprint,t);
      (*nbprint)++;
    }

    free(t);
  }
}

/**
 * print_all_move : permet d'afficher le contenu d'un bucket d'exploration sur la console
 *
 * @param {bucket_explorer*} b - le bucket d'exploration
 */
void print_all_move(bucket_explorer* b) {
  int n = 1; // On démarre les numéros à 1 pour avoir ce genre d'affichage :
             //    MOVE 1 ........
             //    MOVE 2 ......
             //    MOVE 3 ..
             //    ...
  print_all_move_(b, NULL, &n);
}

// permet de retourner un tableau de positions correspondant au meilleur à jouer contenu dans un bucket_explorer
/**
 * @deprecated
 * get_best_move : récupère le meilleure mouvement d'un bucket d'exploration (la meilleure rafle possible)
 *
 * @param {bucket_explorer*} b - le bucket d'exploration
 * @param {int*} n - la taille du tableau retourné
 *
 * @return {position*} retourne la meilleure rafle d'un bucket_explorer sous forme de tableau
 */
position* get_best_move(bucket_explorer* b, int *n) {

  *n = 0;

  if (b != NULL) {

    // On recupère la position courrante de où on se trouve dans le bucket_explorer
    position *t = (position*)malloc(sizeof(position));
    t->lig = b->pos->lig;
    t->col = b->pos->col;

    // Si c'est un cas trivial, on retourne la position courrante
    if (b->up_left == NULL && b->up_right == NULL && b->down_left == NULL && b->down_right == NULL) {
      *n = 1;
      return t;
    }

    // Sinon ce n'est pas un cas trivial et on doit récupérer la direction qui contient le plus de mouvements
    int nb = b->len_up_left+b->len_up_right+b->len_down_left+b->len_down_right;

    // On prépare les tableaux
    int *tn = (int*)malloc(nb*sizeof(int));
    position **tp = (position**)malloc(nb*sizeof(position*));

    // On explore le bucket_explorer dans toutes les directions
    int j = 0;
    for (int i = 0; i < b->len_up_left; i++) {
      tp[j] = get_best_move(&b->up_left[i],&tn[j]);
      j++;
    }
    for (int i = 0; i < b->len_up_right; i++) {
      tp[j] = get_best_move(&b->up_right[i],&tn[j]);
      j++;
    }
    for (int i = 0; i < b->len_down_left; i++) {
      tp[j] = get_best_move(&b->down_left[i],&tn[j]);
      j++;
    }
    for (int i = 0; i < b->len_down_right; i++) {
      tp[j] = get_best_move(&b->down_right[i],&tn[j]);
      j++;
    }

    // On recupère la direction qui contient le plus de mouvements
    int nmax = tn[0];
    position *tmax = tp[0];
    int index = 0;
    for (int i = 0; i < nb; i++) {
      if (tn[i] > nmax) {
        nmax = tn[i];
        tmax = tp[i];
        index = i;
      }
    }

    // On ajoute la position actuelle à la direction qui contient le plus de mouvements
    position *tfinal = (position*)malloc((nmax + 1)*sizeof(position));
    tfinal[0].lig = t->lig;
    tfinal[0].col = t->col;
    for (int i = 0; i < nmax; i++) {
      tfinal[i+1].lig = tmax[i].lig;
      tfinal[i+1].col = tmax[i].col;
    }

    // On retourne le nombre de mouvements
    *n = nmax + 1;

    // On libère les tableaux
    for (int i = 0; i < nb; i++) {
      if (tp[i] != NULL) free(tp[i]);
    }
    free(tn);
    free(tp);
    free(t);

    // On retourne le tableau
    return tfinal;
  }



  return NULL;
}

/**
 * @private
 * bucket_explorer_to_array_ : permet de transformer un bucket_explorer en bucket_array
 * (elle recupère chaque chemin menant à une feuille dans le bucket_explorer et ajoute ce chemin dans un tableau)
 *
 * @param {bucket_explorer*} b - le bucket d'exploration
 * @param {position**|bucket_array_item} t - le tableau qui contiendra le chemin courrant
 * @param {int} len - taille du tableau t
 * @param (int*) size - taille du tableau tpp
 * @param (int*) msize - taille du tableau tpp en mémoire
 * @param (position****) tpp - pointeur qui pointe vers un bucket_array
 * (pointeur qui pointe vers un tableau de pointeurs qui pointent vers des tableaux de pointers qui pointent vers des positions)
 */
static void bucket_explorer_to_array_(bucket_explorer* b, position** t, int len, int *size, int *msize, position ****tpp) {


  if (b == NULL) return;

  position **ct = malloc((len+2)*sizeof(position*));
  for (int i = 0; i < len; i++) {
    ct[i] = malloc(sizeof(position));
    ct[i]->lig = t[i]->lig;
    ct[i]->col = t[i]->col;
  }
  ct[len] = malloc(sizeof(position));
  ct[len]->lig = b->pos->lig;
  ct[len]->col = b->pos->col;
  ct[len+1] = NULL;


  for (int i = 0; i < b->len_up_left; i++) {
    bucket_explorer_to_array_(&b->up_left[i],ct,len+1,size,msize,tpp);
  }
  for (int i = 0; i < b->len_up_right; i++) {
    bucket_explorer_to_array_(&b->up_right[i],ct,len+1,size,msize,tpp);
  }
  for (int i = 0; i < b->len_down_left; i++) {
    bucket_explorer_to_array_(&b->down_left[i],ct,len+1,size,msize,tpp);
  }
  for (int i = 0; i < b->len_down_right; i++) {
    bucket_explorer_to_array_(&b->down_right[i],ct,len+1,size,msize,tpp);
  }

  if (b->up_left==NULL && b->up_right==NULL && b->down_left==NULL && b->down_right==NULL) {
    (*tpp)[*size] = ct;
    (*size)++;
    if ((*size)+1 > (*msize)-1) {
      *msize *= 2;
      position ***tp = realloc(*tpp,(*msize)*sizeof(position**));
      *tpp = tp;
    }
  } else {
    for (int i = 0; i < len+1; i++) free(ct[i]);
    free(ct);
  }

}


/**
 * bucket_explorer_to_array : permet de transformer un bucket_explorer en bucket_array
 * (elle recupère chaque chemin menant à une feuille dans le bucket_explorer et ajoute ce chemin dans un tableau)
 *
 * @param {bucket_explorer*} b - le bucket d'exploration
 * @param {int*} size - taille du tableau retourné
 *
 * @param {position***|bucket_array} retourne un bucket_array qui contient les rafles possibles présentent dans un bucket_explorer mais sous forme de tableaux
 */
bucket_array bucket_explorer_to_array(bucket_explorer* b, int *size) {
  if (b == NULL) return NULL;

  int msize = 2;

  bucket_array p = malloc(msize*sizeof(position**));

  position ****p_pointer = malloc(sizeof(bucket_array));
  *p_pointer = p;
 
  *size = 0;
  
  bucket_explorer_to_array_(b, NULL, 0, size, &msize, p_pointer);
  bucket_array p_pointert = *p_pointer;
  free(p_pointer);
  return p_pointert;
}

#endif