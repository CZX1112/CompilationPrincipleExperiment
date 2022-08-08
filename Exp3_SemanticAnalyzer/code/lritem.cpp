#include "lritem.h"

std::pair<unsigned int, unsigned int> LR0item::getItem() const
{
    return item;
}

unsigned int LR0item::getLeft() const
{
    return item.first;
}

unsigned int LR0item::getRight() const
{
    return item.second;
}

bool LR0item::operator==(const LR0item& item) const
{
    return this->getItem() == item.getItem();
}

bool LR0item::operator<(const LR0item& item) const
{
    return this->getItem() < item.getItem();
}



std::pair<LR0item, std::string> LR1item::getItem() const
{
    return item;
}

LR0item LR1item::getLeft() const
{
    return item.first;
}

std::string LR1item::getRight() const
{
    return item.second;
}

bool LR1item::operator==(const LR1item& item) const
{
    return this->getItem() == item.getItem();
}


bool LR1item::operator<(const LR1item& item) const
{
    return this->getItem() < item.getItem();
}
