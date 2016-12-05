
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
	int r, c;
	int currentNum = 0;
	for (r = 0; r < MATRIX_RESOLUTION; r++)
	{
		for (c = 0; c < MATRIX_RESOLUTION; c++)
		{
			if ((*m_image)[r][c] == 0)
			{
				continue;
			}
			else if((*m_image)[r-1][c] == 0 && (*m_image)[r][c+1] == 0)
			{
				currentNum++;
				(*m_image)[r][c] = currentNum;
				m_figurePoints->push_back(std::pair<int, int>(r, c));
			}
			else if()

		}
	}
}
