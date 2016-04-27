#/usr/bin/env bash

cd "$(dirname "$0")"
make -C ../

lisboa_small_nodes="../data/lisboa_small/nodes.txt"
lisboa_small_roads="../data/lisboa_small/roads.txt"
lisboa_small_subroads="../data/lisboa_small/subroads.txt"
lisboa_small_start_node="1709297025"
lisboa_small_goal_node="2704229029"
lisboa_small_overpass_turbo_result_file="lisboa_small-overpass.txt"

../bin/CAL1516T2GE --maptxtfiles "$lisboa_small_nodes" "$lisboa_small_roads" "$lisboa_small_subroads" "$lisboa_small_start_node" "$lisboa_small_goal_node" "$lisboa_small_overpass_turbo_result_file"
