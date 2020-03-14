#include "pathplanner.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>
#include <stdexcept>

#include <iostream>
#include <functional>

namespace pathplanner {

AStarPlanner::RoverModelConfig::RoverModelConfig(float weight, float maxSpeed) :
        m_weight(weight),
        m_maxSpeed(maxSpeed)
{
    if (m_weight <= 0.f)
        throw std::invalid_argument("weight");
    if (maxSpeed <= 0.f)
        throw std::invalid_argument("maxSpeed");

    m_distance = &RoverModelConfig::euclideanDist;
}

AStarPlanner::RoverModelConfig::RoverModelConfig(float weight, float maxSpeed,
                                                 std::function<float(int, int, int, int)> distFunc) :
        RoverModelConfig(weight, maxSpeed)
{
    if (!distFunc)
        throw std::invalid_argument("distFunc");
    m_distance = distFunc;
}

float AStarPlanner::RoverModelConfig::euclideanDist(int ax, int ay, int bx, int by)
{
    const float dx = ax - bx;
    const float dy = ay - by;
    return std::sqrt(dx*dx + dy*dy);
}


AStarPlanner::AStarPlanner(int width, int height, const std::vector<std::uint8_t>& elevation, const RoverModelConfig& modelConfig)
    : m_width(width), m_height(height), m_elevation(elevation), m_modelConfig(modelConfig)
{
}

inline int AStarPlanner::mkIdx(int x, int y) const
{
    return m_width*y + x;
}

PathAndCosts AStarPlanner::plan(const Pos& start, const Pos& dest,
                                int width, int height, const std::vector<std::uint8_t>& elevation, const RoverModelConfig& modelConfig, float relaxation)
{
    return AStarPlanner(width, height, elevation, modelConfig).plan(start, dest, relaxation);
}

PathAndCosts AStarPlanner::plan(const Pos& start, const Pos& dest, float relaxation)
{
    using PQNode = std::pair<float, int>;

    if (relaxation < 1.f)
    {
        throw std::invalid_argument("relaxation < 1.0");
    }

    std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> pq;
    std::vector<float> costs(m_width*m_height, std::numeric_limits<float>::infinity());
    std::vector<bool> closed(m_width*m_height, false);
    std::vector<int> predecessors(m_width*m_height, -1);

    const int dstIdx = mkIdx(dest.x, dest.y);
    m_dstX = dest.x;
    m_dstY = dest.y;

    // insert start node
    PQNode startNode{relaxation*heuristic(start.x, start.y), mkIdx(start.x, start.y) };
    costs[startNode.second] = 0;
    pq.push(startNode);

    // main loop: as long as there are still nodes that are discovered but not handled, go on
    while (!pq.empty())
    {
        // Get the top element from the queue, i.e., the one with the lowest f value (i.e., the most promising
        // one w.r.t. current estimates), and remove it.
        const auto currentIdx = pq.top().second;
        pq.pop();

        // Since we cannot modify elements in the priority queue, we simply add them as new ones.
        // However, this leads to invalid entries in the queue, which have already been handled.
        if (closed[currentIdx]) continue;

        // Is this the goal field? If yes, we've found a path.
        if (currentIdx == dstIdx)
        {
            // reconstruct path and return it
            Path path;
            int idx = currentIdx;
            while (predecessors[idx] != -1)
            {
                const int y = idx / m_width;
                const int x = idx % m_width;
                path.emplace_back(Pos{x, y});
                idx = predecessors[idx];
            }
            path.emplace_back(Pos{idx % m_width, idx / m_width});
            std::reverse(path.begin(), path.end());
            return {path, costs[dstIdx]};
        }

        // mark this node as being completely handled
        closed[currentIdx] = true;

        const int curX = currentIdx % m_width;
        const int curY = currentIdx / m_width;

        // consider all possible successor fields
        static int _dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
        static int _dy[] = { -1,  0,  1,-1, 1,-1, 0, 1 };
        for (int i = 0; i < 8; ++i)
        {
            // if (dx == 0 && dy == 0) continue;
            const auto dx = _dx[i];
            const auto dy = _dy[i];

            const int cx = curX + dx;
            const int cy = curY + dy;

            // are we allowed to move there? if not, ignore
            if (impassable(cx, cy)) continue;

            const int cIdx = mkIdx(cx, cy);

            // node already handled? if yes, ignore
            if (closed[cIdx]) continue;

            // cost of getting here
            const float g = costs[currentIdx] + cost(curX, curY, cx, cy);

            // cost greater than already found path? ignore this one
            if (g > costs[cIdx]) continue;

            // this path is best until now; record it
            predecessors[cIdx] = currentIdx; // remember predecessor node to reconstruct path later on
            costs[cIdx] = g; // remember cost for going here

            // put f value also into the Node so the priority queue works using the f value
            pq.emplace(PQNode{g + relaxation*heuristic(cx, cy), cIdx});
        }
    }

    // return empty set to indicate that we didn't find a path
    return { {}, std::numeric_limits<float>::infinity()};
}

// Returns the cost of going from node a to b, also considering elevation. Works only properly
// for neighboring nodes.
inline float AStarPlanner::cost(int ax, int ay, int bx, int by) const
{
    const float dist = m_modelConfig.m_distance(ax, ay, bx, by);

    // When going uphill, we're slower mainly because of the grade resistance which grows linearly with grade.
    // When going slower, air resistance becomes lower as well, but total resistance is still dominated by
    // grade resistance, so we can ignore the decreased air resistance here.
    // When going downhill, we can go faster (due to negative grade resistance), but at high velocities the air
    // resistance shouldn't be neglected any more. Since it grows quadratically with velocity, we take the
    // square root of the possible speed gain. Hence we reflect that it's faster to drive on flat ground than
    // to go over hills, as the additional cost of going uphill is higher than the saved cost of going downhill.
    //
    // We could model this more accurately using the math on, e.g.,
    //     http://kreuzotter.de/english/espeed.htm#forml
    // which considers additional resisting powers like rolling resistance. However, the model used here
    // also reflects the general idea.
    const float elevationChange = elevation(bx,by) - elevation(ax,ay);

    float elevationFactor = 1.0;
    if (m_modelConfig.m_weight > 0)
    {
        if (elevationChange > 0) elevationFactor = 1 / (elevationChange * (1 + m_modelConfig.m_weight));
        else if (elevationChange < 0) elevationFactor = std::sqrt(-float(elevationChange) * (1.0f + m_modelConfig.m_weight));
    }
    return dist / std::min(3.f, m_modelConfig.m_baseSpeed * elevationFactor);
}

inline float AStarPlanner::heuristic(int x, int y) const
{
    return m_modelConfig.m_distance(x, y, m_dstX, m_dstY) / m_modelConfig.m_maxSpeed;
}

inline std::uint8_t AStarPlanner::elevation(int x, int y) const
{
    return m_elevation[mkIdx(x,y)];
}

inline bool AStarPlanner::impassable(int x, int y) const
{
    // out of bounds?
    if (x < 0 || y < 0 || x >= m_width || y >= m_height) return true;
    const int idx = mkIdx(x, y);
    if (m_elevation[idx] == 0) return true;
    return false;
}

} // namespace
