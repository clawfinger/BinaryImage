// Transas_check_figure_2.0.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "CFigureRecognition.h"

int main()
{
	CFigureRecognition instance;
	instance.readFile("square1.txt");
	instance.showImage();
	std::cout << std::endl;
	instance.markFigures();
	instance.showImage();
	system("pause");
    return 0;
}

