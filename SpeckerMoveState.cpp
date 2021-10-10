#include<iostream>
#include<stdexcept>
using namespace std;

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
        else if (coins_to >= coins_from || coins_to < 0)){
            throw logic_error("invalid coins");
        }
        else{
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