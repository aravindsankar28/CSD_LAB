package cacheCoherence;

public class Globals {
	//DEBUG flag
	public static boolean debug;
	
	public static void printDebug(String str){
		if(debug)
			System.out.println("TRACE : " + str);
	}
}
