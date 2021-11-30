#!/usr/bin/env bash
#
# Copyright (c) 2017-2019 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
#
# This script runs all contrib/devtools/lint-*.sh files, and fails if any exit
# with a non-zero status code.

# This script is intentionally locale dependent by not setting "export LC_ALL=C"
# in order to allow for the executed lint scripts to opt in or opt out of locale
# dependence themselves.

set -u

SCRIPTDIR=$(dirname "${BASH_SOURCE[0]}")
LINTALL=$(basename "${BASH_SOURCE[0]}")

EXIT_CODE=0

function cirrus_grep_format()
{
    log=$(basename "${1:?}")
    msg="${2:?}"
    IFS=
    echo "${3:?}" | while read -r LINE; do
            path=$(echo "$LINE" | cut -d: -f 1)
            line=$(echo "$LINE" | cut -d: -f 2)
           cirrus_format "$0" "$msg" "$path" "$line"
    done
}

function cirrus_format()
{
    log=$(basename "${1:?}")
    msg="${2:?}"
    path="${3:?}"
    line="${4:?}"
    msg=${msg/\"//g}
    printf '{"level": "failure", "message": "%s: %s", "path": "%s", "start_line": %s, "end_line": %s}\n'  "$log" "$msg" "$path" "$line" "$line" >> "$REPORTS_DIR/${log}-cirrus.json"
}

export -f cirrus_format
export -f cirrus_grep_format

for f in "${SCRIPTDIR}"/lint-*.sh; do
  if [ "$(basename "$f")" != "$LINTALL" ]; then
    if ! "$f"; then
      echo "^---- failure generated from $f"
      EXIT_CODE=1
    fi
  fi
done

exit ${EXIT_CODE}
