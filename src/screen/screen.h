//
// Created by Sean on 14/05/2025.
//

#ifndef SCREEN_H
#define SCREEN_H
#include <string>
#include <utility>
#include <chrono>
#include <ctime>

static int counter = 0;

class Screen {
    private:
        std::string name;
        int id;
        std::chrono::system_clock::time_point creationTime;

    public:
    // Constructor
        explicit Screen(std::string name) : name(std::move(name))
        {
            id=++counter;
            creationTime = std::chrono::system_clock::now();
        }
        std::string getname() const;
        void setname(std::string name);
        [[nodiscard]] int getID() const;
        [[nodiscard]] std::string displayTimestamp() const;
};



#endif //SCREEN_H
