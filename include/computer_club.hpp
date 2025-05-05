#ifndef COMPUTER_CLUB_HPP
#define COMPUTER_CLUB_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stdexcept>

struct Time {
    int minutes;

    static Time fromString(const std::string& timeStr);
    std::string toString() const;
    bool operator<(const Time& other) const;
    bool operator<=(const Time& other) const;
    int operator-(const Time& other) const;
};

struct Event {
    Time time;
    int id;
    std::vector<std::string> args;

    static Event parse(const std::string& line);
};

class ComputerClub {
    friend class ComputerClubTest;
public:
    ComputerClub(int tables, Time start, Time end, int price);

    void processEvents(const std::vector<Event>& events);
    void generateClosingEvents();
    void printResults() const;
    int getTableRevenue(int tableNumber) const {
        if (tableNumber < 1 || tableNumber > tablesCount)
            throw std::out_of_range("Invalid table number");
        return tables[tableNumber].revenue;
    }
    Time getTableOccupiedSince(int table) const { return tables[table].occupiedSince; }
    bool isTableOccupied(int table) const { return tables[table].isOccupied; }
    const std::vector<std::string>& getOutput() const { return output; }

private:
    struct Table {
        int revenue = 0;
        Time occupiedSince;
        bool isOccupied = false;
    };

    int tablesCount;
    Time startTime;
    Time endTime;
    int pricePerHour;

    std::vector<Table> tables;
    std::map<std::string, int> clients;
    std::queue<std::string> waitingQueue;
    std::vector<std::string> output;

    void handleEvent(const Event& event);
    void clientArrived(const Time& time, const std::string& client);
    void clientSit(const Time& time, const std::string& client, int table);
    void clientWait(const Time& time, const std::string& client);
    void clientLeave(const Time& time, const std::string& client);

    void addError(const Time& time, const std::string& message);
    void freeTable(const Time& time, int table);
    int calculateRevenue(int minutes) const;
};

#endif // COMPUTER_CLUB_HPP