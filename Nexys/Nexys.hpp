#pragma once
#include <fmt/core.h>
#include <ixwebsocket/IXWebSocketServer.h>

#include <array>
#include <bitset>
#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <nlohmann/json.hpp>
#include <sstream>

using namespace std::literals::chrono_literals;

using json = nlohmann::json;

class Peripherals final
{
private:
    uint16_t               lastLeds = 0;
    std::array<uint8_t, 8> sevseg   = {0};

    std::bitset<16>             switches = {0};
    std::map<std::string, bool> btns;

    std::mutex     smtx{};
    ix::WebSocket& conn;

public:
    Peripherals(ix::WebSocket& wsconn)
        : conn(wsconn), btns({{"BTNL", false},
                              {"BTNU", false},
                              {"BTNR", false},
                              {"BTND", false},
                              {"BTNC", false},
                              {"RSTN", false}})
    {
    }

    void SetSwitch(json& gpio);
    void SetButton(json& btn);
    bool GetBTNL();
    bool GetBTNU();
    bool GetBTNR();
    bool GetBTND();
    bool GetBTNC();
    bool GetRSTN();

    uint16_t GetSwitches();

    void WSSendLEDs(uint16_t leds);

    void WSSendSevSeg(uint8_t digit, uint8_t AN);
};

class Nexys
{
public:
    Nexys(ix::WebSocket& wsconn) : periph(wsconn) {}

    void StartMainLoop();
    void StopMainLoop() { started.store(false); }

    void SetInputs(json inp)
    {
        periph.SetSwitch(inp["gpio"]);
        periph.SetButton(inp["gpio"]);
    };

private:
    std::atomic<bool> started = false;

    Peripherals periph;
};
