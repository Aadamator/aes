BUILD_DIR ?= build
CMAKE ?= cmake
CTEST ?= ctest

.PHONY: benchmark build clean configure test test_aes_ni test_naive test_optimized test_t_tables update

all: configure build

benchmark_encrypt:
	@echo ">>> Running benchmarks"
	@cmake -E make_directory .benchmarks
	./$(BUILD_DIR)/benchmarks/naive_encrypt_benchmark \
		--benchmark_display_aggregates_only=true \
		--benchmark_out=./.benchmarks/naive_encrypt.json \
		--benchmark_out_format=json \
		--benchmark_repetitions=10
	./$(BUILD_DIR)/benchmarks/library_encrypt_benchmark \
		--benchmark_display_aggregates_only=true \
		--benchmark_out=./.benchmarks/library_encrypt.json \
		--benchmark_out_format=json \
		--benchmark_repetitions=10

build:
	@echo ">>> Building $(BUILD_DIR)/ directory"
	$(CMAKE) --build $(BUILD_DIR)

clean:
	@echo ">>> Removing $(BUILD_DIR)/ directory"
	rm -rf $(BUILD_DIR)

configure: clean
	@echo ">>> Configuring $(BUILD_DIR)/ directory"
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release

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
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
