#include<iostream>
#include<stdexcept>
using namespace std;

//-------------------------------------------------------------------------MOVE-----------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------------------------------------------//

class Move {
private:
    int source_heap, source_coins, target_heap, target_coins;

public:
    // Take sc coins from heap sh and put tc coins to heap th
    Move(int sh, int sc, int th, int tc) : source_heap(sh), source_coins(sc), target_heap(th), target_coins(tc) {}

    int getSource() const {
        return source_heap;
    }

    int getSourceCoins() const {
        return source_coins;
    }

    int getTarget() const {
        return target_heap;
    }

    int getTargetCoins() const {
        return target_coins;
    }

    friend ostream & operator << (ostream &out, const Move &move) {
        if (move.getTargetCoins() > 0) {
            out << "takes " << move.source_coins << " coins from heap " << move.source_heap << " and puts " << move.target_coins << " to heap " << move.target_heap << endl;
        }
        else {
            out << "takes " << move.source_coins << " coins from heap " << move.source_heap << " and puts nothing" << endl;
        }
        return out;
    }

};

//--------------------------------------------------------STATE--------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------//

class State {
private:
    int heaps_number, *coins_of_heap;

public:
    //State with h heaps, where the i-th heap starts with c[i] coins
    State(int h, const int c[]) : heaps_number(h) {
        coins_of_heap = new int[h];
        for (int i = 0; i < h; i++) {
            coins_of_heap[i] = c[i];
        }
    }

    ~State() {
        delete [] coins_of_heap;
    }

    void next(const Move &move) throw(logic_error) {
        int heap_from = move.getSource();
	    int coins_from = move.getSourceCoins();
        if (heap_from >= heaps_number || heap_from < 0) {
		    throw logic_error("invalid heap");
	    }
        else if (coins_from > coins_of_heap[heap_from] || coins_from < 0) {
		    throw logic_error("invalid coins");
	    }
        else {
            coins_of_heap[heap_from] -= coins_from;
        }

        int heap_to = move.getTarget();
	    int coins_to = move.getTargetCoins();
        if (heap_to >= heaps_number || heap_to < 0){
            throw logic_error("invalid heap");
        }
        else if (coins_to >= coins_from || coins_to < 0) {
            throw logic_error("invalid coins");
        }
        else {
            coins_of_heap[heap_to ] += coins_to;
        }
    }

    bool winning() const {
        bool win =  true;
        for (int i = 0; i < heaps_number; i++) {
            if (coins_of_heap[i] != 0) {
                win = false;
                break;
            }
        }
        return win;
    }

    int getHeaps() const {
        return heaps_number;
    }

    int getCoins(int h) const throw(logic_error) {
        if (h >= heaps_number || h < 0) {
            throw logic_error("invalid heap");
        }
        else {
            return coins_of_heap[h];
        }
    }

    friend ostream & operator << (ostream &out, const State &state) {
        if (state.heaps_number == 1) {
            out << state.coins_of_heap[0] << endl;
        }
        else {
            out << state.coins_of_heap[0];
            for (int i = 1; i < state.heaps_number; i++) {
                out << ", " << state.coins_of_heap[i];
            }
            out << endl;
        }
        return out;
    }

};

//---------------------------------------------------------------------PLAYERS--------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//


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
        out << player.getType() << " player " << player.name;
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
   SneakyPlayer (const string &s): Player(s) {}
	const string &getType() const override{
		return type;
	}
	Move play(const State &s) override{
		int i, minim;
		for (i = 0; i < s.getHeaps(); i++) {
			if (s.getCoins(i) !=0 ) {
				minim = s.getCoins(i);
				break;
			}	
		}
		int heap_pos = i;
		for (int j = heap_pos; j < s.getHeaps(); j++) {
			if ((s.getCoins(j) < minim) && (s.getCoins(j)!=0)) {
				heap_pos = j;
				minim = s.getCoins(j);
			}
		}
		Move move(heap_pos, minim, 0, 0);
		return move;
	}
};

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

//----------------------------------------------------------------------------GAME---------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------//

class Game {
public:
    Game(int heaps, int players) : heaps_number(heaps), players_number(players) {
        game_heap = new int[heaps];
        game_player = new Player*[players];
    }
    ~Game() {
        delete [] game_heap;
        delete [] game_player;
    }

    void addHeap(int coins) throw(logic_error) {
        static int i = 0;
        if (coins < 0) {
            throw logic_error("invalid coins");
        }
        else {
            game_heap[i] = coins;
            i++;
        }
    }
    void addPlayer(Player *player) throw(logic_error) {
        static int i = 0;
        if (i < players_number) {
            game_player[i] = player;
            i++;
        }
        else {
            throw logic_error("invalid players");
        }
    }
    void play(ostream &out) throw(logic_error) {
        if (players_number <= 0) {
            throw logic_error("invalid Players");
        }
        else if (heaps_number <= 0) {
            throw logic_error("Invalid heps");
        }
        else {
            State start(heaps_number, game_heap);
            int i;
            out << "State: " << start << endl;
            while (start.winning() == false) {
                for (i = 0; i < players_number; i++) {
                    if (start.winning() == true) {
                        break;
                    }
                    else{
                        out << *game_player[i] << " " << game_player[i]->play(start) << endl;
                        start.next (game_player[i]->play(start));
                        out << "State: " << start << endl;
                    }
                }
            }
            out << *game_player[i-1] << " wins" << endl;
        }
    }
private:
    int *game_heap, heaps_number, players_number;
    Player **game_player;
};

int main () {
	Game specker(3, 4);
	specker.addHeap(10);
	specker.addHeap(20);
	specker.addHeap(17);
	specker.addPlayer(new SneakyPlayer("Tom"));
	specker.addPlayer(new SpartanPlayer("Mary"));
	specker.addPlayer(new GreedyPlayer("Alan"));
	specker.addPlayer(new RighteousPlayer("Robin"));
	specker.play(cout);
	return 0;
}