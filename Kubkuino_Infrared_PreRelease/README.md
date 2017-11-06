# Specjalizowany czujnik temperaturowy:
## Mikroprocesorowy analizator cieplny

Wersja testowa. Będą zmiany w kodzie i hardware w miarę dostępności części.  

Zrobiłeś kawę albo herbatę i nie chcesz pilnować aż ostygnie? To coś dla Ciebie!
  
Do zrobienia:  
	-Ładniejszy interfejs,  
	-Naprawa wskaźników temperatury  
	-Rozróżnianie HELLO_STD od HELLO_IR (GY-906)  
	-Zmiana wyświetlacza na SSD1306 128x32 (i2c)  
	
Arduino:  
  
	Kod:  
		#define _NAME (String)"[NAZWA]" - Ustawia nazwę jako domyślną  
		#define _PASS (String)"[4 cyfry]" - Ustawia wpisane cyfry jako hasło do parowania  
		#define TONE [TON] - Dźwięk alarmu (uzupełnić na podstawie vars.h)  
		#define ALARM_TIME [milisekundy] - Ile milisekund Arduino ma piszczeć  
  
	Komendy:  
		X - Pierwszy argument  
		Y - Drugi argument  
  
		e; - Nawiązanie połączenia (zwraca "HELLO_IR\r\n") MUSI ZOSTAĆ WYSŁANE  
		qquit; - Rozłączenie MUSI ZOSTAĆ WYSŁANE (specjanie takie długie - na wypadek błędu komunikacji,  
			który wysyła 'q' zamiast 'r'. Nie wiem czemu.)  
		CXX.X; - Ustawienie temperatury zimna  
		HXX.X; - Ustawienie temperatury ciepła  
			C?;/H?; - sprawdzenie temperatur  
		S[komenda AT]; - Przesyła komendę AT do modułu Bluetooth. Zwraca odpowiedź modułu.  
		d; - Reset modułu Bluetooth, ustawienie domyślnej nazwy i hasła  
		r - Zwraca bieżący odczyt z czujnika  
		t; - Zwraca stan jako char (H - Hot, R - Ready, C - Cold)  
		DXY; - Moduł zarządzania komponentami:  
			0Y* - LCD (1 - włącz, 0 - wyłącz)  
			1Y* - Podswietlenie (1 - włącz, 0 - wyłącz)  
			2Y - BT (1 - włącz, 0 - wyłącz)  
			3Y* - LED (1 - włącz, 0 - wyłącz)  
			4Y* - Wyciszenie (1 - wyciszony, 0 - głośny)  
			6 - Status LCD, Podświetlenie, LED, Alarm (1 - włączony, 0 - wyłączony)  
			8 - Przejście w tryb uśpienia  
  
		*Jak jako argument poda się znak zapytania (?) zostanie zwrócony stan pojedynczego modułu  
  
Każda komenda jest zakończona średnikiem ';'!  
