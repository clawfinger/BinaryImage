
#include "stdafx.h"
#include "CFigureRecognition.h"



CFigureRecognition::SFigure::SFigure(int l, int r, int c)  
{
	label = l;
	row = r;
	column = c;
	perimeter = 0;
	square = 0;
	circularity = 0;
	haralickCircularity = 0;
	minRadialDistance = 0;
	maxRadialDistance = 0;
	centroidRow = -1;
	centroidColumn = -1;
}


CFigureRecognition::CFigureRecognition()
{
	//m_image.resize(MATRIX_RESOLUTION, vector<int>(MATRIX_RESOLUTION));
	//m_tempImage.resize(MATRIX_RESOLUTION, vector<int>(MATRIX_RESOLUTION));
}

CFigureRecognition::~CFigureRecognition()
{
	
}

bool CFigureRecognition::readFile(std::string fileName)   
{
	std::ifstream input(fileName);

	if (!input.is_open())
	{
		std::cout << "File open failure";
		return false;
	}

	char character;
	input.get(character);

	int i = 0;
	while(true)
	{
		m_image.push_back(std::vector<int>());  //added
		while(character != '\n')
		{
			m_image[i].push_back(std::stoi(&character));
			input.get(character);
		}
		i++;
		input.get(character);
		if (input.peek() == EOF)
			break;
		
	}
	input.close();
	m_matrixResolution = int(m_image[0].size());
	return true;

}





void CFigureRecognition::markFigures()   //2 pass marking algorithm with union-find 
{
	/*
	  _
	_|C|_
   |B|A|D|  4-connected mask
	 |E|


	*/
	int currentNum = 1;
	int B, C;
	for (int r = 1; r < m_matrixResolution; r++)
	{
		for (int c = 1; c < m_matrixResolution; c++)
		{
			
			B = m_image[r][c - 1];
			C = m_image[r - 1][c];

			if (m_image[r][c] == 1)
			{

				if (B == 0 && C == 0)
				{
					m_image[r][c] = currentNum;
					makeParent(currentNum);
					currentNum++;					
				}
				else if (B != 0 && C == 0)
				{
					m_image[r][c] = B;
				}
				else if (B == 0 && C != 0)
				{
					m_image[r][c] = C;
				}
				else if (B != 0 && C != 0)
				{
					if (B == C)
					{
						m_image[r][c] = C;
					}
					else
					{
						m_image[r][c] = std::min(B,C);
						unionParent(B, C);
					}

				}
			}
		}
	}


	for (int r = 0; r < m_matrixResolution; r++)
	{
		for (int c = 0; c < m_matrixResolution; c++)
		{
			int A = m_image[r][c];
			{
				if (A == 0)
				{
					continue;
				}
				else
				{
					int currentLabel = findParent(A);
					m_image[r][c] = currentLabel;
					m_figurePoints.emplace(currentLabel, SFigure(currentLabel, r, c));
					//square
					m_figurePoints.at(currentLabel).square++;
					//gathering data for calculating centroid
					m_figurePoints.at(currentLabel).figurePoints.push_back(std::pair<int, int>(r, c));
					//border points
					if (isBorderPoint(r, c))
					{
						m_figurePoints.at(currentLabel).borderPoints.push_back(std::pair<int, int>(r, c));
					}

				}
			}
		}
	}
}

//simple union-find structure method

int CFigureRecognition::findParent(int label)
{
	if (m_parentArray[label] == label)
		return label;
	else return findParent(m_parentArray[label]);
}

//simple union-find structure method

void CFigureRecognition::unionParent(int B, int C)
{
	int rootB = findParent(B);
	int rootC = findParent(C);
	if (rootB != rootC)
	{
		m_parentArray[rootB] = rootC;
	}
}

//simple union-find structure method

void CFigureRecognition::makeParent(int label)
{
	m_parentArray[label] = label;
}

//figure closing to get rid of missing pixels inside the figure

void CFigureRecognition::figureClosing()
{
	m_tempImage.resize(m_matrixResolution, vector<int>(m_matrixResolution));
	//dilation
	int r, c;
	for (r = 0; r < m_matrixResolution; r++)
	{
		for (c = 0; c < m_matrixResolution; c++)
		{
			if (m_image[r][c] == 1)
			{
				for (auto neighborPoint : getNeighborPoints(r, c))
				{
					m_tempImage[neighborPoint.first][neighborPoint.second] = 1;
				}
			}
		}
	}
	//erosion
	for (r = 0; r < m_matrixResolution; r++)
	{
		for (c = 0; c < m_matrixResolution; c++)
		{
			bool B = false;
			bool C = false;
			bool D = false;
			bool E = false;

			if (m_tempImage[r][c] == 1)
			{
				if ((c - 1) < 0)
				{
					B = false;
				}
				else if (m_tempImage[r][c - 1] != 0)
				{
					B = true;
				}
				if ((r - 1) < 0)
				{
					C = false;
				}
				else if (m_tempImage[r - 1][c] != 0)
				{
					C = true;
				}
				if ((c + 1) >= m_matrixResolution)
				{
					D = false;
				}
				else if (m_tempImage[r][c + 1] != 0)
				{
					D = true;
				}
				if ((r + 1) >= m_matrixResolution)
				{
					E = false;
				}
				else if (m_tempImage[r + 1][c] != 0)
				{
					E = true;
				}

				if (!(B && C && D && E))
				{
					m_image[r][c] = 0;
				}
				else m_image[r][c] = 1;
			}
		}

	}

}

bool CFigureRecognition::isBorderPoint(int r, int c)
{
	int A = m_image[r][c];
	if (A == 0)
	{
		return false;
	}
	int B = m_image[r][c - 1];
	int C = m_image[r - 1][c];
	int D = m_image[r][c + 1];
	int E = m_image[r + 1][c];

	if (!(B && C && D && E))
	{
		return true;
	}
	else return false;
}

//calculating the circularity parameter by Robert M. Haralick

void CFigureRecognition::calculateHaralickCircularity()
{
	float meanRadialDistance = 0.0;
	float standartDeviation = 0.0;
	for (auto figures : m_figurePoints)
	{
		float centroidRow = figures.second.centroidRow;
		float centroidColumn = figures.second.centroidColumn;

		float sum = 0;
		float radialDistance = 0;

		float min = std::sqrt(std::pow((figures.second.row - centroidRow), 2)
			+ std::pow((figures.second.column - centroidColumn), 2));
		float max = min;

		for (auto borderPoint : figures.second.borderPoints)
		{
			radialDistance = std::sqrt(std::pow((borderPoint.first - centroidRow), 2)
				+ std::pow((borderPoint.second - centroidColumn), 2));


			if (radialDistance > max)
			{
				max = radialDistance;
			}

			if (radialDistance < min)
			{
				min = radialDistance;
			}
			
			sum += radialDistance;
		}

		m_figurePoints.at(figures.first).minRadialDistance = min;
		m_figurePoints.at(figures.first).maxRadialDistance = max;
		m_figurePoints.at(figures.first).radialDistanceRatio = max / min;

		meanRadialDistance = sum / figures.second.borderPoints.size();

		sum = 0;

		for (auto borderPoint : figures.second.borderPoints)
		{
			sum += std::pow(std::sqrt(std::pow((borderPoint.first - centroidRow), 2)
				+ std::pow((borderPoint.second - centroidColumn), 2)) - meanRadialDistance, 2);
		}

		standartDeviation = std::sqrt(sum / figures.second.borderPoints.size());
		m_figurePoints.at(figures.first).haralickCircularity = meanRadialDistance / standartDeviation;
		
	}

}

void CFigureRecognition::calculateCircularity()
{
	for (auto figure : m_figurePoints)
	{
		m_figurePoints.at(figure.first).circularity = (4 * figure.second.square * 3.14159) / std::pow(figure.second.perimeter, 2);
		//m_figurePoints->at(figure.first).circularity = std::pow(figure.second.perimeter, 2) / figure.second.square;
		//m_figurePoints->at(figure.first).circularity = figure.second.square / std::pow(figure.second.perimeter, 2);
	}
}

void CFigureRecognition::calculateCentroid()
{
	for (auto figure : m_figurePoints)
	{
		int sumOfRows = 0;
		int sumOfColumns = 0;
		int size = (int)figure.second.figurePoints.size();

		for (auto point : figure.second.figurePoints)
		{
			sumOfColumns += point.second;
			sumOfRows += point.first;
		}
		m_figurePoints.at(figure.first).centroidRow = (float(sumOfRows) / size);
		m_figurePoints.at(figure.first).centroidColumn = (float(sumOfColumns) / size);
	}
}

void CFigureRecognition::calculateAxis()  //calculating the length of X axis of figure
{
	int xAxisLenght = 0;
	int yAxisLenght = 0;
	for (auto figure : m_figurePoints)
	{
		int min = figure.second.borderPoints[0].first;
		int max = figure.second.borderPoints[0].first;
		for (auto point : figure.second.borderPoints)
		{
			if (point.first < min) {
				min = point.first;
			}
			else if(point.first > max){
				max = point.first;
			}
		}
		yAxisLenght = max - min;
		min = figure.second.borderPoints[0].second;
		max = figure.second.borderPoints[0].second;
		for (auto point : figure.second.borderPoints)
		{
			if (point.second < min) {
				min = point.second;
			}
			else if (point.second > max) {
				max = point.second;
			}
		}
		xAxisLenght = max - min;
		m_figurePoints.at(figure.first).extremalAxisLength = xAxisLenght + 1;
	}
}

void CFigureRecognition::calculateMoments()
{
	for (auto figure : m_figurePoints)
	{
		float sumOfRowDifference = 0;
		float sumOfColumnDifference = 0;
		for (auto point : figure.second.figurePoints)
		{
			sumOfRowDifference += std::pow((point.first - figure.second.centroidRow), 2);
			sumOfColumnDifference += std::pow((point.second - figure.second.centroidColumn), 2);
		}
		m_figurePoints.at(figure.first).secondOrderRowMoment = (float)sumOfRowDifference / figure.second.square;
		m_figurePoints.at(figure.first).secondOrderColumnMoment = (float)sumOfColumnDifference / figure.second.square;
	}
}







void CFigureRecognition::calculatePerimeter()
{

	//P = K + A(sqrt(2)-1)
	for (auto figure : m_figurePoints)
	{
		int numOfDiagonalConnections = 0;
		for (auto borderPoint : figure.second.borderPoints)
		{
			//checking diagonal neighbors
			vector<std::pair<int, int>> diagonalPoints = getDiagonalPoints(borderPoint.first, borderPoint.second);
			for (auto x : diagonalPoints)
			{
				if (isBorderPoint(x.first, x.second))
				{
					//Incrementing A if both points betwin checked pair is not border points
					if (!isBorderPoint(borderPoint.first, x.second) && !isBorderPoint(x.first, borderPoint.second))
					{
						numOfDiagonalConnections++;
					}
				}
			}

			
		}
		m_figurePoints.at(figure.first).perimeter = figure.second.borderPoints.size() + 
			(numOfDiagonalConnections * (std::sqrt(2) - 1))/2;
	}
	
	
}



vector<std::pair<int, int>> CFigureRecognition::getDiagonalPoints(int r, int c)  //returning vector of diagonal neighbors coords
{
	vector<std::pair<int, int>> points{
		std::pair<int, int>(r+1, c+1), 
		std::pair<int, int>(r - 1, c - 1), 
		std::pair<int, int>(r - 1, c + 1),
		std::pair<int, int>(r + 1, c - 1)
	};
	return points;
}

vector<std::pair<int, int>> CFigureRecognition::getNeighborPoints(int r, int c) //returning vector of orthogonal neighbors
{
	return vector<std::pair<int, int>> { 
		std::pair<int, int>(r, c + 1),
		std::pair<int, int>(r, c - 1),
		std::pair<int, int>(r + 1, c),
		std::pair<int, int>(r - 1, c),
	};
}

void CFigureRecognition::makeDecision()
{
	markFigures();
	figureClosing();
	calculateCentroid();
	calculatePerimeter();
	calculateHaralickCircularity();
	calculateCircularity();
	calculateAxis();
	calculateMoments();

	for (auto figure : m_figurePoints)
	{
		//magic numbers picked from data.txt
		std::cout << "Figure label: " << figure.first << std::endl;
		std::cout << "Row moment: " << figure.second.secondOrderRowMoment << std::endl;
		std::cout << "Column moment: " << figure.second.secondOrderColumnMoment << std::endl;
		/*if (figure.second.square < 21 || figure.second.square > 100)
			std::cout << "Unknown figure" << std::endl;
		else*/ if (figure.second.circularity < 0.96)
			std::cout << "Unknown figure" << std::endl;	
		else	 if (std::abs(figure.second.radialDistanceRatio - 1.4) < 0.02)
		{
			std::cout << "Its s square! Upper leftmost corner [row:column]: [" << figure.second.row
				<< ':' << figure.second.column << ']' << std::endl;
			std::cout << "Square side is: " << m_figurePoints.at(figure.first).extremalAxisLength << std::endl;
		}
		else if (std::round(figure.second.haralickCircularity) >= 15)
		{
			std::cout << "Its a circle! Center coordinates are: [" << figure.second.centroidRow << ':'
				<< figure.second.centroidColumn << ']' << std::endl;
			std::cout << "Diameter of circle: " << m_figurePoints.at(figure.first).extremalAxisLength << std::endl;
		}
		else
			std::cout << "Unknown figure" << std::endl;
	}
}


