#include "../include/observers.h"

#include <iostream>

void ConsoleObserver::on_fight(const std::shared_ptr<NPC> attacker,
                               const std::shared_ptr<NPC> defender,
                               bool win)
{
    if (win && attacker && defender)
    {
        std::cout << std::endl
                  << "Murder --------" << std::endl;
        attacker->print();
        defender->print();
    }
}

FileObserver::FileObserver(const std::string &path) : out(path, std::ios::trunc) {}

void FileObserver::on_fight(const std::shared_ptr<NPC> attacker,
                            const std::shared_ptr<NPC> defender,
                            bool win)
{
    if (win && out && attacker && defender)
    {
        out << "Kill: " << *attacker << " -> " << *defender << '\n';
        out.flush();
    }
}
