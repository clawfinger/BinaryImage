
#include "stdafx.h"
#include "CFigureRecognition.h"

using std::vector;


CFigureRecognition::CFigureRecognition(int resolution)
{
	m_image = new vector<vector<int>>(MATRIX_RESOLUTION, vector<int>(MATRIX_RESOLUTION));
	m_figurePoints = new std::vector<std::pair<int, int>>;
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
	for (int r = 1; r < MATRIX_RESOLUTION; r++)
	{
		for (int c = 1; c < MATRIX_RESOLUTION; c++)
		{
			int B, C;
			B = (*m_image)[r][c - 1];
			C = (*m_image)[r - 1][c];

			if ((*m_image)[r][c] == 0)
			{
				continue;
			}
			else
			{

				if (B == 0 && C == 0)
				{
					(*m_image)[r][c] = currentNum;
					m_parentArray[currentNum] = currentNum;
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
	for (int r = 1; r < MATRIX_RESOLUTION; r++)
	{
		for (int c = 1; c < MATRIX_RESOLUTION; c++)
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
				//m_figurePoints->push_back(std::pair<int, int>(r, c));
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

