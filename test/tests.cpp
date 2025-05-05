#include "computer_club.hpp"
#include <gtest/gtest.h>

TEST(TimeTest, ParseValidTime) {
    EXPECT_EQ(Time::fromString("09:00").minutes, 540);
    EXPECT_EQ(Time::fromString("23:59").minutes, 1439);
}

TEST(TimeTest, ParseInvalidTime) {
    EXPECT_THROW(Time::fromString("9:00"), std::invalid_argument);
    EXPECT_THROW(Time::fromString("09-00"), std::invalid_argument);
    EXPECT_THROW(Time::fromString("25:00"), std::invalid_argument);
}

TEST(EventTest, ParseBasicEvent) {
    Event e = Event::parse("08:48 1 client1");
    EXPECT_EQ(e.time.minutes, 528);
    EXPECT_EQ(e.id, 1);
    ASSERT_EQ(e.args.size(), 1);
    EXPECT_EQ(e.args[0], "client1");
}

class ComputerClubTest : public ::testing::Test {
protected:
    void SetUp() override {
        club = new ComputerClub(3, Time::fromString("09:00"),
            Time::fromString("19:00"), 10);
    }

    void TearDown() override {
        delete club;
    }

    ComputerClub* club;
};

TEST_F(ComputerClubTest, ClientArrivalBeforeOpen) {
    club->processEvents({ Event::parse("08:48 1 client1") });
    const auto& output = club->getOutput();
    ASSERT_GE(output.size(), 2);
    EXPECT_EQ(output[1], "08:48 1 client1");
    EXPECT_EQ(output[2], "08:48 13 NotOpenYet");
}

TEST_F(ComputerClubTest, ClientDoubleArrival) {
    club->processEvents({
        Event::parse("09:01 1 client1"),
        Event::parse("09:02 1 client1")
        });
    const auto& output = club->getOutput();
    ASSERT_GE(output.size(), 4);
    EXPECT_EQ(output[3], "09:02 13 YouShallNotPass");
}

TEST_F(ComputerClubTest, ClientSitAtBusyTable) {
    club->processEvents({
        Event::parse("09:01 1 client1"),
        Event::parse("09:02 2 client1 1"),
        Event::parse("09:03 1 client2"),
        Event::parse("09:04 2 client2 1") 
        });

    const auto& output = club->getOutput();
    bool found = std::any_of(output.begin(), output.end(),
        [](const std::string& s) { return s.find("PlaceIsBusy") != std::string::npos; });

    EXPECT_TRUE(found) << "Expected error 'PlaceIsBusy' not found in output";
}

TEST_F(ComputerClubTest, FullWorkflow) {
    club->processEvents({
        Event::parse("09:41 1 client1"),
        Event::parse("09:48 1 client2"),
        Event::parse("09:54 2 client1 1"),
        Event::parse("10:25 2 client2 2"),
        Event::parse("12:33 4 client1")
        });

    EXPECT_EQ(club->getTableRevenue(1), 30);
}

TEST_F(ComputerClubTest, ClosingTimeRevenue) {
    club->processEvents({ Event::parse("09:01 1 client1") });
    club->processEvents({ Event::parse("09:02 2 client1 1") });
    club->generateClosingEvents();

    EXPECT_EQ(club->getTableRevenue(1), 100);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}