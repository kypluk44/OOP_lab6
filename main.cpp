#include "../include/battle.h"
#include "../include/observers.h"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>

namespace
{
    void print_menu()
    {
        std::cout << "\nCommands:\n"
                  << "1 - Add NPC\n"
                  << "2 - Show NPCs\n"
                  << "3 - Save to file\n"
                  << "4 - Load from file\n"
                  << "5 - Battle (set distance)\n"
                  << "0 - Exit\n"
                  << "Choice: ";
    }

    NpcType read_type(int value)
    {
        switch (value)
        {
        case 1:
            return OrkType;
        case 2:
            return SquirrelType;
        case 3:
            return DruidType;
        default:
            return Unknown;
        }
    }

    bool in_bounds(int x, int y)
    {
        constexpr int min_coord = 0;
        constexpr int max_coord = 500;
        return x >= min_coord && x <= max_coord && y >= min_coord && y <= max_coord;
    }

    void add_npc(set_t &npcs, const std::vector<std::shared_ptr<IFightObserver>> &observers)
    {
        int type_input = 0;
        std::cout << "Choose type (1 - Ork, 2 - Squirrel, 3 - Druid): ";
        if (!(std::cin >> type_input))
            return;
        const auto type = read_type(type_input);
        if (type == Unknown)
        {
            std::cout << "Unknown type\n";
            return;
        }

        std::string name;
        int x = 0;
        int y = 0;
        std::cout << "Name: ";
        std::cin >> name;
        if (name_exists(npcs, name))
        {
            std::cout << "Name must be unique\n";
            return;
        }
        std::cout << "Coordinates (x y): ";
        std::cin >> x >> y;
        if (!in_bounds(x, y))
        {
            std::cout << "Coordinates must be between 0 and 500\n";
            return;
        }

        auto npc = factory(type, name, x, y, observers);
        if (npc)
            npcs.insert(npc);
    }

    void do_save(const set_t &npcs)
    {
        std::string filename = "npc.txt";
        std::cout << "Filename (default npc.txt): ";
        std::cin >> filename;
        save(npcs, filename);
    }

    void do_load(set_t &npcs, const std::vector<std::shared_ptr<IFightObserver>> &observers)
    {
        std::string filename = "npc.txt";
        std::cout << "Filename (default npc.txt): ";
        std::cin >> filename;
        auto loaded = load(filename, observers);
        for (auto &npc : loaded)
        {
            if (!npc)
                continue;
            if (name_exists(npcs, npc->get_name()))
            {
                std::cout << "Skip duplicate name: " << npc->get_name() << '\n';
                continue;
            }
            npcs.insert(npc);
        }
    }

    void do_fight(set_t &npcs)
    {
        size_t distance = 0;
        std::cout << "Distance: ";
        std::cin >> distance;
        auto dead = fight(npcs, distance);
        for (auto &d : dead)
            npcs.erase(d);
        std::cout << "Killed: " << dead.size() << "\n";
    }
}

int main()
{
    auto console_observer = std::make_shared<ConsoleObserver>();
    auto file_observer = std::make_shared<FileObserver>("log.txt");

    std::vector<std::shared_ptr<IFightObserver>> observers{console_observer, file_observer};

    set_t npcs;

    bool running = true;
    while (running)
    {
        print_menu();
        int choice = -1;
        if (!(std::cin >> choice))
            break;

        switch (choice)
        {
        case 1:
            add_npc(npcs, observers);
            break;
        case 2:
            print_all(npcs, std::cout);
            break;
        case 3:
            do_save(npcs);
            break;
        case 4:
            do_load(npcs, observers);
            break;
        case 5:
            do_fight(npcs);
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << "Unknown command\n";
            break;
        }

        if (!std::cin.good())
            break;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return 0;
}
