all: out/Cache.o out/CacheController.o out/LRU_Cache.o out/LFU_Cache.o out/RR_Cache.o out/Combined.o
	ar rvs out/libcache.a out/*.o
out/Cache.o: Cache.h Cache.cpp
	g++ -c Cache.cpp -o out/Cache.o -fPIC
out/CacheController.o: CacheController.h CacheController.cpp
	g++ -c CacheController.cpp -o out/CacheController.o -fPIC
out/LRU_Cache.o: LRU_Cache.h LRU_Cache.cpp
	g++ -c LRU_Cache.cpp -o out/LRU_Cache.o -fPIC
out/LFU_Cache.o: LFU_Cache.h LFU_Cache.cpp
	g++ -c LFU_Cache.cpp -o out/LFU_Cache.o -fPIC
out/RR_Cache.o: RR_Cache.h RR_Cache.cpp
	g++ -c RR_Cache.cpp -o out/RR_Cache.o -fPIC
out/Combined.o: combine.cpp
	g++ -c combine.cpp -o out/Combined.o -fPIC
clean:	
	rm out/*

	
