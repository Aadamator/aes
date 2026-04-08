BUILD_DIR ?= build
CMAKE ?= cmake
CTEST ?= ctest
BENCHMARK_TARGETS := \
    benchmark_library_decrypt \
    benchmark_library_encrypt \
    benchmark_naive_decrypt \
    benchmark_naive_encrypt \
    benchmark_ni_instructions_decrypt \
    benchmark_ni_instructions_encrypt \
    benchmark_optimized_decrypt \
    benchmark_optimized_encrypt \
    benchmark_t_tables_decrypt \
    benchmark_t_tables_encrypt

.PHONY: benchmark \
	benchmark_decrypt \
	benchmark_encrypt \
	benchmark_report \
    build \
    clean \
    configure \
    test \
    test_naive \
    test_ni_instructions \
    test_optimized \
    test_t_tables \
    update

all: configure build

###
# benchmarking
###

benchmark: benchmark_decrypt \
	benchmark_encrypt

benchmark_decrypt: benchmark_library_decrypt \
	benchmark_naive_decrypt \
	benchmark_ni_instructions_decrypt \
	benchmark_optimized_decrypt \
	benchmark_t_tables_decrypt

benchmark_encrypt: benchmark_library_encrypt \
	benchmark_naive_encrypt \
	benchmark_ni_instructions_encrypt \
	benchmark_optimized_encrypt \
	benchmark_t_tables_encrypt

$(BENCHMARK_TARGETS): benchmark_%:
	@echo ">>> Running $* benchmark"
	@cmake -E make_directory .benchmarks
	@./$(BUILD_DIR)/benchmarks/$*_benchmark \
       --benchmark_display_aggregates_only=true \
       --benchmark_out=./.benchmarks/$*_benchmark.json \
       --benchmark_out_format=json > /dev/null 2>&1
	@echo ">>> Finished $* benchmark"

benchmark_report:
	python3 benchmarks/reporter.py

###
# building
###

build:
	@echo ">>> Building $(BUILD_DIR)/ directory"
	$(CMAKE) --build $(BUILD_DIR)

clean:
	@echo ">>> Removing $(BUILD_DIR)/ directory"
	rm -rf $(BUILD_DIR)

configure: clean
	@echo ">>> Configuring $(BUILD_DIR)/ directory"
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release

update:
	@echo ">>> Re-configuring $(BUILD_DIR)/ directory"
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release

###
# testing
###

test:
	@echo ">>> Running all unit tests"
	$(CTEST) --test-dir $(BUILD_DIR) --output-on-failure

test_naive:
	@echo ">>> Running naive implementation unit tests"
	$(CTEST) --test-dir $(BUILD_DIR) -R "test_naive" --output-on-failure

test_ni_instructions:
	@echo ">>> Running NI instructions implementation unit tests"
	$(CTEST) --test-dir $(BUILD_DIR) -R "test_ni_instructions"
	--output-on-failure

test_optimized:
	@echo ">>> Running optimized implementation unit tests"
	$(CTEST) --test-dir $(BUILD_DIR) -R "test_optimized" --output-on-failure

test_t_tables:
	@echo ">>> Running T-tables implementation unit tests"
	$(CTEST) --test-dir $(BUILD_DIR) -R "test_t_tables" --output-on-failure
