#include "../include/druid.h"

#include "../include/ork.h"
#include "../include/squirrel.h"

#include <iostream>

Druid::Druid(const std::string &name, int x, int y) : NPC(DruidType, name, x, y) {}
Druid::Druid(std::istream &is) : NPC(DruidType, is) {}

bool Druid::accept(const std::shared_ptr<NPC> &attacker)
{
    return attacker->fight(std::dynamic_pointer_cast<Druid>(shared_from_this()));
}

bool Druid::fight(const std::shared_ptr<Ork> &other)
{
    fight_notify(other, false);
    return false;
}

bool Druid::fight(const std::shared_ptr<Squirrel> &other)
{
    fight_notify(other, true);
    return true;
}

bool Druid::fight(const std::shared_ptr<Druid> &other)
{
    fight_notify(other, false);
    return false;
}

void Druid::print() const
{
    std::cout << "Druid " << name << " (" << x << ", " << y << ")" << std::endl;
}

void Druid::save(std::ostream &os) const
{
    NPC::save(os);
}
