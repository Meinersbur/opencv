#! /bin/bash

~/build/pencilcc/release_autotools/pencilcc --pencil-ext=.pencil --target=cuda boxFilter.pencil.c -c --no-cc "$@" -DapplyBoxFilter_cpu=applyBoxFilter -v --ppcg-arg=-v --isl-schedule-max-coefficient=2
mv boxFilter.pencil_stage_kernel.cu cuda/
