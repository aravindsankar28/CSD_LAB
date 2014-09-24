#include "Tomasulo.h"
#include <fstream>
#include <iostream>
#include <vector>
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
		cout << decoded_instruction->opcode<<" "<< decoded_instruction->ops[0].value << 
		decoded_instruction->ops[1].value << decoded_instruction->ops[2].value<<  endl;
		
		// Now, check if an RRF entry can be assigned to the destination operand. If not, stop
		int found_dest_reg =  rrf->find_non_busy_register();

		if(found_dest_reg < 0)
			break;
		int dest_reg_rrf = found_dest_reg;
		// doing destination allocate now. Not dest will always be a register

		ARF_Entry arf_entry = arf->get_entry(decoded_instruction->ops[0].value); 
		arf_entry.busy = 1; // set busy bit in arf
		arf_entry.tag = dest_reg_rrf; // set tag as the offset in rrf
		RRF_Entry rrf_entry = rrf->get_entry(dest_reg_rrf);
		rrf_entry.valid = 0; // set valid bit of rrf entry = 0 
		rrf_entry.busy = 1; // set busy bit of rrf entry = 1

		// Now check src1 and src2 in the register files  - TODO


		instruction_buffer->pop();

		i++;



	}
}