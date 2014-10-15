import java.util.*;
public class Bus {
	String[] instructions;
	boolean[] empty;
	
	//Snooping => reading this value
	String currInstruction;
	
	public void addInstruction(int procID, String instr){
		instructions[procID] = instr;
		empty[procID] = false;
	}
	
	//Bus Arbitration Logic
	public int chooseInstruction(){
		Random r = new Random();
		int index = r.nextInt(instructions.length);
		currInstruction = instructions[index];
		return index;
	}
	
}
