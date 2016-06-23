
# Compilador y banderas
CXX = g++
CFLAGS = -c -Wall -g


# Definicion de directorios para el proyecto
SOURCE_DIR = ./src
BUILD_DIR = ./build
INCLUDE = ./include
BIN_DIR = .

# Definicion de archivos fuente y objeto
# '_' indica solo el nombre del archivo, sin incluir el directorio
SOURCES  = $(wildcard $(SOURCE_DIR)/*.cpp)
_SOURCES = $(notdir $(SOURCES))

_OBJECTS = $(patsubst %.cpp, %.o, $(_SOURCES))
OBJECTS	 = $(addprefix $(BUILD_DIR)/, $(_OBJECTS))


TARGET = $(BIN_DIR)/MESI


all: $(TARGET) Criba

deps: 
	sudo apt-get install libopenmpi-dev mpich2

Criba: cribacon cribasin

cribasin: $(SOURCE_DIR)/cribasin.c
	gcc -std=c99 -o $@ $^

cribacon: $(SOURCE_DIR)/cribacon.c
	mpicc.mpich2 -std=c99 -o $@ $^

prueba1: ./test/prueba1.cpp $(OBJECTS)
	$(CXX) $< $(filter-out %main.o, $(OBJECTS)) -I$(INCLUDE) -o $@


$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	
# Regla general para hacer archivos objeto
# a partir de archivos cpp
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) -o $@ -I$(INCLUDE) $(CFLAGS) $<
	
clean:
	rm -f $(OBJECTS) $(TARGET) prueba1 cribacon cribasin
	
	
# Dependency Rules
$(BUILD_DIR)/CacheMemory.o: $(INCLUDE)/CacheMemory.h $(INCLUDE)/CacheSet.h  $(SOURCE_DIR)/CacheMemory.cpp
$(BUILD_DIR)/CacheSet.o: $(INCLUDE)/CacheSet.h $(SOURCE_DIR)/CacheSet.cpp
$(BUILD_DIR)/main.o:	$(INCLUDE)/CacheMemory.h 

$(BUILD_DIR)/MemoryBus.o: $(INCLUDE)/MemoryBus.h  $(INCLUDE)/CacheMemory.h $(SOURCE_DIR)/MemoryBus.cpp

.PHONY: test all clean deps

test:
	@echo $(SOURCES)
	@echo $(_SOURCES)
	@echo $(OBJECTS)
	@echo $(_OBJECTS)
	@echo $(filter-out %main.o, $(OBJECTS))
	
