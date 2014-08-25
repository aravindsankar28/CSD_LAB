#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include "Cache.h"
using namespace std;

int main(int argc, char **argv) {
	//std::cout << "Hello, world!" << std::endl;

	std::ifstream input("/home/aravind/CacheSimulator/config.txt");
	std::string line;
	int levels;
	int i = 0;
	std::getline(input,line);
	std::string::size_type sz; 
	int x =  strtol(line.substr(9).c_str(),0,10);
	levels = x; // Found levels
	std::getline(input,line); // should get \n 
	//Cache **c = new Cache(10,10,10,10,10);
	for (int i = 0; i < levels; ++i)
	{
		// level i
		std::getline(input,line);
		

		std::getline(input,line);
		std::string size_str = line.substr(7);
		int size = strtol(size_str.substr(0,(size_str.size()-2)).c_str(),0,10);
		cout << size<< endl;

		std::getline(input,line);
		int assoc = strtol(line.substr(16).c_str(),0,10);
		cout << assoc <<endl;

		std::getline(input,line);
		std::string block_size_str = line.substr(13);
		int block_size = strtol(block_size_str.substr(0,block_size_str.size()-2).c_str(),0,10);
		cout << block_size << endl;

		std::getline(input,line);
		int hit_latency = strtol(line.substr(14).c_str(),0,10);
		cout << hit_latency <<endl;

		std::getline(input,line);
		
		std::string policy = line.substr(21);
		cout << policy << endl;

		std::getline(input,line); // \n 
		
	}

	
	std::getline(input,line);
	std::getline(input,line);
	cout << line.substr(14);

	return 0;
}
