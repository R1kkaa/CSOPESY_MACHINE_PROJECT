//
// Created by Sean on 04/08/2025.
//

//TODO:: ADD MEMORY MUTEX TO EVERY FUNCTION
#include "MemoryManager.h"

#include <fstream>
#include <iostream>

static int PAGE_OUT = 0;
static int PAGE_IN = 0;
MemoryManager* MemoryManager::instance = nullptr;
//Find Victim through FIFO
int MemoryManager::findVictimFrame(){
    int tickEntered = frames.at(0).tickEntered;
    int currFrame = 0;
    for (int i = 0; i < frames.size(); i++)
    {
        if (frames.at(i).tickEntered < tickEntered)
        {
            tickEntered = frames.at(i).tickEntered;
            currFrame = i;
        }
    }
    return currFrame;
}

Pages* MemoryManager::getPageInMemory(int processid, int pageid)
{
    auto index = findPageinMemory(processid, pageid);
    if (index == -1)
        return nullptr;
    else
        return &*frames.at(index).page;

}
//check for empty frames, if found then allocate page,
// else find a victim frame to replace
int MemoryManager::allocatePage(Pages pages)
{
    for (int i = 0; i < frames.size(); i++)
    {
        if (nullptr == frames.at(i).page)
        {
            frames.at(i).page = std::make_unique<Pages>(pages);
            return i;
        }
    }
    //find frame to replace
    int replaceposition = findVictimFrame();
    //place frame in backing store
    evictPage(*frames.at(replaceposition).page);
    //put new page in frame
    frames.at(replaceposition).page = std::make_unique<Pages>(pages);
    return replaceposition;
}

//find if page exists in memory
int MemoryManager::findPageinMemory(int processid, int pageid)
{
    for (int i = 0; i < frames.size(); i++)
    {
        //if page exists in frame
        if (frames.at(i).page != nullptr)
        {
            int existingpid = frames.at(i).page->getProcessId();
            int existingpageid = frames.at(i).page->getPageId();
            if (existingpid == processid && existingpageid == pageid)
                return i;
        }
    }
    return -1;
}
//remove page from frame
void MemoryManager::emptyFrame(int framenum)
{
    frames.at(framenum).page = nullptr;
}

//find page and write in memory
void MemoryManager::writeInMemory(int processid, std::string address, uint16_t value)
{
auto [pageid, offset] = parseAddress(address);
    int index = findPageinMemory(processid, pageid);
    //not in memory
    if (index == -1)
    {
        index = handlePageFault(processid, pageid, address);
    }
    frames.at(index).page->writeUint16(offset, value);
}

//find page and read memory
uint16_t MemoryManager::readInMemory(int processid, std::string address)
{
    auto [pageid, offset] = parseAddress(address);
    int index = findPageinMemory(processid, pageid);
    //not in memory
    if (index == -1)
    {
        index = handlePageFault(processid, pageid, address);
    }
    return frames.at(index).page->readUint16(offset);
}

int MemoryManager::handlePageFault(int processid, int pageid, std::string address)
{
    //check in backing store
    //if page is in backing store
    if (hasPage(processid, pageid))
    {
        //load page from backing store and place in memory
        return loadPage(processid, pageid);
    }
    //if page is not in backing store
        //create new page
        auto page = Pages(pageid, processid, memPerFrame);
        //allocate page to memory
        return allocatePage(page);
}
//TODO:: Store Page into Backing Store
void MemoryManager::evictPage(Pages pages)
{
    //add to backing store
    addPage(pages.getProcessId(), pages.getPageId(), pages.getData());
    PAGE_OUT++;
}

//TODO:: Load Page from Backing Store to Memory
int MemoryManager::loadPage(int processid, int pageid)
{
    //create placeholder object
    auto page = Pages(pageid,processid,memPerFrame);
    //retrieve from backing store
    page.setData(getPage(processid,pageid));
    //allocate into the memory
    int index = allocatePage(page);
    PAGE_IN++;
    return index;
}

//TODO:: Parse Address - returns page id
std::pair<int, int> MemoryManager::parseAddress(std::string address) {
    int value = std::stoi(address, nullptr, 16);
    int pageid = value / memPerFrame;
    int offset = value % memPerFrame;
    return std::make_pair(pageid, offset);
}

// Convert vector<uint8_t> to hex string
std::string MemoryManager::vectorToHex(const std::vector<uint8_t>& data) const {
    std::stringstream ss;
    for (uint8_t byte : data) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
    }
    return ss.str();
}

// Convert hex string to vector<uint8_t>
std::vector<uint8_t> MemoryManager::hexToVector(const std::string& hex) const {
    std::vector<uint8_t> data;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::strtol(byteString.c_str(), nullptr, 16));
        data.push_back(byte);
    }
    return data;
}

// Create entry string
std::string MemoryManager::createEntry(int process_id, int page_id, const std::vector<uint8_t>& data) const {
    return std::to_string(process_id) + "," + std::to_string(page_id) + "," + vectorToHex(data);
}

    // Add or update a page
    void MemoryManager::addPage(int process_id, int page_id, const std::vector<uint8_t>& data) {
        std::vector<std::string> entries = readAllEntries();
        std::string new_entry = createEntry(process_id, page_id, data);
        bool found = false;

        // Look for existing entry to update
        for (std::string& entry : entries) {
            int existing_pid, existing_page_id;
            std::vector<uint8_t> existing_data;

            if (parseEntry(entry, existing_pid, existing_page_id, existing_data)) {
                if (existing_pid == process_id && existing_page_id == page_id) {
                    entry = new_entry;
                    found = true;
                    break;
                }
            }
        }

        // If not found, add new entry
        if (!found) {
            entries.push_back(new_entry);
        }

        writeAllEntries(entries);
    }

    // Remove a specific page
    bool MemoryManager::removePage(int process_id, int page_id) {
        std::vector<std::string> entries = readAllEntries();
        std::vector<std::string> filtered_entries;
        bool found = false;

        for (const std::string& entry : entries) {
            int existing_pid, existing_page_id;
            std::vector<uint8_t> existing_data;

            if (parseEntry(entry, existing_pid, existing_page_id, existing_data)) {
                if (existing_pid == process_id && existing_page_id == page_id) {
                    found = true;
                    // Skip this entry (don't add to filtered)
                    continue;
                }
            }
            filtered_entries.push_back(entry);
        }

        if (found) {
            writeAllEntries(filtered_entries);
        }

        return found;
    }

    // Remove all pages for a process
    bool MemoryManager::removeProcess(int process_id) {
        std::vector<std::string> entries = readAllEntries();
        std::vector<std::string> filtered_entries;
        bool found = false;

        for (const std::string& entry : entries) {
            int existing_pid, existing_page_id;
            std::vector<uint8_t> existing_data;

            if (parseEntry(entry, existing_pid, existing_page_id, existing_data)) {
                if (existing_pid == process_id) {
                    found = true;
                    // Skip this entry (don't add to filtered)
                    continue;
                }
            }
            filtered_entries.push_back(entry);
        }

        if (found) {
            writeAllEntries(filtered_entries);
        }

        return found;
    }

    // Get a page (returns empty vector if not found)
    std::vector<uint8_t> MemoryManager::getPage(int process_id, int page_id) const {
        std::vector<std::string> entries = readAllEntries();

        for (const std::string& entry : entries) {
            int existing_pid, existing_page_id;
            std::vector<uint8_t> existing_data;

            if (parseEntry(entry, existing_pid, existing_page_id, existing_data)) {
                if (existing_pid == process_id && existing_page_id == page_id) {
                    return existing_data;
                }
            }
        }

        return std::vector<uint8_t>(); // Empty vector if not found
    }

    // Check if a page exists
    bool MemoryManager::hasPage(int process_id, int page_id) const {
        std::vector<std::string> entries = readAllEntries();

        for (const std::string& entry : entries) {
            int existing_pid, existing_page_id;
            std::vector<uint8_t> existing_data;

            if (parseEntry(entry, existing_pid, existing_page_id, existing_data)) {
                if (existing_pid == process_id && existing_page_id == page_id) {
                    return true;
                }
            }
        }

        return false;
    }
// Read all entries from file
std::vector<std::string> MemoryManager::readAllEntries() const {
    std::vector<std::string> entries;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            entries.push_back(line);
        }
    }

    return entries;
}

// Write all entries to file
void MemoryManager::writeAllEntries(const std::vector<std::string>& entries) const {
    std::ofstream file(filename);
    for (const std::string& entry : entries) {
        file << entry << "\n";
    }
}

// Parse entry format: "process_id,page_id,hex_data"
bool MemoryManager::parseEntry(const std::string& entry, int& process_id, int& page_id, std::vector<uint8_t>& data) const {
    std::istringstream iss(entry);
    std::string token;

    // Get process_id
    if (!std::getline(iss, token, ',')) return false;
    process_id = std::stoi(token);

    // Get page_id
    if (!std::getline(iss, token, ',')) return false;
    page_id = std::stoi(token);

    // Get hex data
    if (!std::getline(iss, token)) return false;
    data = hexToVector(token);
    return true;
}