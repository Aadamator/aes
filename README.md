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
├─ docs/
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
│   ├── constants.c             <-- Common constants used across each implementation, e.g. S-boxes.
│   ├── CMakeLists.txt          <-- Header declarations and public linking.
│   └── ...
├─ tests/
│   ├── decrypt.c               <-- Unit tests to test AES-128 decryption implementation(s).
│   ├── encrypt.c               <-- Unit tests to test AES-128 encryption implementation(s).
│   ├── CMakeLists.txt          <-- Defines test executables and registers them with CTest.
│   └── ...
├── .editorconfig               <-- Editor configuration file.
├── CMakeLists.txt              <-- Root-level build entrypoint: defines the project, fetches dependencies, and wires up subdirectories.
├── LICENSE
├── Makefile                    <-- root-level scripts
├── README.md
└── ...
```

<sup>[Back to top ^][table-of-contents]</sup>

## 🪄 2. Usage

### 2.1. Requirements

* [CMake >=3.21](https://cmake.org/download/)
* A C compiler with C11 support:
  - Linux: [**GCC**](https://gcc.gnu.org/install/) or [**Clang**](https://releases.llvm.org/download.html)
  - macOS: **Apple Clang** (via [Xcode Command Line Tools](https://developer.apple.com/documentation/xcode/installing-the-command-line-tools))
  - Windows: **MSVC** (via [Visual Studio Build Tools](https://code.visualstudio.com/docs/cpp/config-msvc)) or **Clang/MinGW**
* [GNU Make (Optional)](https://www.gnu.org/software/make/)

<sup>[Back to top ^][table-of-contents]</sup>

### 2.2. Setup

#### 2.2.1. Using CMake

1. Configure the build and fetch dependencies:
```bash
$ cmake -S . -B build
```

2. Build the source files:
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

Unit tests are located in the [`tests/`](./tests/) directory and are implemented using [Unity](https://github.com/ThrowTheSwitch/Unity).

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

TBC...

<sup>[Back to top ^][table-of-contents]</sup>

## 📑 4. Appendix

### 4.1. Implementations

| Name                                       | Description                                          |
|--------------------------------------------|------------------------------------------------------|
| [`naive`](./impls/naive/aes_128.c)         | A naive implementation of AES-128.                   |
| [`optimized`](./impls/optimized/aes_128.c) | An optimized version of the naive implementation.    |
| [`t_tables`](./impls/t_tables/aes_128.c)   | The optimized implementation using T-tables.         |
| [`aes_ni`](./impls/aes_ni/aes_128.c)       | The optimized implementation using utilizing AES-NI. |

### 4.2. Useful commands

| Name                                                                      | Description                                                                                                              |
|---------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------|
| `cmake -S . -B build`/`make configure`                                    | Configures the `build/` directory and fetches dependencies.                                                              |
| `cmake --build build`/`make build`                                        | Compiles the source files to the `build/` directory.                                                                     |
| `ctest --test-dir build`/`make test`                                      | Runs unit tests for all implementations.                                                                                 |
| `ctest --test-dir build -R <implementation>`/`make test_<implementation>` | Runs unit tests for a specific implementation, one of: <br/>* `naive`<br/>* `optimized`<br/>* `t_tables`<br/>* `aes_ni`. |
| `make update`                                                             | Re-configures the build directory - useful for caching previously downloaded dependencies.                               |

<sup>[Back to top ^][table-of-contents]</sup>

## 📄 5. License

Please refer to the [LICENSE][license] file.

<sup>[Back to top ^][table-of-contents]</sup>

<!-- links -->
[license]: ./LICENSE
[table-of-contents]: #table-of-contents
