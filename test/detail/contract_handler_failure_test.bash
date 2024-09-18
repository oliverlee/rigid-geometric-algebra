#!/usr/bin/env bash
set -euxo pipefail

ret=0
out="$(2>&1 ./test/detail/contract_handler_test)" || ret=$?

echo "$out" | grep "PRECONDITION FAILURE"
echo "$out" | grep "test/detail/contract_handler_test.cpp:10"
echo "$out" | grep "main()"

[ $ret -ne 0 ]
