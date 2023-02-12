// POP 2021-01-12 projekt 2 Rykowska Aleksandra EIT 6 184616, Dev-C++ 5.11, TDM-GCC 4.9.2 64-bit

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

void zapisz_histogram(int tab_ilosc_wystapien[50], string plik_zapis);
int wczytaj_losowania(string data_zakres_od, string data_zakres_do, int tab_ilosc_wystapien[], string & data_min, string & data_max);
int str_to_x_int(string losy, char separator, int nr_liczby);
void sortuj_tablice(int tab_ilosc_wystapien[], int tab1_sort[], int tab2_sort[]);
void wczytaj_zakres_dat(string & data_zakres_od, string & data_zakres_do);
int rysuj_menu(string data_min, string  data_max, string data_zakres_od, string data_zakres_do, int wszystkie_losowania, int ilosc_los_w_zakresie);
void pokaz_prawdopod(int tab_ilosc_wystapien[50]);
void pokaz_6max_min(int tab1_sort[50], int tab2_sort[50], string data_zakres_od, string data_zakres_do);

main ()
{
	int ret;
	int liczba = 0;
	int tab_ilosc_wystapien[50]; // 0, 1-49, w poz 0 trzymamy ilosc losowan
	int wszystkie_losowania = 0;
	int zakres_losowania = 0;
	int tab1_sort[50]; // ilosc wystapien posortowana
	int tab2_sort[50]; // cyfry posortowane wed³ug ilosci wystapien
	int wynik = 0;
	string plik_zapis;
	string data_min = "01.01.0001";
	string data_max = "31.12.9999";
	string data_zakres_od = "01.01.0001";
	string data_zakres_do = "31.12.9999";
	
	// zerujemy tablice
	for (int i=0; i< 50; i++)
		tab_ilosc_wystapien[i] = 0;
	for (int i=0; i< 50; i++)
		tab1_sort[i]= 0;
	for (int i=0; i< 50; i++)
		tab2_sort[i]= 0;
			
	wynik = wczytaj_losowania(data_zakres_od, data_zakres_do, tab_ilosc_wystapien, data_min, data_max);
	if (wynik == -1)
		{
			cout << "\nKoniec programu. Popraw plik wejsciowy!";
			return -1;
		}
	wszystkie_losowania = tab_ilosc_wystapien[0];
	
	
	// przykladowy zakres dat:
	data_zakres_od ="01.01.2001";
	data_zakres_do ="31.12.2002";	
	wynik = wczytaj_losowania(data_zakres_od, data_zakres_do, tab_ilosc_wystapien, data_min, data_max);
	zakres_losowania = tab_ilosc_wystapien[0];
	
	do
	{
		wynik = rysuj_menu(data_min, data_max, data_zakres_od, data_zakres_do, wszystkie_losowania, zakres_losowania);
		// w zaleznosci od zwroconej wartosci wywolaj inne funkcje
		
		switch (wynik)
		{
			case 0:
				wczytaj_zakres_dat(data_zakres_od, data_zakres_do);
				wczytaj_losowania(data_zakres_od, data_zakres_do, tab_ilosc_wystapien, data_min, data_max);
				zakres_losowania = tab_ilosc_wystapien[0];
				break;
				
			case 1:
				wczytaj_losowania(data_min, data_max, tab_ilosc_wystapien, data_min, data_max);
				plik_zapis = "histogram_caly.bmp"; 
				zapisz_histogram(tab_ilosc_wystapien,  plik_zapis);
				break;
				
			case 2: 
				wczytaj_losowania(data_zakres_od, data_zakres_do, tab_ilosc_wystapien, data_min, data_max);
				plik_zapis = "histogram_zakres.bmp"; 
				zapisz_histogram(tab_ilosc_wystapien,  plik_zapis);		
				break;
				
			case 3:
				wczytaj_losowania(data_min, data_max, tab_ilosc_wystapien, data_min, data_max);
				pokaz_prawdopod(tab_ilosc_wystapien);
				break;
				
			case 4:
				wczytaj_losowania(data_zakres_od, data_zakres_do, tab_ilosc_wystapien, data_min, data_max);
				pokaz_prawdopod(tab_ilosc_wystapien);
				break;
				
			case 5:
				wczytaj_losowania(data_zakres_od, data_zakres_do, tab_ilosc_wystapien, data_min, data_max);
				sortuj_tablice(tab_ilosc_wystapien, tab1_sort, tab2_sort);
				pokaz_6max_min(tab1_sort,tab2_sort,data_zakres_od, data_zakres_do);
				break;
				
			case 6:
				cout << "\n WYJSCIE\n";
				break;
		}
	} while (wynik != 6);

	return 0;
}

void zapisz_histogram(int tab_ilosc_wystapien[50], string plik_zapis)
{
	cout << "Zapis do pliku: (" << plik_zapis << ")\n";
	ofstream plik_wy(plik_zapis.c_str(), std::ofstream::binary);
	char* wsk_znak_b = new char[1];
	
	// naglowek 14 + 40 bajtów (zawsze taki sam dla obrazu 320x200 pikseli):
	*wsk_znak_b = 0x42;  // znak szesnastkowo na asci = "B"
	plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x4D;  // znak szesnastkowo na asci = "M"
	plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x36;
	plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0xEE;
	plik_wy.write (wsk_znak_b,1);	
	*wsk_znak_b = 0x02;
	plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x00;
	for (int i=0; i<5; i++)
		plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x36;
	plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x00;
	for (int i=0; i<3; i++)
		plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x28;
	plik_wy.write (wsk_znak_b,1);	
	*wsk_znak_b = 0x00;
	for (int i=0; i<3; i++)
		plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x40; // do 0x01 i 0x40 = 320 pikseli szerokoœæ
	plik_wy.write (wsk_znak_b,1);		
	*wsk_znak_b = 0x01; //
	plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x00;
	plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x00;
	plik_wy.write (wsk_znak_b,1);		
	*wsk_znak_b = 0xC8; // 0xC8 = 200 pikseli wysokoœæ
	plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x00;
	for (int i=0; i<3; i++)
		plik_wy.write (wsk_znak_b,1);	
	*wsk_znak_b = 0x01;
	plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x00;
	plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x18;
	plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x00;
	for (int i=0; i<6; i++)
		plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0xEE;
	plik_wy.write (wsk_znak_b,1);	
	*wsk_znak_b = 0x02;
	plik_wy.write (wsk_znak_b,1);
	*wsk_znak_b = 0x00;
	for (int i=0; i<17; i++)
		plik_wy.write (wsk_znak_b,1);
	// koniec naglowka pliku BMP i naglowka DIB (54 Bajty)
	
	/*
	od 36(h) do  2EE35(h) =  (54 - 192053 + 1) = 192000 bajtow = 320x200x3(3bajty na 1 pixel) ) 
	
	 wypisz 49 liczb z totolotka w 49 kolumnach
	320 pixeli / 49 = 6,53 pixeli na kolumne, czyli uzyjemy 5 pixeli, a 6-ty bêdzie granic¹
	*/

	// Skalowanie histogramu - Znajdz najwieksza wartosc i podziel przez 200 pix. 
	int max_wart=0;
	for (int i=1; i<49; i++)
		if (tab_ilosc_wystapien[i] > max_wart) // sprawdzamy dla kazdej liczby totolotka czy jest maksymalna
			max_wart = tab_ilosc_wystapien[i];
	float skala;
	skala = 200.0 / max_wart;  // np dla wartosci 400 , skala  = 0.5
	int tmp = 0;
		
	for (int i=0; i<200; i++) // 200 wierszy
	{
		for (int j=0; j<49; j++) // 49 kolumn x 6 (5+1) pixeli = 294 pixele; 
		{
			for (int k=0; k<5; k++) // 5 pixeli na kolumne x3 bajty  (jeden pixel zajmuje 3 bajty (bajt Blue, Green, Red))
			{
				tmp = tab_ilosc_wystapien[j+1];
				if ( (tmp*skala)>i ) // rysujemy slupki
				{
					*wsk_znak_b = 0x6F;  // kolor slupka
					plik_wy.write (wsk_znak_b,1); // B
					plik_wy.write (wsk_znak_b,1); // G
					plik_wy.write (wsk_znak_b,1); // R
				}
				else
				{
					*wsk_znak_b = 0x2F;  // kolor tla powyzej slupka
					plik_wy.write (wsk_znak_b,1); // B
					plik_wy.write (wsk_znak_b,1); // G
					plik_wy.write (wsk_znak_b,1); // R
				}
			}
			*wsk_znak_b = 0x2F;  // kolor granicy (1 pixel)
			plik_wy.write (wsk_znak_b,1); // B
			plik_wy.write (wsk_znak_b,1); // G
			plik_wy.write (wsk_znak_b,1); // R
		}
		for (int j=0; j<26; j++) // 320 - 294 = 26 pixeli pustych
			for (int k=0; k<3; k++) // 3 bajty na pixela
			{
				*wsk_znak_b = 0x2F;
				plik_wy.write (wsk_znak_b,1);
			}			
	}
	plik_wy.close();
	cout << "\nPlik histogram zostal zapisany na dysku\n";
	cout << "\nNacisnij Enter aby kontynuowac ";
	cin.ignore();
	cin.get();
}

int wczytaj_losowania(string data_zakres_od, string data_zakres_do, int tab_ilosc_wystapien[], string & data_min, string & data_max)
{ 
	// wczytujemy plik input.txt z losowania
	for (int i=0; i< 50; i++)
		tab_ilosc_wystapien[i] = 0; // zerujemy tablice
	string nr_losowania;
	string data_losowania;
	string losy; 
	int los_x = 0; // dla odczytania 6 wylosowanych liczb z jednego losowania
	int rr_min=9999;
	int mm_min=12;
	int dd_min=31;
	int rr_max =0; 
	int mm_max =0;
	int dd_max=0;
	int rr, mm, dd; // biezacy odczyt
	int rr_od, mm_od, dd_od;
	int rr_do, mm_do, dd_do;
	char separator = '.';
	int int_dzien_od;
	int int_dzien_do;
	int int_dzien;
	
	rr_od = str_to_x_int(data_zakres_od, separator, 3);
	mm_od = str_to_x_int(data_zakres_od, separator, 2);
	dd_od = str_to_x_int(data_zakres_od, separator, 1);
	int_dzien_od =rr_od*366 + mm_od*31 + dd_od; // dla ulatwienia przeliczam na numer dnia
	
	rr_do = str_to_x_int(data_zakres_do, separator, 3);
	mm_do = str_to_x_int(data_zakres_do, separator, 2);
	dd_do = str_to_x_int(data_zakres_do, separator, 1);
	int_dzien_do =rr_do*366 + mm_do*31 + dd_do;
	
	ifstream plik_wej("input.txt"); // tworzenie nowej zmiennej  i przypisanie jej do pliku wejsciowego
	
	if ( !plik_wej.good() )
	{
		cout << "Plik nie zostal znaleziony. Koniec programu.";
		exit (0);
	}
	else
	{
		cout << "Wczytywanie pliku... \n";
		while (!plik_wej.fail())
		{
			plik_wej >> nr_losowania;
			if ( plik_wej.fail() )
				break; // przerwij petle jezeli jest problem z odczytaniem danych w pliku
			plik_wej >> data_losowania;
			separator = '.';
			rr = str_to_x_int(data_losowania, separator, 3);
			mm = str_to_x_int(data_losowania, separator, 2);
			dd = str_to_x_int(data_losowania, separator, 1);
			// ustaw nowy dzien min:
			if ( rr < rr_min ) 
			{
				rr_min = rr;
				mm_min = mm;
				dd_min = dd;
			}
			if (rr == rr_min)
			{
				if (mm < mm_min)
				{
					mm_min = mm;
					dd_min = dd;
				}
				if (mm == mm_min)
				{
					if (dd < dd_min)
						dd_min = dd;
				}
			}
			// ustaw nowy dzien max:
			if ( rr > rr_max ) 
			{
				rr_max = rr;
				mm_max = mm;
				dd_max = dd;
			}
			if (rr == rr_max)
			{
				if (mm > mm_max)
				{
					mm_max = mm;
					dd_max = dd;
				}
				if (mm == mm_max)
				{
					if (dd > dd_max)
						dd_max = dd;
				}
			}			
			
			
			plik_wej >> losy; // string np: "2,5,13,24,32,49"
			
			// sprawdz czy jestesmy w interesujacym nas zakresie  "data od <> data do"
			// jesli tak to zapisujey dane do tablicy
			/*
						02-02-1999
						04-04-2001
			05-05-2001					data od ( zakres )
						06-06-2001
						(...)
						01-01-2002
			02-02-2002					data do (zakres)
						03-03-2002
						06-06-2003
			*/			
			
			int_dzien =rr*366 + mm*31 + dd; // numer biezacy dzien
			// interesuja nas tylko losy z danego zakresu dat
			if (  (int_dzien >= int_dzien_od)  and (int_dzien <= int_dzien_do) )
			{
				tab_ilosc_wystapien[0] = tab_ilosc_wystapien[0] + 1; // zliczamy ilosc losowan, dodajac 1
				for (int j=0 ; j<6; j++)    // dla kazdej z 6 wylosowanych liczb
				{
					separator = ',';
					los_x = str_to_x_int(losy, separator, j+1); // odczytaj kazda z szesciu wylosowanych liczb
					if (los_x == -1) 
					{
						// niepoprawna struktura danych, wypisz w jakiej lini problem
						cout << "\nProblem w lini z losowaniem nr: ";
						cout << nr_losowania;
						plik_wej.close();
						return -1;
					}
					tab_ilosc_wystapien[los_x] = tab_ilosc_wystapien[los_x] +1;	// dla wylosowanej zwiekszamy o 1 ilosc wystapien
				}
			}

		}
		plik_wej.close();
		cout << "Plik wczytany\n" << endl;
	}
	
	// funkcja bedzie zwracala 2 daty:
	//data_min
	//data_max
	
	// zamiana integera na stringa za pomoca strumienia: stringstream
	data_min = "";
	stringstream ss; // strumien
	if (dd_min < 10) // dodanie 0 dla liczb od 1 do 9
		data_min = data_min + "0";
	ss << dd_min;
	ss << ".";
	if (mm_min <10)
		ss << "0";
	ss << mm_min;
	ss << ".";
	ss << rr_min;
	data_min = data_min + ss.str(); // data minimum jest ustawiona i zwracana przez funkcje
	

	data_max = "";
	ss.str(""); // czyszczenie strumienia
	if (dd_max <10)
		data_max = data_max + "0";
	ss << dd_max;
	ss << ".";
	if (mm_max <10)
		ss << "0";
	ss << mm_max;
	ss << ".";
	ss << rr_max;
	data_max = data_max + ss.str();
}

// losy - losy lub data
int str_to_x_int(string losy, char separator, int nr_liczby)
{
	// zamieniamy napisy na liczby
	// string = 2,5,13,24,32,49  nr = 3; wynik = 13
	// string = 02.03.2020   nr =3  wynik = 2020
	char szukanyZnak = separator;
	int licznik = 0;
	string s = "";
	int los = 0;
	int ret = 0; // jesli -1 to problem z formatem danych wejsciowych
	int poprzedniaPozycja = -1;
	int znalezionaPozycja = losy.find( szukanyZnak );
	
	if( znalezionaPozycja == std::string::npos ) // nie znalazl separatora wiec cos nie tak poszlo 
    {
        cout << "Zly format danych w pliku wejsciowym !";
		cout << "\n";
        return -1;
    }

    do
    {
		s = ""; // zamiana stringa na integer za pomoca strumienia: stringstream
		s.insert (0,losy,poprzedniaPozycja+1,znalezionaPozycja-poprzedniaPozycja-1);
		// wybiera liczby ze stringa np. z losów "15,8,16,17,32,37" 
		// od poz 0
		// wez 2 znaki
		// s = "15"
		// dziala tez dla daty, np. "01.02.2001"
		// od poz 2
		// wez 2 znaki
		// s= "02"
		stringstream strumien(s); // wrzucamy string np "15" do strumienia
		strumien >> los;   // odczytujemy integer = 15 ze strumienia
		licznik ++;
		if (licznik == nr_liczby )
			ret = los;
		poprzedniaPozycja = znalezionaPozycja;
        znalezionaPozycja = losy.find( szukanyZnak, znalezionaPozycja + 1 );
    } while( znalezionaPozycja != std::string::npos );
    // ostatnia cyfra nie konczy sie separatorem
    // szukamy od poprzeniejPozycja do konca stringa
    s ="";
    int dl;
    dl = losy.length(); // dlugosc stringa
    s.insert (0,losy,poprzedniaPozycja+1,dl-poprzedniaPozycja);
   	stringstream strumien(s);
	strumien >> los;
	licznik ++;
	if (licznik == nr_liczby )
		ret = los;

	return ret;
}

void sortuj_tablice(int tab_ilosc_wystapien[], int tab1_sort[], int tab2_sort[])
{
	int max=0;
	int liczba =0;
	int nr_kol=1;
	
	tab1_sort[0] = tab_ilosc_wystapien[0];  // przepisanie wartosci ile bylo losowan
	
	// sortowanie od max do min wg ilosci wystapien
	for (int j=1; j<50; j++)
	{
		for (int i=1 ; i<50; i++)
		{
			if ( tab_ilosc_wystapien[i] >= max ) // znajdz max wartosc i zapamiataj
			{
				max = tab_ilosc_wystapien[i];
				liczba = i;
			}	
		}
		
		// znalazl max wiec zapisz do nowej tablicy 1 oraz 2
		tab1_sort[nr_kol] = max;
		tab2_sort[nr_kol] = liczba;
		tab_ilosc_wystapien[liczba] = 0; // skasuj ja, wyzeruj by kolejny raz ja pominal
		nr_kol ++;
		max = 0;
		liczba =0;
	}
	
}

void wczytaj_zakres_dat(string & data_zakres_od, string & data_zakres_do)
{
	// podaj date od i date do :
	cout << "Podaj date dla zakresu ktory chcesz zrobic analize w formacie: dd.mm.rrrr np: 01.01.2001 \nData od: ";
	cin >> data_zakres_od;
	cout << "Data do: ";
	cin >> data_zakres_do;
}

int rysuj_menu(string data_min, string  data_max, string data_zakres_od, string data_zakres_do, int wszystkie_losowania, int ilosc_los_w_zakresie)
{
	int ret = -1;
	do
	{
		system( "cls" );  // czyscimy ekran
		cout << "Analiza wynikow totolotka - Aleksandra Rykowska 2021\n\n";
		cout << "Plik wejsciowy zawiera " << wszystkie_losowania << " losowania z zakresu:  ";
		cout << data_min;
		cout << " - ";
		cout << data_max;
		cout << "\n";
		cout << "Ustawiony zakres dat przez uzytkownika zawiera " << ilosc_los_w_zakresie << " losowan od:  ";
		cout << data_zakres_od;
		cout << " - ";
		cout << data_zakres_do;
		cout << "\n\n";
		
		cout << "Menu:\n";
		cout << "(0) - zmien daty zakresu uzytkownia\n";
		cout << "(1) - utworz histogram.bmp z calosci\n";
		cout << "(2) - utworz histogram.bmp z zakresu dat\n";
		cout << "(3) - pokaz prawdopodobienstwo z calosci\n";
		cout << "(4) - pokaz prawdopodobienstwo z zakresu dat\n";
		cout << "(5) - pokaz 6 najczesciej i najrzadziej wystepujacych w zakresie dat\n";
		cout << "(6) - wyjscie\n" <<endl;
		cout << "Podaj 0-6: ";
		cin >> ret;
	} while ( !( (ret>(-1)) and (ret<7) ) );
	return ret;
}

void pokaz_prawdopod(int tab_ilosc_wystapien[50])
{
	//wypisz na ekranie wszystkie 49 wystapienia 
	//cout << "Ilosc losowan: ";
	//cout << tab_ilosc_wystapien[0];
	float prawdop = 0.1;
	cout << "\n";
	for (int i =1 ; i <50 ; i++)
	{
		prawdop = (float) tab_ilosc_wystapien[i] / tab_ilosc_wystapien[0] * 100;
		cout << "liczba: " << i << " ilosc wystapien: " << tab_ilosc_wystapien[i] << " prawdopodobienstwo: " << prawdop << "% \n";
	}
	
	cout << "\nNacisnij Enter aby kontynuowac ";
	cin.ignore();
	cin.get();
}

void pokaz_6max_min(int tab1_sort[50],int tab2_sort[50], string data_zakres_od, string data_zakres_do)
{
	float prawdop = 0.1;
	//wypisz na ekranie z tabeli posortowanej
	cout << "Dla zakresu dat: " << data_zakres_od << " - " << data_zakres_do;
	cout << "\nNajczesciej 6 wystepujacych: \n";
	for (int i =1 ; i < 7 ; i++)
	{
		prawdop = (float) tab1_sort[i] / tab1_sort[0] * 100;
		cout << "cyfra: " << tab2_sort[i]  << " ilosc wystapien: " << tab1_sort[i] << " Prawdop: " << prawdop << "% \n";
	}
	cout << "\nNajrzadziej 6 wystepujacych: \n";
	for (int i =44 ; i < 50 ; i++)
	{
		prawdop = (float) tab1_sort[i] / tab1_sort[0] * 100;
		cout << "cyfra: " << tab2_sort[i]  << " ilosc wystapien: " << tab1_sort[i] << " Prawdop: " << prawdop << "% \n";
	}
	
	/*
	 to samo zapisujemy do pliku na dysku
	*/
	cout << "\nWyniki zostana zapisane do pliku output.txt\n";
	
	ofstream output("output.txt");
	if(!output) 
	{
		cout << "Plik nie zostal otwarty\n";
		exit(0);
	}
	output << "Dla zakresu dat: " << data_zakres_od << " - " << data_zakres_do ;
	output << "\nNajczesciej 6 wystepujacych: \n";
	for (int i =1 ; i < 7 ; i++)
	{
		prawdop = (float) tab1_sort[i] / tab1_sort[0] * 100;
		output << "cyfra: " << tab2_sort[i]  << " ilosc wystapien: " << tab1_sort[i] << " Prawdop: " << prawdop << "% \n";
	}
	output << "\nNajrzadziej 6 wystepujacych: \n";
	for (int i =44 ; i < 50 ; i++)
	{
		prawdop = (float) tab1_sort[i] / tab1_sort[0] * 100;
		output << "cyfra: " << tab2_sort[i]  << " ilosc wystapien: " << tab1_sort[i] << " Prawdop: " << prawdop << "% \n";
	}	
	output.close();
	
	cout << "\nNacisnij Enter aby kontynuowac ";
	cin.ignore();
	cin.get();
}
