#include <iostream>
using namespace std;


class Player {
private:
    string shirtName; 
    int number;
    Player(const Player&);              // prevents use of copy-constr and copy assign operator
    Player& operator=(const Player&);   // also doesn't let the derived classes to use it

public:
    // More efficient way to declare a constructor: member initialization
    Player(const string& name, const int nr)
    : shirtName(name),      // <-- shirtName is copy_constructed from name
      number(nr)            // build-in types have no difference in cost initialization ~ assignment
    { cout << "Constructing player shirt no " << number << ".\n"; }
    ~Player() { cout << "Deconstructing player shirt no " << number << ".\n"; }

    int getNumber() { return number; }
};


class Goalie : private Player{
private:
    string glovesSize;

public:
    Goalie(const string& name, const int nr, const string& size)
    : Player(name, nr),
      glovesSize(size)
    { cout << "Constructing goalie shirt no " << this->getNumber() << ".\n"; }
    ~Goalie() { cout << "Deconstructing goalie shirt no " << this->getNumber() << ".\n"; }
};


int main()
{
    Player p1("Andrei", 10);
    Goalie g1("Bogdan", 1, "M");

    return 0;
}