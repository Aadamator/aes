BUILD_DIR ?= build
CMAKE ?= cmake
CTEST ?= ctest

.PHONY: build clean configure test test_naive update

all: configure build

build:
	@echo ">>> Building $(BUILD_DIR)/ directory"
	$(CMAKE) --build $(BUILD_DIR)

clean:
	@echo ">>> Removing $(BUILD_DIR)/ directory"
	rm -rf $(BUILD_DIR)

configure: clean
	@echo ">>> Configuring $(BUILD_DIR)/ directory"
	$(CMAKE) -S . -B $(BUILD_DIR)

test: build
	@echo ">>> Running all unit tests"
	$(CTEST) --test-dir $(BUILD_DIR)

test_naive: build
	@echo ">>> Running naive implementation unit tests"
	$(CTEST) --test-dir $(BUILD_DIR) -R naive

update:
	@echo ">>> Re-configuring $(BUILD_DIR)/ directory"
	$(CMAKE) -S . -B $(BUILD_DIR)
