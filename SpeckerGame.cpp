#include<iostream>
#include<stdexcept>
using namespace std;

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