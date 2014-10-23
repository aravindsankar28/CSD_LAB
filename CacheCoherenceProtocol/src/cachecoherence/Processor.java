package cachecoherence;


import java.util.*;
/**
 * @author Ganesh
 */
public class Processor implements Runnable {
	
	Bus bus;
	Cache cache;
	int id;
	int currInstrNum;
	int totalInstr;
	
	volatile boolean running;
	
	public Processor(int id, Bus bus, int total){
		this.bus = bus;
		this.id = id;
		cache = new Cache();
		currInstrNum = 0;
		totalInstr = total;
	}
	
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
		

		Globals.printDebug("PROC  : Generated Instruction \""+
								instruction +
								"\" from Processor P"+
								this.id);
		
		return instruction;
	}
	
	public boolean isRunning(){
		return running;
	}
	@Override
	public void run() {
		running = true;

		Globals.printDebug("PROC  : Processor "	+
							this.id		+
							" started."	);

		while(currInstrNum < totalInstr){
			if(bus.instructions[this.id].isEmpty()){
				String instr = generateInstruction();
				bus.instructions[this.id].write(instr);
				currInstrNum ++;
			}
		}

		running = false;
	
		Globals.printDebug("PROC  : Processor " + this.id + "done.");
		
		return;
	}

}
