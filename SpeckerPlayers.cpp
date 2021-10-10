#include<iostream>
using namespace std;

class Player {
protected:
    string name;

public:
    Player(const string &n) : name(n) {} 
    virtual ~Player() {
        name = '/0';
    }

    virtual const string & getType() const = 0;
    virtual Move play(const State &s) = 0;

    friend ostream & operator << (ostream &out, const Player &player) {
        out << player.getType() << "player" << player.name << endl;
        return out;
    }
};

class GreedyPlayer : public Player {
private:
    string type = "Greedy";
public:
    GreedyPlayer(const string &s) : Player(s) {}
    const string &getType() const override {
        return type;
    }

    Move play(const State &s) override {
        int maxim = s.getCoins(0), heap_pos = 0;
        for (int i = 1; i < s.getHeaps(); i++) {
            if (s.getCoins(i) > maxim) {
                maxim = s.getCoins(i);
                heap_pos = i;
            }
        }
        Move move(heap_pos, maxim, 0, 0);
        return move;
    }
};

class SpartanPlayer : public Player {
private:
    string type = "Spartan";
public:
    SpartanPlayer(const string &s) : Player(s) {}
    const string &getType() const override {
        return type;
    }

    Move play(const State &s) override {
        int maxim = s.getCoins(0), heap_pos = 0;
        for (int i = 1; i < s.getHeaps(); i++) {
            if (s.getCoins(i) > maxim) {
                maxim = s.getCoins(i);
                heap_pos = i;
            }
        }
        Move move(heap_pos, 1, 0, 0);
        return move;
    }
};

class SneakyPlayer : public Player {
private:
    string type = "Sneaky";
public:
    SneakyPlayer(const string &s) : Player(s) {}
    const string &getType() const override {
        return type;
    }

    Move play(const State &s) override {
        int minim = s.getCoins(0), heap_pos = 0;
        for (int i = 1; i < s.getHeaps(); i++) {
            if (s.getCoins < minim) {
                minim = s.getCoins(i);
                heap_pos = i;
            }
        }
        Move move(heap_pos, minim, 0, 0);
        return move;
    }
}

class RighteousPlayer : public Player {
private:
    string type = "Righteous";
public:
    RighteousPlayer(const string &s) : Player(s) {}
    const string &getType() const override {
        return type;
    }

    Move play(const State &s) override {
        int maxim = s.getCoins(0), heap_pos=0;
		for (int i = 1; i < s.getHeaps(); i++) {
			if (s.getCoins(i) > maxim) {
				heap_pos = i;
				maxim = s.getCoins(i);
			}
		}
		
		int j, minim = s.getCoins(0), heap_pos2 = 0;
		for (j = 1; j < s.getHeaps(); j++) {
			if (s.getCoins(j) < minim) {
				heap_pos2 = j;
				minim = s.getCoins(j);
			}
		}
		if (maxim %2 == 0) {
			Move move(heap_pos, maxim/2, heap_pos2, maxim/2-1);
			return move;
		}
		else {		
			Move move(heap_pos, maxim/2+1, heap_pos2, maxim/2);
			return move;
		}
	}
};