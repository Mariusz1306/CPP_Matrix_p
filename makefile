Main: Main.o Matrix.o
	g++ -Wall $^ -o $@

Matrix.o: Matrix.cpp Matrix.h
	g++ -c -Wall $< -o $@

Main.o: Main.cpp Matrix.h
	g++ -c -Wall $< -o $@

.PHONY: clean

clean:
	-rm Matrix.o Main.o Main
