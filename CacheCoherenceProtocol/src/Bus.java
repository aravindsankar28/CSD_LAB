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
	
	InstrReg[] instructions;
	MemController mem;
	int numProcs;
	
	//ID of processor most recently served
	int rspId;
	
	/**
	 * Constructor
	 * 
	 * @param numProcs Number of processors in the ICN
	 */
	public Bus(int numProcs){
		instructions = new InstrReg[numProcs];
		this.numProcs = numProcs;
		rspId = -1;
	}
	
	/**
	 * Bus Arbitration Logic
	 * @return A ScheduledInstruction containing instruction and processor id 
	 * 		   chosen for servicing
	 */
	public ScheduledInstruction chooseInstruction(){
		String chosenInstr = "";
		int chosenPid = -1;
		
		if(instructions[0].isEmpty() && !instructions[1].isEmpty()){
			chosenPid = 1;
		}
		else if(instructions[1].isEmpty() && !instructions[0].isEmpty()){
			chosenPid = 0;
		}
		else if(!instructions[1].isEmpty() && !instructions[0].isEmpty()){
			Random r = new Random();
			
			int mult = 0;
			if(rspId == 0)
				mult = -1;
			else if(rspId == 1)
				mult = 1;
			
			//setting bias
			float bias = (float) 0.25;
			
			//ensures same processor doesn't get served repeatedly
			float prob = r.nextFloat();
			if(prob < 0.5 + mult*bias){
				//choose processor 0
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
		
		if(chosenPid >= 0)
			chosenInstr = instructions[chosenPid].value;		
		
		return new ScheduledInstruction(chosenInstr, chosenPid);
	}
	
	/**
	 * Run function
	 */
	public void run(){
		while(true){
			ScheduledInstruction si = chooseInstruction();
			String currInstr = si.getInstr();
			rspId = si.getPid();
			synchronized(mem){
				//TODO: Fill up here
			}
		}
	}
}
