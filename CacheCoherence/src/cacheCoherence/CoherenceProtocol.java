package cacheCoherence;

import java.util.Random;

class DecodedInstruction {
	String type;
	int block_number;
	int pid;

	public DecodedInstruction(String instruction, int pid) {
		String[] splitInstruction = instruction.split(" ");
		
		this.pid = pid;
		this.type = splitInstruction[0];
		
		if(splitInstruction[0].contentEquals("LOAD"))
		{	
			this.type = "READ";
			this.block_number = Integer.parseInt(splitInstruction[2]);
		}
		
		else
		{
			this.type = "WRITE";
			this.block_number = Integer.parseInt(splitInstruction[1]);
		}
		
		
	}
	void print()
	{
		System.out.println(this.type+" "+this.block_number+" ("+this.pid+")");
	}

}

public class CoherenceProtocol {
	Processor[] processors;

	public CoherenceProtocol(Processor[] processors) {
		this.processors = processors;
	}
	
	ProtocolState getState(DecodedInstruction instruction)
	{
		int cache_line_number = instruction.block_number % processors[instruction.pid].cache.size;
		int tag = instruction.block_number/processors[instruction.pid].cache.size;
		
		if(tag == processors[instruction.pid].cache.tagArray[cache_line_number])
		{
			return processors[instruction.pid].cache.stateArray[cache_line_number];
		}
		else
			return ProtocolState.I;
	}
	boolean needToAccessBus(DecodedInstruction instruction)
	{
		ProtocolState state = getState(instruction);
		if(instruction.type == "READ" && (state == ProtocolState.S  || state == ProtocolState.E || state == ProtocolState.M)) 
		{
			return false;
		}
		else if(instruction.type == "WRITE" && (state == ProtocolState.E || state == ProtocolState.M))
		{
			return false;
		}
		return true;
	}
	
	void executeMESIInstruction(DecodedInstruction instruction,int pid)
	{
		//TODO: add code here for state transitions - Look at pid, instruction params and change state accordingly.
		ProtocolState current_proc_state = getState(instruction);
		
		switch(current_proc_state)
		{
		case E:
			break;
		case I:
			break;
		case M:
			break;
		case O:
			break;
		case S:
			break;
		default:
			break;
		
		}
		
	}
	public void simulateMESIProtocol() {
		int cycle = 0;
		boolean flag_0,flag_1;
		boolean p0_has_executed_last_cycle = false,p1_has_executed_last_cycle = false;
		Random r = new Random();
		DecodedInstruction instruction_p0 = new DecodedInstruction(processors[0].generateInstruction(),0);
		DecodedInstruction instruction_p1 = new DecodedInstruction(processors[1].generateInstruction(),1);
		while (cycle < 10) {
			
			if(p0_has_executed_last_cycle)
				instruction_p0 = new DecodedInstruction(processors[0].generateInstruction(),0);
			
			if(p1_has_executed_last_cycle)
				instruction_p1 = new DecodedInstruction(processors[1].generateInstruction(),1);
			
			p0_has_executed_last_cycle = false;
			p1_has_executed_last_cycle = false;
			
			flag_0 = needToAccessBus(instruction_p0);
			flag_1 = needToAccessBus(instruction_p1);
			
			if(! flag_0)
			{
				executeMESIInstruction(instruction_p0, 0);
				p0_has_executed_last_cycle = true;
				Globals.printDebug("Executing instruction "+instruction_p0.type+ " "+instruction_p0.block_number+" on processor 0");
			}
			if( ! flag_1)
			{
				executeMESIInstruction(instruction_p1, 1);
				p0_has_executed_last_cycle = true;

				Globals.printDebug("Executing instruction "+instruction_p1.type+ " "+instruction_p1.block_number+" on processor 1");
			}
			
			if(flag_0 && flag_1)
			{
				// only one is allowed.
				// the rejected guy will appear in the next cycle.
				float prob = r.nextFloat();
				
				if(prob < 0.5)
					{
						executeMESIInstruction(instruction_p0, 0);
						p0_has_executed_last_cycle = true;
						Globals.printDebug("Executing instruction "+instruction_p0.type+ " "+instruction_p0.block_number+" on processor 0");
					}
				else
					{
						executeMESIInstruction(instruction_p1, 1);
						p1_has_executed_last_cycle = true;
						Globals.printDebug("Executing instruction "+instruction_p1.type+ " "+instruction_p1.block_number+" on processor 1");
					}
				
			}
			
			

			cycle++;
		}
	}
}
