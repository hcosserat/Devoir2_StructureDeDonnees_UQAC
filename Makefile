# Makefile du projet de structures de donnees

CXX ?= g++
CXXFLAGS ?= -std=c++20 -Wall -Wextra -pedantic

ifeq ($(OS),Windows_NT)
    EXE_EXT := .exe
    RUN_PREFIX := 
    RM_FILE := del /Q
    RM_FORCE := 2>nul
else
    EXE_EXT :=
    RUN_PREFIX := ./
    RM_FILE := rm -f
    RM_FORCE :=
endif

TARGET_DIR := .
SRC_DIR := src
SDA_DIR := $(SRC_DIR)/SDA/Kd-Tree

# ------------------------------------------
# Exercice 1 - KD-Tree
# ------------------------------------------
EX1_TARGET := $(TARGET_DIR)/ex1$(EXE_EXT)
EX1_SOURCES := $(SRC_DIR)/Ex1/main_ex1.cpp \
               $(SDA_DIR)/Kd-Tree.cpp
EX1_OBJECTS := $(EX1_SOURCES:.cpp=.o)
EX1_INCLUDES := -I$(SDA_DIR)

# ------------------------------------------
# Exercice 2 - a completer plus tard
# ------------------------------------------
EX2_TARGET := $(TARGET_DIR)/ex2$(EXE_EXT)
EX2_SOURCES := $(SRC_DIR)/Ex2/main_ex2.cpp
EX2_OBJECTS := $(EX2_SOURCES:.cpp=.o)
EX2_INCLUDES :=

# ------------------------------------------
# Exercice 3 - a completer plus tard
# ------------------------------------------
EX3_TARGET := $(TARGET_DIR)/ex3$(EXE_EXT)
EX3_SOURCES := $(SRC_DIR)/Ex3/main_ex3.cpp
EX3_OBJECTS := $(EX3_SOURCES:.cpp=.o)
EX3_INCLUDES :=

# ------------------------------------------
# Exercice 4 - a completer plus tard
# ------------------------------------------
EX4_TARGET := $(TARGET_DIR)/ex4$(EXE_EXT)
EX4_SOURCES := $(SRC_DIR)/Ex4/main_ex4.cpp
EX4_OBJECTS := $(EX4_SOURCES:.cpp=.o)
EX4_INCLUDES :=

.PHONY: all ex1 ex2 ex3 ex4 run-ex1 run-ex2 run-ex3 run-ex4 clean clean-ex1 clean-ex2 clean-ex3 clean-ex4 rebuild

all: ex1 ex2 ex3 ex4

ex1: $(EX1_TARGET)
ex2: $(EX2_TARGET)
ex3: $(EX3_TARGET)
ex4: $(EX4_TARGET)

$(EX1_TARGET): $(EX1_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EX2_TARGET): $(EX2_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EX3_TARGET): $(EX3_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EX4_TARGET): $(EX4_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(EX1_INCLUDES) $(EX2_INCLUDES) $(EX3_INCLUDES) $(EX4_INCLUDES) -c $< -o $@

run-ex1: $(EX1_TARGET)
	@echo "Lancement de l'exercice 1..."
	@$(RUN_PREFIX)$(EX1_TARGET)

run-ex2: $(EX2_TARGET)
	@echo "Lancement de l'exercice 2..."
	@$(RUN_PREFIX)$(EX2_TARGET)

run-ex3: $(EX3_TARGET)
	@echo "Lancement de l'exercice 3..."
	@$(RUN_PREFIX)$(EX3_TARGET)

run-ex4: $(EX4_TARGET)
	@echo "Lancement de l'exercice 4..."
	@$(RUN_PREFIX)$(EX4_TARGET)

clean:
	-@$(RM_FILE) $(EX1_OBJECTS) $(EX1_TARGET) $(EX2_OBJECTS) $(EX2_TARGET) $(EX3_OBJECTS) $(EX3_TARGET) $(EX4_OBJECTS) $(EX4_TARGET) $(RM_FORCE)
	@echo Nettoyage termine

clean-ex1:
	-@$(RM_FILE) $(EX1_OBJECTS) $(EX1_TARGET) $(RM_FORCE)
	@echo Exercice 1 nettoye

clean-ex2:
	-@$(RM_FILE) $(EX2_OBJECTS) $(EX2_TARGET) $(RM_FORCE)
	@echo Exercice 2 nettoye

clean-ex3:
	-@$(RM_FILE) $(EX3_OBJECTS) $(EX3_TARGET) $(RM_FORCE)
	@echo Exercice 3 nettoye

clean-ex4:
	-@$(RM_FILE) $(EX4_OBJECTS) $(EX4_TARGET) $(RM_FORCE)
	@echo Exercice 4 nettoye

rebuild: clean all