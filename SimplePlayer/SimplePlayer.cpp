// SimplePlayer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Player\PlayerWindow.h"
#include "Window\WindowContainer.h"

int main() {
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	WindowContainer<PlayerWindow> playerWnd(L"Player");

	// tmp logic
	Sleep(INFINITE);

	return 0;
}

