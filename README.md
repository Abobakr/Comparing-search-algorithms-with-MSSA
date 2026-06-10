# MSSA C++ Search Algorithm Benchmark

A self-contained C++ benchmark project that compares classical search algorithms with regression-guided and MSSA-inspired indexed search strategies on large sorted datasets.

## What this project does

- Implements traditional `binarySearch` and `interpolationSearch` on sorted arrays.
- Implements MSSA-inspired search functions:
  - `MSSA_Binary` and `MSSA_Interpolation`
  - `game_changer_MSSKCA` to compute adaptive search parameters
  - `parts_game_changer_MSSKCA` to segment data into partitions and reduce error bounds
- Implements regression-based search guides using:
  - linear regression
  - quadratic regression
  - cubic regression
  - exponential regression
  - power regression
- Runs timing experiments for three synthetic distributions:
  - Normal distribution
  - Uniform distribution
  - Exponential distribution
- Verifies correctness with "NO-MISSED element" validation checks.

## Repository contents

- `main.cpp` — main C++ source file with full benchmark implementation.
- `main.exe` — compiled executable (if present from previous build).
- `README.md` — this guide.
- `normal distribution - comparision.jpg` — distribution comparison visualization for the normal case.
- `exponential distribution - comparision.jpg` — distribution comparison visualization for the exponential case.
- `MSSA-Matlab Paper Version/` — MATLAB scripts and supporting files for the companion paper and further algorithm exploration.

## Key algorithms in `main.cpp`

### Classic search methods

- `binarySearch(const vector<double>&, double)`
- `interpolationSearch(const vector<double>&, double)`

These are used as baseline comparisons for speed and correctness.

### MSSA-inspired lookup

- `MSSA_Binary(...)`
- `MSSA_Interpolation(...)`

These variants use a predicted starting position together with a bounded error window instead of searching the full sorted range. The prediction uses a computed `inv_key` factor and a maximum error bound (`max_err`).

### MSSKCA and Game Changer parameter discovery

- `classic_MSSKCA(...)`
- `game_changer_MSSKCA(...)`
- `parts_game_changer_MSSKCA(...)`

These functions compute search parameters from the data distribution itself. `game_changer_MSSKCA` walks through a sequence of candidate data points, finding the strongest error-bound changer. The partitioned version splits the dataset to refine the prediction across segments.

### Regression-based index prediction

The project also benchmarks regression models as index estimators for search: `linearRegression`, `quadraticRegression`, `cubicRegression`, `exponentialRegression`, and `powerRegression`.

Each model is used to calculate a predicted position and then validated with `Reg_Binary` or `Reg_Interpolation` inside a small error margin.

## How to build and run

### Build with `g++`

Open a PowerShell or terminal in the project folder and run:

```powershell
g++ -std=c++17 -O2 -o main.exe main.cpp
```

### Run the benchmark

```powershell
.\\main.exe
```

Optionally pass a partition count to enable the partitioned game changer algorithm:

```powershell
.\\main.exe 4
```

If `parts_count` is greater than `1`, the benchmark will perform an additional partitioned MSSKCA test.

## What the benchmark measures

For each distribution, the program:

1. Generates `N = 1_000_000` values.
2. Sorts the data.
3. Computes predictive search parameters.
4. Generates `SEARCH_COUNT = N * 10` search targets.
5. Sorts the targets.
6. Measures execution time for each search strategy.

The final output includes runtime comparisons for classic search, MSSA-guided search, partitioned MSSA search, and each regression-based search.

## Dataset distributions tested

- Normal distribution: `normal_distribution<>` around `N / 2` with standard deviation `N / 6`
- Uniform distribution: `uniform_real_distribution<>(0, N * 10)`
- Exponential distribution: `exponential_distribution<>(1.0 / (N / 5.0))`

These cases evaluate how the algorithms behave under different sorted data shapes.

## MATLAB companion materials

The `MSSA-Matlab Paper Version/` folder contains MATLAB scripts used for research and visualization, including:

- `BSA.m`
- `CompareAlgorithms.m`
- `ExecTimeBar.m`
- `Game_Changer_MSSCKA.m`
- `ISA.m`
- `MSSA_BSA.m`
- `MSSA_ISA.m`
- `MSSCKA_Parallel.m`
- `last_calculations.mat`

These scripts are useful for generating figures, validating results, and reproducing the core paper experiments.

## Images and visual context

![Normal Distribution Comparison](./normal%20distribution%20-%20comparision.jpg)

![Exponential Distribution Comparison](./exponential%20distribution%20-%20comparision.jpg)

> The images show how the project compares algorithm performance and data behavior for two of the main distributions.

## Suggested future development

- Add command-line flags for:
  - `--size` to set dataset size
  - `--targets` to set search count
  - `--seed` for reproducible random data
  - `--distribution` to choose `normal`, `uniform`, or `exponential`
- Add CSV or JSON benchmark output for automated analysis.
- Add a proper unit test suite and performance regression tests.
- Add support for additional distributions such as Poisson, log-normal, or bimodal.
- Improve numeric stability and range handling for regression and interpolation formulas.
- Add threaded or SIMD-accelerated search benchmarks for multi-core performance.
- Add a small interactive mode that lets users search custom values and compare the predicted index vs. actual index.
- Convert repeatable mathematics functions into templated utilities and separate the benchmarking harness from algorithm implementations.

## Notes

- `classic_MSSKCA` is intentionally skipped for very large `N` because it is too slow at scale.
- The current code uses absolute values for generated random values and sorts them before search.
- The program is designed for analysis and research rather than production search use.

## License

This project is provided as-is for algorithm research and benchmarking.
