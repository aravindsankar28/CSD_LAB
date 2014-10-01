#include "LSU.h"

#include "Tomasulo.h"
#include <stdint.h>


void LSU::try_pop_from_store_queue()
{

  if(store_queue.size() > 0 )
  {

    // TODO check completed bit before popping
   Store_Queue_Entry *head = &store_queue.front();
  if (!head->completed)
    return;
  if( !this->is_busy || head->latency_timer < 5 )
  {
    
     if(head->latency_timer == 5)
      cout << "Starting to pop from store queue" <<endl;

     if(head->latency_timer != 5 && ! this->is_busy)
        {
          cout << "ERROR" << endl;
        }
      this->is_busy = true;
      if(head->latency_timer >0)
        head->latency_timer --;

      if(head->latency_timer == 0)
      {
        store_queue.pop_front();
        cout << "Poppping from store queue "<<endl;
	this->mem[head->memory_address] = head->data_to_store;
        this->is_busy = false;
      }

    }
  }

}
bool LSU::is_forwarding_possible(int memory_address)
{
  for (int i = 0; i < store_queue.size(); ++i)
  {
    cout << i << "  "<< store_queue.size()<<endl;
    Store_Queue_Entry sqe = store_queue[i];
    cout <<"Mem address " << memory_address <<" sqe " <<sqe.memory_address<<endl;
    if(memory_address == sqe.memory_address)
      return true;
  }
  return false;
}
void LSU::issue_instruction(int instruction_number,int opcode, int src1, int src2, int dest, ROB* rob, RRF* rrf, bool is_forwarding )
{
  
  //this->reqd_cycles = get_cycles(opcode);
  if(opcode == STORE)
  {
    this->instruction_number = instruction_number;
    this->opcode = opcode;
    this->curr_cycle = 0;
    this->reqd_cycles = 1; // only need to push to store queue.
    this->is_store_adding_in_progress = true;
    this->src1 = src1;
    this->src2 = src2;
    this->dest = dest;   
    cout << "instruction " << this->instruction_number << " issued on LSU "<<endl;

  }
  else if(opcode == LOAD && is_forwarding)
  {
      // load which is not being forwarded
      this->instruction_number_for = instruction_number;
      this->opcode_for = opcode;
      this->reqd_cycles_for = 1;
      this->curr_cycle_for = 0;
      this->commited_for = false;
      this->src1_for = src1;
      this->src2_for = src2;
      this->dest_for = dest;
      this->is_forwarding = is_forwarding;
      cout << "Instruction " << this->instruction_number_for << " issued on LSU for forwarding "<<endl;

  }
  else if(opcode == LOAD) 
  {
      // load which is not being forwarded
      this->is_busy = true;
      this->instruction_number_mem = instruction_number;
      this->opcode_mem = opcode;
      this->reqd_cycles_mem = 4; // For now set load latency as 4.
      this->curr_cycle_mem = 0;
      this->commited_mem = false;
      this->src1_mem = src1;
      this->src2_mem = src2;
      this->dest_mem = dest;

      cout << "instruction " << this->instruction_number_mem << " issued on LSU "<<endl;


  }

 
  this->rob = rob;
  this->rrf = rrf;
  }

void LSU::commit()
{

//TODO
  if(this->commited_mem && this->opcode_mem == LOAD)
    {

      this->rob->set_complete(this->dest_mem);
      RRF_Entry *rrf_entry = this->rrf->get_entry(this->dest_mem);
      rrf_entry->valid = true;

      if(! this->is_forwarding)
        this->is_busy = false;

      this->commited_mem = false;
      rrf_entry->valid = true;

      rrf_entry->data = this->scratch_mem;  
      cout << "LSU Commit done of instruction "<< this->instruction_number_mem<<endl;
      
    }

    if(this->commited_for && this->opcode_for == LOAD)
    {

      this->rob->set_complete(this->dest_for);
      RRF_Entry *rrf_entry = this->rrf->get_entry(this->dest_for);
      rrf_entry->valid = true;

      if(! this->is_forwarding)
        this->is_busy = false;

      this->commited_for = false;
      rrf_entry->valid = true;

      rrf_entry->data = this->scratch_for;  
      cout << "1 : LSU Commit done of instruction "<< this->instruction_number_for<<endl;

      
    }
    if(this->commited && this->opcode == STORE)
    {
        this->rob->set_complete_store(this->dest);
        cout << "2 : LSU Commit done of instruction "<< this->instruction_number<<endl;
	this->commited = false;
    }
    cout << endl;


}
int LSU::find_val(int memory_address)
{
  for (int i = 0; i < store_queue.size(); ++i)
  {
    Store_Queue_Entry sqe = store_queue[i];
    if(memory_address == sqe.memory_address)
      return sqe.data_to_store;
  }
}

void LSU::run()
{
  this->curr_cycle++;
  this->curr_cycle_for++;
  this->curr_cycle_mem ++;
  
  //cout << this->curr_cycle <<" " <<this->reqd_cycles << endl;
 
    //this->scratch = this->calculate();
  
  
    if(this->opcode == STORE && this->curr_cycle == this->reqd_cycles && this->is_store_adding_in_progress)
    {
      // now actually push to store queue
      Store_Queue_Entry* store_queue_entry = new Store_Queue_Entry();
      store_queue_entry->data_to_store = this->src2;
      store_queue_entry->memory_address = this->src1;
      cout << "sqe mem address" << this->src1 <<endl;
      store_queue_entry->instruction_number = this->instruction_number;
      store_queue_entry->latency_timer = 5; // for now 
      store_queue.push_back(*store_queue_entry);
      this->is_store_adding_in_progress = false;
      this->commited = true;
       cout << "Finished execution of instruction "<<this->instruction_number<< " by pushing to store queue "<<endl;
    }

   if(this->opcode_for == LOAD && this->curr_cycle_for == this->reqd_cycles_for && this->is_forwarding)
    {
      // will be true only if forwarding possible
        if(is_forwarding_possible(this->src1))
        {
          this->scratch_for = find_val(this->src1);
          this->is_forwarding = false;
          this->commited_for = true;
          cout << "Finished execution of instruction "<<this->instruction_number_for<< " by load forwarding"<<endl;
        }
        else
        {
          cout << "STUPID - if forwarding not possible why come here :( ";
        }
    }
       
      if(this->opcode_mem == LOAD && this->is_busy && this->curr_cycle_mem == this->reqd_cycles_mem)
      {
	cout << "loading "<<endl;
          // fetch from memory
          this->scratch_mem = this->mem[this->src1_mem];
          this->commited_mem = true;
          cout << "Finished execution of instruction "<<this->instruction_number_mem<< " by accessing cache"<<endl;
      }
    
   
  
}


int LSU::calculate()
{
  int s1 = this->src1;
  int s2 = this->src2;
}
