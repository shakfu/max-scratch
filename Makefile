CHUCK_PACKAGE := "$(HOME)/Documents/Max\ 8/Packages/max-scratch"
WORKING_DIR := $(shell pwd)
PACKAGE_NAME := $(shell basename $(WORKING_DIR))
PACKAGE :=  "$(HOME)/Documents/Max\ 8/Packages/$(PACKAGE_NAME)"
SCRIPTS := source/scripts
BUILD := build


.PHONY: macos clean setup show

all: macos

macos:
	@mkdir -p build && cd build && cmake -GXcode .. && cmake --build . --config 'Release'

clean:
	@rm -rf build

show:
	@echo "$(PACKAGE)"

setup:
	@git submodule init
	@git submodule update
	@if ! [ -L "$(PACKAGE)" ]; then \
		ln -s "$(WORKING_DIR)" "$(PACKAGE)" ; \
		echo "... symlink created" ; \
	else \
		echo "... symlink already exists" ; \
	fi