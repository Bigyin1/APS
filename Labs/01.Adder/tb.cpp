#include "Vtb.h"

int main(int argc, char** argv)
{
    
    Vtb top = Vtb();
    while (!Verilated::gotFinish())
    {
        top.eval();
    }
}
