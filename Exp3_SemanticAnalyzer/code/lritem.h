#ifndef LRITEM_H
#define LRITEM_H

#include <vector>
#include <string>
#include <iostream>

class LR0item
{
private:
    std::pair<unsigned int, unsigned int> item;
public:
    LR0item()
    {
        item = std::make_pair(0,0);
    }
    LR0item(unsigned int l, unsigned int r)
    {
        item = std::make_pair(l,r);
    }
    LR0item(std::pair<unsigned int, unsigned int> item)
    {
        this->item = item;
    }

    std::pair<unsigned int, unsigned int> getItem() const;
    unsigned int getLeft() const;
    unsigned int getRight() const;
    bool operator==(const LR0item &item) const;
    bool operator<(const LR0item &item) const;
};

class LR1item
{
private:
    std::pair<LR0item, std::string> item;
public:
    LR1item()
    {
        item = std::make_pair(LR0item(), "$");
    }
    LR1item(const LR0item &first, const std::string &second)
    {
        item = std::make_pair(first, second);
    }
    LR1item(std::pair<LR0item, std::string> item)
    {
        this->item = item;
    }

    std::pair<LR0item, std::string> getItem() const;
    LR0item getLeft() const;
    std::string getRight() const;
    bool operator==(const LR1item &item) const;
    bool operator<(const LR1item &item) const;
};

#endif // LRITEM_H
