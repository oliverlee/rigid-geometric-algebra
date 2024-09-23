#!/bin/bash
set -euxo pipefail

# gcov wrapper used with coverage prevent
# writing data in intermediate format
# https://github.com/bazelbuild/bazel/blob/7aa9e7726f4ebf934c9c317c7850e64d901a6141/tools/test/collect_cc_coverage.sh#L165-L169
if [[ $# -ne 0 ]] && [[ "$1" == "-i" ]]; then
  shift
fi
exec ./external/llvm18_toolchain_llvm/bin/llvm-cov gcov ${@:-}
