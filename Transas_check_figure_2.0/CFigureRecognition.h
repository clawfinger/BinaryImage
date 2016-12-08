#ifndef CFIGUREREGNITION_H
#define CFIGUREREGNITION_H

#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>
#include <cmath>



class CFigureRecognition
{
public:
	CFigureRecognition(int resolution = MATRIX_RESOLUTION);
	~CFigureRecognition();
	bool readFile(std::string);
	void showImage();
	void markFigures();
private:
	enum { MATRIX_RESOLUTION = 15};
	int m_parentArray[MATRIX_RESOLUTION+1];
	std::vector<std::vector<int>>* m_image;
	std::vector<std::pair<int, int>>* m_figurePoints;
	int findParent(int label);
	void unionParent(int B, int C);
	void makeParent(int label);
};

#endif