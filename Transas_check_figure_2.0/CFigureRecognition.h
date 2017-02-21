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

#define MATRIX_RESOLUTION 15
using std::vector;




class CFigureRecognition
{
public:
	CFigureRecognition();
	~CFigureRecognition();
	bool readFile(std::string);
	void makeDecision();


private:	

	struct SFigure //structure, describing figure properties
	{
		SFigure(int l, int r, int c);
		int label;
		int row;
		int column;
		int square;
		double perimeter;
		double centroidRow;
		double centroidColumn;
		double haralickCircularity;
		double circularity;
		double minRadialDistance;
		double maxRadialDistance;
		double radialDistanceRatio;
		int extremalAxisLength;
		vector<int> centroidRows;
		vector<int> centroidColumns;
		vector<std::pair<int, int>> borderPoints;
	};

	//members
	int m_matrixResolution;
	int m_parentArray[MATRIX_RESOLUTION];
	std::vector<std::vector<int>> m_image;
	std::vector<std::vector<int>> m_tempImage;
	std::map<int, SFigure> m_figurePoints;	

private:
	int findParent(int label);
	void unionParent(int B, int C);
	void makeParent(int label);

	void markFigures();
	void figureClosing();
	void calculatePerimeter();
	bool isBorderPoint(int r, int c);
	void calculateHaralickCircularity();
	void calculateCircularity();
	void calculateCentroid();	
	void calculateAxis();

	vector<std::pair<int, int>> getDiagonalPoints(int r, int c);
	vector<std::pair<int, int>> getNeighborPoints(int r, int c);	

};

#endif