#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include "common.h"

namespace pathplanner {

struct PathAndCosts
{
    Path path;
    float finalCosts;
};

/**
 * @brief Weighted A* implementation
 */
class AStarPlanner
{
public:
    struct RoverModelConfig
    {
        /**
         * Signature:
         *     auto dist = distFunc(ax, ay, bx, by);
         */
        using DistFunc = std::function<float(int, int, int, int)>;

        /**
         * @param weight The rover's weight. Influences cost model for traveling up/downhill.
         * @param maxSpeed Rover's maximum speed, in normalized units. Rover's travelling speed in flat terrain is \a m_baseSpeed.
         * @param distFunc A user provided distance function between 2 points in the map, used by the heuristic.
         */
        RoverModelConfig(float weight, float maxSpeed, DistFunc distFunc);

        /**
         * @brief Creates a config with a default euclidian distance
         */
        RoverModelConfig(float weight, float maxSpeed);

        float m_weight;
        /**
         * Base speed is defaulted to 1.0 cell/<time unit>. Set directly for changing its value.
         */
        float m_baseSpeed;
        float m_maxSpeed;
        DistFunc m_distance;

    protected:
        /**
         * @brief euclidean distance between two nodes
         */
        static float euclideanDist(int ax, int ay, int bx, int by);
    };

    /**
     * Constructs a new A* planner.
     *
     * @param width Width of the map.
     * @param height Height of the map.
     * @param elevation vector containing elevation (elevation == 0 marks impassable nodes).
     */
    AStarPlanner(int width, int height, const std::vector<std::uint8_t>& elevation, const RoverModelConfig& modelConfig);

    /**
     * Plan a path from start to dest using currently set parameters (cf. constructor).
     *
     * @param start Position of the start node.
     * @param dest  Position of the destination node.
     * @param relaxation The A* relaxation parameter. >= 1
     * @return Found path and final costs for that path; path will be empty if none was found.
     */
    PathAndCosts plan(const Pos& start, const Pos& dest, float relaxation);

    /**
     * Static convience function to create a single instance of the planner and call plan() on it.
     *
     * @param start Position of the start node.
     * @param dest Position of the destination node.
     * @param width Width of the map.
     * @param height Height of the map.
     * @param elevation vector containing elevation (elevation == 0 marks impassable nodes).
     * @return Found path and final costs for that path; path will be empty if none was found.
     */
    static PathAndCosts plan(const Pos& start, const Pos& dest,
                             int width, int height, const std::vector<std::uint8_t>& elevation, const RoverModelConfig& modelConfig, float relaxation);

protected: // cost model related methods

    // Returns the cost of going from node a to b, also considering elevation. Works only properly
    // for neighboring nodes.
    float cost(int ax, int ay, int bx, int by) const;

    // A* heuristic: returns an upper bound for the cost for going from node to the target.
    float heuristic(int x, int y) const;

    uint8_t elevation(int x, int y) const;

    bool impassable(int x, int y) const;


private: // methods
    int mkIdx(int x, int y) const;

    const int m_width;
    const int m_height;
    const std::vector<std::uint8_t>& m_elevation;
    const RoverModelConfig m_modelConfig;

    int m_dstX;
    int m_dstY;
};

} // namespace
