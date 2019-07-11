#pragma once
class Cracker
{
public:

	Cracker(const int maximumThreads, Algorithm *algorithm);
	~Cracker();
	
	std::string start(std::string hash);
	bool loadDictionary(const std::string filePath);

private:
	
	std::vector<std::string> sliceVector(std::vector<std::string> const dictionary, const int start, const int end);
	
	void compareHashes(std::vector<std::string>dictionarySlice, std::string hash);
	
	void progressBar(const std::string name, const unsigned int value, const unsigned int totalValue);
	
	std::string getTimestamp();
	
	template<typename Worker> void addWorker(Worker &&job);

	/// <summary>
	/// Int used to store the thread count used by the <see cref="Cracker" /> instance
	/// </summary>
	const int threadCount;

	/// <summary>
	/// int used to store the progress of hashing
	/// </summary>
	int totalProgress = 0;
	
	/// <summary>
	/// int used to store the final progress value
	/// </summary>
	int totalPercentage;
	
	/// <summary>
	/// float used to store the rate of how fast the progress bar updates during hashing
	// 0.1 == update every 10%, 0.9 == update every 0.90%, etc.
	/// </summary>
	float updateRate;

	/// <summary>
	/// Vector is used to store the thread pool
	/// </summary>
	std::vector<std::thread> threadPool;

	/// <summary>
	/// Atomic bool is used to stop all the running hashing threads when the password has been found
	/// </summary>
	std::atomic<bool> complete = false;

	/// <summary>
	/// Mutex is used to ensure that multiple progress bars cannot be wrote to the console at once while cracking
	/// </summary>
	std::mutex progessBarMutex;


	/// <summary>
	/// Mutex and Condtional variable to ensure that the hashing threads do not run until there is strings loaded into the dictionary vector
	/// </summary>
	std::mutex dictionaryMutex;
	std::condition_variable cv;

	/// <summary>
	/// Vector is used to store the strings loaded from the dictionary file to be used while cracking
	/// </summary>
	std::vector<std::string> dictionary;
	
	/// <summary>
	/// Pointer is used to store the algorithm used while cracking
	/// </summary>
	Algorithm *algorithm;
	

	/// <summary>
	/// String is used to store the cracked password when completed
	/// </summary>
	std::string cracked;
	
};

