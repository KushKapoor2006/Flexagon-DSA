# Makefile for the Flex-Pipe Simulator (simulator/Makefile)

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic -Isrc/
LDFLAGS =

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp, obj/%.o, $(SOURCES))
HEADERS = $(wildcard src/*.h src/*.hpp)

TARGET = flexpipe_sim

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

obj/%.o: src/%.cpp $(HEADERS)
	@mkdir -p obj
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

outputs:
	@mkdir -p outputs

clean:
	@echo "Cleaning up..."
	rm -f $(TARGET)
	rm -rf obj
	rm -f outputs/*.csv

# Baseline + Flex runs for original inputs
run_vgg_baseline: $(TARGET) outputs
	./$(TARGET) inputs/vgg16.json baseline

run_vgg_flexpipe: $(TARGET) outputs
	./$(TARGET) inputs/vgg16.json pipelined_prefetch

run_distilbert_baseline: $(TARGET) outputs
	./$(TARGET) inputs/distilbert.json baseline

run_distilbert_flexpipe: $(TARGET) outputs
	./$(TARGET) inputs/distilbert.json pipelined_prefetch

# Tuned runs (single tuned file per network)
run_vgg_tuned_baseline: $(TARGET) outputs
	./$(TARGET) inputs/vgg16_tuned.json baseline

run_vgg_tuned_flexpipe: $(TARGET) outputs
	./$(TARGET) inputs/vgg16_tuned.json pipelined_prefetch

run_distilbert_tuned_baseline: $(TARGET) outputs
	./$(TARGET) inputs/distilbert_tuned.json baseline

run_distilbert_tuned_flexpipe: $(TARGET) outputs
	./$(TARGET) inputs/distilbert_tuned.json pipelined_prefetch

# Run all four experiments (2 networks × 2 modes)
run_all: run_vgg_baseline run_vgg_flexpipe run_distilbert_baseline run_distilbert_flexpipe \
         run_vgg_tuned_baseline run_vgg_tuned_flexpipe run_distilbert_tuned_baseline run_distilbert_tuned_flexpipe
