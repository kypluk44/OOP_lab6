#include "../include/battle.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <memory>

class CounterObserver : public IFightObserver
{
public:
    void on_fight(const std::shared_ptr<NPC> attacker,
                  const std::shared_ptr<NPC> defender,
                  bool win) override
    {
        if (win)
        {
            ++count;
            last_attacker = attacker;
            last_defender = defender;
        }
    }

    size_t count{0};
    std::shared_ptr<NPC> last_attacker;
    std::shared_ptr<NPC> last_defender;
};

TEST(FightRules, OrkKillsDruid)
{
    auto observer = std::make_shared<CounterObserver>();
    std::vector<std::shared_ptr<IFightObserver>> observers{observer};

    set_t npcs;
    npcs.insert(factory(OrkType, "ork1", 0, 0, observers));
    npcs.insert(factory(DruidType, "druid1", 1, 1, observers));

    auto dead = fight(npcs, 5);
    for (auto &d : dead)
        npcs.erase(d);

    ASSERT_EQ(npcs.size(), 1u);
    EXPECT_EQ(observer->count, 1u);
}

TEST(FightRules, DruidKillsSquirrelOnly)
{
    auto observer = std::make_shared<CounterObserver>();
    std::vector<std::shared_ptr<IFightObserver>> observers{observer};

    set_t npcs;
    npcs.insert(factory(DruidType, "druid2", 0, 0, observers));
    npcs.insert(factory(SquirrelType, "squirrel1", 0, 1, observers));
    npcs.insert(factory(OrkType, "ork2", 100, 100, observers));

    auto dead = fight(npcs, 5);
    for (auto &d : dead)
        npcs.erase(d);

    ASSERT_EQ(npcs.size(), 2u);
    EXPECT_EQ(observer->count, 1u);
}

TEST(Storage, SaveAndLoad)
{
    std::vector<std::shared_ptr<IFightObserver>> observers;
    set_t npcs;
    npcs.insert(factory(OrkType, "ork3", 10, 10, observers));
    npcs.insert(factory(SquirrelType, "squirrel2", 20, 20, observers));

    const std::string filename = "npc_test_data.txt";
    save(npcs, filename);

    auto loaded = load(filename, observers);

    ASSERT_EQ(loaded.size(), 2u);

    std::filesystem::remove(filename);
}

TEST(DistanceBoundary, KillAtEdge)
{
    auto observer = std::make_shared<CounterObserver>();
    std::vector<std::shared_ptr<IFightObserver>> observers{observer};

    set_t npcs;
    npcs.insert(factory(OrkType, "ork_edge", 0, 0, observers));
    npcs.insert(factory(DruidType, "druid_edge", 6, 8, observers)); // расстояние 10

    auto dead = fight(npcs, 10);

    ASSERT_EQ(dead.size(), 1u);
    EXPECT_EQ(observer->count, 1u);
}

TEST(DistanceBoundary, NoKillBelowEdge)
{
    auto observer = std::make_shared<CounterObserver>();
    std::vector<std::shared_ptr<IFightObserver>> observers{observer};

    set_t npcs;
    npcs.insert(factory(OrkType, "ork_close", 0, 0, observers));
    npcs.insert(factory(DruidType, "druid_close", 6, 8, observers)); // расстояние 10

    auto dead = fight(npcs, 9);

    ASSERT_EQ(dead.size(), 0u);
    EXPECT_EQ(observer->count, 0u);
}

TEST(ObserverCountsMultipleKills, DruidVsTwoSquirrels)
{
    auto observer = std::make_shared<CounterObserver>();
    std::vector<std::shared_ptr<IFightObserver>> observers{observer};

    set_t npcs;
    npcs.insert(factory(DruidType, "druid_multi", 100, 100, observers));
    npcs.insert(factory(SquirrelType, "sq1", 102, 100, observers));
    npcs.insert(factory(SquirrelType, "sq2", 103, 101, observers));

    auto dead = fight(npcs, 5);

    ASSERT_EQ(dead.size(), 2u);
    EXPECT_EQ(observer->count, 2u);
}

TEST(ZeroDistanceFight, KillOnSameCell)
{
    auto observer = std::make_shared<CounterObserver>();
    std::vector<std::shared_ptr<IFightObserver>> observers{observer};

    set_t npcs;
    npcs.insert(factory(OrkType, "ork_same", 200, 200, observers));
    npcs.insert(factory(DruidType, "druid_same", 200, 200, observers));

    auto dead = fight(npcs, 0);

    ASSERT_EQ(dead.size(), 1u);
    EXPECT_EQ(observer->count, 1u);
}
