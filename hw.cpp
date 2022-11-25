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



class Puck {
private:
    int radius, height;
    string colorName;
    void init(const Puck& pck);     // used for copy assign operator

protected:
    string getColorName() { return colorName; }

public:
    Puck(const int r, const int h, const string& color)
    : radius(r),
      height(h),
      colorName(color)
    { cout << "Constructing puck " << getColorName() << ".\n"; }
    ~Puck() { cout << "Deconstructing puck " << getColorName() << ".\n"; }
    Puck(const Puck& pck);
    Puck& operator=(const Puck& pck);
};

class GiftedPuck : private Puck {
private:
    bool isGifted;

public:
    GiftedPuck(const int r, const int h, const string& color, const bool gifted)
    : Puck(r, h, color),
      isGifted(gifted)
    { cout << "Constructing gifted puck " << getColorName() << ".\n"; }
    ~GiftedPuck() { cout << "Deconstructing gifted puck " << getColorName() << ".\n"; }
    GiftedPuck(const GiftedPuck& pck);
    GiftedPuck& operator=(const GiftedPuck& pck);
};


void Puck::init(const Puck& pck)
{
    /*
        This func was created to prevent code duplication for initialization of copy assign oper and copy constr
        Because we use the member initialization method on the copy constr there will be no code duplication
        This remain just a "nice to have" func for the sake of practising
    */
    radius = pck.radius;
    height = pck.height;
    colorName = pck.colorName;
}

Puck& Puck::operator=(const Puck& pck)
{
    if (this == &pck) {
        cout << "Puck " << getColorName() << " self-assignment reached.\n"; // comment not needed in real implementation
        return *this;
    }

    init(pck);
    return *this;
}

Puck::Puck(const Puck& pck)
: radius(pck.radius),
  height(pck.height),
  colorName(pck.colorName)
{ cout << "Puck " << getColorName() << " copy constructor.\n"; }


GiftedPuck& GiftedPuck::operator=(const GiftedPuck& pck)
{
    if (this == &pck) {
        cout << "GiftedPuck " << getColorName() << " self-assignment reached.\n";
        return *this;
    }

    Puck::operator=(pck);   // assigning base class part of operator=
    isGifted = pck.isGifted;
    return *this;
}

GiftedPuck::GiftedPuck(const GiftedPuck& pck)
: Puck(pck),
  isGifted(pck.isGifted)    // invoke the base class copy constructor
{ cout << "GiftedPuck " << getColorName() << " copy constructor.\n"; }


int main()
{
    Player pl("Andrei", 10);
    Goalie gl("Bogdan", 1, "M");

    Puck p1(5, 3, "black");
    Puck p2(p1);
    GiftedPuck gf1(10, 5, "pink", false);
    GiftedPuck gf2 = gf1;

    p2 = p2;
    gf2 = gf2;

    return 0;
}