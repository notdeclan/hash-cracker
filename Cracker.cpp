#include "pch.h"
#include "Cracker.h"

/// <summary>
/// Initializes a new instance of the <see cref="Cracker" /> class.
/// </summary>
/// <param name="maximumThreads">The maximum threads.</param>
/// <param name="algorithm">The hashing algorithm to be used</param>
Cracker::Cracker(int threadCount, Algorithm *algorithm) : threadCount(threadCount) {
	this->algorithm = algorithm;
	this->threadPool = std::vector<std::thread>(threadCount);
	this->updateRate = 0.10F; // update every 10%;
	this->totalPercentage = threadCount * 100;
	return;
}

/// <summary>
/// Finalizes an instance of the <see cref="Cracker" /> class.
/// </summary>
Cracker::~Cracker()
{
	delete algorithm;
	return;
}

/// <summary>
/// Gets the current system timestamp.
/// </summary>
/// <returns>
/// std::string containing timestamp
/// </returns>
std::string Cracker::getTimestamp() {
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
	char timestamp[100];
	struct tm buf;
	errno_t err = localtime_s(&buf, &currentTime);
	std::strftime(timestamp, sizeof(timestamp), "%d/%m/%y %H:%M:%S", &buf);

	return timestamp;
}

/// <summary>
/// Starts the cracker threads
/// </summary>
/// <param name="hash">The hash to be cracked</param>
/// <returns>
/// string containing cracked hash if found
/// </returns>
std::string Cracker::start(std::string hash)
{
	if (complete) {
		throw new std::invalid_argument("Instance of cracker can only be used once, please create another before starting again");
		return std::string();
	}
	
	std::unique_lock<std::mutex> lck(dictionaryMutex); 
	while (dictionary.size() <= 0) { // wait until the dictionary has been loaded
		std::cout << "Please load dictionary before calling run() method";
		cv.wait(lck); 
	}
	
	std::cout << std::left << std::setw(20) << "Time Started: " << std::setw(4) << getTimestamp() << std::endl;
	std::cout << std::left << std::setw(20) << "Threads: " << std::setw(4) << threadCount << std::endl;
	std::cout << std::left << std::setw(20) << "Algorithm: " << std::setw(4) << *algorithm << std::endl;
	std::cout << std::left << std::setw(20) << "Hash: " << std::setw(4) << hash << std::endl;

	std::transform(hash.begin(), hash.end(), hash.begin(), ::tolower); // transform hash to lowercase because hashes are just hex encoded strings essentially (so 0..9 a..z)
	progressBar("Hashes Completed:", 0, totalPercentage); // create progress bar

	for (unsigned int threadID = 0, startIndex = 0; threadID < threadCount; threadID++) // for each thread
	{
		// math to to account for the fact some threads will be slightly larger depending on how many threads/dictionary items
		unsigned int endIndex = startIndex + (dictionary.size() / threadCount) + (threadID < dictionary.size() % threadCount ? 1 : 0);
		addWorker([=] {
			// create a smaller version of the dictionary instead of passing the whole dictionary because its way faster because threads have different heaps that have to be loaded with the vector (slow)
			compareHashes(sliceVector(dictionary, startIndex, endIndex), hash); // start cracking
		});
		startIndex = endIndex; // update start index for the next thread
	}

	for (auto&& thread : threadPool) // loop through all the threads in pool
		if (thread.joinable()) // if there's a thread joinable
			thread.join(); // join it
	
	return cracked;
}

/// <summary>
/// Thread function which is used to hash the passwords in the dictionary slice and compare them to the hash
/// </summary>
/// <param name="dictionarySlice">vector containing passwords to hash</param>
/// <param name="hash">The hash to crack</param>
void Cracker::compareHashes(std::vector<std::string>dictionarySlice, std::string hash)
{
	Algorithm* algorithmClone = algorithm->clone(); // create a seperate instance of the algorithm for every thread to make it more thread safe
	std::string password; // variable to store the current password being checked
	int progress = 0;
	int updatePercentage = dictionarySlice.size() * updateRate; 

	for (int index = 0; index < dictionarySlice.size() && !complete; index++) { // loop through each item in the dictionary while complete is false
		progress++;
		if (progress == updatePercentage) {  // if current progress is equal to the update rate
			totalProgress += updateRate * 100; // update total progress			
			if (progessBarMutex.try_lock()) { // only update if the mutex is free, otherwise dont bother
				progressBar("Hashes Completed:", totalProgress, totalPercentage); // update progress bar
				progessBarMutex.unlock();
			}
			progress = 0; // reset progress until next update
		}

		password = dictionarySlice[index]; // update string
		if (algorithmClone->hash(password) == hash) { // compare hashes
			cracked = password; // set cracked
			complete = true; // update atomic
			break; // break loop
		}
	}

	delete algorithmClone; // remove clone from heap
	return;
}

/// <summary>
/// Prints a progress bar to the console.
/// </summary>
/// <param name="name">The name to print in the progress bar</param>
/// <param name="value">The current value</param>
/// <param name="totalValue">The total value</param>
void Cracker::progressBar(const std::string name, const unsigned int value, const unsigned int totalValue)
{
	if ((value != totalValue) && (value % (totalValue / 100 + 1) != 0)) return; // if value is greater than total value

	float ratio = value / (float)totalValue;
	int c = ratio * 50;
	std::cout << std::left << std::setw(20) << name << std::setw(3) << (int)(ratio * 100) << "% [";
	for (int x = 0; x < c; x++) {
		std::cout << "#";
	}
	for (int x = c; x < 50; x++) {
		std::cout << " ";
	}
	std::cout << "]\r" << std::flush;
	return;
}

/// <summary>
/// Function is used to load a dictionary file into memory.
/// </summary>
/// <param name="filePath">The file path.</param>
/// <returns>
/// True if any passwords are loaded,  false if failed to open the file or no passwords found in file
/// </returns>
bool Cracker::loadDictionary(const std::string dictionaryPath)
{
	std::ifstream input(dictionaryPath, std::ios::in); // open file
	std::ifstream::pos_type fileSize;
	if (!input.is_open()) { // if file could not be opened 
		std::cout << "Failed to open dictionary file: " << dictionaryPath << std::endl;
		input = std::ifstream();
		return false; 
	}
	else {
		fileSize = std::ifstream(dictionaryPath, std::ifstream::ate | std::ifstream::binary).tellg(); // set file size variable
		std::cout << std::left << std::setw(20) << "Dictionary: " << std::setw(4) << dictionaryPath << " (" << fileSize / 1048576 << "MB)" << std::endl;
	}

	std::string line;
	while (getline(input, line)) { // for each line in file
		if (line != "") { // if it's not empty
			dictionary.emplace_back(line); // add to dictionary vector
		}
		progressBar("Loading Dictionary:", (input.tellg() / 100) + 1, fileSize / 100); // update progress bar
	}

	input.close(); // close file
	std::cout << std::endl << std::endl; // add new line after progress bar
	
	std::unique_lock<std::mutex> lck(dictionaryMutex); // unlock start function thread
	cv.notify_all();

	return dictionary.size() > 0;
}


/// <summary>
/// Function is used to create a smaller sized vector based on a start and an end index
/// </summary>
/// <param name="dictionary">The vector to slice.</param>
/// <param name="start">The start.</param>
/// <param name="end">The end.</param>
/// <returns>
/// Sliced vector
/// </returns>
std::vector<std::string> Cracker::sliceVector(std::vector<std::string> const dictionary, const int startIndex, int endIndex)
{
	auto first = dictionary.cbegin() + startIndex;
	auto last = dictionary.cbegin() + endIndex;
	return std::vector<std::string>(first, last);
}

/// <summary>
/// Function is used to add a new worker function to the thread pool
/// </summary>
/// <param name="job">The job.</param>
template<typename Worker> void Cracker::addWorker(Worker&& function)
{
	// find an ended thread
	for (auto&& thread : threadPool)
	{
		if (thread.joinable()) // still running or waiting to join	
			continue; // goto next thread

		thread = std::thread(function); // start the function in this thread
		return;
	}
	
	// if not wait for one
	for (auto&& thread : threadPool)
	{
		if (!thread.joinable()) // dead thread (not run or already joined)
			continue; // goto next thread

		thread.join(); // rejoin the thread
		thread = std::thread(function); // start the function in this thread
		return;
	}
	return;
}