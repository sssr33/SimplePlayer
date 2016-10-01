// SimplePlayer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <memory>
#include <libhelpers\HJson.h>
#include <libhelpers\Json\JsonObjectNative.h>

#include "Player\PlayerWindow.h"
#include "Window\WindowContainer.h"

/*
TODO
- exceptions instead of true/false (maybe)
*/

int main() {
	std::unique_ptr<JsonObject> json2 = std::make_unique<JsonObjectNative>();

	//(*json2)["s"]["t"]["a"]["l"]["k"]["e"]["r"] = true;
	//(*json2)["s"]["t"]["a"]["l"]["k"]["e"]["r2"] = true;
	//(*json2) = true;

	(*json2)[0][0] = 22;
	(*json2)[0][1] = 3.14f;
	(*json2)[1][0] = 4.3;
	(*json2)[1][1] = "Hello";

	auto bbbb = (int)(*json2)[0][0];
	auto ffff = (float)(*json2)[0][1];
	auto dddd = (double)(*json2)[1][0];
	auto ssss = (std::string)(*json2)[1][1];

	auto json2Text = json2->ToString();

	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	WindowContainer<PlayerWindow> playerWnd(L"Player");

	while (true)
	{

	}

	return 0;
}

