#include "Tomasulo.h"
#include <fstream>
#include <iostream>
#include <string>
#define FILE_NAME "program.txt"

using namespace std;

void Tomasulo::fetch_instructions_to_cache()
{
	
	std::ifstream input(FILE_NAME);
	std::string line;
	
	while(std::getline(input,line))
	{

		instruction_cache->push(line);
	}

}

void Tomasulo::fetch_instructions_to_buffer()
{
	int i = 0;
	while(instruction_buffer->size() < max_instruction_buffer_size && i < issue_size)
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
	while(i < issue_size)
	{
		string instruction = instruction_buffer->front(); // fetch the first instruction from buffer.
		// Now actually decode to get oopcode and perands

		Decoded_Instruction *decoded_instruction = new Decoded_Instruction(instruction);
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

		ARF_Entry arf_entry_dest = arf->get_entry(dest.value); 
		arf_entry_dest.busy = 1; // set busy bit in arf
		arf_entry_dest.tag = dest_reg_rrf; // set tag as the offset in rrf
		RRF_Entry rrf_entry_dest = rrf->get_entry(dest_reg_rrf);
		rrf_entry_dest.valid = 0; // set valid bit of rrf entry = 0 
		rrf_entry_dest.busy = 1; // set busy bit of rrf entry = 1

		rrf->entries[dest_reg_rrf] = rrf_entry_dest;
		arf->entries[dest.value] = arf_entry_dest;

		Res_Station_Entry res_station_entry; // new rs station entry 

		res_station_entry.dest_tag = found_dest_reg; // set destination tag
		// Source Read
		// Now check src1 and src2 in the register files.
		if ( ! src1.is_immediate )
		{
			
			// If ARF entry is not busy
			ARF_Entry arf_entry_src1 = arf->get_entry(src1.value);
			if(! arf_entry_src1.busy)
				{
					res_station_entry.src1_data_present= 1;
					res_station_entry.src1_data = arf_entry_src1.data;
				}
			else
			{
				// Arf entry busy
				// Get corresponding rrf entry using the tag present in the arf entry.
				RRF_Entry rrf_entry_src1 = rrf->get_entry(arf_entry_src1.tag);
				if(! rrf_entry_src1.valid)
				{
					// if rrf entry is not valid, set tag for src1 in the res station entry.
					res_station_entry.src1_tag = arf_entry_src1.tag;
					res_station_entry.src1_data_present = 0;
				}
				else
				{
					// if rrf entry is valid, set data for src1 in the res station entry.
					res_station_entry.src1_data = rrf_entry_src1.data;
					res_station_entry.src1_data_present = 1;	
				}
			}
		}
		else
			{
				// operand is immediate
				res_station_entry.src1_data_present = 1;
				res_station_entry.src1_data = src1.value;
			}


		// Source read for src2
		if ( ! src2.is_immediate )
		{
			
			// If ARF entry is not busy
			ARF_Entry arf_entry_src2 = arf->get_entry(src2.value);
			if(! arf_entry_src2.busy)
				{
					res_station_entry.src2_data_present= 1;
					res_station_entry.src2_data = arf_entry_src2.data;
				}
			else
			{
				// Arf entry busy
				// Get corresponding rrf entry using the tag present in the arf entry.
				RRF_Entry rrf_entry_src2 = rrf->get_entry(arf_entry_src2.tag);
				if(! rrf_entry_src2.valid)
				{
					// if rrf entry is not valid, set tag for src1 in the res station entry.
					res_station_entry.src2_tag = arf_entry_src2.tag;
					res_station_entry.src2_data_present = 0;
				}
				else
				{
					// if rrf entry is valid, set data for src1 in the res station entry.
					res_station_entry.src2_data = rrf_entry_src2.data;
					res_station_entry.src2_data_present = 1;	
				}
			}
		}
		else
			{
				// operand is immediate
				res_station_entry.src2_data_present = 1;
				res_station_entry.src2_data = src2.value;
				
			}
			// Set opcode and busy bit in the entry
			res_station_entry.opcode = decoded_instruction->opcode;
			res_station_entry.exec_over = 0;
			res_station_entry.busy = true;
			
			// Add to station
			if(rs->size < rs->max_size)
				rs->add_entry(res_station_entry);


		instruction_buffer->pop();

		i++;



	}
}

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
}

int get_cycles(int opcode)
{
  return 1;
}

