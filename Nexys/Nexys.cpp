#include "Nexys.hpp"

#include <iostream>

static uint8_t sevSegToHex(uint8_t Digit)
{
    uint8_t Digit_Hex = 0;

    switch (Digit)
    {
        case 0x01:
            Digit_Hex = 0;
            break;
        case 0x4f:
            Digit_Hex = 1;
            break;
        case 0x12:
            Digit_Hex = 2;
            break;
        case 0x06:
            Digit_Hex = 3;
            break;
        case 0x4c:
            Digit_Hex = 4;
            break;
        case 0x24:
            Digit_Hex = 5;
            break;
        case 0x20:
            Digit_Hex = 6;
            break;
        case 0x0f:
            Digit_Hex = 7;
            break;
        case 0x00:
            Digit_Hex = 8;
            break;
        case 0x04:
            Digit_Hex = 9;
            break;
        case 0x08:
            Digit_Hex = 10;
            break;
        case 0x60:
            Digit_Hex = 11;
            break;
        case 0x31:
            Digit_Hex = 12;
            break;
        case 0x42:
            Digit_Hex = 13;
            break;
        case 0x30:
            Digit_Hex = 14;
            break;
        case 0x38:
            Digit_Hex = 15;
            break;
        default:
            break;
    }

    return Digit_Hex;
}

static uint8_t anodeToIdx(uint8_t AN)
{
    switch (AN)
    {
        case 0xFE:
            return 0;
        case 0xFD:
            return 1;
        case 0xFB:
            return 2;
        case 0xF7:
            return 3;
        case 0xEF:
            return 4;
        case 0xDF:
            return 5;
        case 0xBF:
            return 6;
        case 0x7F:
            return 7;
        default:
            return 0;
    }
}

static char digitToASCII(uint8_t digit)
{
    if (digit < 10)
        return digit + 48;
    else
        return digit + 55;
}

void Peripherals::WSSendSevSeg(uint8_t digit, uint8_t AN)
{
    constexpr auto sendInterval = 333ms;
    static auto    lastSend     = std::chrono::steady_clock::now();

    uint8_t digitHex = sevSegToHex(digit);

    uint8_t digitIdx = anodeToIdx(AN);

    
    this->sevseg.at(digitIdx) = digitHex;
    
    auto now = std::chrono::steady_clock::now();
    
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSend) >= sendInterval)
    {
        // fmt::println("{}: {}({})", digitIdx, digitHex, digit);

        json j;
        for (size_t i = 0; i < sevseg.size(); i++)
        {
            j["sevseg"].push_back(std::string(1, digitToASCII(sevseg.at(i))));
        }

        conn.sendText(j.dump());

        lastSend = std::chrono::steady_clock::now();
    }
}

void Peripherals::WSSendLEDs(uint16_t leds)
{
    if (leds == lastLeds)
        return;

    json j;

    std::bitset<16> ledsMap     = leds;
    std::bitset<16> lastLedsMap = lastLeds;

    for (size_t i = 0; i < ledsMap.size(); ++i)
    {
        if (ledsMap[i] == lastLedsMap[i])
            continue;

        j["gpio"].push_back({{"id", fmt::format("LD{}", i)}, {"on", ledsMap[i] ? true : false}});
    }

    conn.sendText(j.dump(4));

    lastLeds = leds;
}

void Peripherals::SetSwitch(json& gpio)
{
    std::lock_guard<std::mutex> lock(smtx);

    std::string id = gpio["id"];

    if (!id.starts_with("SW"))
        return;

    std::stringstream sstream(id.substr(2));
    size_t            idx;
    sstream >> idx;

    if (idx > 15)
        return;

    switches.set(idx, gpio["on"]);
}

uint16_t Peripherals::GetSwitches()
{
    std::lock_guard<std::mutex> lock(smtx);
    return static_cast<uint16_t>(switches.to_ullong());
}

void Peripherals::SetButton(json& btn)
{
    std::lock_guard<std::mutex> lock(smtx);

    std::string bid = btn["id"];
    btns[bid]       = btn["on"];
}

bool Peripherals::GetBTNL()
{
    std::lock_guard<std::mutex> lock(smtx);
    return btns["BTNL"];
}

bool Peripherals::GetBTNU()
{
    std::lock_guard<std::mutex> lock(smtx);
    return btns["BTNU"];
}

bool Peripherals::GetBTNR()
{
    std::lock_guard<std::mutex> lock(smtx);
    return btns["BTNR"];
}

bool Peripherals::GetBTND()
{
    std::lock_guard<std::mutex> lock(smtx);
    return btns["BTND"];
}

bool Peripherals::GetBTNC()
{
    std::lock_guard<std::mutex> lock(smtx);
    return btns["BTNC"];
}

bool Peripherals::GetRSTN()
{
    std::lock_guard<std::mutex> lock(smtx);
    return btns["RSTN"];
}
