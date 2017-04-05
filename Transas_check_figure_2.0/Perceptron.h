#pragma once
#include <vector>
#include <fstream>
#include <random>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>
class Neuron
{
private:
	int m_numberOfInputs;
	std::vector<float> m_weights;
	float m_learningSpeed;
	std::string m_weightsFileName;
	float m_idealOutput;
	int m_teachIterationNumber;
public:
	Neuron(std::string filename, int numberOfInputs = 2);
	~Neuron();
	void teach(std::vector<std::vector<float>> teachingSet);
	float analize(std::vector<float> data);
private:
	float calculateOutput(std::vector<float> currentInput);
	float activationFunction(float result);
	void saveWeights();
	void loadWeights();
	void initWeights();
};

class Perceptron
{
private:
	std::list<Neuron*> m_neuronList;
	int m_numberOfInputs;
public:
	Perceptron(int numberOfInputs = 4);
	void addNeuron(std::string weightsfilename);
	std::vector<float> analize(std::vector<float> inputVector);
	~Perceptron();
};