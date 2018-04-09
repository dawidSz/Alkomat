// POP 2016-12-05 projekt 1 Sza³wiñski Dawid AIR 3 165532 Œrodowisko: Dev-C++ 5.11, kompilator: TDM-GCC 4.9.2 64-bit Release

#include<iostream>
#include<iomanip>

using namespace std;

const int max_d = 40;
const int max_z = 1000;

struct dawka{
	
	string rodzaj;
	int ilosc_napoju;
	int godzina;
	float ilosc_alkoholu;
	
};

void wypisz_menu();
float wczytaj_plec();
int wczytaj_wage();
void wczytaj_dawki(dawka[], int&, float&);
int oblicz_czas(dawka[], float, float [3][max_z], int, int&);
float na_promile(float, int, float);
void pisz_tabele(float, int, int, float);
float szukaj_maks(float [3][max_z], int);
void konwertuj(float);
void rysuj_wykres(int, float [3][max_z], float);

int main(){
	
	wypisz_menu();
	cout << "Podaj charakteryzacje osoby pijacej oraz kolejne dawki alkoholu: " << endl << endl;
	
	float wsp_korekcji = wczytaj_plec(), il_alko = 0;
	int waga = wczytaj_wage(), il_dawek = 0;
	
	dawka dawki[max_d];
	wczytaj_dawki(dawki, il_dawek, il_alko);
	
	float tab[3][max_z];
	for(int i=0; i<max_z; i++) tab[2][i]=0;
	int czas_promile = 0;
	int czas = (oblicz_czas(dawki, wsp_korekcji, tab, waga, czas_promile))*5;
	
	system("cls");
	
	float maks_st = szukaj_maks(tab, czas);
	
	pisz_tabele(il_alko, czas_promile, czas, maks_st);
	rysuj_wykres(czas, tab, maks_st);

	system("pause");
	return 0;
}

void wypisz_menu(){
	
	cout << "ALKOMAT" << endl << "Witaj w programie obliczajacym czas, w ktorym alkohol calkowicie usunie sie z Twojego organizmu." << endl;
	cout << "Charakteryzacje osoby spozywajacej uzywke podaj w nastepujacy sposob:" << endl;
	cout << "'Mezczyzna: waga 80, piwo 500 g:2215, wodka 100 g:2245, wino 200 g:2300.'" << "<--- WAZNA KROPKA NA KONCU" << endl;
	cout << "Oznacza to, ze: mezczyzna o wadze 80kg o godz. 22:15 wypil 0.5l piwa, o 22:45 - 0.1l wodki oraz o 23:00 - 0.2l wina." << endl;
	cout << "Dane moga byc wprowadzane tylko i wylacznie w ten sposob, a dokladnosc czasu to 5 minut (np. 22:05). Inaczej program nie spelni swojej funkcji." << endl;
	cout << "Zawartosc alkoholu w trunkach: " << endl;
	cout << "Piwo - ok. 5%" << endl << "Wino - ok. 20%" << endl << "Wodka - ok. 40%" << endl << endl;
	
	system("pause");
	system("cls");
	
}

float wczytaj_plec(){
	
	string plec;
	
	cin >> plec;
	
	if(plec == "Kobieta:") return 0.6;
	else if(plec == "Mezczyzna:") return 0.7;
	else cout << "Podano bledne dane. Uruchom ponownie program i wpisz dane jeszcze raz...";
	
}

int wczytaj_wage(){
	
	string wolny_wyraz;
	cin >> wolny_wyraz;
	
	int waga;
	if(wolny_wyraz=="waga") {
		cin >> waga;
		return waga;
	}
	else cout << "Podano bledne dane. Uruchom ponownie program i wpisz dane jeszcze raz...";
		
}

void wczytaj_dawki(dawka dawki[], int& il_dawek, float& il_alko){
		
	string przecinek=",";
	char wyr_godzina[max_d];
	
	for(int i=0; przecinek==","; i++){
		cin >> przecinek;
		if(przecinek==","){
			cin >> dawki[i].rodzaj;
			cin >> dawki[i].ilosc_napoju;
			cin.getline(wyr_godzina, 3, ':');
			cin >> dawki[i].godzina;
			il_dawek++;
		}
		else break;
	}
	
	for(int i=0; i<il_dawek; i++){
		if(dawki[i].rodzaj=="piwo")		dawki[i].ilosc_alkoholu = (dawki[i].ilosc_napoju)*0.05*0.79;
		if(dawki[i].rodzaj=="wodka")	dawki[i].ilosc_alkoholu = (dawki[i].ilosc_napoju)*0.4*0.79;
		if(dawki[i].rodzaj=="wino")		dawki[i].ilosc_alkoholu = (dawki[i].ilosc_napoju)*0.2*0.79;
		il_alko += dawki[i].ilosc_alkoholu;
	}
	
	
}

int oblicz_czas(dawka dawki[], float wsp_korekcji, float tab[3][max_z], int waga, int& czas_promile){
	
	int iloraz;
	tab[0][0] = dawki[0].godzina;
	
	for(int i=1; i<max_z; i++){
		iloraz = tab[0][i-1]/100;
		if(!(tab[0][i-1] == 2355)){
			if((tab[0][i-1] + 5) == ((iloraz * 100) + 60)) tab[0][i] = (iloraz * 100) + 100;
			else tab[0][i] = tab[0][i-1] + 5;
		} 
		else tab[0][i] = 0;
	}
	
	int ind_dawek=0, licznik = 1;
	tab[1][0] = 0;
	
	for(int i=0; i<max_z; i++){
		if(dawki[ind_dawek].godzina == tab[0][i]) {
			tab[1][i] += dawki[ind_dawek].ilosc_alkoholu;
			ind_dawek++;
		}
		if(wsp_korekcji > 0.65) tab[1][i+1] = tab[1][i] - (10./12.);
		if(wsp_korekcji < 0.65) tab[1][i+1] = tab[1][i] - (8./12.);
		if(tab[1][i+1] <= 0) {
			tab[1][i+1] = 0;
			break;
		}
		licznik++;
	}
	
	for(int i=0; i<max_z; i++){
		tab[2][i] = na_promile(wsp_korekcji, waga, tab[1][i]);
		if(tab[2][i] > 0.2) czas_promile++;
		if(tab[2][i] == 0) break;
	}
	
	return licznik;
}

float na_promile(float wsp_korekcji, int waga, float do_zamiany){
	
	return do_zamiany / (waga*wsp_korekcji);
	
}

void pisz_tabele(float il_alko, int czas_promile, int czas, float maks_st){
	
	cout << " il. alkoholu(gramy) | stezenie maks(promile) | czas(stezenie < 0.2 promila) | czas do uzyskania trzezwosci" << endl;
	cout << setw(21) << il_alko << "|" << setw(24) << maks_st << "|" << setw(22) << (czas_promile*5)/60. << " godziny" << "|" << setw(21) << czas/60. << " godziny" << endl;
	cout << endl << endl;
	
}

float szukaj_maks(float tab[3][max_z], int czas){
	
	float maks = tab[2][0];
	
	for (int i=0; i<((czas/5)-1); i++){
		if(tab[2][i+1] > tab[2][i]) maks = tab[2][i+1];
	} 
	return maks;
}

void konwertuj(float a){
	
	int l1 = a/1000;
	a = a - l1*1000; 
	
	int l2 = a/100;
	a = a - l2*100;
	
	int l3 = a/10;
	a = a - l3*10;
	
	cout << l1 << l2 << ":" << l3 << a << "|";
}

void rysuj_wykres(int czas, float tab[3][max_z], float maks_st){
	
	int x;
	if(czas < 250) x=1;
	if(czas >= 250 and czas < 500) x=2;
	if(czas >= 500 and czas < 750) x=3;
	if(czas >= 750 and czas < 1000) x=4;
	if(czas >= 1000 and czas < 1250) x=5;
	if(czas >= 1250 and czas < 1500) x=6;
	if(czas >= 1500 and czas < 1750) x=7;
	if(czas >= 1750 and czas < 2000) x=8;
	if(czas >= 2000 and czas < 2250) x=9;
	if(czas >= 2250 and czas < 2500) x=10;
	if(czas >= 2500 and czas < 2750) x=11;
	if(czas >= 2750 and czas < 3000) x=12;
	if(czas >= 3000 and czas < 3250) x=13;
	if(czas >= 3250 and czas < 3500) x=14;
	if(czas >= 3500 and czas < 3750) x=15;
	if(czas >= 3750 and czas < 4000) x=16;
	if(czas >= 4000 and czas < 4250) x=17;
	if(czas >= 4250 and czas < 4500) x=18;
	
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << setw(7) << 0 << setw(37) << maks_st/2. << setw(36) << maks_st << endl;    
	
	float skok = maks_st/73.; 

	for(int j=0; j<50; j++){
		konwertuj(tab[0][j*x]);
			if(!(tab[2][j*x]==0)) for(float i=0; i<tab[2][j*x]; i+=skok) cout << "-";
			else {
				cout << endl;
				break;
			}
		cout << endl;
	} 
	
	cout << setw(7) << 0 << setw(37) << maks_st/2. << setw(36) << maks_st << endl;
	cout << "--------------------------------------------------------------------------------" << endl; 
	
}

