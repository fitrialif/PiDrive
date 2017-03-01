#pragma once
#include<vector>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include "FileIterator.h"
#include "Network.h"
#include <string>
#include <tchar.h>
#include <strsafe.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "normalize.h"
#include <cstdlib>
#include <cmath>
#include <experimental/filesystem>
#include <sstream>
//correctly add inertia=0; to fix inertia bug
using namespace std;
int main()
{
	string tts;
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		return 1;
	}
	
	srand(time(NULL));
	
	vector<vector<double>> ndandelions;//normalized dandelions
	vector<vector<double>> nroses;//normalized roses
	vector<vector<double>> ntulips;//normalized tulips
	vector<vector<double>> ndasies;//normalized dasies
	vector<vector<double>> nsunflowers;//normalized sunflowers
	
	sf::Text text;
	text.setFont(font);
	text.setFillColor(sf::Color(255,255,255,255));
	text.setPosition(0.0, 0.0);
	text.setCharacterSize(10);

/*
	vector<string>dandelions = fIterator("dandelion");
	vector<sf::Image>dandelionsI;
	vector<string>tulips = fIterator("tulips");
	vector<sf::Image>tulipsI;
	vector<string>roses = fIterator("roses");
	vector<sf::Image>rosesI;
	vector<string>sunflowers = fIterator("sunflowers");
	vector<sf::Image>sunflowersI;
	vector<string>dasies = fIterator("daisy");
	vector<sf::Image>dasiesI;


	int xcrop = 140; //normalize all images to 140x140px
	int ycrop = 140;

	//load each type of image
	//temporary image
	sf::Image tim;
	for (int i = 0; i < dandelions.size(); ++i) {//dandelions
		tim.loadFromFile(dandelions[i]);
		dandelionsI.push_back(tim);//load the image
		//put the image into the master database
		ndandelions.push_back(nm(tim, xcrop, ycrop));


	}

	//free up the memories
	vector<sf::Image>().swap(dandelionsI);
	vector<string>().swap(dandelions);

	for (int i = 0; i < roses.size(); ++i) {//roses
		tim.loadFromFile(roses[i]);
		rosesI.push_back(tim);//load the image
								   //put the image into the master database
		nroses.push_back(nm(tim, xcrop, ycrop));
	}
	//free up the memories
	vector<sf::Image>().swap(rosesI);
	vector<string>().swap(roses);


	for (int i = 0; i < tulips.size(); ++i) {//tulips
		tim.loadFromFile(tulips[i]);
		tulipsI.push_back(tim);//load the image
								   //put the image into the master database
		ntulips.push_back(nm(tim, xcrop, ycrop));
	}
	//free up the memories
	vector<sf::Image>().swap(tulipsI);
	vector<string>().swap(tulips);


	for (int i = 0; i < sunflowers.size(); ++i) {//sunflowers
		tim.loadFromFile(sunflowers[i]);
		sunflowersI.push_back(tim);//load the image
								   //put the image into the master database
		nsunflowers.push_back(nm(tim, xcrop, ycrop));
	}
	//free up the memories
	vector<sf::Image>().swap(sunflowersI);
	vector<string>().swap(sunflowers);
	

	for (int i = 0; i < dasies.size(); ++i) {//dasies
		tim.loadFromFile(dasies[i]);
		dasiesI.push_back(tim);//load the image
								   //put the image into the master database
		ndasies.push_back(nm(tim, xcrop, ycrop));
	}
	///
	stringstream ss;
	stringstream aa;
	string s;
	int abc = ndasies[0].size();//abc==19600
	int bcd = ndasies.size();//bcd==633

	ss <<abc;
	aa << bcd;
	s = ss.str()+"\n"+aa.str();

	///
	//free up the memories
	vector<sf::Image>().swap(dasiesI);
	vector<string>().swap(dasies);





	//setup
	vector<unsigned> topology;// makes a net with three input, two hidden and one out
	int epochs = 1;//SET EPOCHS HERE
					   //more setup
	topology.push_back(19600);//first layer neurons. Adjust for 0 based?
	//topology.push_back(9000);//hidden layer neurons--can have multiple hidden layers
	topology.push_back(50);
	topology.push_back(50);
	topology.push_back(5);//output layer neurons
						  ///test
	net mynet(topology);
	//PESUDO CODE

	//loop for X epochs
	//train net on all the dandelions
	//train net on all the roses
	//...

	//evaluate:
	//have a folder of images that it gets resultvals for

	////////////////////
	
	//training--loop these
	for (unsigned y = 0; y < epochs; ++y) {
		//dasies first (daisy=1)
		for (unsigned k = 0; k < ndasies.size(); ++k)//only do it for the amount of dasies in the folder HELPME
		{	
				mynet.feedforward(ndasies[k]);

				mynet.backprop({ 1.0, -1.0, -1.0, -1.0, -1.0 });//1 is dasies, -1 is nothing.
		}			
	}

	//using
	//mynet.getresults(resultvals);
*/

vector<unsigned> topology;// makes a net with three input, two hidden and one out
int epochs = 100000;//SET EPOCHS HERE
			   //more setup
topology.push_back(5);//first layer neurons. Adjust for 0 based?
						  //topology.push_back(9000);//hidden layer neurons--can have multiple hidden layers

topology.push_back(3);//output layer neurons
					  ///test
net mynet(topology);


//training--loop these
/*
for (unsigned y = 0; y < epochs; ++y) {
	//dasies first (daisy=1)

	mynet.feedforward({ 5, 4, 3, 2, 1 });

	mynet.backprop({0, 1, 0});//1 is dasies, -1 is nothing.
	
}*/
vector<double>grr;
mynet.loadWeights(grr);
mynet.saveWeights(grr);
//using
vector<double> results;
results.resize(3);
mynet.feedforward({ 5, 4, 3, 2, 1 });
mynet.getresults(results);
vector<double> scaledresults;
scaledresults.resize(3);
sf::Color cfill=sf::Color(sqrt(results[0]*results[0])*255, sqrt(results[1] * results[1]) * 255, sqrt(results[2] * results[2]) * 255, 255);
scaledresults = { sqrt(results[0] * results[0]) * 255, sqrt(results[1] * results[1]) * 255, sqrt(results[2] * results[2]) * 255 };

sf::Text blah;
sf::Font fonty;
fonty.loadFromFile("arial.ttf");
blah.setFont(fonty);
blah.setString(to_string(scaledresults[0])+","+ to_string(scaledresults[1]) + "," +		to_string(scaledresults[2]));
blah.setFillColor(sf::Color(0, 255, 255, 255));
blah.setPosition(200, 200);
	sf::RenderWindow window(sf::VideoMode(1500, 700), "Network template");
	sf::CircleShape netout;
	netout.setFillColor(cfill);
	netout.setPosition(400, 400);
	netout.setRadius(50);
	//text.setString(s);
	//window.draw(text);
	window.display();

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.draw(netout);
		window.draw(blah);
		window.display();
		window.clear();
		//text.setString("");

		
//		temp.clear();
		}
}
