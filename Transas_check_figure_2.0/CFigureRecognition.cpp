
#include "stdafx.h"
#include "CFigureRecognition.h"




SFigure::SFigure(int l, int r, int c)
{
	label = l;
	row = r;
	column = c;
	perimeter = 0;
	square = 0;
}




CFigureRecognition::CFigureRecognition(int resolution)
{
	m_image = new vector<vector<int>>(MATRIX_RESOLUTION, vector<int>(MATRIX_RESOLUTION));
	m_figurePoints = new std::map<int, SFigure>;
}

CFigureRecognition::~CFigureRecognition()
{
	delete m_image;
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
			if (A == 0)
			{
				continue;
			}
			else
			{
				int currentLabel = findParent(A);
				(*m_image)[r][c] = currentLabel;
				m_figurePoints->emplace(currentLabel, SFigure(currentLabel, r, c));
				m_figurePoints->at(currentLabel).square++;
				m_figurePoints->at(currentLabel).centroidRows.push_back(r);
				m_figurePoints->at(currentLabel).centroidColumns.push_back(c);
			}
		}
	}
}

void CFigureRecognition::_showFiguresList()
{
	for (auto x : (*m_figurePoints))
	{
		std::cout << "Figure label: " << x.first << std::endl;
		std::cout << "Figure upper leftmost corner [row:column]: [" << x.second.row 
			<< ':'  << x.second.column << ']'<< std::endl;
		std::cout << "Figure square: " << x.second.square << std::endl;
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

		std::cout << "Centroid row: " << (float(sumOfRows) / sizeOfRows) << std::endl;
		std::cout << "Centroid column: " << (float(sumOfColumns) / sizeOfColumns) << std::endl;
		std::cout << std::endl;
	}
	std::cout << std::endl;
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


