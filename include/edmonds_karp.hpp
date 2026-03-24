#pragma once

#include "../data_structures/Graph.h"
#include "../include/node_data.hpp"

int computeEdmondsKarp(Graph<DataNode> &graph, DataNode source, DataNode sink);