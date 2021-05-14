#!/bin/sh

for F in Fuzzers/*; do

  [ -x "${F}" -a -f "${F}" ] || continue

  NAME=$(basename "$F")
  mkdir -p "${NAME}"
  ln -sf ../Fuzzers/${NAME} ${NAME}/fuzzer

done
