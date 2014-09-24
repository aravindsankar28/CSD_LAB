#include <iostream>
#include <deque>
#include "Tomasulo.h"


using namespace std;

int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;

    ROB *rob =  new ROB(8);
    Res_Station *rs = new Res_Station(8);
    ARF *arf = new ARF(8);
    RRF *rrf = new RRF(8);
    Tomasulo t(8,8,8,2,rob,rs,arf,rrf);
    t.fetch_instructions_to_cache();

    t.fetch_instructions_to_buffer();
    
    t.decode_instructions();
    return 0;
}
