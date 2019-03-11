
#include<stdio.h>
#include"conio2.h"
#include "Header.h"
#include<stdlib.h>
#include<time.h>

#include <iostream>
using namespace std;

/* Comment: in the final program declare appropriate constants, e.g.,
   to eliminate from your program numerical values by replacing them
   
   with well defined identifiers */

int main() { // x=40; y=12;
	int zn = 0, x = POZYCJA_PLANSZY_X+ROZMIAR_PLANSZY/2, y = POZYCJA_PLANSZY_Y+ROZMIAR_PLANSZY/2, attr = 7, back = 0, zero = 0;
	//char txt[32];
	int score = 0; // punkty gracza
	int count; // liczba wszystkich dostepnych liter
	int o = 0; // orientacja slowa
	bool premia = false;
	int licznik_slowa = 0;
	srand(time(NULL));
	// if the program is compiled in pure C, then we need to initialize
	// the library ourselves; __cplusplus is defined only if a C++ compiler
	// is used
#ifndef __cplusplus
	Conio2_Init();
#endif
	// settitle sets the window title
	settitle("MarcinID");

	// allocating memory
	LETTER *letters = new LETTER[ILOSC_LITER];
	LETTER *my_box = new LETTER[CAPACITY_OF_BOX];
	pole **plansza = new pole *[ROZMIAR_PLANSZY];

	for (int i = 0; i < ROZMIAR_PLANSZY; i++)
		plansza[i] = new pole[ROZMIAR_PLANSZY];

	new_game(letters, my_box, plansza, &score);
	
	// hide the blinking cursor
	_setcursortype(_NOCURSOR);
		count = 0; 
	do {
		fill_my_box(my_box, letters, &count);
		
		//count = 0; 
		//for (int i = 0; i < ILOSC_LITER; i++) { // counting how many letter do we have in big box
		//	count += letters[i].counter;
		//}
		RYSUJ(&zn,&x,&y,&attr,&zero,&back,my_box,letters,plansza, &count);

		textcolor(WHITE);

		//score = punktacja(plansza, premia);
		show_pts(score);

		cursors(&zn, &x, &y, &zero,&o);

		if(zn == ' ') attr = (attr + 1) % 16; // space
		else if(zn == 0x09) back = (back + 1) % 16;	// tab
		else if (zn == 0x77 || zn == 0x57) { // "w" command
			wymien(zn, my_box, letters);
		}
		else if (zn == 0x6e || zn == 0x4e) { // "n" command
			new_game(letters, my_box,plansza, &score);
		}
		else if (zn == 0x69 || zn == 0x49) { // "i" command <- 'o' command inside
			wpisywanie_slowa(&zn, &x, &y, &attr, &zero, &back, my_box, letters, plansza,&count,&o, &premia);
		}
		else if (zn == 's' || zn == 'S') { // 's' command (save)
			zapisz(plansza, letters, score,licznik_slowa);
		}
		else if (zn == 'l' || zn == 'L') { // 'l' command (load)
			wczytaj(plansza, letters, &score, &licznik_slowa);
		}
		score = punktacja(plansza, &premia,&licznik_slowa); // licznie punktow

	} while (zn != 'q');

	gotoxy(1, 27);

	// freeing memory
	delete[] letters;
	letters = nullptr;
	delete[] my_box;
	my_box = nullptr;
 
	for (int i = 0; i < ROZMIAR_PLANSZY; i++) 
		delete[] plansza[i];

	delete[] plansza;
	plansza = nullptr;

	_setcursortype(_NORMALCURSOR);	// show the cursor so it will be
					// visible after the program ends
	return 0;
	}