#include "computer_club.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    try {
        std::string line;
        if (!std::getline(file, line)) throw std::runtime_error("Empty file");
        int tables = std::stoi(line);
        if (tables <= 0) throw std::runtime_error("Invalid number of tables");

        if (!std::getline(file, line)) throw std::runtime_error("Missing working hours");
        std::istringstream timeStream(line);
        std::string startStr, endStr;
        if (!(timeStream >> startStr >> endStr)) throw std::runtime_error("Invalid time format");
        Time startTime = Time::fromString(startStr);
        Time endTime = Time::fromString(endStr);

        if (!std::getline(file, line)) throw std::runtime_error("Missing price per hour");
        int price = std::stoi(line);
        if (price <= 0) throw std::runtime_error("Invalid price");

        std::vector<Event> events;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            events.push_back(Event::parse(line));
        }

        ComputerClub club(tables, startTime, endTime, price);
        club.processEvents(events);
        club.printResults();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}