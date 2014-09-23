all: main.o Tomasulo.o DecBuff_Entry.o ALU.o ROB.o ARF.o RRF.o 
	g++ -o run main.o Tomasulo.o DecBuff_Entry.o ALU.o ROB.o ARF.o RRF.o
main.o: main.cpp

Tomasulo.o: Tomasulo.h Tomasulo.cpp
	g++ -c Tomasulo.cpp -o Tomasulo.o -fPIC	

DecBuff_Entry.o: DecBuff_Entry.h DecBuff_Entry.cpp
	g++ -c DecBuff_Entry.cpp -o DecBuff_Entry.o -fPIC

ALU.o: ALU.h ALU.cpp
	g++ -c ALU.cpp -o ALU.o -fPIC

ROB.o: ROB.h ROB.cpp
	g++ -c ROB.cpp -o ROB.o -fPIC
ARF.o: ARF.h ARF.cpp
	g++ -c ARF.cpp -o ARF.o -fPIC	
RRF.o: RRF.h RRF.cpp
	g++ -c RRF.cpp -o RRF.o -fPIC


clean:	
	rm *.o

	
