CC      = c++
#CFLAGS  = -I../../boost_lib/ -I../src/ -I../../boost_lib/include/
CFLAGS  = -I./src/ -I boost_1_60_0 -std=c++11
LDFLAGS = -g
ODIR    = ./obj
_DEPS   = IPort.hpp TPortIn.hpp TPortOut.hpp IBlock.hpp
SRC     = ./src
DEPS    = $(patsubst %,$(SRC)/%,$(_DEPS))

_OBJ    = TPortIn.o TPortOut.o IPort.o TBlock.o main.o
OBJ     = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRC)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

#../obj/ut.o: ut.cpp
#	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f ./obj/*.o main.exe  
