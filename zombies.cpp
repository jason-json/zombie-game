#include <stdio.h>
#include <ncurses.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <ctime>
#include <unistd.h>

using namespace std;

class character{
  public:
	char name[50];
	int score;
	void draw(int, int, WINDOW *win, char n);
	int shot(int, WINDOW *win);
	void squareStuffed(int, int, int, int, char, WINDOW *);
	character();
};

class zombie{
  public:
	void drawZombie(int *, int, WINDOW *win);
	void starZombie(int *, WINDOW *win);
	void reproduceZombie(int *, WINDOW *win);
	void killZombie(int *, int, WINDOW *win);
	int checaPierde(int *, WINDOW *win);
	zombie();
};

class recordScore{
  public:
	void save(char name[], int score, int duration);
	int search(string searchName);
	recordScore();
};

character::character(){
	score = 0;
}

zombie::zombie()
{
}

recordScore::recordScore()
{
}

void character::draw(int row, int column, WINDOW *win, char n){

	init_pair(5, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	init_pair(12, COLOR_CYAN, COLOR_BLACK);

	wattron(win, COLOR_PAIR(5) | A_BOLD);
	mvwprintw(win, row, column, "  o  ");
	mvwprintw(win, row + 1, column, "/| |\\");
	mvwprintw(win, row + 2, column, " /_\\");
	wattroff(win, COLOR_PAIR(5) | A_BOLD);

	wattron(win, COLOR_PAIR(12) | A_BOLD);
	mvwprintw(win, row + 1, column + 1, "! !");
	wattroff(win, COLOR_PAIR(12) | A_BOLD);

	wattron(win, COLOR_PAIR(3) | A_BOLD);
	mvwprintw(win, row + 1, column + 2, "%c", n);
	wattroff(win, COLOR_PAIR(3) | A_BOLD);
}

int character::shot(int column, WINDOW *win){
	int i;
	int time;

	init_pair(2, COLOR_RED, COLOR_BLACK);
	wattron(win, COLOR_PAIR(2));

	for (i = 35; i >= 2; i--){
		mvwprintw(win, i, column + 2, ".");
		for (time = 1; time <= 10000000; time += 3)
			;
		wrefresh(win);
	}

	for (i = 2; i <= 35; i++){
		mvwprintw(win, i, column + 2, " ");
	}
	score++;
	return score;
	wattroff(win, COLOR_PAIR(2));
}

void character::squareStuffed(int row, int column, int height, int width, char c, WINDOW *win){

	int i, j;	
	int stores; 

	for (i = 0; i < height; i += 1, row++){

		for (j = 0, stores = column; j < width; j += 1, stores++){

			mvwprintw(win, row, stores, "%c", c);

		} 

	} 

} 

void zombie::drawZombie(int *monster, int shot, WINDOW *win){

	int i;
	int j;

	init_pair(10, COLOR_RED, COLOR_BLACK);
	init_pair(11, COLOR_WHITE, COLOR_BLACK);

	for (j = 1; j <= 160; j += 6){

		wattron(win, COLOR_PAIR(10) | A_BOLD); /* activa los colores */

		for (i = 2; i <= monster[2 + j]; i++){

			if (i == monster[2 + j]){

				wattron(win, COLOR_PAIR(11) | A_BOLD); /* activa los colores */
				mvwprintw(win, i, 2 + j, "0  ");
				mvwprintw(win, i + 1, 2 + j, "||\\");
				mvwprintw(win, i + 2, 2 + j, "/\\");
				wattroff(win, COLOR_PAIR(11) | A_BOLD); /* desactiva los colores */

			}else
				mvwprintw(win, i, 2 + j, "!");
		}
	}

	wattroff(win, COLOR_PAIR(10) | A_BOLD); 
}

void zombie::starZombie(int *monster, WINDOW *win){

	int i;

	for (i = 1; i <= 160; i += 6){

		monster[i + 3] = 1 + (int)(3.0 * rand() / (RAND_MAX + 1.0));
	}
}

void zombie::reproduceZombie(int *monster, WINDOW *win){

	int i;

	for (i = 3; i <= 100; i += 1){

		if (0 + (int)(20.0 * rand() / (RAND_MAX + 1.0)) == 1)
			monster[i]++;
	}
}

void zombie::killZombie(int *monster, int shot, WINDOW *win){

	monster[shot] = 0;
}

int zombie::checaPierde(int *monster, WINDOW *win){

	int i;
	int condition = 0;

	for (i = 3; i <= 160; i += 6){

		if (monster[i] > 32)
			condition = 1;

	}

	return condition;

} 

void recordScore::save(char name[], int score, int duration){
	ofstream fileScores("scores/scores.txt", ios::app);

	fileScores << name << " " << score << " " << duration << endl;

	fileScores.close();
}

int recordScore::search(string searchName){

	string name, scores;
	ifstream fileScores;
	fileScores.open("scores/scores.txt");

	if (fileScores.fail()){
		cout << "Error en el fileScores!!" << endl;
	}else{

		while (!fileScores.eof()){
			fileScores >> name >> scores;

			if (searchName == name){
				return 1;
			}else{
				return 0;
			}
		}
	}
	fileScores.close();
}

void menu();
void History();
void game(char name[], char n);
void clearEmpty(string name, char n);
void see();

int main(int argc, char const *argv[]){
	initscr();

	if (has_colors() == FALSE){
		endwin();
		cout << "Sorry \n Your terminal does not handle colors";
		return 1;
	}

	start_color();
	init_pair(1, COLOR_MAGENTA, COLOR_WHITE);
	bkgd(COLOR_PAIR(1));
	attrset(A_BOLD | A_BLINK | A_UNDERLINE);

	attroff(A_BOLD | A_BLINK | A_UNDERLINE);
	refresh();
	menu();

	endwin();
	return 0;
}

void menu(){

	int op, cont = 0;

	WINDOW* windowMenu;

	windowMenu = newwin(30, 150, 5, 0);

	initscr();

	wrefresh(windowMenu);
	mvwprintw(windowMenu, 1, 55, "Zombies");

	mvwprintw(windowMenu, 3, 55, "1.Play");
	mvwprintw(windowMenu, 4, 55, "2.See scores");
	mvwprintw(windowMenu, 5, 55, "3.Close game");
	wprintw(windowMenu, "\n");

	while (op != 0){

		mvwscanw(windowMenu, 6, 55, "%d", &op);
		switch (op){
		case 1:
			History();

			break;
		case 2:

			see();

			break;

		case 3:
			endwin();
			system("clear");
			exit(0);
			break;
		default:
			wrefresh(windowMenu);

			mvwprintw(windowMenu, 8, 55, "No valid option");

			break;
		}
	}
	endwin();
}

void History(){
	recordScore r;
	character p;
	int key, b = 0, a = 0;
	char n;
	WINDOW* windowHistory;

	windowHistory = newwin(30, 150, 5, 0);
	init_pair(1, COLOR_RED, COLOR_BLACK);

	initscr();

	while (a == 0){

		mvwprintw(windowHistory, 1, 55, "Resistance to surge of zombies");

		mvwprintw(windowHistory, 3, 55, "Enter your username:");
		mvwgetstr(windowHistory, 5, 55, p.name);

		int find = r.search(p.name);

		while (find != 1 && b == 0){

			wrefresh(windowHistory);

			p.draw(8, 55, windowHistory, p.name[0]);

			wgetch(windowHistory);
			wrefresh(windowHistory);

			wclear(windowHistory);

			p.draw(10, 40, windowHistory, p.name[0]);

			mvwprintw(windowHistory, 10, 60, "  o  ");
			mvwprintw(windowHistory, 10, 70, "¡Hey %s Help us!", p.name);
			mvwprintw(windowHistory, 11, 60, "/| |\\");
			mvwprintw(windowHistory, 12, 60, " /_\\");

			mvwprintw(windowHistory, 2, 101, "  o  ");
			mvwprintw(windowHistory, 3, 101, "/| |\\");
			mvwprintw(windowHistory, 4, 101, " /_\\");

			mvwprintw(windowHistory, 5, 100, "  o  ");
			mvwprintw(windowHistory, 6, 100, "/| |\\");
			mvwprintw(windowHistory, 7, 100, " /_\\");

			mvwprintw(windowHistory, 16, 100, "  o  ");
			mvwprintw(windowHistory, 17, 100, "/| |\\");
			mvwprintw(windowHistory, 18, 100, " /_\\");

			mvwprintw(windowHistory, 19, 101, "  o  ");
			mvwprintw(windowHistory, 20, 101, "/| |\\");
			mvwprintw(windowHistory, 21, 101, " /_\\");

			wgetch(windowHistory);
			wrefresh(windowHistory);

			wclear(windowHistory);

			for (int i = 40; i < 60; ++i){
				mvwprintw(windowHistory, 5, i, "-");
			}

			wattron(windowHistory, COLOR_PAIR(1) | A_BOLD);
			mvwprintw(windowHistory, 2, 50, "0  ");
			mvwprintw(windowHistory, 3, 50, "||\\");
			mvwprintw(windowHistory, 4, 50, "/\\");
			wattroff(windowHistory, COLOR_PAIR(1) | A_BOLD);

			p.draw(16, 40, windowHistory, p.name[0]);
			for (int i = 70; i < 150; ++i){
				mvwprintw(windowHistory, 2, i, "*");
			}

			for (int i = 2; i < 15; ++i){
				mvwprintw(windowHistory, i, 70, "*");
			}

			for (int i = 70; i < 150; ++i){
				mvwprintw(windowHistory, 15, i, "*");
			}
			mvwprintw(windowHistory, 16, 67, "¡**");
			mvwprintw(windowHistory, 16, 60, "  o  ");
			mvwprintw(windowHistory, 4, 71, "Hey %s we have predicted a wave of zombies like the one you see behind", p.name);
			mvwprintw(windowHistory, 5, 71, "the line, you can only kill them by shooting them in the head, but none");
			mvwprintw(windowHistory, 6, 71, "of us has the courage to do it, we want you to help us keep them");
			mvwprintw(windowHistory, 7, 71, "behind the wire as much as you can.");
			mvwprintw(windowHistory, 8, 71, "Every time you shoot, you give it a zombie even if you do not see it, since we do not ");
			mvwprintw(windowHistory, 9, 71, "know how many they are. ");
			mvwprintw(windowHistory, 13, 71, "GOOD LUCK!");
			mvwprintw(windowHistory, 17, 60, "/| |\\");
			mvwprintw(windowHistory, 18, 60, " /_\\");
			mvwprintw(windowHistory, 18, 70, ".");
			wgetch(windowHistory);

			game(p.name, p.name[0]);
			endwin();
		}
	}

	endwin();
}

void clearEmpty(char name[], char n){

	WINDOW *clearEmpty;

	clearEmpty = newwin(50, 150, 0, 0);

	initscr();
	refresh();
	wrefresh(clearEmpty);
	werase(clearEmpty);
	init_pair(1, COLOR_MAGENTA, COLOR_WHITE);
	wbkgd(clearEmpty, COLOR_PAIR(1) | A_BOLD);


	werase(clearEmpty);
	wrefresh(clearEmpty); 
	nocbreak(); 
	menu();
}

void game(char name[], char n){

	clock_t duration;
	recordScore r;
	character p;
	int pun;
	zombie z;
	WINDOW *windowPlay;

	windowPlay = newwin(50, 150, 0, 0);

	initscr();
	refresh();
	wrefresh(windowPlay);
	werase(windowPlay);
	init_pair(1, COLOR_MAGENTA, COLOR_WHITE);

	
	int key;

	int naveX = 1;		   
	const int NAVE_Y = 35; 

	int monster[161] = {0};

	srand(time(NULL));

	z.starZombie(monster, windowPlay);

	keypad(windowPlay, TRUE);  
	curs_set(FALSE);			
	cbreak();					
	nodelay(windowPlay, TRUE); 

	if (has_colors()){

		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(8, COLOR_GREEN, COLOR_BLACK);
		init_pair(13, COLOR_WHITE, COLOR_BLUE);

	}else{

		endwin();
		cout << "You do not have support to use colors!";
	

	} /* fin de else */

	wbkgd(windowPlay, COLOR_PAIR(1) | A_BOLD);
	wborder(windowPlay, '|', '|', '-', '-', '+', '+', '+', '+'); /* draw un cuadro */

	wattron(windowPlay, COLOR_PAIR(12)); /* activa los colores */
	mvwprintw(windowPlay, 3, 115, "%s", name);

	for (int i = 1; i < 100; i += 6){
		mvwprintw(windowPlay, 34, i, "-");
		mvwprintw(windowPlay, i, 105, "|");
	}

	mvwprintw(windowPlay, 0, 1, "BACK F2");
	wattroff(windowPlay, COLOR_PAIR(12)); /* desactiva los colores */

	wattron(windowPlay, COLOR_PAIR(13)); /* activa los colores */
	mvwprintw(windowPlay, 0, 35, "\"KILL THE ZOMBIS WAVE\"");
	wattroff(windowPlay, COLOR_PAIR(13)); /* desactiva los colores */

	wattron(windowPlay, COLOR_PAIR(1)); /* activa los colores */

	z.drawZombie(monster, 1, windowPlay);
	duration = clock();
	do{

	
		if (key == KEY_UP || key == KEY_LEFT || key == KEY_RIGHT){

			p.squareStuffed(2, 3, 32, 100, ' ', windowPlay);
			z.drawZombie(monster, 1, windowPlay);

		} 

		if ((int)(10000.0 * rand() / (RAND_MAX + 1.0)) == 0){

			p.squareStuffed(2, 3, 32, 100, ' ', windowPlay);
			z.drawZombie(monster, 1, windowPlay);

		} 

		p.draw(NAVE_Y, naveX, windowPlay, n);

		key = wgetch(windowPlay);

		if (key == KEY_RIGHT && naveX != 100)
			naveX++;

		else if (key == KEY_LEFT && naveX != 1)
			naveX--;

		else if (key == KEY_UP){

			pun = p.shot(naveX, windowPlay);
			z.killZombie(monster, naveX + 2, windowPlay);

			wrefresh(windowPlay);

			mvwprintw(windowPlay, 5, 115, "shots: %i", pun);
		}

		p.squareStuffed(35, 1, 3, 160, ' ', windowPlay);

		z.drawZombie(monster, 1, windowPlay);

		if ((int)(1000.0 * rand() / (RAND_MAX + 1.0)) == 0)
			z.reproduceZombie(monster, windowPlay);

		if (z.checaPierde(monster, windowPlay)){
			duration = clock() - duration;
			z.drawZombie(monster, 1, windowPlay);
			key = KEY_F(2);
			p.draw(NAVE_Y, naveX, windowPlay, n);
			nodelay(windowPlay, FALSE);
			wattron(windowPlay, COLOR_PAIR(8) | A_BOLD | A_UNDERLINE);
			p.squareStuffed(35, 1, 5, 160, ' ', windowPlay);
			mvwprintw(windowPlay, 30, 115, "YOU LOST");
			mvwprintw(windowPlay, 31, 115, "THANK YOU FOR HELPING");
			mvwprintw(windowPlay, 32, 115, "DURED SECONDS: %i ", int(duration) / CLOCKS_PER_SEC);
			r.save(name, pun, int(duration) / CLOCKS_PER_SEC);
			wattroff(windowPlay, COLOR_PAIR(8) | A_BOLD | A_UNDERLINE);
			while (wgetch(windowPlay) != KEY_F(2));
		}

	} while (key != KEY_F(2));

	wattroff(windowPlay, COLOR_PAIR(1));
	werase(windowPlay);
	wrefresh(windowPlay);
	nocbreak();
	clearEmpty(name, n);

	endwin();
}

void see(){
	recordScore r;
	WINDOW* ven;

	char c;

	ven = newwin(30, 150, 5, 0);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	init_pair(12, COLOR_CYAN, COLOR_BLACK);
	char name[50], shots[50], duration[50];
	FILE* f;
	f = fopen("scores/scores.txt", "r");
	initscr();

	if (f != NULL){

		while (!feof(f)){
			fscanf(f, "%s %s %s \n", name, shots, duration);
			wclear(ven);
			wrefresh(ven);

			mvwprintw(ven, 4, 55, "Name: %s", name);
			mvwprintw(ven, 5, 55, "Shots: %s", shots);
			mvwprintw(ven, 6, 55, "Seconds: %s", duration);

			wattron(ven, COLOR_PAIR(5) | A_BOLD);
			mvwprintw(ven, 8, 55, "  o  ");
			mvwprintw(ven, 9, 55, "/| |\\");
			mvwprintw(ven, 10, 55, " /_\\");
			wattroff(ven, COLOR_PAIR(5) | A_BOLD);

			wattron(ven, COLOR_PAIR(12) | A_BOLD);
			mvwprintw(ven, 9, 55 + 1, "! !");
			wattroff(ven, COLOR_PAIR(12) | A_BOLD);

			wattron(ven, COLOR_PAIR(3) | A_BOLD);
			mvwprintw(ven, 9, 55 + 2, "%c", name[0]);
			wattroff(ven, COLOR_PAIR(3) | A_BOLD);
			wgetch(ven);
		}
		fclose(f);
	}

	mvwprintw(ven, 13, 55, "End of players");

	wgetch(ven);
	menu();
	endwin();
}
