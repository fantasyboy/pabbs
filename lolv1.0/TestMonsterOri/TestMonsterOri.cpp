// TestMonsterOri.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <cmath>
#include <ctime>
#include <iostream>
int main()
{
	auto a = GetTickCount();
	Sleep(3000);
	auto b = GetTickCount();

	auto c = b - a;
	std::cout << c << std::endl;

	getchar();
	getchar();

	return 0;
}

