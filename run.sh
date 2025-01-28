#!/bin/bash
set -e

# Create output directories if they don't exist
mkdir -p /app/corpus /app/findings

# Run AFL++ with the fuzzer
afl-fuzz -i /app/corpus -o /app/findings \
    -m 4096 -t 1000 \
    -- ./fuzz_advent @@
