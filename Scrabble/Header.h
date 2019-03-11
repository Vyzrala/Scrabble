#pragma once

#define POZYCJA_LEGENDY_X 3
#define POZYCJA_LEGENDY_Y 1
#define POZYCJA_PLANSZY_X 60
#define POZYCJA_PLANSZY_Y 3
#define ROZMIAR_PLANSZY 15
#define ILOSC_LITER 26
#define CAPACITY_OF_BOX 7
#define WSBON 3
#define PREMIA_S 7
#define PREMIA_P 50

struct LETTER {
	char token; // Sign of the letter
	int points; // how many point for that letter
	int counter; // how many of that letter we can find in box
	bool availability; // if available true, if not false
	bool marked; //used to change the tile
	int color; // describe the color of letter
};

struct pole {
	char token; // Sign of the letter
	int points; // points for letter
	int LS; // bonus for letter
	int WS; // bonus for word
};

void cursors(int*, int*, int*, int*,int*, int=0);

void show_box(LETTER *,int, int); 

void uzupelnij(LETTER*, LETTER*, int*);

int losuj();

void komunikat(int, int, char*);

void name_of_file(char*);

void wczytaj(pole**, LETTER*, int*, int*);

void zapisz(pole**, LETTER*, int, int);

void orientation(int*, int*, int*, int *, int p);

void put_on_board(pole**, LETTER*, int*, int*, int*, int, LETTER*, LETTER*);

void wstaw(LETTER *, LETTER *, int , int*);

void fill_my_box(LETTER *, LETTER *, int*);

void wymien(int, LETTER *, LETTER *);

void box_of_letters(LETTER *);

void Menu(int, int, int, int, int, int);

bool czy_mozna_wstawic(pole**, LETTER*, int, int, int*,int*);

void wpisywanie_slowa(int*, int*, int*, int*, int*, int*, LETTER *, LETTER*, pole**, int*, int*, bool*);

int punktacja(pole**, bool*, int*);

void Rysuj_plansze(pole **);

void RYSUJ(int*, int*, int*, int*, int*, int*, LETTER *, LETTER*, pole**, int*);

void new_game(LETTER *, LETTER *, pole **, int*);

bool warunki(pole**, int*, int*, int*, int , int, int);

bool czy_sasiad(pole**, int*, int*, int*, int*);

void color(pole**, int*, int*, int*, int, int, int,int, LETTER*);

void show_pts(int);

// przez to ze funkcje przyjmuja rozne typy zmiennych kompilator wie ktorej funkcji uzyc co nie powoduje konfliktu 
// takich samych nazw uzylem bo funkcje robia dok³adnie to samo tylko na roznych typach zmiennych

//pole
void zeruj_pole_planszy(pole**, int, int);

void zeruj_tablice(pole**, int);

//letter
void zeruj_tablice(LETTER *, int);

void zeruj_pole(LETTER *, int);