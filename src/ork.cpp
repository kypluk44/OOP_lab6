#include "../include/ork.h"

#include "../include/druid.h"
#include "../include/squirrel.h"

#include <iostream>

Ork::Ork(const std::string &name, int x, int y) : NPC(OrkType, name, x, y) {}
Ork::Ork(std::istream &is) : NPC(OrkType, is) {}

bool Ork::is_ork() const
{
    return true;
}

bool Ork::accept(const std::shared_ptr<NPC> &attacker)
{
    return attacker->fight(std::dynamic_pointer_cast<Ork>(shared_from_this()));
}

bool Ork::fight(const std::shared_ptr<Ork> &other)
{
    fight_notify(other, false);
    return false;
}

bool Ork::fight(const std::shared_ptr<Squirrel> &other)
{
    fight_notify(other, false);
    return false;
}

bool Ork::fight(const std::shared_ptr<Druid> &other)
{
    fight_notify(other, true);
    return true;
}

void Ork::print() const
{
    std::cout << "Ork " << name << " (" << x << ", " << y << ")" << std::endl;
}

void Ork::save(std::ostream &os) const
{
    NPC::save(os);
}
