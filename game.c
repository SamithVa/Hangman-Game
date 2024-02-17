#include "game.h"

#define USER_ATTEMPT 6
GtkContainer *currentWin = NULL;
GtkEntry *input = NULL;
GtkLabel *user_attempt = NULL;
GtkImage *image = NULL;
int user_attempt_int = 0;
int left_char = 0;

// Word display hideword G__s
GtkLabel *word_display = NULL;

char word_selected[10] = {};
char hideWord[10] = {};
char new_hide_word[20] = {};
int hideIndex[10] ={ -1 };
int hideLengthGlobal = 0;

int ansCorrect = 0;
int correctAns = 0;


GtkWidget *userInput = NULL;

void end_program(){
    gtk_main_quit();
}

void removeAllElementInContainer(GtkWidget *wid, gpointer *ptr){
    gtk_container_forall(ptr, G_CALLBACK(gtk_widget_destroy), NULL);
}

// use to remove extra space in words added from library
char* deblank(char* str)
{
	char* out = str, * put = str;

	for (; *str != '\0'; ++str)
	{
		if (*str != ' ' && *str != '\n')
			*put++ = *str;
	}
	*put = '\0';
	return out;
}


void game(int wordLength, int hideLength, int attempt, gpointer *win) {

    //back button
    GtkButton *back_btn = gtk_button_new_with_label("Back");
    gtk_widget_set_name(back_btn, "back_btn");

    g_signal_connect(back_btn, "clicked", select_difficulty_menu, win);


	hideLengthGlobal = hideLength;
	// GTK quit when use click
	gtk_container_forall(win, G_CALLBACK(gtk_widget_destroy), NULL);

    currentWin = win;

	GtkWidget *vBox = gtk_vbox_new(FALSE, 5);

    strcpy(new_hide_word, "");
    strcpy(hideWord, "");


    GtkLabel *user_input_display = gtk_label_new(":");
    image = gtk_image_new_from_file("1.png");

    char tmp[20];
    user_attempt_int = attempt;
    sprintf(tmp, "You have %d attempts", attempt);
    user_attempt = gtk_label_new(tmp);
    gtk_widget_set_name(user_attempt, "user_attempt");
    input = gtk_entry_new();
    gtk_entry_set_width_chars(input, 1);

    // set name for the entry
    gtk_widget_set_name(input, "user_input");

    GtkButton *btn = gtk_button_new_with_label("Submit");
    gtk_widget_set_name(btn, "submit_btn");


    // read words from file "words.txt"
	FILE* myFile;
	fopen_s(&myFile, "words.txt", "r");

	char words[1000][10] ={};
	char tmpWord[10] = {};

	int count = 0;


	while (fgets(tmpWord, 10, myFile)) {
		deblank(tmpWord);
		if (strlen(tmpWord) == wordLength) {
			strcpy(words[count++],tmpWord);
		}
	}

	srand(time(NULL));

	int randomWordIndex = rand() % count;

	char word[10] = {};
	strcpy(word, words[randomWordIndex]);
	strcpy(word_selected, word);

	int ran = -1;
	int occur = 0;
	for (int i = 0; i < hideLength; i++) {
		ran = rand() % wordLength;
		occur = 0;
		for (int j = 0; j < i; j++) {
			if (ran == hideIndex[j]) {
				occur++;
				break;
			}
		}
		if (!occur)
			hideIndex[i] = ran;
		else
			i--;
	}


	for (int j = 0; j < hideLength; j++) {
		for (int i = 0; i < wordLength; i++) {
			if (i == hideIndex[j])
				hideWord[i] = '_';
			else if (hideWord[i] != '_')
				hideWord[i] = word[i];
		}
	}
	char userInput = '1';

    for(int i= 0; i< wordLength; i++){
        new_hide_word[2*i] = hideWord[i];
        new_hide_word[2*i+1] = ' ';
        if(i == wordLength-1)
            new_hide_word[2*i+1] = '\0';
    }

	word_display = gtk_label_new(new_hide_word);
	gtk_widget_set_name(word_display, "word_display");

	g_signal_connect(btn, "clicked", G_CALLBACK(getInput), user_input_display);

	// used to get input when key "return" is entered
	g_signal_connect(input, "activate", G_CALLBACK(getInput), user_input_display);



    gtk_box_pack_start(vBox, back_btn, FALSE, FALSE, 0);
	gtk_box_pack_start(vBox, word_display, TRUE, TRUE, 0);
	gtk_box_pack_start(vBox, image, TRUE,TRUE, 0);
	gtk_box_pack_start(vBox, input, TRUE, TRUE, 0);
	gtk_box_pack_start(vBox, user_input_display, TRUE, TRUE, 10);
	gtk_box_pack_start(vBox, user_attempt, TRUE, TRUE, 0);
	gtk_box_pack_start(vBox, btn, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(win), vBox);
    gtk_widget_show_all(win);


	if (ansCorrect == hideLength)
		printf("\nYOU WIN");
	else
		printf("\nYOU LOSE");

	printf("\nAns: %s\n", word);


	fclose(myFile);
}

void getInput(GtkWidget *wid, gpointer *user_input_label){
    const char *buff = gtk_entry_get_text(GTK_ENTRY(input));
    gtk_label_set_text(GTK_LABEL(user_input_label), buff);
    printf("\n%c", buff[0]);


    char tmp[20];
    sprintf(tmp, "You have %d attempts", user_attempt_int);
    gtk_label_set_text(user_attempt, tmp);


    //printf("Check Ans: %d", check_char(buff[0]));
    /*
    char *blank = ' ';
    gtk_entry_buffer_insert_text(buff, 0, blank, 2);
    */
    int tmpInt = check_char(buff[0]);
    gtk_entry_set_text(GTK_ENTRY(input), "");

    correctAns += tmpInt;
    if(tmpInt != 0){
        gtk_label_set_text(word_display, new_hide_word);
    }
    else{
        char imageStr[5];
        sprintf(imageStr, "%d.png", USER_ATTEMPT-user_attempt_int+2);
        user_attempt_int--;
        gtk_image_set_from_file(image, imageStr);
        sprintf(tmp, "You have %d attempts", user_attempt_int);
        gtk_label_set_text(user_attempt, tmp);
    }

    if(correctAns == hideLengthGlobal)
        victory();
    if(user_attempt_int <= 0)
        gameover();
}

// return 1 when found, else 0
int check_char(char userInput){
    int ansCorrectInFun = 0;
    for (int i = 0; i < hideLengthGlobal; i++) {
        if(hideWord[hideIndex[i]] == '_'){
            if (word_selected[hideIndex[i]] == userInput || word_selected[hideIndex[i]] == (userInput - 'a' + 'A') || word_selected[hideIndex[i]] == userInput - 'A' + 'a') {
                hideWord[hideIndex[i]] = word_selected[hideIndex[i]];
                new_hide_word[hideIndex[i]*2] = word_selected[hideIndex[i]];
                ansCorrectInFun++;
            }
        }
    }
    return ansCorrectInFun;
}





void select_difficulty_menu(GtkWidget *wid, gpointer *win){
    // Delete everything in win
    gtk_widget_set_name(win, "select_difficulty_win");
    gtk_container_forall(win, G_CALLBACK(gtk_widget_destroy), NULL);

    strcpy(word_selected, "");
    strcpy(hideWord, "");
    strcpy(new_hide_word, "");
    word_selected[0] = '\0';
    hideWord[0] = '\0';
    new_hide_word[0]= '\0';

    ansCorrect = 0;
    correctAns = 0;
    for(int i=0; i < 10; i++)
        hideIndex[i] = -1;
    hideLengthGlobal = 0;

    // difficulty :
    GtkLabel *note = gtk_label_new("Select Difficulty: ");
    GtkButton *easy_btn = gtk_button_new_with_label("Easy");
    GtkButton *medium_btn = gtk_button_new_with_label("Medium");
    GtkButton *hard_btn = gtk_button_new_with_label("Hard");

    gtk_widget_set_name(note, "select_difficulty_label");

    GtkWidget *vBox = gtk_vbox_new(FALSE, 5);

    g_signal_connect(easy_btn, "clicked", G_CALLBACK(easy), win);
    g_signal_connect(medium_btn, "clicked", G_CALLBACK(medium), win);
    g_signal_connect(hard_btn, "clicked", G_CALLBACK(hard), win);

    gtk_widget_set_name(easy_btn, "easy_btn");
    gtk_widget_set_name(medium_btn, "medium_btn");
    gtk_widget_set_name(hard_btn, "hard_btn");


    gtk_box_pack_start(vBox, note, TRUE, TRUE, 0);
    gtk_box_pack_start(vBox, easy_btn, TRUE, TRUE, 0 );
    gtk_box_pack_start(vBox, medium_btn, TRUE, TRUE, 0);
    gtk_box_pack_start(vBox, hard_btn, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(win), vBox);
    gtk_widget_show_all(win);
}

void easy(GtkWidget *wid, gpointer *win){
    // clear the container
    gtk_container_forall(win, G_CALLBACK(gtk_widget_destroy), NULL);
    game(4, 2, 6, win);
}

void medium(GtkWidget *wid, gpointer *win){
    gtk_container_forall(win, G_CALLBACK(gtk_widget_destroy), NULL);
    game(5, 3, 6, win);
}

void hard(GtkWidget *wid, gpointer *win){
    gtk_container_forall(win, G_CALLBACK(gtk_widget_destroy), NULL);
    game(8, 4, 6, win);
}

void victory(){
    gtk_container_forall(currentWin, G_CALLBACK(gtk_widget_destroy), NULL);
    gtk_widget_set_name(currentWin, "victory_win");

    GtkLabel *victory_label = gtk_label_new("YOU WIN!");
    gtk_widget_set_name(victory_label, "victory_label");

    GtkVBox *vBox = gtk_vbox_new(FALSE, 10);
    GtkButton *play_again_btn = gtk_button_new_with_label("Click to play again");
    gtk_widget_set_name(play_again_btn, "play_again_btn");

    g_signal_connect(play_again_btn, "clicked", G_CALLBACK(select_difficulty_menu), currentWin);

    gtk_box_pack_start(vBox, victory_label, TRUE, TRUE, 0);
    gtk_box_pack_start(vBox, play_again_btn, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(currentWin),vBox);
    gtk_widget_show_all(currentWin);
}


void gameover(){
    gtk_container_forall(currentWin, G_CALLBACK(gtk_widget_destroy), NULL);

   // GtkLabel *correct_ans = gtk_label_new(word_selected);
    GtkLabel *gameover_label = gtk_label_new("GAMEOVER!!");
    gtk_widget_set_name(gameover_label, "gameover_label");


    // Display answer to the user
    char ans[20];
    sprintf(ans, "Correct Answer: %s", word_selected);
    GtkLabel *answer = gtk_label_new(ans);
    gtk_widget_set_name(answer, "answer_display");
    gtk_widget_set_name(currentWin, "gameover_win");



    GtkVBox *vBox = gtk_vbox_new(FALSE, 10);
    GtkButton *play_again_btn = gtk_button_new_with_label("Click to play again");
    gtk_widget_set_name(play_again_btn, "play_again_btn");

    g_signal_connect(play_again_btn, "clicked", G_CALLBACK(select_difficulty_menu), currentWin);

    gtk_box_pack_start(vBox, answer, FALSE, FALSE, 5);
    gtk_box_pack_start(vBox, gameover_label, TRUE, TRUE, 0);
    gtk_box_pack_start(vBox, play_again_btn, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(currentWin),vBox);
    gtk_widget_show_all(currentWin);
}

