CXX ?= g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Werror -pedantic -I.

.PHONY: test clean

test: build/test_controller
	./build/test_controller

build/test_controller: tests/test_controller.cpp config.h fan_controller.h sample_timer.h thermistor.h
	mkdir -p build
	$(CXX) $(CXXFLAGS) tests/test_controller.cpp -o $@

clean:
	rm -rf build
