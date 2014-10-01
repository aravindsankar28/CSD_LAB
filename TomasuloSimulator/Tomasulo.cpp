  #include "Tomasulo.h"
  #include <fstream>
  #include <iostream>
  #include <string>
  #define FILE_NAME "program1.txt"

  using namespace std;

  Tomasulo::Tomasulo(int num_arch_reg, int num_renamed_reg, int num_rs_entries, int issue_size, ROB* rob, Res_Station* rs, ARF* arf, RRF* rrf)
  {
      this->num_arch_reg = num_arch_reg;
      this->num_renamed_reg= num_renamed_reg;
      this->num_rs_entries = num_rs_entries;
      this->issue_size = issue_size;
      this->rob = rob;
      this->rs = rs;
      this->arf = arf;
      this-> rrf = rrf;
      this->instruction_cache = new queue<string>();
      this->instruction_buffer = new queue<string>();
      this->max_instruction_buffer_size = 4;
      this->cycle = 0;
      this->store_index = 0;
      this->curr_instr = 0;
      this->alu = new ALU[NUM_INT_UNITS];
      this->lsu = new LSU(Memory,8);
      this->alu->instruction_cycles = this->instruction_cycles;
      this->lsu->instruction_cycles = this->instruction_cycles;
    
  }

  void Tomasulo::fetch_instructions_to_cache()
  {
	  
	  std::ifstream input(FILE_NAME);
	  std::string line;
	  
	  while(std::getline(input,line))
		  instruction_cache->push(line);

  }

  void Tomasulo::fetch_instructions_to_buffer()
  {
	  int i = 0;
	  while(instruction_buffer->size() < max_instruction_buffer_size && i < issue_size && instruction_cache->size() > 0)
	  {
		  string instruction = instruction_cache->front();
		  instruction_cache->pop();
		  instruction_buffer->push(instruction);
		  cout << "Fetching instruction "<<instruction <<endl;
		  i++;
	  }
  }

  void Tomasulo::decode_instructions()
  {
	  int i = 0;
	  
	  while(i < issue_size && instruction_buffer->size() > 0 && rs->size < rs->max_size && rob->get_size() < rob->max_size)
	  {
		  
		  string instruction = instruction_buffer->front(); // fetch the first instruction from buffer.

		  // Now actually decode to get opcode and operands
		  Decoded_Instruction *decoded_instruction = new Decoded_Instruction(instruction);

		  curr_instr ++;

		  /*cout << decoded_instruction->opcode<<" "<< decoded_instruction->ops[0].value << 
		  decoded_instruction->ops[1].value << decoded_instruction->ops[2].value<<  endl;*/

		  cout << "Decoding Instruction "<<instruction <<" "<<curr_instr <<endl;
		  operand dest,src1,src2;
		  if(decoded_instruction->opcode != "STORE")
		  {	
			  dest = decoded_instruction->ops[0];
			  src1 = decoded_instruction->ops[1];
			  src2 = decoded_instruction->ops[2];
		  }
		  else
		  {
			  // case of STORE
		    
			  //Since the destination is anyway unsused, we use it to store the tag.
			  //Increment the store index
			  src1 = decoded_instruction->ops[0];
			  src2 = decoded_instruction->ops[1];
		  }

		  
		  Res_Station_Entry* res_station_entry =  rs->get_free_entry();
		   res_station_entry->instruction_number = curr_instr;
		  // Now, check if an RRF entry can be assigned to the destination operand. If not, stop
		  // In case of a store, rrf entry need not be created for destination.
		  int found_dest_reg = 0;
		  
		  if(decoded_instruction->opcode != "STORE")
		  {
			  found_dest_reg =  rrf->find_non_busy_register();

			  if(found_dest_reg < 0)
				  break;

			  int dest_reg_rrf = found_dest_reg;
			  // doing destination allocate now. Not dest will always be a register

			  ARF_Entry arf_entry_dest = *(arf->get_entry(dest.value)); 
			  arf_entry_dest.busy = 1; // set busy bit in arf
			  arf_entry_dest.tag = dest_reg_rrf; // set tag as the offset in rrf
			  RRF_Entry rrf_entry_dest = *(rrf->get_entry(dest_reg_rrf));
			  rrf_entry_dest.valid = 0; // set valid bit of rrf entry = 0 
			  rrf_entry_dest.busy = 1; // set busy bit of rrf entry = 1

			  rrf->entries[dest_reg_rrf] = rrf_entry_dest;
			  arf->entries[dest.value] = arf_entry_dest;
			  res_station_entry->dest_tag = found_dest_reg; // set destination tag
	  }
		  // Source Read
		  // Now check src1 and src2 in the register files.
		  if ( ! src1.is_immediate )
		  {
			  
			  // If ARF entry is not busy
			  ARF_Entry arf_entry_src1 = *(arf->get_entry(src1.value));
			  if(! arf_entry_src1.busy)
				  {
					  res_station_entry->src1_data_present= 1;
					  res_station_entry->src1_data = arf_entry_src1.data;
				  }
			  else
			  {
				  // Arf entry busy
				  // Get corresponding rrf entry using the tag present in the arf entry.
				  RRF_Entry rrf_entry_src1 = *(rrf->get_entry(arf_entry_src1.tag));
				  if(! rrf_entry_src1.valid)
				  {
					  // if rrf entry is not valid, set tag for src1 in the res station entry.
					  res_station_entry->src1_tag = arf_entry_src1.tag;
					  res_station_entry->src1_data_present = 0;
				  }
				  else
				  {
					  // if rrf entry is valid, set data for src1 in the res station entry.
					  res_station_entry->src1_data = rrf_entry_src1.data;
					  res_station_entry->src1_data_present = 1;	
				  }
			  }
		  }
		  else
			  {
				  // operand is immediate
				  res_station_entry->src1_data_present = 1;
				  res_station_entry->src1_data = src1.value;
			  }

			  // IF LOAD, then src2 does not exist
		  // Source read for src2
		  if ( ! src2.is_immediate && decoded_instruction->opcode != "LOAD")
		  {
			  
			  // If ARF entry is not busy
			  ARF_Entry arf_entry_src2 = *(arf->get_entry(src2.value));
			  if(! arf_entry_src2.busy)
				  {
					  res_station_entry->src2_data_present= 1;
					  res_station_entry->src2_data = arf_entry_src2.data;
				  }
			  else
			  {

				  // Arf entry busy
				  // Get corresponding rrf entry using the tag present in the arf entry.
				  RRF_Entry rrf_entry_src2 = *(rrf->get_entry(arf_entry_src2.tag));
				  if(! rrf_entry_src2.valid)
				  {

					  // if rrf entry is not valid, set tag for src1 in the res station entry.
					  res_station_entry->src2_tag = arf_entry_src2.tag;
					  res_station_entry->src2_data_present = 0;
					  
				  }
				  else
				  {
					  // if rrf entry is valid, set data for src1 in the res station entry.
					  res_station_entry->src2_data = rrf_entry_src2.data;
					  res_station_entry->src2_data_present = 1;	
				  }
			  }
		  }
		  else if(decoded_instruction->opcode != "LOAD")
			  {
				  // operand is immediate
				  res_station_entry->src2_data_present = 1;
				  res_station_entry->src2_data = src2.value;
			  }
			  // Set opcode and busy bit in the entry
			  res_station_entry->opcode = decoded_instruction->opcode;
			  res_station_entry->exec_over = 0;
			  res_station_entry->busy = true;
			  rs->size ++;
			  // Add to station
			  //if(rs->size < rs->max_size)
			  //	rs->add_entry(res_station_entry);

		  ROB_Entry* rob_entry = new ROB_Entry();
		  rob_entry->instruction_number = curr_instr;
		  rob_entry->tag = res_station_entry->dest_tag;
		  rob_entry->opcode = decoded_instruction->opcode;
		  if(decoded_instruction->opcode == "STORE"){
		    lsu->rob_position_recent_store = rob->entries.size();
		    res_station_entry->dest_tag = this->store_index;
		    this->store_index++;
		  }
		  rob_entry->dest = res_station_entry->dest_tag;
		  rob_entry->tag = res_station_entry->dest_tag;
		  rob_entry->src1 = res_station_entry->src1_data;
		  rob_entry->src2 = res_station_entry->src2_data;
		  bool flag = rob->attempt_push(*rob_entry);
		  
		  if(! flag)
			  cout <<"ERROR" <<endl;

		  instruction_buffer->pop();

		  i++;

	  }
  }




  void Tomasulo::simulate()
  {
	  // For now 
	  fetch_instructions_to_cache();

	  while(rob->get_size()>0 || instruction_buffer->size() >0 || instruction_cache->size() >0 || lsu->store_queue.size() > 0)
	  {
	    if(cycle >12)
	      break;
	    
	    //rob->print();
		  cout<<"Cycle "<<cycle+1<<endl;
		  bool rob_popped = commit_instructions();
		  execute_instructions();
		  decode_instructions();
		  fetch_instructions_to_buffer();
		  cycle ++;
		  
		  for (int i = 0; i < NUM_INT_UNITS; ++i)
		  {
			  alu[i].commit();
		  }


		  lsu->try_pop_from_store_queue();

		  lsu->commit();
		  if(rob_popped)
		  {
			  while(rob->scratch_queue.size() > 0)
			  {
				/*  cout << "rob scratch " <<endl;
				  cout<< rob->scratch_queue.front().instruction_number<< endl;
				*/  ROB_Entry scratch = rob->scratch_queue.front();
// 				  cout << "rob scratch popped" <<endl;
				   rob->scratch_queue.pop_front();
				   
				  //Get instrution number of this instruction
				  int ip = scratch.instruction_number;
				  
				  //Check if it is a store instruction
				  deque<Store_Queue_Entry>::iterator iter = lsu->store_queue.begin();
				  bool is_store_instr = false;
				  //pointer to entry in store queue
				  Store_Queue_Entry *sqe;
				  while (iter != lsu->store_queue.end()){
				    sqe = &(*iter);
				    
				    //if this is true, then the instruction corresponds to a store
				    if(sqe->instruction_number == ip){
				      is_store_instr = true;
				      break;
				    }
				    iter++;
				  }
				  
				  //This is a store instruction.
				  //Note: sqe points to the corresponding entry in store queue
				  if (is_store_instr){
				    sqe->completed = true;
				  }
				  
				  else{
				    rrf->entries[scratch.tag].busy = 0;
				    rrf->entries[scratch.tag].valid = 0;
				    //cout << rob->scratch.tag <<endl;
				    for (int i = 0; i < arf->size; ++i)
				    {
					  if(arf->entries[i].tag == scratch.tag)
						  {
							  // found an arf entry.
							  arf->entries[i].data = rrf->entries[scratch.tag].data;
							  arf->entries[i].busy = 0;
						  }

					  // not found => put peace
				    }
				  }
				 
				  
			  }
		  }
		  //this->rob->print();
		  //display_arf();
		  //display_rrf();
		  //display_rs();
	  }
	  cout << "all done" <<endl;
	  //cout << instruction_buffer->size()<<endl;

  }
  bool Tomasulo::commit_instructions()
  {
	  return rob->attempt_pop();

  }
  void Tomasulo::execute_instructions()
  {
	  // Res station should check all entries in arf and rrf and update it's entries. (Why ?)
	  rs->display();
	  /*cout << rs->get_entry(0)->src1_data_present <<endl;
	  cout << rs->get_entry(0)->src2_data_present <<endl;*/

	  for (int i = 0; i < rs->max_size ; ++i)
	  {
		  Res_Station_Entry* rs_entry = rs->get_entry(i);
		  if (rs_entry->busy)
		  {
			  // Which means there is some valid entry there
			  // Check if src1 and src2 have data available.
			  if(! rs_entry->src1_data_present)
			  {
				  int tag = rs_entry->src1_tag;
				  if(rrf->get_entry(tag)->valid)
				  {
					  rs_entry->src1_data_present = 1;
					  rs_entry->src1_data = rrf->get_entry(tag)->data;
				  }
			  }
			  // src 2's tag can be < 0 in case of a load
			  if(! rs_entry->src2_data_present && rs_entry->src2_tag > 0)
			  {
				  int tag = rs_entry->src2_tag;
				  if(rrf->get_entry(tag)->valid)
				  {
					  rs_entry->src2_data_present = 1;
					  rs_entry->src2_data = rrf->get_entry(tag)->data;
				  }
			  }
		  }

	  }
	  // Then look for functional units and schedule them 
	  for (int i = 0; i < rs->max_size ; ++i)
	  {
		  Res_Station_Entry* rs_entry = rs->get_entry(i);
		  if (rs_entry->busy && rs_entry->src1_data_present && rs_entry->src2_data_present)
		  {
			  
			  // Now look at opcode for non-load / store instuctions
			  if(rs_entry->opcode != "LOAD" && rs_entry->opcode != "STORE")
			  {

				  int opcode_code = opcode_helper(rs_entry->opcode);
				  // check if any ALU is currently free.
				  for (int j = 0; j < NUM_INT_UNITS; ++j)
				  {

					  if(! alu[j].is_busy)
					  {

						  cout << "Scheduling Instruction " <<rs_entry->instruction_number<<" on ALU " << j <<" : ";
						  rs_entry->display();
						  alu[j].issue_instruction(rs_entry->instruction_number,opcode_code,rs_entry->src1_data,rs_entry->src2_data,rs_entry->dest_tag,rob,rrf);
						  // Issued instruction to a free ALU - so  remove that entry from reservation station and break.
						  rs->remove_entry(rs_entry);
						  break;
					  }
				  }
			  }
			  
		  }
	  }
	  // Handling loads and stores seperately
// 	  cout << "LOAD start" <<endl; 
	  // First, find a load which has least ip and is ready.
	  int min_i = 0;
	  int min_instruction_number = -1;
	  int min_store_instr = -1;
	  Res_Station_Entry *chosen_load = NULL;
	  
	  for (int i = 0; i < rs->max_size ; ++i)
	  {
		  Res_Station_Entry* rs_entry = rs->get_entry(i);
		  if (rs_entry->busy && rs_entry->opcode == "LOAD" && rs_entry->src1_data_present)
		  {
			  // this entry is a load and is ready. 
  // 			if(min_instruction_number < 0 || rs_entry->instruction_number < min_instruction_number )
  // 				{
  // 					min_i = i;
  // 					min_instruction_number = rs_entry->instruction_number;
  // 				}
		    min_i = i;
		  
		    //TODO:Check for dependencies
		    int last_store_index = -1;
		    for (int j = 0; j < rob->entries.size(); j++){
		      ROB_Entry re = rob->entries[j];
		      if(re.tag == rs_entry->dest_tag && re.opcode == "LOAD")
			break;
		      
		      if(re.opcode == "STORE" && re.src1 == rs_entry->src1_data)
		      {
			last_store_index = j;
		      }
		    }
		    if(last_store_index >=0 && !rob->entries[last_store_index].exec)
		      continue;
		    
		    chosen_load = rs_entry;
		    
		    break;
		  }
		  /*
		  else if(rs_entry->busy && rs_entry->opcode == "LOAD")
			  //Load has been found, but not ready
			  any_load_found = true;*/
	  }
	 
	  // found a ready load -- 	
	  // Now found the first ready load in program order -- Now check that there are no other stores in prog. order b4 it that 
	  // could a possible alias. If stores exist , stall.
	  
		  // which means we found some load that is ready.
		  // Now search for any stores to see if we can have any conflicts.
  // 		bool is_conflicting = false;
  // 		for (int i = 0; i < rs->max_size; ++i)
  // 		{
  // 			Res_Station_Entry* rs_entry = rs->get_entry(i);
  // 			// look for a store b4 it in program order
  // 			if (rs_entry->busy && rs_entry->opcode == "STORE" && rs_entry->instruction_number < min_instruction_number)
  // 			{
  // 				if((rs_entry->src1_data_present && rs_entry->src1_data == chosen_load->src1_data) || ! rs_entry->src1_data_present)
  // 					{
  // 						is_conflicting = true;
  // 						break;
  // 						// there exists a conflicting store - so stall
  // 					}
  // 			}
  // 		}

		  if(chosen_load != NULL)
		  {
			  // We have a ready load , and no stores before it that can possibly conflict.
			  // So issue it !! Not so fast dude !
			  // Check if load forwarding is possible. 
			  // If not, check if LSU is busy

			  bool flag = lsu->is_forwarding_possible(chosen_load->src1_data);
			  cout << "Is forwarding for instruction "<<chosen_load->instruction_number<<" "<<flag<<endl;
			  if(flag)
			  {
				  rs->remove_entry(chosen_load);
				  // do load forwarding even if lsu is busy
				  lsu->issue_instruction(chosen_load->instruction_number, this->opcode_helper(chosen_load->opcode),
					  chosen_load->src1_data,chosen_load->src2_data,chosen_load->dest_tag,rob,rrf,true);

			  }
			  else if(! lsu->is_busy)
			  {

				  lsu->issue_instruction(chosen_load->instruction_number, this->opcode_helper(chosen_load->opcode),
					  chosen_load->src1_data,chosen_load->src2_data,chosen_load->dest_tag,rob,rrf,false);

				  rs->remove_entry(chosen_load);
				  // lsu not busy
			  }
			  else
			  {
				  // lsu busy -- nothing to do -- put peace and stall.

			  }

		  }
		   
		  // Check for stores.
		  // Find first ready store according to program order.
		  int min_store_i = -1;
		  int min_tag = -1;
		  Res_Station_Entry *chosen_store = NULL;
		  for (int i = 0; i < rs->max_size ; ++i)
		  {
			  Res_Station_Entry* rs_entry = rs->get_entry(i);
			  if (rs_entry->busy && rs_entry->opcode == "STORE" && rs_entry->src1_data_present && rs_entry->src2_data_present)
			  {
				  if(min_tag < 0 || rs_entry->dest_tag < min_tag )
				  {
						  chosen_store = rs_entry;
						  min_tag = rs_entry->dest_tag;
						  
				  }
			  }
// 			  ROB_Entry re = rob->entries[i];
// 			  if(re.opcode == "STORE" && !re.exec && chosen_load->src1_data == re.src1){
// 			    chosen_store = &re;
// 			    break;
// 			  }
		  }
		  
		  
		  if (chosen_store != NULL && lsu->store_queue.size() < lsu->store_queue_max_size)
		  {
			  
			  // Assuming only one store can be removed from res. station and we've found a store.
			  lsu->issue_instruction(chosen_store->instruction_number, this->opcode_helper(chosen_store->opcode),
					  chosen_store->src1_data,chosen_store->src2_data,chosen_store->dest_tag,rob,rrf,false);
			  
			  rs->remove_entry(chosen_store);
		  }
// 		   cout << "chosen store" <<endl;

	  // Instruction issuing to ALU's completed.
	  // Now call ALU to run
	  for (int i = 0; i < NUM_INT_UNITS; ++i)
	  {
		  alu[i].run();
	  }
	  // LSU run
	  lsu->run();

  }

  void Tomasulo::display_arf()
  {
	  arf->display();
  }

  void Tomasulo::display_rrf()
  {
	  rrf->display();
  }

  void Tomasulo::display_rs()
  {
	  rs->display();
  }

  int Tomasulo::opcode_helper(string opcode)
  {
	  if(opcode == "ADD")
		  return ADD;
	  else if(opcode == "SUB")
		  return SUB;
	  else if (opcode == "MUL")
		  return MUL;
	  else if (opcode == "DIV")
		  return DIV;
	  else if (opcode == "AND")
		  return AND;
	  else if(opcode == "OR")
		  return OR;
	  else if(opcode == "XOR")
		  return XOR;
	  else if(opcode == "LOAD")
		  return LOAD;
	  else if(opcode == "STORE")
		  return STORE;
	  else
		  return -1;


  }
  int get_cycles(int opcode)
  {
    return 1;
  }

