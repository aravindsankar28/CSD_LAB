package cachecoherence;

import java.util.*;

/**
 * Class to return values from @method chooseInstruction()
 * @author ganesh
 *
 */
class ScheduledInstruction{
	String instr;
	int pid;
	
	public ScheduledInstruction(String s, int id){
		instr = s;
		pid = id;
	}
	
	public String getInstr(){
		return instr;
	}
	
	public int getPid(){
		return pid;
	}
}

/**
 * Model of the ICN (here, a bus) as a Runnable
 * @author ganesh
 *
 */
public class Bus implements Runnable {
	
	volatile InstrReg[] instructions;
	Processor[] procs;
	volatile boolean running;
	MemController mem;
	int numProcs;
	
	
	//ID of processor most recently served
	int rspId;
	
	/**
	 * Constructor
	 * 
	 * @param numProcs Number of processors in the ICN
	 */
	public Bus(Processor[] procs, MemController mem){
		instructions = new InstrReg[procs.length];
		this.numProcs = procs.length;
		this.mem = mem;
		this.procs = procs;
		for (int i = 0; i < numProcs; i++){
			instructions[i] = new InstrReg();
		}
		rspId = -1;
	}
	
	/**
	 * Bus Arbitration Logic
	 * @return A ScheduledInstruction containing instruction and processor id 
	 * 		   chosen for servicing
	 */
	public ScheduledInstruction chooseInstruction(int nestSize){
		String chosenInstr = "";
		int chosenPid = -1;
		
		if(nestSize == 10){
			Globals.printDebug("BUS   : nestSize reached 10. Aborting...\n");
			return null;
		}
		if(instructions[0].isEmpty() && !instructions[1].isEmpty()){
			chosenPid = 1;
		}
		else if(instructions[1].isEmpty() && !instructions[0].isEmpty()){
			chosenPid = 0;
		}
		else if(!instructions[1].isEmpty() && !instructions[0].isEmpty()){
			Random r = new Random();
			
			float mult = 0;
			if(rspId == 0)
				mult = -1;
			else if(rspId == 1)
				mult = 1;
			
			//setting bias
			float bias = (float) 0.25;
			//ensures same processor doesn't get served repeatedly
			float prob = r.nextFloat();
			Globals.printDebug("BUS   : Generated prob = " + prob );
			
			if(prob < (0.5 + mult*bias)){
				//choose processor 0
				Globals.printDebug("BUS   : choosing P0 if probability < " + 
														(0.5 + mult*bias));
				chosenPid = 0;
			}
			else{
				//choose processor 1
				chosenPid = 1;
			}
		}
		else{
			chosenPid = -1;
		}
		
		if(chosenPid >= 0){
			chosenInstr = instructions[chosenPid].value;		
			return new ScheduledInstruction(chosenInstr, chosenPid);
		}
		
		else
			return chooseInstruction(nestSize + 1);
	}
	

	public boolean mustRun(){
		for (Processor p : procs){
			if(p.running)
				return true;
		}
		return false;
	}
	/**
	 * Run function
	 */
	public void run(){
		Globals.printDebug("BUS   : Bus started.");
		running = true;
		
		while(!mustRun());
		
		while(this.mustRun()){
//			System.out.println("In while : Bus");
			
			ScheduledInstruction si = chooseInstruction(0);
			String currInstr = si.getInstr();
			rspId = si.getPid();
			if(rspId >= 0)
				instructions[rspId].flush();
			
			Globals.printDebug("BUS   : Processor "+ rspId +" chosen.");
			synchronized(mem){
				//TODO: Fill up here
			}
		}
		
		if(Globals.debug){
			System.out.println("TRACE : BUS : Bus stopped.");
		}
		running = false;
	}
}
