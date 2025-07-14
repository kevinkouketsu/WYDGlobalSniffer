#include "Hooks.hpp"
#include "Structs.hpp"
#include <format>
#include <iostream>
#include <sstream>

template <>
struct std::formatter<Direction> : std::formatter<std::string_view> {
    auto format(Direction status, auto& ctx) const {
        std::string_view name = "Invalid";
        switch (status) {
        case Direction::Rx:      name = "Receive"; break;
        case Direction::Tx:   name = "Transmit"; break;
        }
        return std::formatter<std::string_view>::format(name, ctx);
    }
};

template <>
struct std::formatter<MSG_STANDARD> : std::formatter<std::string_view> {
    auto format(MSG_STANDARD msg, auto& ctx) const {
        auto t = std::format("size={:02X}, packet id={:03X}, client id={}", msg.Size, msg.Type, msg.ID);
        return std::formatter<std::string_view>::format(t, ctx);
    }
};

template <>
struct std::formatter<MSG_Action> : std::formatter<std::string_view> {
    auto format(MSG_Action msg, auto& ctx) const {
        auto t = std::format("{}x{}y, effect={}, speed={}, targetX={}, targetY={}", msg.PosX, msg.PosY, msg.Effect, msg.Speed, msg.TargetX, msg.TargetY);

        std::stringstream str;
        for (auto route : msg.Route)
            str << (int)route << ",";

        return std::formatter<std::string_view>::format(std::format("{}, route={}", t, str.str()), ctx);
    }
};

template <>
struct std::formatter<MSG_UseItem> : std::formatter<std::string_view> {
    auto format(MSG_UseItem msg, auto& ctx) const {
        auto t = std::format("sourceType={}, sourcePos={}, destType={}, destPos={}, gridX={}, gridY={}, itemId{}",
            msg.SourType, msg.SourPos, msg.DestType, msg.DestPos, msg.GridX, msg.GridY, msg.ItemID);

        return std::formatter<std::string_view>::format(t, ctx);
    }
};

static int MyClientId = 0;

struct STRUCT_ITEM
{
    unsigned short Index;
    struct
    {
        unsigned char Index;
        unsigned char Value;
    } Effect[3];

    unsigned char  Unknown[4];
};

struct p397
{
    MSG_STANDARD Header; // 0 - 11

    char Name[24]; // 12 - 35
    STRUCT_ITEM Item[12]; // 36 - 131

    char Slot[12]; // 132 - 143
    int Gold[12]; // 144 - 191

    unsigned short Tax;
    short Index; // 194 - 195
};

void HKD_OnPacketEvent(char* szBuffer, Direction direction)
{
    MSG_STANDARD* p = reinterpret_cast<MSG_STANDARD*>(szBuffer);

    if (direction == Direction::Rx)
    {
        if (p->Type == 0x114)
        {
            MyClientId = *(short*)&szBuffer[1386];
        }
    }
    else
    {
        if (p->Type == 0x36C)
        {
            auto action = reinterpret_cast<MSG_Action*>(szBuffer);
            std::cout << std::format("Action: {}\n", *action);
        }
        if (p->Type == 0x397)
        {
            auto autoTrade = reinterpret_cast<p397*>(szBuffer);

            std::cout << std::format("AutoTrade: {}", autoTrade->Name) << std::endl;;
        }
        if (p->Type == 0x373)
        {
            std::cout << std::format("UseItem: {}\n", *reinterpret_cast<MSG_UseItem*>(szBuffer)) << std::endl;;
        }
    }

    std::cout << std::format("direction={} {}\n", direction, *p);
}