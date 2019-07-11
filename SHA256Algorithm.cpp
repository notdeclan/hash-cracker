#include "pch.h"
#include "SHA256Algorithm.h"



/// <summary>
/// Initializes a new instance of the <see cref="SHA256Algorithm"/> class.
/// </summary>
SHA256Algorithm::SHA256Algorithm()
{
	this->algorithmName = "SHA256";
	this->hashLength = 64;
}

/// <summary>
/// Finalizes an instance of the <see cref="SHA256Algorithm"/> class.
/// </summary>
SHA256Algorithm::~SHA256Algorithm()
{
	delete output;
}

/// <summary>
/// Returns SHA256 hashed version of param password
/// </summary>
/// <param name="password">The password.</param>
/// <returns>Hashed password string</returns>
std::string SHA256Algorithm::hash(std::string &password)
{
	mtx.lock();
	SHA256_Init(&context);
	SHA256_Update(&context, password.c_str(), password.length());
	SHA256_Final(digest, &context);

	for (int n = 0; n < 32; ++n) {
		snprintf(&(output[n * 2]), 32 * 2, "%02x", (unsigned int)digest[n]); // express the digest in hex 
	}
	mtx.unlock(); 

	return output;
}

/// <summary>
/// Clones this instance.
/// </summary>
/// <returns></returns>
Algorithm * SHA256Algorithm::clone()
{
	return new SHA256Algorithm();
}

