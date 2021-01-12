// Viterbi.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
// Autorzy: Jakub Maciesza, Marta Matusiak vel Matuszewska

#include "pch.h"
#include <iostream>
#include <fstream>
#define PI 3.141592654
using namespace std;
// funkcje od Agi
float gauss(float mean, float sigma) {
	double x;
	double z;

	z = (double)rand() / RAND_MAX;
	if (z == 1.0) z = 0.9999999;
	x = sigma * sqrt(2.0 * log(1.0 / (1.0 - z)));

	z = (double)rand() / RAND_MAX;
	if (z == 1.0) z = 0.9999999;
	return((float)(mean + x * cos(2 * PI * z)));
}

void kanal(float es_n0, long dl_kan, int *wej, float *wyj) {
	float mean = 0;
	float es = 1;
	float sygnal;
	float sigma;
	float s_n;
	long y;

	s_n = (float)pow(10, (es_n0 / 10));
	sigma = (float)sqrt(es / (2 * s_n));

	for (y = 0; y < dl_kan; y++) {
		sygnal = 2 * *(wej + y) - 1;
		*(wyj + y) = sygnal + gauss(mean, sigma);
	}
}

// generowanie wejściowego wektora losowego
int generujWektor(int *wektor, int n) {
	for (int i = 0; i < n; i++) {
		wektor[i] = rand() % 2;
	}
	return *wektor;
}
// działanie rejestru przesuwnego
// ---------------------------------
// | 1 | D | D2 | D3 | D4 | D5 | D6|
// ---------------------------------
void przesuwanieRejestru(int rejestr[7], int input, int G4, int G5, int G6) {
	G4 = (input + rejestr[2] + rejestr[3] + rejestr[5] + rejestr[6]) % 2;
	G5 = (input + rejestr[1] + rejestr[4] + rejestr[6]) % 2;
	G6 = (input + rejestr[1] + rejestr[2] + rejestr[3] + rejestr[4] + rejestr[6]) % 2;

	for (int i = 6; i > 0; i--) {
		rejestr[i] = rejestr[i - 1];
	}
	rejestr[0] = input;
}

void koder(int *wektor, int n, int esn0)
{
	generujWektor(wektor, n);

	int dlugoscWektora = n + 7;

	int *wektor_wejsciowy = new int[dlugoscWektora];
	int *rejestr = new int[7];
	int *output = new int[3*dlugoscWektora];
	int G4 = 0, G5 = 0, G6 = 0;

	for (int i = 0; i < dlugoscWektora; i++)
	{
		wektor_wejsciowy[i] = 0;
	}

	for (int i = 0; i < n; i++)
	{
		wektor_wejsciowy[i] = wektor[i];
	}

	for (int i = 0; i < dlugoscWektora; i++)
	{
		int input = wektor_wejsciowy[i];
		przesuwanieRejestru(rejestr, input, G4, G5, G6);
		output[i] = G4;
		output[dlugoscWektora + i] = G5;
		output[2*dlugoscWektora + i] = G6;
	}

	puszczeniePrzezKanal(output, esn0, n);
}

void puszczeniePrzezKanal(int *output, int esn0, int n)
{
	int dlugoscWektora = n + 7;
	float *outputPoKanale = new float[3*dlugoscWektora];

	kanal(esn0, 3 * dlugoscWektora, output, outputPoKanale);
}

void zapisDoPliku(double *w1, double *w2, double *w3, double dl) {
	ofstream plik;
	plik.open("wyniki.txt");

	plik << "Wartosci Es/n0:" << endl;
	for (int i = 0; i < dl; i++) {
		plik << w1[i] << " ";
	}
	plik << endl;
	plik << "Wartosci BER - kanal:" << endl;
	for (int i = 0; i < dl; i++) {
		plik << scientific << w2[i] << " ";
	}
	plik << endl;
	plik << "Wartosci BER - kodowanie + kanal:" << endl;
	for (int i = 0; i < dl; i++) {
		plik << scientific << w3[i] << " ";
	}
	plik << endl;
	plik.close();
}

int main()
{
	// zmienne
	float esn0;
	int wybor = 0;
	int ileBitow = 0;
	//menu
	cout << "podaj wartosc esn0:" << endl;
	cin >> esn0;
	cout << "Wybierz liczbe bitow w ramce(1,2,3):\n1. 100 \n2. 500 \n3. 1000" << endl;
	cin >> wybor;
	switch (wybor) {
	case 1: 
		ileBitow = 100;
		break;
	case 2:
		ileBitow = 500;
		break;
	case 3:
		ileBitow = 1000;
		break;
	}

	cout << "ilosc bitow: " << ileBitow;


}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
