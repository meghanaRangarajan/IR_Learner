testmake: db_decoder.cpp cpp_code.h constants.h
	  g++ db_decoder.cpp -l pigpio -pthread -lsqlite3 -shared -o decoder.so -g
	
driver: db_decoder.cpp cpp_code.h constants.h
	g++ db_decoder.cpp main.cpp -l pigpio -pthread -lsqlite3 
	
.PHONY: clean

clean:
	rm a.out

