// Proper Blackjack

/* FIXES
    - Ds
*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <algorithm>

#define iterations 500000
#define decks 4

#define dealerHigh 17
#define dealerHard false
#define high 21

int deal(std::vector<int> deck, std::vector<int> playerCards, std::vector<int> dealerCards) {
    int random;
    std::vector<int> playerSplitCards(0, 0);
    int playerSum = 0;
    int playerSplitSum = 0;
    int dealerSum = 0;
    // Hard = 1; Soft = -1; DD = 2;
    int playerSumHard = 1;
    int playerSplitSumHard = 1;
    int dealerSumHard = 1;

    

    /* policy
                        DEALER VISIBLE
                2   3   4   5   6   7   8   9  10   A

        ~8      H   H   H   H   H   H   H   H   H   H
         9      H   D   D   D   D   H   H   H   H   H
    H   10      D   D   D   D   D   D   D   D   H   H
        11      D   D   D   D   D   D   D   D   D   D
    A   12      H   H   S   S   S   H   H   H   H   H
        13      S   S   S   S   S   H   H   H   H   H
    R   14      S   S   S   S   S   H   H   H   H   H
        15      S   S   S   S   S   H   H   H   H   H
    D   16      S   S   S   S   S   H   H   H   H   H
        17      S   S   S   S   S   S   S   S   S   S
        18~     S   S   S   S   S   S   S   S   S   S

        13      H   H   H   D   D   H   H   H   H   H
    S   14      H   H   H   D   D   H   H   H   H   H
        15      H   H   D   D   D   H   H   H   H   H
    O   16      H   H   D   D   D   H   H   H   H   H
        17      H   D   D   D   D   H   H   H   H   H
    F   18      Ds  Ds  Ds  Ds  Ds  S   S   H   H   H
        19      S   S   S   S   Ds  S   S   S   S   S
    T   20~     S   S   S   S   S   S   S   S   S   S

         2      H   H   P   P   P   P   H   H   H   H
    S    3      H   H   P   P   P   P   H   H   H   H
         4      -------------------------------------
    P    5      -------------------------------------
         6      H   P   P   P   P   H   H   H   H   H
    L    7      P   P   P   P   P   P   H   H   H   H
         8      P   P   P   P   P   P   P   P   P   P
    I    9      P   P   P   P   P   S   P   P   S   S
        10      -------------------------------------
    T    A      P   P   P   P   P   P   P   P   P   P

    S = Stay; H = Hit; D = DD; P = Split

    HARD = POSITIVE; SOFT = NEGATIVE
    */
    std::vector<int> stay;
    std::vector<int> hit;
    std::vector<int> dd;
    std::vector<int> split;
    switch(dealerCards[0]) {
        case 2:
            stay = {13, 14, 15, 16, 17, 18, 19, 20, 21, -19, -20, -21};
            hit = {4, 5, 6, 7, 8, 9, 12, -12, -13, -14, -15, -16, -17};
            dd = {10, 11, -18};
            split = {7, 8, 9, 11};
            break;
        case 3:
            stay = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, -19, -20, -21};
            hit = {4, 5, 6, 7, 8, 12, -12, -13, -14, -15, -16};
            dd = {9, 10, 11, -17, -18};
            split = {6, 7, 8, 9, 11};
            break;
        case 4:
            stay = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, -19, -20, -21};
            hit = {4, 5, 6, 7, 8, -12, -13, -14};
            dd = {9, 10, 11, -15, -16, -17, -18};
            split = {2, 3, 6, 7, 8, 9, 11};
            break;
        case 5:
            stay = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, -19, -20, -21};
            hit = {4, 5, 6, 7, 8, -12};
            dd = {9, 10, 11, -13, -14, -15, -16, -17, -18};
            split = {2, 3, 6, 7, 8, 9, 11};
            break;
        case 6:
            stay = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, -20, -21};
            hit = {4, 5, 6, 7, 8, -12};
            dd = {9, 10, 11, -13, -14, -15, -16, -17, -18, -19};
            split = {2, 3, 6, 7, 8, 9, 11};
            break;
        case 7:
            stay = {17, 18, 19, 20, 21, -18, -19, -20, -21};
            hit = {4, 5, 6, 7, 8, 9, 12, 13, 14, 15, 16, -12, -13, -14, -15, -16, -17};
            dd = {10, 11};
            split = {2, 3, 7, 8, 11};
            break;
        case 8:
            stay = {17, 18, 19, 20, 21, -18, -19, -20, -21};
            hit = {4, 5, 6, 7, 8, 9, 12, 13, 14, 15, 16, -12, -13, -14, -15, -16, -17};
            dd = {10, 11};
            split = {8, 9, 11};
            break;
        case 9:
            stay = {17, 18, 19, 20, 21, -19, -20, -21};
            hit = {4, 5, 6, 7, 8, 9, 12, 13, 14, 15, 16, -12, -13, -14, -15, -16, -17, -18};
            dd = {10, 11};
            split = {8, 9, 11};
            break;
        case 10:
            stay = {17, 18, 19, 20, 21, -19, -20, -21};
            hit = {4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, -12, -13, -14, -15, -16, -17, -18};
            dd = {11};
            split = {8, 11};
            break;
        case 11:
            stay = {17, 18, 19, 20, 21, -19, -20, -21};
            hit = {4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, -12, -13, -14, -15, -16, -17, -18};
            dd = {11};
            split = {8, 11};
            break;
    }
  
    if (playerCards[0] == playerCards[1] && std::find(split.begin(), split.end(), playerCards[0]) != split.end()) {
        random = rand()%deck.size();
        playerCards[1] = deck[random];
        if (playerSum > high) {
            playerSum -= 10;
        }

        playerSplitCards.push_back(playerCards[0]);
        
        random = rand()%deck.size();
        playerSplitCards.push_back(deck[random]);
        
        playerSplitSum += playerSplitCards[0];
        playerSplitSum += playerSplitCards[1];
        if (playerSplitSum > high) {
            playerSplitSum -= 10;
        }
    }

    dealerSum += dealerCards[0];
    dealerSum += dealerCards[1];
    if (dealerSum > high) {
        dealerSum -= 10;
    }
    playerSum += playerCards[0];
    playerSum += playerCards[1];
    if (playerSum > high) {
        playerSum -= 10;
    }
    
    // confirm hardness
    if (std::find(dealerCards.begin(), dealerCards.end(), 11) != dealerCards.end()) {
        dealerSumHard = -1;
    }
    if (std::find(playerCards.begin(), playerCards.end(), 11) != playerCards.end()) {
        playerSumHard = -1;
    }
    if (std::find(playerSplitCards.begin(), playerSplitCards.end(), 11) != playerSplitCards.end()) {
        playerSplitSumHard = -1;
    }

    // add cards while sum matches HIT or DD conditions for player
    int counter = 0;
    while (std::find(hit.begin(), hit.end(), playerSum*playerSumHard) != hit.end() || std::find(dd.begin(), dd.end(), playerSum*playerSumHard) != dd.end()) {
        if (std::find(dd.begin(), dd.end(), playerSum*playerSumHard) != dd.end() && counter == 0) {
            random = rand()%deck.size();
            playerCards.push_back(deck[random]);
            playerSum += deck[random];
            if (deck[random] == 11) {
                if (playerSum <= high) {
                    playerSumHard = -1;
                } else {
                    playerSum -= 10;
                }
            }
            if (playerSum > high && playerSumHard == -1) {
                playerSum -= 10;
                playerSumHard = 1;
            }
            playerSumHard = 2;
            break;
        }
        random = rand()%deck.size();
        playerCards.push_back(deck[random]);
        playerSum += deck[random];
        if (deck[random] == 11) {
            if (playerSum <= high) {
                playerSumHard = -1;
            } else {
                playerSum -= 10;
            }
        }
        if (playerSum > high && playerSumHard == -1) {
            playerSum -= 10;
            playerSumHard = 1;
        }
        counter += 1;
    }
    // add cards while sum matches HIT or DD conditions for playerSplit
    counter = 0;
    while (std::find(hit.begin(), hit.end(), playerSplitSum*playerSplitSumHard) != hit.end() || std::find(dd.begin(), dd.end(), playerSplitSum*playerSplitSumHard) != dd.end()) {
        if (std::find(dd.begin(), dd.end(), playerSplitSum*playerSplitSumHard) != dd.end() && counter == 0) {
            random = rand()%deck.size();
            playerSplitCards.push_back(deck[random]);
            playerSplitSum += deck[random];
            if (deck[random] == 11) {
                if (playerSplitSum <= high) {
                    playerSplitSumHard = -1;
                } else {
                    playerSplitSum -= 10;
                }
            }
            if (playerSplitSum > high && playerSplitSumHard == -1) {
                playerSplitSum -= 10;
                playerSplitSumHard = 1;
            }
            playerSplitSumHard = 2;
            break;
        }
        random = rand()%deck.size();
        playerSplitCards.push_back(deck[random]);
        playerSplitSum += deck[random];
        if (deck[random] == 11) {
            if (playerSplitSum <= high) {
                playerSplitSumHard = -1;
            } else {
                playerSplitSum -= 10;
            }
        }
        if (playerSplitSum > high && playerSplitSumHard == -1) {
            playerSplitSum -= 10;
            playerSplitSumHard = 1;
        }
        counter += 1;
    }
    // add cards for dealer
    while (dealerSum < dealerHigh || ((dealerSumHard == -1 && dealerSum == dealerHigh) && dealerHard == true)) {
        random = rand()%deck.size();
        dealerCards.push_back(deck[random]);
        dealerSum += deck[random];
        if (deck[random] == 11) {
            if (dealerSum <= high) {
                dealerSumHard = -1;
            } else {
                dealerSum -= 10;
            }
        }
        if (dealerSum > high && dealerSumHard == -1) {
            dealerSum -= 10;
            dealerSumHard = 1;
        }
    }
    
    double wins = 0;
    if (dealerSum > high) {
        if (playerSum == high && playerCards.size() == 2 && playerSplitSum == 0) {
            wins += 1.5;
        } else if (playerSum > high) {
            wins -= 1;
            if (playerSumHard == 2) {
                wins -= 1;
            }
        } else if (playerSum <= high) {
            wins += 1;
            if (playerSumHard == 2) {
                wins += 1;
            }
        }
        if (playerSplitSum > high && playerSplitSum > 0) {
            wins -= 1;
            if (playerSplitSumHard == 2) {
                wins -= 1;
            }
        } else if (playerSplitSum <= high && playerSplitSum > 0) {
            wins += 1;
            if (playerSplitSumHard == 2) {
                wins += 1;
            }
        }
    } else {
        if (playerSum == high && playerCards.size() == 2 && dealerSum != high && playerSplitSum == 0) {
            wins += 1.5;
        } else if (playerSum > high || playerSum < dealerSum) {
            wins -= 1;
            if (playerSumHard == 2) {
                wins -= 1;
            }
        } else if (playerSum > dealerSum) {
            wins += 1;
            if (playerSumHard == 2) {
                wins += 1;
            }
        }
        if ((playerSplitSum > high || playerSplitSum < dealerSum) && playerSplitSum > 0) {
            wins -= 1;
            if (playerSplitSumHard == 2) {
                wins -= 1;
            }
        } else if (playerSplitSum > dealerSum && playerSplitSum > 0) {
            wins += 1;
            if (playerSplitSumHard == 2) {
                wins += 1;
            }
        }
    }

    
    

    if (false) {
        std::cout << "P1: ";
        for (int i=0; i<playerCards.size(); i++) {
            std::cout << playerCards[i] << ", ";
        }
        std::cout << "\b\b = " << playerSum << std::endl;
        if (playerSplitSum > 0) {
            std::cout << "P2: ";
            for (int i=0; i<playerSplitCards.size(); i++) {
                std::cout << playerSplitCards[i] << ", ";
            }
            std::cout << "\b\b = " << playerSplitSum << std::endl;
        }
        std::cout << "DL: ";
        for (int i=0; i<dealerCards.size(); i++) {
            std::cout << dealerCards[i] << ", ";
        }
        std::cout << "\b\b = " << dealerSum << std::endl;
        std::cout << wins << std::endl;
    }



    if (wins == 4) {
        return 0;
    } else if (wins == 3) {
        return 1;
    } else if (wins == 2) {
        return 2;
    } else if (wins == 1) {
        return 3;
    } else if (wins == 0) {
        return 4;
    } else if (wins == -1) {
        return 5;
    } else if (wins == -2) {
        return 6;
    } else if (wins == -3) {
        return 7;
    } else if (wins == -4) {
        return 8;
    } else if (wins == 1.5) {
        return 9;
    }
    return -1;
}

int main() {
    auto timeAmount = std::chrono::high_resolution_clock::now();

    std::string fileName;
    std::cout << "Enter file name: ";
    std::cin >> fileName;
    std::ofstream file(fileName + ".txt");

    std::vector<int> deck = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};
    
    std::vector<long double> stats(10, 0);
    long double sum = 0.0;
    int index = 0;
    srand(time(0));
    std::vector<int> playerCards(0, 0);
    std::vector<int> dealerCards(0, 0);
    std::vector<int> player1Range = {2, 11};
    std::vector<int> player2Range = {2, 11};
    std::vector<int> dealer1Range = {2, 11};
    std::vector<int> dealer2Range = {2, 11};
    for (int i=player1Range[0]; i<=player1Range[1]; i++) {
        playerCards.push_back(i);
        for (int j=player2Range[0]; j<=player2Range[1]; j++) {
            playerCards.push_back(j);
            for (int k=dealer1Range[0]; k<=dealer1Range[1]; k++) {
                dealerCards.push_back(k);
                for (int l=dealer2Range[0]; l<=dealer2Range[1]; l++) {
                    dealerCards.push_back(l);
                    std::cout << playerCards[0] << ", " << playerCards[1] << " vs. " << dealerCards[0] << ", " << dealerCards[1] << std::endl;
                    for (int m=0; m<iterations; m++) {
                        index = deal(deck, playerCards, dealerCards);
                        stats[index] += 1;
                        if (index == 9) {
                            sum += 1.5;
                        } else {
                            sum += 4 - index;
                        }
                    }
                    file << i << ", " << j << ", " << k << ", " << l;
                    for (int m=0; m<stats.size(); m++) {
                        file << ", " << stats[m]/iterations;
                        stats[m] = 0;
                    }
                    file << "\n";
                    dealerCards.pop_back();
                }
                dealerCards.pop_back();
            }
            playerCards.pop_back();
        }
        playerCards.pop_back();
    }

    std::cout << std::endl << sum/iterations << std::endl;

    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - timeAmount).count() / 1000 << " milliseconds\n";
    return 0;
}