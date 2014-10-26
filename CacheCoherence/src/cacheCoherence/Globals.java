package cacheCoherence;

import java.util.Random;

public class Globals {
	//DEBUG flag
	public static boolean debug;
	public static Random random = new Random();
	
	public static void printDebug(String str){
		if(debug)
			System.out.println("TRACE : " + str);
	}
	
	public static int coherenceTrans;
	
	public static int busRd;
	
	public static int busRdEx;
	
	public static int sharedLineRead;
}
