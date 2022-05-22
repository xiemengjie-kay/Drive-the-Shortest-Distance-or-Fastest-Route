// main.cpp
//
// ICS 46 Winter 2022
// Project #5: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.


#include "InputReader.hpp"
#include <iostream>
#include "RoadMapReader.hpp"
#include "TripReader.hpp"
#include <iomanip>
#include <sstream>


// convertTime() converts seconds into its components - hours,
// minutes, and seconds
std::string convertTime(double& s)
{
    std::string time = "";
    if (s >= 3600)
    {
        int t = s;
        int i = t / 3600;
        if (i == 1)
        {
            time += "1 hr ";
        }
        else
        {
            time += std::to_string(i);
            time += " hrs ";
        }
        s = s - i * 3600;
    }
    if (s >= 60)
    {
        int t = s;
        int i = t / 60;
        if (i == 1)
        {
            time += "1 min ";
        }
        else
        {
            time += std::to_string(i);
            time += " mins ";
        }
        s = s - i * 60;
    }
    return time;
}


// sortPredocessors() takes the resulf of calling findShortestPaths() and
// sorts the result into topological ordering.
std::vector<int> sortPredocessors(Trip& trip, 
    std::map<int, std::map<int, int>>& map)
{
    std::vector<int> order;
    order.push_back(trip.endVertex);
    int current = trip.endVertex;
    int pred = map[trip.startVertex][trip.endVertex];
    while (current != pred)
    {
        order.push_back(pred);
        current = pred;
        pred = map[trip.startVertex][current];
    }
    return order;
}


int main()
{
    InputReader inputReader{std::cin};
    RoadMapReader roadMapReader;
    RoadMap roadMap = roadMapReader.readRoadMap(inputReader);
    TripReader tripReader;
    std::vector<Trip> trips = tripReader.readTrips(inputReader);

    std::map<int, std::map<int, int>> shortPathMap;
    std::map<int, std::map<int, int>> shortTimeMap;
    if (roadMap.isStronglyConnected())
    {
        for (Trip& trip : trips)
        {
            if (trip.metric == TripMetric::Distance)
            {
                // the shortest path starting from the given startVertex
                // has not already been found
                if (shortPathMap.find(trip.startVertex) == shortPathMap.end())
                {
                    shortPathMap[trip.startVertex] = 
                        roadMap.findShortestPaths(trip.startVertex, 
                        [](const RoadSegment& r){ return r.miles; });
                }

                std::cout << "Shortest distance from " 
                    << roadMap.vertexInfo(trip.startVertex) << " to " <<
                    roadMap.vertexInfo(trip.endVertex) << std::endl;
                std::cout << "  Begin at " 
                    << roadMap.vertexInfo(trip.startVertex) << std::endl;
                double totalDistance = 0.0;
                std::vector<int> order = sortPredocessors(trip, shortPathMap);
                int fromVertex = trip.startVertex;
                for (int i = order.size()-2; i >= 0; i--)
                {
                    int toVertex = order[i];
                    double dis = roadMap.edgeInfo(fromVertex, toVertex).miles;
                    totalDistance += dis;
                    std::stringstream tmp;
                    tmp << std::setprecision(1) << std::fixed << dis;
                    dis = std::stod(tmp.str());
                    std::cout << "  Continue to " <<
                    roadMap.vertexInfo(order[i]) << " (" << dis<<" miles)"
                    << std::endl;
                    fromVertex = toVertex;
                }
                std::cout << "Total distance: "<< std::setprecision(1) << 
                    std::fixed << totalDistance<<" miles\n\n";
                
            }
            else if (trip.metric == TripMetric::Time)
            {
                // the shortest path starting from the given startVertex
                // has not already been found
                if (shortTimeMap.find(trip.startVertex) == shortTimeMap.end())
                {
                    shortTimeMap[trip.startVertex] = 
                        roadMap.findShortestPaths(trip.startVertex, 
                        [](const RoadSegment& r)
                        { return (r.miles/r.milesPerHour);});
                }

                std::cout << "Shortest driving time from " 
                    << roadMap.vertexInfo(trip.startVertex) << " to " <<
                    roadMap.vertexInfo(trip.endVertex) << std::endl;
                std::cout << "  Begin at " 
                    << roadMap.vertexInfo(trip.startVertex) << std::endl;
                double totalTime = 0.0;
                std::vector<int> order = sortPredocessors(trip, shortTimeMap);
                int fromVertex = trip.startVertex;
                for (int i = order.size()-2; i >= 0; i--)
                {
                    int toVertex = order[i];
                    double dis = roadMap.edgeInfo(fromVertex, toVertex).miles;
                    double mph = roadMap.edgeInfo(fromVertex, 
                        toVertex).milesPerHour;
                    double s = (dis/mph) * 3600;
                    totalTime += s;
                    std::string time = convertTime(s);
                    std::stringstream tmp;
                    tmp << std::setprecision(1) << std::fixed << s;
                    time += tmp.str() + " secs";
                    std::cout << "  Continue to " <<
                    roadMap.vertexInfo(order[i]) << " ("<< 
                    std::setprecision(1) << std::fixed << dis<<" miles @ "
                    << std::setprecision(1) << std::fixed << mph 
                    << "mph = "<<time << ")" << std::endl;
                    fromVertex = toVertex;
                }
                std::string time = convertTime(totalTime);
                std::stringstream tmp;
                tmp << std::setprecision(1) << std::fixed << totalTime;
                time += tmp.str() + " secs";
                std::cout << "Total time: "<< time << "\n\n";
            }
        }
    }
    else
    {
        std::cout << "Disconnected Map" << std::endl;
    }

    return 0;
}

