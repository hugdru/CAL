#/usr/bin/env bash

cd "$(dirname "$0")"
make -C ../

sao_paulo_nodes="../data/sao_paulo/nodes.txt"
sao_paulo_roads="../data/sao_paulo/roads.txt"
sao_paulo_subroads="../data/sao_paulo/subroads.txt"
sao_paulo_start_node="415141369"
sao_paulo_goal_node="660760990"
sao_paulo_overpass_turbo_result_file="sao_paulo-overpass.txt"

../bin/CAL1516T2GE --maptxtfiles "$sao_paulo_nodes" "$sao_paulo_roads" "$sao_paulo_subroads" "$sao_paulo_start_node" "$sao_paulo_goal_node" "$sao_paulo_overpass_turbo_result_file"
