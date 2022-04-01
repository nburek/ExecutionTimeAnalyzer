# Execution Time Analyzer

## Problem
When profiling code running on microcontrollers it can
sometimes be difficult to get accurate estimates of how long operations
are taking and where time is being spent without impacting the
performance.

## Solution
Take the example Analyzer and create a new analyzer that is
meant for collecting data on execution time by using GPIO channels. The
analyzer supports up to two channels. The first channel is the top level
timer that will record the periods when the line is high and export that
data as a Frame. The second (optional) channel can be used to time
events within the top level timing. While the Timer channel is high,
toggling the SubTimer channel will create sub-frames recording the time
of the events within the top level Timer event.

## Building the Analyzer

### Windows

```bat
mkdir build
cd build
cmake .. -A x64
cmake --build .
:: built analyzer will be located at SampleAnalyzer\build\Analyzers\Debug\SimpleSerialAnalyzer.dll
```

### MacOS

```bash
mkdir build
cd build
cmake ..
cmake --build .
# built analyzer will be located at SampleAnalyzer/build/Analyzers/libSimpleSerialAnalyzer.so
```

### Linux

```bash
mkdir build
cd build
cmake ..
cmake --build .
# built analyzer will be located at SampleAnalyzer/build/Analyzers/libSimpleSerialAnalyzer.so
```