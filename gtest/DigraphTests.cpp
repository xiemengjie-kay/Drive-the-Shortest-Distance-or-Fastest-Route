#include <gtest/gtest.h>
#include "Digraph.hpp"


TEST(DigraphTests, emptyDigraphVertexCountAndEdgeCountAreZeros)
{
    Digraph<std::string, int> d;
    EXPECT_EQ(0, d.vertexCount());
    EXPECT_EQ(0, d.edgeCount());
}


TEST(DigraphTests, addSeveralVertices)
{
    Digraph<std::string, int> d;
    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");

    EXPECT_EQ(3, d.vertexCount());
    EXPECT_EQ(0, d.edgeCount());
}


TEST(DigraphTests, addVertexTheGivenVertexNumberIsOccupied_ThrowDigraphException)
{
    Digraph<std::string, int> d;
    d.addVertex(0, "a");
    EXPECT_THROW(d.addVertex(0, "b"), DigraphException);
}


TEST(DigraphTests, addEdgeThrowException)
{
    Digraph<std::string, int> d;
    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");

    EXPECT_THROW(d.addEdge(0, 3, 8), DigraphException);
    EXPECT_THROW(d.addEdge(-1, 1, -8), DigraphException);
    EXPECT_THROW(d.addEdge(9, 5, 10), DigraphException);

    d.addEdge(0, 1, 8);
    EXPECT_THROW(d.addEdge(0, 1, 8), DigraphException);
 
    d.addEdge(0, 2, 6);
    EXPECT_EQ(2, d.edgeCount());
}


TEST(DigraphTests, countEdgeForGivenVertexNumber)
{
    Digraph<std::string, int> d;
    EXPECT_THROW(d.edgeCount(-1), DigraphException);
    EXPECT_THROW(d.edgeCount(0), DigraphException);

    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");

    d.addEdge(0, 1, 8);
    d.addEdge(0, 2, 6);
    EXPECT_EQ(2, d.edgeCount());
    EXPECT_EQ(3, d.vertexCount());
    EXPECT_EQ(2, d.edgeCount(0));
    EXPECT_EQ(0, d.edgeCount(1));
    EXPECT_EQ(0, d.edgeCount(2));
}


TEST(DigraphTests, verticesReturnVectorOfVertexNumbers)
{
    Digraph<std::string, int> d;
    std::vector<int> v1;
    EXPECT_EQ(v1, d.vertices());

    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(-1, "c");

    d.addEdge(0, 1, 8);
    d.addEdge(0, -1, 6);
    
    std::vector<int> v{-1, 0, 1};
    EXPECT_EQ(v, d.vertices());
}


TEST(DigraphTests, edgesReturnVectorOfAllEdges)
{
    Digraph<std::string, int> d;

    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(-1, "c");

    std::vector<std::pair<int, int>> e;
    EXPECT_EQ(e, d.edges());

    d.addEdge(0, 1, 8);
    d.addEdge(0, -1, 6);
    d.addEdge(1, -1, 10);
    
    std::vector<std::pair<int, int>> e2{{0, 1}, {0, -1}, {1, -1}};
    EXPECT_EQ(e2, d.edges());
}


TEST(DigraphTests, edgesReturnVectorOfAllEdgesAtGivenVertexNumber)
{
    Digraph<std::string, int> d;

    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");

    std::vector<std::pair<int, int>> e;
    for (unsigned int i = 0; i < 3; i++)
    {
        EXPECT_EQ(e, d.edges(i));
    }
    EXPECT_THROW(d.edges(-1), DigraphException);

    d.addEdge(0, 1, 8);
    d.addEdge(0, 2, 6);
    d.addEdge(1, 2, 10);
    
    std::vector<std::pair<int, int>> e2{{0, 1}, {0, 2}};
    EXPECT_EQ(e2, d.edges(0));
    std::vector<std::pair<int, int>> e3{{1, 2}};
    EXPECT_EQ(e3, d.edges(1));
}


TEST(DigraphTests, vertexInfo)
{
    Digraph<std::string, int> d;

    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(-1, "c");

    d.addEdge(0, 1, 8);
    d.addEdge(0, -1, 6);
    d.addEdge(1, -1, 10);
    
    EXPECT_THROW(d.vertexInfo(5), DigraphException);
    EXPECT_EQ("a", d.vertexInfo(0));
    EXPECT_EQ("b", d.vertexInfo(1));
}


TEST(DigraphTests, edgeInfo)
{
    Digraph<std::string, int> d;

    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");

    d.addEdge(0, 1, 8);
    d.addEdge(0, 2, 6);
    d.addEdge(1, 2, 10);
    
    EXPECT_THROW(d.edgeInfo(1, 5), DigraphException);
    EXPECT_THROW(d.edgeInfo(-1, 3), DigraphException);
    EXPECT_THROW(d.edgeInfo(2, 1), DigraphException);
    EXPECT_EQ(6, d.edgeInfo(0, 2));
    EXPECT_EQ(10, d.edgeInfo(1, 2));
}


TEST(DigraphTests, removeVertex)
{
    Digraph<std::string, int> d;

    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");

    d.addEdge(0, 1, 8);
    d.addEdge(0, 2, 6);
    d.addEdge(1, 2, 10);
    d.addVertex(3, "d");
    d.addEdge(2, 3, 3);
    
    d.removeVertex(2);
    EXPECT_THROW(d.edgeInfo(0, 2), DigraphException);
    EXPECT_THROW(d.removeVertex(2), DigraphException);
    EXPECT_EQ(3, d.vertexCount());
    EXPECT_EQ(1, d.edgeCount());

    d.removeVertex(1);
    EXPECT_EQ(2, d.vertexCount());
    EXPECT_EQ(0, d.edgeCount());

}


TEST(DigraphTests, removeEdge)
{
    Digraph<std::string, int> d;

    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");

    d.addEdge(0, 1, 8);
    d.addEdge(0, 2, 6);
    d.addEdge(1, 2, 10);
    d.addVertex(3, "d");
    d.addEdge(2, 3, 3);
    
    EXPECT_THROW(d.removeEdge(0, 5), DigraphException);
    EXPECT_THROW(d.removeEdge(1, 0), DigraphException);

    d.removeEdge(0, 2);
    EXPECT_EQ(4, d.vertexCount());
    EXPECT_EQ(3, d.edgeCount());
}


TEST(DigraphTests, copyConstructor)
{
    Digraph<std::string, int> d;
    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");
    d.addEdge(0, 1, 8);
    d.addEdge(0, 2, 6);
    d.addEdge(1, 2, 10);
    d.addVertex(3, "d");
    d.addEdge(2, 3, 3);

    Digraph<std::string, int> d2 = d;
    EXPECT_EQ(d2.vertexCount(), d.vertexCount());
    EXPECT_EQ(d2.edgeCount(), d.edgeCount());

    d2.removeEdge(2, 3);
    EXPECT_EQ(3, d2.edgeCount());
    EXPECT_EQ(4, d.edgeCount());

    d.removeVertex(0);
    EXPECT_EQ(3, d.vertexCount());
    EXPECT_EQ(2, d.edgeCount());
    EXPECT_EQ(4, d2.vertexCount());
    EXPECT_EQ(3, d2.edgeCount());

    d2.addEdge(1, 3, 15);
    EXPECT_EQ(4, d2.vertexCount());
    EXPECT_EQ(4, d2.edgeCount());
}


TEST(DigraphTests, assignmentOperator)
{
    Digraph<std::string, int> d;
    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");
    d.addEdge(0, 1, 8);
    d.addEdge(0, 2, 6);
    d.addEdge(1, 2, 10);
    d.addVertex(3, "d");
    d.addEdge(2, 3, 3);

    Digraph<std::string, int> d2;
    d2.addVertex(10, "x");
    d2.addVertex(20, "c");
    d2.addEdge(10, 20, 15);
    d2.addEdge(20, 10, 20);

    d2 = d;
    EXPECT_EQ(d2.vertexCount(), d.vertexCount());
    EXPECT_EQ(d2.edgeCount(), d.edgeCount());

    d2.removeEdge(2, 3);
    EXPECT_EQ(3, d2.edgeCount());
    EXPECT_EQ(4, d.edgeCount());

    d.removeVertex(0);
    EXPECT_EQ(3, d.vertexCount());
    EXPECT_EQ(2, d.edgeCount());
    EXPECT_EQ(4, d2.vertexCount());
    EXPECT_EQ(3, d2.edgeCount());

    d2.addEdge(1, 3, 15);
    EXPECT_EQ(4, d2.vertexCount());
    EXPECT_EQ(4, d2.edgeCount());
}


TEST(DigraphTests, moveConstructor)
{
    Digraph<std::string, int> d;
    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");
    d.addEdge(0, 1, 8);
    d.addEdge(0, 2, 6);
    d.addEdge(1, 2, 10);
    d.addVertex(3, "d");
    d.addEdge(2, 3, 3);

    Digraph<std::string, int> d2 = std::move(d);
    EXPECT_EQ(4, d2.vertexCount());
    EXPECT_EQ(4, d2.edgeCount());
    EXPECT_EQ(0, d.edgeCount());
    EXPECT_EQ(0, d.vertexCount());

    d2.removeEdge(2, 3);
    EXPECT_EQ(3, d2.edgeCount());
    EXPECT_EQ(0, d.edgeCount());
}


TEST(DigraphTests, moveAssignmentOperator)
{
    Digraph<std::string, int> d;
    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");
    d.addEdge(0, 1, 8);
    d.addEdge(0, 2, 6);
    d.addEdge(1, 2, 10);
    d.addVertex(3, "d");
    d.addEdge(2, 3, 3);

    Digraph<std::string, int> d2;
    d2.addVertex(10, "x");
    d2.addVertex(20, "c");
    d2.addEdge(10, 20, 15);
    d2.addEdge(20, 10, 20);
    
    d2 = std::move(d);
    EXPECT_EQ(4, d2.vertexCount());
    EXPECT_EQ(4, d2.edgeCount());
    EXPECT_EQ(2, d.edgeCount());
    EXPECT_EQ(2, d.vertexCount());

    d2.removeEdge(2, 3);
    EXPECT_EQ(3, d2.edgeCount());
    EXPECT_EQ(2, d.edgeCount());
}


TEST(DigraphTests, isStronglyConnected)
{
    Digraph<std::string, int> d;
    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");
    d.addEdge(0, 1, 8);
    d.addEdge(2, 0, 6);
    d.addEdge(1, 2, 10);
    d.addEdge(0, 2, 7);
    EXPECT_TRUE(d.isStronglyConnected());

    d.addVertex(3, "d");
    d.addEdge(2, 3, 3);
    EXPECT_FALSE(d.isStronglyConnected());
}


TEST(DigraphTests, findShortestPathsInDAG)
{
    Digraph<std::string, double> d;
    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");
    d.addVertex(3, "d");
    d.addVertex(4, "e");
    d.addVertex(5, "f");
    d.addVertex(6, "g");

    d.addEdge(0, 1, 8.0);
    d.addEdge(0, 2, 6.0);
    d.addEdge(1, 3, 10.0);
    d.addEdge(2, 3, 15.0);
    d.addEdge(2, 4, 9.0);
    d.addEdge(3, 4, 14.0);
    d.addEdge(3, 5, 4.0);
    d.addEdge(4, 5, 13.0);
    d.addEdge(4, 6, 17.0);
    d.addEdge(5, 6, 7.0);

    EXPECT_FALSE(d.isStronglyConnected());
    EXPECT_EQ(7, d.vertexCount());
    EXPECT_EQ(10, d.edgeCount());

    std::map<int, int> sp{{0,0}, {1,0}, {2,0}, {3,1}, {4,2}, {5,3}, {6,5}};
    EXPECT_EQ(sp, d.findShortestPaths(0, [](const double& e){ return e;}));
}


TEST(DigraphTests, findShortestPathsInDG)
{
    Digraph<std::string, double> d;
    d.addVertex(0, "a");
    d.addVertex(1, "b");
    d.addVertex(2, "c");
    d.addVertex(3, "d");
    d.addVertex(4, "e");

    d.addEdge(0, 1, 1);
    d.addEdge(1, 3, 4);
    d.addEdge(3, 2, 5);
    d.addEdge(2, 0, 7);
    d.addEdge(3, 4, 10.0);
    d.addEdge(2, 4, 9.0);
    d.addEdge(4, 2, 12.0);
    
    EXPECT_TRUE(d.isStronglyConnected());
    EXPECT_EQ(5, d.vertexCount());
    EXPECT_EQ(7, d.edgeCount());

    std::map<int, int> sp{{0,0}, {1,0}, {2,3}, {3,1}, {4,3}};
    EXPECT_EQ(sp, d.findShortestPaths(0, [](const double& e){ return e;}));
}


TEST(DigraphTests, singleVertex)
{
    Digraph<std::string, int> d;
    d.addVertex(0, "a");
    EXPECT_TRUE(d.isStronglyConnected());
}
