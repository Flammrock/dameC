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
 * gameplay.h
 *
 * Ce fichier contient toutes les fonctions de bases permettant de faire des opérations simples sur le plateau
 */

#ifndef CHECKERS_GAMEPLAY_H
#define CHECKERS_GAMEPLAY_H



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









/**********************\
*                      *
*       FUNCTIONS      *
*                      *
\**********************/


/* retourne le joueur courant de *g*/
joueur getJoueurCourant(jeu*g){
	return g->tabJ[g->jCourant];
}

/* retourne la valeur (dans l'enum caseVal) de la position p dans le jeu g **/
caseVal getCaseVal(jeu* g, position p){
	return g->plateau[p.lig*g->NB_Col+p.col];
}

/* fonction permettant de changer la valeur de la position p du jeu *g en lui donnant la valeur s **/
void setCaseVal(jeu* g, position p, caseVal s){
	g->plateau[p.lig*g->NB_Col+p.col]=s;
}

/* fonction pour initialiser le plateau de *g a un plateau vide de dimensions nl lignes et nc colonnes 
 * si on l'utilise une seconde fois pour changer la taille du plateau, ne pas oublier de faire free de g->plateau avant 
 * */
void initBlank(jeu *g, int nl, int nc) {
    if (g->plateau!=NULL) free(g->plateau);
    if (g->plateau_s!=NULL) free(g->plateau_s);
    if (g->jeux!=NULL) free(g->jeux);
    if (g->tabJ[0].nom!=NULL) free(g->tabJ[0].nom);
    if (g->tabJ[1].nom!=NULL) free(g->tabJ[1].nom);
    for (int i = 0; i < g->len_jeux; i++) {
      if (g->jeux[i].plateau!=NULL) free(g->jeux[i].plateau);
    }
    g->NB_Col = nc;
    g->NB_Lig = nl;
    g->plateau = calloc(1,g->NB_Lig*g->NB_Col*sizeof(int));
    g->plateau_s = calloc(1,g->NB_Lig*g->NB_Col*sizeof(highlight_info));
    g->jCourant = 0;
    g->tabJ[0].nom = NULL;
    g->tabJ[1].nom = NULL;
    g->size_jeux = 2;
    g->len_jeux = 0;
    g->is_center = 1;
    g->jeux = malloc(g->size_jeux*sizeof(state_jeu));
}

/* fonction pour initialiser le plateau de *g a un plateau 
 * de dimensions nl lignes et nc colonnes
 * avec des pieces blanches sur les nli premieres lignes aux cases prevues par les regles
 * avec des pieces noires sur les nli dernieres lignes aux cases prevues par les regles
 *
 * on peut utiliser initBlank pour commencer et ensuite on se sert de setCaseVal
 */
void init(jeu* g, int nl, int nc, int nli){
  initBlank(g, nl, nc);
  g->NB_LigInit = nli;
  int p = nl%2==0?1:0;
  for (int y = 0; y < nli; y++) {
    for (int x = 0; x < nc; x+=2) {
      if (x+(y%2) < nc) g->plateau[y*nc+x+(y%2)] = PB;
      if (x+((y+p)%2) < nc) g->plateau[(nl-y-1)*nc+x+((y+p)%2)] = PN;
    }
  }
}

/**
 * fonction pour initialier le plateau vide de *g a un plateau 
 * de dimensions nl lignes et nc colonnes
 */
void init_(jeu* g, int nl, int nc, int nli){
  initBlank(g, nl, nc);
  g->NB_LigInit = nli;
  /*for (int y = 0; y < nli; y++) {
    for (int x = 0; x < nc; x+=2) {
      if (x+(y%2) < nc) g->plateau[y*nc+x+(y%2)] = PB;
      if (x+((y+1)%2) < nc) g->plateau[(nl-y-1)*nc+x+((y+1)%2)] = PN;
    }
  }*/
}

/**
 * free_jeu : permet de libérer un jeu
 *
 * @param {jeu*} g - le jeu à libérer
 */
void free_jeu(jeu*g) {
  if (g==NULL) return;
  if (g->plateau!=NULL) free(g->plateau);
  if (g->plateau_s!=NULL) free(g->plateau_s);
  if (g->jeux!=NULL) free(g->jeux);
  if (g->tabJ[0].nom!=NULL) free(g->tabJ[0].nom);
  if (g->tabJ[1].nom!=NULL) free(g->tabJ[1].nom);
  for (int i = 0; i < g->len_jeux; i++) {
    if (g->jeux[i].plateau!=NULL) free(g->jeux[i].plateau);
  }
  free(g);
}

/* retourne 1 si les deux positions p et q ont les memes coordonnées et 0 sinon */
// checked ✔
int equals(position p,position q){
  if((p.col == q.col) && (p.lig == q.lig)){
    return 1;
  } else {
    return 0;
  }
}

/* retourne 1 si la position est bien dans le damier du jeu g et 0 sinon */
// checked ✔
int isValide(jeu* g, position p) {
	return p.lig >= 0 && p.lig <= g->NB_Lig-1 && p.col >= 0 && p.col <= g->NB_Col-1;
}

/* retourne 1 si la position p est vide dans le damier du jeu g et 0 sinon
 * (par rapport aux dimensions NB_Lig et NB_Col du plateau)
 * */
// checked ✔
int isVide(jeu* g, position p){
  if (VIDE == g->plateau[p.lig*g->NB_Col+p.col]) {
   return 1;
  }else{
    return 0;
  }
}

/* retourne la couleur (du type enuméré couleur) de la position p dans le jeu *g 
 * (NOCOL si la case est Vide)
 * */
// checked ✔
couleur getCouleur(jeu* g, position p){
  caseVal v = g->plateau[p.lig*g->NB_Col+p.col];
  if (v == PN || v == DN) {
    return NOIR;
  }
  if (v == PB || v == DB) {
    return BLANC;
  }
	return NOCOL;
}

/* retourne le type de piece de la position p dans le jeu *g 
 * (NOTYPE si la case est Vide)
 * */
// checked ✔
typePiece getTypePiece(jeu* g, position p){
caseVal v = g->plateau[p.lig*g->NB_Col+p.col];
  if (v == PB || v == PN) return Pion;
  if (v == DB || v == DN) return Dame;
  return NOTYPE;
}

/* retourne 1 si la position p dans *g est une piece capturee (voir type caseVal) et 0 sinon */
// checked ✔
int isCapture(jeu* g, position p){
	caseVal v = g->plateau[p.lig*g->NB_Col+p.col];
  return v == PNC || v == DNC || v == PBC || v == DBC;
}

/* permet de switcher la valeur d'une position p de capturée à non capturée et vice versa */
// checked ✔
void switchCapture(jeu* g, position p){
  caseVal v = g->plateau[p.lig*g->NB_Col+p.col];
  if (v == PN) {
    g->plateau[p.lig*g->NB_Col+p.col]=PNC;
  } else if (v == PB) {
    g->plateau[p.lig*g->NB_Col+p.col]=PBC;
  } else if (v == PNC) {
    g->plateau[p.lig*g->NB_Col+p.col]=PN;
  } else if (v == PBC) {
    g->plateau[p.lig*g->NB_Col+p.col]=PB;
  } else if (v == DN) {
    g->plateau[p.lig*g->NB_Col+p.col]=DNC;
  } else if (v == DB) {
    g->plateau[p.lig*g->NB_Col+p.col]=DBC;
  } else if (v == DNC) {
    g->plateau[p.lig*g->NB_Col+p.col]=DN;
  } else if (v == DBC) {
    g->plateau[p.lig*g->NB_Col+p.col]=DB;
  }
}

/* transforme en dames tous les pions qui sont arrives sur la rangee du fond adverse
 * retourne le nombre de pions ainsi promus
 */
// checked ✔
int promotion(jeu*g){
	for (int x = 0; x < g->NB_Col; x++) {
    if (g->plateau[(g->NB_Lig-1)*g->NB_Col+x] == PB) g->plateau[(g->NB_Lig-1)*g->NB_Col+x] = DB;
    if (g->plateau[x] == PN) g->plateau[x] = DN;
  }
	return -1;
}

/*
 * effectue le deplacement de la piece de p vers qu
 * en supposant que c'est bien un deplacement valide (sans verification)
 */
// checked ✔
void doDeplace(jeu*g, position deb, position fin){	
  g->plateau[fin.lig*g->NB_Col+fin.col] = g->plateau[deb.lig*g->NB_Col+deb.col];
  g->plateau[deb.lig*g->NB_Col+deb.col] = VIDE;
}

/*
 * effectue un mouvement de la piece p vers en capturant la piece en position prise
 * en supposant que c'est bien une prise valide (sans aucune verification)
 */
// checked ✔
void doMange(jeu*g, position p, position q, position prise){
  doDeplace(g, p, q);
  if (!isCapture(g,prise)) switchCapture(g, prise);
}


/* annule la capture de toutes les pieces capturees du plateau */
// checked ✔
void undoCapture(jeu*g) {
  for (int y = 0; y < g->NB_Lig; y++) {
    for (int x = 0; x < g->NB_Col; x++) {
      if (isCapture(g,(position){y,x})) switchCapture(g, (position){y,x});
    }
  }
}

void undoHighlight(jeu*g) {
  for (int y = 0; y < g->NB_Lig; y++) {
    for (int x = 0; x < g->NB_Col; x++) {
      g->plateau_s[y*g->NB_Col+x] = (highlight_info){HIGHLIGHT_MODE_EMPTY,0};
    }
  }
}

void doHighlight(jeu*g,position p) {
  g->plateau_s[p.lig*g->NB_Col+p.col].mode = HIGHLIGHT_MODE_HOVER;
}

void doSelect(jeu*g,position p) {
  g->plateau_s[p.lig*g->NB_Col+p.col].mode = HIGHLIGHT_MODE_SELECT;
}

void doSelectMultiple(jeu*g,position* ps,int size) {
  for (int i = 0; i < size; i++) {
    g->plateau_s[ps[i].lig*g->NB_Col+ps[i].col] = (highlight_info){HIGHLIGHT_MODE_MULTI_SELECT,i+1};
  }
}

void doHighlightIndex(jeu*g,position p,int index) {
  g->plateau_s[p.lig*g->NB_Col+p.col] = (highlight_info){HIGHLIGHT_MODE_HOVER,index};
}

void doSelectIndex(jeu*g,position p,int index) {
  g->plateau_s[p.lig*g->NB_Col+p.col] = (highlight_info){HIGHLIGHT_MODE_SELECT,index};
}

/* supprime du plateau toutes les pièces capturées */
// checked ✔
void removeCapture(jeu*g) {
	for (int y = 0; y < g->NB_Lig; y++) {
    for (int x = 0; x < g->NB_Col; x++) {
      if (isCapture(g,(position){y,x})) g->plateau[y*g->NB_Col+x] = VIDE;
    }
  }
}

/* retourne la position Voisine de p en avancant dans la direction donnee par dirlig dircol */
// checked ✔
position posVoisine(position p, int dircol, int dirlig){
	return (position){p.lig+dirlig,p.col+dircol};
}

/* retourne 1 si les deux positions p et q sont sur une même diagonale et 0 sinon */
// checked ✔
int isMemeDiag(position p, position q){
  if (equals(p,q)) return 1;
  return (int)floor(fabs(((float)p.lig - (float)q.lig)/((float)p.col - (float)q.col))) == 1;
}

/* teste si le deplacement d'un pion de p vers q dans *g correspond à un mouvement sans prise valide*/
// checked ✔
int isMoveElemPionValide(jeu*g, position p, position q) {
  if (!isValide(g, p)) return 0;
  if (!isValide(g, q)) return 0;
  if (!(g->plateau[p.lig*g->NB_Col+p.col] == PB || g->plateau[p.lig*g->NB_Col+p.col] == PN)) return 0;
  if (g->plateau[q.lig*g->NB_Col+q.col] != VIDE) return 0;
  if (equals(p,q)) return 0;
  if (getCouleur(g,p)==BLANC && !((p.col+1 == q.col || p.col-1 == q.col) && p.lig+1 == q.lig)) return 0;
  if (getCouleur(g,p)==NOIR && !((p.col+1 == q.col || p.col-1 == q.col) && p.lig-1 == q.lig)) return 0;
	return 1;
}

// @deprecated : @see isMangePionValideTemp (à la fin de ce fichier)
/* teste si le deplacement d'une pion de p vers q dans *g correspond à une prise valide
 * retourne la position de la piece prise si c'est le caseVal
 * retourne une position non valide {-1,-1} sinon
 */
// checked ✔
position isMangePionValide(jeu*g, position p, position q){
  if (!isValide(g, p)) return (position){-1,-1};
  if (!isValide(g, q)) return (position){-1,-1};
  if (!(getTypePiece(g,p) == Pion)) return (position){-1,-1};
  if (getTypePiece(g,q) != NOTYPE) return (position){-1,-1};
  if (equals(p,q)) return (position){-1,-1};
  if (!isMemeDiag(p,q)) return (position){-1,-1};

  if (abs(p.col-q.col)!=2 || abs(p.lig-q.lig)!=2)  return (position){-1,-1};
  
  int dx = p.col > q.col ? -1 : 1;
  int dy = p.lig > q.lig ? -1 : 1;
  position pionProche = posVoisine(p,dx,dy);

  if (!isValide(g, pionProche)) return (position){-1,-1};

  if (getTypePiece(g,pionProche)==NOTYPE) return (position){-1,-1};
  
  if (getCouleur(g,pionProche)==getCouleur(g,p)) return (position){-1,-1};

  return pionProche;
}


/* teste si le deplacement d'une dame de p vers q dans *g correspond à un mouvement sans prise valide*/
// checked ✔
int isMoveElemDameValide(jeu*g, position p, position q){
	if (!isValide(g, p)) return 0;
  if (!isValide(g, q)) return 0;
  if (!(g->plateau[p.lig*g->NB_Col+p.col] == DB || g->plateau[p.lig*g->NB_Col+p.col] == DN)) return 0;
  if (g->plateau[q.lig*g->NB_Col+q.col] != VIDE) return 0;
  if (equals(p,q)) return 0;
  if (!isMemeDiag(p,q)) return 0;
  int dx = p.col > q.col ? -1 : 1;
  int dy = p.lig > q.lig ? -1 : 1;
  position temp = p;
  while(isValide(g, temp)) {
    temp = posVoisine(temp,dx,dy);
    if (g->plateau[temp.lig*g->NB_Col+temp.col] != VIDE) return 0;
    if (equals(temp,q)) return 1;
  }
  return 0;
}

// @deprecated : @see isMangeDameValideTemp (à la fin de ce fichier)
/* teste si le deplacement d'une dame de p vers q dans *g correspond à une prise valide
 * retourne la position de la piece prise si c'est le caseVal
 * retourne une position non valide {-1,-1} sinon
 */
// checked ✔
position isMangeDameValide(jeu*g, position p, position q){
  if (!isValide(g, p)) return (position){-1,-1};
  if (!isValide(g, q)) return (position){-1,-1};
  if (getTypePiece(g,p) != Dame) return (position){-1,-1};
  
  if (getTypePiece(g,q) != NOTYPE) return (position){-1,-1};
  if (equals(p,q)) return (position){-1,-1};
  if (!isMemeDiag(p,q)) return (position){-1,-1};
  
  int dx = p.col > q.col ? -1 : 1;
  int dy = p.lig > q.lig ? -1 : 1;
  position pionProche = p;

  // on déclare deux compteur
  int count = 0;

  // position de la pièce prise
  position piece_prise = p;

  while(!equals(pionProche,q)){

    // mettre à jour pionProche avec dx et dy
    pionProche = posVoisine(pionProche,dx,dy);

    // on regarde si la position est valide
    if (!isValide(g, pionProche)) return (position){-1,-1};

    // si même couleur on quitte
    if (getCouleur(g,pionProche)==getCouleur(g,p)) return (position){-1,-1};

    // on incrémente le nombre de pièce trouvées
    if (getTypePiece(g,pionProche)!=NOTYPE) count++;

    // si on rencontre pour la 1ère fois une pièce, on "sauvegarde" sa position
    if (count==1) {count++;piece_prise = pionProche;}

    // si on a trouvé plus que 2 pièces entre p et q alors on quitte
    if (count > 2) return (position){-1,-1};

  }
  return piece_prise;
}


/* fonction reunissant les deux fonctions isMoveElemPionValide et isMoveElemDameValide
 * teste si le deplacement de la piece présente en position p vers q dans *g correspond à un mouvement sans prise valide
 */
// checked ✔
int isMoveElemValide(jeu*g, position p, position q){
  typePiece t = getTypePiece(g,p);
  if (t==Pion) return isMoveElemPionValide(g,p,q);
  if (t==Dame) return isMoveElemDameValide(g,p,q);
	return 0;
}


/* fonction reunissant les deux fonctions isMangeElemPionValide et isMangeElemDameValide
 * teste si le deplacement de la piece présente en position p vers la position q dans *g correspond à une prise valide
 * retourne la position de la piece prise si c'est le caseVal
 * retourne une position non valide {-1,-1} sinon
 */
// checked ✔
position isMangeValide(jeu*g, position p, position q){
  typePiece t = getTypePiece(g,p);
  if (t==Pion) return isMangePionValide(g,p,q);
  if (t==Dame) return isMangeDameValide(g,p,q);
	return (position){-1,-1};
}

/* fonction utilisé par la fonction explore (@see bucket_explorer.h) */
// checked ✔
position isMangePionValideTemp(jeu*g, position p, position q){
  if (!isValide(g, p)) return (position){-1,-1};
  if (!isValide(g, q)) return (position){-1,-1};
  //if (!(getTypePiece(g,p) == Pion)) return (position){-1,-1}; /* modification */
  if (getTypePiece(g,q) != NOTYPE) return (position){-1,-1};
  if (equals(p,q)) return (position){-1,-1};
  if (!isMemeDiag(p,q)) return (position){-1,-1};
  
  int dx = p.col > q.col ? -1 : 1;
  int dy = p.lig > q.lig ? -1 : 1;
  position pionProche = posVoisine(p,dx,dy);

  if (!isValide(g, pionProche)) return (position){-1,-1};

  if (getTypePiece(g,pionProche)==NOTYPE) return (position){-1,-1};
  
  if (getCouleur(g,pionProche)==getCouleur(g,p)) return (position){-1,-1};

  return pionProche;
}

/* fonction utilisé par la fonction explore (@see bucket_explorer.h) */
// checked ✔
position isMangeDameValideTemp(jeu*g, position p, position q){
  if (!isValide(g, p)) return (position){-1,-1};
  if (!isValide(g, q)) return (position){-1,-1};
  //if (getTypePiece(g,p) != Dame) return (position){-1,-1}; /* modification */
  
  if (getTypePiece(g,q) != NOTYPE) return (position){-1,-1};
  if (equals(p,q)) return (position){-1,-1};
  if (!isMemeDiag(p,q)) return (position){-1,-1};
  
  int dx = p.col > q.col ? -1 : 1;
  int dy = p.lig > q.lig ? -1 : 1;
  position pionProche = p;

  int count = 0;

  position piece_prise = p;

  while(!equals(pionProche,q)){

    pionProche = posVoisine(pionProche,dx,dy);

    if (!isValide(g, pionProche)) return (position){-1,-1};

    if (getCouleur(g,pionProche)==getCouleur(g,p)) return (position){-1,-1};

    if (getTypePiece(g,pionProche)!=NOTYPE) count++;

    if (count==1) {count++;piece_prise = pionProche;}

    if (count > 2) return (position){-1,-1};

  }
  return piece_prise;
}

#endif