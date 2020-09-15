/* 
Projekt: Generator Labiryntu (Maze Generator)
Autorzy: Pawel Wasniowski
				 Jan Magiera
Opis:
Progam zostal napisany w oparciu o algorytm nazwany 'recursive backtracking', znany tez jako depth-first search.
Algorytm dziala wedlug schematu:
1. Funkcja przyjmuje jako parametr komórke labiryntu.
2. Ta komórka oznaczana jest jako odwiedzona (visited) i wrzucana na stos.
3. Jesli stos nie jest pusty, to:
	3.a Zrzuc te komórke ze stosu
	3.b Sprawdz czy sasiadujace komorki byly odwiedzone
	3.c Jesli jest dostepny nieodwiedzony sasiad, wrzuc te komórke na stos i wybierz losowo sasiada
	3.d Usun sciane pomiedzy komórka, a wybranym sasiadem i umiesc go na stosie.
	Lepsze wytlumaczenie algorytmu: https://www.youtube.com/watch?v=Y37-gB83HKE
Rysowanie labiryntu i gracza:
- Narysowanie pelnej siatki labiryntu
- Wygenerowanie przez inny watek wlasciwego labiryntu
- Usuniecie scian, w miejscach, w których jest przejscie
- Narysowanie celu i gracza
- Po kazdej aktualizacji pozycji gracza, jest on rysowany w nowym miejscu, a na poprzednim rysowany jest jego obraz
	o kolorze tla. W ten sposób unikamy odswiezania calego labiryntu, przy zmianie pozycji.
Przejscie labiryntu/reset:
- Labirynt mozna przejsc, przemieszczajac ikonke gracza, w miejsce ikony celu.
- Labirynt mozna zresetowac manualnie, za pomoca przycisku na plytce.
Po przejsciu labiryntu, losowany jest nowy.
Ze wzgledu na zastosowany generator liczb losowych, z biblioteki stdlib, losowana liczba pseudolosowa jest zalezna od momentu, w ktorym ta funkcja zostanie wywolana.
Oznacza to, ze przy kazdym starcie programu, pierwszy labirynt zawsze bedzie taki sam.
Kolejne plansze, ze wzgledu na roznice w czasie pomiedzy przycisnieciem guzika, badz przejsciem labiryntu, beda juz losowe i nie powinny sie powtorzyc (statystycznie).
Sterowanie:
- Przez interfejs UART, wyprowadzony na zlaczu USB, program odczytuje znaki. Znaki umowne przyjeto za: u (Up), d (Down), l (Left), r (Right) i n (New).
W naszym zamysle, do sterowania uzywamy akcelerometru na plytce KL25Z (opis w main.c zwiazanym z KL25Z). 
Komunikacja odbywa sie poprzez posrednika, w postaci komputera ze skryptem, napisanym w Pythonie, do przenoszenia danych z jednego UARTu, na drugi.
Zdecydowalismy sie zrobic to w ten sposób, poniewaz laczenie plytek bezposrednio USB, ze wzgledu na zasilanie, jest trudne, a przez goldpiny niewygodnie uzywa sie plytki z akcelerometrem.
Zyskujemy na tym mozliwosc sterowania bezposrednio przez UART komputera (np: przez PuTTy), badz inne urzadzenie, które jest w stanie wyslac na UART porzadane dane.
*/

#include "stm32f4xx_hal.h"
#include "stdlib.h"
#include "delay.h"
#include "usart.h"
#include "GUI.h"	
#include "cmsis_os2.h"

#define WIDTH  320 // szerokoœæ ekranu 
#define HEIGHT  240 // wysokoœæ ekranu
#define ROW  6 // iloœæ kolumn labiryntu 
#define COLLUMN  8 // iloœæ wierszy labiryntu
#define WW  40 // szerokosc pola. 320/8 v 240/6

/*
	struktura opisuj¹ca wszystkie czêœci labiryntu
	top, down, left, rignt => pola opisuj¹ce istnienie poszczególnych œcian danej czêœci
	v => (visited) indykator czy dana czêœæ zosta³a ju¿ odwiedzona przy generowaniu
*/
struct mazeTile {
	uint8_t top;
	uint8_t down;
	uint8_t left;
	uint8_t right;
	uint8_t visited;
} maze[COLLUMN][ROW];


/* 
	tablica w której zapamiêtywani s¹ dostêpni s¹siedzi podczas generowania
*/
struct adjacent {
	uint8_t x;
	uint8_t y;
 } adjacents[4];

 uint8_t stack[2000]; // tablica s³u¿¹ca jako stos
 int sp = 0; // stack pointer na tablice wy¿ej

 uint8_t av; // av (aviable) iloœæ dostêpnych s¹siadów
 
/*
	wspó³rzêdne gracza
*/
struct player {
	 uint8_t x;
	 uint8_t x_old;
	 uint8_t y;
	 uint8_t y_old;
 } Player;

 /*
	wspó³rzêdne celu
*/
 struct goal{
	 uint8_t x;
	 uint8_t y;
 } Goal;
 
/*----------------------------------------------------*/
//Semafory
osSemaphoreId_t display_id; 
osSemaphoreId_t generate_id;
osSemaphoreId_t player_id;
/*----------------------------------------------------*/



void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
 
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
 
  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
 
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
 
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}


/*
	Generowanie labiryntu poprzez przypisanie 0 lub 1 struktury maze, wpisanie 0 powoduje usuniecie sciany
	[x1, y1] - Wspólrzedne komórki na stosie
	[x2, y2] - Wspólrzedne komórki sasiadujacej

	vx - Wektor przesuniecia w poziomie
	vy - Wektor przesuniecia w pionie
*/
void drawPassage(int x1, int y1, int x2, int y2)
{
	int vx = x1 - x2;
	int vy = y1 - y2;
	
	if (vx == 1)
	{
		maze[x1][y1].left = 0;
		maze[x2][y2].right = 0;
	}
	
	if (vx == -1)
	{
		maze[x1][y1].right = 0;
		maze[x2][y2].left = 0;
	}
	
	if (vy == 1)
	{
		maze[x1][y1].top = 0;
		maze[x2][y2].down	= 0;
	}
	
	if (vy == -1)
	{
		maze[x1][y1].down = 0;
		maze[x2][y2].top = 0;
	}
}

/*
	Rekurencyjna funkcja generujaca labirynt
	x => pozycja horyzontalana
	y => pozycja wertykalna
	x,y => wspólrzêdna pocz¹tku algorytmu 

	 => (available) iloœæ dostêpnych s¹siadów
*/
void recGenerate(int x,int y)
{
	maze[x][y].visited = 1; // odwiedzony pocz¹tek labiryntu
	av = 0; // na pocz¹tku jest 0 s¹siadów
	do
	{
		// sprawdzenie nieodwiedzonych s¹siadów		
		if (x+1<COLLUMN && maze[x+1][y].visited == 0) 
		{
			adjacents[av].x = x+1;
			adjacents[av].y = y;
			av++;
		}
		
		if (x-1>=0 && maze[x-1][y].visited == 0) 
		{
			adjacents[av].x = x-1;
			adjacents[av].y = y;
			av++;
		}
		
		if (y-1>=0 && maze[x][y-1].visited == 0) 
		{
			adjacents[av].x = x;
			adjacents[av].y = y-1;
			av++;
		}
		
		if (y+1<ROW && maze[x][y+1].visited == 0) 
		{
			adjacents[av].x = x;
			adjacents[av].y = y+1;
			av++;
		}
		
		stack[sp]=av; // dodanie na stos dostepnych s¹siadów - potrzebne przy wracaniu do danej czêœci labiryntu
		if (av > 0) // jeœli jest chocia¿ 1 dostêpny s¹siad to zostaje wybrany losowy s¹siad ze wszystkich dostêpnych, a przed wejœciem do nowej czêœci zwiêkszamy stos natomiast po wyjœciu go zmniejszamy
		{
			int ran = rand() % av;
			drawPassage(x,y,adjacents[ran].x, adjacents[ran].y);
			sp++;
			recGenerate(adjacents[ran].x, adjacents[ran].y);
			sp--;
		}
		
	} while (stack[sp] > 0);
}

/*
	Sekwencja zapelniajaca labirynt 1, nastepnie wywoluje ona utworzenie nowego labiryntu i ustawienie gracza oraz celu
*/
void clearMaze()
{
	for (int i=0;i<COLLUMN;i++)
	{
		for (int j=0;j<ROW;j++)
		{
			maze[i][j].top = 1;
			maze[i][j].right = 1;
			maze[i][j].down = 1;
			maze[i][j].left = 1;
			maze[i][j].visited = 0;
		}
	}
	recGenerate(0,0);
	
	Player.x = 0;
	Player.y = 0;
	Player.x_old = 0;
	Player.y_old = 0;
	
	Goal.x = COLLUMN - 1;
	Goal.y = ROW - 1;
	
}

/*
	Funkcja odpowiedzialna za rysowanie labiryntu, celu i gracza w pozycji poczatkowej
*/
void mazeDisp()
{
	// draw maze
	int x = 0;
	int y = 0;
	for (int i=0;i<COLLUMN;i++)
	{
		for (int j=0;j<ROW;j++)
		{
			int x = i * WW-1;
			int y = j * WW-1;
			
			GUI_SetColor(GUI_BLACK); // kolor bialy
			
			if (maze[i][j].top == 1) 
			{
				GUI_FillRect(x  , y  , x+WW  , y+1  );
			}
			if (maze[i][j].down == 1)
			{
				GUI_FillRect(x  , y+WW, x+WW  , y+WW+1);
			}
			if (maze[i][j].right == 1)
			{
				GUI_FillRect(x+WW, y  , x+WW+1, y+WW  );
			}
			if (maze[i][j].left == 1)
			{
				GUI_FillRect(x  , y  , x+1  , y+WW  );
			}	
		}
	}
	
	// draw goal
	x = Goal.x*WW - 1;
	y = Goal.y*WW - 1;
	GUI_SetColor(GUI_RED); // czerwony 
	GUI_FillRect(x + WW/4 , y + WW/4 , x+WW , y+WW);
	
	
	// draw player
	x = Player.x*WW - 1;
	y = Player.y*WW - 1;
	GUI_SetColor(GUI_BLUE); // niebieski
	GUI_FillCircle(x + WW/2, y + WW/2, WW/4);

	
}
/*
	Funkcja odpowiedzialna za rysowanie poruszajacego sie gracza
	Po wykonaniu ruchu przez osobe sterujaca, gracz rysowany jest w nowym miejscu, a stara pozycja jest usuwana
*/
void drawPlayer(void* params)
{
	    while (1) 
	{
			osSemaphoreAcquire(player_id, osWaitForever);
			int x = Player.x*WW - 1;
			int y = Player.y*WW - 1;
			GUI_SetColor(GUI_BLUE); // niebieski
			GUI_FillCircle(x + WW/2, y + WW/2, WW/4);
			if(Player.x != Player.x_old || Player.y != Player.y_old)
			{
				x = Player.x_old*WW - 1;
				y = Player.y_old*WW - 1;
				GUI_SetColor(GUI_WHITE); // czarny
				GUI_FillCircle(x + WW/2, y + WW/2, WW/4);
			}
			GUI_Exec();
      GUI_X_ExecIdle();
			osSemaphoreRelease(player_id);
	}
}

/*
	Funkcja sprawdzajaca czy gracz dodtarl do celu
	Zwraca: True - gdy gracz osiagnal cel
*/
bool isFinished()
{
	return (Player.x == Goal.x && Player.y == Goal.y);
}

/*
	Warunek powowduj¹cy wyswietlenie labiryntu po otrzymaniu semafor display_id
*/
void DISPLAYThread(void* params) 
{                     
    while (1) 
	{
		osSemaphoreAcquire(display_id, osWaitForever);
		GUI_Clear();
		mazeDisp();
    GUI_Exec();
    GUI_X_ExecIdle();
  }
}

/*
	Watek ten generuje nowy labirynt po otrzymaniu semafora generate_id i uwalnia semafor display_id, który powoduje wywolanie watku odpowiedzialnego za jego przedstawienie graficzne
*/
void GENERATORThread(void* params) 
{
    while (1) {
		osSemaphoreAcquire(generate_id, osWaitForever);
		clearMaze();
		osSemaphoreRelease(display_id);
    }
}
/*
	Zapamietanie poprzedniej pozycji gracza
*/
void setOldPosition()
{
	Player.x_old = Player.x;
	Player.y_old = Player.y;
}
/*
	Watek obslugi URAT, odczytuje on dane przychodzace i odpowiada za kolizje. 
	Oprócz tego sprawdza warunek ukonczenia gry lub manualnego resetu labiryntu
*/

void UARTThread(void* params)
{
	char c;
	while(1)
	{		
		if(USART_GetChar(&c))
		{
			
			if (c == 'u') 
			{
				if (maze[Player.x][Player.y].top == 0){
					setOldPosition();
					Player.y--;
					osSemaphoreRelease(player_id);
				}				
			}
			
			if (c == 'd') 
			{
				if (maze[Player.x][Player.y].down == 0){
					setOldPosition();
					Player.y++;
					osSemaphoreRelease(player_id);
				}
			}
			
			if (c == 'l') 
			{
				if (maze[Player.x][Player.y].left == 0){
					setOldPosition();
					Player.x--;
					osSemaphoreRelease(player_id);
				}							
			}
			
			if (c == 'r') 
			{
				if (maze[Player.x][Player.y].right == 0) {
					setOldPosition();
					Player.x++;
					osSemaphoreRelease(player_id);
				}				
			}
			
			if (c == 'n' || isFinished()) 
			{
				Player.x_old = 0;
				Player.y_old = 0;
				osSemaphoreRelease(generate_id);
			}
		}
		
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1)
		{
					Player.x_old = 0;
					Player.y_old = 0;
					osSemaphoreRelease(generate_id);
				while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1);
		}
	}
}

void button()
{
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	 GPIO_InitTypeDef gpio; 			// obiekt gpio bedacy konfiguracja portów GPIO
	 gpio.Pin = GPIO_PIN_0; 			// konfigurujemy pin 5
	 gpio.Mode = GPIO_MODE_INPUT; // jako wejscie
	 gpio.Pull = GPIO_NOPULL;			//wylaczamy PULLUP
	 HAL_GPIO_Init(GPIOA, &gpio); // inicjalizacja modulu GPIOA
}

void init()
{
		osKernelInitialize();
    HAL_Init();
    SystemClock_Config();
		button();
		GUI_Init(); 
    USART_Init();
		GUI_SetBkColor(GUI_WHITE);
}
int main(void)
{
	init(); //inicjalizacji funkcji startowych
	// id semaforów
	display_id = osSemaphoreNew(1U, 0U, NULL);
	generate_id = osSemaphoreNew(1U, 0U, NULL);
	player_id = osSemaphoreNew(1U, 0U, NULL);
	
	// pierwsza generacja labiryntu
	osSemaphoreRelease(generate_id);
	
	osThreadNew(UARTThread, NULL, NULL);
	osThreadNew(DISPLAYThread, NULL, NULL);
	osThreadNew(GENERATORThread, NULL, NULL);
	osThreadNew(drawPlayer, NULL, NULL);
	
  // Start the Systemu
  osKernelStart();
  return 0;
}
