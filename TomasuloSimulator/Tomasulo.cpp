#include "Tomasulo.h"
#include <fstream>
#include <iostream>
#include <string>
#define FILE_NAME "program.txt"

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
	while(instruction_buffer->size() < max_instruction_buffer_size && i < issue_size && instruction_cache->size() >0)
	{
		string instruction = instruction_cache->front();
		instruction_cache->pop();
		instruction_buffer->push(instruction);
		i++;
	}
}

void Tomasulo::decode_instructions()
{
	int i = 0;
	while(i < issue_size && instruction_buffer->size() > 0 && rs->size < rs->max_size && rob->get_size() < rob->max_size)
	{
		string instruction = instruction_buffer->front(); // fetch the first instruction from buffer.
		// Now actually decode to get oopcode and perands

		Decoded_Instruction *decoded_instruction = new Decoded_Instruction(instruction);
		curr_instr ++;
		/*cout << decoded_instruction->opcode<<" "<< decoded_instruction->ops[0].value << 
		decoded_instruction->ops[1].value << decoded_instruction->ops[2].value<<  endl;*/

		operand dest = decoded_instruction->ops[0];
		operand src1 = decoded_instruction->ops[1];
		operand src2 = decoded_instruction->ops[2];

		// Now, check if an RRF entry can be assigned to the destination operand. If not, stop
		int found_dest_reg =  rrf->find_non_busy_register();

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

		Res_Station_Entry* res_station_entry =  rs->get_free_entry();
		res_station_entry->instruction_number = curr_instr;
		res_station_entry->dest_tag = found_dest_reg; // set destination tag
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


		// Source read for src2
		if ( ! src2.is_immediate )
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
		else
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

	while(cycle < 4)
	{
		cout << "At start of Cycle "<< cycle << endl;
		commit_instructions();
		execute_instructions();
		decode_instructions();
		fetch_instructions_to_buffer();
		cycle ++;

		for (int i = 0; i < NUM_INT_UNITS; ++i)
		{
			alu[i].commit();
		}
		//display_arf();
		//display_rrf();
		//display_rs();
	}
	cout << instruction_buffer->size()<<endl;

}
void Tomasulo::commit_instructions()
{
	rob->attempt_pop();
}
void Tomasulo::execute_instructions()
{
	// Res station should check all entries in arf and rrf and update it's entries. (Why ?)
	// 
	
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

			if(! rs_entry->src2_data_present)
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
						cout << "Can schedule on ALU " << j <<" : ";
						rs_entry->display();
						alu[j].issue_instruction(rs_entry->instruction_number,opcode_code,rs_entry->src1_data,rs_entry->src2_data,rs_entry->dest_tag,rob,rrf);
						// Issued instruction to a free ALU - so  remove that entry from reservation station and break.
						rs->remove_entry(rs_entry);
						break;
					}
				}
			}
			else
			{
				// to handle later 
			}
		}
	}
	// Instruction issuing to ALU's completed.
	// Now call ALU to run
	for (int i = 0; i < NUM_INT_UNITS; ++i)
	{
		alu[i].run();
	}

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

