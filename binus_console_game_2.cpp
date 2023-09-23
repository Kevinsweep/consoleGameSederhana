
#include <iostream>
#include <windows.h>
#include <thread>
#include <stdio.h>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

int nScreenWidth = 80;
int nScreenHeight = 30;
int nScreenArea = nScreenWidth * nScreenHeight;
wstring wsMap;
int nPointCounter = 0;

//class snakes {
//	char tail;
//	int nSnakeLen;
//};


bool Collision_detection1(int nCurrentX, int nCurrentY, int nScreenWidth, bool &bEaten) {
	for(int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++) {
			if (wsMap[(nCurrentY - y) * nScreenWidth + x + nCurrentX] == '#') {
				return false;
			}
			else if (wsMap[(nCurrentY - y) * nScreenWidth + x + nCurrentX] == 4) {
				bEaten = true;
				nPointCounter++;
			}
		}

	return true;
}

int main()
{
	//map
	wsMap += L"################################################################################";//1
	wsMap += L"#                                                                              #";//2
	wsMap += L"#                                                                              #";//3
	wsMap += L"#                                                                              #";//4
	wsMap += L"#                                                                              #";//5
	wsMap += L"#                                                                              #";//6
	wsMap += L"#                                                                              #";//7
	wsMap += L"#           ##########           ###########               ##########          #";//8
	wsMap += L"#           #                                                       #          #";//9
	wsMap += L"#           #                                                       #          #";//10
	wsMap += L"#           #                                                       #          #";//11
	wsMap += L"#           #                                                       #          #";//12
	wsMap += L"#           #                                                       #          #";//13
	wsMap += L"#                                #############                                 #";//14
	wsMap += L"#                                #############                                 #";//15
	wsMap += L"#                                #############                                 #";//16
	wsMap += L"#                                #############                                 #";//17
	wsMap += L"#                                #############                                 #";//18
	wsMap += L"#            #                                                       #         #";//19
	wsMap += L"#            #                                                       #         #";//20
	wsMap += L"#            #                                                       #         #";//21
	wsMap += L"#            #                                                       #         #";//22
	wsMap += L"#            #                                                       #         #";//23
	wsMap += L"#            ###########           ###########               #########         #";//24
	wsMap += L"#                                                                              #";//25
	wsMap += L"#                                                                              #";//26
	wsMap += L"#                                                                              #";//27
	wsMap += L"#                                                                              #";//28
	wsMap += L"#                                                                              #";//29
	wsMap += L"################################################################################";//30
																								 
	//create screen buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	int nCurrentX1 = nScreenWidth / 2;
	int nCurrentY1 = nScreenHeight - 2;
	int nFoodX = 0, nFoodY = 0;

	bool bKey1[4];
	bool bEaten = true;


	std::chrono::milliseconds ms(20000);
	auto end = std::chrono::system_clock::now() + ms;


	while (system_clock::now() < end) {
		//spawn food====================================================
		if (bEaten == true) {
			wsMap[nFoodY * nScreenWidth + nFoodX] = L' ';
			nFoodX = rand() % 80;
			nFoodY = rand() % 30;
			while (wsMap[nFoodY * nScreenWidth + nFoodX] == L'#') {
				nFoodX = rand() % 80;
				nFoodY = rand() % 30;
			}
			bEaten = false;
		}

		//game timing ==================================================
		this_thread::sleep_for(50ms);

		//movement =====================================================
		for (int k = 0; k < 4; k++)								// RLDU
			bKey1[k] = (0x8000 & GetAsyncKeyState((unsigned char)("DASW"[k]))) != 0;
		
		
		//player 1(bottom)
		nCurrentX1 += (bKey1[0] && Collision_detection1(nCurrentX1 + 1, nCurrentY1, nScreenWidth, bEaten)) ? 1 : 0;
		nCurrentX1 -= (bKey1[1] && Collision_detection1(nCurrentX1 - 1, nCurrentY1, nScreenWidth, bEaten)) ? 1 : 0;
		nCurrentY1 += (bKey1[2] && Collision_detection1(nCurrentX1, nCurrentY1 + 1, nScreenWidth, bEaten)) ? 1 : 0;
		nCurrentY1 -= (bKey1[3] && Collision_detection1(nCurrentX1, nCurrentY1 - 1, nScreenWidth, bEaten)) ? 1 : 0;

		//display playfield
		for (int x = 0; x < nScreenWidth; x++)
			for (int y = 0; y < nScreenHeight; y++)
				screen[y * nScreenWidth + x] = wsMap[y * nScreenWidth + x];

		// display character 1
		for (int px = 0; px < 3; px++)
			for (int py = 0; py < 3; py++)
				screen[(nCurrentY1 - py) * nScreenWidth + nCurrentX1 + px] = 4;

		//display food
		wsMap[nFoodY * nScreenWidth + nFoodX] = 4;
		screen[nFoodY * nScreenWidth + nFoodX] = wsMap[nFoodY * nScreenWidth + nFoodX];

		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	
	}

	CloseHandle(hConsole);
	cout << "game over" << endl;
	cout << "your score is " << nPointCounter << endl;
	cout << "press enter to close game.......";
	getchar();

	
}