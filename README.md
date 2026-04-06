<div align="center">

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)][license]

</div>

<h1 align="center">
  Group 16: Efficiency of AES Implementations
</h1>

---

### Table of contents

* [1. Overview](#-1-overview)
  - [1.1. Project Structure](#11-project-structure)
* [2. Usage](#-2-usage)
  - [2.1. Requirements](#21-requirements)
  - [2.2. Setup](#22-setup)
    - [2.2.1. Using CMake](#221-using-cmake)
    - [2.2.2. Using GNU Make (Recommended)](#222-using-gnu-make-recommended)
* [3. Testing and Benchmarking](#-3-testing-and-benchmarking)
  - [3.1. Unit Tests](#31-unit-tests)
    - [3.1.1. Using CTest](#311-using-ctest)
    - [3.1.2. Using GNU Make (Recommended)](#312-using-gnu-make-recommended)
  - [3.2. Benchmarking](#32-benchmarking)
    - [3.2.1. Running the benchmarks](#321-running-the-benchmarks)
    - [3.2.2. Using GNU Make (Recommended)](#322-using-gnu-make-recommended)
    - [3.2.3. Reporting](#323-reporting)
* [4. Appendix](#-4-appendix)
  - [4.1. Projects](#41-implementations)
  - [4.2. Useful commands](#42-useful-commands)
* [5. License](#-5-license)

## 🔭 1. Overview

### 1.1 Introduction

The Advanced Encryption Standard (AES) is one of the most used (if not the most used) current cryptographic algorithms. It has been designed from the ground up to be efficiently implementable in software. Nonetheless, due to its widespread use, it has become common for CPU to have dedicated hardware instructions (AES-NI) that allow for an even faster implementation of AES.

In this project, four versions of AES are implemented in C and their respective performance profiles are determined.

### 1.2. Project Structure

This repo uses a pseudo-monorepo structure:

```text
.
├─ benchmarks/
│   ├── utilities/              <-- Utility functions and constants to aid in the benchmarking.
│   │   └── ...
│   ├── CMakeLists.txt          <-- Defines test executables and links libraries.
│   ├── decrypt.cpp             <-- Benchmarks an AES-128 decryption implementation.
│   ├── encrypt.cpp             <-- Benchmarks an AES-128 encryption implementation.
│   ├── library_decrypt.cpp     <-- Benchmarks a third-party library AES-128 decryption implementation.
│   ├── library_encrypt.cpp     <-- Benchmarks a third-party library AES-128 encryption implementation.
│   ├── reporter.py             <-- Script used to aggregate the benchmarks into a JSON report.
│   └── ...
├─ docs/
│   ├── report/                 <-- LaTeX source files for the report.
│   │   ├── main.tex
│   │   └── ...
│   └── ...
├─ impls/
│   ├── <aes_implementation>/
│   │   ├── aes_128.c           <-- The AES-128 implementation.
│   │   ├── CMakeLists.txt      <-- Links common header library and declares alias.
│   │   └── ...
│   └── ...
├─ include/
│   ├── common/
│   │   ├── aes_128.h           <-- Common header interface used across each implementation.
│   │   ├── CMakeLists.txt      <-- Header declarations.
│   │   └── ...
│   └── ...
├─ lib/
│   ├── constants.h
│   ├── constants.c             <-- Common constants used across each implementation.
│   ├── gf.h
│   ├── gf.c                    <-- Utility functions related to Galois field arithmetic.
│   ├── s_box.h
│   ├── s_box.c                 <-- Utility functions and constants related to the S-box look-up
table.
│   ├── state_utils.h
│   ├── state_utils.c           <-- Utility functions used in state manipulation, e.g. mapping matrices.
│   ├── CMakeLists.txt          <-- Header declarations and public linking.
│   └── ...
├─ tests/
│   ├── utilities/              <-- Utility functions and constants to aid in the tests.
│   │   └── ...
│   ├── test_decrypt.c          <-- End-to-end tests for AES-128 decryption implementation(s).
│   ├── test_encrypt.c          <-- End-to-end tests for AES-128 encryption implementation(s).
│   ├── test_encrypt_rounds.c   <-- Unit tests for per round iteration.
│   ├── CMakeLists.txt          <-- Defines test executables and registers them with CTest.
│   └── ...
├── .editorconfig               <-- Editor configuration file.
├── CMakeLists.txt              <-- Root-level build entrypoint: defines the project, fetches dependencies, and wires up subdirectories.
├── LICENSE
├── Makefile                    <-- Root-level scripts
├── README.md
└── ...
```

<sup>[Back to top ^][table-of-contents]</sup>

## 🪄 2. Usage

### 2.1. Requirements

* [CMake >=3.21](https://cmake.org/download/)
* A C compiler with at least C11 support **and** a C++ compiler with at least C++11 support:
  - Linux: [**GCC**](https://gcc.gnu.org/install/) or [**Clang**](https://releases.llvm.org/download.html)
  - macOS: **Apple Clang** (via [Xcode Command Line Tools](https://developer.apple.com/documentation/xcode/installing-the-command-line-tools))
  - Windows: **MSVC** (via [Visual Studio Build Tools](https://code.visualstudio.com/docs/cpp/config-msvc)) or **Clang/MinGW**
* [GNU Make (Optional)](https://www.gnu.org/software/make/)
* [Python v3.10.12+](https://www.python.org/downloads/) (optional - used to generate the report)

> ⚠️ **NOTE:** While the core AES-128 algorithms are written entirely in C11, the benchmarking framework relies on Google Benchmark, which requires C++11 to compile.

<sup>[Back to top ^][table-of-contents]</sup>

### 2.2. Setup

#### 2.2.1. Using CMake

1. Configure the build and fetch dependencies:
```bash
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

2. Build the source files into executables:
```bash
$ cmake --build build
```

<sup>[Back to top ^][table-of-contents]</sup>

#### 2.2.2. Using GNU Make (Recommended)

If GNU Make is installed, simple run:
```bash
$ make
```

<sup>[Back to top ^][table-of-contents]</sup>

## 📊 3. Testing and Benchmarking

### 3.1. Unit Tests

Unit tests are located in the [`tests/`](./tests/) directory and are implemented using CTest as the test runner and [Unity](https://github.com/ThrowTheSwitch/Unity) as an assertation library.

<sup>[Back to top ^][table-of-contents]</sup>

#### 3.1.1. Using CTest

1. Ensure that the build directory is configured and built as described in [2.2.1. Using CMake](#221-using-cmake).

2. Run the unit tests:
```bash
$ ctest --test-dir build
```

<sup>[Back to top ^][table-of-contents]</sup>

#### 3.1.2. Using GNU Make (Recommended)

1. Ensure that the build directory is configured and built as described in [2.2.2. Using GNU Make](#222-using-gnu-make-recommended).

2. Run the unit tests:
```bash
$ make test
```

### 3.2. Benchmarking

Benchmarking utilizes Google's [Microbenchmarks](https://github.com/google/benchmark) as a benchmark task runner (written in C++) to run benchmarks for each C implementation and a third-party implementation from the [mbed TLS](https://github.com/ARMmbed/mbedtls) project.

The third-party implementation is used to compare the performance of the AES implementations and exposes efficient single block AES-128 encryption/decryption functions (see their [documentation](https://mbed-tls.readthedocs.io/projects/api/en/v3.6.0/api/file/aes_8h/#_CPPv421mbedtls_aes_crypt_ecbP19mbedtls_aes_contextiAL16E_KhAL16E_h) for more details)

#### 3.2.1. Running the benchmarks

1. Ensure that the build directory is configured and built as described in [2.2.1. Using CMake](#221-using-cmake).

2. Each benchmark is run separately from their corresponding built executable, replacing `<ni_instructions|naive|optimized|t_tables>` with the name of the implementation to benchmark and `<decrypt|encrypt>` with the type of benchmark to run:
```shell
./build/benchmarks/<ni_instructions|naive|optimized|t_tables>_<decrypt|encrypt>_benchmark \
		--benchmark_display_aggregates_only=true \
		--benchmark_out=./.benchmarks/<ni_instructions|naive|optimized|t_tables>_<decrypt|encrypt>.json \
		--benchmark_out_format=json
```

> ⚠️ **NOTE:** You must create the `.benchmarks/` directory before running the benchmark.

<sup>[Back to top ^][table-of-contents]</sup>

#### 3.2.2. Using GNU Make (Recommended)

1. Ensure that the build directory is configured and built as described in [2.2.2. Using GNU Make](#222-using-gnu-make-recommended).

2. For convenience, benchmarks can be run, replacing `<decrypt|encrypt>` with the type of benchmark to run:
```bash
$ make -j benchmark
```

> ⚠️ **NOTE:** As each benchmark for each implementation is run in a separate process, the `-j` flag instructs `make` to run multiple benchmarks in parallel if the system supports multiple CPU cores.

<sup>[Back to top ^][table-of-contents]</sup>

#### 3.2.3. Reporting

When the benchmarking is run, a JSON report is generated for each implementation and each algorthim (i.e. encryption and decryption) to the `.benchmarks/` directory. In order to aggregate these results into a single report, the [`reporter.py`](./benchmarks/reporter.py) script can be used.

To run the script you must have Python v3.10.12+ installed and can be run simply by using:

```shell
$ python3 ./benchmarks/reporter.py
```

The script will generate the aggregated report to `.benchmarks/aggregated_benchmarks.json`.

<sup>[Back to top ^][table-of-contents]</sup>

## 📑 4. Appendix

### 4.1. Implementations

| Name                                                   | Description                                         |
|--------------------------------------------------------|-----------------------------------------------------|
| [`naive`](./impls/naive/aes_128.c)                     | A naive implementation of AES-128.                  |
| [`optimized`](./impls/optimized/aes_128.c)             | An optimized version of the naive implementation.   |
| [`t_tables`](./impls/t_tables/aes_128.c)               | The optimized implementation using T-tables.        |
| [`ni_instructions`](./impls/ni_instructions/aes_128.c) | The optimized implementation using NI instructions. |

### 4.2. Useful commands

| Name                                                                      | Description                                                                                                                                  |
|---------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------|
| `cmake -S . -B build`/`make configure`                                    | Configures the `build/` directory and fetches dependencies.                                                                                  |
| `cmake --build build`/`make build`                                        | Compiles the source files to the `build/` directory.                                                                                         |
| `ctest --test-dir build`/`make test`                                      | Runs unit tests for all implementations.                                                                                                     |
| `ctest --test-dir build -R <implementation>`/`make test_<implementation>` | Runs unit tests for a specific implementation, one of: <br/>* `naive`<br/>* `optimized`<br/>* `t_tables`<br/>* `ni_instructions`.            |
| `make -j benchmark`                                                       | Runs all benchmarks across all implementations and the third-party library AES-128 block implementation.                                     |
| `make -j benchmark_decrypt`                                               | Runs benchmarks across all decryption implementations and the third-party library AES-128 block implementation.                              |
| `make -j benchmark_encrypt`                                               | Runs benchmarks across all encryption implementations and the third-party library AES-128 block implementation.                              |
| `make benchmark_report`                                                   | Aggregates all the benchmark reports into a single JSON report to `.benchmarks/aggregated_benchmarks.json` and pretty prints to the console. |
| `make test`                                                               | Runs all tests across all implementations.                                                                                                   |
| `make test_naive`                                                         | Runs tests specific to the naive AES implementation.                                                                                         |
| `make test_ni_instructions`                                               | Runs tests specific to the AES with NI instructions implementation.                                                                          |
| `make test_optimized`                                                     | Runs tests specific to the optimized AES implementation.                                                                                     |
| `make test_t_tables`                                                      | Runs tests specific to the AES implementation using T-tables.                                                                                |
| `make update`                                                             | Re-configures the build directory - useful for caching previously downloaded dependencies.                                                   |

<sup>[Back to top ^][table-of-contents]</sup>

## 📄 5. License

Please refer to the [LICENSE][license] file.

<sup>[Back to top ^][table-of-contents]</sup>

<!-- links -->
[license]: ./LICENSE
[table-of-contents]: #table-of-contents
