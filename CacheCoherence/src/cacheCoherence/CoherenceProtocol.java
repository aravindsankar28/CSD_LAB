package cacheCoherence;

import java.util.Random;

/**
 * Decoded version of instruction on the bus
 * @author aravind
 *
 */

class DecodedInstruction {
	
	String type;		//READ or WRITE
	int blockNumber;	
	int pid;

	/**
	 * Constructor. 
	 * @param instruction : A string of the form <TYPE> <ADDR>, 
	 * 						where TYPE can be {READ, WRITE} and
	 * 		 				0 <= ADDR < 1024
	 * @param pid		  : ID of processor issuing the instruction
	 */
	public DecodedInstruction(String instruction, int pid) {
		
		//split instruction to get words
		String[] splitInstruction = instruction.split(" ");
		
		this.pid = pid;
		
		//Generate decoded instruction
		if(splitInstruction[0].contentEquals("LOAD"))
		{	
			this.type = "READ";
			this.blockNumber = Integer.parseInt(splitInstruction[2]);
		}
		
		else if (splitInstruction[0].contentEquals("STORE"))
		{
			this.type = "WRITE";
			this.blockNumber = Integer.parseInt(splitInstruction[1]);
		}
		
		else{
			Globals.printDebug("Unknown instruction : " + splitInstruction[0]);
		}
	}
	
	void printDecodedInstruction()
	{
		System.out.println(this.type+" "+this.blockNumber+" ("+this.pid+")");
	}

}


public class CoherenceProtocol {
	Processor[] processors;

	/**
	 * Constructors
	 * @param processors : Array containing the processors in the system
	 */
	public CoherenceProtocol(Processor[] processors) {
		this.processors = processors;
	}
	
	/**
	 * Get state of reqd block in the given cache
	 * @param instruction : Decoded instruction object
	 * @param cache		  : Cache to check in
	 * @return			  : ProtocolState of block in @param cache
	 */
	ProtocolState getStateFromCache(DecodedInstruction instruction, Cache cache)
	{
		//get cache line and tag of the block required
		int cacheLine = instruction.blockNumber % cache.size;
		int tag = instruction.blockNumber/cache.size;
		
		//if requested block is present in the cache, return it
		
		if(tag == cache.tagArray[cacheLine])
		{
			return cache.stateArray[cacheLine];
		}
		
		//requested block not present. Return I state.
		return ProtocolState.I;
	}
	
	/**
	 * Get state of the required block in the processor issuing the instruction
	 * @param instruction DecodedInstruction object
	 * @return ProtocolState of required block in the processor
	 */
	ProtocolState getState(DecodedInstruction instruction)
	{
		Cache cache = processors[instruction.pid].cache;
		return getStateFromCache(instruction, cache);
	}
	
	ProtocolState getOtherState(DecodedInstruction instruction)
	{
		//set cache to other Cache
		Cache cache = processors[1 - instruction.pid].cache;
		return getStateFromCache(instruction, cache);
	}
	/**
	 * Find out if a given instruction needs to access the bus or not.
	 * @param instruction DecodedInstruction object.
	 * @return true, if a bus access is required. false otherwise
	 */
	boolean needToAccessBus(DecodedInstruction instruction)
	{
		ProtocolState state = getState(instruction);
		
		//Reading from S, E or M will not generate a bus access
		if(instruction.type == "READ" && (state == ProtocolState.S  || state == ProtocolState.E || state == ProtocolState.M)) 
		{
			return false;
		}
		
		//Writing when in E or M state will not generate a bus access
		else if(instruction.type == "WRITE" && (state == ProtocolState.E || state == ProtocolState.M))
		{
			return false;
		}
		
		//Remaining all will generate a bus access
		return true;
	}
	
	/**
	 * Execute an instruction, assuming MESI protocol
	 * @param instruction : Decoded instruction object
	 * @param pid : Id of processor issuing the instruction
	 */
	void executeMESIInstruction(DecodedInstruction instruction,int pid)
	{
		//TODO: add code here for state transitions - Look at pid, instruction params and change state accordingly.
		ProtocolState currProcState = getState(instruction);
		int reqdBlock = instruction.blockNumber;
		String instrType = instruction.type;
		Cache myCache = processors[pid].cache;
		Cache otherCache = processors[1-pid].cache;
		
		switch(currProcState)
		{
		
		case E:
			//if write, then invalidate other blocks
			if(instrType.equals("WRITE")){
				myCache.tagArray[myCache.getCacheLine(reqdBlock)] = reqdBlock/myCache.size;
				myCache.stateArray[myCache.getCacheLine(reqdBlock)] = ProtocolState.M;
			}
			
			//nothing to do if it is a read
			break;
		
		case I:
			if(getOtherState(instruction) != ProtocolState.I){
				myCache.tagArray[myCache.getCacheLine(reqdBlock)] = reqdBlock/myCache.size;
				myCache.stateArray[myCache.getCacheLine(reqdBlock)] = ProtocolState.S;
				otherCache.stateArray[otherCache.getCacheLine(reqdBlock)] = ProtocolState.S;
			}
			else{
				myCache.tagArray[myCache.getCacheLine(reqdBlock)] = reqdBlock/myCache.size;
				myCache.stateArray[myCache.getCacheLine(reqdBlock)] = ProtocolState.E;
			}
			break;
		
		case M:
			//Nothing to do if block is in Modified state
			break;
		
		case S:
			if (instrType == "WRITE"){
				myCache.tagArray[myCache.getCacheLine(reqdBlock)] = reqdBlock/myCache.size;
				myCache.stateArray[myCache.getCacheLine(reqdBlock)] = ProtocolState.M;
				otherCache.stateArray[otherCache.getCacheLine(reqdBlock)] = ProtocolState.I;
			}
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
				Globals.printDebug("Executing instruction "+instruction_p0.type+ " "+instruction_p0.blockNumber+" on processor 0");
			}
			if( ! flag_1)
			{
				executeMESIInstruction(instruction_p1, 1);
				p0_has_executed_last_cycle = true;

				Globals.printDebug("Executing instruction "+instruction_p1.type+ " "+instruction_p1.blockNumber+" on processor 1");
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
						Globals.printDebug("Executing instruction "+instruction_p0.type+ " "+instruction_p0.blockNumber+" on processor 0");
					}
				else
					{
						executeMESIInstruction(instruction_p1, 1);
						p1_has_executed_last_cycle = true;
						Globals.printDebug("Executing instruction "+instruction_p1.type+ " "+instruction_p1.blockNumber+" on processor 1");
					}
				
			}
			cycle++;
		}
	}
}
