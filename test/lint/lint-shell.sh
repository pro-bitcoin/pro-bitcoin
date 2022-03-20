#!/usr/bin/env bash
#
# Copyright (c) 2018-2021 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
#
# Check for shellcheck warnings in shell scripts.

export LC_ALL=C

# Disabled warnings:
disabled=(
    SC2162 # read without -r will mangle backslashes.
)

EXIT_CODE=0

if ! command -v shellcheck > /dev/null; then
    echo "Skipping shell linting since shellcheck is not installed."
    exit $EXIT_CODE
fi

SHELLCHECK_CMD=(shellcheck --external-sources --check-sourced --source-path=SCRIPTDIR)
EXCLUDE="--exclude=$(IFS=','; echo "${disabled[*]}")"
# Check shellcheck directive used for sourced files
mapfile -t SOURCED_FILES < <(git ls-files | xargs gawk '/^# shellcheck shell=/ {print FILENAME} {nextfile}')
mapfile -t GUIX_FILES < <(git ls-files contrib/guix contrib/shell | xargs gawk '/^#!\/usr\/bin\/env bash/ {print FILENAME} {nextfile}')
mapfile -t FILES < <(git ls-files -- '*.sh' | grep -vE 'src/(leveldb|secp256k1|minisketch|univalue)/')
SHELLCHECK_LOG=/tmp/$$
if ! "${SHELLCHECK_CMD[@]}" "$EXCLUDE" "${SOURCED_FILES[@]}" "${GUIX_FILES[@]}" "${FILES[@]}" > $SHELLCHECK_LOG; then
    cat "$SHELLCHECK_LOG"
    # test/lint/lint-python.sh:11:1: warning: MYPY_REPORT_FILE appears unused. Verify use (or export if used externally). [SC2034]
    OLDIFS="$IFS"
    IFS=
    set -x
    while read -r LINE; do
        msg="$(echo "$LINE" | cut -d : -f 4- | sed 's/"//g')"
        path="$(echo "$LINE" | cut -d : -f 1)"
        line="$(echo "$LINE" | cut -d : -f 2)"
        [ -n "$CIRRUS_BASE_SHA" ] && cirrus_format "$0" "$msg" "$path" "$line"
    done < "$SHELLCHECK_LOG"
    IFS="$OLDIFS"
    EXIT_CODE=1
fi

exit $EXIT_CODE
