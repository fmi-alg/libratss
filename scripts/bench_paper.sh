#!/bin/bash

DATA_PATH=/data/osm/pbfs
PRG_PATH=./

for d in $(ls -1S ${DATA_PATH} | tac); do
        for sig in 23 32 53 64 128; do
                echo "${d} -> ${d}_${sig}.stats"
                ${PRG_PATH}/paper_bench -fp "poi" -p ${p} -o ${d}_${p}.stats ${DATA_PATH}/${d}
        done
done
