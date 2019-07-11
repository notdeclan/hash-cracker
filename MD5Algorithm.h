#pragma once

/// <summary>
/// Class is based on base class Algorithm and is used to create MD5 Hashes
/// </summary>
/// <seealso cref="Algorithm" />
class MD5Algorithm : public Algorithm
{
public:
	MD5Algorithm();
	~MD5Algorithm();

	virtual std::string hash(std::string &password);
	virtual Algorithm* clone();
private:
	std::mutex mtx;
	MD5_CTX context;
	unsigned char digest[16];
	char *output = (char*)malloc(33);
};