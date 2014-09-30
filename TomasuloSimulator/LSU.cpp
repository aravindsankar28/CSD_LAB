#include "LSU.h"

#include "Tomasulo.h"
#include <stdint.h>


void LSU::try_pop_from_store_queue()
{

  if(store_queue.size() > 0 )
  {

    // TODO check completed bit before popping
   Store_Queue_Entry *head = &store_queue.front();

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
        this->is_busy = false;
      }

    }
  }

}
bool LSU::is_forwarding_possible(int memory_address)
{
  for (int i = 0; i < store_queue.size(); ++i)
  {
    Store_Queue_Entry sqe = store_queue[i];
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
    this->reqd_cycles = 1; // only need to push to store queue.
    this->is_store_adding_in_progress = true;

    cout << "instruction " << this->instruction_number << " issued on LSU "<<endl;

  }
  else if(opcode == LOAD && is_forwarding)
  {
      this->instruction_number = instruction_number;
      this->opcode = opcode;
      this->is_forwarding = is_forwarding;
      this->reqd_cycles = 1;

      cout << "instruction " << this->instruction_number << " issued on LSU "<<endl;

  }
  else if(opcode == LOAD) 
  {
      // load which is not being forwarded
      this->is_busy = true;
      this->instruction_number_mem = instruction_number;
      this->opcode_mem = opcode;
      this->reqd_cycles_mem = 2; // For now set load latency as 2.
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

    if(this->commited && this->opcode == LOAD)
    {

      this->rob->set_complete(this->dest);
      RRF_Entry *rrf_entry = this->rrf->get_entry(this->dest);
      rrf_entry->valid = true;

      if(! this->is_forwarding)
        this->is_busy = false;

      this->commited = false;
      rrf_entry->valid = true;

      rrf_entry->data = this->scratch;  
      cout << "LSU Commit done of instruction "<< this->instruction_number<<endl;

      
    }
    else if(this->commited && this->opcode == STORE)
    {
        this->rob->set_complete_ip(this->instruction_number);
        cout << "LSU Commit done of instruction "<< this->instruction_number_mem<<endl;
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
  this->curr_cycle_mem ++;
  
  //cout << this->curr_cycle <<" " <<this->reqd_cycles << endl;
  if(this->is_busy && this->curr_cycle_mem == this->reqd_cycles_mem || this->curr_cycle == this->reqd_cycles && this->is_forwarding
    || this->curr_cycle == this->reqd_cycles && this->is_store_adding_in_progress)
  {
    
    //this->scratch = this->calculate();
    if(this->opcode == STORE)
    {
      // now actually push to store queue
      Store_Queue_Entry* store_queue_entry = new Store_Queue_Entry();
      store_queue_entry->data_to_store = this->src2;
      store_queue_entry->memory_address = this->src1;
      store_queue_entry->instruction_number = this->instruction_number;
      store_queue_entry->latency_timer = 5; // for now 
      store_queue.push_back(*store_queue_entry);
      this->is_store_adding_in_progress = false;
      this->commited = true;
       cout << "Finished execution of instruction "<<this->instruction_number<< " by pushing to store queue "<<endl;
    }

   else if(this->opcode == LOAD)
    {
      // will be true only if forwarding possible
        if(is_forwarding_possible(this->src1))
        {
          this->scratch = find_val(this->src1);
          this->is_forwarding = false;
          this->commited = true;
          cout << "Finished execution of instruction "<<this->instruction_number<< " by load forwarding"<<endl;
        }
        else
        {
          cout << "STUPID - if forwarding not possible why come here :( ";
        }
    }
       
      if(this->opcode_mem == LOAD)
      {
          // fetch from memory
          this->scratch_mem = this->mem[this->src1_mem];
          this->commited_mem = true;
          cout << "Finished execution of instruction "<<this->instruction_number_mem<< " by accessing cache"<<endl;
      }
    
   
  }
}


int LSU::calculate()
{
  int s1 = this->src1;
  int s2 = this->src2;
}
