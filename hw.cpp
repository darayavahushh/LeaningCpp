#define _HAS_AUTO_PTR_ETC 1  // https://devblogs.microsoft.com/cppblog/stl-fixes-in-vs-2015-update-3/
#include <memory>   // also needed for auto_ptr
#include <iostream>

using namespace std;


class Uncopyable {
    protected:
        Uncopyable() {}
        virtual ~Uncopyable() {}
    private:
        Uncopyable(const Uncopyable&);
        Uncopyable& operator = (const Uncopyable&);
}; 


class Player: private Uncopyable {
private:
    string shirtName; 
    int number;
    /* the below is done by class Uncopyable */
    //Player(const Player&);              // prevents use of copy-constr and copy assign operator
    //Player& operator=(const Player&);   // also doesn't let the derived classes to use it

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



class Puck : private Uncopyable {
private:
    int radius, height;
    string colorName;
    void init(const Puck& pck);     // used for copy assign operator

public:
    Puck(const int r, const int h, const string& color)
    : radius(r),
      height(h),
      colorName(color)
    { cout << "Constructing puck " << getColorName() << ".\n"; }
    ~Puck() { cout << "Deconstructing puck " << getColorName() << ".\n"; }
    Puck(const Puck& pck);
    Puck& operator=(const Puck& pck);

    string getColorName() { return colorName; }
    void setColorName(const string& color) { this->colorName = color;}
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
    
    bool getIsGifted() { return this->isGifted; }
    void setIsGifted(const bool b) { this->isGifted = b; }
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


// HW3
// factory function
Puck* createPuck(const int r, const int h, const string& color)
{
    return new Puck(r, h, color);   // return ptr to dynamically allocated object in the Puck hierarchy
}

void gift(GiftedPuck& gp) {
    cout << "Give GiftedPuck away." << endl;
    gp.setIsGifted(true);
};

void receive(GiftedPuck& gp) {
    cout << "Receive GiftedPuck back." << endl;
    gp.setIsGifted(false);
};

// create Gift class (same as Lock but for my example)
class Gift : private Uncopyable {   // prohibit copying in the Gift class: this RAII obj should not be copied
private:
    GiftedPuck& giftPtr;

public:
    Gift(GiftedPuck& ptr): giftPtr(ptr) {
        gift(giftPtr);
    }
    ~Gift() {
        receive(giftPtr);
    }
};



int main()
{
    // HW1
    cout << "\n~ HW1 ~\n";
    Player pl("Andrei", 10);
    Goalie gl("Bogdan", 1, "M");


    // HW2
    cout << "\n~~ HW2 ~~\n";
    Puck p1(5, 3, "black");
    Puck p2(p1);
    GiftedPuck gf1(10, 5, "pink", false);
    GiftedPuck gf2 = gf1;
    GiftedPuck gf3(20, 10, "red", false);
    GiftedPuck gf4(20, 10, "red", true);

    cout << "---------------\n";
    cout << "GiftedPuck no.1 is " << gf1.getIsGifted() << ".\n";
    gf1 = gf3 = gf4;
    cout << "GiftedPuck no.1 is " << gf1.getIsGifted() << ".\n";
    cout << "---------------\n";

    p2 = p2;
    gf2 = gf2;
    cout << "---------------\n";


    // HW3
    cout << "\n~~~ HW3 ~~~\n";
    auto_ptr<Puck> autoPk1(createPuck(1, 1, "yellow_"));
    cout << "Puck " << autoPk1->getColorName() << ".\n";
    auto_ptr<Puck> autoPk2(autoPk1);
    cout << "Puck "; //<< autoPk1->getColorName() << ".\n";    // now autoPk1 is null!!!

    cout << "\n---------------\n";
    shared_ptr<Puck> sharedPk1(createPuck(1, 1, "pink_"));
    cout << "Shared count: " << sharedPk1.use_count() << endl;  // shows the instance number of sharedPk1
    shared_ptr<Puck> sharedPk2(sharedPk1);
    cout << "Shared count: " << sharedPk1.use_count() << endl;
    sharedPk2->setColorName("neon_pink_");
    cout << "Puck " << sharedPk1->getColorName() << ".\n";  // because sharedPk1 and sharedPk2 point to the same instance the color will change for both

    cout << "---------------\n";
    GiftedPuck gf5(27, 3, "blue", false);
    cout << "GiftedPuck no.5 is " << gf5.getIsGifted() << ".\n";
    Gift* giftPuck = new Gift(gf5);
    cout << "GiftedPuck no.5 is " << gf5.getIsGifted() << ".\n";
    delete giftPuck;
    cout << "GiftedPuck no.5 is " << gf5.getIsGifted() << ".\n";
    cout << endl;

    return 0;
}