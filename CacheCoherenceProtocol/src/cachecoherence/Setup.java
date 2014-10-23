package cachecoherence;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Setup {
	int numProcs;
	Processor[] proc;
	Bus bus;
	MemController mem;
	
	public Setup(int numProcs, int totalInstr, int memBlocks){
		this.numProcs = numProcs;

		mem = new MemController(memBlocks);
		proc = new Processor[numProcs];
		bus = new Bus(proc, mem);
		
		for (int i = 0; i < numProcs; i++){
			proc[i] = new Processor(i, bus, totalInstr);
		}
	}
	
	public void startRun(){	
		Globals.printDebug("SETUP : Program started.");
	
		ExecutorService procExecutor = Executors.newCachedThreadPool();
		for(Processor p : this.proc){
			procExecutor.submit(p);
		}
		procExecutor.submit(bus);
		procExecutor.shutdown();
		
		
		while(!procExecutor.isTerminated()){
		}
		
		
		/*
		Thread[] pool = new Thread[10];
		int index = 0;
		for(Processor p : this.proc){
			pool[index] = new Thread(p);
			index ++;
		}
		
		pool[index] = new Thread(bus);
		index++;
		
		for(int i = 0; i < index; i++){
			pool[i].start();
		}
		for(int i = 0; i < index; i++){
			try{
				pool[i].join();
			}
			catch (Exception e){
				System.out.println(e.getMessage());
				e.printStackTrace();
			}
		}
		
		*/
		
		Globals.printDebug("SETUP : Program exited.");
				
		
	}

}
