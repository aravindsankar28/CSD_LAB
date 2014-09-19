#ifndef ROB_H
#define ROB_H

#include <deque>
#include "RRF.h"

using namespace std;

struct ROB_Entry{
public:
  int tag;
  bool exec;
}

class ROB{
  int max_size;
  deque<ROB_Entry> entries;
public:
  ROB(int max_size);
  void attempt_push(ROB_Entry r);
  void attempt_pop();
  void set_complete(int tag);
};