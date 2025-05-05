#include "computer_club.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <numeric>

Time Time::fromString(const std::string& timeStr) {
    if (timeStr.size() != 5 || timeStr[2] != ':')
        throw std::invalid_argument("Invalid time format");

    int hours = std::stoi(timeStr.substr(0, 2));
    int minutes = std::stoi(timeStr.substr(3, 2));

    if (hours < 0 || hours >= 24 || minutes < 0 || minutes >= 60)
        throw std::invalid_argument("Invalid time value");

    return { hours * 60 + minutes };
}

std::string Time::toString() const {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes / 60 << ":"
        << std::setw(2) << std::setfill('0') << minutes % 60;
    return oss.str();
}

bool Time::operator<(const Time& other) const { return minutes < other.minutes; }
bool Time::operator<=(const Time& other) const { return minutes <= other.minutes; }
int Time::operator-(const Time& other) const { return minutes - other.minutes; }

Event Event::parse(const std::string& line) {
    std::istringstream iss(line);
    std::string timeStr, idStr;
    iss >> timeStr >> idStr;

    Event event;
    event.time = Time::fromString(timeStr);
    event.id = std::stoi(idStr);

    std::string arg;
    while (iss >> arg) event.args.push_back(arg);
    return event;
}

ComputerClub::ComputerClub(int tables, Time start, Time end, int price)
    : tablesCount(tables), startTime(start), endTime(end), pricePerHour(price) {
    this->tables.resize(tablesCount + 1);
}

void ComputerClub::processEvents(const std::vector<Event>& events) {
    output.push_back(startTime.toString());

    for (const auto& event : events) {
        handleEvent(event);
    }

    generateClosingEvents();
    output.push_back(endTime.toString());
}

void ComputerClub::handleEvent(const Event& event) {
    output.push_back(event.time.toString() + " " + std::to_string(event.id) +
        (event.args.empty() ? "" : " " + std::accumulate(
            std::next(event.args.begin()), event.args.end(), event.args[0],
            [](std::string a, std::string b) { return a + " " + b; })));

    try {
        switch (event.id) {
        case 1: clientArrived(event.time, event.args[0]); break;
        case 2: clientSit(event.time, event.args[0], std::stoi(event.args[1])); break;
        case 3: clientWait(event.time, event.args[0]); break;
        case 4: clientLeave(event.time, event.args[0]); break;
        default: throw std::invalid_argument("Unknown event ID");
        }
    }
    catch (const std::exception& e) {
        addError(event.time, e.what());
    }
}

void ComputerClub::clientArrived(const Time& time, const std::string& client) {
    if (time < startTime || endTime <= time)
        throw std::runtime_error("NotOpenYet");
    if (clients.count(client))
        throw std::runtime_error("YouShallNotPass");

    clients[client] = -1; // -1 means not seated
}

void ComputerClub::clientSit(const Time& time, const std::string& client, int table) {
    if (!clients.count(client)) {
        throw std::runtime_error("ClientUnknown");
    }

    if (table < 1 || table > tablesCount) {
        throw std::runtime_error("Invalid table number");
    }
    if (tables[table].isOccupied) {
        throw std::runtime_error("PlaceIsBusy");
    }

    int prevTable = clients[client];
    if (prevTable != -1) {
        freeTable(time, prevTable);
    }

    tables[table].isOccupied = true;
    tables[table].occupiedSince = time;
    clients[client] = table;
}

void ComputerClub::clientWait(const Time& time, const std::string& client) {
    if (!clients.count(client)) {
        throw std::runtime_error("ClientUnknown");
    }

    bool hasFreeTables = false;
    for (int i = 1; i <= tablesCount; ++i) {
        if (!tables[i].isOccupied) {
            hasFreeTables = true;
            break;
        }
    }
    if (hasFreeTables) {
        throw std::runtime_error("ICanWaitNoLonger!");
    }

    if (waitingQueue.size() >= tablesCount) {
        clients.erase(client);
        output.push_back(time.toString() + " 11 " + client);
        return;
    }

    waitingQueue.push(client);
    clients[client] = -2; // special code for waiting clients
}

void ComputerClub::clientLeave(const Time& time, const std::string& client) {
    if (!clients.count(client)) {
        throw std::runtime_error("ClientUnknown");
    }

    int table = clients[client];
    if (table != -1 && table != -2) {
        freeTable(time, table);
    }

    clients.erase(client);

    if (table >= 1 && table <= tablesCount && !waitingQueue.empty()) {
        std::string nextClient = waitingQueue.front();
        waitingQueue.pop();
        clientSit(time, nextClient, table);
        output.push_back(time.toString() + " 12 " + nextClient + " " + std::to_string(table));
    }
}

void ComputerClub::freeTable(const Time& time, int table) {
    if (table < 1 || table > tablesCount || !tables[table].isOccupied) return;

    int minutesOccupied = time - tables[table].occupiedSince;
    tables[table].revenue += calculateRevenue(minutesOccupied);
    tables[table].isOccupied = false;
}

int ComputerClub::calculateRevenue(int minutes) const {
    int hours = (minutes + 59) / 60;
    return hours * pricePerHour;
}

void ComputerClub::generateClosingEvents() {
    std::vector<std::string> remainingClients;
    for (const auto& [client, table] : clients) {
        remainingClients.push_back(client);
        if (table != -1) freeTable(endTime, table);
    }

    std::sort(remainingClients.begin(), remainingClients.end());
    for (const auto& client : remainingClients) {
        output.push_back(endTime.toString() + " 11 " + client);
    }
}

void ComputerClub::printResults() const {
    for (const auto& line : output) {
        std::cout << line << std::endl;
    }

    for (int i = 1; i <= tablesCount; ++i) {
        const auto& table = tables[i];
        int totalMinutes = (table.revenue / pricePerHour) * 60;
        Time totalTime{ totalMinutes };
        std::cout << i << " " << table.revenue << " " << totalTime.toString() << std::endl;
    }
}

void ComputerClub::addError(const Time& time, const std::string& message) {
    output.push_back(time.toString() + " 13 " + message);
}