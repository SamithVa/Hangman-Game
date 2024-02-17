#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>


void select_difficulty_menu(GtkWidget *wid, gpointer *win);
void easy(GtkWidget *wid, gpointer *win);
void removeAllElementInContainer(GtkWidget *wid, gpointer *ptr);
char* deblank(char* str);
void game(int wordLength, int hideLength, int attempt, gpointer *win);
void getInput(GtkWidget *wid, gpointer *win);
void gameover();
void easy(GtkWidget *wid, gpointer *win);
void medium(GtkWidget *wid, gpointer *win);
void hard(GtkWidget *wid, gpointer *win);
int check_char(char userInput);
void end_program();
void victory();

#endif // GAME_H_INCLUDED
