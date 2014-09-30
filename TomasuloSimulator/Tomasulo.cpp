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
    this->curr_instr = 0;
    this->alu = new ALU[NUM_INT_UNITS];
    this->lsu = new LSU(Memory,8);
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
			src1 = decoded_instruction->ops[0];
			src2 = decoded_instruction->ops[1];
		}

		cout << src1.is_immediate <<endl;
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

	while(rob->get_size()>0 || instruction_buffer->size() >0 || instruction_cache->size() >0 )
	{
		cout << "At start of Cycle "<< cycle << endl;
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
				}
				rob->scratch_queue.pop_front();
			}
			// not found => put peace
		}

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

	// First, find a load which has least ip and is ready.
	int min_i = 0;
	int min_instruction_number = -1;
	bool any_load_found = false;

	for (int i = 0; i < rs->max_size ; ++i)
	{
		Res_Station_Entry* rs_entry = rs->get_entry(i);
		if (rs_entry->busy && rs_entry->opcode == "LOAD" && rs_entry->src1_data_present)
		{
			// this entry is a load and is ready. 
			if(min_instruction_number < 0 || rs_entry->instruction_number < min_instruction_number )
				{
					min_i = i;
					min_instruction_number = rs_entry->instruction_number;
				}

		}
		else if(rs_entry->busy && rs_entry->opcode == "LOAD")
			any_load_found = true;

	}
	// Now found the first ready load in program order -- Now check that there are no other stores in prog. order b4 it that 
	// could a possible alias. If stores exist , stall.
	if(min_instruction_number >= 0 )
	{
		// which means we found some load that is ready.
		// Now search for any stores to see if we can have any conflicts.
		bool is_conflicting = false;
		for (int i = 0; i < rs->max_size; ++i)
		{
			Res_Station_Entry* rs_entry = rs->get_entry(i);
			// look for a store b4 it in program order
			if (rs_entry->busy && rs_entry->opcode == "STORE" && rs_entry->instruction_number < min_instruction_number)
			{
				if((rs_entry->src1_data_present && rs_entry->src1_data == rs->get_entry(min_i)->src1_data) || ! rs_entry->src1_data_present)
					{
						is_conflicting = true;
						break;
						// there exists a conflicting store - so stall
					}
			}
		}

		if(! is_conflicting)
		{
			// We have a ready load , and no stores before it that can possibly conflict.
			// So issue it !! Not so fast dude !
			// Check if load forwarding is possible. 
			// If not, check if LSU is busy

			bool flag = lsu->is_forwarding_possible(rs->get_entry(min_i)->src1_data);
			if(flag)
			{
				rs->remove_entry(rs->get_entry(min_i));
				// do load forwarding even if lsu is busy
				lsu->issue_instruction(rs->get_entry(min_i)->instruction_number, this->opcode_helper(rs->get_entry(min_i)->opcode),
					rs->get_entry(min_i)->src1_data,rs->get_entry(min_i)->src2_data,rs->get_entry(min_i)->dest_tag,rob,rrf,true);

			}
			else if(! lsu->is_busy)
			{

				lsu->issue_instruction(rs->get_entry(min_i)->instruction_number, this->opcode_helper(rs->get_entry(min_i)->opcode),
					rs->get_entry(min_i)->src1_data,rs->get_entry(min_i)->src2_data,rs->get_entry(min_i)->dest_tag,rob,rrf,false);

				rs->remove_entry(rs->get_entry(min_i));
				// lsu not busy
			}
			else
			{
				// lsu busy -- nothing to do -- put peace and stall.

			}

		}

	}
	else if(any_load_found)
	{
		// which means no load was found that was ready. 
		// in such a case put peace and stall
	}
	else
	{
		// No loads found, so check for stores.
		// Find first ready store according to program order.
		int min_store_i = -1;
		int min_store_instruction_number = -1;
		for (int i = 0; i < rs->max_size ; ++i)
		{
			Res_Station_Entry* rs_entry = rs->get_entry(i);
			if (rs_entry->busy && rs_entry->opcode == "STORE" && rs_entry->src1_data_present && rs_entry->src2_data_present)
			{
				
				if(min_store_instruction_number < 0 || rs_entry->instruction_number < min_store_instruction_number )
					{
						min_store_i = i;
						min_store_instruction_number = rs_entry->instruction_number;
					}
			}

		}
		if (min_store_instruction_number >= 0 && lsu->store_queue.size() < lsu->store_queue_max_size)
		{
			
			// Assuming only one store can be removed from res. station and we've found a store.
			lsu->issue_instruction(rs->get_entry(min_store_i)->instruction_number, this->opcode_helper(rs->get_entry(min_store_i)->opcode),
					rs->get_entry(min_store_i)->src1_data,rs->get_entry(min_store_i)->src2_data,rs->get_entry(min_store_i)->dest_tag,rob,rrf,false);

			rs->remove_entry(rs->get_entry(min_store_i));
		}



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

