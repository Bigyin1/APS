
#include "Nexys.hpp"
#include "Vnexys.h"

void Nexys::StartMainLoop()
{
    static Vnexys top = Vnexys();

    started.store(true);
    top.CLK100 = 1;
    top.resetn = 1;

    while (started.load() && !Verilated::gotFinish())
    {
        top.eval();

        top.resetn = !this->periph.GetRSTN();
        top.BTND   = this->periph.GetBTND();
        top.SW     = this->periph.GetSwitches();

        this->periph.WSSendLEDs(top.LED);

        uint8_t digit = (top.CA << 6) + (top.CB << 5) + (top.CC << 4) + (top.CD << 3) +
                        (top.CE << 2) + (top.CF << 1) + (top.CG);
        this->periph.WSSendSevSeg(digit, top.AN);

        top.CLK100 = !top.CLK100;
    }
}
