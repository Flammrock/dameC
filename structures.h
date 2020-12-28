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
 * 
 * Fichiers contenant toutes les enum et struct pour le jeu
 * 
 */ 

#ifndef CHECKERS_STRUCTURES_H
#define CHECKERS_STRUCTURES_H

/* la structure representant les valeurs possibles des cases du plateau 
 * VIDE si aucune piexe n'occupe la case
 * les noms PB,PN,DB,DN correpondent respectivement a pion blanc, pion noir, dame blanche, dame noire
 * les versions avec un C sont pour les pieces qui sont capturées et seront utiles quand on cherchera a modeliser les sequences de prise
 * remarquez que la version capturée d'une piece a toujours une valeur negative egale a l'opposée de la version non capturée
 */
typedef enum caseVal{
	DNC=-4,DBC,PNC,PBC,VIDE,PB,PN,DB,DN
}caseVal;



typedef enum highlight_mode {
  HIGHLIGHT_MODE_EMPTY, // No Highlight
  HIGHLIGHT_MODE_HOVER, // Highlight free case where piece can go (with index)
  HIGHLIGHT_MODE_SELECT, // Highlight piece to move
  HIGHLIGHT_MODE_MULTI_SELECT // Highlight multiple pieces to move (with index)
} highlight_mode;
typedef struct highlight_info {
  highlight_mode mode;
  int index;
} highlight_info;





/* un type enumere pour les couleurs qui servira pour les pieces et pour les joueurs
 * la valeur NOCOL qui sera utile pour les fonctions qui devront retourner la couleur de la piece occupant une case donnée : si la case est vide, on retournera NOCOL
 */
typedef enum couleur{
	NOIR,NOCOL,BLANC
}couleur;

/* idem que couleur mais pour le type des pieces */
typedef enum typePiece{
	NOTYPE,Pion,Dame
}typePiece;

/* alias de enum pour différencier joueur humain et controlé par la machine */
typedef enum typeJoueur{
	Humain,
	CPU
}typeJoueur;

/* alias de structure pour représenter un joueur */
typedef struct joueur{
	char *nom;
	typeJoueur type; //0 pour humain, 1 pour machine
	couleur coul;
} joueur;

struct jeu;

typedef struct state_jeu {
  caseVal* plateau;
  int nb;
} state_jeu;

/* alias de structure centrale du jeu qui contient toute la configuration d'une partie en cours */
typedef struct jeu{
	int NB_Lig;
	int NB_Col;
	int NB_LigInit;
	caseVal* plateau;
  highlight_info* plateau_s; // cases sélectionnées
	int jCourant;
	joueur tabJ[2];

  state_jeu *jeux;
  int size_jeux;
  int len_jeux;

  int is_center;

  int is_virtual; // 1 => aucun temps de pause et le damier n'est jamais dessiné
                  // 0 => temps de pause et le damier est dessiné


}jeu;



/* alias de structure utile pour simplifier l'ecriture du code et qui modelise une position du plateau */
typedef struct position{
	int lig,col;
}position;



#endif