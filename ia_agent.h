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

#ifndef CHECKERS_IA_AGENT_H
#define CHECKERS_IA_AGENT_H


/**********************\
*                      *
*       INCLUDE        *
*                      *
\**********************/

#include "ia_core.h"
#include "game.h"


typedef struct agent agent;
struct agent {
  neural_network nn;
  int discount_factor;
};


/**
 * Le réseau de neuronnes devra être de la même taille que le plateau du jeu
 * pour pouvoir lui passer le jeu en entrée
 * (input layer composé de 4*g->NB_Col*g->NB_Lig neurones)
 *
 * Le réseau de neuronnes est 4 fois plus grand, en effet, on lui passe le jeu en 5 couches :
 *     - une couche qui indique s'il s'agit d'un pion blanc (qui appartient à l'agent)
 *     - une couche qui indique s'il s'agit d'un pion noir (qui n'appartient pas à l'agent)
 *     - une couche qui indique s'il s'agit d'une dame blanche (qui appartient à l'agent)
 *     - une couche qui indique s'il s'agit d'une dame noir (qui n'appartient pas à l'agent)
 *
 * Pour certains réseaux de neuronnes, on passe en entrée des images en passant par 3 canaux les composantes rouge, vert et bleu (ici c'est le même principe)
 */

/*
0 0 0: free
1 0 0: white piece
0 0 1: black piece
1 1 0: white king
0 1 1: black king
*/

/**********************\
*                      *
*      FUNCTIONS       *
*                      *
\**********************/

*agent agent_create(jeu*g) {

  agent* ia = malloc(sizeof(agent));

  // la structure du réseau de neurones
  // { 4*g->NB_Col*g->NB_Lig,  --> on veut passer le jeu avec différente couche
  //  X,   --> hidden layer composé de X neurones, c'est là où on traite l'information
  //  ,  --> output, on attend en résultat un bit (le résultat du XOR)
  // }
  int input[] = {2,10,1};

  ia->nn = neural_network_create(_1D(structure));

}




#endif