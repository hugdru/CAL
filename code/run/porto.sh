#/usr/bin/env bash

cd "$(dirname "$0")"
make -C ../

porto_nodes="../data/porto/nodes.txt"
porto_roads="../data/porto/roads.txt"
porto_subroads="../data/porto/subroads.txt"
porto_start_node="25503962"
porto_goal_node="4097873936"
porto_overpass_turbo_result_file="porto-overpass.txt"

../bin/CAL1516T2GE --maptxtfiles "$porto_nodes" "$porto_roads" "$porto_subroads" "$porto_start_node" "$porto_goal_node" "$porto_overpass_turbo_result_file"
# valgrind --leak-check=full --show-leak-kinds=all --tool=memcheck -v ../bin/CAL1516T2GE --maptxtfiles "$porto_nodes" "$porto_roads" "$porto_subroads" "$porto_start_node" "$porto_goal_node" "$porto_overpass_turbo_result_file"
