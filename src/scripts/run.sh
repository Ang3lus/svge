#!/usr/bin/env bash
LSAN_OPTIONS=suppressions=sanitizers/leak.supp ./bin/svge
