# Optimizing a Student Record Search System in C

Final Project Part 2 — Erik Contreras

## Purpose
Compare a linked-list, linear-search baseline against a contiguous sorted-array binary-search optimization. Both implementations use identical deterministic student records and search queries. Even IDs are present; odd IDs are absent. Half the queries are hits and half are misses.

## Compile
Run in the repository directory (GCC or Clang with C11 support):

```sh
cc -std=c11 -O2 -Wall -Wextra -pedantic baseline.c -o baseline
cc -std=c11 -O2 -Wall -Wextra -pedantic optimized.c -o optimized
```

## Run benchmarks

```sh
./baseline
./optimized
```

Each executable tests 1,000, 10,000, 100,000, and 1,000,000 records, uses 200 deterministic queries per size, runs 2 warmups and 7 timed repetitions per size, and reports the median nanoseconds per query. Timing excludes dataset construction and verification. Run both programs on the same machine under similar load.

## Output
CSV rows: `records,median_ns_per_query,checksum`. The checksum must match between programs for every size. Speedup is baseline median divided by optimized median. Do not interpret the checksum as elapsed time.

## Assembly comparison
Use Compiler Explorer (https://godbolt.org) with C and `-O2`. Compare the `lookup` functions in `baseline.c` and `optimized.c` using the same compiler version and target. Capture both assembly screenshots for the written report. The `noinline` attribute helps preserve a separately visible function.

## Memory management
The baseline allocates one node per record and frees every node. The optimized version allocates one contiguous array and frees it. Both free their query buffers. Allocation failure is checked.

## Notes
This benchmark is a controlled demonstration, not a full student-record management application. The baseline has O(n) lookup time; the optimized version has O(log n) lookup time. Build costs and updates are not included in the measured lookup times. Benchmark timings must be measured on the student's own machine for the final report.
