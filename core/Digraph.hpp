// Constraints on VertexInfo and EdgeInfo: both of them need to have
// copy constructors, assignment operators, and destructors.

// Digraph.hpp
//
// ICS 46 Winter 2022
// Project #5: Rock and Roll Stops the Traffic
//
// This header file declares a class template called Digraph, which is
// intended to implement a generic directed graph.  The implementation
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//
// In general, directed graphs are all the same, except in the sense
// that they store different kinds of information about each vertex and
// about each edge; these two types are the type parameters to the
// Digraph class template.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <exception>
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>


// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}



// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a struct template.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a struct template.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};



// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
// You'll need to implement the member functions declared here; each has a
// comment detailing how it is intended to work.
//
// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one (i.e., any change to the copy will not affect the
    // original).
    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d) noexcept;

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph() noexcept;

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d) noexcept;

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const noexcept;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const noexcept;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the predecessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;


private:
    // Add whatever member variables you think you need here.  One
    // possibility is a std::map where the keys are vertex numbers
    // and the values are DigraphVertex<VertexInfo, EdgeInfo> objects.
    int vertexCount_;
    int edgeCount_;
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> adjList;


    // You can also feel free to add any additional member functions
    // you'd like (public or private), so long as you don't remove or
    // change the signatures of the ones that already exist.

    // DFTr() depth first traverses the graph starting at the given
    // vertex, and returns the number of vertices it visits
    void DFTr(const std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>& g, 
        const DigraphVertex<VertexInfo, EdgeInfo>& v, int num,
        std::vector<int>& visited, int& find) const;

};



// You'll need to implement the member functions below.  There's enough
// code in place to make them compile, but they'll all need to do the
// correct thing instead.

template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph()
    : vertexCount_{0}, edgeCount_{0}
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
    : vertexCount_{0}, edgeCount_{0}
{
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> tempList;
    for (auto& [num, vertex] : d.adjList)
    {
        // copy constructors of corresponding objects are called
        tempList[num] = vertex;
    }
    adjList = tempList;
    vertexCount_ = d.vertexCount_;
    edgeCount_ = d.edgeCount_;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(Digraph&& d) noexcept
    : vertexCount_{0}, edgeCount_{0}
{
    std::swap(vertexCount_, d.vertexCount_);
    std::swap(edgeCount_, d.edgeCount_);
    std::swap(adjList, d.adjList);
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph() noexcept
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
    if (this != &d)
    {
        std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> tempList;
        for (auto& [num, vertex] : d.adjList)
        {
            tempList[num] = vertex;
        }
        adjList = tempList;
        vertexCount_ = d.vertexCount_;
        edgeCount_ = d.edgeCount_;
    }
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(Digraph&& d) noexcept
{
    if (this != &d)
    {
        std::swap(vertexCount_, d.vertexCount_);
        std::swap(edgeCount_, d.edgeCount_);
        std::swap(adjList, d.adjList); 
    }
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{
    std::vector<int> result;
    for (auto& [num, vertex] : adjList)
    {
        result.push_back(num);
    }
    return result;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
    std::vector<std::pair<int, int>> result;
    for (auto& [num, vertex] : adjList)
    {
        for (auto& edge : vertex.edges)
        {
            result.push_back({edge.fromVertex, edge.toVertex});
        }
    }
    return result;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
    if (adjList.find(vertex) == adjList.end())
    {
        throw DigraphException{"Digraph edges(): the given vertex does not exist."};
    }
    else
    {
        std::vector<std::pair<int, int>> result;
        for (auto& edge : adjList.at(vertex).edges)
        {
            result.push_back({edge.fromVertex, edge.toVertex});
        }
        return result;
    }
}


template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
    if (adjList.find(vertex) == adjList.end())
    {
        throw DigraphException{"Digraph vertexInfo(): the given vertex does not exist."};
    }
    return adjList.at(vertex).vinfo;
}


template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
    if (adjList.find(fromVertex) == adjList.end() || 
        adjList.find(toVertex) == adjList.end())
    {
        throw DigraphException{"Digraph edgeInfo(): either of vertices does not exist."};
    }
    else
    {
        for (auto& edge : adjList.at(fromVertex).edges)
        {
            if (edge.toVertex == toVertex) return edge.einfo;
        }
        throw DigraphException{"Digraph edgeInfo(): the edge does not exist."};
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
    if (adjList.find(vertex) != adjList.end())
    {
        std::string errorMessage{""};
        errorMessage += "Digraph addVertex(): vertex number "
            + std::to_string(vertex) + " has already existed in the graph.";
        throw DigraphException{errorMessage}; 
    }
    else
    {
        DigraphVertex<VertexInfo, EdgeInfo> v;
        v.vinfo = vinfo;
        adjList[vertex] = v;
        vertexCount_++;
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
{
    if (adjList.find(fromVertex) == adjList.end() || 
        adjList.find(toVertex) == adjList.end())
    {
        throw DigraphException{"Digraph addEdge(): one of the vertices does not exist."};
    }
    else
    {   
        for (auto& edge : adjList[fromVertex].edges)
        {
            if (edge.fromVertex == fromVertex && edge.toVertex == toVertex)
            {
                throw DigraphException{"Digraph addEdge(): the same edge is already present in the graph."};
            }
        }
        DigraphEdge<EdgeInfo> e{fromVertex, toVertex, einfo};
        adjList[fromVertex].edges.push_back(e);
        edgeCount_++;
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
    if (adjList.find(vertex) == adjList.end())
    {
        throw DigraphException{"Digraph removeVertex(): the vertex does not exist already."};
    }
    else
    {
        edgeCount_ -= adjList[vertex].edges.size();
        adjList.erase(vertex);
        vertexCount_--;
        for (auto& [num, dVertex] : adjList)
        {
            for (auto edge = dVertex.edges.begin(); edge!= dVertex.edges.end();
                ++edge)
            {
                if ((*edge).toVertex == vertex)
                {
                    dVertex.edges.erase(edge);
                    edgeCount_--;
                    break;
                }
            }
        }
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
    if (adjList.find(fromVertex) == adjList.end() || 
        adjList.find(toVertex) == adjList.end())
    {
        throw DigraphException{"Digraph removeEdge(): one of the vertices does not exist."};
    }
    else
    {
        std::list<DigraphEdge<EdgeInfo>>& dEdges = adjList[fromVertex].edges;
        int oldSz = dEdges.size();
        dEdges.remove_if([&](DigraphEdge<EdgeInfo>& e)
            {return e.toVertex == toVertex;});
        int newSz = dEdges.size();
       
        if (newSz < oldSz)
        {
            edgeCount_--;
        }
        else
        {
            throw DigraphException{"Digraph removeEdge(): the edge is not already present in the graph."};
        }
    }

}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const noexcept
{
    return vertexCount_;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const noexcept
{
    return edgeCount_;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
    if (adjList.find(vertex) == adjList.end())
    {
        throw DigraphException{"Digraph edgeCount(): the vertex does not exist."};
    }
    return adjList.at(vertex).edges.size();
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::DFTr(
    const std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>& g, 
    const DigraphVertex<VertexInfo, EdgeInfo>& v, int num,
    std::vector<int>& visited, int& find) const
{
    visited.push_back(num);
    find++;

    for (auto& edge : g.at(num).edges)
    {
        if (!std::any_of(visited.begin(), visited.end(), 
            [&](int i){ return i == edge.toVertex; }))
        {
            DFTr(g, g.at(edge.toVertex), edge.toVertex, visited, find);
        }
    }
}


template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
    for (auto& [num, vertex] : adjList)
    {
        std::vector<int> visited;
        // test, for each vertex, we'll be able to reach every other vertex
        int find = 0;
        DFTr(adjList, vertex, num, visited, find);
        if (vertexCount_ != find) return false;
    }
    return true;
}


template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
    int startVertex,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
    if (adjList.find(startVertex) == adjList.end())
    {
        throw DigraphException{"Digraph findShortestPaths(): the startVertex is not valid."};
    }
    std::map<int, int> result;
    std::map<int, bool> k;
    std::map<int, double> d;

    for (auto& [num, vertex] : adjList)
    {
        k[num] = false;
        result[num] = num;
        if (num == startVertex) d[num] = 0.0;
        else d[num] = std::numeric_limits<double>::infinity();
    }

    auto compare = [&](const int& num1, const int& num2)
        {return d[num1] > d[num2];};
    std::priority_queue<int, std::vector<int>, decltype(compare)> pq(compare);
    pq.push(startVertex);

    while(!pq.empty())
    {
        const int vNum = pq.top();
        pq.pop();

        if (k[vNum] == false)
        {
            k[vNum] = true;
           
            for (auto& edge : adjList.at(vNum).edges)
            {
                if (d[edge.toVertex] > d[vNum] + edgeWeightFunc(edge.einfo))
                {
                    d[edge.toVertex] = d[vNum] + edgeWeightFunc(edge.einfo);
                    result[edge.toVertex] = vNum;
                    pq.push(edge.toVertex);
                }
            }
        }
    }

    return result;
}



#endif

