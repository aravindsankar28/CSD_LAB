package cachecoherence;

/**
 * A direct-mapped private cache
 * 
 * @author ganesh
 * 
 */

public class Cache {
	// cache size
	Bus bus;
	int id;
	private int size;

	// Protocol states of memory locations
	ProtocolState[] stateArray;
	MemBlock[] dataArray;
	int[] tagArray;
	public Cache(Bus bus) {
		// Number of cache blocks.
		size = 32;

		// Directly-mapped state array
		stateArray = new ProtocolState[size];
		dataArray = new MemBlock[size];
		tagArray = new int[size];
		// All blocks are invalid in the beginning
		for (int i = 0; i < size; i++) {
			stateArray[i] = ProtocolState.I;
		}
		this.bus = bus;
	}

	boolean executeInstruction(String instr) {
		String instruction_type;
		int block_number;
		int cache_line_number;
		int tag;
		
		String[] splitInstruction = instr.split(" ");

		instruction_type = splitInstruction[0];

		if (instruction_type.contentEquals("LOAD")) {
			block_number = Integer.parseInt(splitInstruction[2]);
			
			cache_line_number = block_number % this.size;
			tag = block_number / this.size;
			
			System.out.println(instruction_type + " "+ block_number);
			ProtocolState state = this.stateArray[cache_line_number];
			
			if (this.tagArray[cache_line_number] != tag)
				state = ProtocolState.I;
			
			switch (state) {
			case I: {
				System.out.println("I state");
				MemBlock block = bus.readFromMemory(block_number);
				bus.finished_0 = false;
				bus.finished_1 = false;
				
				
				
				this.stateArray[cache_line_number] = ProtocolState.S;
				this.dataArray[cache_line_number] = block;
				this.tagArray[cache_line_number] = tag;
			}
				break;
			case E:
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

		} else {
			block_number = Integer.parseInt(splitInstruction[1]);
			block_number = block_number / this.size;
			
			cache_line_number = block_number % this.size;
			tag = block_number / this.size;
			
			System.out.println(instruction_type + " " + block_number);

		}
		return false;
	}

}
