#include "stdafx.h"
#include "Perceptron.h"


Neuron::Neuron(std::string filename, int numberOfInputs)
{
	m_learningSpeed = 0.01f;
	m_weightsFileName = filename;
	m_numberOfInputs = numberOfInputs;
	m_idealOutput = 1.0;
	m_teachIterationNumber = 20000;
}


Neuron::~Neuron()
{
}

void Neuron::teach(std::vector<std::vector<float>> teachingSet)
{
	loadWeights();
	//initWeights();
	for (int i = 0; i < m_teachIterationNumber; i++)
	{
		std::random_shuffle(teachingSet.begin(), teachingSet.end(),
			[](int i) {
			using namespace std;
			random_device rd;
			mt19937 mt(rd());
			std::uniform_real_distribution<> dist(0, i);
			return dist(mt); });

		for (auto &currentVector : teachingSet)
		{
			float output = calculateOutput(currentVector);
			std::cout << output << std::endl;
			for (int i = 0; i < m_numberOfInputs; i++)
			{
				m_weights[i] -= m_learningSpeed * (output - m_idealOutput) * currentVector[i];
			}

		}
	}
	saveWeights();
}

float Neuron::analize(std::vector<float> data)
{
	float output = 0;
	loadWeights();
	output = calculateOutput(data);
	//std::cout << activationFunction(output) << std::endl;
	float percentage = (m_idealOutput - abs(output - m_idealOutput)) * 100;
	return percentage; 
}

float Neuron::calculateOutput(std::vector<float> currentInput)
{
	float sum = 0;
	for (int i = 0; i < m_numberOfInputs; i++)
	{
		sum += m_weights[i] * currentInput[i];
	}
	return sum;
}

float Neuron::activationFunction(float result)
{
	return (result / (1 + std::abs(-result)));
}

void Neuron::saveWeights()
{
	std::ofstream file;
	file.open(m_weightsFileName);
	for (auto weight : m_weights)
	{
		file << weight << ' ';
	}
	file << std::endl;
	file.close();
}

void Neuron::loadWeights()
{
	std::string line;
	std::ifstream file;
	file.open(m_weightsFileName);
	std::getline(file, line);
	std::istringstream in(line);
	for (int i = 0; i < m_numberOfInputs; i++)
	{
		float input;
		in >> input;
		m_weights.push_back(input);
	}
	file.close();
}

void Neuron::initWeights()
{
	using namespace std;
	random_device rd;
	mt19937 mt(rd());
	std::uniform_real_distribution<> dist(-(1 / std::sqrt(m_numberOfInputs)), (1 / std::sqrt(m_numberOfInputs)));
	std::ofstream file;
	file.open(m_weightsFileName);
	for (int i = 0; i < m_numberOfInputs; i++)
	{
		float num = (float)dist(mt);
		m_weights.push_back(num);
		file << num << ' ';
	}
	file << std::endl;
	file.close();
}

Perceptron::Perceptron(int numberOfInputs)
{
	m_numberOfInputs = numberOfInputs;
}

void Perceptron::addNeuron(std::string weightsfilename)
{
	m_neuronList.push_back(new Neuron(weightsfilename, m_numberOfInputs));
}

std::vector<float> Perceptron::analize(std::vector<float> inputVector)
{
	std::vector<float> outputVector;
	for (auto &neuron : m_neuronList)
	{
		outputVector.push_back(neuron->analize(inputVector));
	}
	return outputVector;
}

Perceptron::~Perceptron()
{
	for (auto &listNode : m_neuronList)
	{
		delete listNode;
	}
}
