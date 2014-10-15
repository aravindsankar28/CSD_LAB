/**
 * Processor Thread that generates a memory instruction as and when required
 */

import java.util.*;
/**
 * @author Ganesh
 */
public class Processor implements Runnable {
	Bus bus;
	Cache cache;
	int id;
	private String generateInstruction(){
		String instruction;
		Random random = new Random();
		
		float toss = random.nextFloat();
		if (toss < 0.5){
			instruction = "LOAD R1 ";
			int memLocation = Math.round(random.nextFloat() * 1024);
			instruction += Integer.toString(memLocation);
		}
		else{
			instruction = "STORE ";
			int memLocation = Math.round(random.nextFloat() * 1024);
			instruction += Integer.toString(memLocation) + " R1";
		}
		
		if(Globals.debug){
			System.out.println("TRACE : Generated Instruction \""+
								instruction +
								"\" from Processor P"+
								this.id);
		}
		
		return instruction;
	}
	@Override
	public void run() {
		while(bus.empty[id]){
			String instr = generateInstruction();
			if(bus.empty[this.id]){
				bus.addInstruction(this.id, instr);
			}
		}
	}

}
