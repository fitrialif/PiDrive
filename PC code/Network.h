#pragma once
#include<vector>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include "FileIterator.h"
#include <string>
#include <cassert>
#include <fstream>
#include <experimental/filesystem>
#include <sstream>
#include "normalize.h"
#include <tchar.h>
#include <strsafe.h>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <cmath>
#include <fstream>
//WARNING, THIS USES TANH ACTIVATION FUNCTION, WHICH MEANS THAT THE OUTPUT IS FROM -1 TO 1
using namespace std;

double weightToWrite=0;
double weightToLoad = 0;

struct Connection
{
	double weight;
	double deltaweight;
};

class Neuron;

typedef vector<Neuron> Layer;


// ****************** class Neuron ******************

class Neuron
{
public:
	Neuron(unsigned numOutputs, unsigned myindex);
	void setoutputval(double val) {
		outputval = val;
	}
	double getoutputval(void) const { 
		return outputval;
	}
	void feedforward(const Layer &prevlayer);

	void calcoutputgradients(double targetval);
	void calchiddengradients(const Layer &nextlayer);
	void updateinputweights(Layer &prevlayer);
	vector<Connection> outputweights;
private:
	static double eta;
	static double alpha;
	static double transferfunction(double x);
	static double transferfunctionderivative(double x);
	double sumDOW(const Layer &nextlayer) const;
	double outputval;

	static double randomWeight(void) {
		srand(time(NULL));
		return rand() / double(RAND_MAX);
	}
	unsigned m_myindex;
	double gradient;
};

double Neuron::eta = 0.15;//overall net learning rate
double Neuron::alpha = 0.5;//multiplier of last deltaweight

void Neuron::updateinputweights(Layer &prevlayer)
{
	for (unsigned n = 0; n < prevlayer.size(); ++n) {
		Neuron &neuron = prevlayer[n];
		double olddeltaweight = neuron.outputweights[m_myindex].deltaweight;


		//LEARNING RATE IS eta
			//0.0 IS SLOW LEARNER, 0.2 IS MEDIUM, 1.0 IS RECKLESS
		//MOMENTUM(?!) IS ALPHA
			//0.0 IS NO MOMENTUM, 0.5 IS MODERATE MOMENTUM
		double newdeltaweight =
			eta
			* neuron.getoutputval()
			* gradient
			+ alpha
			* olddeltaweight;

			neuron.outputweights[m_myindex].deltaweight = newdeltaweight;
			neuron.outputweights[m_myindex].weight += newdeltaweight;

	}
}

double Neuron::sumDOW(const Layer &nextlayer) const
{
	double sum = 0.0;

	// Sum our contributions of the errors at the nodes we feed.

	for (unsigned n = 0; n < nextlayer.size() - 1; ++n) {
		sum += outputweights[n].weight * nextlayer[n].gradient;
	}

	return sum;
}


void Neuron::calchiddengradients(const Layer &nextlayer)
{
	double dow = sumDOW(nextlayer);
	gradient = dow * Neuron::transferfunctionderivative(outputval);
}

void Neuron::calcoutputgradients(double targetval)
{
	double delta = targetval - outputval;
	gradient = delta * Neuron::transferfunctionderivative(outputval);
	
}

double Neuron::transferfunction(double x) {
	return tanh(x);
}

double Neuron::transferfunctionderivative(double x) {
	//return 1.0 - x*x;
	return (1 / (cosh(x)*cosh(x)));
}

void Neuron::feedforward(const Layer &prevlayer) {
	double sum = 0.0;

	for (unsigned n = 0; n < prevlayer.size(); ++n) {
		sum += prevlayer[n].getoutputval() *
			prevlayer[n].outputweights[m_myindex].weight;
	}
	outputval = Neuron::transferfunction(sum);
}



Neuron::Neuron(unsigned numOutputs, unsigned myindex)
{
	for (unsigned c = 0; c < numOutputs; ++c) {
		outputweights.push_back(Connection());
		outputweights.back().weight = randomWeight();
	}
	m_myindex = myindex;
}


// ****************** class Net ******************

class net{
public:
	net(const vector<unsigned> &topology);
	void feedforward(const vector<double> &inputvals);
	void backprop(const vector<double> &targetvals);
	void saveWeights(const vector<double> &targetvals);
	void loadWeights(vector<double> &targetvals);
	void getresults(vector<double> &resultvals) const;
	double getrecentaverageerror(void) const { return recentaverageerror; }
private:
	vector<Layer> layers; //layers[layernum[neuronnum]
	double error;
	double recentaverageerror;
	static double recentaveragesmoothingfactor;
};

double net::recentaveragesmoothingfactor = 100.0; // Number of training samples to average over

void net::getresults(vector<double> &resultvals) const
{
	resultvals.clear();

	for (unsigned n = 0; n < layers.back().size() - 1; ++n) {
		resultvals.push_back(layers.back()[n].getoutputval());
	}
}

void net::backprop(const vector<double> &targetvals)
{
	Layer &outputLayer = layers.back();//getting the overall net error (root mean squared error of output neurons)
	error = 0.0;

	for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
		double delta = targetvals[n] - outputLayer[n].getoutputval();
		error += delta * delta;

	}

	error /= outputLayer.size() - 1;//get average error squared
	error = sqrt(error);

	//see how its been doing over the past few generations
	recentaverageerror =
		(recentaverageerror * recentaveragesmoothingfactor + error)
		/ (recentaveragesmoothingfactor + 1.0);

	for (unsigned n = 0; n < outputLayer.size() - 1; ++n)//calculate output layer gradients-?!
	{
		outputLayer[n].calcoutputgradients(targetvals[n]);
	}

	for (unsigned layernum = layers.size() - 2; layernum > 0; --layernum) {//calculate gradients on hidden layers- ?!
		Layer &hiddenlayer = layers[layernum];
		Layer &nextlayer = layers[layernum + 1];
		
		for (unsigned n = 0; n < hiddenlayer.size(); ++n)
		{
			hiddenlayer[n].calchiddengradients(nextlayer);
		}
	}
	// For all layers from outputs to first hidden layer,
	// update connection weights
	for (unsigned layernum = layers.size() - 1; layernum > 0; --layernum) {//update the connection weights in all the layers. USE THIS TO MAKE SAVE/LOADWEIGHTS
		Layer &layer = layers[layernum];
		Layer &prevlayer = layers[layernum - 1];

		for (unsigned n = 0; n < layer.size() - 1; ++n) {
			layer[n].updateinputweights(prevlayer);
			
		}
	}

}
///////////////
void net::saveWeights(const vector<double> &targetvals) {
	ofstream saved;
	saved.open("weights.txt");
	for (unsigned layernum = 0; layernum < layers.size(); ++layernum) {//update the connection weights in all the layers. USE THIS TO MAKE SAVE/LOADWEIGHTS


		for (unsigned n = 0; n < layers[layernum].size() - 1; ++n) {
			for (int i = 0; i < layers[layernum][n].outputweights.size(); ++i) {
				saved << layers[layernum][n].outputweights[i].weight << endl;
			}


		}
	}
	saved.close();
}
void net::loadWeights(vector<double> &targetvals) {
	fstream loaded;
	loaded.open("weights.txt");
	for (unsigned layernum = 0; layernum < layers.size(); ++layernum) {//update the connection weights in all the layers. USE THIS TO MAKE SAVE/LOADWEIGHTS

		for (unsigned n = 0; n < layers[layernum].size() - 1; ++n) {
			for (int i = 0; i < layers[layernum][n].outputweights.size(); ++i) {
				loaded>>layers[layernum][n].outputweights[i].weight;
			}


		}
	}
	loaded.close();
}
///////////////////////

void net::feedforward(const vector<double> &inputvals)
{

	assert(inputvals.size() == layers[0].size() - 1);

	for (unsigned i = 0; i < inputvals.size(); ++i)//taking care of input values
	{
		layers[0][i].setoutputval(inputvals[i]);
	}
	for (unsigned layernum = 1; layernum < layers.size(); ++layernum) {// forward propagation
		Layer &prevlayer = layers[layernum - 1];

		for (unsigned n = 0; n < layers[layernum].size() - 1; ++n)
		{
			layers[layernum][n].feedforward(prevlayer);
		}
	}

}


net::net(const vector<unsigned> &topology) {
	unsigned numlayers = topology.size();
	for (unsigned layernum = 0; layernum < numlayers; ++layernum) {// the problem was here. I didn't write ++layernum, I only wrote layernum
		layers.push_back(Layer());
		unsigned numOutputs = layernum == topology.size() - 1 ? 0 : topology[layernum + 1];


		for (unsigned neuronnum = 0; neuronnum <= topology[layernum]; ++neuronnum) {
			layers.back().push_back(Neuron(numOutputs, neuronnum));
		}

		//set the bais node's output to 1.0;
		layers.back().back().setoutputval(1.0);
	}
}