# Big Num

## Behaviors
I ran a benchmark of various operations with different integral types. Note that __uint128_t is exactly
2x slower which makes sense. Surprised multiplication isn't slower?

```
Running ./bench_primitives
Run on (32 X 6000 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 2048 KiB (x16)
  L3 Unified 36864 KiB (x1)
Load Average: 4.92, 5.05, 4.98
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------------------------
Benchmark                                                      Time             CPU   Iterations
------------------------------------------------------------------------------------------------
integer_multiplication<uint32_t, Add<uint32_t>>            0.125 ns        0.125 ns   5578183234
integer_multiplication<uint32_t, Sub<uint32_t>>            0.125 ns        0.125 ns   5575733249
integer_multiplication<uint32_t, Mul<uint32_t>>            0.125 ns        0.125 ns   5579048441
integer_multiplication<uint32_t, Div<uint32_t>>            0.125 ns        0.125 ns   5578728487
integer_multiplication<uint64_t, Add<uint64_t>>            0.125 ns        0.125 ns   5578751651
integer_multiplication<uint64_t, Sub<uint64_t>>            0.125 ns        0.125 ns   5284554659
integer_multiplication<uint64_t, Mul<uint64_t>>            0.127 ns        0.127 ns   5469704549
integer_multiplication<uint64_t, Div<uint64_t>>            0.125 ns        0.125 ns   5575928316
integer_multiplication<__uint128_t, Add<__uint128_t>>      0.251 ns        0.251 ns   2789791050
integer_multiplication<__uint128_t, Sub<__uint128_t>>      0.251 ns        0.251 ns   2789865368
integer_multiplication<__uint128_t, Mul<__uint128_t>>      0.251 ns        0.251 ns   2789945183
integer_multiplication<__uint128_t, Div<__uint128_t>>      0.251 ns        0.251 ns   2789898136
```