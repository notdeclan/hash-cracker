#include "pch.h"
#include "HashAlgorithm.h"


/// <summary>
/// Virtual function that is to be overloaded by hash functions that use this base class
/// </summary>
/// <param name="password">The password.</param>
/// <returns></returns>
std::string Algorithm::hash(std::string &password)
{
	return password;
}

/// <summary>
/// Clones this instance.
/// </summary>
/// <returns></returns>
Algorithm * Algorithm::clone()
{
	return new Algorithm();
}

/// <summary>
/// Gets the name of the hashing algorithm.
/// </summary>
/// <returns></returns>
std::string Algorithm::getName() const
{
	return algorithmName;
}

/// <summary>
/// Gets the length of the hash.
/// </summary>
/// <returns></returns>
int Algorithm::getLength() const
{
	return hashLength;
}

/// <summary>
/// Overloaded output stream operator used to print the algorithm name.
/// </summary>
/// <param name="output">The output.</param>
/// <param name="algorithm">The algorithm.</param>
/// <returns></returns>
std::ostream & operator<<(std::ostream & output, const Algorithm &algorithm)
{
	output << algorithm.algorithmName;
	return output; 
}