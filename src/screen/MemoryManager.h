//
// Created by Sean on 04/08/2025.
//

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#include <mutex>
#include <shared_mutex>

#include "Pages.h"


class MemoryManager {
    struct Frame {
        int tickEntered;
        std::unique_ptr<Pages> page;

        Frame(int tick = 0)
            : tickEntered(tick), page(std::make_unique<Pages>(-1, -1, 1)) {}
    };
    static MemoryManager* instance;
    int maxMemory;
    int memPerFrame;
    int totalFrames;
    mutable std::shared_mutex memlock;
    std::vector<Frame> frames;
    std::string filename = "csopesy_backing_store.txt";
    MemoryManager(int maxMemory, int memPerFrame) : maxMemory(maxMemory), memPerFrame(memPerFrame)
    {
        totalFrames = maxMemory/memPerFrame;
        frames = std::vector<Frame>(totalFrames);
        for (int i = 0; i < totalFrames; i++)
        {
            frames[i].tickEntered = 0;
            //frames[i].page = std::make_unique<Pages>(i,-1,memPerFrame);
            frames[i].page = nullptr;
        }
    }
    public:
    static void init(int maxMemory, int memPerFrame) {
        if (!instance) {
            instance = new MemoryManager(maxMemory, memPerFrame);
        }
    }
    static void reset() {
        delete instance;
        instance = nullptr;
    }

    static MemoryManager& getInstance() {
        if (!instance) {
            throw std::runtime_error("MemoryManager not initialized. Call init() first.");
        }
        return *instance;
    }

    void evictPage(Pages pages);
    int loadPage(int processid, int pageid);
    std::pair<int, int> parseAddress(std::string address);
    std::string vectorToHex(const std::vector<uint8_t>& data) const;
    std::vector<uint8_t> hexToVector(const std::string& hex) const;
    std::string createEntry(int process_id, int page_id, const std::vector<uint8_t>& data) const;
    void addPage(int process_id, int page_id, const std::vector<uint8_t>& data);
    bool removePage(int process_id, int page_id);
    bool removeProcess(int process_id);
    std::vector<uint8_t> getPage(int process_id, int page_id) const;
    bool hasPage(int process_id, int page_id) const;
    std::vector<std::string> readAllEntries() const;
    void writeAllEntries(const std::vector<std::string>& entries) const;
    bool parseEntry(const std::string& entry, int& process_id, int& page_id, std::vector<uint8_t>& data) const;
    int findVictimFrame();
    void printMemory();
    Pages* getPageInMemory(int processid, int pageid);
    int allocatePage(Pages pages);
    int findPageinMemory(int processid, int pageid);
    void emptyFrame(int framenum);
    void writeInMemory(int processid, std::string address, uint16_t value);
    uint16_t readInMemory(int processid, std::string address);
    void deallocateProcess(int processid);
    int handlePageFault(int processid, int pageid, std::string address);

    // Deleted copy constructor and assignment operator to prevent duplication
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;
};



#endif //MEMORYMANAGER_H
