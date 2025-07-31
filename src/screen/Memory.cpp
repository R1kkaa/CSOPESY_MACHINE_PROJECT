#include "Memory.h"

//const int MAX_MEMORY= 16384;
//const int MEM_PER_FRAME = 16; // we divide this for array and make searching faster; we multiply MEM_PER_FRAME when printing
//const int MEM_PER_PROC = 4096;// Memory per frame in bytes
////const int TOTAL_FRAMES = MAX_MEMORY / MEM_PER_FRAME;
int TOTAL_FRAMES;
int ext_frag = 0;
Memory::Memory() {

}

Memory::Memory(int maxOverallMem, int memPerFrame, int memPerProc)
{
	this->maxOverallMem = maxOverallMem;
	this->memPerFrame = memPerFrame;
	this->memPerProc = memPerProc;
	TOTAL_FRAMES = maxOverallMem / memPerFrame;
	MemoryArray = std::make_shared<std::vector<int>>(TOTAL_FRAMES, -1);
}

int Memory::isSufficient() {
	int freeCount = 0;
	int start = -1;
	for (int i = 0; i < MemoryArray->size(); ++i) {
		if (MemoryArray->at(i) != -1) {
			freeCount = 0;  // reset count
		}
		else {
			if (freeCount == 0) {
				start = i;  // mark potential start index correctly
			}
			freeCount++;

			if (freeCount == (memPerProc / memPerFrame)) {
				return freeCount;  // found enough space
			}
		}
	}
	return -1;  // no space found
}

void Memory::allocate_memory(int currProcess, int start) {
	bool inMemory = Memory::isInMemory(currProcess); //check if currProcess exists inside the memory before allocating the memory
	if (!inMemory && start >= 0) {
		for (int i = start; i < start + (memPerProc/memPerFrame); ++i) { //populate the memory with the id number of the process
			MemoryArray->at(i) = currProcess;
		}
		numProcesses++;
	}
}

void Memory::deallocate_memory(int currProcess) {
	bool found = false;
	for (int& frame : *MemoryArray) {
		if (frame == currProcess) {
			frame = -1;
			found = true;
		}
	}
	if (found && numProcesses > 0)
		numProcesses--;
}

//if process is inside memory already
bool Memory::isInMemory(int currProcess) {
	for (int& frame : *MemoryArray) {
		if (frame == currProcess) {
			return 1;
		}
	}
	return 0;
}

void Memory::printMemoryStatus(const std::string& timequantum) {
	int tempID = 0, start = 0, end = 0;
	std::ofstream memoryReport("memory_tick" + timequantum + ".txt", std::ios::app);

	if (!memoryReport.is_open()) {
		std::cerr << "Error: Could not open file for writing!" << std::endl;
		return;
	}
	memoryReport << "Timestamp: " << currprocess.displayTimestamp() << '\n';
	memoryReport << "Processes in memory: " << numProcesses << '\n';
	memoryReport << "External Fragmentation: " << getExternalFragmentation() * memPerFrame << " KB\n\n";
	memoryReport << "----end---- = : " << maxOverallMem << "\n\n";

	for (int i = 0; i < TOTAL_FRAMES; ++i) {
		if ((*MemoryArray)[i] != -1) {
			//set current processid to temp
			tempID = MemoryArray->at(i);
			start = i;
			while(i < TOTAL_FRAMES && MemoryArray->at(i) == tempID) {
				end = i;
				i++;
			}


			//set upper limit
			int upperLimit = (end+1) * memPerFrame; //ex: 3 * 16 = 48 bytes	
			//set lower limit
			int lowerLimit = start * memPerFrame; //ex: 2 * 16 = 32 bytes

			//print report
			memoryReport << upperLimit << '\n'; //ex: 2 * 16 = 32 bytes
			memoryReport << "P" << tempID << '\n';
			memoryReport << lowerLimit << '\n'; //ex: 2 * 16 = 32 bytes
		}
	}
	memoryReport << "----start---- = : 0 \n\n";

	memoryReport << "\n----------------------------------------\n";
	memoryReport.close();
}

int Memory::getExternalFragmentation() {
	ext_frag = 0;
	for (int i = 0; i < MemoryArray->size(); ++i) {
		if ((*MemoryArray)[i] == -1) {
			ext_frag++;
		}
	}
	return ext_frag;
}


std::string Memory::displayTimestamp() const {
	const std::time_t time = std::chrono::system_clock::to_time_t(creationTimeMemory);
	std::tm* timeInfo = std::localtime(&time); // Convert to local time
	std::ostringstream oss;
	oss << std::put_time(timeInfo, "(%m/%d/%Y, %I:%M:%S %p)");
	return oss.str();
}

void Memory::checkArray() {
	for (int i : *MemoryArray) {
		std::cout << i << std::endl;
	}
}

int Memory::getFrameSize() const {
	return memPerFrame;
}

void Memory::setFrameSize(int memPerFrame){
	this->memPerFrame = memPerFrame;
}

int Memory::getMemorySize() const {
	return maxOverallMem;
}

void Memory::setMemorySize(int maxOverallMem) {
	this->maxOverallMem = maxOverallMem;
}

int Memory::getProcSize() const {
	return memPerProc;
}

void Memory::setProcSize(int memPerProc) {
	this->memPerProc = memPerProc;
}

int Memory::getNumProcesses() const {
	return numProcesses;
}
std::shared_ptr<std::vector<int>> Memory::getMemoryArray() {
	return MemoryArray;
}

