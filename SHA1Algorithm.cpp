#include "pch.h"
#include "SHA1Algorithm.h"


/// <summary>
/// Initializes a new instance of the <see cref="SHA1Algorithm"/> class.
/// </summary>
SHA1Algorithm::SHA1Algorithm()
{
	this->algorithmName = "SHA1";
	this->hashLength = 40;
}

/// <summary>
/// Returns SHA1 hashed version of param password
/// </summary>
/// <param name="password">The password.</param>
/// <returns>Hashed password string</returns>
std::string SHA1Algorithm::hash(std::string &password) {
	mtx.lock();
	SHA1_Init(&context);
	SHA1_Update(&context, password.c_str(), password.length());
	SHA1_Final(digest, &context);

	for (int n = 0; n < 20; ++n) {
		snprintf(&(output[n * 2]), 20 * 2, "%02x", (unsigned int)digest[n]); // express the digest in hex 
	}
	mtx.unlock();
	return output;
}

/// <summary>
/// Clones this instance.
/// </summary>
/// <returns></returns>
Algorithm * SHA1Algorithm::clone()
{
	return new SHA1Algorithm();
}

/// <summary>
/// Finalizes an instance of the <see cref="SHA1Algorithm"/> class.
/// </summary>
SHA1Algorithm::~SHA1Algorithm()
{
	delete output;
}