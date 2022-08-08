#include "production.h"

std::string Production::getLeft() const
{
    return left;
}

std::vector<std::string> Production::getRight() const
{
    return right;
}

void Production::setLeft(std::string left)
{
    this->left = left;
}

void Production::setRight(std::vector<std::string> right)
{
    this->right = right;
}

void Production::addRight(std::string rightOne)
{
    right.push_back(rightOne);
}
