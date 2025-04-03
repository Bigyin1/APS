#pragma once

#include <ixwebsocket/IXWebSocketServer.h>

#include <array>
#include <bitset>
#include <chrono>
#include <format>
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <sstream>

#include "Vnexys_adder.h"

using json = nlohmann::json;

class Peripherals final
{
private:
    uint16_t        lastLeds = 0;
    std::bitset<16> switches = {0};

    std::array<uint8_t, 8> sevseg = {0};

    std::mutex     smtx{};
    ix::WebSocket& conn;

public:
    Peripherals(ix::WebSocket& wsconn) : conn(wsconn) {}

    void UpdateSwitches(json& gpio) noexcept
    {
        std::lock_guard<std::mutex> lock(smtx);

        for (auto& [key, value] : gpio.items())
        {
            if (!key.starts_with("SW"))
                continue;

            std::stringstream sstream(key.substr(2));
            size_t            idx;
            sstream >> idx;

            if (idx > 15)
                continue;

            switches.set(idx, value);
        }
    }

    void SendLEDs(uint16_t leds)
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

            j["gpio"][std::format("LD{}", i)] = ledsMap[i] ? 1 : 0;
        }

        conn.sendText(j.dump(4));

        lastLeds = leds;
    }

    void SendSevSeg(const Vnexys_adder& top);

    uint16_t GetSwitches() noexcept
    {
        std::lock_guard<std::mutex> lock(smtx);
        return static_cast<uint16_t>(switches.to_ullong());
    }
};

class Nexys
{
public:
    Nexys(ix::WebSocket& wsconn) : periph(wsconn) {}

    void StartMainLoop();
    void StopMainLoop() { started.store(false); }

    void UpdateInputs(json inp) { periph.UpdateSwitches(inp["gpio"]); };

private:
    void tick();
    void reset();

    Vnexys_adder      top      = Vnexys_adder();
    vluint64_t        mainTime = 0;
    std::atomic<bool> started  = false;

    Peripherals periph;
};
