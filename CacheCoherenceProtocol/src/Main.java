import cachecoherence.*;

public class Main {

	public static void main(String args[]){

		if (args.length == 1 && args[0].equals("--debug")){
			System.out.println("Debug option enabled.");
			Globals.debug = true;
		}
		
		Setup setup = new Setup(2, 5, 1024);
		setup.startRun();
	}
}
