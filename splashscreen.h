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

// TODO : rajouter le support des caractères unicodes


/**
 * splashscreen.h
 *
 * Ce fichier permet juste de faire un magnifique splashscreen (ça a été fait juste pour le fun)
 *
 * Voici l'objectif :
 *
 *   - au début il n'y a rien
 *   - avec un certain intervalle, on affiche des caractères random qui s'actualisent assez règulièrement
 *   - une fois que tout les caractères random sont affichés, on commence à afficher les vrais caractères du texte qu'on veut afficher
 *   - et ensuite on efface tout avec un certain interval
 *
 * C'est le même principe que le jeu du pendu sauf qu'à la place des "_" (underscore) on met des caractères random
 */



#ifndef CHECKERS_SPLASHSCREEN_H
#define CHECKERS_SPLASHSCREEN_H





/**********************\
*                      *
*       INCLUDE        *
*                      *
\**********************/

#include "display.h" // on aura besoin de ce fichier pour dessiner le splashscreen
                     // (et pour effectuer différentes opérations sur la console)


#include <string.h> // très pratique pour manipuler des chaînes de caractères (utilisé pour gérer le nom des sauvegardes)







/**********************\
*                      *
*        MACRO         *
*                      *
\**********************/

// TODO : remplacer le namespace "MENU_" par "SPLASHSCREEN_"

#define MENU_COLOR_FADEIN YEL   // La couleur des caractères random
#define MENU_FPS 60             // La vitesse d'actualisation
#define MENU_TICK_FADEIN 5      // L'intervalle de temps pour l'apparition d'un caractère random
#define MENU_TICK_RANDOM 10     // L'intervalle de temps pour l'apparition des vrais caractères
#define MENU_TIME_DURATION 500  // Le durée durant laquelle le vrai texte est visible (un petit temps de pause)
#define MENU_TICK_FADEOUT 1     // L'intervalle de temps pour la disparition des caractères
#define MENU_STEP_IN_OUT 4      // Pour faire apparaître le texte et le faire disparaître
#define MENU_STEP_IN 3          // Pour faire apparaître le texte (et le texte ne disparaît pas)

// Quelques macros pratiques pour créer les objets facilement
#define cnc2c(b) straddcolor(NULL,b)
#define c2c(s,b) char2charcolor(s,b)
#define nc2c(s,b) s=straddcolor(s,b)







/**********************\
*                      *
*        STRUCT        *
*                      *
\**********************/

// Un caractère mais contient une info en plus : la couleur
typedef struct charcolor {
  // @TODO: rajouter le support de l'unicode
  char t; // le caractère (uniquement ascii)
  char c[20]; // la couleur sous forme d'échappement ANSI (@see colors.h)
} charcolor;







/**********************\
*                      *
*       FUNCTIONS      *
*                      *
\**********************/

/**
 * char2charcolor : permet de créer une chaîne de caractères colorés à partir d'une chaîne de caractère par défaut
 *
 * @info les chaînes colorées fonctionnent de la même façon que les chaînes classiques (le dernier caractère coloré est '\0')
 *
 * @param {char*} t - la chaîne que l'on veut créer sous forme coloré
 * @param {char*} c - la couleur de la chaîne (échappement ANSI @see colors.h)
 *
 * @return {charcolor*} retourne une chaîne de couleur c
 */
charcolor* char2charcolor(char *t,char* c) {
  int s = strlen(t);
  charcolor* n = malloc((s+1)*sizeof(charcolor));
  for (int i = 0; i < s; i++) {
    n[i].t = t[i];
    n[i].c[0] = '\0';
    strcat(n[i].c,c);
  }
  n[s].t = '\0';
  return n;
}

/**
 * strlencolor : permet de compter le nombre de caractères colorés dans une chaîne colorée
 *
 * @info : même principe que strlen
 *
 * @param {charcolor*} n - la chaîne colorée
 *
 * @return {int} retourne le nobmre de caractères colorés
 */
int strlencolor(charcolor *n) {
  if (n==NULL) return 0;
  int i = 0;
  while (n[i].t!='\0') {
    i++;
  }
  return i;
}

/**
 * print_charcolor : permet d'afficher sur la console la chaîne colorée
 *
 * @param {charcolor*} n - la chaîne colorée à afficher
 */
void print_charcolor(charcolor *n) {
  if (n==NULL) return;
  int s = strlencolor(n);
  char c[20];
  c[0] = '\0';
  for (int i = 0; i < s; i++) {
    if (strcmp(c,n[i].c)!=0) {
      printf("%s",n[i].c);
      c[0] = '\0';
      strcat(c,n[i].c);
    }
    printf("%c",n[i].t);
  }
  printf(RESET);
}

/**
 * free_charcolor : permet de libérer une chaine colorée
 *
 * @param {charcolor*} n - la chaîne colorée à libérer
 */
void free_charcolor(charcolor *n) {
  if (n==NULL) return;
  free(n);
}

/**
 * strcatcolor : permet de concaténer deux chaînes colorées (ne retourne pas une nouvelle chaîne colorée, elle réalloue la chaîne colorée d)
 *
 * @info même principe que strcat
 *
 * @param {charcolor*} d - la chaîne colorée qui sera mis en 1er
 * @param {charcolor*} s - la chaîne colorée qui sera mis en 2ème
 *
 * @return {charcolor*} retourne le nouveau pointeur de la chaîne colorée réalloué à partir du pointeur de d
 */
charcolor* strcatcolor(charcolor *d, charcolor *s) {
  int len = strlencolor(d)+strlencolor(s);
  d = realloc(d,(len+1)*sizeof(charcolor));
  int i = 0;
  int j = 0;
  while (d[i].t!='\0') i++;
  while (s[j].t!='\0') {
    d[i].t = s[j].t;
    d[i].c[0] = '\0';
    strcat(d[i].c,s[j].c);
    i++;
    j++;
  }
  d[len].t = '\0';
  return d;
}

/**
 * straddcolor : permet de concaténer deux chaînes colorées mais libère la chaîne colorée s
 *
 * @param {charcolor*} d - la chaîne colorée qui sera mis en 1er
 * @param {charcolor*} s - la chaîne colorée qui sera mis en 2ème et qui sera libérée
 *
 * @return {charcolor*} retourne le nouveau pointeur de la chaîne colorée réalloué à partir du pointeur de d
 */
charcolor* straddcolor(charcolor *d, charcolor *s) {
  if (d==NULL) {
    d = malloc(sizeof(charcolor));
    d[0].t = '\0';
  }
  d = strcatcolor(d,s);
  free_charcolor(s);
  return d;
}

/**
 * randomansicolor : permet de générer une couleur (échappement ANSI) aléatoire
 *
 * @return {char*} retourne un échappement ANSI permettant de colorer l'output sur la console
 */
char* randomansicolor() {
  int a = rand()%5;
  int b = rand()%2==0?(rand()%(37-30+1)+30):(rand()%(97-90+1)+90);
  int c = rand()%2==0?(rand()%(47-40+1)+40):(rand()%(107-100+1)+100);
  int d = c>47?1:0;
  int e = rand()%3==1?1:0;
  char *f = malloc(20*sizeof(char));
  f[0] = '\0';
  if (e) {
    if (d) {
      strcat(f,"\e[0;");
      char g[10];sprintf(g, "%d", c);
      strcat(f,g);
      strcat(f,"m");
    } else {
      strcat(f,"\e[");
      char g[10];sprintf(g, "%d", c);
      strcat(f,g);
      strcat(f,"m");
    }
  }
  strcat(f,"\e[");
  char h[10];sprintf(h, "%d", a);
  strcat(f,h);
  strcat(f,";");
  char i[10];sprintf(i, "%d", b);
  strcat(f,i);
  strcat(f,"m");
  return f;
}

/**
 * randomcharcolor : permet de créer une chaîne colorée avec das caractères aléatoires
 *
 * @TODO : ajouter un deuxième argumenrt pour choisir la couleur
 *
 * @param {int} len - la longueur de la chaîne colorée aléatoire à générer
 *
 * @return {charcolor*} retourne une chaîne colorée aléatoire
 */
charcolor* randomcharcolor(int len) {
  charcolor *n = malloc((len+1)*sizeof(charcolor));
  for (int i = 0; i < len; i++) {
    n[i].t = (char)((int)rand()%((int)'z'-(int)'0'+1)+(int)'0');
    n[i].c[0] = '\0';
    //char *rc = randomansicolor();
    //strcat(n[i].c,rc);
    strcat(n[i].c,MENU_COLOR_FADEIN);
    //free(rc);
  }
  n[len].t = '\0';
  return n;
}



/**
 * print_in_out : permet de créer un effet fade_in + random + fade_out
 *
 * @param {charcolor*} s - la chaîne colorée à afficher
 * @param {charcolor*} duration - le temps d'affichage de la chaîne
 * @param {int} fadein - intervalle de temps pour le fadein
 * @param {int} faderandom - intervalle de temps pour le random
 * @param {int} fadeout - intervalle de temps pour le fadeout
 * @param {speed} speed - vitesse des transitions
 */
void print_in_out(charcolor *s,int duration,int fadein,int faderandom,int fadeout,int step,int speed) {

  int tsize = strlencolor(s);
  int tsizetmp = tsize;
  int *t = calloc(tsize,sizeof(int));

  clock_t tick = clock();

  int fpsc = 1000/MENU_FPS;

  for (int i = 0; i < tsize; i++) {
    if (s[i].t==' '||s[i].t=='\n') {
      t[i] = 1;
      tsizetmp--;
    }
  }

  int cstatec = 1;

  int ticktime = MENU_TICK_FADEIN;

  // 0->1 => fade in
  // 1->2 => random
  // 2->3 => fade out

  while (1) {

    if (cstatec==1) {
      ticktime = fadein;
    } else if (cstatec==2) {
      ticktime = faderandom;
    } else if (cstatec==3) {
      ticktime = fadeout;
    }

    if (clock()-tick > ticktime * CLOCKS_PER_SEC / 1000) {
      tick = clock();
      if (tsizetmp==0) {
        cstatec++;
        if (cstatec==3) sleepcp(duration);
        if (cstatec==step) break;
        tsizetmp = tsize;
      }
      for (int q = 0; q < speed; q++) {
        int *ttmp = calloc(tsize,sizeof(int));
        int j = 0;
        for (int i = 0; i < tsize; i++) {
          if (t[i]!=cstatec) ttmp[j++] = i;
        }
        t[ttmp[rand()%j]] = cstatec;
        tsizetmp--;
        free(ttmp);
        if (tsizetmp==0) break;
      }
    }

    charcolor *random = randomcharcolor(tsize);
    for (int i = 0; i < tsize; i++) {
      if (cstatec==3) {
        if (t[i]!=3) {
          random[i].t = s[i].t;
          random[i].c[0] = '\0';
          strcat(random[i].c,s[i].c);
        } else {
          random[i].t = ' ';
          random[i].c[0] = '\0';
          strcat(random[i].c,RESET);
        }
        if (s[i].t=='\n'||s[i].t==' ') {
          random[i].t = s[i].t;
          random[i].c[0] = '\0'; strcat(random[i].c,RESET);
        }
        continue;
      }
      if (t[i]==0) {
        random[i].t = ' ';
        random[i].c[0] = '\0';
        strcat(random[i].c,RESET);
      } else if (t[i]==2) {
        random[i].t = s[i].t;
        random[i].c[0] = '\0';
        strcat(random[i].c,s[i].c);
      }
      if (s[i].t=='\n'||s[i].t==' ') {
        random[i].t = s[i].t;
        random[i].c[0] = '\0'; strcat(random[i].c,RESET);
      }
    }

    printf("\e7");
    print_charcolor(random);
    printf("\e8");
    //printf("\n\e[F");
    //printf("\r");


    free_charcolor(random);

    sleepcp(fpsc);
  }

  //print_charcolor(s);
  //printf("\n");

  

  free(t);

}

/**
 * center_text : permet de centrer chaque ligne d'une chaîne colorée dans un rectangle de longueur w (attention la chaîne colorée est une nouvelle chaîne est devra être libérer)
 *
 * @param {charcolor*} s - la chaîne colorée à centrer
 * @param {int} w - la taille du rectangle (en pratique la taille de la console)
 *
 * @return {charcolor*} retourne une nouvelle chaîne correspondant à la chaîne mis en argument mais centrée
 */
charcolor* center_text(charcolor *s, int w) {
  int line = 0;
  int i = 0;
  int len = 0;
  while (s[i].t!='\0') {
    if (s[i].t=='\n') line++;
    i++;
  }
  len = i;
  if (i > 0) line++;
  i = 0;
  int *lens = calloc(line+1,sizeof(int));
  int j = 0;
  while (s[i].t!='\0') {
    if (s[i].t!='\n') lens[j]++;
    else j++;
    i++;
  }
  int sizemargin = len;
  for (int k = 0; k < line; k++) {
    sizemargin += w/2-lens[k]/2;
    lens[k] =  w/2-lens[k]/2;
  }
  sizemargin*=2;
  charcolor *c = malloc((sizemargin+1)*sizeof(charcolor));
  i = 0;
  j = 0;
  int n = 0;
  while (s[i].t!='\0') {
    if (i==0) {
      for (int k = 0; k < lens[j]; k++) {
        c[n].t = ' ';
        c[n].c[0] = '\0'; strcat(c[n].c,RESET);
        n++;
      }
      j++;
    }
    c[n].t = s[i].t;
    c[n].c[0] = '\0'; strcat(c[n].c,s[i].c);
    n++;
    if (s[i].t=='\n') {
      for (int k = 0; k < lens[j]; k++) {
        c[n].t = ' ';
        c[n].c[0] = '\0'; strcat(c[n].c,RESET);
        n++;
      }
      j++;
    }
    i++;
  }
  c[n].t = '\0';
  n++;
  free(lens);
  return c;
}


/**
 * print_splashscreen : permet d'afficher le splasscreen du jeu
 */
void print_splashscreen() {

  resetTerm();

  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);

  charcolor *title = cnc2c(c2c("MAE",BGRN));
  nc2c(title,c2c("L",BRED));
  nc2c(title,c2c("EMMY",BBLU));
  nc2c(title,c2c(" ",RESET));
  nc2c(title,c2c("STUDIO",RESET"\e[4m"));
  nc2c(title,c2c("\n",RESET));
  nc2c(title,c2c("present",RESET"\e[2m"));
  charcolor* centertitle = center_text(title, w);

  charcolor *title2 = cnc2c(c2c("Avec la participation de\n",RESET"\e[2m"));
  nc2c(title2,c2c("EIDD",BBLU));
  charcolor* centertitle2 = center_text(title2, w);



  /******************************************************************
      __ _____ _____    ____  _____    ____  _____ _____ _____ _____ 
   __|  |   __|  |  |  |    \|   __|  |    \|  _  |     |   __|   __|
  |  |  |   __|  |  |  |  |  |   __|  |  |  |     | | | |   __|__   |
  |_____|_____|_____|  |____/|_____|  |____/|__|__|_|_|_|_____|_____|
  *******************************************************************/
  charcolor *title3 = cnc2c(c2c("    __ _____ _____   ",BBLU));
  nc2c(title3,c2c(" ____  _____   ",RESET));
  nc2c(title3,c2c(" ____  _____ _____ _____ _____ \n",BRED));

  nc2c(title3,c2c(" __|  |   __|  |  |  ",BBLU));
  nc2c(title3,c2c("|    \\|   __|  ",RESET));
  nc2c(title3,c2c("|    \\|  _  |     |   __|   __|\n",BRED));

  nc2c(title3,c2c("|  |  |   __|  |  |  ",BBLU));
  nc2c(title3,c2c("|  |  |   __|  ",RESET));
  nc2c(title3,c2c("|  |  |     | | | |   __|__   |\n",BRED));

  nc2c(title3,c2c("|_____|_____|_____|  ",BBLU));
  nc2c(title3,c2c("|____/|_____|  ",RESET));
  nc2c(title3,c2c("|____/|__|__|_|_|_|_____|_____|\n",BRED));

  charcolor* centertitle3 = center_text(title3, w);
  /******************************************************************/

  /*charcolor *title4 = cnc2c(c2c("> ",BRED));
  nc2c(title4,c2c("Appuyez sur ENTREE pour jouer",BWHT));
  nc2c(title4,c2c(" <\n",BRED));
  nc2c(title4,c2c("fais chier",RESET));
  charcolor* centertitle4 = center_text(title4, w);*/

  charcolor *title4 = cnc2c(c2c("> ",BRED));
  nc2c(title4,c2c("Appuyez sur ",RESET));
  nc2c(title4,c2c("[",BBLU));
  nc2c(title4,c2c("ENTREE",BLU));
  nc2c(title4,c2c("]",BBLU));
  nc2c(title4,c2c(" pour jouer",RESET));
  nc2c(title4,c2c(" < \n",BRED));
  charcolor* centertitle4 = center_text(title4, w);

  for (int i = 0; i < h/2-2; i++) printf("\n");

  print_in_out(centertitle,MENU_TIME_DURATION,MENU_TICK_FADEIN,MENU_TICK_RANDOM,MENU_TICK_FADEOUT,MENU_STEP_IN_OUT,1);
  sleepcp(300);
  print_in_out(centertitle2,MENU_TIME_DURATION,MENU_TICK_FADEIN,MENU_TICK_RANDOM,MENU_TICK_FADEOUT,MENU_STEP_IN_OUT,1);
  sleepcp(300);
  printf("\e[F\e[F\e[F");
  print_in_out(centertitle3,MENU_TIME_DURATION,MENU_TICK_FADEIN,MENU_TICK_RANDOM,MENU_TICK_FADEOUT,MENU_STEP_IN,5);
  printf("\n\n\n\n\n\n");
  print_in_out(centertitle4,MENU_TIME_DURATION,MENU_TICK_FADEIN,MENU_TICK_RANDOM,MENU_TICK_FADEOUT,MENU_STEP_IN,2);
  

  
  free(title);
  free(title2);
  free(title3);
  free(title4);
  free(centertitle);
  free(centertitle2);
  free(centertitle3);
  free(centertitle4);

  wait_press_start_button();

}

/**
 * print_splashscreen_GO : permet d'afficher un splashscreen pour le joueur qui a gagné la partie
 */
void print_splashscreen_GO(int n){

  // http://patorjk.com/software/taag/#p=display&f=Fire%20Font-k&t=BLANC%20a%20Vaincu

  resetTerm();
  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);
  if(n==0){
    charcolor *titleGO = cnc2c(c2c(
    "        (                 )                                                    \n"
    "  (   )\\ )    (      ( /(    (                                               \n"
    "( )\\ (()/(    )\\     )\\())   )\\       )   (   (     )  (                 (   \n"
    ")((_) /(_))((((_)(  ((_)\\  (((_)   ( /(   )\\  )\\ ( /(  )\\   (      (    ))\\  \n"
    "((_)_ (_))   )\\ _ )\\  _((_) )\\",BHRED));
    nc2c(titleGO,c2c("___",BHYEL));
    nc2c(titleGO,c2c("  )(_)) ((_)((_))(_))((_)  )\\ )   )\\  /((_)\n",BHRED));
    nc2c(titleGO,c2c("  | _ )| |    ",BHYEL));
    nc2c(titleGO,c2c("(_)",BHRED));
    nc2c(titleGO,c2c("_\\",BHYEL));
    nc2c(titleGO,c2c("(_)",BHRED));
    nc2c(titleGO,c2c("| \\| |",BHYEL));
    nc2c(titleGO,c2c("((",BHRED));
    nc2c(titleGO,c2c("/ __| ",BHYEL));
    nc2c(titleGO,c2c("((",BHRED));
    nc2c(titleGO,c2c("_",BHYEL));
    nc2c(titleGO,c2c(")",BHRED));
    nc2c(titleGO,c2c("_  \\ \\ / /",BHYEL));
    nc2c(titleGO,c2c("((",BHRED));
    nc2c(titleGO,c2c("_",BHYEL));
    nc2c(titleGO,c2c(")",BHRED));
    nc2c(titleGO,c2c("_  (_) _",BHYEL));
    nc2c(titleGO,c2c("(",BHRED));
    nc2c(titleGO,c2c("_",BHYEL));
    nc2c(titleGO,c2c("/(  ((_)(_))(  \n",BHRED));
    nc2c(titleGO,c2c("  | _ \\| |__   / _ \\  | .` | | (__  / _` |  \\ V / / _` | | || ' \\",BHYEL));
    nc2c(titleGO,c2c("))",BHRED));
    nc2c(titleGO,c2c("/ _| | || | \n"
    "  |___/|____| /_/ \\_\\ |_|\\_|  \\___| \\__,_|   \\_/  \\__,_| |_||_||_| \\__|  \\_,_| \n",BHYEL));
    charcolor* centertitleGO = center_text(titleGO, w);
    print_in_out(centertitleGO,MENU_TIME_DURATION,MENU_TICK_FADEIN,MENU_TICK_RANDOM,MENU_TICK_FADEOUT,MENU_STEP_IN,6);
    free(centertitleGO);
    free(titleGO);
  } else {                                                         
    charcolor *titleGOnoir = cnc2c(c2c(
    "    )     )  (    (                                                \n"
    " ( /(  ( /(  )\\ ) )\\ )                                             \n"
    " )\\()) )\\())(()/((()/(      )   (   (     )  (                 (   \n"
    "((_)\\ ((_)\\  /(_))/(_))  ( /(   )\\  )\\ ( /(  )\\   (      (    ))\\  \n",BHRED));
    nc2c(titleGOnoir,c2c(" _",BHYEL));
    nc2c(titleGOnoir,c2c("((",BHRED));
    nc2c(titleGOnoir,c2c("_",BHYEL));
    nc2c(titleGOnoir,c2c(")  ((",BHRED));
    nc2c(titleGOnoir,c2c("_",BHYEL));
    nc2c(titleGOnoir,c2c(")(",BHRED));
    nc2c(titleGOnoir,c2c("_",BHYEL));
    nc2c(titleGOnoir,c2c(")) (",BHRED));
    nc2c(titleGOnoir,c2c("_",BHYEL));
    nc2c(titleGOnoir,c2c("))    )(_)) ((_)((_))(_))((_)  )\\ )   )\\  /((_) \n",BHRED));
    nc2c(titleGOnoir,c2c("| \\| | / _ \\|_ _|| _ \\  ",BHYEL));
    nc2c(titleGOnoir,c2c("((",BHRED));
    nc2c(titleGOnoir,c2c("_",BHYEL));
    nc2c(titleGOnoir,c2c(")",BHRED));
    nc2c(titleGOnoir,c2c("_  ",BHYEL));
    nc2c(titleGOnoir,c2c("\\ \\ / /",BHYEL));
    nc2c(titleGOnoir,c2c("((",BHRED));
    nc2c(titleGOnoir,c2c("_",BHYEL));
    nc2c(titleGOnoir,c2c(")",BHRED));
    nc2c(titleGOnoir,c2c("_  (_) _",BHYEL));
    nc2c(titleGOnoir,c2c("(",BHRED));
    nc2c(titleGOnoir,c2c("_",BHYEL));
    nc2c(titleGOnoir,c2c("/(  ((_)(_))(  \n",BHRED));
    nc2c(titleGOnoir,c2c("| .` || (_) || | |   /  / _` |  \\ V / / _` | | || ' \\",BHYEL));
    nc2c(titleGOnoir,c2c("))",BHRED));
    nc2c(titleGOnoir,c2c("/ _| | || | \n"
    "|_|\\_| \\___/|___||_|_\\  \\__,_|   \\_/  \\__,_| |_||_||_| \\__|  \\_,_| \n",BHYEL));
    charcolor* centertitleGO = center_text(titleGOnoir, w);
    print_in_out(centertitleGO,MENU_TIME_DURATION,MENU_TICK_FADEIN,MENU_TICK_RANDOM,MENU_TICK_FADEOUT,MENU_STEP_IN,6);
    free(centertitleGO);
    free(titleGOnoir);
  }
  for (int i = 0; i < 5; i++) printf("\n\n");
}

#endif