// Transas_check_figure_2.0.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "CFigureRecognition.h"

int main()
{
	CFigureRecognition instance;
	instance.readFile("circle.txt");
	instance.markFigures();
	instance.showImage();
	system("pause");
    return 0;
}

