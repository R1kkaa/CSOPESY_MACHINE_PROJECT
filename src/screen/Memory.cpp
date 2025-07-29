#include "Memory.h"

//const int MAX_MEMORY= 16384;
//const int MEM_PER_FRAME = 16; // we divide this for array and make searching faster; we multiply MEM_PER_FRAME when printing
//const int MEM_PER_PROC = 4096;// Memory per frame in bytes
////const int TOTAL_FRAMES = MAX_MEMORY / MEM_PER_FRAME;
int TOTAL_FRAMES;
int ext_frag = 0;
int numProcesses = 0;

Memory::Memory() {

}

Memory::Memory(int maxOverallMem, int memPerFrame, int memPerProc)
	: maxOverallMem(maxOverallMem), memPerFrame(memPerFrame), memPerProc(memPerProc)
{
	TOTAL_FRAMES = maxOverallMem / memPerFrame;
	MemoryArray = std::make_shared<std::vector<int>>(TOTAL_FRAMES, -1);
}

int Memory::isSufficient() {
	int freeCount = 0;
	int start = -1;

	for (int i : *(MemoryArray)) {
		if (i != -1) {
			freeCount = 0;  // reset count
		}
		else if (i == -1) {
			if (freeCount == 0) {
				start = i;  // mark potential start index
			}
			freeCount++;

			if (freeCount == TOTAL_FRAMES) {
				return start;  // found enough space
			}
		}
	}
	return -1;  // no space found
}

void Memory::allocate_memory(int currProcess, int start) {
	int inMemory = Memory::checkExisting(currProcess); //check if currProcess exists inside the memory before allocating the memory
	if (inMemory == 1) {
		int start = Memory::isSufficient();

		for (int i = start; i <= start + memPerProc; ++i) { //populate the memory with the id number of the process
			(*MemoryArray)[i] = currProcess;
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
	if (found)  
		if (numProcesses > 0) {
			numProcesses--;
		}
}

//if P1 is inside memory already
int Memory::checkExisting(int currProcess) {
	bool found = false;
	for (int& frame : *MemoryArray) {
		if (frame == currProcess) {
			frame = -1;
			return 1;
		}
	}
	return 0;
}

void Memory::printMemoryStatus(const std::string& timequantum) {
	std::ofstream memoryReport("memory_tick" + timequantum + ".txt", std::ios::app);

	if (!memoryReport.is_open()) {
		std::cerr << "Error: Could not open file for writing!" << std::endl;
		return;
	}
	memoryReport << "Timestamp: " << timequantum << '\n';
	memoryReport << "Processes in memory: " << numProcesses << '\n';
	memoryReport << "External Fragmentation: " << getExternalFragmentation() << " KB\n\n";
	memoryReport << "----end---- = : " << maxOverallMem << "\n\n";

	for (int i = 0; i < maxOverallMem; ++i) {
		if ((*MemoryArray)[i] != -1) {
			memoryReport << (*MemoryArray)[i] << " - " << i << '\n';
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

int Memory::getFrameSize() const {
	return memPerFrame;
}

int Memory::getMemorySize() const {
	return maxOverallMem;
}