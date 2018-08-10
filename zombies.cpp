#include <stdio.h>
#include <ncurses.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <ctime>
#include <unistd.h>

using namespace std;

class character
{
  public:
	char name[50];
	int score;
	void draw(int, int, WINDOW *win, char n);
	int disparo(int, WINDOW *win);
	void cuadradoRelleno(int, int, int, int, char, WINDOW *);
	character();
};

class zombie
{
  public:
	void drawZombie(int *, int, WINDOW *win);
	void starZombie(int *, WINDOW *win);
	void reproduceZombie(int *, WINDOW *win);
	void killZombie(int *, int, WINDOW *win);
	int checaPierde(int *, WINDOW *win);
	zombie();
};

class recordScore
{
  public:
	void guadar(char name[], int score, int tiempo);
	int buscar(string searchName);
	recordScore();
};

character::character()
{
	score = 0;
}

// zombie::zombie()
// {
// }

// recordScore::recordScore()
// {
// }

void character::draw(int row, int column, WINDOW *win, char n)
{

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
int character::disparo(int column, WINDOW *win)
{
	int i;
	int time;

	init_pair(2, COLOR_RED, COLOR_BLACK);
	wattron(win, COLOR_PAIR(2));

	for (i = 35; i >= 2; i--)
	{
		mvwprintw(win, i, column + 2, ".");
		for (time = 1; time <= 10000000; time += 3)
			;
		wrefresh(win);
	}

	for (i = 2; i <= 35; i++)
	{
		mvwprintw(win, i, column + 2, " ");
	}
	score++;
	return score;
	wattroff(win, COLOR_PAIR(2));
}

void character::cuadradoRelleno(int row, int column, int altura, int ancho, char c, WINDOW *win)
{

	int i, j;	 /* contadores */
	int almacena; /* almacena la column */

	for (i = 0; i < altura; i += 1, row++)
	{

		for (j = 0, almacena = column; j < ancho; j += 1, almacena++)
		{

			mvwprintw(win, row, almacena, "%c", c);

		} 

	} 

} 

void zombie::drawZombie(int *monster, int disparo, WINDOW *win)
{

	int i;
	int j;

	init_pair(10, COLOR_RED, COLOR_BLACK);
	init_pair(11, COLOR_WHITE, COLOR_BLACK);

	for (j = 1; j <= 160; j += 6)
	{

		wattron(win, COLOR_PAIR(10) | A_BOLD); /* activa los colores */

		for (i = 2; i <= monster[2 + j]; i++)
		{

			if (i == monster[2 + j])
			{

				wattron(win, COLOR_PAIR(11) | A_BOLD); /* activa los colores */
				mvwprintw(win, i, 2 + j, "0  ");
				mvwprintw(win, i + 1, 2 + j, "||\\");
				mvwprintw(win, i + 2, 2 + j, "/\\");
				wattroff(win, COLOR_PAIR(11) | A_BOLD); /* desactiva los colores */

			} /* end else */

			else
				mvwprintw(win, i, 2 + j, "!");
		}
	}

	wattroff(win, COLOR_PAIR(10) | A_BOLD); /* desactiva los colores */
}

void zombie::starZombie(int *monster, WINDOW *win)
{

	int i;

	for (i = 1; i <= 160; i += 6)
	{

		monster[i + 3] = 1 + (int)(3.0 * rand() / (RAND_MAX + 1.0));
	}
}

void zombie::reproduceZombie(int *monster, WINDOW *win)
{

	int i;

	for (i = 3; i <= 100; i += 1)
	{

		if (0 + (int)(20.0 * rand() / (RAND_MAX + 1.0)) == 1)
			monster[i]++;
	}
}

void zombie::killZombie(int *monster, int disparo, WINDOW *win)
{

	monster[disparo] = 0;
}

int zombie::checaPierde(int *monster, WINDOW *win)
{

	int i;
	int condicion = 0;

	for (i = 3; i <= 160; i += 6)
	{

		if (monster[i] > 32)
			condicion = 1;

	} /* end for */

	return condicion;

} /* fin de la función checaPierde */

void recordScore::guadar(char name[], int score, int tiempo)
{
	ofstream archivo("scores/scores.txt", ios::app);

	archivo << name << " " << score << " " << tiempo << endl;

	archivo.close();
}

int recordScore::buscar(string searchName)
{

	string name, scores;
	ifstream archivo;
	archivo.open("scores/scores.txt");
	if (archivo.fail())
	{
		cout << "Error en el archivo!!" << endl;
	}
	else
	{

		while (!archivo.eof())
		{
			archivo >> name >> scores;

			if (searchName == name)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
	archivo.close();
}

void menu();
void Historia();
void juego(char name[], char n);
void vacio(string name, char n);
void ver();

int main(int argc, char const *argv[])
{
	initscr();

	if (has_colors() == FALSE)
	{
		endwin();
		cout << "LO SENTIMOS \n Su terminal no maneja colores";
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

void menu()
{

	int op, cont = 0;

	WINDOW *ventanaMenu;

	ventanaMenu = newwin(30, 150, 5, 0);

	initscr();

	wrefresh(ventanaMenu);
	mvwprintw(ventanaMenu, 1, 55, "Zombies");

	mvwprintw(ventanaMenu, 3, 55, "1.Jugar");
	mvwprintw(ventanaMenu, 4, 55, "2.Ver scores");
	mvwprintw(ventanaMenu, 5, 55, "3.Cerrar Juego");
	wprintw(ventanaMenu, "\n");

	while (op != 0)
	{

		mvwscanw(ventanaMenu, 6, 55, "%d", &op);
		switch (op)
		{
		case 1:

			Historia();

			break;
		case 2:

			ver();

			break;

		case 3:
			endwin();
			system("clear");
			exit(0);
			break;
		default:
			wrefresh(ventanaMenu);

			mvwprintw(ventanaMenu, 8, 55, "Opcion no valida");

			break;
		}
	}
	endwin();
}

void Historia()
{
	recordScore r;
	character p;
	int key, b = 0, a = 0;
	char n;
	WINDOW *ventanaHistoria;

	ventanaHistoria = newwin(30, 150, 5, 0);
	init_pair(1, COLOR_RED, COLOR_BLACK);

	initscr();

	while (a != 0)
		;
	{

		mvwprintw(ventanaHistoria, 1, 55, "Resistencia a oleada de zombies");

		mvwprintw(ventanaHistoria, 3, 55, "Ingresa tu name de usuario:");
		mvwgetstr(ventanaHistoria, 5, 55, p.name);

		int encontro = r.buscar(p.name);

		while (encontro != 1 && b == 0)
		{

			wrefresh(ventanaHistoria);

			p.draw(8, 55, ventanaHistoria, p.name[0]);

			wgetch(ventanaHistoria);
			wrefresh(ventanaHistoria);

			wclear(ventanaHistoria);

			p.draw(10, 40, ventanaHistoria, p.name[0]);

			mvwprintw(ventanaHistoria, 10, 60, "  o  ");
			mvwprintw(ventanaHistoria, 10, 70, "¡Hey %s Ayudanos!", p.name);
			mvwprintw(ventanaHistoria, 11, 60, "/| |\\");
			mvwprintw(ventanaHistoria, 12, 60, " /_\\");

			mvwprintw(ventanaHistoria, 2, 101, "  o  ");
			mvwprintw(ventanaHistoria, 3, 101, "/| |\\");
			mvwprintw(ventanaHistoria, 4, 101, " /_\\");

			mvwprintw(ventanaHistoria, 5, 100, "  o  ");
			mvwprintw(ventanaHistoria, 6, 100, "/| |\\");
			mvwprintw(ventanaHistoria, 7, 100, " /_\\");

			mvwprintw(ventanaHistoria, 16, 100, "  o  ");
			mvwprintw(ventanaHistoria, 17, 100, "/| |\\");
			mvwprintw(ventanaHistoria, 18, 100, " /_\\");

			mvwprintw(ventanaHistoria, 19, 101, "  o  ");
			mvwprintw(ventanaHistoria, 20, 101, "/| |\\");
			mvwprintw(ventanaHistoria, 21, 101, " /_\\");

			wgetch(ventanaHistoria);
			wrefresh(ventanaHistoria);

			wclear(ventanaHistoria);

			for (int i = 40; i < 60; ++i)
			{
				mvwprintw(ventanaHistoria, 5, i, "-");
			}

			wattron(ventanaHistoria, COLOR_PAIR(1) | A_BOLD);
			mvwprintw(ventanaHistoria, 2, 50, "0  ");
			mvwprintw(ventanaHistoria, 3, 50, "||\\");
			mvwprintw(ventanaHistoria, 4, 50, "/\\");
			wattroff(ventanaHistoria, COLOR_PAIR(1) | A_BOLD);

			p.draw(16, 40, ventanaHistoria, p.name[0]);
			for (int i = 70; i < 150; ++i)
			{
				mvwprintw(ventanaHistoria, 2, i, "*");
			}

			for (int i = 2; i < 15; ++i)
			{
				mvwprintw(ventanaHistoria, i, 70, "*");
			}

			for (int i = 70; i < 150; ++i)
			{
				mvwprintw(ventanaHistoria, 15, i, "*");
			}
			mvwprintw(ventanaHistoria, 16, 67, "¡**");
			mvwprintw(ventanaHistoria, 16, 60, "  o  ");
			mvwprintw(ventanaHistoria, 4, 71, "¡Hey %s hemos predesido una oleada de zombies como el que ves detras", p.name);
			mvwprintw(ventanaHistoria, 5, 71, "de la linea, solo se les puede matar disparandoles en la cabeza,pero ninguno");
			mvwprintw(ventanaHistoria, 6, 71, "de nosotros tiene el valor para hacerlo, queremos que nos ayudes a mantenerlos");
			mvwprintw(ventanaHistoria, 7, 71, "detras del alabre de puas lo mas que puedas.");
			mvwprintw(ventanaHistoria, 8, 71, "Cada ves que disparas le das a un zombie aunque no lo veas, ya que no ");
			mvwprintw(ventanaHistoria, 9, 71, "sabemos cuantos son. ");
			mvwprintw(ventanaHistoria, 13, 71, "¡SUERTE!");
			mvwprintw(ventanaHistoria, 17, 60, "/| |\\");
			mvwprintw(ventanaHistoria, 18, 60, " /_\\");
			mvwprintw(ventanaHistoria, 18, 70, ".");
			wgetch(ventanaHistoria);

			juego(p.name, p.name[0]);
			endwin();
		}
	}

	endwin();
}

void vacio(char name[], char n)
{

	WINDOW *vacio;

	vacio = newwin(50, 150, 0, 0);

	initscr();
	refresh();
	wrefresh(vacio);
	werase(vacio);
	init_pair(1, COLOR_MAGENTA, COLOR_WHITE);
	wbkgd(vacio, COLOR_PAIR(1) | A_BOLD);

	/* desactiva los colores */
	werase(vacio);
	wrefresh(vacio); /* refresca la pantalla */
	//wgetch(vacio);		/* espera una pulsación para continuar */
	nocbreak(); /* desactiva el envio inmediato de teclas */
	menu();
}

void juego(char name[], char n)
{

	clock_t tiempo;
	recordScore r;
	character p;
	int pun;
	zombie z;
	WINDOW *ventanaJugar;

	ventanaJugar = newwin(50, 150, 0, 0);

	initscr();
	refresh();
	wrefresh(ventanaJugar);
	werase(ventanaJugar);
	init_pair(1, COLOR_MAGENTA, COLOR_WHITE);

	////////////////////////////////////////77
	/* variables locales */
	int key; /* almacena una tecla */

	int naveX = 1;		   /* coordenada X */
	const int NAVE_Y = 35; /* coordenada Y */

	int monster[161] = {0};

	srand(time(NULL));

	z.starZombie(monster, ventanaJugar);

	keypad(ventanaJugar, TRUE);  /* activa / desactiva las teclas especiales */
	curs_set(FALSE);			 /* hace invisible el cursor fisico */
	cbreak();					 /* activa el envio inmediato de teclas */
	nodelay(ventanaJugar, TRUE); /* activa / desactiva la espera para lectura del teclado */

	if (has_colors() /* verifica si la terminal tiene soporte o no */)
	{

		start_color(); /* inicia el modo de colores */
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(8, COLOR_GREEN, COLOR_BLACK);
		init_pair(13, COLOR_WHITE, COLOR_BLUE);

	} /* fin de if colores */

	else
	{

		endwin();
		cout << "¡No tienes soporte para usar colores!";
		//SALIRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRrr

	} /* fin de else */

	wbkgd(ventanaJugar, COLOR_PAIR(1) | A_BOLD);
	wborder(ventanaJugar, '|', '|', '-', '-', '+', '+', '+', '+'); /* draw un cuadro */

	wattron(ventanaJugar, COLOR_PAIR(12)); /* activa los colores */
	mvwprintw(ventanaJugar, 3, 115, "%s", name);

	for (int i = 1; i < 100; i += 6)
	{
		mvwprintw(ventanaJugar, 34, i, "-");
		mvwprintw(ventanaJugar, i, 105, "|");
	}
	mvwprintw(ventanaJugar, 0, 1, "ATRRAS F2");
	wattroff(ventanaJugar, COLOR_PAIR(12)); /* desactiva los colores */

	wattron(ventanaJugar, COLOR_PAIR(13)); /* activa los colores */
	mvwprintw(ventanaJugar, 0, 35, "\"ANIQUILA LA OLEADA DE ZOMBIES\"");
	wattroff(ventanaJugar, COLOR_PAIR(13)); /* desactiva los colores */

	wattron(ventanaJugar, COLOR_PAIR(1)); /* activa los colores */

	z.drawZombie(monster, 1, ventanaJugar);
	tiempo = clock();
	do
	{

		/* encargado del cielo */
		if (key == KEY_UP || key == KEY_LEFT || key == KEY_RIGHT)
		{

			p.cuadradoRelleno(2, 3, 32, 100, ' ', ventanaJugar);
			z.drawZombie(monster, 1, ventanaJugar);

		} /* end if */

		if ((int)(10000.0 * rand() / (RAND_MAX + 1.0)) == 0)
		{

			p.cuadradoRelleno(2, 3, 32, 100, ' ', ventanaJugar);
			z.drawZombie(monster, 1, ventanaJugar);

		} /* end if */

		p.draw(NAVE_Y, naveX, ventanaJugar, n);

		key = wgetch(ventanaJugar);

		if (key == KEY_RIGHT && naveX != 100)
			naveX++;

		else if (key == KEY_LEFT && naveX != 1)
			naveX--;

		else if (key == KEY_UP)
		{

			pun = p.disparo(naveX, ventanaJugar);
			z.killZombie(monster, naveX + 2, ventanaJugar);

			wrefresh(ventanaJugar);

			mvwprintw(ventanaJugar, 5, 115, "Disparos: %i", pun);
		}

		p.cuadradoRelleno(35, 1, 3, 160, ' ', ventanaJugar);

		z.drawZombie(monster, 1, ventanaJugar);

		if ((int)(1000.0 * rand() / (RAND_MAX + 1.0)) == 0)
			z.reproduceZombie(monster, ventanaJugar);

		if (z.checaPierde(monster, ventanaJugar))
		{
			tiempo = clock() - tiempo;
			z.drawZombie(monster, 1, ventanaJugar);
			key = KEY_F(2);
			p.draw(NAVE_Y, naveX, ventanaJugar, n);
			nodelay(ventanaJugar, FALSE);
			wattron(ventanaJugar, COLOR_PAIR(8) | A_BOLD | A_UNDERLINE);
			p.cuadradoRelleno(35, 1, 5, 160, ' ', ventanaJugar);
			mvwprintw(ventanaJugar, 30, 115, "PERDISTE");
			mvwprintw(ventanaJugar, 31, 115, "GRACIAS POR AYUDAR");
			mvwprintw(ventanaJugar, 32, 115, "SEGUNNDOS DURADOS: %i ", int(tiempo) / CLOCKS_PER_SEC);
			r.guadar(name, pun, int(tiempo) / CLOCKS_PER_SEC);
			wattroff(ventanaJugar, COLOR_PAIR(8) | A_BOLD | A_UNDERLINE);
			while (wgetch(ventanaJugar) != KEY_F(2))
				;
		}

	} while (key != KEY_F(2));

	wattroff(ventanaJugar, COLOR_PAIR(1));
	werase(ventanaJugar);
	wrefresh(ventanaJugar);
	nocbreak();
	vacio(name, n);

	endwin();
}

void ver()
{
	recordScore r;
	WINDOW *ven;

	char c;

	ven = newwin(30, 150, 5, 0);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	init_pair(12, COLOR_CYAN, COLOR_BLACK);
	char name[50], disparos[50], tiempo[50];
	FILE *f;
	f = fopen("scores/scores.txt", "r");
	initscr();

	if (f != NULL)
	{
		while (!feof(f))
		{
			fscanf(f, "%s %s %s \n", name, disparos, tiempo);
			wclear(ven);
			wrefresh(ven);

			mvwprintw(ven, 4, 55, "name: %s", name);
			mvwprintw(ven, 5, 55, "Disparos: %s", disparos);
			mvwprintw(ven, 6, 55, "Segundos: %s", tiempo);

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

	mvwprintw(ven, 13, 55, "Fin de jugadores");

	wgetch(ven);
	menu();
	endwin();
}
