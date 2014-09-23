all: main.o Tomasulo.o DecBuff_Entry.o ALU.o ROB.o ARF.o RRF.o 
	
main.o: main.cpp

Tomasulo.o: Tomasulo.h Tomasulo.cpp
	g++ -c Tomasulo.cpp -o Tomasulo.o -fPIC	

ALU.o: ALU.h ALU.o
	g++ -c ALU.cpp -o ALU.o -fPIC

ROB.o: ROB.h ROB.o
	g++ -c ROB.cpp -o ROB.o -fPIC
ARF.o: ARF.h ARF.o
	g++ -c ARF.cpp -o ARF.o -fPIC	
RRF.o: RRF.h RRF.o
	g++ -c RRF.cpp -o RRF.o -fPIC
DecBuff_Entry.o: DecBuff_Entry.h DecBuff_Entry.o
	g++ -c DecBuff_Entry.cpp -o DecBuff_Entry.o -fPIC

clean:	
	rm *.o

	
