#pragma once

#include "../data_structures/Graph.h"
#include "../include/node_data.hpp"

int computeEdmondsKarp(Graph<DataNode> &graph, DataNode source, DataNode sink);
template <class T>
void testAndVisit(std::queue< Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual);
template <class T>
bool findAugmentingPath(Graph<T> *g, Vertex<T> *s, Vertex<T> *t);
template <class T>
double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t);
template <class T>
void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f);