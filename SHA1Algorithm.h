#pragma once

/// <summary>
/// Class is based on base class Algorithm and is used to create SHA1 Hashes
/// </summary>
/// <seealso cref="Algorithm" />
class SHA1Algorithm : public Algorithm
{
public:
	SHA1Algorithm();
	~SHA1Algorithm();
	virtual std::string hash(std::string &password);
	virtual Algorithm *clone();
private:
	std::mutex mtx;
	SHA_CTX context;
	unsigned char digest[20];
	char *output = (char*) malloc(41);
};
