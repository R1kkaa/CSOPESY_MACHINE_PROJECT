//
// Created by Sean on 04/08/2025.
//

#ifndef PAGES_H
#define PAGES_H
#include <vector>
#include <chrono>
#include <stdexcept>


class Pages {
    int pageId;
    int processId;
    std::vector<uint8_t> data;
    int pageSize;
    bool isDirty;
    bool isValid;

public:
    Pages(int pageId, int processId, int pageSize)
        : pageId(pageId), processId(processId), pageSize(pageSize),
          isDirty(false), isValid(true) {
        data.resize(pageSize, 0);
    }

    // Getters
    int getPageId() const { return pageId; }
    int getProcessId() const { return processId; }
    const std::vector<uint8_t>& getData() const { return data; }
    std::vector<uint8_t>& getData() { return data; }
    bool getDirty() const { return isDirty; }
    bool getValid() const { return isValid; }
    int getPageSize() const { return pageSize; }

    // Setters
    void setDirty(bool dirty) { isDirty = dirty; }
    void setValid(bool valid) { isValid = valid; }
    void setData(std::vector<uint8_t> data)
    {
        pageSize = data.size();
        this->data = std::move(data);
    }
    void resize(int pageSize)
    {
        this->pageSize = pageSize;
        data.resize(pageSize, 0);
    }
    // Read uint16 value from specific offset in page
    uint16_t readUint16(int offset) {
        if (offset + 2 > static_cast<int>(data.size())) {
            offset--;
        }
        return (static_cast<uint16_t>(data[offset]) << 8) | static_cast<uint16_t>(data[offset + 1]);
    }

    // Write uint16 value to specific offset in page
    void writeUint16(int offset, uint16_t value) {
        if (offset + 2 > static_cast<int>(data.size())) {
            offset--;
        }

        data[offset] = static_cast<uint8_t>((value >> 8) & 0xFF);
        data[offset + 1] = static_cast<uint8_t>(value & 0xFF);
        setDirty(true);
    }

    // Copy page data for backing store operations
    /*
    void copyDataFrom(const std::vector<uint8_t>& sourceData) {
        if (sourceData.size() != data.size()) {
            throw std::invalid_argument("Source data size mismatch");
        }
        data = sourceData;
        setDirty(false);
    }
    */
};



#endif //PAGES_H
