#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>

#include "TreeMap.h"
#include "HashMap.h"

using namespace aisdi;

void performTests();



int main()
{
	bool test = true;
	while(test)
	{
		performTests();
		std::cout << "Once again? 0/1: ";
		std::cin >> test;
		std::cout << std::endl;
	}

    std::cout << "The end" << std::endl; 
	return 0;
}

void performTests()
{

	size_t size;

	std::cout << "size?: ";
	std::cin >> size;


	HashMap<unsigned int, std::string> testMap;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	
	for (unsigned int i = 0; i < size; ++i)
	{
		testMap[i] = "abc";
	}

	std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
	std::cout << "operator[] time  =  " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" <<std::endl;



	begin = std::chrono::steady_clock::now();
	
	for (unsigned int i = 0; i < size; ++i)
	{
		testMap.find(i);
	}

	end= std::chrono::steady_clock::now();
	std::cout << "find time  =  " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" <<std::endl;

	begin = std::chrono::steady_clock::now();
	
	for (unsigned int i = 0; i < size; ++i)
	{
		HashMap<unsigned int, std::string> testMap2 = testMap;
	}

	end= std::chrono::steady_clock::now();
	std::cout << "operator=   =  " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" <<std::endl;

	begin = std::chrono::steady_clock::now();
	
	for (unsigned int i = 0; i < size; ++i)
	{
		testMap.remove(i);
	}

	end= std::chrono::steady_clock::now();
	std::cout << "remove time  =  " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds" <<std::endl;






}
