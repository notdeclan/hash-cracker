#pragma once

/// <summary>
/// Class is based on base class Algorithm and is used to create SHA256 Hashes
/// </summary>
/// <seealso cref="Algorithm" />
class SHA256Algorithm : public Algorithm
{
public:
	SHA256Algorithm();
	~SHA256Algorithm();
	 
	virtual std::string hash(std::string &password);
	virtual Algorithm* clone();
private:
	std::mutex mtx;
	SHA256_CTX context;
	unsigned char digest[32];
	char *output = (char*)malloc(65);
};
