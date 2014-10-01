#include "Tomasulo.h"
#include <fstream>
#include <iostream>
#include <string>
#define FILE_NAME "program.txt"
#define LATENCY_FILE_NAME "Latencies.txt"
#define REGISTER_FILE_NAME "Regster_File.txt"
#define MEMORY_FILE_NAME "Memory_File.txt"
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

	this->initialize_memory();
	this->initialize_instruction_cycles();
	this->initialize_register_file();

	this->alu = new ALU[NUM_INT_UNITS];
	this->lsu = new LSU(Memory,8);

	for (int i = 0; i < NUM_INT_UNITS; ++i)
	{
		this->alu[i].instruction_cycles = this->instruction_cycles;
	}
	
	this->lsu->instruction_cycles = this->instruction_cycles;

}

void Tomasulo::initialize_register_file()
{

	std::ifstream input(REGISTER_FILE_NAME);
	std::string line;
	int i = 0;
	while(std::getline(input,line))
	{
		arf->entries[i++].data = atoi(line.c_str());
	}
	
}
void Tomasulo::initialize_memory()
{
	std::ifstream input(MEMORY_FILE_NAME);
	std::string line;
	int i = 1;
	
	Memory[0] =0;
	while(std::getline(input,line))
	{
		Memory[i] = atoi(line.c_str());
	}	

}
void Tomasulo::initialize_instruction_cycles()
{
	std::ifstream input(LATENCY_FILE_NAME);
	std::string line;

	while(std::getline(input,line))
	{
		
		string tmp1,tmp2;
		vector <string> cds;
		stringstream ss (line);
		int i = 0;
		  
		ss >> tmp1;
		ss >> tmp2;
		int opcode_code = this->opcode_helper(tmp1);
		int latency = atoi(tmp2.c_str());
		this->instruction_cycles[opcode_code] = latency;
	}
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
	/* Make sure that instruction buffer has enough capacity b4 fetching to it. */
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
	/* Make sure that reservation station and rob have enough capacity b4 fetching to it. */
	while(i < issue_size && instruction_buffer->size() > 0 && rs->size < rs->max_size && rob->get_size() < rob->max_size)
	{

		string instruction = instruction_buffer->front(); // fetch the first instruction from buffer.

		// Now actually decode to get opcode and operands
		Decoded_Instruction *decoded_instruction = new Decoded_Instruction(instruction);

		curr_instr ++; // maintains instruction pointer

		cout << "Decoding Instruction "<<instruction <<" "<<curr_instr <<endl;

		operand dest,src1,src2;

		if(decoded_instruction->opcode != "STORE")
		{	
			// We always have a destination operand if the instruction isn't a store, so the order is as given here.
			dest = decoded_instruction->ops[0];
			src1 = decoded_instruction->ops[1];
			src2 = decoded_instruction->ops[2];
		}
		else
		{
			// In case of a STORE, we only have 2 source operands and no destination operand.
			src1 = decoded_instruction->ops[0];
			src2 = decoded_instruction->ops[1];
		}

		/* Search for a free entry in the reservation station */
		Res_Station_Entry* res_station_entry =  rs->get_free_entry(); 
		res_station_entry->instruction_number = curr_instr;

		/* DESTINATION ALLCOCATE FOR NON-STORE INSTRUCTIONS */
		// Now, check if an RRF entry can be assigned to the destination operand. If not, stop

		int found_dest_reg = 0;

		if(decoded_instruction->opcode != "STORE")
		{
			found_dest_reg =  rrf->find_non_busy_register(); // see if free rrf entry exists

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
		// SOURCE READ FOR ALL INSTRUCTION TYPES
		// Now do src1 
		if ( ! src1.is_immediate )
		{

			
			ARF_Entry arf_entry_src1 = *(arf->get_entry(src1.value));
			if(! arf_entry_src1.busy)
			{
				/* If ARF entry is not busy, get data from rrf itself */
				res_station_entry->src1_data_present= 1;
				res_station_entry->src1_data = arf_entry_src1.data;
			}
			else
			{
				/* If Arf entry busy, get corresponding rrf entry using the tag present in the arf entry. */
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
			/* If operand is immediate, put value directly */
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

		/* Setting entries of reservation station correctly */
		res_station_entry->opcode = decoded_instruction->opcode;
		res_station_entry->exec_over = 0;
		res_station_entry->busy = true;
		rs->size ++;
		
		/* Create a new ROB entry and set entries correctly */
		ROB_Entry* rob_entry = new ROB_Entry(res_station_entry->dest_tag,decoded_instruction->opcode);
		rob_entry->instruction_number = curr_instr;
		

		if(decoded_instruction->opcode == "STORE"){
			/* In case of STORE, the dest tag contains the store index - which is used to main*/
			lsu->rob_position_recent_store = rob->entries.size();
			res_station_entry->dest_tag = this->store_index; 
			rob_entry->tag = this->store_index;
			this->store_index++;
		}

		rob_entry->src1 = res_station_entry->src1_data;
		rob_entry->src2 = res_station_entry->src2_data;

		if(! rob->attempt_push(*rob_entry)) 
			cout <<"ERROR" <<endl;

		instruction_buffer->pop();
		i++;
	}
}




void Tomasulo::simulate()
{
	
	fetch_instructions_to_cache();
	while(rob->get_size()>0 || instruction_buffer->size() >0 || instruction_cache->size() >0 || lsu->store_queue.size() > 0)
	{
		
		cout<<"Start of cycle "<<cycle+1<<endl;
		if(cycle >10)
			break;
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
				 ROB_Entry scratch = rob->scratch_queue.front();
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
	}
	cout << "all done" <<endl;
	
}
bool Tomasulo::commit_instructions()
{
	return rob->attempt_pop();

}
void Tomasulo::execute_instructions()
{
	// Res station should check all entries in arf and rrf and update it's entries.
		
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
			if(! rs_entry->src2_data_present && rs_entry->src2_tag >= 0)
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

	rs->display();

	// Then look for functional units and schedule them  (ALU instructions only first)
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

	// First, find a load which is free.
	
	Res_Station_Entry *chosen_load = NULL;

	for (int i = 0; i < rs->max_size ; ++i)
	{
		Res_Station_Entry* rs_entry = rs->get_entry(i);
		if (rs_entry->busy && rs_entry->opcode == "LOAD" && rs_entry->src1_data_present)
		{
			// First find a ready load		
			int last_store_index = -1;
			// Now traverse thru ROB from head till this load to find if there are any conficts.
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

			chosen_load = rs_entry; // set chosen load appropriately
			break;
		}
	}

	if(chosen_load != NULL)
	{
		// We have a ready load , and no stores before it that can possibly conflict.
		// So issue it !! Not so fast dude !
		// Check if load forwarding is possible. 
		// If not, check if LSU is busy

		bool flag = lsu->is_forwarding_possible(chosen_load->src1_data);
		if(flag)
		{
			// Do load forwarding even if lsu is busy.
			lsu->issue_instruction(chosen_load->instruction_number, this->opcode_helper(chosen_load->opcode),
					chosen_load->src1_data,chosen_load->src2_data,chosen_load->dest_tag,rob,rrf,true);
			rs->remove_entry(chosen_load);

		}
		else if(! lsu->is_busy)
		{
			// If not, issue load to cache only if LOAD unit is not busy.
			lsu->issue_instruction(chosen_load->instruction_number, this->opcode_helper(chosen_load->opcode),
					chosen_load->src1_data,chosen_load->src2_data,chosen_load->dest_tag,rob,rrf,false);
			rs->remove_entry(chosen_load);
		
		}
		else
		{
			// lsu busy -- nothing to do -- put peace and don't schedule any loads.
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
	}


	if (chosen_store != NULL && lsu->store_queue.size() < lsu->store_queue_max_size)
	{

		// Assuming only one store can be removed from res. station and we've found a store.
		lsu->issue_instruction(chosen_store->instruction_number, this->opcode_helper(chosen_store->opcode),
				chosen_store->src1_data,chosen_store->src2_data,chosen_store->dest_tag,rob,rrf,false);

		rs->remove_entry(chosen_store);
	}

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

