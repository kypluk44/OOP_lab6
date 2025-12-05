#include "../include/npc.h"

#include <utility>

NPC::NPC(NpcType t, std::string nm, int x_pos, int y_pos)
    : name(std::move(nm)), type(t), x(x_pos), y(y_pos)
{
}

NPC::NPC(NpcType t, std::istream &is) : type(t)
{
    is >> name;
    is >> x;
    is >> y;
}

void NPC::subscribe(const std::shared_ptr<IFightObserver> &observer)
{
    observers.push_back(observer);
}

void NPC::fight_notify(const std::shared_ptr<NPC> &defender, bool win)
{
    for (auto &o : observers)
        o->on_fight(shared_from_this(), defender, win);
}

bool NPC::is_close(const std::shared_ptr<NPC> &other, size_t distance) const
{
    return std::pow(x - other->x, 2) + std::pow(y - other->y, 2) <= std::pow(distance, 2);
}

bool NPC::is_ork() const
{
    return false;
}

bool NPC::is_squirrel() const
{
    return false;
}

bool NPC::is_druid() const
{
    return false;
}

void NPC::save(std::ostream &os) const
{
    os << static_cast<int>(type) << ' ' << name << ' ' << x << ' ' << y << '\n';
}

namespace
{
    std::string type_to_string(NpcType t)
    {
        switch (t)
        {
        case OrkType:
            return "Ork";
        case SquirrelType:
            return "Squirrel";
        case DruidType:
            return "Druid";
        default:
            return "Unknown";
        }
    }
}

std::ostream &operator<<(std::ostream &os, NPC &npc)
{
    os << type_to_string(npc.type) << ' ' << npc.name << " {" << npc.x << ", " << npc.y << "}";
    return os;
}
