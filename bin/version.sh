#!/bin/sh
export CHANGES=""
git diff-index --quiet HEAD --
if [ $? ]
then
    CHANHES="-${CHANGES}"
fi
cat > src/version.cpp <<EOF
    char* VERSION = "$(git rev-parse --short HEAD)${CHANGES}";
EOF
