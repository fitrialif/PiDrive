#pragma once
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include "Network.h"
#include <strsafe.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <tchar.h>
#include <cmath>
#include "normalize.h"

namespace fs = std::experimental::filesystem;
using namespace std;

vector<string> fIterator(string searchDirectory)
{
	stringstream names;
	vector<string> fNames;

	string temp;
	size_t oldsize=0;

	names.clear();
	for (fs::directory_entry p : fs::directory_iterator(searchDirectory)) {

		names << p;
		temp = names.str();


			temp.erase(temp.begin(), temp.begin()+oldsize);
		

		fNames.push_back(temp);

		oldsize += temp.size();
		names.clear();
	}


	return fNames;
}