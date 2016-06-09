
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


TARGET = $(BIN_DIR)/Tarea1

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	
# Regla general para hacer archivos objeto
# a partir de archivos cpp
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) -o $@ -I$(INCLUDE) $(CFLAGS) $<
	
clean:
	rm -f $(OBJECTS) $(TARGET)
	
	
# Dependency Rules
$(BUILD_DIR)/CacheMemory.o: $(INCLUDE)/CacheMemory.h $(INCLUDE)/CacheSet.h $(SOURCE_DIR)/CacheMemory.cpp
$(BUILD_DIR)/CacheSet.o: $(INCLUDE)/CacheSet.h $(SOURCE_DIR)/CacheSet.cpp
$(BUILD_DIR)/main.o:	$(INCLUDE)/CacheMemory.h

test:
	@echo $(SOURCES)
	@echo $(_SOURCES)
	@echo $(OBJECTS)
	@echo $(_OBJECTS)
	
