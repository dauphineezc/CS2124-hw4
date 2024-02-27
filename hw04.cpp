// Zoe Dauphinee, homework 4, Classes; Pointers; Association


#include <iostream>
#include <vector>
#include <string>
using namespace std;


// Warrior class
class Warrior {
    friend ostream& operator<<(ostream& os, const Warrior& warrior);
    public:
        Warrior(const string& name, double strength) 
            : name(name), strength(strength), employed(false) {}
        const string& get_name() const { return name; }
        const double get_strength() const { return strength; }
        const bool is_employed() const { return employed; }
        void change_employment_status(bool status) { employed = status; }
        void change_strength(double new_strength) { strength = new_strength; }
    private:
        string name;
        double strength;
        bool employed;
};

// Noble class
class Noble {
    friend ostream& operator<<(ostream& os, const Noble& noble);
    public:
        Noble(const string& name) 
            : name(name), dead(false), army(0) {}
        bool hire(Warrior& warrior);
        bool fire(Warrior& warrior);
        const double army_strength();
        void battle(Noble& opponent);
        void win(Noble& opponent, double my_strength, double opp_strength);
        void lose(Noble& opponent, double my_strength, double opp_strength);
        void tie(Noble& opponent);
    private:
        string name;
        bool dead;
        vector<Warrior*> army;
};



int main() {
	
    Noble art("King Arthur");
    Noble lance("Lancelot du Lac");
    Noble jim("Jim");
    Noble linus("Linus Torvalds");
    Noble billie("Bill Gates");
	
    Warrior cheetah("Tarzan", 10);
    Warrior wizard("Merlin", 15);
    Warrior theGovernator("Conan", 12);
    Warrior nimoy("Spock", 15);
    Warrior lawless("Xena", 20);
    Warrior mrGreen("Hulk", 8);
    Warrior dylan("Hercules", 3);
	
    jim.hire(nimoy);
    lance.hire(theGovernator);
    art.hire(wizard);
    lance.hire(dylan);
    linus.hire(lawless);
    billie.hire(mrGreen);
    art.hire(cheetah);
    art.hire(nimoy);
	
    cout << "==========\n\n"
         << "Status before all battles, etc.\n";
    cout << jim << endl;
    cout << lance << endl;
    cout << art << endl;
    cout << linus << endl;
    cout << billie << endl;
    cout << "==========\n\n";
	
    art.fire(cheetah);
    cout << art << endl;

    lance.fire(nimoy);
    cout << lance << endl;
    cout << "==========\n\n";

    art.battle(lance);
    jim.battle(lance);
    linus.battle(billie);
    billie.battle(lance);

    cout << "==========\n\n"
         << "Status after all battles, etc.\n";
    cout << jim << endl;
    cout << lance << endl;
    cout << art << endl;
    cout << linus << endl;
    cout << billie << endl;
    cout << "==========\n";
	
}



/*
Warrior method implementation
*/

// Warrior output operator
ostream& operator<<(ostream& os, const Warrior& warrior) {
    os << warrior.name << ": " << warrior.strength;
    return os;
}



/*
Noble method implementations
*/

// Noble output operator
ostream& operator<<(ostream& os, const Noble& noble) {
    os << noble.name << " has an army of " << noble.army.size() << endl;
    for (size_t index = 0; index < noble.army.size(); ++index) {
        os << "    " << *noble.army[index] << endl;
    }
    return os;
}

// hire method
bool Noble::hire(Warrior& warrior) {
    // if the Noble is not dead, continue
    if (!dead) {
        // if the warrior is already hired by another Noble, fail
        if (warrior.is_employed()) {
            cout << name << " failed to hire " << warrior.get_name() << endl;
            return false;
        }
        // conditions are good to hire so hire
        else {
            army.push_back(&warrior);
            warrior.change_employment_status(true);
            return true;
        }
    }
    // Noble is dead, cannot hire, return false
    return false;
}

// fire method
bool Noble::fire(Warrior& warrior) {
    // if the Noble is not dead, continue
    if (!dead) {
        // loop through army to find warrior to be fired
        for (size_t index = 0; index < army.size(); ++index) {
            // if found: print statement, remove from army, and return true
            if (army[index] == &warrior) {
                cout << warrior.get_name() 
                    << ", you don't work for me any more! -- ";
                    cout << name << "." << endl;
                // change warrior employment status
                warrior.change_employment_status(false);
                // remove from army vector (maintaining order)
                for (size_t index2 = index; index2 < army.size(); ++index2) {
                    army[index2] = army[index2 + 1];
                }
                army.pop_back();
                return true;
            }
        }
        // got through for loop without finding --> return false
        cout << name << " failed to fire " << warrior.get_name() << endl;
        return false;
    }
    // Noble is dead, cannot fire, return false
    return false;
}

// calculates strength of the entire army
const double Noble::army_strength() {
    double strength = 0;
    for (size_t index = 0; index < army.size(); ++index) {
        strength += army[index]->get_strength();
    }
    return strength;
}

// battle with other Noble's army
void Noble::battle(Noble& opponent) {
    cout << endl << name << " battles " << opponent.name << endl;
    double my_strength = army_strength();
    double opp_strength = opponent.army_strength();

    // both dead -> print statement and return
    if (dead && opponent.dead) {
        cout << "Oh, NO!  They're both dead!  Yuck!" << endl;
        return;
    }
    // Noble is dead -> print statement and return
    else if (dead) {
        cout << "He's dead, " << opponent.name << endl;
        return;
    }
    // opponent is dead -> print statement and return
    else if (opponent.dead) {
        cout << "He's dead, " << name << endl;
        return;
    }

    // Noble wins
    if (my_strength > opp_strength) { 
        win(opponent, my_strength, opp_strength);
        return;
    }
    // opponent wins
    else if (opp_strength > my_strength) {
        lose(opponent, my_strength, opp_strength);
        return;
    }
    // tie
    else if (my_strength == opp_strength) {
        tie(opponent);
        return;
    }
}

// changes strengths for Noble and opponent if Noble wins
void Noble::win(Noble& opponent, double my_strength, double opp_strength) {
    // calculate strength ratio of the two armies
    double reduce_by = (opp_strength/my_strength);
    // go through winning army and reduce each warrior's strength
    for (size_t index = 0; index < army.size(); ++index) {
        double new_strength = army[index]->get_strength() * (1-reduce_by);
        army[index]->change_strength(new_strength);
    }
    // go through losing army and set each warrior's strength to 0
    for (size_t index = 0; index < opponent.army.size(); ++index) {
        opponent.army[index]->change_strength(0);
    }
    // set losing Noble dead boolean to true
    opponent.dead = true;
    // print result
    cout << name << " defeats " << opponent.name << endl;
}

// changes strengths for Noble and opponent if opponent wins
void Noble::lose(Noble& opponent, double my_strength, double opp_strength) {
    // calculate strength ratio of the two armies
    double reduce_by = (my_strength/opp_strength);
    // go through winning army and reduce each warrior's strength
    for (size_t index = 0; index < opponent.army.size(); ++index) {
        double new_strength = opponent.army[index]->get_strength() 
            * (1-reduce_by);
        opponent.army[index]->change_strength(new_strength);
    }
    // go through losing army and set each warrior's strength to 0
    for (size_t index = 0; index < army.size(); ++index) {
        army[index]->change_strength(0);
    }
    // set losing Noble dead boolean to true
    dead = true;
    // print result
    cout << opponent.name << " defeats " << name << endl;
}

// changes strengths for Noble and opponent if they tie
void Noble::tie(Noble& opponent) {
    // go through army and set each warrior's strength to 0
    for (size_t index = 0; index < army.size(); ++index) {
        army[index]->change_strength(0);
    }
    // go through opponent's army and set each warrior's strength to 0
    for (size_t index = 0; index < opponent.army.size(); ++index) {
        opponent.army[index]->change_strength(0);
    }
    // set dead boolean to true for both Nobles
    dead = true;
    opponent.dead = true;
    // print result
    cout << "Mutual Annihilation: ";
    cout << name << " and " << opponent.name << " die at each other's hands\n";
}