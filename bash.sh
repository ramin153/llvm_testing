clang -emit-llvm -S -o test4.ll test4.c -fno-discard-value-names
clang -emit-llvm -S -o test3.ll test3.c -fno-discard-value-names
clang -emit-llvm -S -o test1.ll test1.c -fno-discard-value-names
clang -emit-llvm -S -o test2.ll test2.c -fno-discard-value-names

clang++ -c  LimitedVariable.cpp
clang++ -c  help_function.cpp
clang++  -c  RandomPath.cpp 
clang++  -c  DSE.cpp 

clang++ -o main help_function.o LimitedVariable.o RandomPath.o DSE.o  `llvm-config --cxxflags` `llvm-config --ldflags` `llvm-config --libs` -lpthread -lncurses -ldl -fexceptions
./main test1.ll 
./main test2.ll 
./main test3.ll 
./main test4.ll 