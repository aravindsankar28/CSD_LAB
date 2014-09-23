#ifndef ROB_H
#define ROB_H

#include <deque>


using namespace std;

struct ROB_Entry{
public:
  int tag;
  bool exec;
};

class ROB{
  int max_size;
  deque<ROB_Entry> entries;
public:
  ROB(int max_size);
  bool attempt_push(ROB_Entry r);
  bool attempt_pop();
  void set_complete(int tag);
};

#endif
