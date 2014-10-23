package cachecoherence;



public class MemController {
	
	//number of blocks
	int size;
	MemBlock[] mem;
	MemController(int size){
		this.size = size;
		mem = new MemBlock[size];
	}
}
