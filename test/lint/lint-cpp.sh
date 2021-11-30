#!/usr/bin/env bash
#
# Copyright (c) 2020 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
#
# Check for various C++ code patterns we want to avoid.

export LC_ALL=C

EXIT_CODE=0

OUTPUT=$(git grep -H -n -E "boost::bind\(" -- "*.cpp" "*.h")
if [[ ${OUTPUT} != "" ]]; then
    echo "Use of boost::bind detected. Use std::bind instead."
    echo
    echo "${OUTPUT}"
    [ "$CIRRUS_BASE_SHA" ] && cirrus_grep_format "$0" "use of boost:bind" "$OUTPUT"
    EXIT_CODE=1
fi

exit ${EXIT_CODE}
