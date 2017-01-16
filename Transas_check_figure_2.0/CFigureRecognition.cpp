
#include "stdafx.h"
#include "CFigureRecognition.h"




SFigure::SFigure(int l, int r, int c)
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




CFigureRecognition::CFigureRecognition(int resolution)
{
	m_image = new vector<vector<int>>(MATRIX_RESOLUTION, vector<int>(MATRIX_RESOLUTION));
	m_tempImage = new vector<vector<int>>(MATRIX_RESOLUTION, vector<int>(MATRIX_RESOLUTION));
	m_figurePoints = new std::map<int, SFigure>;
}

CFigureRecognition::~CFigureRecognition()
{
	delete m_image;
	delete m_tempImage;
	delete m_figurePoints;
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

	int r, c;
	for (r = 0; r < MATRIX_RESOLUTION; r++)  
	{
		for (c = 0; c < MATRIX_RESOLUTION; c++)	
		{
			if (character != '\n')
			{
				(*m_image)[r][c] = (std::stoi(&character));
			
			}
			else break;
			input.get(character);
		}
		input.get(character);
		
	}
	input.close();
	return true;

}

void CFigureRecognition::showImage()
{
	int r, c;
	for (r = 0; r < MATRIX_RESOLUTION; r++)
	{
		for (c = 0; c < MATRIX_RESOLUTION; c++)
		{
			std::cout << (*m_image)[r][c];
		}
		std::cout << std::endl;
	}
}

void CFigureRecognition::markFigures()
{
	int currentNum = 1;
	int B, C;
	for (int r = 1; r < MATRIX_RESOLUTION; r++)
	{
		for (int c = 1; c < MATRIX_RESOLUTION; c++)
		{
			
			B = (*m_image)[r][c - 1];
			C = (*m_image)[r - 1][c];

			if ((*m_image)[r][c] == 1)
			{

				if (B == 0 && C == 0)
				{
					(*m_image)[r][c] = currentNum;
					makeParent(currentNum);
					currentNum++;					
				}
				else if (B != 0 && C == 0)
				{
					(*m_image)[r][c] = B;
				}
				else if (B == 0 && C != 0)
				{
					(*m_image)[r][c] = C;
				}
				else if (B != 0 && C != 0)
				{
					if (B == C)
					{
						(*m_image)[r][c] = C;
					}
					else
					{
						(*m_image)[r][c] = std::min(B,C);
						unionParent(B, C);
					}

				}
			}
		}
	}


	for (int r = 0; r < MATRIX_RESOLUTION; r++)
	{
		for (int c = 0; c < MATRIX_RESOLUTION; c++)
		{
			int A = (*m_image)[r][c];
			{
				if (A == 0)
				{
					continue;
				}
				else
				{
					int currentLabel = findParent(A);
					(*m_image)[r][c] = currentLabel;
					m_figurePoints->emplace(currentLabel, SFigure(currentLabel, r, c));
					//square
					m_figurePoints->at(currentLabel).square++;
					//centroid
					m_figurePoints->at(currentLabel).centroidRows.push_back(r);
					m_figurePoints->at(currentLabel).centroidColumns.push_back(c);

					//border points
					if (isBorderPoint(r, c))
					{
						m_figurePoints->at(currentLabel).borderPoints.push_back(std::pair<int, int>(r, c));
					}

				}
			}
		}
	}
}

void CFigureRecognition::makeDecision()
{
	for (auto figure : (*m_figurePoints))
	{
		figure.second.radialDistanceRatio *= 10;		

		std::cout << "Figure label: " << figure.first << std::endl;
		if (figure.second.square < 21 || figure.second.square > 100)
			std::cout << "Unknown figure" << std::endl;
		else if(figure.second.circularity < 0.96)
			std::cout << "Unknown figure" << std::endl;
		else if(std::round(figure.second.radialDistanceRatio) / 10 == 1.4)
		{
			std::cout << "Its s square! Upper leftmost corner [row:column]: [" << figure.second.row
			<< ':' << figure.second.column << ']' << std::endl;
			std::cout << "Square side is: " << m_figurePoints->at(figure.first).extremalAxisLength << std::endl;
		}
		else if (std::round(figure.second.haralickCircularity) >= 15)
		{
			std::cout << "Its a circle! Center coordinates are: [" << figure.second.centroidRow << ':'
				<< figure.second.centroidColumn << ']' << std::endl;
			std::cout << "Diameter of circle: " << m_figurePoints->at(figure.first).extremalAxisLength << std::endl;
		}
		else 
			std::cout << "Unknown figure" << std::endl;
	}
}

void CFigureRecognition::_showFiguresList()
{
	for (auto x : (*m_figurePoints))
	{

		calculateCentroid();
		calculatePerimeter();
		calculateHaralickCircularity();
		calculateCircularity();
		calculateAxis();
		std::cout << "Figure label: " << x.first << std::endl;
		std::cout << "Figure upper leftmost corner [row:column]: [" << x.second.row
			<< ':' << x.second.column << ']' << std::endl;
		std::cout << "Figure square: " << x.second.square << std::endl;
		std::cout << "Centroid row: " << m_figurePoints->at(x.first).centroidRow << std::endl;
		std::cout << "Centroid column: " << m_figurePoints->at(x.first).centroidColumn << std::endl;		
		std::cout << "Perimeter: " << m_figurePoints->at(x.first).perimeter << std::endl;		
		std::cout << "Haralick's Circularity: " << m_figurePoints->at(x.first).haralickCircularity << std::endl;		
		std::cout << "Circularity: " << m_figurePoints->at(x.first).circularity << std::endl;
		
		std::cout << "Radial distance ratio: " << m_figurePoints->at(x.first).maxRadialDistance / m_figurePoints->at(x.first).minRadialDistance << std::endl;
		std::cout << "Minimal radial distance: " << m_figurePoints->at(x.first).minRadialDistance << std::endl;
		std::cout << "Maximum radial distance: " << m_figurePoints->at(x.first).maxRadialDistance << std::endl;
		std::cout << std::endl;
		std::cout << "--------------------------------------------------------------" << std::endl;
		std::cout << std::endl;
	}
	std::cout << std::endl;
}



void CFigureRecognition::calculateHaralickCircularity()
{
	double meanRadialDistance = 0.0;
	double standartDeviation = 0.0;
	for (auto figures : (*m_figurePoints))
	{
		double centroidRow = figures.second.centroidRow;
		double centroidColumn = figures.second.centroidColumn;

		double sum = 0;
		double radialDistance = 0;

		double min = std::sqrt(std::pow((figures.second.row - centroidRow), 2)
			+ std::pow((figures.second.column - centroidColumn), 2));
		double max = min;;

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

		m_figurePoints->at(figures.first).minRadialDistance = min;
		m_figurePoints->at(figures.first).maxRadialDistance = max;
		m_figurePoints->at(figures.first).radialDistanceRatio = max / min;
		meanRadialDistance = sum / figures.second.borderPoints.size();
		sum = 0;

		for (auto borderPoint : figures.second.borderPoints)
		{
			sum += std::pow(std::sqrtf(std::pow((borderPoint.first - centroidRow), 2)
				+ std::pow((borderPoint.second - centroidColumn), 2)) - meanRadialDistance, 2);
		}
		standartDeviation = std::sqrtf(sum / figures.second.borderPoints.size());
		m_figurePoints->at(figures.first).haralickCircularity = meanRadialDistance / standartDeviation;
		
	}

}

void CFigureRecognition::calculateCircularity()
{
	for (auto figure : (*m_figurePoints))
	{
		m_figurePoints->at(figure.first).circularity = (4 * figure.second.square * 3.14159) / std::pow(figure.second.perimeter, 2);
		//m_figurePoints->at(figure.first).circularity = std::pow(figure.second.perimeter, 2) / figure.second.square;
		//m_figurePoints->at(figure.first).circularity = figure.second.square / std::pow(figure.second.perimeter, 2);
	}
}

void CFigureRecognition::calculateCentroid()
{
	for (auto x : (*m_figurePoints))
	{
		int sumOfRows = 0;
		int sumOfColumns = 0;
		int sizeOfRows = x.second.centroidRows.size();
		int sizeOfColumns = x.second.centroidColumns.size();
		for (auto i : x.second.centroidColumns)
		{
			sumOfColumns += i;
		}
		for (auto i : x.second.centroidRows)
		{
			sumOfRows += i;
		}
		m_figurePoints->at(x.first).centroidRow = (float(sumOfRows) / sizeOfRows);
		m_figurePoints->at(x.first).centroidColumn = (float(sumOfColumns) / sizeOfColumns);
	}
}

void CFigureRecognition::calculateAxis()
{
	int xAxisLenght = 0;
	int yAxisLenght = 0;
	for (auto figure : (*m_figurePoints))
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
		m_figurePoints->at(figure.first).extremalAxisLength = xAxisLenght + 1;
	}
}

void CFigureRecognition::figureClosing()
{
	//dilation
	int r, c;
	for (r = 0; r < MATRIX_RESOLUTION; r++)		
	{
		for (c = 0; c < MATRIX_RESOLUTION; c++)
		{
			if ((*m_image)[r][c]  == 1)		
			{
				for (auto neighborPoint : getNeighborPoints(r, c))
				{
					(*m_tempImage)[neighborPoint.first][neighborPoint.second] = 1;
				}
			}									
		}
	}
	//erosion
	for (r = 0; r < MATRIX_RESOLUTION; r++)
	{
		for (c = 0; c < MATRIX_RESOLUTION; c++)
		{
			bool B = false;
			bool C = false;
			bool D = false;
			bool E = false;

			if ((*m_tempImage)[r][c] == 1)
			{
				if ((c - 1) < 0)
				{
					B = false;
				}
				else if ((*m_tempImage)[r][c - 1] != 0)
				{
					B = true;
				}
				if ((r - 1) < 0)
				{
					C = false;
				}
				else if ((*m_tempImage)[r - 1][c] != 0)
				{
					C = true;
				}
				if ((c + 1) > MATRIX_RESOLUTION)
				{
					D = false;
				}
				else if ((*m_tempImage)[r][c + 1] != 0)
				{
					D = true;
				}
				if ((r + 1) > MATRIX_RESOLUTION)
				{
					E = false;
				}
				else if ((*m_tempImage)[r + 1][c] != 0)
				{
					E = true;
				}

				if (!(B && C && D && E))
				{
					(*m_image)[r][c] = 0;
				}
				else (*m_image)[r][c] = 1;
			}
		}

	}
	
}


int CFigureRecognition::findParent(int label)
{
	if (m_parentArray[label] == label)
		return label;
	else return findParent(m_parentArray[label]);
}

void CFigureRecognition::unionParent(int B, int C)
{
	int rootB = findParent(B);
	int rootC = findParent(C);
	if (rootB != rootC)
	{
		m_parentArray[rootB] = rootC;
	}
}

void CFigureRecognition::makeParent(int label)
{
	m_parentArray[label] = label;
}

void CFigureRecognition::calculatePerimeter()
{

	//P = K + A(sqrt(2)-1)
	for (auto figure : (*m_figurePoints))
	{
		int numOfDiagonalConnections = 0;
		for (auto borderPoint : figure.second.borderPoints)
		{
			//проверяем диагональные точки
			vector<std::pair<int, int>> diagonalPoints = getDiagonalPoints(borderPoint.first, borderPoint.second);
			for (auto x : diagonalPoints)
			{
				if (isBorderPoint(x.first, x.second))
				{
					//если точки между текущей и диагональной не граничные то добавляем к А еденицу
					if (!isBorderPoint(borderPoint.first, x.second) && !isBorderPoint(x.first, borderPoint.second))
					{
						numOfDiagonalConnections++;
					}
				}
			}

			
		}
		m_figurePoints->at(figure.first).perimeter = figure.second.borderPoints.size() + 
			(numOfDiagonalConnections * (std::sqrt(2) - 1))/2;
	}
	
	
}

bool CFigureRecognition::isBorderPoint(int r, int c)
{
	int A = (*m_image)[r][c];
	if (A == 0)
	{
		return false;
	}
	int B = (*m_image)[r][c - 1];
	int C = (*m_image)[r - 1][c];
	int D = (*m_image)[r][c + 1];
	int E = (*m_image)[r + 1][c];

	if (!(B && C && D && E))
	{
		return true;
	}
	else return false;
}

vector<std::pair<int, int>> CFigureRecognition::getDiagonalPoints(int r, int c)
{
	vector<std::pair<int, int>> points{
		std::pair<int, int>(r+1, c+1), 
		std::pair<int, int>(r - 1, c - 1), 
		std::pair<int, int>(r - 1, c + 1),
		std::pair<int, int>(r + 1, c - 1)
	};
	return points;
}

vector<std::pair<int, int>> CFigureRecognition::getNeighborPoints(int r, int c)
{
	return vector<std::pair<int, int>> { 
		std::pair<int, int>(r, c + 1),
		std::pair<int, int>(r, c - 1),
		std::pair<int, int>(r + 1, c),
		std::pair<int, int>(r - 1, c),
	};
}

void CFigureRecognition::saveToFile(std::string fileName)
{
	std::ofstream file;
	
	for (auto x : (*m_figurePoints))
	{
		file.open("data.txt", std::ios_base::app);
		file << fileName << '\n';
		file << "Label: " << x.second.label << '\n';
		file << "Square: " << x.second.square << '\n';
		file << "Circularity: " << x.second.circularity << '\n';
		file << "Haralicks: " << x.second.haralickCircularity << '\n';
		file << "Radial distance ratio: " << x.second.maxRadialDistance / x.second.minRadialDistance << '\n';
		file << std::endl;
		file.close();
	}
}

//void gatherData(void)
//{
//	vector<std::string> fileList{
//		"circle5.txt", "circle6a.txt",
//		"circle6b.txt",
//		"circle7.txt", "circle8.txt",
//		"circle9a.txt", "circle9b.txt",
//		"circle9c.txt", "circle10a.txt",
//		"circle10b.txt", "circle10c.txt",
//		"square5.txt", "square6.txt",
//		"square7.txt", "square8.txt",
//		"square9.txt", "square10.txt", };
//	
//	for (auto file : fileList)
//	{
//		CFigureRecognition instance;
//		instance.readFile(file);
//		instance.markFigures();
//		instance._showFiguresList();
//		instance.saveToFile(file);
//	}
//}
