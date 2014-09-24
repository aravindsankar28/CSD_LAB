all: main.o Tomasulo.o Decoded_Instruction.o ALU.o ROB.o ARF.o RRF.o ResStation.o
	g++ -o run main.o Tomasulo.o Decoded_Instruction.o ALU.o ROB.o ARF.o RRF.o ResStation.o
main.o: main.cpp

Tomasulo.o: Tomasulo.h Tomasulo.cpp
	g++ -c Tomasulo.cpp -o Tomasulo.o -fPIC	

Decoded_Instruction.o: Decoded_Instruction.h Decoded_Instruction.cpp
	g++ -c Decoded_Instruction.cpp -o Decoded_Instruction.o -fPIC

ResStation.o: ResStation.h ResStation.cpp
	g++ -c ResStation.cpp -o ResStation.o -fPIC

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

	