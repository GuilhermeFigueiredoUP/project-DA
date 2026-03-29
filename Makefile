# Variáveis
CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall
INCLUDES = -I include -I data_structures

# Alvo principal
all:
	$(CXX) $(CXXFLAGS) $(INCLUDES) src/*.cpp main.cpp -o programa

# Limpar binários
clean:
	rm -f programa