# Specjalizowany czujnik temperaturowy:
## Mikroprocesorowy analizator cieplny

Zrobiłeś kawę albo herbatę i nie chcesz pilnować aż ostygnie? To coś dla Ciebie!

Kubkuino:  
	-.NET Framework 3.5  
	-Visual Studio 2017 Community
	-Ustawianie temperatur  
	-Tryb pomiaru  
	-Ustawianie materiału  
	-Ustawianie kształtu dna (płaskie/wklęsłe)   
	-Włączanie/wyłączanie alarmu  
	-Ustawienia modułu Bluetooth  
	-Zapis ostatnich ustawień  
	-Odczyt trybu pomiaru i materiału  
	-Sprawdzenie materiału i trybu pomiaru na starcie  
	-Ulubione ustawienie  
  
Do zrobienia:  
	-Alarm w pasku powiadomień,  
	-Skan i wybór urządzeń BT na starcie,  
	-Manual (nie, ReadMe się nie liczy),  
	-Ładniejszy interfejs,  
	-Naprawa wskaźników temperatury  
	-Zmiana nazwy na Kubkuino  
	-Podczerwony czujnik temperatury
	-Automatyczne tworzenie nowego pliku do zapisu danych
	-Migracja configa do JSONa  
	-Zrobienie jeszcze ładniejszego programu  
	-Rozróżnianie HELLO_STD od HELLO_IR (GY-906)
  
Tryby:  

	Auto - sam zmienia czujnik temperatury przy wklęsłym dnie i trybie innym niż aluminium  
	Manual - tylko DS18B20 (niezależnie od trybu). Przytrzymaj przycisk żeby wyjść.  
	Wymuś pomiar - poprawka dla dna płaskiego + mniej więcej temperatura dla wklęsłego  
	Pomiar dna - pomiar temperatury dna bez poprawek. Przytrzymaj przycisk żeby wyjść.  
  
Nawigacja:  
	1 - pomiar i granice  
	2 - ustawianie trybu (1 następny, dłuższy wybór)  
	3 - menu zasilania (1 nastepny, dłuższy wybór)  
	4 - tryb pomiaru  
Zakres pracy:  
	20-25 stopni celsjusza
	
Arduino:  
  
	Kod:  
		#define _NAME (String)"[NAZWA]" - Ustawia nazwę jako domyślną  
		#define _PASS (String)"[4 cyfry]" - Ustawia wpisane cyfry jako hasło do parowania  
		#define TONE [TON] - Dźwięk alarmu (uzupełnić na podstawie vars.h)  
		#define ALARM_TIME [milisekundy] - Ile milisekund Arduino ma piszczeć  
  
	Komendy:  
		X - Pierwszy argument  
		Y - Drugi argument  
  
		e; - Nawiązanie połączenia (zwraca "HELLO\r\n") MUSI ZOSTAĆ WYSŁANE  
		qquit; - Rozłączenie MUSI ZOSTAĆ WYSŁANE (specjanie takie długie - na wypadek błędu komunikacji,  
			który wysyła 'q' zamiast 'r'. Nie wiem czemu.)  
		CXX.X; - Ustawienie temperatury zimna  
		HXX.X; - Ustawienie temperatury ciepła  
			C?;/H?; - sprawdzenie temperatur  
		MX; - Ustawia materiał:  
			0 - ceramika  
			1 - aluminium  
			2 - szklanka  
			? - zwraca matariał (0-2)  
		S[komenda AT]; - Przesyła komendę AT do modułu Bluetooth. Zwraca odpowiedź modułu.  
		d; - Reset modułu Bluetooth, ustawienie domyślnej nazwy i hasła  
		r0; - Zwraca temperaturę czujnika dla bieżącego trybu  
		r1; - Zwraca poprawioną temperaturę dla bieżącego trybu  
		t; - Zwraca stan jako char (H - Hot, R - Ready, C - Cold)  
		DXY; - Moduł zarządzania komponentami:  
			0Y* - LCD (1 - włącz, 0 - wyłącz)  
			1Y* - Podswietlenie (1 - włącz, 0 - wyłącz)  
			2Y - BT (1 - włącz, 0 - wyłącz)  
			3Y* - LED (1 - włącz, 0 - wyłącz)  
			4Y* - Wyciszenie (1 - wyciszony, 0 - głośny)  
			5Y* - Rodzaj dna (1 - płaskie, 0 - wklęsłe)  
			6 - Status LCD, Podświetlenie, LED, Alarm, Dno (płaskie) (1 - włączony, 0 - wyłączony)  
			7Y - Tryb pomiaru:  
				0 - automatyczny wybór czujnika (wklęsłe dno gdy inny tryb niż aluminium)  
				1 - pomiar temperatury dna bez poprawki (wklęsłe dno, każdy tryb, temperatura na oko)  
				2 - tylko zewnętrzny czujnik (DS18B20)  
				3 - pomar temperatury dna  
				? - zwraca aktywny tryb (0-3)  
			8 - Przejście w tryb uśpienia  
  
		*Jak jako argument poda się znak zapytania (?) zostanie zwrócony stan pojedynczego modułu  
  
Każda komenda jest zakończona średnikiem ';'!  

Katalog:  
1. Aplikacja: Końcowa wersja aplikacji.  
2. Dokumentacje: Instrukcje obsługi i dokumentacja projektu  
3. libraries: Biblioteki potrzebne do skompilowania programu  
  * Adafruit_BMP085_Library: Biblioteka czujnika temperatury i ciśnienia BMP180.  
  * ClickButton: Sterowanie jednym przyciskiem.  
  * LiquidCrystal_I2C: Biblioteka do obsługi wyświetlacza.  
4. TermoparaDNO: Najbardziej wypasiona wersja programu  
5. Testy: Katalog z programami później dodanymi do głównego kodu 
  * Kubkuino: Druga wersja aplikacji na PC   
  * TestBluetooth: Test komend Bluetooth  
  * TestDokumentacje: Folder roboczy dokumentacji  
  * RoznicaPomiaru: Dane z konsoli dotyczące różnicy temperatur dna a kubka  
  * TestTermoparaV1: Pierwsza wersja aplikacji na PC  
6. ReadMe: To ja!
