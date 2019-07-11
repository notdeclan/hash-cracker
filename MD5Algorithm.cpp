#include "pch.h"
#include "MD5Algorithm.h"


/// <summary>
/// Initializes a new instance of the <see cref="MD5Algorithm"/> class.
/// </summary>
MD5Algorithm::MD5Algorithm()
{
	this->algorithmName = "MD5";
	this->hashLength = 32;
}

/// <summary>
/// Finalizes an instance of the <see cref="MD5Algorithm"/> class.
/// </summary>
MD5Algorithm::~MD5Algorithm()
{
	delete output;
}

/// <summary>
/// Returns MD5 hashes version of param password
/// </summary>
/// <param name="password">The password.</param>
/// <returns>Hashed password string</returns>
std::string MD5Algorithm::hash(std::string &password)
{
	mtx.lock(); // lock mutex
	MD5_Init(&context); 
	MD5_Update(&context, password.c_str(), password.length());
	MD5_Final(digest, &context);

	for (int n = 0; n < 16; ++n) {
		snprintf(&(output[n * 2]), 16 * 2, "%02x", (unsigned int)digest[n]); // express the digest in hex 
	}

	mtx.unlock();
	return output;
}

/// <summary>
/// Clones this instance.
/// </summary>
/// <returns></returns>
Algorithm * MD5Algorithm::clone()
{
	return new MD5Algorithm();
}
