#!/usr/bin/env bash

pushd `dirname $0` > /dev/null
SCRIPTPATH=`pwd -P`
popd > /dev/null

LSAN_OPTIONS=suppressions=$SCRIPTPATH/sanitizers/leak.supp $SCRIPTPATH/bin/svge
