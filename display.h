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

// TODO : rajouter un namespace "display_"


/**
 * display.h
 *
 * Fichier contenant des fonctions d'affichage
 */

//#define AFFICHEJOLI //mettre cette ligne en commentaire pour de l'affichage plus simple sous windows


#ifndef CHECKERS_AFFICHAGE_H
#define CHECKERS_AFFICHAGE_H



/**********************\
*                      *
*       INCLUDE        *
*                      *
\**********************/

#include <string.h>
#include "structures.h"
#include "colors.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#include <conio.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
int getch() {
    struct termios oldtc, newtc;
    int ch;
    tcgetattr(STDIN_FILENO, &oldtc);
    newtc = oldtc;
    newtc.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
    ch=getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
    return ch;
}
#endif // Windows/Linux






/**********************\
*                      *
*        MACRO         *
*                      *
\**********************/

#define BACKGROUND_1         REDHB
#define BACKGROUND_2         BLKB
#define BACKGROUND_SELECTED  CYNHB
#define BACKGROUND_MSELECTED MAGB
#define BACKGROUND_HIGHLIGHT BLUB

#define TIME_STEP 500


/* constantes pour l'affichage des pieces dans print_piece */
#ifdef AFFICHEJOLI
#define BLKDOT "\u26AB"
#define WHTDOT "\u26AA"
#define YELDOT "\xF0\x9F\x9F\xA1"
#define BLKSQU "\xE2\xAC\x9B"//"\u2B1B"
#define WHTSQU "\u2B1C"
#define YELSQU "\xF0\x9F\x9F\xA8"
#else
#define BLKDOT BBLK"PN"
#define WHTDOT BWHT"PB"
#define YELDOT BYEL"PC"
#define BLKSQU BHMAG"DN" //BBLK"DN"
#define WHTSQU BGRN"DB" //BWHT"DB"
#define YELSQU BYEL"DC"
#endif






/**********************\
*                      *
*       FUNCTIONS      *
*                      *
\**********************/

/* fonction pour affichage symboles sous Powershell Windows */
void configWindowsConsole(){
  #ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	setlocale(LC_ALL,"fr_FR.UTF8");
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle, &info);
    COORD new_size =  {
        info.srWindow.Right - info.srWindow.Left + 1,
        info.srWindow.Bottom - info.srWindow.Top + 1
    };
    SetConsoleScreenBufferSize(handle, new_size);
  #endif
  #ifdef _WIN32
	SetConsoleTitle("Jeu de Dame | Ver 1.02");
  #else
    printf("\e]0;Jeu de Dame | Ver 1.02\a\n");
  #endif
}

#ifdef _WIN32
void ShowConsoleCursor(BOOL showFlag) {
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO     cursorInfo;
  GetConsoleCursorInfo(out, &cursorInfo);
  cursorInfo.bVisible = showFlag; // set the cursor visibility
  SetConsoleCursorInfo(out, &cursorInfo);
}
#endif

void display_show_cursor() {
  #ifdef _WIN32
    ShowConsoleCursor(TRUE);
  #else
    printf("\e[?25h");
  #endif
}

void display_hide_cursor() {
  #ifdef _WIN32
    ShowConsoleCursor(FALSE);
  #else
    printf("\e[?25l");
  #endif
}

void display_save_cursor() {
  printf("\e7");
}

void display_restore_cursor() {
  printf("\e8");
}

void get_terminal_size(int* width, int* height) {
#if defined(_WIN32)
	CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *width = (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
    *height = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#elif defined(__linux__)
    struct winsize w;
    ioctl(fileno(stdout), TIOCGWINSZ, &w);
    *width = (int)(w.ws_col);
    *height = (int)(w.ws_row);
#endif // Windows/Linux
printf("");
}

void printfc(char *s,char *c) {
  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);
  int len = strlen(s);
  printf(RESET);
  for (int i = 0; i < w/2-len/2; i++) printf(" ");
  printf("%s%s",c,s);
}

/* fonction pour temporiser l'affichage */
void sleepcp(int milliseconds) {
    clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC / 1000;
    while (clock() < time_end) {
    }
}

/* fonction permettant d'effacer l'ecran du terminal */
void resetTerm() {
    #ifdef _WIN32
    system("@cls||clear");
    #else
    printf(clean);
    #endif
}

void clearstdin() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { }
}

void wait_press_start_button() {
  int cp = 0;
  while(1) {
    cp = getch();
    if (cp==10||cp==13) break;
  }
}

/* fonction permettant de redimensionner le terminal a une taille passeee en parameter */
void resizeTerminal(int haut, int larg){
	printf("\e[8;%d;%d;t",haut,larg);
}

/* fonction permettant de redimensionner le terminal a une taille apropriée pour le damier de *g*/
void resizeTerminalToGame(jeu* g){
	printf("\e[8;100;%d;t",6*(g->NB_Col+3));	
}

void switchcolorbckgrd(int i, int j, highlight_info selected) {
  if (selected.mode == HIGHLIGHT_MODE_MULTI_SELECT) {
      printf(BACKGROUND_MSELECTED);
      return;
    }
    if (selected.mode == HIGHLIGHT_MODE_SELECT) {
      printf(BACKGROUND_SELECTED);
      return;
    }
    if (selected.mode == HIGHLIGHT_MODE_HOVER) {
      printf(BACKGROUND_HIGHLIGHT);
      return;
    }
    if (j == -1) {
        printf(RESET);             //defaultbackground
    } else if (((i + j) % 2) == 0) {
        printf(BACKGROUND_1);             //redbackground

    } else {
        printf(BACKGROUND_2);             //blackbackground
    }
}

void print_piece(caseVal s) {
    switch (s) {
        case PBC:
        case PNC:
            printf(YELDOT);
            break;
        case DNC:
        case DBC:
            printf(YELSQU);
            break;
        case PB:
            printf(WHTDOT);
            break;
        case DB:
            printf(WHTSQU);
            break;
        case PN:
            printf(BLKDOT);
            break;
        case DN:
            printf(BLKSQU);  	
            break;
        default:
            printf("  ");
            break;
    }
}

void print_ligne(jeu* g, int i) {
    int w = 0;
    int h = 0;
    get_terminal_size(&w,&h);
    int margin = w/2-(6*g->NB_Col+14)/2;

    position p;
    p.lig = i;
    caseVal s;
    highlight_info infoh = (highlight_info){HIGHLIGHT_MODE_EMPTY,0};
    switchcolorbckgrd(i, -1, infoh);
    if (g->is_center) {
      for (int i = 0; i < margin; i++) printf(" ");
    }
    printf("       ");
    for (int j = 0; j < g->NB_Col; j++) {
        switchcolorbckgrd(i, j, g->plateau_s[i*g->NB_Col+j]);
        printf("      ");
    }
    switchcolorbckgrd(i, -1, infoh);
    printf("\n");
    if (g->is_center) {
      for (int i = 0; i < margin; i++) printf(" ");
    }
    printf("  %2d   ", i);
    for (int j = 0; j < g->NB_Col; j++) {
        p.col = j;
        s = getCaseVal(g, p);
        switchcolorbckgrd(i, j, g->plateau_s[i*g->NB_Col+j]);
        int oktemp = 0;
        int valuehighlight = g->plateau_s[i*g->NB_Col+j].index;
        if (getTypePiece(g,p)==NOTYPE && g->plateau_s[i*g->NB_Col+j].mode != HIGHLIGHT_MODE_MULTI_SELECT) {
          if (valuehighlight>0) {
            if (valuehighlight<=9) {
              printf(" (%d)",valuehighlight);
            } else {
              oktemp = 1;
              printf(" (%d)",valuehighlight);
            }
          } else {
            printf("  ");
            print_piece(s);
          }
        } else {
          printf("  ");
          print_piece(s);
        }
        switchcolorbckgrd(i, j, g->plateau_s[i*g->NB_Col+j]);
        printf(oktemp?" ":"  ");
    }
    switchcolorbckgrd(i, -1, infoh);
    printf("   %2d   \n", i);
    if (g->is_center) {
      for (int i = 0; i < margin; i++) printf(" ");
    }

    printf("       ");
    for (int j = 0; j < g->NB_Col; j++) {
        p.col = j;
        switchcolorbckgrd(i, j, g->plateau_s[i*g->NB_Col+j]);
        if (getTypePiece(g,p)!=NOTYPE && g->plateau_s[i*g->NB_Col+j].mode == HIGHLIGHT_MODE_MULTI_SELECT) {
          int valuehighlight = g->plateau_s[i*g->NB_Col+j].index;
          if (valuehighlight>0) {
            if (valuehighlight<=9) {
              printf(" (%d)  ",valuehighlight);
              continue;
            } else {
              printf(" (%d) ",valuehighlight);
              continue;
            }
          }
        }
        printf("      ");
    }
    switchcolorbckgrd(i, -1, infoh);

    printf("\n");


}

void print_ligne_char(jeu* g, char c) {
  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);
  int margin = w/2-(6*g->NB_Col+14)/2;
  if (g->is_center) {
    for (int i = 0; i < margin; i++) printf(" ");
  }
  for (int j = 0; j < 12+6*g->NB_Col; j++) {
    printf("%c",c);
  }
  printf("\n");  
}

void print_bordure_horizontale(jeu *g) {
  print_ligne_char(g,' ');
  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);
  int margin = w/2-(6*g->NB_Col+14)/2;
  if (g->is_center) {
    for (int i = 0; i < margin; i++) printf(" ");
  }
  printf("      ");
  for (int j = 0; j < g->NB_Col; j++) {
    printf("   %2d ", j);
  }
  printf("\n");
  print_ligne_char(g,' ');
}

void Affnom(jeu *g,couleur c) {
  if (g->tabJ[0].nom==NULL) return;
  if (g->tabJ[1].nom==NULL) return;
  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);
  int margin = w/2-(6*g->NB_Col+14)/2;
  if (g->is_center) {
    for (int i = 0; i < margin; i++) printf(" ");
  }
  if (c==BLANC) {
    char *s = g->tabJ[0].coul==BLANC?g->tabJ[0].nom:g->tabJ[1].nom;
    joueur j = g->tabJ[0].coul==BLANC?g->tabJ[0]:g->tabJ[1];
    for (int i = 0; i < 6+6*g->NB_Col/2-strlen(s)/2-(j.type==Humain?5:2); i++) printf(" ");
    printf(BRED"%s"RED" (%s)\n"RESET,s,j.type==Humain?"Humain":"IA");
  } else {
    joueur j = g->tabJ[0].coul==NOIR?g->tabJ[0]:g->tabJ[1];
    char *s = g->tabJ[0].coul==NOIR?g->tabJ[0].nom:g->tabJ[1].nom;
    for (int i = 0; i < 6+6*g->NB_Col/2-strlen(s)/2-(j.type==Humain?5:2); i++) printf(" ");
    printf(BBLU"%s"BLU" (%s)"RESET,s,j.type==Humain?"Humain":"IA");
  }
  
}

void center_text_damier(jeu*g) {
  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);
  int margin = w/2-(6*g->NB_Col+14)/2;
  if (g->is_center) {
    for (int i = 0; i < margin; i++) printf(" ");
  }
}

void print_damier(jeu* g) {
    print_ligne_char(g,'-');
    Affnom(g,NOIR);
    print_bordure_horizontale(g);
    for (int i = g->NB_Lig - 1; i>-1; i--) {
        print_ligne(g, i);
    }
    print_bordure_horizontale(g);
    Affnom(g,BLANC);
    print_ligne_char(g,'-');

}

void print_pos(position p){
    printf("%d %d ",p.lig,p.col);
}

//void print_move(move*m){
//	etape *mc = m->list;
//	while (mc != NULL)
//	{
//            print_pos(mc->pos);
//            printf(" -> ");
//            mc = mc->next;
//	}
//	printf("\n");
//}

void print_couleur(couleur c){
	if(c==BLANC)
		printf("Blanc");
	else if(c==NOIR)
		printf("Noir");	
	else
		printf("NoCol");
}

void print_typePiece(typePiece tp){
	if(tp==Pion)
		printf("Pion");
	else if(tp==Dame)
		printf("Dame");
	else
		printf("Vide");
}

void c() {
  getchar();
  resetTerm();
}


void print_info_damier(jeu*g,char *n, int margin, int center) {
  int w = 0;
  int h = 0;
  get_terminal_size(&w,&h);
  int playerwhite = 0;
  int playerblack = 1;
  if (g->tabJ[0].coul==NOIR) {
    playerwhite = 1;
    playerblack = 0;
  }
  int len1 = 20+strlen(n)+1;
  int len2 = 13+4+(g->tabJ[playerblack].nom!=NULL?strlen(g->tabJ[playerblack].nom):6)+(g->tabJ[playerblack].type==Humain?8:5)+6;
  int len3 = 13+5+(g->tabJ[playerwhite].nom!=NULL?strlen(g->tabJ[playerwhite].nom):6)+(g->tabJ[playerwhite].type==Humain?8:5)+6;
  int lenmax = (len1 >= len2 && len1 >= len3) ? len1 : (len2 >= len1 && len2 >= len3) ? len2 : len3;
  margin = w/2-lenmax/2+margin;
  for (int i = 0; i < margin; i++) printf(" ");
  printf("       "YEL"%d"RESET"          ["GRN"%s"RESET"]\n",g->NB_Col,n);
  for (int i = 0; i < margin; i++) printf(" ");
  printf("   ,--------,\n");
  for (int i = 0; i < margin; i++) printf(" ");
  printf("   |        |  "BBLU"NOIR - %s (%s)"RESET"\n",g->tabJ[playerblack].nom,g->tabJ[playerblack].type==Humain?"Humain":"CPU");
  for (int i = 0; i < margin; i++) printf(" ");
  printf(YEL"%d"RESET" |   "YEL"%d"RESET"    |        VS\n",g->NB_Lig,g->NB_LigInit);
  for (int i = 0; i < margin; i++) printf(" ");
  printf("   |        |  "BRED"BLANC - %s (%s)"RESET"\n",g->tabJ[playerwhite].nom,g->tabJ[playerwhite].type==Humain?"Humain":"CPU");
  for (int i = 0; i < margin; i++) printf(" ");
  printf("   '--------'\n");
}



void afficheGameOver(jeu *g,couleur c) {
  int n=rand()%13+1;
  switch(n){
    case 1:
      printfc("La victoire obtenue par la violence n'est pas une victoire,\n",YEL);
      printfc("vaincre avec loyauté est une philosophie.\n",YEL);
      printfc("Giuseppe Papasidero",UBLU);
      break;
    case 2:
      printfc("On apprend plus de nos victoires que des défaites des autres.\n",YEL);
      printfc("Szczepan Yamenski",UBLU);
      break;
    case 3:
      printfc("La plus grande des victoires est celle qui\n",YEL);
      printfc("se gagne en ayant la ferme conviction et le\n",YEL);
      printfc("sentiment net d'avoir offert et donné toutes\n",YEL);
      printfc("les opportunités à son adversaire\n",YEL);
      printfc("pour en sortir grandi.\n",YEL);
      printfc("Goumty Parker",UBLU);
      break;
    case 4:
      printfc("La victoire dans une partie de Dame appartient\n",YEL);
      printfc("la plupart du temps à celui qui voit un peu plus loin que l'adversaire.\n",YEL);
      printfc("Presque Emmanuel Lasker",UBLU);
      break;
    case 5:
      printfc("La victoire va à celui qui prend le plus de risques.\n",YEL);
      printfc("Jean-Claude Killy",UBLU);
      break;
    case 6:
      printfc("Les hommes parlent de la victoire comme d'une chance.\n",YEL);
      printfc("C'est le travail qui fait la victoire.\n",YEL);
      printfc("Ralph Waldo Emerson",UBLU);
      break;
    case 7:
      printfc("Tu sais, la victoire et la défaite,\n",YEL);
      printfc("c'est pareil : ça se traduit par des larmes.\n",YEL);
      printfc("Philippe Léotard",UBLU);
      break;
    case 8:
      printfc("La victoire ne va pas à celui qui part le premier,\n",YEL);
      printfc("mais à celui qui conclut.\n",YEL);
      printfc("Alfonso Caycedo",UBLU);
      break;
    case 9:
      printfc("Seule la victoire est belle.\n",YEL);
      printfc("Peggy Boucher",UBLU);
      break;
    case 10:
      printfc("La victoire donne aussi le privilège de s'apitoyer sur la victime.\n",YEL);
      printfc("Madeleine Ferron",UBLU);
      break;
    case 11:
      printfc("Accepte les défis afin que tu puisses sentir l'euphorie de la victoire.\n",YEL);
      printfc("George S. Patton",UBLU);
      break;
    case 12:
      printfc("Faut-il tant de fois vaincre avant de triompher !\n",YEL);
      printfc("Pierre Corneille",UBLU);
      break;
    default:
      printfc("À vaincre sans péril, on triomphe sans gloire.\n",YEL);
      printfc("Pierre Corneille",UBLU);
      break;
  }
  printf(RESET"\n\n");
  printfc("Appuyez sur [ENTREE] pour revenir au menu principal..","");
  wait_press_start_button();
  resetTerm();
}

void affichageEquality(jeu *g,int whatEquality) {
  resetTerm();
  printfc("EGALITE\n",BHRED);
  switch(whatEquality){
    case 1:
      printfc("Lorsque la même position des pièces se produit\n",YEL);
      printfc("pour la troisième fois, l'arbitre siffle l'égalité.\n",YEL);
      break;
    case 2:
      printfc("Lorsque durant vingt-cinq coups, aucune prise n'a eu lieu,\n",YEL);
      printfc("ni aucun déplacement de pion, l'arbitre siffle l'égalité.\n",YEL);
      break;
    case 3:
      printfc("Lorsque le plateau comporte plus que une dame\n",YEL);
      printfc("contre deux dames, ou une dame contre une dame,\n",YEL);
      printfc("l'arbitre siffle l'égalité.\n",YEL);
      break;
    case 4:
      printfc("Lorsque une position à une dame contre trois dames,\n",YEL);
      printfc("contre deux dames et un pion, ou contre une dame et\n",YEL);
      printfc("deux pions, les deux joueurs ont joué chacun seize coups\n",YEL);
      break;
    default:
      break;
  }
  printf(RESET"\n\n");
  printfc("Appuyez sur [ENTREE] pour revenir au menu principal..","");
  wait_press_start_button();
  resetTerm();
}


 
#endif