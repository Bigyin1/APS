
#include "Nexys.hpp"
#include "Vnexys.h"

void Nexys::StartMainLoop()
{
    static Vnexys top = Vnexys();

    started.store(true);
    top.clk_i   = 1;
    top.arstn_i = 1;

    while (started.load() && !Verilated::gotFinish())
    {
        top.eval();

        top.arstn_i = !this->periph.GetRSTN();
        top.sw_i    = this->periph.GetSwitches();

        this->periph.WSSendLEDs(top.led_o);

        uint8_t digit = (top.ca_o << 6) + (top.cb_o << 5) + (top.cc_o << 4) + (top.cd_o << 3) +
                        (top.ce_o << 2) + (top.cf_o << 1) + (top.cg_o);
        this->periph.WSSendSevSeg(digit, top.an_o);

        top.clk_i = !top.clk_i;
    }
}
