#include "Memory.h"
#include "process.h"
#include <sstream>
#include <iomanip>

using namespace MemoryConfig;

std::vector<int> Memory::memory(TOTAL_FRAMES, -1);
int Memory::numProcesses = 0;

bool Memory::allocate(int pid) {
	int freeCount = 0;
	int start = -1;

	for (int i = 0; i < TOTAL_FRAMES; ++i) {
		if (memory[i] == -1) {
			if (freeCount == 0) start = i;
			freeCount++;
			if (freeCount == FRAMES_PER_PROC) {
				// Allocate frames
				for (int j = start; j < start + FRAMES_PER_PROC; ++j)
					memory[j] = pid;
				numProcesses++;
				return true;
			}
		} else {
			freeCount = 0;
		}
	}

	return false; // not enough contiguous space
}

void Memory::deallocate(int pid) {
	bool found = false;
	for (int& frame : memory) {
		if (frame == pid) {
			frame = -1;
			found = true;
		}
	}
	if (found) numProcesses--;
}

void Memory::printMemoryStatus(const std::string& timestamp) {
	std::ofstream out("memory_tick" + timestamp + ".txt", std::ios::app);

	out << "Timestamp: " << timestamp << '\n';
	out << "Processes in memory: " << numProcesses << '\n';
	out << "External Fragmentation: " << getExternalFragmentation() << " KB\n";

	for (int i = 0; i < TOTAL_FRAMES; ++i) {
		out << (memory[i] == -1 ? '.' : '*');
		if ((i + 1) % 64 == 0) out << '\n';
	}

	out << "\n----------------------------------------\n";
	out.close();
}

int Memory::getNumProcesses() {
	return numProcesses;
}

int Memory::getExternalFragmentation() {
	int freeBlock = 0, blocks = 0;

	for (int i = 0; i < TOTAL_FRAMES; ++i) {
		if (memory[i] == -1) {
			freeBlock++;
		} else if (freeBlock > 0) {
			if (freeBlock < FRAMES_PER_PROC) blocks++;
			freeBlock = 0;
		}
	}
	if (freeBlock > 0 && freeBlock < FRAMES_PER_PROC) blocks++;

	return blocks * MEM_PER_FRAME / 1024; // bytes
}
