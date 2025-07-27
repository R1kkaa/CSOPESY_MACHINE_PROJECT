#include "Memory.h"

const int MAX_MEMORY= 16384;
std::vector<int> MemoryArray(MAX_MEMORY, -1); //creates MAX_MEMORY amount of elements, with -1 as the init value
const int MEM_PER_FRAME = 16; //idk where to put this
const int MEM_PER_PROC = 4096;// Memory per frame in bytes
int ext_frag = 0;
int numProcesses = 0;

Memory process;

//Initialization
//First Fit

void allocate_memory() {
	int freeCount = 0;
	//check for a free space
	for (int i: MemoryArray) {
		//check if it is free
		if (MemoryArray[i] != -1) {
			freeCount = 0;
		}
		else if (MemoryArray[i] == -1) {
			freeCount++; //start counting the free memory blocks
			if (freeCount == MEM_PER_PROC) {
				int start = i;
				int end = i + freeCount;
				//TODO: insert the process id here
			}
		}
	}
}

// has syntax error, but im trying to add the logic here
void deallocate_memory() {
	for (int i : MemoryArray) {
		if (MemoryArray[i].getProcess == process) {
			MemoryArray[i] = -1;
		}
	}
}


void print() {
    std::ofstream memoryReport("memory_stamp.txt", std::ios::trunc);

    if (!memoryReport.is_open()) {
        std::cerr << "Error: Could not open file for writing!" << std::endl;
        return;
    }

    memoryReport << "Timestamp: " + process.executionTime() << std::endl;

    memoryReport.close();
}

