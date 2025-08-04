
#include "src/shell/shell.h"
#include "src/screen/MemoryManager.h"
int main() {
    //Shell::start();
    MemoryManager::init(4096,16);
    //MemoryManager::getInstance().writeInMemory(0,"0x00", 16);
    int val = MemoryManager::getInstance().readInMemory(0,"0x00");
    std::cout<<val<< std::endl;
    std::cout << "FINISHED" << std::endl;
    return 0;
}