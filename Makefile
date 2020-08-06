EXE=gra

$(EXE):
	g++ src/*.cpp -l GL -l GLU -l glut -o $(EXE)

.PHONY: clean run

clean:
	rm -f $(EXE)

run: $(EXE)
	./gra
