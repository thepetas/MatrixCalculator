CXX=g++
LD=g++
CFLAGS=-Wall -pedantic -std=c++11 -Wno-long-long -O0 -ggdb
NAME=matrixcalculator
src/AbstractMatrix.o: src/AbstractMatrix.cpp src/AbstractMatrix.h \
 src/CFraction.h src/CException.h src/CBuilder.h src/CDiagonalMatrix.h \
 src/CMatrix.h src/CSparseMatrix.h
	$(CXX) $< $(CFLAGS) -c -o $@

src/CBuilder.o: src/CBuilder.cpp src/CBuilder.h src/AbstractMatrix.h \
 src/CFraction.h src/CException.h src/CDiagonalMatrix.h src/CMatrix.h \
 src/CSparseMatrix.h
	$(CXX) $< $(CFLAGS) -c -o $@

src/CCalculator.o: src/CCalculator.cpp src/CCalculator.h src/CFraction.h \
 src/CException.h src/AbstractMatrix.h src/CBuilder.h \
 src/CDiagonalMatrix.h src/CMatrix.h src/CSparseMatrix.h \
 src/CExpression.h
	$(CXX) $< $(CFLAGS) -c -o $@

src/CDiagonalMatrix.o: src/CDiagonalMatrix.cpp src/CDiagonalMatrix.h \
 src/AbstractMatrix.h src/CFraction.h src/CException.h src/CBuilder.h \
 src/CMatrix.h src/CSparseMatrix.h
	$(CXX) $< $(CFLAGS) -c -o $@

src/CException.o: src/CException.cpp src/CException.h
	$(CXX) $< $(CFLAGS) -c -o $@
src/CExpression.o: src/CExpression.cpp src/CExpression.h src/AbstractMatrix.h \
 src/CFraction.h src/CException.h src/CBuilder.h src/CDiagonalMatrix.h \
 src/CMatrix.h src/CSparseMatrix.h
	$(CXX) $< $(CFLAGS) -c -o $@

src/CFraction.o: src/CFraction.cpp src/CFraction.h src/CException.h
	$(CXX) $< $(CFLAGS) -c -o $@

src/CMatrix.o: src/CMatrix.cpp src/CMatrix.h src/CFraction.h src/CException.h \
 src/AbstractMatrix.h src/CBuilder.h src/CDiagonalMatrix.h \
 src/CSparseMatrix.h
	$(CXX) $< $(CFLAGS) -c -o $@

src/CSparseMatrix.o: src/CSparseMatrix.cpp src/CSparseMatrix.h \
 src/AbstractMatrix.h src/CFraction.h src/CException.h src/CBuilder.h \
 src/CDiagonalMatrix.h src/CMatrix.h
	$(CXX) $< $(CFLAGS) -c -o $@

src/main.o: src/main.cpp src/CCalculator.h src/CFraction.h src/CException.h \
 src/AbstractMatrix.h src/CBuilder.h src/CDiagonalMatrix.h src/CMatrix.h \
 src/CSparseMatrix.h src/CExpression.h
	$(CXX) $< $(CFLAGS) -c -o $@

calculator: src/AbstractMatrix.o src/CBuilder.o src/CCalculator.o src/CDiagonalMatrix.o src/CException.o \
 src/CExpression.o src/CFraction.o src/CMatrix.o src/CSparseMatrix.o src/main.o
	$(LD) -o ./$(NAME) $^ 

compile: calculator

run:
	./${NAME} -f examples/MATRIX.data

doc:
	doxygen ./Doxyfile

make: compile doc

all: make

clean:
	rm -r ./${NAME} ./src/*.o ./doc 2>/dev/null || true