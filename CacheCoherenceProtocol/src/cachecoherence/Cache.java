package cachecoherence;

/**
 * A direct-mapped private cache
 * @author ganesh
 *
 */



public class Cache {
	//cache size
	private int size;
	
	//Protocol states of memory locations
	ProtocolState[] stateArray;
	
	
	public Cache(){
		//Number of cache blocks.
		size = 32;
		
		//Directly-mapped state array
		stateArray = new ProtocolState[size];
		
		//All blocks are invalid in the beginning
		for(int i = 0; i < size; i++){
			stateArray[i] = ProtocolState.I;
		}
	}
	
}

