
#include "conio2.h"
#include "Header.h"
#include<stdio.h>
#include<string.h>
#include<windows.h>

#include <iostream>
using namespace std;

void show_box(LETTER *box,int ilosc,int pozycja) { // shows boxes on screen, starts from left

	int p = 0; // skok, co jaki odstep ma wyswietlac litere
	for (int i = 0; i < ilosc; i++) {
		
		if (box[i].availability) 
			textcolor(box[i].color);
		else 
			textcolor(box[i].color);
		
		if (box[i].marked) 
			textbackground(BLUE);
		else 
			textbackground(BLACK);
		
		gotoxy(POZYCJA_LEGENDY_X + p, POZYCJA_LEGENDY_Y +pozycja);
		putch(box[i].token);
		textcolor(WHITE);
		p += 2;
	}
}

void box_of_letters(LETTER *letters) { // sets the bag of letters at the start of game
	int counter[ILOSC_LITER] = {9,2,2,4,12,2,3,2,9,1,1,4,2,6,8,2, 1,6,4,6,4,2,2,1,2, 1};
	//							A,B,C,D, E,F,G,H,I,J,K,L,M,N,O,P, Q,R,S,T,U,V,W,X,Y, Z
     int points[ILOSC_LITER] = {1,3,3,2, 1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};

	for (int i = 0; i < ILOSC_LITER; i++) {
		letters[i].token=65+i;					 // 65 is an ASCII code of "A"
		letters[i].counter = counter[i];
		letters[i].points = points[i];
		letters[i].availability = true;
		letters[i].marked = false;
		letters[i].color = GREEN;
	}
};

void zeruj_pole_planszy(pole **plansza, int x, int y) {
	plansza[x][y].token = NULL;
	plansza[x][y].points = NULL;
	plansza[x][y].LS = 1;
	plansza[x][y].WS = 0;
}

void zeruj_tablice(pole **plansza, int size) {
	for (int i = 0; i < size; i++) 
		for (int j = 0; j < size; j++) 
			zeruj_pole_planszy(plansza, i, j);
}

void zeruj_tablice(LETTER *nowy, int size) {
	for (int i = 0; i < size; i++) 
		zeruj_pole(nowy, i);
}

void zeruj_pole(LETTER *nowy, int index) {
	
	nowy[index].token = NULL;
	nowy[index].counter = NULL;
	nowy[index].points = NULL;
	nowy[index].availability = true;
	nowy[index].marked = false;
	nowy[index].color = NULL;
};

int losuj() {// losuje numer indexu litery z tablicy letters
	int m =  rand() % (ILOSC_LITER) + 0;
	return m;
}

void name_of_file(char *nazwa) { // wpisywanie nazwy pliku
	int index = 0;
	char klaw;
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 25);
	cputs("Type in a name of file: ");
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 26);
	do {
		klaw = getch();
		if (klaw != 0x0d) { // jak klika sie enter to on sie wpisuje enter w nazwe pliku i plik sie przez to nie tworzy, dlatego wpisuje tylko jesli nie jest enterem
			nazwa[index] = klaw;
			putch(nazwa[index]);
			index++;
		}
	} while (klaw != 0x0d);
	nazwa[index] = '.';
	nazwa[index + 1] = 't';
	nazwa[index + 2] = 'x';
	nazwa[index + 3] = 't';
	nazwa[index + 4] = '\0';
}

void zapisz(pole **plansza, LETTER *letters, int score, int licznik_slowa) {
	char nazwa[60];
	char tekst[60] = { "Nie udalo sie utworzyc pliku :/" };
	name_of_file(nazwa);

	FILE *plik = fopen(nazwa, "w+");

	if (plik != NULL) {
		fseek(plik, 0, SEEK_SET);
		//punkty
		fputc(score, plik);
		fputc(licznik_slowa, plik);
		//plansza
		for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
			for (int j = 0; j < ROZMIAR_PLANSZY; j++) {
				fputc(plansza[i][j].token, plik);
				fputc(plansza[i][j].points, plik);
				fputc(plansza[i][j].LS, plik);
				fputc(plansza[i][j].WS, plik);
			}
		}
		// lettersy
		for (int i = 0; i < ILOSC_LITER; i++) {
			fputc(letters[i].token, plik);
			fputc(letters[i].points, plik);
			fputc(letters[i].marked, plik);
			fputc(letters[i].counter, plik);
			fputc(letters[i].color, plik);
			fputc(letters[i].availability, plik);
		}
		fclose(plik); 
	}
	else komunikat(POZYCJA_PLANSZY_X, POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY + 4, tekst);
}

void wczytaj(pole**plansza, LETTER *letters, int* score, int*licznik_slowa) {
	char nazwa[32];
	char tekst[60] = {"Nie udalo sie otworzyc pliku :/"};
	name_of_file(nazwa);

	FILE *plik = fopen(nazwa, "r");
	if (plik != NULL) {
		zeruj_tablice(plansza, ROZMIAR_PLANSZY);
		zeruj_tablice(letters, ILOSC_LITER);
		//punkty
		*score = fgetc(plik);
		*licznik_slowa = fgetc(plik);
		//plansza
		for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
			for (int j = 0; j < ROZMIAR_PLANSZY; j++) {
				plansza[i][j].token = fgetc(plik);
				plansza[i][j].points = fgetc(plik);
				plansza[i][j].LS = fgetc(plik);
				plansza[i][j].WS = fgetc(plik);
			}
		}
		// lettersy
		for (int i = 0; i < ILOSC_LITER; i++) {
			letters[i].token = fgetc(plik);
			letters[i].points = fgetc(plik);
			letters[i].marked = fgetc(plik);
			letters[i].counter = fgetc(plik);
			letters[i].color = fgetc(plik);
			letters[i].availability = fgetc(plik);
		}
		fclose(plik);
	}
	else komunikat(POZYCJA_PLANSZY_X, POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY + 4, tekst);
}

void wstaw(LETTER *my_box, LETTER *letters, int index, int *i) {// (do tego, stad, index sk¹d wstawic, index gdzie wstawic) // puts object from table to table

	my_box[*i] = letters[index];
	letters[index].counter--;
	my_box[*i].counter = 1;

	if (letters[index].counter == 0) {
		letters[index].availability = false;
		letters[index].color = RED;
	}
};

void uzupelnij(LETTER *letters, LETTER *my_box, int *i) {
	if (my_box[*i].token == NULL) {

		int m = losuj();
		if (letters[m].availability)
			wstaw(my_box, letters, m, i);
		else
			(*i)--;
	}
}

void fill_my_box(LETTER *my_box, LETTER *letters, int* count) { // fulfill player's box
	int licz = *count;
	for (int i = 0; i < ILOSC_LITER; i++) { // licznie liter
		licz += letters[i].counter;
	}
	if (licz<= CAPACITY_OF_BOX) { // jesli podczas uzupe³niania boxa gracza jest mniej niz 7 liter (domyslnie) ogolnie, to box gracza nie jest uzupelniany do konca
		for (int i = 0; i < CAPACITY_OF_BOX; i++) {
			if (licz > 0) {
				--(licz);
				uzupelnij(letters, my_box, &i);
			}
			else break;
		}
	}
	else {
		for (int i = 0; i < CAPACITY_OF_BOX; i++) 
			uzupelnij(letters, my_box, &i);
	}
	*count = licz;
};

void wymien(int zn, LETTER *box, LETTER *letters) { // change of chosen tiles
	//marking and unmarking (49 is an ASCII code of 1 "one")
	do {
		zn = getch();

		if (box[zn - 49].marked) // if I want to unmark the letter 
			box[zn - 49].marked = false;
		else					 // if I want to mark the letter
			box[zn - 49].marked = true;

		show_box(box, CAPACITY_OF_BOX, 23);
		
	} while (zn != 0x0d && zn != 119); //enter or 'w'

	//exchange of tile 
	for (int i = 0; i < CAPACITY_OF_BOX; i++) {
		if (box[i].marked) {
			for (int j = 0; j < ILOSC_LITER; j++) {
				if (letters[j].token == box[i].token) {
					zeruj_pole(box, i);

					letters[j].counter++;
					if (letters[j].counter > 0) {
						letters[j].availability = true;
						letters[j].color = GREEN;
					}
				}
			}
		}
	}
};

void Menu(int zn,int x, int y, int attr, int zero, int back) {
	char txt[32];
	textbackground(BLACK);
	clrscr();
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y);
	textcolor(LIGHTRED);
	cputs("Marcin Hebdzynski, 171608, ID");
	textcolor(WHITE);
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 1);
	cputs("Implemented functions: a,b,c,d,e,f,g,h,i"); // a,b,c,d,e,f,g,h,i
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 2);
	cputs("q       = exit");
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 3);
	cputs("cursors = moving");
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 4);
	cputs("space   = change color");
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 5);
	cputs("tab	  = change background color");
	if (zero) sprintf(txt, "key code: 0x00 0x%02x", zn);
	else sprintf(txt, "key code: 0x%02x", zn);
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 6);
	cputs(txt);
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 7);
	cputs("w	  = change chosen tile");
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 8);
	cputs("1 - 7   = chosing tiles");
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 9);
	cputs("n	  = new game");
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 10);
	cputs("i	  = insert a word");
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 11);
	cputs("o	  = change of orientation of word");
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 12);
	cputs("s	  = save");
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 13);
	cputs("l	  = load");

	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 17);
	cputs("Score: ");
	// boxes
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 19);
	cputs("Letters in box: ");
	gotoxy(POZYCJA_LEGENDY_X, POZYCJA_LEGENDY_Y + 22);
	cputs("Your box: ");
}

void wpisywanie_slowa(int *zn, int *x, int *y, int *attr, int *zero, int *back, LETTER *my_box, LETTER *letters, pole **plansza, int *count, int *o, bool *premia){

	textcolor(WHITE);

	int p = 0; // dlugosc/index ostatniej litery  wpisywanego slowa 
	gotoxy(POZYCJA_LEGENDY_X +20,POZYCJA_LEGENDY_Y +22);
	cputs("Type in a word: ");

	LETTER *box_copy = new LETTER[CAPACITY_OF_BOX];
	//kopiowanie dostepnych liter
	for (int i = 0; i < CAPACITY_OF_BOX; i++) 
		box_copy[i] = my_box[i];

	LETTER *word = new LETTER[ROZMIAR_PLANSZY]; // stores word that player want to insert

	// wpisywanie slowa do wpisania na na tablice (przygotowywanie slowa)
	do {
		*zn = getch();
		if (*zn == 0x1b) { // esc 
			gotoxy(POZYCJA_LEGENDY_X + 20, POZYCJA_LEGENDY_Y + 23);
			clreol();
			p = 0;
			zeruj_tablice(box_copy, CAPACITY_OF_BOX);
			zeruj_tablice(word, ROZMIAR_PLANSZY);
		}
		else if (*zn == 0x08 && p>0 ) { // backspace
			p--;
			for (int i = CAPACITY_OF_BOX; i >=0; i--) { // pêtla od tylu bo inaczej wyrzuca jakis blad(idk why), a poza tym lepiej [³adniej(?)] zwraca litery do tablicy
				if (word[p].token == box_copy[i].token && !box_copy[i].availability) {
					box_copy[i].availability = true;
					box_copy[i].color = GREEN;
					zeruj_pole(word, p);
					break;
				}
			}
			if (word[p].color == RED) zeruj_pole(word, p); // usuwanie litery ktorej gracz nie mial w swoim boxie

			gotoxy(POZYCJA_LEGENDY_X + 20 + p, POZYCJA_LEGENDY_Y + 23);
			putch(word[p].token);
		}
		else if(p<ROZMIAR_PLANSZY && p>=0) {// wpisywanie liter
			for (int i = 0; i < CAPACITY_OF_BOX; i++) { // "-32" jest po to, zeby wpisywaly sie wielkie litery
				if ((box_copy[i].token == *zn || box_copy[i].token == *zn-32) && box_copy[i].availability && box_copy[i].color != RED) { // wpisywanie posiadadej litery
					word[p] = box_copy[i];
					box_copy[i].availability = false;
					box_copy[i].color = RED;
					break;
				}
				else if (box_copy[i].token != *zn) { // wpisywanie litery ktora nie jest teraz dostepna dla gracza
					word[p].token = *zn-32; // wpisz drukowana
					word[p].points = 0;
					word[p].color = RED;
				}
			}
			gotoxy(POZYCJA_LEGENDY_X + 20 + p, POZYCJA_LEGENDY_Y + 23);
			textcolor(word[p].color);
			putch(word[p].token);
			p++;
		}
		//show_box(box_copy, CAPACITY_OF_BOX, 25);
	} while (*zn != 0x0d); // enter
	textcolor(WHITE);

	p--; // dlugosc wpisanego slowa
	     // dekrementacja bo po wpisaniu litery automatycznie inkrementuje wskaznik 'p'
		 // i czeka na wpisanie litery w nastêpn¹ komórkê tablicy 'word'
	     // wiec po zakonczeniu wpisywania trzeba wróciæ do poprzedniej dlugosci

	// realokacja
	LETTER *show_word = new LETTER[p];

	for (int i = 0; i < p; i++) {
		show_word[i] = word[i];
	}
	delete[] word;
	word = NULL;

	//If I used letter from my box to create a new word (marked RED) delete this object 
	for (int i = 0; i < CAPACITY_OF_BOX; i++) {
		if (box_copy[i].color == RED) {
			zeruj_pole(box_copy, i);
		}
	}

	// poruszanie kursorem i wyswietlanie slowa
	int licznik = 0; // zlicza ilosc pasujacych liter w danym miejscu na planszy
	int liczba_red = 0; // zlicza ilosc liter zamarkowanych czerwono(brakujacych w boxie gracza) w slowie do wstawienia na plansze
	do {
		if (*zn == 0x1b) { //esc
			zeruj_tablice(show_word, p);
			zeruj_tablice(box_copy, CAPACITY_OF_BOX);
			clreol();
			gotoxy(*x, *y);
			clreol();
			gotoxy(POZYCJA_LEGENDY_X + 20, POZYCJA_LEGENDY_Y + 23);
			clreol();
			gotoxy(POZYCJA_LEGENDY_X + 20, POZYCJA_LEGENDY_Y + 22);
			clreol();
			wpisywanie_slowa(zn, x, y, attr, zero, back, my_box, letters, plansza, count,o, premia);
		}

		RYSUJ(zn, x, y, attr, zero, back, my_box, letters, plansza, count);

		gotoxy(POZYCJA_LEGENDY_X + 20, POZYCJA_LEGENDY_Y + 22);
		cputs("Your word: ");
		//putch(p+48); // write number of letter of word that player has inserted 
		//putch(32); // write space
		//cputs("letters");
		//show_box(box_copy, CAPACITY_OF_BOX, 25);

		// wyswietlanie wpisanego slowa w miesjscu gdzie bylo wpisywane
		gotoxy(POZYCJA_LEGENDY_X + 20 , POZYCJA_LEGENDY_Y + 23);
		for (int i = 0; i < p; i++) {
			textcolor(show_word[i].color);
			putch(show_word[i].token);
		}
		textcolor(WHITE);

		// change of orientation 
		orientation(zn, x, y, o, p);
		char blad[60] = { "Za dlugi wyraz " };

		
		//wyswietlanie slowa na planszy
		licznik = 0;
		liczba_red = 0;
		if (((*o ==0 ) && (*x + p <= POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY)) || // zeby nie wylasil slowem poza plansze
			((*o==1)&&(*y + p <= POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY))) {
			switch (*o) {
			case 0: // poziomo
				for (int i = 0; i < p; i++) {
					if (show_word[i].color == RED) liczba_red++; // liczy ilosc liter ktorych gracz nie posiada w boxie, a wpisal z klawiatury
					if (czy_mozna_wstawic(plansza, show_word, i, 0, x, y)) licznik++; // liczba liter podswietlonych na zielono
				}
				for (int i = 0; i < p; i++) { // kolorowanie wyswietlanego napisu
					gotoxy(*x + i, *y);
					color(plansza, x, y, o, p, liczba_red, licznik, i, show_word);
				}
				break;
			case 1: // pionowo
				for (int i = 0; i < p; i++) {
					if (show_word[i].color == RED) liczba_red++; // liczy ilosc liter ktorych gracz nie posiada w boxie, a wpisal z klawiatury
					if (czy_mozna_wstawic(plansza, show_word, 0, i, x, y)) licznik++; // liczba liter podswietlonych na zielono
				}
				for (int i = 0; i < p; i++) { // kolorowanie wyswietlanego napisu
					gotoxy(*x, *y + i);
					color(plansza, x, y, o, p, liczba_red, licznik, i, show_word);
				}
				break;
			}
		}
		else { // jesli wstawiany wraz wyszedlby poza plansze to nie pozwala wstawic, wyswietla komunikat mowiacy o niepowodzeniu wstawiania slowa
			komunikat(POZYCJA_PLANSZY_X, POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY + 4, blad);
			break;
		}

		cursors(zn, x, y, zero,o,p); // obsluga kursorów
	} while ((*zn)!= 0x0d); // enter

	char error[60] = { "Niedozwolona operacja " };
	char tekst[60] = { "Jestes w menu " }; // po kliknieci bedzie w menu, napisane po to zeby nie pogubic sie w ktorym miejscu w programie aktulanie jestem :)
	if (((*o == 0) && (*x + p <= POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY)) || // zeby nie wylasil slowem poza plansze
		((*o == 1) && (*y + p <= POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY))) {

		if (warunki(plansza, x, y, o, p, liczba_red, licznik) && czy_sasiad(plansza, o, &p, x, y) && liczba_red==0) put_on_board(plansza, show_word, x, y, o, p, my_box, box_copy);
		else komunikat(POZYCJA_PLANSZY_X, POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY + 4, error);
		if (p == PREMIA_S) *premia = true; // premia za wpisanie w jednym ruchu 7 liter
	}
	else komunikat(POZYCJA_PLANSZY_X, POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY + 4,tekst);
	
	p = 0;
	// freeing memory
	delete[] show_word;
	show_word = NULL;
	delete[] box_copy;
	box_copy = NULL;
}

void color(pole**plansza,int* x,int* y,int* o,int p,int liczba_red,int licznik, int i, LETTER *show_word){
	// kolorowanie slowa - graficzna (wizualna podpowiedz) czy mozna umiescici slowo w danym miejscu
	if (warunki(plansza, x, y, o, p, liczba_red, licznik) && czy_sasiad(plansza, o, &p, x, y) && licznik==p) textcolor(GREEN);
	else textcolor(RED);

	putch(show_word[i].token);
	textcolor(WHITE);
}

bool warunki(pole **plansza, int *x, int * y, int * o, int p, int liczba_red, int licznik) {

	// Does centre of board is empty? If it is then you cannot insert a word
	if (plansza[ROZMIAR_PLANSZY / 2][ROZMIAR_PLANSZY / 2].token == NULL) { // jesli plansza jest pusta (brak slowa na srodku) to sprawdza czy gracz "ma zamiar" wstawic slowo na srodku planszy
		if (*o == 0) { // poziomo
			if (*y == POZYCJA_PLANSZY_Y + (ROZMIAR_PLANSZY / 2) && // if player wants to insert word in the centre of board
			   (*x <= POZYCJA_PLANSZY_X + (ROZMIAR_PLANSZY / 2) && *x + p > POZYCJA_PLANSZY_X + (ROZMIAR_PLANSZY / 2))) {
				return true;
			}
			else  return false;
		}
		else if (*o == 1) { // pionowo
			if (*x == POZYCJA_PLANSZY_X + (ROZMIAR_PLANSZY / 2) && // if player wants to insert word in the centre of board
			   (*y <= POZYCJA_PLANSZY_Y + (ROZMIAR_PLANSZY / 2) && *y + p > POZYCJA_PLANSZY_Y + (ROZMIAR_PLANSZY / 2))) {
				return true;
			}
			else return false;
		}
	}
	// if centre of board is filled and player plases word that will connect with word that already is on the board
	else if (plansza[ROZMIAR_PLANSZY / 2][ROZMIAR_PLANSZY / 2].token != NULL) {
		if (licznik != p) { // jesli ilosc liter podswietlonych na zielono nie jest równa dlugosci wpisywanego slowa, to nie mozna wpisac slowa
			return false;
		}
		else return true;
	}
}

void orientation(int *zn, int *x, int *y, int *o, int p) {
	char tekst[60] = { "You cant change the orientation " };
	// zmiana orinetacji
	if ((*zn == 'o' || *zn == 'O')) {
		if ( ((*o == 1) && (*x + p - 1 < POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY)) || // jesli zmienia z pionu na poziom 
			 ((*o == 0) && (*y + p - 1 < POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY)) ) // jesli zmienia z poziomu na pion
			(*o)++;
		else komunikat(POZYCJA_PLANSZY_X, POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY + 4, tekst);
		if (*o == 2) (*o) = 0; // po inkremantacji z 1 (z pionu) na 2 wraca do 0 (poziomu)
	}
}
 
void komunikat(int x, int y, char *tekst) { // wyswietla dany komunikat
	gotoxy(x, y);
	textcolor(RED);
	cputs(tekst);
	textcolor(WHITE);
	gotoxy(1, 27);
	system("PAUSE");
}

bool czy_sasiad(pole** plansza, int *o, int * p, int *x, int *y) {
	// sprawdzanie czy do okola wpisywanego slowa sa jakies litery, warunek na to zeby slowa sie laczyly
	if (plansza[ROZMIAR_PLANSZY / 2][ROZMIAR_PLANSZY / 2].token == NULL) return true;
	else {
		if (*o == 0) { // poziom
			if (*x > POZYCJA_PLANSZY_X && *x + *p <= POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY) { // poczatek i koniec wyrazu
				if (plansza[*x - POZYCJA_PLANSZY_X - 1][*y - POZYCJA_PLANSZY_Y].token != NULL) return true;
				if (plansza[*x - POZYCJA_PLANSZY_X + *p][*y - POZYCJA_PLANSZY_Y].token != NULL) return true;
			}
			if (*x >= POZYCJA_PLANSZY_X && *x + *p <= POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY && *y == POZYCJA_PLANSZY_Y) { // gora
				for (int i = 0; i < *p && *x + i <= POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY; i++) {
					if (plansza[*x - POZYCJA_PLANSZY_X + i][*y - POZYCJA_PLANSZY_Y + 1].token != NULL) return true;
				}
			}
			if (*x >= POZYCJA_PLANSZY_X && *x + *p <= POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY && *y == POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY-1) {// do³
				for (int i = 0; i < *p && *x + i <= POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY; i++) {
					if (plansza[*x - POZYCJA_PLANSZY_X + i][*y - POZYCJA_PLANSZY_Y - 1].token != NULL) return true;
				}
			}
			if(*y > POZYCJA_PLANSZY_Y && *y< POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY-1 &&
			   *x > POZYCJA_PLANSZY_X && *x + *p < POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY) { // szeroko pojety srodek planszy xd
				for (int i = 0; i < *p && *x + i <= POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY; i++) {
					if (plansza[*x - POZYCJA_PLANSZY_X + i][*y - POZYCJA_PLANSZY_Y - 1].token != NULL) return true;
					if (plansza[*x - POZYCJA_PLANSZY_X + i][*y - POZYCJA_PLANSZY_Y + 1].token != NULL) return true;
				}
			}
		}
		if (*o == 1) { // pion
			if (*y > POZYCJA_PLANSZY_Y && *y + *p <= POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY) { // poczatek i koniec wyrazu
				if (plansza[*x - POZYCJA_PLANSZY_X][*y - POZYCJA_PLANSZY_Y - 1].token != NULL) return true;
				if (plansza[*x - POZYCJA_PLANSZY_X][*y - POZYCJA_PLANSZY_Y + *p].token != NULL) return true;
			}
			if (*y >= POZYCJA_PLANSZY_Y && *y + *p <= POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY && *x == POZYCJA_PLANSZY_X) {// prawo
				for (int i = 0; i < *p && *y + i <= POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY; i++) {
					if (plansza[*x - POZYCJA_PLANSZY_X + 1][*y - POZYCJA_PLANSZY_Y + i].token != NULL) return true;
				}
			}
			if (*y >= POZYCJA_PLANSZY_Y && *y + *p <= POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY && *x == POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY-1) { // lewo
				for (int i = 0; i < *p && *y + i <= POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY; i++) {
					if (plansza[*x - POZYCJA_PLANSZY_X -1][*y - POZYCJA_PLANSZY_Y +i].token != NULL) return true;
				}
			}
			if (*x > POZYCJA_PLANSZY_X && *x<POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY-1 &&
				*y >POZYCJA_PLANSZY_Y && *y + *p < POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY) { // srodek
				for (int i = 0; i < *p && *y + i <= POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY; i++) {
					if (plansza[*x - POZYCJA_PLANSZY_X - 1][*y - POZYCJA_PLANSZY_Y + i].token != NULL) return true;
					if (plansza[*x - POZYCJA_PLANSZY_X + 1][*y - POZYCJA_PLANSZY_Y + i].token != NULL) return true;
				}
			}
		}
		return false;
	}
}

void put_on_board(pole **plansza, LETTER *show_word,int *x, int *y, int *o, int p, LETTER *my_box, LETTER *box_copy) {
	// wstawia slowo na plansze
	switch (*o) {
	case 0: { // poziomo
			for (int i = 0; i < p; i++) {
				if (czy_mozna_wstawic(plansza, show_word, i, 0, x, y)) {
					plansza[*x - POZYCJA_PLANSZY_X + i][*y - POZYCJA_PLANSZY_Y].token = show_word[i].token;
					if (show_word[i].points != 0)
						plansza[*x - POZYCJA_PLANSZY_X + i][*y - POZYCJA_PLANSZY_Y].points = show_word[i].points;
				}
			}
			zeruj_tablice(show_word, p);
		break;
	}
	case 1: { // pionowo
			for (int i = 0; i < p; i++) {
				if (czy_mozna_wstawic(plansza, show_word, 0, i, x, y)) {
					plansza[*x - POZYCJA_PLANSZY_X][*y - POZYCJA_PLANSZY_Y + i].token = show_word[i].token;
					if (show_word[i].points != 0)
						plansza[*x - POZYCJA_PLANSZY_X][*y - POZYCJA_PLANSZY_Y + i].points = show_word[i].points;
				}
			}
			zeruj_tablice(show_word, p);
		break;
	}

	}
	// po wpisaniu liter na plansze mozna usunac wstawione litery z pojemnika z literami gracza (nadpisaæ tablice)
	for (int i = 0; i < CAPACITY_OF_BOX; i++)
		my_box[i] = box_copy[i];
}

int punktacja(pole ** plansza, bool* premia, int*licznik_slowa) { // zliczanie lunktow
	int score = 0;
	int bonus = 0;
	int bonus2 = 0;
	for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
		for (int j = 0; j < ROZMIAR_PLANSZY; j++) {
			if (plansza[i][j].WS == NULL) { // jesil nie ma bonusu za slowo to licz normalnie
				score += plansza[i][j].LS*plansza[i][j].points;
			}
			else if(plansza[i][j].token!=NULL && plansza[i][j].WS!=0) { //jesli jest bonus za slowo i w tym polu jest litera
				int x_licz = 0;
				for (int k = 0; k < ROZMIAR_PLANSZY; k++) {// poziomo
					bonus += plansza[k][j].LS*plansza[k][j].points;
					if (plansza[k][j].WS != 0) x_licz++;
				}
				score+=bonus*plansza[i][j].WS*x_licz;
				int y_licz = 0;
				for (int k = 0; k < ROZMIAR_PLANSZY; k++) { // pionowo
					bonus += plansza[i][k].LS*plansza[i][k].points;
					if (plansza[i][k].WS != 0) y_licz++;
				}
				score += bonus2 * plansza[i][j].WS*y_licz;
			}
		}
	}
	score += *licznik_slowa; // jak wpisane slowo mialo 7 liter to dodaje 50 pkt do odpowiedniej zmiennej i podaczas kolejnego ilczenia punktow dodaje te punkty (pamieta o nich)
	if (*premia) { // premia za wpisanie 7 liter
		 *licznik_slowa += PREMIA_P;
		score += PREMIA_P;
		*premia = false;
	}
	return score;
}

bool czy_mozna_wstawic(pole **plansza, LETTER*show_word, int i, int j, int *x,int *y) { // sprawdza czy pole jest wolne
	int poziomo=i;
	int pionowo=j;

	if (i == 0) i = j; //pionowo (zrobione po to, zeby mozna by³o chodzic po tablicy zmienna 'i')

	// jesli litera na planszy jest taka sama jak litera w slowie i gracz nie posiada tej litery (mozna wpisac)
	if (plansza[*x - POZYCJA_PLANSZY_X + poziomo][*y - POZYCJA_PLANSZY_Y+pionowo].token == show_word[i].token && show_word[i].color == RED) return true;
	// Jesli pole na planszy jest puste i gracz posiada ta litere (mozna wpisac)
	else if (plansza[*x - POZYCJA_PLANSZY_X + poziomo][*y - POZYCJA_PLANSZY_Y+pionowo].token == NULL && show_word[i].color == GREEN) return true;
	// w innym przypadku nie mozna wstawiæ litery 
	else return false;    
};

void RYSUJ(int *zn, int *x, int *y, int *attr, int *zero, int *back,LETTER *my_box,LETTER *letters,pole** plansza, int *count) {

	Menu(*zn, *x, *y, *attr, *zero, *back);
	
	Rysuj_plansze(plansza);

	show_box(letters, ILOSC_LITER, 20);

	show_box(my_box, CAPACITY_OF_BOX, 23);
	
	char tekst[60] = { "Nie masz wiecej liter " };

	if (*count == 0) komunikat(POZYCJA_PLANSZY_X, POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY + 4, tekst);

		/*gotoxy(POZYCJA_LEGENDY_X+16, POZYCJA_LEGENDY_Y + 19);
		cout << *count;*/

		/*int p = 0;
			for (int i = 0; i < ILOSC_LITER; i++) {


				gotoxy(POZYCJA_LEGENDY_X + p, POZYCJA_LEGENDY_Y + 21);
				putch(letters[i].points+48);
				textcolor(WHITE);
				p += 2;
			}
*/
	//gotoxy(POZYCJA_PLANSZY_X+2, POZYCJA_LEGENDY_Y + ROZMIAR_PLANSZY + 3);
	//cout << "x = " << *x << " y = " << *y;
	//
	

	// wywietlanie kursora
	gotoxy(*x, *y);
	textcolor(*attr);
	textbackground(*back);
	putch('+');
}

void cursors(int *zn, int *x, int *y, int *zero,int *o, int p) {
	*zero = 0;
	*zn = getch();

	if (*zn == 0) {

		*zero = 1;		// if this is the case then we read
		*zn = getch();		// the next code knowing that this // will be a special key
		
		if (*zn == 0x48 && *y > POZYCJA_PLANSZY_Y) (*y)--; // up
		if (*zn == 0x4b && *x > POZYCJA_PLANSZY_X) (*x)--; // left
		
		// zeby podczas umieszczanie wpisanego slowa (zmianie orientacji) nie mozna bylo wyjsc slowem poza plansze
		if (*o == 1) { // Pionowo
			if (*zn == 0x50 && (*y < POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY - 1) && (*y+1+p<=POZYCJA_PLANSZY_Y+ROZMIAR_PLANSZY)) (*y)++; // down 
			if (*zn == 0x4d && (*x < POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY - 1)) (*x)++; // right 
		}
		else { // poziomo
			if (*zn == 0x4d && (*x < POZYCJA_PLANSZY_X + ROZMIAR_PLANSZY - 1) && (*x+1+p<=POZYCJA_PLANSZY_X+ROZMIAR_PLANSZY)) (*x)++; // right 
			if (*zn == 0x50 && (*y < POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY - 1)) (*y)++; // down 
		}
	}
}

void Rysuj_plansze(pole **plansza) {
	//rysuje ramke
	for (int i = -1; i < ROZMIAR_PLANSZY + 1; i++) {
		gotoxy(POZYCJA_PLANSZY_X + i, POZYCJA_PLANSZY_Y);
		for (int j = -1; j < ROZMIAR_PLANSZY + 1; j++) {
			gotoxy(POZYCJA_PLANSZY_X + i, POZYCJA_PLANSZY_Y + j);
			if (i == -1 || j == -1 || i == ROZMIAR_PLANSZY || j == ROZMIAR_PLANSZY) {
				putch('*');
			}
		}
	}

	// plansza[ oœ X][ oœ Y].token = '#';
	// i = x, j = y

	// rysuje plansze razem z bonusami
	for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
		gotoxy(POZYCJA_PLANSZY_X + i, POZYCJA_PLANSZY_Y);
		for (int j = 0; j < ROZMIAR_PLANSZY; j++) {
			gotoxy(POZYCJA_PLANSZY_X + i, POZYCJA_PLANSZY_Y + j);

			if (i == (ROZMIAR_PLANSZY / 2) && i == j && plansza[i][j].token == NULL) {
				textbackground(BROWN); // mark centre of board if there is not any letter
				//plansza[i][j].token = '.';
			}
			else if (plansza[i][j].LS == 2) { // Duzy "X" na planszy
				//textcolor(LIGHTRED);
				textbackground(LIGHTRED);
			}
			else textbackground(BLACK);
			
			if (plansza[i][j].WS != NULL) {
				textbackground(RED);
			}
			putch(plansza[i][j].token);
			textcolor(WHITE);
		}
	}


	// punkty za litery wpisane na planszy

	//for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
	//	gotoxy(POZYCJA_PLANSZY_X + 17 + i, POZYCJA_PLANSZY_Y);
	//	for (int j = 0; j < ROZMIAR_PLANSZY; j++) {
	//		gotoxy(POZYCJA_PLANSZY_X + 17 + i, POZYCJA_PLANSZY_Y + j);
	//		/*if (plansza[i][j].points != 0) {
	//			putch(plansza[i][j].points + 48);
	//			textcolor(WHITE);
	//		}*/
	//	//	if (plansza[i][j].WS != NULL) {
	//			putch(plansza[i][j].points+48);
	//		//}
	//	}
	//}
}

void new_game(LETTER *letters, LETTER *my_box, pole **plansza, int*score) {

	zeruj_tablice(letters, ILOSC_LITER);
	zeruj_tablice(my_box, CAPACITY_OF_BOX);
	zeruj_tablice(plansza, ROZMIAR_PLANSZY);
	
	box_of_letters(letters);
	(*score) = 0;

	// bonus za litere
	for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
		for (int j = 0; j < ROZMIAR_PLANSZY; j++) {
			if (i == j || i == ROZMIAR_PLANSZY - j -1) {
				plansza[i][j].LS = 2;
			}
		}
	}
	// bonus za slowo
	plansza[2][5].WS = WSBON;
	plansza[2][9].WS = WSBON;
	plansza[5][2].WS = WSBON;
	plansza[5][12].WS = WSBON;
	plansza[9][2].WS = WSBON;
	plansza[9][12].WS = WSBON;
	plansza[12][5].WS = WSBON;
	plansza[12][9].WS = WSBON;
};

void show_pts(int score) {
	char tekst[60] = {"You are over 10 000, you master of scrabble! "};
	gotoxy(POZYCJA_LEGENDY_X + 7, POZYCJA_LEGENDY_Y + 17);
	
	//sposob wyswietlania punktow 
	if (score < 10 ) {// mniejsze niz 10
		putch(score + 48);
	}
	else if (score > 9 && score < 100) { // pomiedzy 10, a 100
		int dziesiatki;
		dziesiatki = ((score - (score % 10)) / 10);
		putch(dziesiatki + 48); // wyswietla dziesiatki
		gotoxy(POZYCJA_LEGENDY_X + 8, POZYCJA_LEGENDY_Y + 17);
		putch((score % 10) + 48); //wyswietla jednosci;
	}
	else if (score > 99 && score <1000) {// powmiedzy 100, a 1000
		int setki, dziesiatki;
		setki = ((score - (score % 100)) / 100);
		putch(setki + 48);
		gotoxy(POZYCJA_LEGENDY_X + 8, POZYCJA_LEGENDY_Y + 17);
		dziesiatki = score % 100;
		dziesiatki = ((dziesiatki - (dziesiatki % 10)) / 10);
		putch(dziesiatki + 48);
		gotoxy(POZYCJA_LEGENDY_X + 9, POZYCJA_LEGENDY_Y + 17);
		putch((score % 10) + 48);
	}
	else if (score > 999 && score <10000) {// pomiedzy 1000, a 10 000
		int tysiace, setki, dziesiatki;
		tysiace = ((score - (score % 1000)) / 1000);
		putch(tysiace + 48);
		gotoxy(POZYCJA_LEGENDY_X + 8, POZYCJA_LEGENDY_Y + 17);
		setki = score % 1000;
		setki = ((setki - (setki % 100)) / 100);
		putch(setki + 48);
		gotoxy(POZYCJA_LEGENDY_X + 9, POZYCJA_LEGENDY_Y + 17);
		dziesiatki = score % 100;
		dziesiatki = ((dziesiatki - (dziesiatki % 10)) / 10);
		putch(dziesiatki + 48);
		gotoxy(POZYCJA_LEGENDY_X + 10, POZYCJA_LEGENDY_Y + 17);
		putch((score % 10) + 48);
		//komunikat(POZYCJA_PLANSZY_X, POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY + 4, tekst);
	}
	else if(score>9999) komunikat(POZYCJA_PLANSZY_X, POZYCJA_PLANSZY_Y + ROZMIAR_PLANSZY + 4, tekst);
}
