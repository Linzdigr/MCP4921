GPP=g++
CFLAGS=-I.
OBJ = main.o MCP4921.o

%.o: %.c $(DEPS)
	$(GPP) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(GPP) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~
