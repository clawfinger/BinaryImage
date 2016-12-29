#ifndef CFIGUREREGNITION_H
#define CFIGUREREGNITION_H

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>
#include <cmath>

using std::vector;

struct SFigure
{
	SFigure(int l, int r, int c);
	int label;
	int row;
	int column;
	int square;
	int perimeter;
	int centroidRow;
	int centroidColumn;
	vector<int> centroidRows;
	vector<int> centroidColumns;
	vector<std::pair<int, int>> borderPoints;
};

class CFigureRecognition
{
public:
	CFigureRecognition(int resolution = MATRIX_RESOLUTION);
	~CFigureRecognition();
	bool readFile(std::string);
	void showImage();
	void markFigures();
	void _showFiguresList();
	enum { MATRIX_RESOLUTION = 15 };
private:	
	int m_parentArray[MATRIX_RESOLUTION+1];
	std::vector<std::vector<int>>* m_image;
	std::map<int, SFigure>* m_figurePoints;
	int findParent(int label);
	void unionParent(int B, int C);
	void makeParent(int label);
	
};

#endif