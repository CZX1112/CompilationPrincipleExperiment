#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <string>
#include <vector>

class Production
{
public:
    //Production();
    Production()
    {
        left="";
    }

    Production(std::string left, std::vector<std::string>right)
    {
        this->left = left;
        this->right = right;
    }

    std::string getLeft() const;
    std::vector<std::string> getRight() const;
    void setLeft(std::string left);
    void setRight(std::vector<std::string> right);
    void addRight(std::string rightOne);

private:
    std::string left;
    std::vector<std::string> right;
};

#endif // PRODUCTION_H
