#!/bin/sh

set -ue

ThisDir="$(dirname "$(readlink -f "$0")")"
cd "$ThisDir"
  
Files="$(find . -name '*.h')"

Dest="/usr/local/include/lr/"
mkdir -p "$Dest"

cp -v $Files  "$Dest"
