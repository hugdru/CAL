#/usr/bin/env bash

cd "$(dirname "$0")"
make -C ../

lisboa_super_big_nodes="../data/lisboa_super_big/nodes.txt"
lisboa_super_big_roads="../data/lisboa_super_big/roads.txt"
lisboa_super_big_subroads="../data/lisboa_super_big/subroads.txt"
lisboa_super_big_start_node="410697669"
lisboa_super_big_goal_node="416828390"
lisboa_super_big_overpass_turbo_result_file="lisboa_super_big-overpass.txt"

../bin/CAL1516T2GE --maptxtfiles "$lisboa_super_big_nodes" "$lisboa_super_big_roads" "$lisboa_super_big_subroads" "$lisboa_super_big_start_node" "$lisboa_super_big_goal_node" "$lisboa_super_big_overpass_turbo_result_file"
