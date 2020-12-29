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



/**********************\
*                      *
*         TODO         *
*                      *
\**********************/

// TODO: finish the developpement of the game :
    // TODO: implement second resetTerm with callback function as argument -> simplify the drawing of header after each clear
    // TODO: implement submenu inside submenu of savemanager menu to give the choice to user to :
    //             - rename the save (optionnal but a little important)
// TODO: create readme.txt
// TODO: finish to code deep learning algorithm based on q-learning (in ia_agent.h)



/**********************\
*                      *
*       GLOBAL         *
*                      *
\**********************/

#include <stdio.h>
#include "structures.h"

// peut être modifier dans le sous-menu option (valeur par défaut)
int nb_lin = 10;
int nb_col = 10;
int nb_plin = 4;

jeu* game = NULL; // cette variable contiendra le jeu








/**********************\
*                      *
*       INCLUDE        *
*                      *
\**********************/

#include "game_interface.h" // On inclut toutes les fonctions liées à l'interface du jeu
#include "game_play.h"       // On inclut toutes les fonctions liées au fonctionnement du jeu






/**********************\
*                      *
*         MAIN         *
*                      *
\**********************/

//[ENTRY_POINT]
int main() {
    
  resetTerm();

  // On initialise le jeu
  game_init();

  // On fait tourner le jeu
  game_run();

  // On ferme le jeu
  return game_end();

}
