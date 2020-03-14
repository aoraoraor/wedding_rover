#include "pathplanner.h"

#include <gtest/gtest.h>
#include <iostream>

struct World
{
    std::vector<uint8_t> elevation;
    std::vector<bool   > impassable;
    int width, height;

private:
    static World buildSimple()
    {
        World world;
        world.width = 20;
        world.height = 20;
        world.elevation = std::vector<uint8_t>(world.width * world.height, 1);
        world.impassable = std::vector<bool>(world.width * world.height, false);
        return world;
    }
    static World buildWall()
    {
        World world = buildSimple();
        for (int x = 6; x <= 9; ++x) world.impassable[3 * world.width + x] = true;
        for (int y = 2; y <= 6; ++y) world.impassable[y * world.width + 12] = true;
        return world;
    }
    static World buildMountain()
    {
        World world = buildWall();
        for (int y = 7; y <= 19; ++y)
        {
            world.elevation[y * world.width +  9] += 1;
            world.elevation[y * world.width + 10] += 2;
            world.elevation[y * world.width + 11] += 4;
            world.elevation[y * world.width + 12] += 5;
            world.elevation[y * world.width + 13] += 3;
            world.elevation[y * world.width + 14] += 2;
        }
        return world;
    }
    static World buildDownhill()
    {
        World world = buildWall();
        for (int x = 0; x <= 15; ++x)
        {
            for (int y = 0; y < 20; y++)
            {
                world.elevation[y * world.width + x] += (15-x);
            }
        }
        return world;
    }

    static World buildMoreWalls()
    {
        World world = buildSimple();
        for (int x = 1; x <= 3; ++x) world.impassable[1 * world.width + x] = true;
        for (int y = 2; y <= 13; ++y) world.impassable[y * world.width + 1] = true;
        for (int x = 2; x <= 12; ++x) world.impassable[13 * world.width + x] = true;
        for (int y = 0; y <= 11; ++y) world.impassable[y * world.width + 5] = true;
        for (int y = 0; y <= 11; ++y) world.impassable[y * world.width + 9] = true;
        for (int y = 4; y <= 12; ++y) world.impassable[y * world.width + 7] = true;
        for (int y = 1; y <= 12; ++y) world.impassable[y * world.width + 11] = true;
        for (int y = 4; y <= 12; ++y) world.impassable[y * world.width + 12] = true;
        for (int y = 1; y <= 4; ++y) world.impassable[y * world.width + 13] = true;

        //for (int y = 14; y < 20; ++y) world.impassable[y * world.width + 10] = true;

        return world;
    }


public:
    static const World& simple() { static const World world = buildSimple();  return world; }
    static const World& wall() { static const World world = buildWall();  return world; }
    static const World& mountain() { static const World world = buildMountain();  return world; }
    static const World& downhill() { static const World world = buildDownhill();  return world; }
    static const World& moreWalls() { static const World world = buildMoreWalls();  return world; }
};

TEST(PathPlannerTest, TestSimple1)
{
    const World& world = World::simple();
    AStarPlanner planner(world.width, world.height, world.elevation, world.impassable, 1);

    const std::vector<std::pair<int, int> > path = planner.plan(1, 1, 7, 1);

    int x = 1;
    for (auto it = path.begin(); it != path.end(); ++it, ++x) ASSERT_EQ(std::make_pair(x, 1), *it);
    ASSERT_NEAR(6, planner.getFinalCost(), 1e-4);
}

TEST(PathPlannerTest, TestSimple2)
{
    const World& world = World::simple();
    AStarPlanner planner(world.width, world.height, world.elevation, world.impassable, 1);

    const std::vector<std::pair<int, int> > path = planner.plan(1, 3, 9, 11);
    int x = 1, y = 3;
    for (auto it = path.begin(); it != path.end(); ++it, ++x, ++y) ASSERT_EQ(std::make_pair(x, y), *it);
    ASSERT_NEAR(8 * sqrt(2), planner.getFinalCost(), 1e-4);
}

TEST(PathPlannerTest, TestSimple3)
{
    const World& world = World::simple();
    AStarPlanner planner(world.width, world.height, world.elevation, world.impassable, 1);

    const std::vector<std::pair<int, int> > path = planner.plan(15, 2, 15, 19);
    int y = 2;
    for (auto it = path.begin(); it != path.end(); ++it, ++y) ASSERT_EQ(std::make_pair(15, y), *it);
    ASSERT_NEAR(17, planner.getFinalCost(), 1e-4);
}

TEST(PathPlannerTest, TestSimple4)
{
    const World& world = World::simple();
    AStarPlanner planner(world.width, world.height, world.elevation, world.impassable, 1);

    const std::vector<std::pair<int, int> > path = planner.plan(17, 9, 6, 12);
    // many paths possible, so we don't check actual path
    ASSERT_NEAR(3 * sqrt(2) + 8, planner.getFinalCost(), 1e-4);
}

TEST(PathPlannerTest, TestWall1)
{
    const World& world = World::wall();
    AStarPlanner planner(world.width, world.height, world.elevation, world.impassable, 1);

    const std::vector<std::pair<int, int> > path = planner.plan(5, 3, 7, 2);

    const int expX[] = { 5, 6, 7 };
    const int expY[] = { 3, 2, 2 };
    int i = 0;
    for (auto it = path.begin(); it != path.end(); ++it, ++i) ASSERT_EQ(std::make_pair(expX[i], expY[i]), *it);
    ASSERT_NEAR(sqrt(2) + 1, planner.getFinalCost(), 1e-4);
}

TEST(PathPlannerTest, TestWall2)
{
    const World& world = World::wall();
    AStarPlanner planner(world.width, world.height, world.elevation, world.impassable, 1);

    const std::vector<std::pair<int, int> > path = planner.plan(7, 5, 13, 2);
    ASSERT_NEAR(1 + 5 * sqrt(2), planner.getFinalCost(), 1e-4);
}

TEST(PathPlannerTest, TestWall3)
{
    const World& world = World::wall();
    AStarPlanner planner(world.width, world.height, world.elevation, world.impassable, 1);

    const std::vector<std::pair<int, int> > path = planner.plan(4, 3, 13, 4);
    ASSERT_NEAR(8 + 3 * sqrt(2), planner.getFinalCost(), 1e-4);
}

TEST(PathPlannerTest, TestMountain1)
{
    const World& world = World::mountain();
    AStarPlanner planner(world.width, world.height, world.elevation, world.impassable, 1);

    // shouldn't be affected by mountain
    const std::vector<std::pair<int, int> > path = planner.plan(5, 3, 7, 2);

    const int expX[] = { 5, 6, 7 };
    const int expY[] = { 3, 2, 2 };
    int i = 0;
    for (auto it = path.begin(); it != path.end(); ++it, ++i) ASSERT_EQ(std::make_pair(expX[i], expY[i]), *it);
    ASSERT_NEAR(sqrt(2) + 1, planner.getFinalCost(), 1e-4);
}

TEST(PathPlannerTest, TestMountain2)
{
    const World& world = World::mountain();
    const double weight = 1;
    AStarPlanner planner(world.width, world.height, world.elevation, world.impassable, weight);

    const std::vector<std::pair<int, int> > path = planner.plan(8, 8, 12, 8);

    // no chance but to go straight up
    int x = 8;
    for (auto it = path.begin(); it != path.end(); ++it, ++x) ASSERT_EQ(std::make_pair(x, 8), *it);
    ASSERT_NEAR(3 * (1 * (weight+1)) + 1 * (2 * (weight+1)), planner.getFinalCost(), 1e-4);
}

TEST(PathPlannerTest, TestMountain3)
{
    const World& world = World::mountain();
    const double weight = 3; // higher weight to force optimal path avoiding mountain
    AStarPlanner planner(world.width, world.height, world.elevation, world.impassable, weight);

    const std::vector<std::pair<int, int> > path = planner.plan(8, 8, 15, 8);

    // avoiding the mountain and taking the north route should be cheapest, since we're so heavy here
    ASSERT_NEAR(7 + 7 * sqrt(2), planner.getFinalCost(), 1e-4);
}

TEST(PathPlannerTest, TestMountain4)
{
    const World& world = World::mountain();
    const double weight = 5;
    AStarPlanner planner(world.width, world.height, world.elevation, world.impassable, weight);

    const std::vector<std::pair<int, int> > path = planner.plan(17, 13, 10, 19);

    // it's cheapest to take the shortest possible way up (i.e. non-diagonal) and the long way down (i.e., diagonal)
    ASSERT_NEAR(2 * sqrt(2) + 2 * (2 * (1 + weight)) + 1 * (1 * (1 + weight)) + 2 + sqrt(2) * (1 / sqrt(1 * (1 + weight))) + sqrt(2) * (1 / sqrt(2 * (1 + weight))), planner.getFinalCost(), 1e-4);
}

TEST(PathPlannerTest, TestDownhill)
{
    const World& world = World::downhill();
    const double weight = 1;
    AStarPlanner planner(world.width, world.height, world.elevation, world.impassable, weight);

    {
        const std::vector<std::pair<int, int> > path = planner.plan(0, 0, 18, 0);
        std::cout << "final cost = " << planner.getFinalCost() << std::endl;
        ASSERT_NEAR(13.6066, planner.getFinalCost(), 1e-4);
    }

    {
        const std::vector<std::pair<int, int> > path = planner.plan(0, 0, 18, 3);
        std::cout << "final cost = " << planner.getFinalCost() << std::endl;
        ASSERT_NEAR(14.4853, planner.getFinalCost(), 1e-4);
    }

    {
        const std::vector<std::pair<int, int> > path = planner.plan(0, 0, 18, 5);
        std::cout << "final cost = " << planner.getFinalCost() << std::endl;
        ASSERT_NEAR(15.1924, planner.getFinalCost(), 1e-4);
    }
}


TEST(PathPlannerTest, TestMoreWalls)
{
    const World& world = World::moreWalls();
    const double weight = 1;
    AStarPlanner planner(world.width, world.height, world.elevation, world.impassable, weight);

    const std::vector<std::pair<int, int> > path = planner.plan(4, 0, 12, 3);
    for (auto p : path) std::cout << "(" << p.first << ", " << p.second << ")" << std::endl;
    std::cout << "final cost = " << planner.getFinalCost() << std::endl;

    EXPECT_NEAR(6 * sqrt(2) + 39, planner.getFinalCost(), 1e-4);
}

