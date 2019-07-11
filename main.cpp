// Hash Cracker.cpp : This file contains the 'main' function. Program execution begins and ends here

#include "pch.h"
#include <iostream>


int main(int argc, char** argv) {
	std::string hash = "";
	std::string dictionaryPath;
	unsigned int threadCount;
	Algorithm *algorithm = NULL;

	while (algorithm == NULL) {
		std::cout << "Algorithm: " << std::endl;
		std::cout << "MD5 (1), SHA1 (2), SHA256 (3)" << std::endl << "> ";
		std::string algorithmString;
		std::cin >> algorithmString;
		try {
			switch (stoi(algorithmString)) { // parse string to int and then compare values
			case 1:
				algorithm = new MD5Algorithm();
				continue;
			case 2:
				algorithm = new SHA1Algorithm();
				continue;
			case 3:
				algorithm = new SHA256Algorithm();
				continue;
			default:
				break;
			}
		}
		catch (std::invalid_argument e) { // will throw if the user inputs something over than a number 
			// just continue
		}

		std::cout << "Please enter a valid Algorithm ID" << std::endl;
		continue;
	}

	while (hash == "") {
		std::cout << "Hash: " << std::endl << "> ";
		std::string temp_hash;
		std::cin >> temp_hash;
		if (temp_hash.length() != algorithm->getLength()) { // check that the hash is the correct length
			std::cout << "Please enter a correct " << algorithm->getName() << " hash" << std::endl;
			continue;
		}
		// TODO: if the user enters in a hash with the same length as another algorithm offer the suggestion of changing algorithm
		hash = temp_hash;
	}

	std::cout << "Dictionary Path: " << std::endl << "> ";
	std::cin >> dictionaryPath;

	std::cout << "Thread Count: " << std::endl << "> ";
	std::cin >> threadCount;
	system("CLS");

	auto startTime = std::chrono::high_resolution_clock::now();
	Cracker cracker(threadCount, algorithm); // create new cracker instance

	if (cracker.loadDictionary(dictionaryPath)) {
		auto loadDictionaryFinish = std::chrono::high_resolution_clock::now();
		std::string password = cracker.start(hash);
		if (password.size() > 0) {
			std::cout << std::endl << std::endl << std::left << std::setw(20) << "Hash Found: " << std::setw(4) << password << std::endl << std::endl;
		}
		else {
			std::cout << std::endl << std::endl << "Hash not found in Dictionary" << std::endl << std::endl;
		}

		auto finishTime = std::chrono::high_resolution_clock::now();

		std::cout << std::left << std::setw(20) << "Dictionary Loading: " << std::setw(4) << std::chrono::duration_cast<std::chrono::milliseconds>(loadDictionaryFinish - startTime).count() << "ms" << std::endl;
		std::cout << std::left << std::setw(20) << "Hashing: " << std::setw(4) << std::chrono::duration_cast<std::chrono::milliseconds>(finishTime - loadDictionaryFinish).count() << "ms" << std::endl;
		std::cout << std::left << std::setw(20) << "Total Time: " << std::setw(4) << std::chrono::duration_cast<std::chrono::milliseconds>(finishTime - startTime).count() << "ms" << std::endl;
	}
	while (true) {
		// just to keep console window open at the end of execution
	}
	return 0;
}
