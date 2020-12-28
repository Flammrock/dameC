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


// TODO: ajouter un gestionnaire pour facilité la
//       gestion des "additional data" (voir plus bas)
// TODO: implémenter une fonction pour compter le
//       nombre de caractères dans une chaîne sans compter les échappements ANSI
// TODO: ajouter un comportement de contrôle et de communication plus élaboré


#ifndef CHECKERS_MENU_H
#define CHECKERS_MENU_H


/**********************\
*                      *
*       INCLUDE        *
*                      *
\**********************/

#include <stdio.h>
#include <string.h>
#include <math.h> // log10 (afin de compter le nom de digits dans un nombre)

#include "display.h" // pour pouvoir effectuer des opérations sur la console




/*
 __  __                  
|  \/  |                 
| \  / | ___ _ __  _   _ 
| |\/| |/ _ \ '_ \| | | |
| |  | |  __/ | | | |_| |
|_|  |_|\___|_| |_|\__,_|

*/





/**********************\
*                      *
*    STRUCT & ENUM     *
*                      *
\**********************/

/**
 * Utilisé pour implémenter un item qui permet de modifier une valeur
 * Donne le type de la variable passée en pointeur castée en void*
 */
typedef enum menu_type {
  MENU_INT,
  MENU_CHAR,  /*not yet implemented*/
  MENU_STRING /*not yet implemented*/
} menu_type;

struct menu;



// Utilisé pour faire un simple système de communication entre les fonctions appelés lors du choix des items et du menu
typedef int menu_message;

// On retourne cette valeur pour indiquer qu'on ne fait rien de particulier on qu'on continue à afficher le menu
#define MENU_NO_MESSAGE 0

// On retourne cette valeur pour indiquer qu'on quitte le menu d'un niveau
#define MENU_LEFT -1

// On retourne cette valeur pour indiquer qu'on quitte tous les menus empiler
#define MENU_LEFT_ALL -2




/**
 * Structure d'un Item qui sera ajouté dans un menu
 */
typedef struct menu_item {

  // La chaîne de caractères à afficher dans le menu
  // @default : NULL
  char *name;

  // La longueur de la chaîne du champ "nom" (cette valeur est précalculé pour simplifier un poil la complexité)
  // @default : 0
  int length;

  // Si le joueur sélectionne cet Item, si ce champ pointe vers un menu alors on empile ce menu dans une pile
  // Et on affiche ce sous-menu
  // @default : NULL
  struct menu *submenu;

  // Si cet Item est juste un séparateur alors il n'est pas sélectionner (permet d'afficher du texte dans le menu et de séparer les Items du menu)
  // @default : 0
  int is_separator; // 1 => séparateur, 0 => un simple item

  // Si cet Item est une option alors il permet de modifier une valeur de manière interactive
  //    - le champ "option_value" doit nécesairement pointer vers une variable
  //    - le type de la variable pointé par le champ "option_value" doit être indiqué dans le champ "option_type"
  // @default : 0
  int is_option; // 1 => option, 0 => un simple item

  // Si cet Item est caché alors il n'est pas sélectionnable et il est invicible
  // (utile pour par exemple cacher des données un peu de la même manière qu'avec les "<input type="hidden"></>" en HTML5)
  // @default : 0
  int is_hide; // 1 => caché, 0 => un simple item

  // Si cet Item est un bouton retour alors si cet Item est sélectionné il permet de dépiler la pile et de revenir
  // dans le menu précédent (permet de créer un système de sous-menu très facilement)
  // @default : 0
  int is_back; // 1 => bouton retour, 0 => un simple item

  // @event menu_event
  // Si ce pointeur pointe vers fonction est non NULL, alors si le joueur choisi cette Item : cette fonction est appelée
  //
  //    /**
  //     * _fn : @event
  //     *
  //     * @param {menu*} - on passe le menu où se trouve cet Item
  //     * @param {menu_item*} - on passe cet Item lui-même en argument
  //     * @param {int} - position de l'Item dans le menu
  //     *
  //     * @return {menu_message} - retourne un nombre qui suivant ça valeur changera le comportement du menu
  //     */
  //
  // Ce champ contient une fonction, ceci permet de propager l'événèment (CHOIX DE CET ITEM) dans une autre fonction
  // pour laisser plus de liberté dans l'implémentation d'un menu
  // @default : NULL
  menu_message (*_fn)(struct menu*,struct menu_item*,int);

  // Pointe vers une variable qui pourra être modifier par le joueur (attention le champ "is_option" doit être à 1)
  // @default : NULL
  void* option_value;

  // On indique le type de la variable pointé par le champ "option_value" ici (@see menu_type (ci-dessus))
  // @default : MENU_INT
  menu_type option_type;

  // additional data
  // le développeur met ce qu'il veut dedans, ce sera à lui de gérer ces données
  // @default : NULL
  void* data;

} menu_item;

/**
 * Structure d'un menu
 */
typedef struct menu {

  // Tableau de pointeurs qui pointe vers des variables de type menu_item (@see menu_item)
  menu_item **items;

  // Permet de centrer le menu dans la console
  // @default : 1
  int is_center; // 1 => centré, 0 => non-centré

  // Contient le nombre d'Items contenu dans le menu
  int length;

  // Contient la taille alloué en mémoire pour le tableau
  int size;

  // additional data
  // le développeur met ce qu'il veut dedans, ce sera à lui de gérer ces données
  // @default : NULL
  void *data;

} menu;

/**
 * Structure d'un élément d'une pile de type "menu_pile" (@see menu_pile)
 * Cette structure permet d'empiler des menus (utilisé pour pouvoir implémenter un système de sous-menus)
 */
typedef struct menu_pile_item {

  // Un pointeur qui pointe vers un menu
  menu* m;

  // Un pointeur qui pointe vers le prochain menu dans la liste
  struct menu_pile_item* next;

  // Position du menu_item (@see menu_item) qui a amené à empiler ce menu
  // Permet de retourner sur l'Item du menu parent lorsqu'on sélectionne un menu_item avec son champ "is_back" à 1
  int index;

} menu_pile_item;

/**
 * Structure d'une pile où on empile des menus
 * (inspiré de la structure move (@see structures.h))
 */
typedef struct menu_pile {
  menu_pile_item *first; // la pile (liste chaînée)
  menu_pile_item *last;  // le dernier élément empilé (le dernier élément de la liste chaînée)
} menu_pile;

/**
 * Structure d'un choix d'item
 * Lorsque le joueur choisi un item dans le menu, la fonction chargée de faire fonctionner le menu (ci-dessous)
 * retournera une variable de type "menu_item_choice" qui contiendra toutes les informations nécessaires pour savoir quel item a été sélectionné par le joueur
 *
 * @TODO: retourner aussi l'item en question
 */
typedef struct menu_item_choice {

  // Position de l'item dans le menu (attention, ça retourne la position de l'item parmi les items sélectionnables (les séparateurs et les items cachés ne seront pas comptés))
  int index;

  // Le menu dans lequel se trouve l'item sélectionné (utile pour savoir si la sélectionnée s'est fait dans un sous-menu (il suffira de comparer les pointers))
  menu* m;

  // Si une fonction (champ _fn d'un menu_item (@see menu_item)) _fn d'un item est exécutée, s'il s'agit d'un message
  // qui quitte le menu, on pourra retrouver ça valeur ici pour pouvoir la propagée dans la pile d'appelle (call stack)
  menu_message message;

} menu_item_choice;








/**********************\
*                      *
*       FUNCTIONS      *
*                      *
\**********************/



/**
 * menu_create_item_ : permet de créer un menu_item avec les valeurs par défaut (cette fonction est "privé" et ne peut être utiliser que dans ce fichier)
 *
 * @param {char*} name - nom de l'item (champ name)
 * @param {int} is_option - si 1 alors on construit un item option sinon 0 alors un item classique
 * @param {void*} value - pointeur qui pointe vers une variable (uniquement s'il s'agit d'une option)
 * @param {menu_item} option_type - type de la variable pointé par la variable value
 *
 * @return {menu_item*} retourne un menu_item (qu'on pourra ensuite ajouté dans un menu)
 */
static menu_item* menu_create_item_(char *name, int is_option, void* value, menu_type option_type) {
  menu_item* item = malloc(sizeof(menu_item));
  if (name!=NULL) {
    item->name = malloc((strlen(name)+1)*sizeof(char));
    item->name[0] = '\0';
    strcat(item->name,name);
    item->length = strlen(name);
  } else {
    item->name = NULL;
    item->length = 0;
  }
  item->_fn = NULL;
  item->submenu = NULL;
  item->is_separator = 0;
  item->is_back = 0;
  item->is_hide = 0;
  item->is_option = is_option;
  item->option_value = value;
  item->option_type = option_type;
  item->data = NULL;
  return item;
}

/**
 * menu_create_item : permet de créer un menu_item
 *
 * @param {char*} name - nom de l'item
 *
 * @return {menu_item*} retourne un menu_item (qu'on pourra ensuite ajouté dans un menu)
 */
menu_item* menu_create_item(char *name) {
  return menu_create_item_(name,0,NULL,0);
}

/**
 * menu_create_item_submenu : permet de créer un menu_item qui une fois sélectionné affiche un sous-menu
 *
 * @param {char*} name - nom de l'item
 * @param {menu*} m - sous-menu à afficher en cas de sélection
 *
 * @return {menu_item*} retourne un menu_item (qu'on pourra ensuite ajouté dans un menu)
 */
menu_item* menu_create_item_submenu(char *name,menu* m) {
  menu_item* item = menu_create_item_(name,0,NULL,0);
  item->submenu = m;
  return item;
}

/**
 * menu_create_separator : permet de créer un menu_item qui ne sera pas sélectionnable
 * l'item n'a pas de nom, ce n'est pas un séparateur custom (la fonction affichera seulement un "\n" pour cet item)
 *
 * @return {menu_item*} retourne un menu_item (qu'on pourra ensuite ajouté dans un menu)
 */
menu_item* menu_create_separator() {
  menu_item* item = menu_create_item_(NULL,0,NULL,0);
  item->is_separator = 1;
  return item;
}

/**
 * menu_create_back : permet de créer un menu_item qui une fois sélectionné permet de retourner au menu précédent
 * (à utiliser dans un sous-menu)
 *
 * @param {char*} name - nom de l'item
 *
 * @return {menu_item*} retourne un menu_item (qu'on pourra ensuite ajouté dans un menu)
 */
menu_item* menu_create_back(char *name) {
  menu_item* item = menu_create_item_(name,0,NULL,0);
  item->is_back = 1;
  return item;
}

/**
 * menu_create_custom_separator : permet de créer un menu_item qui ne sera pas sélectionnable
 * (ça affichera simplement du texte)
 *
 * @param {char*} name - nom de l'item
 *
 * @return {menu_item*} retourne un menu_item (qu'on pourra ensuite ajouté dans un menu)
 */
menu_item* menu_create_custom_separator(char *name) {
  menu_item* item = menu_create_item_(name,0,NULL,0);
  item->is_separator = 1;
  return item;
}

/**
 * menu_create_option : permet de créer un menu_item qui une fois sélectionné permettra de modifier une valeur de manière interactive (cet item aura son champ "is_option" à 1)
 *
 * @param {char*} name - nom de l'item
 * @param {void*} value - pointeur qui pointe vers une variable
 * @param {menu_item} option_type - type de la variable pointé par la variable value
 *
 * @return {menu_item*} retourne un menu_item (qu'on pourra ensuite ajouté dans un menu)
 */
menu_item* menu_create_option(char *name, void* value, menu_type option_type) {
  return menu_create_item_(name,1,value,option_type);
}

// simple déclaration
void free_menu(menu*);

/**
 * free_menu_item : permet de libérer absolument tout ce qui se trouve dans l'item (s'il y a un sous-menu alors le sous-menu est libéré)
 * Seul les données additionnelles et le champ option_value ne sont pas libérés
 *
 * @param {menu_item*} item - l'item à l'ibérer
 */
void free_menu_item(menu_item* item) {
  if (item==NULL) return;
  if (item->name!=NULL) free(item->name);
  if (item->submenu!=NULL) free_menu(item->submenu);
  free(item);
}

/**
 * free_menu : permet de libérer absolument tout ce qui se trouve dans un menu (les sous-menus et les items)
 * Seul les données additionnelles et le champ option_value ne sont pas libérés
 *
 * @param {menu*} m - le menu a libérer
 */
void free_menu(menu* m) {
  if (m==NULL) return;
  if (m->items!=NULL) {
    for (int i = 0; i < m->length; i++) {
      free_menu_item(m->items[i]);
    }
    free(m->items);
  }
  free(m);
}

/**
 * menu_create : permet de créer un menu (le menu retourner doit être par la suite être libérer avec la fonction free_menu)
 *
 * @return {menu*} retourne un menu (alloué dynamiquement)
 */
menu* menu_create() {
  menu* m = malloc(sizeof(menu));
  m->length = 0;
  m->size = 2;
  m->is_center = 1;
  m->items = malloc(m->size*sizeof(menu_item*));
  m->data = NULL;
  return m;
}

/**
 * menu_scale_items : fonction qui permet de réallouer le tableau qui contient tout les items (pour de l'agrandir)
 *
 * @param {menu*} m - menu à rescaler
 */
static void menu_scale_items(menu* m) {
  if (m->length+1 > m->size-1) {
    m->size *= 2;
    m->items = realloc(m->items,m->size*sizeof(menu_item*));
  }
}

/**
 * menu_insert_item : permet d'ajouter un item dans un menu
 *
 * @param {menu*} m - le menu où on va ajouter l'item
 * @param {menu_item*} item - l'item que l'on ajoute dans le menu
 */
void menu_insert_item(menu* m, menu_item* item) {
  m->items[m->length++] = item;
  menu_scale_items(m);
}

/**
 * menu_insert_separator : permet d'ajouter un séparateur directement dans le menu sans devoir créé un séparateur manuellement
 *
 * @param {menu*} m - le menu où l'on va ajouter un séparateur
 */
void menu_insert_separator(menu *m) {
  m->items[m->length++] = menu_create_separator();
  menu_scale_items(m);
}

/**
 * menu_insert_custom_separator : permet d'ajouter un séparateur custom directement dans le menu sans devoir créé un séparateur manuellement
 *
 * @param {menu*} m - le menu où l'on va ajouter un séparateur
 * @param {char*} name - nom du séparateur
 */
void menu_insert_custom_separator(menu *m, char *name) {
  m->items[m->length++] = menu_create_custom_separator(name);
  menu_scale_items(m);
}

/**
 * menu_count_digit : permet de compter le nombre de digits dans un nombre
 * (si le nombre est négatif, on compte aussi le signe moins)
 *
 * @param {int} n - un nombre où l'on souhaite compter ses digits
 *
 * @return {int} retourne le nombre de digits dans le nombre passé en argument
 */
int menu_count_digit(int n) {
  int digitCount = 1;
  int num = n;
  if(num < 0) num = num *-1;
  if(num){
    digitCount = (int)log10((double)num) + 1;
  }
  return n < 0 ? digitCount+1 : digitCount;
}

/**
 * menu_len_value : permet de calculer la longueur de la valeur pointéé par le champ "option_value" (dans l'item (@see menu_item))
 *
 * @TODO supporter d'autres types (seul les int sont supportés)
 *
 * @param {menu_item*} item - l'item où l'on souhaite calculer la longueur de la valeur pointée par le champ "option_value"
 *
 * @return {int} retourne le la longueur de la valeur pointée par le champ 
 */
int menu_len_value(menu_item *item) {
  if (item==NULL) return 0;
  if (!item->is_option) return 0;
  if (item->option_type==MENU_INT) {
    return menu_count_digit(*((int*)item->option_value));
  }
  return 0;
}

/**
 * menu_print_value : permet d'afficher sur la console la valeur pointée par le champ "option_value" (dans l'item (@see menu_item))
 *
 * @TODO supporter d'autres types (seul les int sont supportés)
 *
 * @param {menu_item*} item - l'item où l'on souhaite afficher la valeur pointée par le champ "option_value" sur la console
 */
void menu_print_value(menu_item *item) {
  if (item==NULL) return;
  if (!item->is_option) return;
  if (item->option_type==MENU_INT) {
    printf("%d",*((int*)item->option_value));
  }
}

/**
 * print_menu : permet de faire fonctionner un menu et de l'afficher sur la console et de faire naviguer dans le menu et les sous-menus le joueur
 *
 * @TODO : découper cette fonction en plein de petites fonctions pour simplifier la lisibilité et la rendre plus souple à des modifications/ajouts/suppressions
 *
 * @param {menu*} mn - le menu à afficher
 * @param {int} index - la position de l'item qu'on l'affiche par défaut lorsqu'on affiche le menu pour la 1ère fois
 * @param {int} margin - permet d'ajouter un peu de margin (négatif ou positif) pour pouvoir positionner le menu + à gauche ou + à droite
 * @param {char*} color_normal - échappement ANSI permettant de colorer les items lorsqu'ils ne sont pas sélectionnés (@see colors.h)
 * @param {char*} color_select - échappement ANSI permettant de colorer les items lorsqu'ils sont sélectionnés (@see colors.h)
 *
 * @return {menu_item_choice} retourne des informations sur l'item sélectionné
 */
menu_item_choice print_menu(menu* mn, int index, int margin, int margin_select, char *color_normal, char *color_select) {

  // On récupère la taille de la console pour pouvoir le centré par la suite
  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);

  // On prépare la pile (on va venir empiler les sous-menus dedans (et les dépiler quand le joueur sélectionnera un item de retour))
  menu_pile* pile = malloc(sizeof(menu_pile));

  // On empile le menu "principal"
  pile->first = malloc(sizeof(menu_pile_item));
  pile->first->m = mn;
  pile->first->next = NULL;
  pile->first->index = index;
  pile->last = pile->first;

  // On stocke le nombre d'items dans le menu courrant dans la variable "n"
  int n = mn->length;

  // On sotcke ici le message que nous enverra une fonction du champ _fn d'un menu_item (@see menu_item)
  menu_message msg = MENU_NO_MESSAGE;

  // @TODO : utiliser un define
  // Pour les menu_item options, on affiche au minimum 6 points (c'est juste pour le design)
  int nbpointmargin = 6;

  // Si cette valeur est à 0, on continue d'afficher le menu courrant, si cette variable vaut 1, alors on quitte le menu (et on libère les ressources alloués temporairement comme la pile)
  int cmdtmpt = 0;

  // Ctte variable contiendra le menu courrant
  menu *m = mn;

  while (1) {

    // On sélectionne le dernier menu empiler
    if (pile->last==NULL) m = mn;
    else m = pile->last->m;

    // On restocke dans "n" le nombre d'items
    n = m->length;

    // On précalcule la longueur maximum des valeurs des items (si ce sont des options)
    int lenmaxvalue = 0;

    // On précalcule la longueur maximum des noms des items
    int lenmaxname = 0;

    // On précalcule le nombre total de séparateurs
    int countsep = 0;

    // On précalcule le nombre total d'items cachés
    int counthide = 0;

    // On précalcule
    for (int i = 0; i < n; i++) {
      if (m->items[i]->name!=NULL) {
        int len = m->items[i]->length;
        if (len > lenmaxname) lenmaxname = len;
      }
      if (m->items[i]->is_separator||m->items[i]->is_hide) {
        countsep++;
      }
      if (m->items[i]->is_hide) {
        counthide++;
      }
      if (m->items[i]->is_option) {
        int len = menu_len_value(m->items[i]);
        if (len > lenmaxvalue) lenmaxvalue = len;
      }
    }

    // Variable temporaire qui contiendra le nombre courrant de séparateurs trouvés de puis l'item i
    int countsept = 0;

    // On fait une boucle for sur n pour pouvoir afficher tout les items
    for (int i = 0; i < n; i++) {

      // Si l'item est caché ou s'il s'agit d'un séparateur, alors on incrémente le nombre d'éléments non-sélectionnable
      if (m->items[i]->is_separator||m->items[i]->is_hide) countsept++;

      // Si l'item est caché alors, on affiche rien, on passe au prochain item
      if (m->items[i]->is_hide) continue;

      // Si l'item a un nom alors on va pouvoir afficher de manière stylé son nom
      if (m->items[i]->name!=NULL) {

        // On recupère la longueur du nom de l'item qui a été précalculer lors de la création d'item
        int len = m->items[i]->length;

        // On regarde si cet item est sélectionné
        int is_select = i-countsept==index;

        // En fonction de si l'item est sélectionné ou pas, on choisi la couleur adapté
        char *currentcolor = is_select ? color_select : color_normal;

        // S'il s'agit d'un séparateur custom
        if (m->items[i]->is_separator) {
          // on calcul le margin (en fonction de la longueur maximal et de la longueur du nom afin de centrer l'item avec les autres items)
          int marginsep = margin+1+lenmaxname/2-len/2;

          // On stocke le margin dans une variable (par forcément utile)
          char marginstrsep[marginsep+1];
          for (int j = 0; j < marginsep; j++) marginstrsep[j] = ' ';
          marginstrsep[marginsep] = '\0';

          // Si le menu est centré dans la console alors on rajoute des espaces pour le centré correctement
          if (m->is_center) {
            for (int i = 0; i < w/2-(lenmaxname+2*margin+2)/2; i++) printf(" ");
          }

          // Enfin, on affiche l'item
          printf("\e[2K%s%s%s%s\n"RESET,marginstrsep,color_normal,m->items[i]->name,marginstrsep);

          // On passe à l'item suivant
          continue;
        }

        // Si l'item est une option
        if (m->items[i]->is_option) {

          // On compte le nombre de point à rajouter de tel sorte à ce que tout soit aligné
          int nbpoint = nbpointmargin+lenmaxname-len;
          int lenmodtwo = len%2;
          int lenvalue = menu_len_value(m->items[i]);
          int nlen = len+nbpoint+2+lenmaxvalue;

          // On calcul le margin (qui est différent selon si l'item est sélectionné ou pas (c'est juste pour le design encore une fois)) (ceic est le margin à l'intérieur)
          int marginlen = is_select?margin_select:margin+lenmaxname/2-nlen/2;
          char marginstr[marginlen+1];
          for (int j = 0; j < marginlen; j++) marginstr[j] = ' ';
          marginstr[marginlen] = '\0';

          // On calcul le margin (qui est différent selon si l'item est sélectionné ou pas (c'est juste pour le design encore une fois)) (ceic est le margin à l'extérieur)
          int marginlenselect = is_select?margin-margin_select+lenmaxname/2-nlen/2:0;
          char marginstrselect[marginlenselect+1];
          for (int j = 0; j < marginlenselect; j++) marginstrselect[j] = ' ';
          marginstrselect[marginlenselect] = '\0';

          // Si le menu est centré dans la console alors on rajoute des espaces pour le centré correctement
          if (m->is_center) {
            for (int i = 0; i < w/2-(lenmaxname+2*margin+2)/2; i++) printf(" ");
          }

          // On affiche l'item
          printf("\e[2K%s%s>%s%s",marginstrselect,currentcolor,marginstr,m->items[i]->name);

          // On affiche les points
          for (int j = 0; j < nbpoint; j++) {
            if ((j+lenmodtwo+1)%2) printf(".");
            else printf(" ");
          }

          // On affiche la valeur
          printf("[");
          for (int j = 0; j < lenmaxvalue-lenvalue; j++) {
            printf(" ");
          }
          menu_print_value(m->items[i]);
          printf("]");

          // On affiche le reste
          printf("%s%s%s<\n"RESET,currentcolor,marginstr,!is_select?nlen%2==0?" ":"":"");

          // On passe à l'item suivant
          continue;
        }
        
        // Si on arrive ici, alors il s'agit d'un item classique qui ne nécessite aucun traitement particulier

        // On calcul le margin interne
        int marginlen = is_select?margin_select:margin+lenmaxname/2-len/2;
        char marginstr[marginlen+1];
        for (int j = 0; j < marginlen; j++) marginstr[j] = ' ';
        marginstr[marginlen] = '\0';

        // On calcul le margin interne
        int marginlenselect = is_select?margin-margin_select+lenmaxname/2-len/2:0;
        char marginstrselect[marginlenselect+1];
        for (int j = 0; j < marginlenselect; j++) marginstrselect[j] = ' ';
        marginstrselect[marginlenselect] = '\0';

        // Si le menu est centré dans la console alors on rajoute des espaces pour le centré correctement
        if (m->is_center) {
          for (int i = 0; i < w/2-(lenmaxname+2*margin+2)/2; i++) printf(" ");
        }

        // On affiche l'item
        printf("\e[2K%s%s>%s%s%s%s%s<\n"RESET,marginstrselect,currentcolor,marginstr,m->items[i]->name,currentcolor,marginstr,!is_select?len%2==0?" ":"":"");

      } else {
        // Si on arrive ici, alors l'item n'a pas de nom et n'est pas caché, on affiche simplement un saut de ligne
        printf("\n");
      }


    }

    // Si on arrive ici alors on a terminé d'afficher le menu (de le dessiner), maintenant on attend une action du joueur (on attend une entrée clavier)


    // Cette variable contiendra la valeur de la touche pressée
    int cp = 0;

    while(1) {
      cp = getch(); // fonction qui n'attend pas le retour à la ligne pour retourner le caractère (version légèrement modifier d'un scanf("%c",&cp))

      // @TODO : vérifier si certaine console renvoie les touches du flèche du bas et flèche du haut différemment

      // si la console fonctionne comme une console sur windows (cmd.exe)
	    if (cp==224) { // Si la touche est un CTRL (on suppose qu'on est dans le cas d'une console window)
		    cp = getch();
		    if (cp==80) {index++;if(index>=n-countsep){index=n-1-countsep;}break;}
        if (cp==72) {index--;if(index<0){index=0;}break;}
      }

      // sinon, la console fonctionne comme une console sur linuex
	    else if (cp==(int)'\e') { // en revanche, si on commence à recevoir un échappement ANSI, on suppose qu'on est dans le cas d'une console linux)
        cp = getch();
        if (cp==(int)'[') {
          cp = getch();
          if (cp=='B') {index++;if(index>=n-countsep){index=n-1-countsep;}break;}
          if (cp=='A') {index--;if(index<0){index=0;}break;}
          continue;
        }
      }

      // En revanche si le joueur appui sur entrée (10 sur linux et 13 sur windows), alors on suppose que le joueur a sélectionné l'item, en fonction de l'item (s'il contient un sous-menu, s'il s'agir d'une option, etc) on agira différment
      if (cp==10||cp==13) {cmdtmpt = 1;break;}
    }

    // Si le joueur a sélectionné un item
    if (cmdtmpt) {
      
      // On récupère la véritable position de l'item sélectionné parmi tout les items du menu (en prenant en compte le fait que certains items ne sont pas sélectionnables)
      int indexselected = 0;
      int countseptt = 0;
      for (int i = 0; i < n; i++) {
        if (m->items[i]->is_separator||m->items[i]->is_hide) countseptt++;
        if (i-countseptt==index) {
          indexselected = i;
          break;
        }
      }

      // On récupère l'item
      menu_item* item = m->items[indexselected];

      // On distingue différent cas suivant la nature de l'item

      // Si l'item est une option
      if (item->is_option) {

        // On remonte le curseur au niveau de l'item
        for (int i = 0; i < n-counthide-indexselected; i++) printf("\e[F");

        // On efface la ligne (pour montrer qu'on change bien la valeur de cet item en particulier)
        printf("\e[2K");

        // On place le curseur au milieu
        if (m->is_center) {
          for (int i = 0; i < w/2-lenmaxname/2+margin+1-3; i++) printf(" ");
        } else {
          for (int i = 0; i < lenmaxname/2+margin+1-3; i++) printf(" ");
        }

        // On fait un scanf
        // @TODO supporter d'autres types (seul les int sont supportés)
        scanf("%d",(int*)item->option_value);

        // Comme le scanf ne récupère que ce qui l'intéresse, on vide nous même le stdin
        clearstdin();

        // On se replace à la fin du menu
        for (int i = 0; i < n-counthide-indexselected-1; i++) printf("\n");

        // On remonte tout en haut du menu pour se préparer à réafficher le menu (le redessiner)
        for (int i = 0; i < n-counthide; i++) printf("\e[F\e[2K");

        // On ne souhaite pas quitter le menu, on met volontairement la valeur de cmdtmpt à 0
        cmdtmpt = 0;

        // On recommence tout en haut de la boucle principal
        continue;

      }
      
      // Sinon l'item contient un sous-meun
      else if (item->submenu!=NULL) {

        // Dans ce cas, on a juste à empiler le menu dans la pile
        menu_pile_item *tmpm = pile->first;
        menu_pile_item *parent = pile->first;
        while (tmpm!=NULL) {
          parent = tmpm;
          tmpm = tmpm->next;
        }
        if (parent==NULL) {
          pile->first = malloc(sizeof(menu_pile_item));
          pile->first->m = item->submenu;
          pile->first->index = index; // On sauvegarde l'index de l'item qui nous a permis de naviguer dans le sous-menu
          pile->first->next = NULL;
          pile->last = pile->first;
        } else {
          menu_pile_item *nitem = malloc(sizeof(menu_pile_item));
          nitem->m = item->submenu;
          nitem->index = 0;
          nitem->next = NULL;
          parent->next = nitem;
          parent->index = index; // On sauvegarde l'index de l'item qui nous a permis de naviguer dans le sous-menu
          pile->last = nitem;
        }

        // On remonte tout en haut du menu pour se préparer à réafficher le menu (le redessiner)
        for (int i = 0; i < n-counthide; i++) printf("\e[F\e[2K");

        // Comme on rentre dans un sous-menu, on démarre la sélection au 1er item par défaut (pour pouvoir changer ça, il faut simuler le sous-menu à l'aide d'une fonction que l'on place dans le champ _fn de l'item)
        index = 0;

        // On ne souhaite pas quitter le menu, on met volontairement la valeur de cmdtmpt à 0
        cmdtmpt = 0;

        // On recommence tout en haut de la boucle principal
        continue;
      }
      
      // Sinon l'item est un bouton retour
      else if (item->is_back) {

        // Dans ce cas, on dépile la pile

        // Si la pile est vide ou contient seulement le menu principal
        if (pile->first==NULL||pile->first==pile->last) {
          for (int i = 0; i < n; i++) printf("\e[F\e[2K"); // On remonte tout en haut du menu pour se préparer à réafficher le menu (le redessiner)
          cmdtmpt = 0; // On ne souhaite pas quitter le menu, on met volontairement la valeur de cmdtmpt à 0
          continue; // On recommence tout en haut de la boucle principal
        }

        // Sinon la pile n'est pas vide est on dépile
        menu_pile_item *tmpm = pile->first;
        menu_pile_item *parent = pile->first;
        menu_pile_item *parentparent = pile->first;
        while (tmpm!=NULL) {
          parentparent = parent;
          parent = tmpm;
          tmpm = tmpm->next;
        }
        free(parentparent->next);
        parentparent->next = NULL;
        pile->last = parentparent;

        // On remonte tout en haut du menu pour se préparer à réafficher le menu (le redessiner)
        for (int i = 0; i < n-counthide; i++) printf("\e[F\e[2K");

        // On ne souhaite pas quitter le menu, on met volontairement la valeur de cmdtmpt à 0
        cmdtmpt = 0;

        // On se repositionne sur l'item qui nous a permis de rentrer dans le sous-menu
        index = parentparent->index;

        // On recommence tout en haut de la boucle principal
        continue;

      }
      
      // Sinon l'item contient une fonction
      else if (item->_fn!=NULL) {

        // Dans ce cas, on exécute la fonction en passant en argument le menu courrant (là où se trouve le menu, l'item sélectionné, et la position de l'item parmi les items sélectionnables) (@see menu_item)
        msg = item->_fn(m,item,index);

        // Si le message indique de quitter le menu, on quitte le menu
        if (msg==-1||msg==-2||msg>0) {

          // -1 c'est pour seulement quitter le menu une fois, -2 c'est pour quitter tout les menus empiler
          if (msg==-1) msg = MENU_NO_MESSAGE;

          // dans ce cas, on quitte la boucle
          break;
        }

        // Sinon, on ne souhaite pas quitter le menu, on met volontairement la valeur de cmdtmpt à 0
        cmdtmpt = 0;

        // On recommence tout en haut de la boucle principal
        continue;
      }

      // Sinon, il s'agit d'un item normal, par conséquent, c'est le développeur qui décide ce qu'il va se passer une fois la sélection d'un tel item (on se prépare donc à retourner toutes les info sur l'item choisit)
      break;
    }

    // Si on arrive ici, alors le joueur a simplement appuyé sur les flèches du haut ou du bas
    // On remonte tout en haut du menu pour se préparer à réafficher le menu (le redessiner)
    for (int i = 0; i < n-counthide; i++) printf("\e[F\e[2K");

  }

  // On libère la pile
  menu *mcoice = pile->last->m;
  menu_pile_item *tmpm = pile->first;
  while (tmpm!=NULL) {
    menu_pile_item *tmpm2 = tmpm->next;
    free(tmpm);
    tmpm = tmpm2;
  }
  free(pile);

  // On retourne les info de l'item choisit par le joueur
  return (menu_item_choice){index,mcoice,msg};

}



/*

// ANCIENNE VERSION
// (qui est beaucoup plus courte mais avec à peine 1% des fonctionnalités de la nouvelle version :D)

#define MENU_SEPARATOR "{SEPARATOR}"


int print_menu_(int n, ...) {
  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);

  int cmdtmpt = 0;
  int countsep = 0;
  va_list vl2;
  va_start(vl2,n);
  int maxlen = 0;
  for (int i = 0; i < n; i++) {
    char *val=va_arg(vl2,char*);
    if (strcmp(val,MENU_SEPARATOR)==0) countsep++;
    int len = strlen(val);
    if (len > maxlen) maxlen = len;
  }
  va_end(vl2);
  char sep[] = "     ";
  int lensep = strlen(sep);
  for (int i = 0; i < h/2-n/2; i++) printf("\n");
  int index = 0; 
  while (1) {

    va_list vl;
    va_start(vl,n);
    int countsept = 0;
    for (int i=0;i<n;i++) {
      char *val=va_arg(vl,char*);
      int len = strlen(val);
      printf("\e[2K");
      if (strcmp(val,MENU_SEPARATOR)==0) {
        printf("\n");
        countsept++;
        continue;
      }
      if (i-countsept==index) {
        for (int j = 0; j < w/2-len/2; j++) printf(" ");
        printf(BRED">"" %s ""<"RESET"\n",val);
      } else {
        for (int j = 0; j < w/2-maxlen/2-lensep; j++) printf(" ");
        printf(BBLU">");
        printf("%s",sep);
        for (int j = 0; j < maxlen/2-len/2; j++) printf(" ");
        printf(" %s ",val);
        printf("%s",sep);
        for (int j = 0; j < maxlen/2-len/2-len%2; j++) printf(" ");
        printf("<"RESET"\n");
      }
      
    }
    va_end(vl);

    int cp = 0;
    while(1) {
      cp = getch();
      
      if (cp==(int)'\e') {
        cp = getch();
        if (cp==(int)'[') {
          cp = getch();
          if (cp=='B') {index++;if(index>=n-countsep){index=n-1-countsep;}break;}
          if (cp=='A') {index--;if(index<0){index=0;}break;}
          continue;
        }
      }

      if (cp==10) {cmdtmpt = 1;break;}
    }

    if (cmdtmpt) break;

    for (int i = 0; i < n; i++) printf("\e[F");

   // sleepcp(0);

  }

  return index;
}
*/





#endif