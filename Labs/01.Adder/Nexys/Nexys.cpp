#include "Nexys.hpp"

#include <iostream>

static uint8_t sevSegToHex(uint8_t Digit)
{
    uint8_t Digit_Hex = 0;

    if (Digit == 0x01)
        Digit_Hex = 0;
    if (Digit == 0x4f)
        Digit_Hex = 1;
    if (Digit == 0x12)
        Digit_Hex = 2;
    if (Digit == 0x06)
        Digit_Hex = 3;
    if (Digit == 0x4c)
        Digit_Hex = 4;
    if (Digit == 0x24)
        Digit_Hex = 5;
    if (Digit == 0x20)
        Digit_Hex = 6;
    if (Digit == 0x0f)
        Digit_Hex = 7;
    if (Digit == 0x00)
        Digit_Hex = 8;
    if (Digit == 0x04)
        Digit_Hex = 9;
    if (Digit == 0x08)
        Digit_Hex = 10;
    if (Digit == 0x60)
        Digit_Hex = 11;
    if (Digit == 0x31)
        Digit_Hex = 12;
    if (Digit == 0x42)
        Digit_Hex = 13;
    if (Digit == 0x30)
        Digit_Hex = 14;
    if (Digit == 0x38)
        Digit_Hex = 15;

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

void Peripherals::SendSevSeg(const Vnexys_adder& top)
{
    constexpr auto sendInterval = 333ms;
    static auto    lastSend     = std::chrono::steady_clock::now();

    uint8_t digit = (top.CA << 6) + (top.CB << 5) + (top.CC << 4) + (top.CD << 3) + (top.CE << 2) +
                    (top.CF << 1) + (top.CG);

    uint8_t digitHex = sevSegToHex(digit);

    uint8_t digitIdx = anodeToIdx(top.AN);

    this->sevseg.at(digitIdx) = digitHex;

    auto now = std::chrono::steady_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSend) >= sendInterval)
    {

        json j;
        for (size_t i = 0; i < sevseg.size(); i++)
        {
            j[fmt::format("SevSegDispl{}", i)] = digitToASCII(sevseg.at(i));
        }

        conn.sendText(j.dump());

        lastSend = std::chrono::steady_clock::now();
    }
}

void Nexys::StartMainLoop()
{
    started.store(true);
    top.CLK100 = 1;

    while (started.load() && !Verilated::gotFinish())
    {
        top.eval();

        top.SW = this->periph.GetSwitches();

        this->periph.SendLEDs(top.LED);
        this->periph.SendSevSeg(top);

        top.resetn = 1;
        if (mainTime <= 100)
            top.resetn = 0;

        tick();
    }
}

void Nexys::tick()
{
    top.CLK100 = !top.CLK100;
    mainTime += 10;
}

void Nexys::reset() { top.resetn = 0; }
