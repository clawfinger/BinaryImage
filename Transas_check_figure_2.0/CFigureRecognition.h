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
		//float perimeter;
		float centroidRow;
		float centroidColumn;
		float haralickCircularity;
		float radialDistanceRatio;
		float secondOrderRowMoment;
		float secondOrderColumnMoment;
		int extremalAxisLength;
		vector<std::pair<int, int>> figurePoints;
		vector<std::pair<int, int>> borderPoints;
	};

	//members
	int m_matrixResolution;
	int m_parentArray[100];
	std::vector<std::vector<int>> m_image;
	std::vector<std::vector<int>> m_tempImage;
	std::map<int, SFigure> m_figurePoints;	

private:
	int findParent(int label);
	void unionParent(int B, int C);
	void makeParent(int label);

	void markFigures();
	void figureClosing();
	bool isBorderPoint(int r, int c);
	void calculateHaralickCircularity();
	void calculateCentroid();	
	void calculateAxis();
	void calculateMoments();
	vector<std::pair<int, int>> getNeighborPoints(int r, int c);	

};

#endif