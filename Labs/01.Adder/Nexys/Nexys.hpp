#pragma once

#include <ixwebsocket/IXWebSocketServer.h>

#include <bitset>
#include <mutex>

#include "Vnexys_adder.h"

class NexysState
{

private:
    std::bitset<16> oldLeds;
    std::bitset<16> leds;

    std::bitset<16> switches;
    // std::bitset<5>  buttons;

    std::mutex smtx;

public:
    void setSwitch(size_t idx, bool val)
    {
        std::lock_guard<std::mutex> lock(smtx);
        this->switches.set(idx, val);
    }

    void setLED(size_t idx, bool val) { this->leds.set(idx, val); }

    std::optional<std::bitset<16>> getLEDs()
    {
        if (leds == oldLeds)
            return std::nullopt;

        oldLeds = leds;
        return leds;
    }
};

class Nexys
{

public:
    Nexys(ix::WebSocket& wsconn) : conn(wsconn) {}
    ~Nexys() { conn.close(); }

    bool IsStarted() { return started.load(); }

    void Start();

    void UpdateInputs();

private:
    void tick();
    void reset();

    ix::WebSocket& conn;

    Vnexys_adder      top      = Vnexys_adder();
    vluint64_t        mainTime = 0;
    std::atomic<bool> started  = false;

    NexysState state;
};
