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

// TODO: trouver des meilleures noms pour les fonctions de manière à ce que ce soit plus intuitif
// namespace utilisé : "savemanager"

// TODO: créer un itérateur avec une fonction callback en argument => rendre plus lisible le code et le simplifier

/**
 * loader.h
 *
 * Ce fichier contient le "loading screen" (fonction print_loader)
 * Ce fichier contient également toutes les fonctions pour charger
 * et sauvegarder des parties de jeux.
 *
 * Les sauvegardes sont enregistrées dans le fichier "save.data"
 * (défini par un define (voir plus bas))
 *
 */

#ifndef CHECKERS_LOADER_H
#define CHECKERS_LOADER_H




/**********************\
*                      *
*       INCLUDE        *
*                      *
\**********************/

#include "display.h" // on aura besoin de ce fichier pour dessiner le menu
                     // (et pour effectuer différentes opérations sur la console)

#include "menu.h" // on propose au joueur de sélectionner la sauvegarde à chargée via un menu

#include <string.h> // très pratique pour manipuler des chaînes de caractères (utilisé pour gérer le nom des sauvegardes)
#include <errno.h> // pour déterminer si un fichier existe (on vérifie si le fichier "save.data" existe bien)

#include "helper.h"



/**********************\
*                      *
*        MACRO         *
*                      *
\**********************/

#define LOADER_FPS 10            // on attend 100ms entre chaque animation du "loading screen"
#define FILE_SAVE  "save.data"   // le nom du fichier où on enregistre les sauvegardes
#define MAX_LEN_SAVE_NAME 200    // limite en taille pour les noms des sauvegardes






/**********************\
*                      *
*    STRUCT & ENUM     *
*                      *
\**********************/

typedef enum state_savemanager {

  // Si l'enregistrement' ou le chargrement est un succès
  STATE_SAVEMANAGER_SUCCESS,

  // Si aucune sauvegarde avec le nom spéicifié n'est trouvée
  STATE_SAVEMANAGER_UNKNOW,

  // Si le fichier "save.data" est corrompu (impossiblité de parser les données)
  STATE_SAVEMANAGER_CORRUPTED,

  // Si une erreur IO (ce qui n'arrivera que dans 0.000000001% des cas (en théorie))
  STATE_SAVEMANAGER_ERROR

} state_savemanager;







/**********************\
*                      *
*       GLOBAL         *
*                      *
\**********************/

// Animation du chargement, chaque caractère de la chaîne sera affiché pendant 100ms à la suite
char loader[] = "|\\-/";




/**********************\
*                      *
*      PROTOTYPE       *
*                      *
\**********************/

/**
 * print_loader : permet d'afficher un "loading screen" pendant un certain temps (oui c'est un faux chargement (il faudrait créer 2 thread pour pouvoir charger et afficher le chargement en même temps))
 *
 * @param {int} duration - en millisecondes
 *
 * @return cette fonction ne retourne rien, elle contente d'afficher
 */
void print_loader(int duration);

/**
 * savemanager_save_game : permet de sauvegarder une partie et de l'enregistrer dans "save.data"
 *
 * @param {jeu*} g - un pointer vers une variable de type jeu (@see structures.h)
 * @param {char*} s - nom de la sauvegarde
 *
 * @return cette fonction retourne une variable de type state_savemanager qui indique l'état de l'opération (s'il y a eu une erreur, si c'est un succès..etc..)
 */
state_savemanager savemanager_save_game(jeu*g,char *s);

/**
 * savemanager_load_save : permet de charger une sauvegarde qui se trouve dans le fichier "save.data" (si l'opération est un succès alors la variable jeu* passée en argument contiendra le jeu chargé)
 *
 * @param {jeu*} g - un pointer vers une variable de type jeu (@see structures.h) (là où on mettra tout ce qu'on a pu chargé du fichier "save.data")
 * @param {char*} s - nom de la sauvegarde à charger
 *
 * @return cette fonction retourne une variable de type state_savemanager qui indique l'état de l'opération (s'il y a eu une erreur, si c'est un succès..etc..)
 */
state_savemanager savemanager_load_save(jeu*g,char *s);

/**
 * savemanager_remove_save : permet de supprimer une sauvegarde qui se trouve dans le fichier "save.data"
 *
 * @param {char*} s - nom de la sauvegarde à supprimer
 *
 * @return cette fonction retourne une variable de type state_savemanager qui indique l'état de l'opération (s'il y a eu une erreur, si c'est un succès..etc..)
 */
state_savemanager savemanager_remove_save(char *s);

/**
 * savemanager_count_save : permet de compter le nombre de sauvegarde qui se trouve dans le fichier "save.data"
 *
 * @TODO: retourner un état "state_savemanager" et retourner la nombre de sauvegarde via un int*
 *
 * @return cette fonction retourne -1 s'il y a eu une erreur, sinon elle retourne le nombre de sauvegarde
 */
int savemanager_count_save();

/**
 * savemanager_getindex : permet de retourner les positions dans le fichier de toutes les sauvegardes dans un tableau de type long (ça permet d'accèder aux positions des sauvegardes dans le fichier beaucoup plus facilement)
 *
 * @param {int*} length - On mettra dans cette variable la taille du tableau retourné
 *
 * @return cette fonction retourne NULL s'il y a eu une erreur, sinon elle retourne un tableau de type long contenant les positions de toutes les sauvegardes dans le fichier "save.data"
 */
long* savemanager_getindex(int* length);

/**
 * savemanager_getnames : permet de retourner les noms de toutes les sauvgardes dans un tableau qui se trouvent dans le fichier "save.data"
 *
 * @param {int*} length - On mettra dans cette variable la taille du tableau retourné
 *
 * @return cette fonction retourne NULL s'il y a eu une erreur, sinon elle retourne un tableau de type char* contenant les noms de toutes les sauvegardes dans le fichier "save.data"
 */
char** savemanager_getnames(int* length);

/**
 * savemanager_load_save_by_index : permet de charger une sauvegarde en fournissant sa position dans le fichier "save.data" (si l'opération est un succès alors la variable jeu* passée en argument contiendra le jeu chargé)
 *
 * @param {jeu*} g - un pointer vers une variable de type jeu (@see structures.h) (là où on mettra tout ce qu'on a pu chargé du fichier "save.data")
 * @param {long} index - nom de la sauvegarde à charger
 *
 * @return cette fonction retourne une variable de type state_savemanager qui indique l'état de l'opération (s'il y a eu une erreur, si c'est un succès..etc..)
 */
state_savemanager savemanager_load_save_by_index(jeu*g,long index);

/**
 * savemanager_submenu_save_delete : supprime la sauvegarde choisi par le joueur lorsque le joueur sélectionne le bouton "SUPPRIMER" dans le menu
 *
 * @Event {menu_event} (fonction appelé lors du choix d'un item dans le menu) (@see menu.h) (les arguments sont décrits dans menu.h et ce qui est retourné)
 *
 */
menu_message savemanager_submenu_save_delete(menu* m, menu_item* item, int i);

/**
 * savemanager_submenu_save : lorsque le joueur choisi une sauvegarde, on affiche le menu : "CHARGER", "RENOMMER", "SUPPRIMER", "RETOUR"
 *
 * @Event {menu_event} (fonction appelé lors du choix d'un item dans le menu) (@see menu.h) (les arguments sont décrits dans menu.h et ce qui est retourné)
 *
 */
menu_message savemanager_submenu_save(menu* m, menu_item* item, int i);

/**
 * savemanager_menu : permet d'afficher la liste des sauvegardes sous forme d'un menu où le joueur peut sélectionner une sauvegarde pour ainsi obtenir plus d'info sur celle-ci et pour pouvoir la chargée ou bien la supprimée selon le choix du joueur
 *
 * @param {jeu*} g - un pointer vers une variable de type jeu (@see structures.h) (là où on mettra la sauvegarde qu'on a chargé du fichier "save.data")
 *
 * @return cette fonction retourne 0 si le joueur n'a rien chargé où s'il y a eu une erreur, sinon cette fonction renvoie 1 pour indiqué que le joueur a chargé une partie
 */
int savemanager_menu(jeu*g);







/**********************\
*                      *
*       FUNCTIONS      *
*                      *
\**********************/



/**
 * print_loader : permet d'afficher un "loading screen" pendant un certain temps (oui c'est un faux chargement (il faudrait créer 2 thread pour pouvoir charger et afficher le chargement en même temps))
 *
 * @param {int} duration - en millisecondes
 *
 * @return cette fonction ne retourne rien, elle contente d'afficher
 */
void print_loader(int duration) {
  resetTerm();
  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);
  for (int i = 0; i < h-2; i++) printf("\n");
  clock_t tick = clock();
  clock_t tick2 = clock();
  int fpsc = 1000/LOADER_FPS;
  int i = 0;
  int len = strlen(loader);
  while (clock() - tick < duration * CLOCKS_PER_SEC / 1000) {
    for (int k = 0; k < w-17; k++) printf(" ");
    printf("%s %c %sChargement\n",BRED,loader[i],RESET);
    printf("\e[F");
    sleepcp(fpsc);
    i++;
    i %= len;
  }
}

/**
 * savemanager_save_game : permet de sauvegarder une partie et de l'enregistrer dans "save.data"
 *
 * @param {jeu*} g - un pointer vers une variable de type jeu (@see structures.h)
 * @param {char*} s - nom de la sauvegarde
 *
 * @return cette fonction retourne une variable de type state_savemanager qui indique l'état de l'opération (s'il y a eu une erreur, si c'est un succès..etc..)
 */
state_savemanager savemanager_save_game(jeu*g,char *s) {
  FILE* file = NULL;
  int isexist = is_exist_file(FILE_SAVE);
  if (isexist==0) {
    file = fopen(FILE_SAVE,"rb+");
  } else if (isexist==1) {
    file = fopen(FILE_SAVE,"wb+");
  } else {
    return STATE_SAVEMANAGER_ERROR;
  }
  if (file==NULL) return STATE_SAVEMANAGER_ERROR;
  fseek(file,0,SEEK_END);

  long start = ftell(file);

  int len = strlen(s);
  fwrite(&len, sizeof(int), 1, file);
  fwrite(s, sizeof(char), len, file);

  long sizedatapos = ftell(file);
  long size = 0;
  fwrite(&size, sizeof(long), 1, file);

  fwrite(&g->NB_Lig, sizeof(int), 1, file);
  fwrite(&g->NB_Col, sizeof(int), 1, file);
  fwrite(&g->NB_LigInit, sizeof(int), 1, file);
  fwrite(g->plateau, sizeof(caseVal), g->NB_Lig*g->NB_Col, file);
  fwrite(g->plateau_s, sizeof(highlight_info), g->NB_Lig*g->NB_Col, file);
  fwrite(&g->jCourant, sizeof(int), 1, file);

  for (int i = 0; i < 2; i++) {
    if (g->tabJ[i].nom!=NULL) {
      int lenname = strlen(g->tabJ[i].nom);
      fwrite(&lenname, sizeof(int), 1, file);
      if(lenname>0) {
        fwrite(g->tabJ[i].nom, sizeof(char), lenname, file);
      }
    } else {
      int lenname = 0;
      fwrite(&lenname, sizeof(int), 1, file);
    }
    fwrite(&g->tabJ[i].coul, sizeof(couleur), 1, file);
    fwrite(&g->tabJ[i].type, sizeof(typeJoueur), 1, file);
  }
  
  fwrite(&g->size_jeux, sizeof(int), 1, file);
  fwrite(&g->len_jeux, sizeof(int), 1, file);
  for (int i = 0; i < g->len_jeux; i++) {
    fwrite(&g->jeux[i].nb, sizeof(int), 1, file);
    fwrite(g->jeux[i].plateau, sizeof(caseVal), g->NB_Lig*g->NB_Col, file);
  }

  fwrite(&g->is_center, sizeof(int), 1, file);

  size = ftell(file)-start;
  fseek(file,sizedatapos,SEEK_SET);
  fwrite(&size, sizeof(long), 1, file);

  fclose(file);
  return STATE_SAVEMANAGER_SUCCESS;
}

/**
 * savemanager_load_save_ : permet de charger une partie de la sauvegarde qui se trouve dans le fichier "save.data" (tout est mis dans la vairable jeu* mis en argument), cette fonction est "privé" et ne peut être utilisée que depuis ce fichier
 *
 * @param {FILE*} file - pointer du fichier vers le fichier "save.data", le curseur du fichier est déjà mis au bon endroit, il reste juste à lire (pas besoin de chercher ça a été fait par la fonction qui l'a appelé)
 * @param {jeu*} g - un pointer vers une variable de type jeu (@see structures.h) (là où on mettra tout ce qu'on trouvera dans "save.data")
 *
 * @return cette fonction retourne une variable de type state_savemanager qui indique l'état de l'opération (s'il y a eu une erreur, si c'est un succès..etc..)
 */
static state_savemanager savemanager_load_save_(FILE* file, jeu*g) {
  int err = 0;
  err = fread(&g->NB_Lig, sizeof(int), 1, file);
  if (err!=1) return STATE_SAVEMANAGER_CORRUPTED;

  err = fread(&g->NB_Col, sizeof(int), 1, file);
  if (err!=1) return STATE_SAVEMANAGER_CORRUPTED;

  err = fread(&g->NB_LigInit, sizeof(int), 1, file);
  if (err!=1) return STATE_SAVEMANAGER_CORRUPTED;

  initBlank(g, g->NB_Lig, g->NB_Col);

  err = fread(g->plateau, sizeof(caseVal), g->NB_Lig*g->NB_Col, file);
  if (err!=g->NB_Lig*g->NB_Col) return STATE_SAVEMANAGER_CORRUPTED;

  err = fread(g->plateau_s, sizeof(highlight_info), g->NB_Lig*g->NB_Col, file);
  if (err!=g->NB_Lig*g->NB_Col) return STATE_SAVEMANAGER_CORRUPTED;

  err = fread(&g->jCourant, sizeof(int), 1, file);
  if (err!=1) return STATE_SAVEMANAGER_CORRUPTED;

  for (int i = 0; i < 2; i++) {
    int lenname = 0;
    err = fread(&lenname, sizeof(int), 1, file);
    if (err!=1) return STATE_SAVEMANAGER_CORRUPTED;
    g->tabJ[i].nom = NULL;
    if (lenname > 0) {
      g->tabJ[i].nom = malloc(200*sizeof(char));
      err = fread(g->tabJ[i].nom, sizeof(char), lenname, file);
      if (err!=lenname) return STATE_SAVEMANAGER_CORRUPTED;
      g->tabJ[i].nom[lenname] = '\0';
    }
    err = fread(&g->tabJ[i].coul, sizeof(couleur), 1, file);
    if (err!=1) return STATE_SAVEMANAGER_CORRUPTED;
    err = fread(&g->tabJ[i].type, sizeof(typeJoueur), 1, file);
    if (err!=1) return STATE_SAVEMANAGER_CORRUPTED;
  }

  err = fread(&g->size_jeux, sizeof(int), 1, file);
  if (err!=1) return STATE_SAVEMANAGER_CORRUPTED;

  err = fread(&g->len_jeux, sizeof(int), 1, file);
  if (err!=1) return STATE_SAVEMANAGER_CORRUPTED;

  g->jeux = realloc(g->jeux,g->size_jeux*sizeof(state_jeu));
  for (int i = 0; i < g->len_jeux; i++) {
    err = fread(&g->jeux[i].nb, sizeof(int), 1, file);
    if (err!=1) return STATE_SAVEMANAGER_CORRUPTED;
    err = fread(&g->jeux[i].plateau, sizeof(caseVal), g->NB_Lig*g->NB_Col, file);
    if (err!=g->NB_Lig*g->NB_Col) return STATE_SAVEMANAGER_CORRUPTED;
  }

  err = fread(&g->is_center, sizeof(int), 1, file);
  if (err!=1) return STATE_SAVEMANAGER_CORRUPTED;

  return STATE_SAVEMANAGER_SUCCESS;
}

/**
 * savemanager_load_save : permet de charger une sauvegarde qui se trouve dans le fichier "save.data" (si l'opération est un succès alors la variable jeu* passée en argument contiendra le jeu chargé)
 *
 * @param {jeu*} g - un pointer vers une variable de type jeu (@see structures.h) (là où on mettra tout ce qu'on a pu chargé du fichier "save.data")
 * @param {char*} s - nom de la sauvegarde à charger
 *
 * @return cette fonction retourne une variable de type state_savemanager qui indique l'état de l'opération (s'il y a eu une erreur, si c'est un succès..etc..)
 */
state_savemanager savemanager_load_save(jeu*g,char *s) {
  FILE* file = fopen(FILE_SAVE,"r+b");
  if (file==NULL) return STATE_SAVEMANAGER_ERROR;
  jeu *gametmp = calloc(1,sizeof(jeu));
  int len;
  int err = 0;

  fseek(file,0,SEEK_END);
  long sizefile = ftell(file);
  fseek(file,0,SEEK_SET);

  while(!feof(file)) {

    long start = ftell(file);

    // get save name
    err = fread(&len, sizeof(int), 1, file);
    if (len > MAX_LEN_SAVE_NAME) {fclose(file);free(gametmp);return STATE_SAVEMANAGER_CORRUPTED;}
    if (err!=1) {fclose(file);free(gametmp);return STATE_SAVEMANAGER_CORRUPTED;}
    char namesave[len+1];
    namesave[len] = '\0';
    err = fread(namesave, sizeof(char), len, file);
    if (err!=len) {fclose(file);free(gametmp);return STATE_SAVEMANAGER_CORRUPTED;}

    // get save size
    long sizesave = 0;
    err = fread(&sizesave, sizeof(long), 1, file);
    if (err!=1) {fclose(file);free(gametmp);return STATE_SAVEMANAGER_CORRUPTED;}

    // DEBUG:
    //printf("current save : "BRED"%s"RESET"\n",namesave);
    //printf("len          : %d\n",len);
    //printf("size         : %ld\n",sizesave);

    // if we found the save
    if (strcmp(namesave,s)==0) {

      state_savemanager isloaded = savemanager_load_save_(file, gametmp);


      fclose(file);

      if (isloaded!=STATE_SAVEMANAGER_SUCCESS) {
        free_jeu(gametmp);
        return isloaded;
      }

      if (g->plateau!=NULL) free(g->plateau);
      if (g->plateau_s!=NULL) free(g->plateau_s);
      if (g->jeux!=NULL) free(g->jeux);
      if (g->tabJ[0].nom!=NULL) free(g->tabJ[0].nom);
      if (g->tabJ[1].nom!=NULL) free(g->tabJ[1].nom);
      for (int i = 0; i < g->len_jeux; i++) {
        if (g->jeux[i].plateau!=NULL) free(g->jeux[i].plateau);
      }

      memcpy(g, gametmp, sizeof(jeu));
      free(gametmp);
      
      return STATE_SAVEMANAGER_SUCCESS;
    }

    // else we skip this save
    fseek(file,start,SEEK_SET);
    fseek(file,sizesave,SEEK_CUR);

    if (ftell(file)==sizefile) break;

  }
  free(gametmp);
  fclose(file);
  return STATE_SAVEMANAGER_UNKNOW;
}

/**
 * savemanager_remove_save : permet de supprimer une sauvegarde qui se trouve dans le fichier "save.data"
 *
 * @param {char*} s - nom de la sauvegarde à supprimer
 *
 * @return cette fonction retourne une variable de type state_savemanager qui indique l'état de l'opération (s'il y a eu une erreur, si c'est un succès..etc..)
 */
state_savemanager savemanager_remove_save(char *s) {

  FILE* file = fopen(FILE_SAVE,"r+b");
  if (file==NULL) return STATE_SAVEMANAGER_ERROR;
  int len;
  int err = 0;

  long start = 0;
  long end = 0;

  long tmpindex = 0;

  int mode = 0;

  fseek(file,0,SEEK_END);
  long sizefile = ftell(file);
  fseek(file,0,SEEK_SET);

  FILE* filetmp = NULL;

  while(!feof(file)) {

    // write all other save in temp file
    if (mode) {
      int c;
      if ((c = fgetc(file)) == EOF) break;
      long pos = ftell(file);
      if (pos==start) mode = 2;
      if (pos==end) mode = 1;
      if (mode==1) fputc(c, filetmp);
      continue;
    }

    tmpindex = ftell(file);

    // get save name
    err = fread(&len, sizeof(int), 1, file);
    if (err!=1) {fclose(file);return STATE_SAVEMANAGER_CORRUPTED;}
    char namesave[len+1];
    namesave[len] = '\0';
    err = fread(namesave, sizeof(char), len, file);
    if (err!=len) {fclose(file);return STATE_SAVEMANAGER_CORRUPTED;}

    // get save size
    long sizesave = 0;
    err = fread(&sizesave, sizeof(long), 1, file);
    if (err!=1) {fclose(file);return STATE_SAVEMANAGER_CORRUPTED;}

    // if we found the save
    if (strcmp(namesave,s)==0) {
      start = tmpindex;
      end = tmpindex+sizesave;
      mode = 1;
      filetmp = fopen("tmp"FILE_SAVE,"w+b");
      if (file==NULL) {
        fclose(file);
        return STATE_SAVEMANAGER_ERROR;
      }
      fseek(file,0,SEEK_SET);
      continue;
    }

    // else we skip this save
    fseek(file,tmpindex,SEEK_SET);
    fseek(file,sizesave,SEEK_CUR);

    if (ftell(file)==sizefile) break;

  }

  fclose(file);
  if (mode) {
    fclose(filetmp);
    if (remove(FILE_SAVE)==0) {
      if (rename("tmp"FILE_SAVE,FILE_SAVE)==0) {
        return STATE_SAVEMANAGER_SUCCESS;
      }
    }
    return STATE_SAVEMANAGER_ERROR;
  } else {
    return STATE_SAVEMANAGER_UNKNOW;
  }
}

/**
 * savemanager_count_save : permet de compter le nombre de sauvegarde qui se trouve dans le fichier "save.data"
 *
 * @TODO: retourner un état "state_savemanager" et retourner la nombre de sauvegarde via un int*
 *
 * @return cette fonction retourne -1 s'il y a eu une erreur, sinon elle retourne le nombre de sauvegarde
 */
int savemanager_count_save() {

  // on ouvre le fichier
  FILE* file = fopen(FILE_SAVE,"r+b");
  if (file==NULL) return -1;

  int err = 0; // on stockera dans cette variable les erreurs de lecture

  int len; // contiendra la longueur du nom des sauvegardes

  int nb_saves = 0; // on stocke le nombre de sauvegardes

  // on récupère la taille du fichier
  fseek(file,0,SEEK_END);
  long sizefile = ftell(file);
  fseek(file,0,SEEK_SET);

  while(!feof(file)) {

    long start = ftell(file);

    // get save name
    err = fread(&len, sizeof(int), 1, file);
    if (err!=1) {fclose(file);return -1;}
    char namesave[len+1];
    namesave[len] = '\0';
    err = fread(namesave, sizeof(char), len, file);
    if (err!=len) {fclose(file);return -1;}

    // get save size
    long sizesave = 0;
    err = fread(&sizesave, sizeof(long), 1, file);
    if (err!=1) {fclose(file);return -1;}

    nb_saves++;

    // else we skip this save
    fseek(file,start,SEEK_SET);
    fseek(file,sizesave,SEEK_CUR);

    if (ftell(file)==sizefile) break;

  }

  fclose(file);

  return nb_saves;
}

/**
 * savemanager_getindex : permet de retourner les positions dans le fichier de toutes les sauvegardes dans un tableau de type long (ça permet d'accèder aux positions des sauvegardes dans le fichier beaucoup plus facilement)
 *
 * @param {int*} length - On mettra dans cette variable la taille du tableau retourné
 *
 * @return cette fonction retourne NULL s'il y a eu une erreur, sinon elle retourne un tableau de type long contenant les positions de toutes les sauvegardes dans le fichier "save.data"
 */
long* savemanager_getindex(int* length) {

  *length = 0;
  int nb_saves = savemanager_count_save();
  if (nb_saves <= 0) return NULL;

  long* index = malloc(nb_saves*sizeof(long));
  if (index==NULL) return NULL;
  int i = 0;

  // on ouvre le fichier
  FILE* file = fopen(FILE_SAVE,"r+b");
  if (file==NULL) {free(index);return NULL;}

  int err = 0; // on stockera dans cette variable les erreurs de lecture

  int len; // contiendra la longueur du nom des sauvegardes

  // on récupère la taille du fichier
  fseek(file,0,SEEK_END);
  long sizefile = ftell(file);
  fseek(file,0,SEEK_SET);

  while(!feof(file)) {

    long start = ftell(file);

    // get save name
    err = fread(&len, sizeof(int), 1, file);
    if (err!=1) {fclose(file);free(index);return NULL;}
    char namesave[len+1];
    namesave[len] = '\0';
    err = fread(namesave, sizeof(char), len, file);
    if (err!=len) {fclose(file);free(index);return NULL;}

    // get save size
    long sizesave = 0;
    err = fread(&sizesave, sizeof(long), 1, file);
    if (err!=1) {fclose(file);free(index);return NULL;}

    if (i>nb_saves-1) {
      free(index);
      fclose(file);
      return NULL;
    }
    index[i++] = start;

    // else we skip this save
    fseek(file,start,SEEK_SET);
    fseek(file,sizesave,SEEK_CUR);

    if (ftell(file)==sizefile) break;

  }

  fclose(file);

  *length = i;
  return index;
}

/**
 * savemanager_getnames : permet de retourner les noms de toutes les sauvgardes dans un tableau qui se trouvent dans le fichier "save.data"
 *
 * @param {int*} length - On mettra dans cette variable la taille du tableau retourné
 *
 * @return cette fonction retourne NULL s'il y a eu une erreur, sinon elle retourne un tableau de type char* contenant les noms de toutes les sauvegardes dans le fichier "save.data"
 */
char** savemanager_getnames(int* length) {

  *length = 0;
  int nb_saves = savemanager_count_save();
  if (nb_saves <= 0) return NULL;

  char **names = calloc(nb_saves,sizeof(char*));
  if (names==NULL) return NULL;
  int i = 0;

  // on ouvre le fichier
  FILE* file = fopen(FILE_SAVE,"r+b");
  if (file==NULL) {free(names);return NULL;}

  int err = 0; // on stockera dans cette variable les erreurs de lecture

  int len; // contiendra la longueur du nom des sauvegardes

  // on récupère la taille du fichier
  fseek(file,0,SEEK_END);
  long sizefile = ftell(file);
  fseek(file,0,SEEK_SET);

  while(!feof(file)) {

    long start = ftell(file);

    // get save name
    err = fread(&len, sizeof(int), 1, file);
    if (err!=1) {fclose(file);for(int j = 0; j < i; j++){if(names[j]!=NULL){free(names[j]);}}free(names);return NULL;}
    char namesave[len+1];
    namesave[len] = '\0';
    err = fread(namesave, sizeof(char), len, file);
    if (err!=len) {fclose(file);for(int j = 0; j < i; j++){if(names[j]!=NULL){free(names[j]);}}free(names);return NULL;}

    // get save size
    long sizesave = 0;
    err = fread(&sizesave, sizeof(long), 1, file);
    if (err!=1) {fclose(file);for(int j = 0; j < i; j++){if(names[j]!=NULL){free(names[j]);}}free(names);return NULL;}

    if (i>nb_saves-1) {
      for(int j = 0; j < i; j++){if(names[j]!=NULL){free(names[j]);}}
      free(names);
      fclose(file);
      return NULL;
    }
    names[i] = malloc((len+1)*sizeof(char));
    names[i][0] = '\0';
    strcat(names[i],namesave);
    i++;

    // else we skip this save
    fseek(file,start,SEEK_SET);
    fseek(file,sizesave,SEEK_CUR);

    if (ftell(file)==sizefile) break;

  }

  fclose(file);

  *length = i;
  return names;
}

/**
 * savemanager_load_save_by_index : permet de charger une sauvegarde en fournissant sa position dans le fichier "save.data" (si l'opération est un succès alors la variable jeu* passée en argument contiendra le jeu chargé)
 *
 * @param {jeu*} g - un pointer vers une variable de type jeu (@see structures.h) (là où on mettra tout ce qu'on a pu chargé du fichier "save.data")
 * @param {long} index - nom de la sauvegarde à charger
 *
 * @return cette fonction retourne une variable de type state_savemanager qui indique l'état de l'opération (s'il y a eu une erreur, si c'est un succès..etc..)
 */
state_savemanager savemanager_load_save_by_index(jeu*g,long index) {

  // on ouvre le fichier
  FILE* file = fopen(FILE_SAVE,"r+b");
  if (file==NULL) return STATE_SAVEMANAGER_ERROR;

  jeu *gametmp = calloc(1,sizeof(jeu));

  int err = 0; // on stockera dans cette variable les erreurs de lecture

  int len; // contiendra la longueur du nom des sauvegardes

  // on récupère la taille du fichier
  fseek(file,0,SEEK_END);
  long sizefile = ftell(file);
  fseek(file,0,SEEK_SET);

  while(!feof(file)) {

    long start = ftell(file);

    // get save name
    err = fread(&len, sizeof(int), 1, file);
    if (err!=1) {fclose(file);free(gametmp);return STATE_SAVEMANAGER_CORRUPTED;}
    char namesave[len+1];
    namesave[len] = '\0';
    err = fread(namesave, sizeof(char), len, file);
    if (err!=len) {fclose(file);free(gametmp);return STATE_SAVEMANAGER_CORRUPTED;}

    // get save size
    long sizesave = 0;
    err = fread(&sizesave, sizeof(long), 1, file);
    if (err!=1) {fclose(file);free(gametmp);return STATE_SAVEMANAGER_CORRUPTED;}

    if (index==start) {

      state_savemanager isloaded = savemanager_load_save_(file, gametmp);

      fclose(file);

      if (isloaded!=STATE_SAVEMANAGER_SUCCESS) {
        free_jeu(gametmp);
        return isloaded;
      }

      if (g->plateau!=NULL) free(g->plateau);
      if (g->plateau_s!=NULL) free(g->plateau_s);
      if (g->jeux!=NULL) free(g->jeux);
      if (g->tabJ[0].nom!=NULL) free(g->tabJ[0].nom);
      if (g->tabJ[1].nom!=NULL) free(g->tabJ[1].nom);
      for (int i = 0; i < g->len_jeux; i++) {
        if (g->jeux[i].plateau!=NULL) free(g->jeux[i].plateau);
      }

      memcpy(g, gametmp, sizeof(jeu));
      free(gametmp);
      
      return STATE_SAVEMANAGER_SUCCESS;
    }

    // else we skip this save
    fseek(file,start,SEEK_SET);
    fseek(file,sizesave,SEEK_CUR);

    if (ftell(file)==sizefile) break;

  }

  free(gametmp);
  fclose(file);

  return STATE_SAVEMANAGER_UNKNOW;
}

/**
 * savemanager_submenu_save_load : charge la sauvegarde choisi par le joueur lorsque le joueur sélectionne le bouton "CHARGER" dans le menu
 *
 * @Event {menu_event} (fonction appelé lors du choix d'un item dans le menu) (@see menu.h) (les arguments sont décrits dans menu.h et ce qui est retourné)
 *
 */
menu_message savemanager_submenu_save_load(menu* m, menu_item* item, int i) {
  jeu *tempg = calloc(1,sizeof(jeu));
  if (tempg==NULL) return MENU_NO_MESSAGE;
  state_savemanager isloaded = savemanager_load_save(tempg,m->items[0]->name);
  if (isloaded==STATE_SAVEMANAGER_SUCCESS) {
    jeu* g = (jeu*)m->data;
    if (g->plateau!=NULL) free(g->plateau);
    if (g->plateau_s!=NULL) free(g->plateau_s);
    if (g->jeux!=NULL) free(g->jeux);
    if (g->tabJ[0].nom!=NULL) free(g->tabJ[0].nom);
    if (g->tabJ[1].nom!=NULL) free(g->tabJ[1].nom);
    for (int i = 0; i < g->len_jeux; i++) {
      if (g->jeux[i].plateau!=NULL) free(g->jeux[i].plateau);
    }
    memcpy(g, tempg, sizeof(jeu));
    free(tempg);
    return 1;
  }
  free(tempg);
  resetTerm();
  if (item->data!=NULL) {
    print_info_damier((jeu*)item->data,m->items[0]->name,0,1);
    printf("\n");
  }
  // TODO: display message error if load fail
  return MENU_NO_MESSAGE;
}

/**
 * savemanager_submenu_save_delete : supprime la sauvegarde choisi par le joueur lorsque le joueur sélectionne le bouton "SUPPRIMER" dans le menu
 *
 * @Event {menu_event} (fonction appelé lors du choix d'un item dans le menu) (@see menu.h) (les arguments sont décrits dans menu.h et ce qui est retourné)
 *
 */
menu_message savemanager_submenu_save_delete(menu* m, menu_item* item, int i) {
  resetTerm();
  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);
  for (int j = 0; j < w/2-12; j++) printf(" ");
  printf(BRED"["YEL"SOUHAITEZ-VOUS SUPPRIMER\n");
  for (int j = 0; j < w/2-(17+m->items[0]->length)/2; j++) printf(" ");
  printf("LA SAUVEGARDE \""GRN"%s"YEL"\""BRED"]"RESET"\n",m->items[0]->name);
  printf("\n");
  menu *m2 = menu_create();
  menu_item* item_yes = menu_create_item(GRN"OUI");
  menu_item* item_no = menu_create_item(RED"NON");
  item_yes->length = 3;
  item_no->length = 3;
  menu_insert_item(m2, item_yes);
  menu_insert_item(m2, item_no);
  menu_item_choice choice = print_menu(m2,1,8,2,BBLU,BRED);
  resetTerm();
  if (item->data!=NULL) {
    print_info_damier((jeu*)item->data,m->items[0]->name,0,1);
    printf("\n");
  }
  free_menu(m2);
  if (choice.index==0) {
    savemanager_remove_save(m->items[0]->name);
    return MENU_LEFT_ALL;
  } else {
    return MENU_NO_MESSAGE;
  }
}

/**
 * savemanager_submenu_save : lorsque le joueur choisi une sauvegarde, on affiche le menu : "CHARGER", "RENOMMER", "SUPPRIMER", "RETOUR"
 *
 * @Event {menu_event} (fonction appelé lors du choix d'un item dans le menu) (@see menu.h) (les arguments sont décrits dans menu.h et ce qui est retourné)
 *
 */
menu_message savemanager_submenu_save(menu* m, menu_item* item, int i) {

  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);

  resetTerm();
  jeu *tempg = calloc(1,sizeof(jeu));

  state_savemanager isloaded = savemanager_load_save(tempg,item->name);

  menu *m2 = menu_create();

  m2->data = m->data;

  menu_item* itemn = menu_create_item(item->name);
  itemn->is_hide = 1;
  menu_insert_item(m2, itemn);

  menu_message msg = MENU_NO_MESSAGE;

  if (isloaded==STATE_SAVEMANAGER_SUCCESS) {
    print_info_damier(tempg,item->name,0,1);
    printf("\n");
    menu_item* item1 = menu_create_item(GRN"CHARGER");
    item1->length = 7;
    item1->data = (void*)tempg;
    item1->_fn = savemanager_submenu_save_load;
    menu_item* item2 = menu_create_item(MAG"RENOMMER");
    item2->length = 8;
    item2->data = (void*)tempg;
    menu_item* item3 = menu_create_item(YEL"SUPPRIMER");
    item3->length = 9;
    item3->data = (void*)tempg;
    item3->_fn = savemanager_submenu_save_delete;
    menu_insert_item(m2, item1);
    menu_insert_item(m2, item2);
    menu_insert_item(m2, item3);
    menu_insert_separator(m2);
    menu_insert_item(m2, menu_create_item("RETOUR"));
    menu_item_choice choice = print_menu(m2,3,8,2,BBLU,BRED);
    msg = choice.message;
  } else {
    for (int j = 0; j < w/2-12; j++) printf(" ");
    printf(BRED"["YEL"UNE ERREUR EST SURVENUE\n");
    for (int j = 0; j < w/2-19; j++) printf(" ");
    printf("PENDANT LE CHARGEMENT DE LA SAUVEGARDE"BRED"]"RESET"\n");
    printf("\n");
    menu_insert_item(m2, menu_create_item("RETOUR"));
    menu_item_choice choice = print_menu(m2,0,8,2,BBLU,BRED);
  }

  resetTerm();

  free_menu(m2);
  free(tempg);
  return msg;
}

/**
 * savemanager_menu : permet d'afficher la liste des sauvegardes sous forme d'un menu où le joueur peut sélectionner une sauvegarde pour ainsi obtenir plus d'info sur celle-ci et pour pouvoir la chargée ou bien la supprimée selon le choix du joueur
 *
 * @param {jeu*} g - un pointer vers une variable de type jeu (@see structures.h) (là où on mettra la sauvegarde qu'on a chargé du fichier "save.data")
 *
 * @return cette fonction retourne 0 si le joueur n'a rien chargé où s'il y a eu une erreur, sinon cette fonction renvoie 1 pour indiqué que le joueur a chargé une partie
 */
int savemanager_menu(jeu*g) {

  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);

  int current_page = 1;
  int tmp_names_length = 0;
  int names_length = 0;


  int menu_index = 0;

  while(1) {


    names_length = 0;
    char **names = savemanager_getnames(&names_length);

    int nb_items = 2;
    int nb_pages = names_length/nb_items+1-((names_length%nb_items)==0?1:0);

    if (names_length!=tmp_names_length) {
      if (current_page>nb_pages) current_page = nb_pages;
      menu_index = 0;
    }

    tmp_names_length = names_length;

    resetTerm();

    menu* m = menu_create();

    m->data = (void*)g;

    char textpage[32];
    sprintf(textpage,"(Page %d/%d)",current_page,nb_pages);
    

    menu_insert_custom_separator(m,"LISTE DES SAUVEGARDES :");
    menu_insert_custom_separator(m,textpage);
    menu_insert_separator(m);
    int j = 0;
    for (int i = (current_page-1)*nb_items; i < current_page*nb_items; i++) {
      if (i > names_length-1) break;
      menu_item* item = menu_create_item(names[i]);
      item->_fn = savemanager_submenu_save;
      menu_insert_item(m, item);
      j++;
    }

    menu_insert_separator(m);

    int previous_index = -3;
    int next_index = -3;

    if (nb_pages > 1) {
      if (current_page>1) {
        menu_item* previous_item = menu_create_item(MAG"<- PRECEDENT");
        previous_item->length = 12;
        menu_insert_item(m, previous_item);
        previous_index = j;
        j++;
        if (menu_index==-2) menu_index = previous_index;
      }
      if (current_page<nb_pages) {
        menu_item* next_item = menu_create_item(MAG"SUIVANT ->");
        next_item->length = 10;
        menu_insert_item(m, next_item);
        next_index = j;
        j++;
        if (menu_index==-1) menu_index = next_index;
      }
      if (menu_index==-2) menu_index = next_index;
      if (menu_index==-1) menu_index = previous_index;
    }

    if (menu_index<0) menu_index = 0;
    menu_insert_item(m, menu_create_item("RETOUR"));

    menu_item_choice choice = print_menu(m,menu_index,8,2,BBLU,BRED);

    // if previous
    if (choice.index==previous_index) {
      current_page--;
      menu_index = -2;
    }

    // if next
    else if (choice.index==next_index) {
      current_page++;
      menu_index = -1;
    }

    

    for (int i = 0; i < names_length; i++) {
      if (names[i]!=NULL) free(names[i]);
    }
    free(names);

    free_menu(m);

    if (choice.message>0) return 1;

    if (choice.index==j) break;

  }

  /*
  // TODO: create foreach function that call callback function with loaded save in argument to print with appropriate function

  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);

  // on compte le nombre de sauvegardes
  int nb_saves = savemanager_count_save();

  // on calcule le nombre d'items par page
  int nb_items = h/8-1;

  jeu *tempg = calloc(1,sizeof(jeu));
  if (tempg==NULL) return -1;

  // on recupère les indices des sauvegardes
  int index_length = 0;
  long *index = savemanager_getindex(&index_length);
  if (index==NULL) {free(tempg);return -1;}

  printf("nb saves = %d\n",nb_saves);
  printf("nb items = %d\n",nb_items);

  if (nb_saves <= nb_items) {
    for (int i = 0; i < nb_saves; i++) {
      //print_info_damier(&gametmp,"masave",20);
    }
  } else {
    for (int i = 0; i < nb_saves; i++) {
      if (savemanager_load_save_by_index(tempg,index[i])==STATE_SAVEMANAGER_SUCCESS) {
        print_info_damier(tempg,"masave",20);
        printf("\n");
      }
    }
  }

  if (index!=NULL) free(index);
  free(tempg);*/

  return 0;
}


#endif