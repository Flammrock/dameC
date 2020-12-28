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
 * game.h
 *
 * Ce fichier contient les fonctions les plus importantes
 */


#ifndef CHECKERS_GAME_H
#define CHECKERS_GAME_H


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
#include <math.h>
#include "colors.h"
#include "structures.h"
#include "bucket_explorer.h"






/**********************\
*                      *
*       FUNCTIONS      *
*                      *
\**********************/

/**
 * game_init : permet d'initialiser la variable globale game
 */
void game_init() {
  configWindowsConsole();        // On configure la console pour windows
  srand(time(NULL));             // On initialise le random
  game = calloc(1,sizeof(jeu));  // On alloue de la mémoire pour la variable globale game
}

/**
 * game_end : permet d'arrêter le jeu proprement (à appeler juste avant que la fonction main termine)
 *
 * @TODO Faire en sorte que cette fonction retourne 1 s'il y a eu une erreur sinon 0 si tout c'est bien passé
 *
 * @return {int}
 */
int game_end() {
  //free_jeu();
  return 0;
}


/**
 * @private
 * getMoveSansPriseTmp : fonction temporaire qui permet de récupérer tout les mouvements sans prise depuis une position p
 *
 * @param {position***|bucket_array} tp - tableau que l'on rempli en y mettant la liste des mouvements sans prise depuis une position p
 * @param {position} p - la position de la pièce
 * @param {int} dx - 1 si on se dirige à droite, -1 si on se dirige à gauche
 * @param {int} dy - 1 si on se dirige en bas, -1 si on se dirige à droite
 * @param {int*} k - indice courrant
 * @param {int} size - nombre de cases vides
 */
static void getMoveSansPriseTmp(bucket_array tp, position p, int dx, int dy, int *k, int size) {
  if (!size) return;
  int j = *k;
  position q = p;
  for (int i = 0; i < size; i++) {
    q = posVoisine(q,dx,dy);
    if (i == 0) {
      tp[j] = malloc(3*sizeof(position*));
      tp[j][0] = malloc(sizeof(position));
      tp[j][0]->lig = p.lig;
      tp[j][0]->col = p.col;
      tp[j][1] = malloc(sizeof(position));
      tp[j][1]->lig = q.lig;
      tp[j][1]->col = q.col;
      tp[j][2] = NULL;
    } else {
      tp[j] = malloc((3+i)*sizeof(position*));
      position_array_copy(tp[j-1], tp[j]);
      tp[j][1+i] = malloc(sizeof(position));
      tp[j][1+i]->lig = q.lig;
      tp[j][1+i]->col = q.col;
      tp[j][2+i] = NULL;
    }
    j++;
  }
  *k = j;
}

/**
 * getMoveSansPrise : permet de récupérer la liste des mouvements sans prise depuis une position p
 *
 * @param {jeu*} g - le jeu
 * @param {position} p - la position p
 * @param {int*} size - la taille du tableau retourné
 *
 * @return {position***|bucket_array} retourne la liste des mouvements sans prise depuis une position p
 */
bucket_array getMoveSansPrise(jeu*g, position p, int *size) {
  *size = 0;
  if (!isValide(g,p)) return NULL;

  // S'il s'agit d'un pion, on a juste au maximum 2 mouvements possibles
  if (getTypePiece(g,p)==Pion) {
    int dy = getCouleur(g,p)==BLANC?1:-1;
    position p1 = posVoisine(p,-1,dy);
    position p2 = posVoisine(p,1,dy);
    int v1 = isValide(g,p1) && getTypePiece(g,p1)==NOTYPE;
    int v2 = isValide(g,p2) && getTypePiece(g,p2)==NOTYPE;
    if (v1) (*size)++;
    if (v2) (*size)++;
    if (*size <= 0) return NULL;
    bucket_array tp = malloc((*size)*sizeof(position**));
    int i = 0;
    if (v1) {
      tp[i] = malloc(3*sizeof(position*));
      tp[i][0] = malloc(sizeof(position));
      tp[i][0]->lig = p.lig;
      tp[i][0]->col = p.col;
      tp[i][1] = malloc(sizeof(position));
      tp[i][1]->lig = p1.lig;
      tp[i][1]->col = p1.col;
      tp[i][2] = NULL;
      i++;
    }
    if (v2) {
      tp[i] = malloc(3*sizeof(position*));
      tp[i][0] = malloc(sizeof(position));
      tp[i][0]->lig = p.lig;
      tp[i][0]->col = p.col;
      tp[i][1] = malloc(sizeof(position));
      tp[i][1]->lig = p2.lig;
      tp[i][1]->col = p2.col;
      tp[i][2] = NULL;
      i++;
    }
    return tp;
  }
  
  // Si c'est une dame
  else if (getTypePiece(g,p)==Dame) {

    int size1 = 0;
    explore_get_next_piece_sans_prise(g, p, 1, 1, &size1);

    int size2 = 0;
    explore_get_next_piece_sans_prise(g, p, 1, -1, &size2);

    int size3 = 0;
    explore_get_next_piece_sans_prise(g, p, -1, 1, &size3);

    int size4 = 0;
    explore_get_next_piece_sans_prise(g, p, -1, -1, &size4);

    *size = size1 + size2 + size3 + size4;

    if (*size <= 0) return NULL;

    

    bucket_array tp = malloc((*size)*sizeof(position**));

    int j = 0;
    getMoveSansPriseTmp(tp, p, 1, 1, &j, size1);
    getMoveSansPriseTmp(tp, p, 1, -1, &j, size2);
    getMoveSansPriseTmp(tp, p, -1, 1, &j, size3);
    getMoveSansPriseTmp(tp, p, -1, -1, &j, size4);

    return tp;
  }
  return NULL;
}

/**
 * canMove : permet de savoir si une pièce à la position p peut bouger/manger
 *
 * @param {jeu*} g - le jeu
 * @param {position} p - la position
 *
 * @return {int} 1 => la pièce à la position p peut bouger/manger, 0 => la pièce ne peut pas bouger
 */
int canMove(jeu*g,position p) {

    // on regarde déjà si la position p est valide
    if (!isValide(g,p)) return 0;

    // s'il y a aucune pièce alors aucun coup possible depuis la position p
    if (getTypePiece(g,p)==NOTYPE) return 0;

    // si pion
    if (getTypePiece(g,p)==Pion) {
            
        // on regarde si le pion peut avancer
        position q1 = posVoisine(p,1,getCouleur(g,p)==NOIR?-1:1);
        position q2 = posVoisine(p,-1,getCouleur(g,p)==NOIR?-1:1);
        if (isMoveElemPionValide(g,p,q1)) return 1;
        if (isMoveElemPionValide(g,p,q2)) return 1;
        // on regarde si le pion peut manger
        position qm1 = posVoisine(p,2,2);
        position qm2 = posVoisine(p,2,-2);
        position qm3 = posVoisine(p,-2,2);
        position qm4 = posVoisine(p,-2,-2);
        position invalide = (position){-1,-1};
        if (!equals(isMangePionValide(g,p,qm1),invalide)) return 1;
        if (!equals(isMangePionValide(g,p,qm2),invalide)) return 1;
        if (!equals(isMangePionValide(g,p,qm3),invalide)) return 1;
        if (!equals(isMangePionValide(g,p,qm4),invalide)) return 1;

    
    }
    // sinon dame
    else if (getTypePiece(g,p)==Dame) {
    
        // on fait la recherche dans toutes les directions
        int dx[] = {1,1,-1,-1};
        int dy[] = {1,-1,1,-1};
        
        for (int i = 0; i < 4; i++) {
    
            // on regarde si la dame peut avancer
            position temp = p;
            int find = 0;
            while (1) {
                temp = posVoisine(p,dx[i],dy[i]);
                if (!isValide(g,temp)) break;
                if (isVide(g,temp)) return 1;
                if (getTypePiece(g,temp)!=NOTYPE) {
                    if (getCouleur(g,temp)!=getCouleur(g,p)) find = 1;
                    break;
                }
            }
            // on regarde si la dame peut manger
            if (find) {
                if (isVide(g,posVoisine(temp,dx[i],dy[i]))) return 1;
            }
        
        }
    
    }
    
    // si on arrive ici, alors il n'y a aucun coup possible depuis la position p
    return 0;

}


/**
 * getMove : permet de récupérer les mouvements possibles depuis une position p, si des rafles sont possibles depuis la position p, alors cette fonction retourne les meilleures rafles
 *
 * @param {jeu*} g - le jeu
 * @param {position} p - la position p
 * @param {int*} size - la taille du tableau retourné
 * @param {int*} iscatch - 0 => mouvement sans prise, 1 => mouvement avec prise
 *
 * @return {position***|bucket_array} retourne un bucket_array contenant les mouvements disponibles depuis une position p
 */
bucket_array getMove(jeu*g,position p, int *size, int *iscatch) {

  // On récupère toutes les rafles possibles depuis la position p
  bucket_explorer *b = explore(g,p);

  *size = 0;
  *iscatch = 0;

  // On transforme le bucket d'exploration en tableau
  bucket_array ab = bucket_explorer_to_array(b,size);

  // S'il n'y a pas de rafles, alors on retourne la liste des mouvements sans prise
  if (is_bucket_empty(b)) {
    free_bucket_array(ab,*size);
    free_bucket_explorer(b);
    return getMoveSansPrise(g, p, size);
  }

  // On a plus besoin du bucket (on l'a maintenant sous forme de tableau)
  free_bucket_explorer(b);

  // On fait savoir qu'il s'agit d'une rafle
  *iscatch = 1;

  // On récupère les meilleures rafles de longueurs équivalentes
  int maxmovelen = 0;
  int *indexs = calloc(*size,sizeof(int));
  int k = 1;
  int nb = 0;
  for (int i = 0; i < *size; i++) {
    int j = 0;
    while (ab[i][j]!=NULL) {
      j++;
    }
    if (j > maxmovelen) {
      maxmovelen = j;
      k++;
      nb = 0;
    }
    if (j == maxmovelen) {
      indexs[i] = k;
      nb++;
    }
  }

  // On stock ces mouvements dans un nouveau bucket_array
  bucket_array abt = malloc(nb*sizeof(position**));

  // On libère les moins bons mouvements
  int v = 0;
  for (int i = 0; i < *size; i++) {
    if (indexs[i] != k) {
      int j = 0;
      while (ab[i][j]!=NULL) {
        free(ab[i][j]);
        j++;
      }
      free(ab[i]);
    } else {
      abt[v++] = ab[i];
    }
  }
  free(ab);

  *size = nb; // On y stocke le nombre de meilleures coups

  return abt;
  
}

/**
 * hightlight_move : permet de mettre en surbrillance un mouvement pour mieux le voir sur le damier
 *
 * @param {jeu*} g - le jeu
 * @param {position**|bucket_array_item} p - le mouvement à mettre en surbrillance
 */
void hightlight_move(jeu*g,position**p) {
  if (p!=NULL) {
    position* n = p[0];
    int i = 0;
    while (p[i]!=NULL) {
      doHighlight(g,(position){p[i]->lig,p[i]->col});
      i++;
      n = p[i];
    }
  }
}

/**
 * hightlight_move_index : permet de mettre en surbrillance un mouvement pour mieux le voir sur le damier en affichant le numéro du mouvement (dans le cas où on veut afficher plusieurs mouvements)
 *
 * @param {jeu*} g - le jeu
 * @param {position**|bucket_array_item} p - le mouvement à mettre en surbrillance
 * @param {int} index - le numéro du mouvement (que l'on verra afficher sur le damier)
 */
void hightlight_move_index(jeu*g,position**p,int index) {
  if (p!=NULL) {
    position* n = p[0];
    int i = 0;
    while (p[i]!=NULL) {
      if (p[i+1]==NULL && index > 0) {
        doHighlightIndex(g,(position){p[i]->lig,p[i]->col},index);
      } else {
        doHighlight(g,(position){p[i]->lig,p[i]->col});
      }
      i++;
      n = p[i];
    }
  }
}

/**
 * hightlight_bucket_array : permet de mettre en surbrillance une liste de mouvements our mieux les voir sur le damier
 *
 * @param {jeu*} g - le jeu
 * @param {position***|bucket_array} ba - la liste de mouvements à mettre en surbrillance
 * @param {int} size - la taille du bucket_array
 */
void hightlight_bucket_array(jeu*g,bucket_array ba,int size) {
  for (int i = 0; i < size; i++) {
    hightlight_move_index(g,ba[i],i+1);
  }
}

/**
 * doMove : permet d'effectuer un mouvement en déplaçant la pièce (et en mangeant s'il s'agit d'une rafle)
 *
 * @param {jeu*} g - le jeu
 * @param {position**|bucket_array_item} p - le mouvement à faire
 */
void doMove(jeu *g,position** p) {
  if (p!=NULL) {
    if (p[1]!=NULL) {

      // S'il s'agit d'un simple mouvement
      if (p[2]==NULL) {

        int i = 0;

        position pr = (position){p[i]->lig,p[i]->col};
        position qr = (position){p[i+1]->lig,p[i+1]->col};

        if (isMoveElemValide(g,pr,qr)) {
          doDeplace(g,pr,qr);
          resetTerm();
          print_damier(g);
          return;
        }

        position prise = isMangeValide(g,pr,qr);
        if (!equals(prise,(position){-1,-1})) {
          doMange(g,pr,qr,prise);
          resetTerm();
          print_damier(g);
          sleepcp(TIME_STEP);
          removeCapture(g);
          resetTerm();
          print_damier(g);
          return;
        }

      // S'il s'agit d'un mouvement plus complexe
      } else {

        int i = 0;
        while (p[i]!=NULL && p[i+1]!=NULL) {
          position pr = (position){p[i]->lig,p[i]->col};
          position qr = (position){p[i+1]->lig,p[i+1]->col};

          position prise = isMangeValide(g,pr,qr);
          if (!equals(prise,(position){-1,-1})) {
            doMange(g,pr,qr,prise);
            sleepcp(TIME_STEP);
            resetTerm();
            print_damier(g);
            continue;
          }

          i++;
        }
        sleepcp(TIME_STEP);
        removeCapture(g);
        resetTerm();
        print_damier(g);

      }

    }

  }
}

/**
 * @deprecated
 * getBestMoveColor : permet de récupérer la meilleure rafle d'un joueur
 *
 * @param {jeu*} g - le jeu
 * @param {couleur} c - la couleur du joueur (joueur blanc ou joueur noir)
 * @param {int*} sizeptr - la taille du tableau retourné
 *
 * @return {position*} retourne la meilleure rafle (dans le cas où il y a des rafles équivalentes, ça peut poser problème, de plus cette fonction n'est pas compatible avec les autres)
 */
position* getBestMoveColor(jeu*g,couleur c,int *sizeptr) {
  
  int lplen = 0;
  int lpsize = 2;
  position* lp = malloc(lpsize*sizeof(position));
  int maxmove = 0;
  for (int y = 0; y < g->NB_Lig; y++) {
    for (int x = 0; x < g->NB_Col; x++) {
      int size = 0;
      int iscatch = 0;
      if (getCouleur(g,(position){y,x})==c) {
        bucket_array bestmovetmp = getMove(g, (position){y,x}, &size, &iscatch);

        if (iscatch && size > maxmove) {
          maxmove = size;
          free(lp);
          lpsize = 2;
          lplen = 0;
          lp = malloc(lpsize*sizeof(position));
          lp[lplen++] = (position){y,x};
        } else if (iscatch && size == maxmove) {
          if (lplen+1 > lpsize-1) {
            lpsize*=2;
            lp = realloc(lp,lpsize*sizeof(position));
          }
          lp[lplen++] = (position){y,x};
        }

        if (bestmovetmp!=NULL) free_bucket_array(bestmovetmp,size);
        
      }
    }
  }

  *sizeptr = lplen;
  
  return lp;
  
}

/**
 * gameOver : permet de savoir si un joueur a perdu
 *
 * @param {jeu*} g - le jeu
 * @param {couleur} c - la couleur du joueur que l'on veut tester
 *
 * @return {int} 1 => si le joueur a perdu, 0 => si le joueur n'a pas perdu (ça veut dire qui lui reste des coups à jouer)
 */
int gameOver(jeu *g, couleur c){
  int size = 0;
  int sertarien = 0;
  for (int y = 0; y < g->NB_Lig; y++) {
    for (int x = 0; x < g->NB_Col; x++) {
      position temp = (position){y,x};
      if (getCouleur(g,temp)==c && canMove(g,temp)) {
        return 0;
      }
    }
  }
  return 1;
}

/**
 * create_state : permet de créer un état du jeu (@see structures.h)
 *
 * @param {jeu*} g - le jeu
 *
 * @return {state_jeu} retourne une copie de l'état du plateau du jeu
 */
state_jeu create_state(jeu *g) {
  state_jeu s;
  s.nb = 1;
  s.plateau = malloc(g->NB_Col*g->NB_Lig*sizeof(caseVal));
  for (int i = 0; i < g->NB_Col*g->NB_Lig; i++) {
      s.plateau[i] = g->plateau[i];
  }
  return s;
}

/**
 * free_state : permet de libérer un state_jeu
 *
 * @param {state_jeu} s - l'état du jeu que l'on veut libérer
 */
void free_state(state_jeu s) {
  if (s.plateau!=NULL) free(s.plateau);
}

/**
 * tabequal : permet de vérifier si l'état du jeu est le même qu'un état sauvegarder dans un state_jeu
 *
 * @TODO renommer cette fonction pour qu'elle soit plus intuitive
 *
 * @param {jeu*} g - le jeu
 * @param {state_jeu} s - l'état de jeu que l'on veut comparer
 *
 * @return {int} 1 => l'état du jeu et le jeu sont égale, 0 => non égale
 */
int tabequal(jeu *g,state_jeu s){
  for (int i = 0; i < g->NB_Col*g->NB_Lig; i++) {
      if (g->plateau[i] != s.plateau[i]) {
        return 0;
      }
  }
  return 1;
}

/**
 * @deprecated (cette fonction ne rapporte pas assez d'info)
 * nbrPiecePlato : permet de compter le nombre de pièces sur le plateau
 *
 * @TODO renommer cette fonction pour qu'elle soit plus intuitive
 *
 * @param {jeu*} g - le jeu
 *
 * @return {int} retourne le nombre de pièces présent sur le plateau du jeu
 */
int nbrPiecePlato(jeu *g){
  int nbrPiece = 0;
  for (int y = 0; y < g->NB_Lig; y++) {
    for (int x = 0; x < g->NB_Col; x++) {
      if(getTypePiece(g,(position){y,x}) != VIDE){
        nbrPiece++;
      }
    }
  }
  return nbrPiece;
}

/**
 * gameEquality : permet de savoir si on peut considérer la partie comme une égalité suivant les règles du jeu de dame
 *
 * @param {jeu*} g - le jeu
 * @param {typePiece} typepiece - le type de la pièce qui vient de bouger
 * @param {int} isprise - 1 => un mange a eu lieu, 0 => aucune pièce n'a été mangée
 *
 * @return {int} 1 => on considère la partie comme une égalité, 0 => on continue la partie
 */
int gameEquality(jeu *g,typePiece p,int isprise){

  // On compte le nombre de pions et de dames
  int nbdame = 0;
  int nbpion = 0;
  for (int y = 0; y < g->NB_Lig; y++) {
    for (int x = 0; x < g->NB_Lig; x++) {
      typePiece t = getTypePiece(g,(position){y,x});
      if (t==Pion) nbpion++;
      else if (t==Dame) nbdame++;
    }
  }

  if (g->len_jeux > 0 && nbpion+nbdame > 3) {

    // Si il y a des pièces capturées ou si un pion bouge en avant
    if(isprise || p==Pion) {

      // On libère la mémoire qu'on n'utilise plus
      for (int i = 0; i < g->len_jeux; i++) {
        free_state(g->jeux[i]);
      }
      g->size_jeux = 2;
      g->len_jeux = 0;
      free(g->jeux);
      
      g->jeux = malloc(g->size_jeux*sizeof(state_jeu));
    }

  }

  // On compte le nombre de coups durant laquelle il n'y a eu aucun mouvement de pions et de prises
  int occurrences = 0;
  for (int i = 0; i < g->len_jeux; i++) {
    occurrences+=g->jeux[i].nb;
  }

  // Pour autant qu’il n’y ait pas de phase de jeu en cours, la fin de partie de deux dames contre une dame, et a fortiori, de une dame contre une dame, sera considérée égale.
  if (nbpion==0 && (nbdame==3||nbdame==2)) return 3;


  // S’il n'y a plus que trois dames, deux dames et un pion, ou une dame et deux pions contre une dame, la fin de partie sera considérée comme égale lorsque les deux joueurs auront encore joué chacun 16 coups au maximum
  if (nbpion+nbdame<=3) {
    if (occurrences>=32) return 4;
  }

  // On scale la taille du tableau g->jeux
  if (g->len_jeux+1 > g->size_jeux-1) {
    g->size_jeux *= 2; // complexité amorti
    g->jeux = realloc(g->jeux,g->size_jeux*sizeof(state_jeu));
  }
  
  // On teste tout les états en memoire par rapport au plateau actuel
  int isegal = 0;
  int index = 0;
  for(int i = 0; i < g->len_jeux; i++){
    if(tabequal(g,g->jeux[i])){
      isegal = 1;
      index = i;
      break;
    }
  }

  // La fin de partie est considérée comme égale lorsque la même position se représente pour la troisième fois, le même joueur ayant le trait.
  if (isegal) {
    g->jeux[index].nb++;
    if (g->jeux[index].nb==3) {
      return 1;
    }
  } else {
    g->jeux[g->len_jeux++] = create_state(g); //crée la copie du jeu
  }

  // Si, durant 25 coups, il n'y a ni déplacement de pion ni prise, la fin de partie est considérée comme égale.
  if (occurrences>=25) return 2;

  
  // Si on arrive jusqu'ici alors on ne considère pas la partie comme une égalité
  return 0;
}

/**
 * get_random_move : permet de récupérer un mouvement aléatoire d'une pièce aléatoire de la couleur c
 *
 * @param {jeu*} g - le jeu
 * @param {couleur} c - la couleur du joueur
 *
 * @return {bucket_array_item} retourne le mouvement choisi aléatoirement
 */
bucket_array_item get_random_move(jeu*g,couleur c,int* iscatch) {

  bucket_array_item move = NULL;

  int testsizebestmove = 0;
  position* testpbestmove = getBestMoveColor(g,c,&testsizebestmove);
  int iscatchbasize = 0;

  // On force le joueur à faire un mange (ou une rafle) s'il y en a une
  if (testsizebestmove > 0) {
    position pchoose = testpbestmove[rand()%testsizebestmove];
    int basize = 0;
    bucket_array ba = getMove(g, pchoose, &basize, &iscatchbasize);
    move = free_bucket_array_expectone(ba, basize, rand()%basize);
  }
  
  // Sinon, on lui laisse le choix
  else {
    int sizelist = 2;
    int lenlist = 0;
    position* list = malloc(sizelist*sizeof(position));
    // On itère sur le plateau et on récupère toutes les pièces qui peuvent bouger
    for (int y = 0; y < g->NB_Lig; y++) {
      for (int x = 0; x < g->NB_Col; x++) {
        position temp = (position){y,x};
        if (getCouleur(g,temp)==c && canMove(g,temp)) {
          list[lenlist++] = temp;
          if (lenlist+1>sizelist-1) {
            sizelist *= 2;
            list = realloc(list,sizelist*sizeof(position));
          }
        }
      }
    }

    // On choisi au hasard une pièce qui peut bouger
    position p = list[rand()%lenlist];
    free(list);

    int sizeba = 0;
    bucket_array ba = getMove(g, p, &sizeba, &iscatchbasize);

    move = free_bucket_array_expectone(ba, sizeba, rand()%sizeba);
  }

  if (testpbestmove!=NULL) free(testpbestmove);

  *iscatch = iscatchbasize;

  return move;


}









#endif