# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Setup

`build/`, `cmake/`, and the contents of `modules/<name>/` are **not in git**. They are pulled in by the [gil](https://github.com/chronoxor/gil) tool from the entries in `.gitlinks`. Without this step every documented build command fails because the scripts and third-party sources are missing.

```bash
pip3 install gil
gil update   # fetches build/, cmake/, modules/Catch2, modules/HdrHistogram, modules/cpp-optparse, modules/zlib
```

## Build Commands

The project uses CMake (minimum 3.20) with C++23. All build scripts live under `build/` (after `gil update`).

**Full build (generate + build + test + install):**
```bash
cd build && ./unix.sh        # Linux / macOS
cd build && unix.bat         # Windows (Cygwin / MSYS2)
cd build && mingw.bat        # Windows (MinGW)
cd build && vs.bat           # Windows (Visual Studio)
```

**Step by step (from `build/Unix/`):**
```bash
./01-generate.sh   # cmake -DCMAKE_BUILD_TYPE=Release into temp/
./02-build.sh      # make -j4 all
./03-tests.sh      # ctest -V
./04-install.sh    # make -j4 install  (outputs to bin/)
```

**Doxygen API docs:** the top-level `CMakeLists.txt` defines a `doxygen` target driven by `documents/Doxyfile`. Run `make doxygen` from the configured build directory.

**Code coverage (Unix only, added in `eb5631ed`):** the `CodeCoverage` CMake module is included from `cmake/`. Coverage targets are gated on the toolchain — see `cmake/CodeCoverage.cmake` for the available targets and required flags.

**Run tests directly:**
```bash
./bin/cppbenchmark-tests --durations yes --order lex
```

**Run a single test by name (Catch2 filter):**
```bash
./bin/cppbenchmark-tests "[CppBenchmark][Launcher]"
```

**Run an example benchmark:**
```bash
./bin/cppbenchmark-example-atomic
./bin/cppbenchmark-example-atomic --output=csv
./bin/cppbenchmark-example-atomic --filter=".*CAS.*"
```

## Architecture

### Library (`cppbenchmark`)

The library is compiled from `include/benchmark/` (headers) and `source/benchmark/` (implementations) into `libcppbenchmark.a`. The single public include is `benchmark/cppbenchmark.h`.

When CppBenchmark is consumed as a subdirectory by a parent CMake project, set `-DCPPBENCHMARK_MODULE=ON`. The top-level `CMakeLists.txt` gates examples, tests, and the install step on `if(NOT CPPBENCHMARK_MODULE)`, so this flag yields the library target only.

**Benchmark class hierarchy:**
- `BenchmarkBase` — base for all benchmark types; holds `Settings`, manages phases and launch counting
- `Benchmark` — single-thread benchmark; override `Run(Context&)`
- `BenchmarkThreads` — multi-thread benchmark; override `RunThread(ContextThreads&)`
- `BenchmarkPC` — producers/consumers benchmark; override `RunProducer(ContextPC&)` and `RunConsumer(ContextPC&)`

**Fixture class hierarchy** (mixed in via multiple inheritance):
- `Fixture` — `Initialize(Context&)` / `Cleanup(Context&)` called per attempt
- `FixtureThreads` — adds `InitializeThread` / `CleanupThread` per thread
- `FixturePC` — adds `InitializeProducer` / `CleanupProducer` / `InitializeConsumer` / `CleanupConsumer`

**Settings (fluent builder):**  
`Settings` configures attempts (default 5), duration (default 5 s), operations count, threads, producers/consumers, parameters (single/pair/triple with ranges and selector functions), and latency histograms (HDR). `SettingsThreads` and `SettingsPC` extend it.

**Execution flow:**
1. `LauncherConsole` (singleton) collects benchmark builders registered by the `BENCHMARK_*` macros at static init time.
2. `LauncherConsole::Initialize(argc, argv)` parses CLI flags (`--filter`, `--output=console|csv|json`, `--list`, `--quiet`, `--histograms`).
3. `LauncherConsole::Execute()` calls `Launcher::Launch(pattern)` which iterates the benchmark plan (cartesian product of params × attempts) and dispatches to each benchmark's `Launch()`.
4. `LauncherConsole::Report()` feeds results to the selected `Reporter` implementation.

**Phase / metrics system:**
- `PhaseCore` owns the actual measurement state and children sub-phases.
- `PhaseMetrics` accumulates timing (min/max/avg/total), operations, items, bytes, custom typed key-value pairs, and optionally an HDR histogram for latency.
- `Context` (and its thread/PC variants) gives benchmark code access to parameters (`x()`, `y()`, `z()`), `metrics()`, and phase management (`StartPhase` / `ScopePhase` / `StopPhase`).

**Dynamic benchmarks** (`Executor` singleton):  
Use `BENCHCODE_SCOPE("name")`, `BENCHCODE_START` / `BENCHCODE_STOP`, and `BENCHCODE_REPORT()` to instrument arbitrary code paths without a `BENCHMARK_MAIN()` harness.

**Reporters:** `ReporterConsole`, `ReporterCSV`, `ReporterJSON` all implement `Reporter` and write to a provided `std::ostream` (default: `std::cout`).

### Macros (user-facing API)

Defined in `include/benchmark/cppbenchmark.h`:

| Macro | Use |
|---|---|
| `BENCHMARK(name, ops)` | Inline single-thread benchmark |
| `BENCHMARK_FIXTURE(Fixture, name, ops)` | Single-thread with static or dynamic fixture |
| `BENCHMARK_THREADS(name, threads, ops)` | Multi-thread benchmark |
| `BENCHMARK_THREADS_FIXTURE(Fixture, ...)` | Multi-thread with fixture |
| `BENCHMARK_CLASS(Type, name, ops)` | Register a pre-defined class |
| `BENCHMARK_MAIN()` | Expands to `main()` wiring up `LauncherConsole` |
| `BENCHCODE_SCOPE/START/STOP/REPORT` | Dynamic (inline) benchmarks |

### Modules (third-party, under `modules/`)

| Module | Purpose |
|---|---|
| `Catch2` | Unit test framework (amalgamated) |
| `cpp-optparse` | CLI argument parsing for `LauncherConsole` |
| `HdrHistogram` | High-Dynamic-Range histograms for latency measurement |
| `zlib` | Compression (dependency of HdrHistogram) |

Each module has a corresponding `modules/<name>.cmake` fetch/configure file.

### Tests (`tests/`)

Tests use Catch2 and exercise `Launcher`, `Reporter`, `Environment`, and `System` directly — not through the macro API. The `TestBenchmark` / `TestLauncher` pattern in `tests/test_launcher.cpp` is the canonical way to test new launcher or reporter behavior.

### Examples (`examples/`)

Each `.cpp` file is a standalone executable (`cppbenchmark-example-<name>`). They are the best reference for benchmark authoring patterns: `atomic.cpp` (fixture), `threads.cpp` (multi-thread), `spsc.cpp`/`mpmc.cpp` (producers/consumers), `executor.cpp` (dynamic benchmarks).
