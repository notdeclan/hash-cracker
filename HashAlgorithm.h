#pragma once
class  Algorithm {
public:
	virtual ~Algorithm() = default;
	virtual Algorithm* clone();
	virtual std::string hash(std::string &password);

	std::string getName() const;
	int getLength() const;
	friend std::ostream &operator<<(std::ostream &output, const Algorithm &D);

protected:
	std::string algorithmName;
	int hashLength;
};