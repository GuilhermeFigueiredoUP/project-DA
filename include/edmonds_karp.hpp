#pragma once

#include "../data_structures/Graph.h"
#include "../include/node_data.hpp"

/// @brief Computes the maximum flow in a graph using the Edmonds-Karp algorithm
/// @param graph The graph to compute the maximum flow for
/// @param source The source vertex
/// @param sink The sink vertex
/// @return The maximum flow value
int computeEdmondsKarp(Graph<DataNode> &graph, DataNode source, DataNode sink);

/// @brief Tests and visits a vertex in the graph
/// @tparam T template type for the graph vertices
/// @param q queue of vertices to visit
/// @param e edge being evaluated
/// @param w vertex being evaluated
/// @param residual residual capacity of the edge
template <class T>
void testAndVisit(std::queue< Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual);

/// @brief Finds an augmenting path in the graph using BFS
/// @tparam T template type for the graph vertices
/// @param g The graph to search in
/// @param s The source vertex
/// @param t The sink vertex
/// @return True if an augmenting path is found, false otherwise
template <class T>
bool findAugmentingPath(Graph<T> *g, Vertex<T> *s, Vertex<T> *t);

/// @brief Finds the minimum residual capacity along a path
/// @tparam T template type for the graph vertices
/// @param s The source vertex
/// @param t The sink vertex
/// @return The minimum residual capacity along the path
template <class T>
double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t);

/// @brief Augments the flow along a path
/// @tparam T template type for the graph vertices
/// @param s The source vertex
/// @param t The sink vertex
/// @param f The flow to augment along the path
template <class T>
void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f);