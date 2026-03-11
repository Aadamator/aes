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

test:
	@echo ">>> Running all unit tests"
	$(CTEST) --test-dir $(BUILD_DIR) --output-on-failure

test_aes_ni:
	@echo ">>> Running AES-NI implementation unit tests"
	$(CTEST) --test-dir $(BUILD_DIR) -R "aes_ni_(decrypt|encrypt)_tests" --output-on-failure

test_naive:
	@echo ">>> Running naive implementation unit tests"
	$(CTEST) --test-dir $(BUILD_DIR) -R "naive_(decrypt|encrypt)_tests" --output-on-failure

test_optimized:
	@echo ">>> Running optimized implementation unit tests"
	$(CTEST) --test-dir $(BUILD_DIR) -R "optimized_(decrypt|encrypt)_tests" --output-on-failure

test_t_tables:
	@echo ">>> Running T-tables implementation unit tests"
	$(CTEST) --test-dir $(BUILD_DIR) -R "t_tables_(decrypt|encrypt)_tests" --output-on-failure

update:
	@echo ">>> Re-configuring $(BUILD_DIR)/ directory"
	$(CMAKE) -S . -B $(BUILD_DIR)
