// Transas_check_figure_2.0.cpp: ���������� ����� ����� ��� ����������� ����������.਍⼀⼀ഀ
#include "stdafx.h"
#include "CFigureRecognition.h"
int main()
{
	//gatherData();

	while (true)
	{
		std::cout << "Enter filename: ";
		std::string fileName;
		std::cin >> fileName;
		CFigureRecognition instance;
		instance.readFile(fileName);
		instance.markFigures();
		instance.showImage();
		instance._showFiguresList();
		instance.makeDecision();
	}
	return 0;
}