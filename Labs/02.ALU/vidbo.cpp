
#include "Nexys.hpp"
#include "Valu_opcodes_pkg.h"


void Nexys::StartMainLoop()
{
    static Vnexys_alu top      = Vnexys_alu();
    static vluint64_t   mainTime = 0;

    started.store(true);
    top.CLK100 = 1;

    while (started.load() && !Verilated::gotFinish())
    {
        top.eval();

        top.SW = this->periph.GetSwitches();

        this->periph.SendLEDs(top.LED);

        uint8_t digit = (top.CA << 6) + (top.CB << 5) + (top.CC << 4) + (top.CD << 3) +
                        (top.CE << 2) + (top.CF << 1) + (top.CG);
        this->periph.SendSevSeg(digit, top.AN);

        top.resetn = 1;
        if (mainTime <= 100)
            top.resetn = 0;

        top.CLK100 = !top.CLK100;
        mainTime+= 10;
    }
}
