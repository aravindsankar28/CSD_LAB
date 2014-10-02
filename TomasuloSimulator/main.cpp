#include <iostream>
#include <deque>
#include "Tomasulo.h"
#include <stdlib.h>
#include <cstring>

using namespace std;


int main(int argc, char **argv) {
    bool debug = false;
    if (argc == 2 && strcmp(argv[1], "--debug") == 0){
      cout<<"Running in debugging mode"<<endl;
      debug = true;
    }
    else{
      debug = false;
      cout<<"Running in normal mode"<<endl;
    }
    ROB *rob =  new ROB(8, debug);
    Res_Station *rs = new Res_Station(8, debug);
    ARF *arf = new ARF(8, debug);
    RRF *rrf = new RRF(8, debug);
    Tomasulo t(8,8,8,2,rob,rs,arf,rrf, debug);
    t.simulate();
    t.opcode_helper("ADD");
    return 0;
}
