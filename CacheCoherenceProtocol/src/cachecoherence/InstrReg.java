package cachecoherence;


public class InstrReg {
	volatile String value;
	volatile boolean empty;
	
	InstrReg(){
		value = "";
		empty = true;
	}
	
	public void write(String val){
		value = val;
		empty = false;
	}
	
	public void flush(){
		value = "";
		empty = true;
	}
	
	public boolean isEmpty(){
		return empty;
	}
}
