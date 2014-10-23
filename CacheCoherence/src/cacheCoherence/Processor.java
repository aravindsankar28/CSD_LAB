package cacheCoherence;

import java.util.Random;


class Cache{
	int size;
	ProtocolState[] stateArray;
	int[] tagArray;
	
	public Cache(int size)
	{
		this.size = size;
		stateArray = new ProtocolState[size];
		tagArray = new int[size];
		for(int i = 0; i < size; i++)
			this.stateArray[i] = ProtocolState.I;
	}
}
public class Processor {
	Cache cache;
	int id;
	public Processor(int id)
	{
		this.id = id;
		cache = new Cache(32);
		
	}
	public String generateInstruction()
	{
		String instruction = "";
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
		
/*
		Globals.printDebug("Generated Instruction \""+
								instruction +
								"\" from Processor P"+
								this.id);*/

		return instruction;
		
	}
}
