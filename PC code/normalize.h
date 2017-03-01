#pragma once
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include "Network.h"
#include <tchar.h>
#include <strsafe.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <cmath>
#include "FileIterator.h"

using namespace std;

vector<double> nm(sf::Image im, int xcrop, int ycrop)//takes in an image, grayscales it, and returns a vector of grayscale values of a certain size. 
{//									currently, I am normalizing to 140x140px. //xcrop and ycrop are the parameters to crop the image. it will crop from upper left
	vector<double> normal;
	for (int i = 0; i < xcrop; ++i) {//these go through and grayscale the necessary pixels. save the values
		for (int j = 0; j < ycrop; ++j) {
			normal.push_back((im.getPixel(i, j).r+ im.getPixel(i, j).g+ im.getPixel(i, j).b)/3);
		}
	}
	//go through and crop. save the values. I think it already did this



	return normal;
}