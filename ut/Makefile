CC      = c++
CFLAGS  = -I../src/ -I ../boost_1_60_0 -std=c++11
LDFLAGS = -g -lm
ODIR    = ../obj
_DEPS   = IBlock.hpp IData.hpp IPort.cpp IPort.hpp TBlock.cpp TBlock.hpp Tcnexception.hpp TConstBlock.hpp TData.hpp TDataVector.hpp TMultiplierBlock.hpp TPortIn.cpp TPortIn.hpp TPortOut.cpp TPortOut.hpp TReaderFromOctMatrix.cpp TReaderFromOctMatrix.hpp TWriterToOctMatrix.cpp TWriterToOctMatrix.hpp

SRC     = ../src
DEPS    = $(patsubst %,$(SRC)/%,$(_DEPS))

_OBJ    = IPort.o TBlock.o TPortIn.o TPortOut.o ut.o TReaderFromOctMatrix.o TWriterToOctMatrix.o TKohonen.o
OBJ     = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRC)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

example: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
#	./example

../obj/ut.o: ut.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f ../obj/*.o example  
