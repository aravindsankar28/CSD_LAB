import cacheCoherence.CoherenceProtocol;
import cacheCoherence.Globals;
import cacheCoherence.Processor;


public class Main {

	/**
	 * @param args
	 */
	
	
	
	
	public static void main(String[] args) {
		
		if (args.length == 1 && args[0].equals("--debug")){
			System.out.println("Debug option enabled.");
			//Globals.debug = true;
		}
		
		Processor[] processors = new Processor[2];

		processors[0] = new Processor(0);
		processors[1] = new Processor(1);
	
		
		CoherenceProtocol cp = new CoherenceProtocol(processors);
		cp.simulateMOESIProtocol();
	}

}
