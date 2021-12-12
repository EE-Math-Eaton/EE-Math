#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

#define cardType 10
#define high 21
#define matrixSize 49

#define dealerHigh 17
#define dealerHard true

std::vector<std::vector<long double>> matrixMultiplication(std::vector<std::vector<long double>> m, std::vector<std::vector<long double>> n) {
    std::vector<std::vector<long double>> result;
    for (int i=0; i<m.size(); i++) {
        result.push_back({});
        for (int j=0; j<n[i].size(); j++) {
            result[i].push_back(0);
            for (int k=0; k<n.size(); k++) {
                result[i][j] += m[i][k] * n[k][j];
            }
        }
    }
    return result;
}

std::vector<std::vector<long double>> matrixPower(std::vector<std::vector<long double>> m, std::vector<std::vector<long double>> n, int power) {
    std::vector<std::vector<long double>> result;
    for (int i=0; i<m.size(); i++) {
        result.push_back({});
        for (int j=0; j<m.size(); j++) {
            result[i].push_back(0);
            for (int k=0; k<m.size(); k++) {
                result[i][j] += m[i][k] * n[k][j];
            }
        }
    }
    if (power < 2) {
        return m;
    } else if (power == 2) {
        return result;
    } else {
        return matrixPower(m, result, power-1);
    }
}

void dealer(std::vector<std::vector<long double>> &playerMatrix, std::vector<int> dealerCards, long double playerSum, bool D, int index, std::vector<std::vector<long double>> splitsHand) {
    // {A, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    std::vector<long double> probs = {1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 4.0/13};

    if (dealerCards[0] == 11 && dealerCards[1] == 11) {
        dealerCards[1] = 1;
    }
    if ((dealerCards[0] + dealerCards[1] >= dealerHigh && (dealerCards[0] != 11 && dealerCards[1] != 11)) || (dealerCards[0] + dealerCards[1] > dealerHigh || (dealerCards[0] + dealerCards[1] == dealerHigh && dealerHard == false))) {
        if (splitsHand.size() == 0) {
            if (D == false) {
                if (dealerCards[0] + dealerCards[1] < playerSum) {
                    // SW
                    playerMatrix[index][matrixSize-7] = 1;
                } else if (dealerCards[0] + dealerCards[1] > playerSum) {
                    // SL
                    playerMatrix[index][matrixSize-5] = 1;
                } else if (dealerCards[0] + dealerCards[1] == playerSum) {
                    // T
                    playerMatrix[index][matrixSize-6] = 1;
                }
            } else {
                if (dealerCards[0] + dealerCards[1] < playerSum) {
                    // DW
                    playerMatrix[index][matrixSize-8] = 1;
                } else if (dealerCards[0] + dealerCards[1] > playerSum) {
                    // DL
                    playerMatrix[index][matrixSize-4] = 1;
                } else if (dealerCards[0] + dealerCards[1] == playerSum) {
                    // T
                    playerMatrix[index][matrixSize-6] = 1;
                }
            }
        } else {
            int wins = 0;
            if (D == false) {
                if (dealerCards[0] + dealerCards[1] < playerSum) {
                    // SW
                    wins += 1;
                } else if (dealerCards[0] + dealerCards[1] > playerSum) {
                    // SL
                    wins -= 1;
                }
            } else {
                if (dealerCards[0] + dealerCards[1] < playerSum) {
                    // DW
                    wins += 2;
                } else if (dealerCards[0] + dealerCards[1] > playerSum) {
                    // DL
                    wins -= 2;
                }
            }
            if (playerSum > 1) {
                playerSum = 1;
            }
            // S
            for (int i=cardType+2; i<=high; i++) {
                if (splitsHand[0][i-(cardType+2)] > 0) {
                    if (dealerCards[0] + dealerCards[1] < i) {
                        // SW
                        playerMatrix[index][matrixSize-(wins+1)-6] += splitsHand[0][i-(cardType+2)] * playerSum;
                    } else if (dealerCards[0] + dealerCards[1] > i) {
                        // SL
                        playerMatrix[index][matrixSize-(wins-1)-6] += splitsHand[0][i-(cardType+2)] * playerSum;
                    } else if (dealerCards[0] + dealerCards[1] == i) {
                        // T
                        playerMatrix[index][matrixSize-(wins)-6] += splitsHand[0][i-(cardType+2)] * playerSum;
                    }
                }
            }
            // H
            for (int i=4; i<=high; i++) {
                if (splitsHand[0][i-4+cardType] > 0) {
                    if (dealerCards[0] + dealerCards[1] < i) {
                        // SW
                        playerMatrix[index][matrixSize-(wins+1)-6] += splitsHand[0][i-4+cardType] * playerSum;
                    } else if (dealerCards[0] + dealerCards[1] > i) {
                        // SL
                        playerMatrix[index][matrixSize-(wins-1)-6] += splitsHand[0][i-4+cardType] * playerSum;
                    } else if (dealerCards[0] + dealerCards[1] == i) {
                        // T
                        playerMatrix[index][matrixSize-(wins)-6] += splitsHand[0][i-4+cardType] * playerSum;
                    }
                }
            }
            // D
            for (int i=cardType+1; i<=high; i++) {
                if (splitsHand[0][i-(cardType+1)+cardType+(high-3)] > 0) {
                    if (dealerCards[0] + dealerCards[1] < i) {
                        // DW
                        playerMatrix[index][matrixSize-(wins+2)-6] += splitsHand[0][i-(cardType+1)+cardType+(high-3)] * playerSum;
                    } else if (dealerCards[0] + dealerCards[1] > i) {
                        // DL
                        playerMatrix[index][matrixSize-(wins-2)-6] += splitsHand[0][i-(cardType+1)+cardType+(high-3)] * playerSum;
                    } else if (dealerCards[0] + dealerCards[1] == i) {
                        // T
                        playerMatrix[index][matrixSize-(wins)-6] += splitsHand[0][i-(cardType+1)+cardType+(high-3)] * playerSum;
                    }
                }
            }
            // SL
            if (splitsHand[0][matrixSize-5] > 0) {
                playerMatrix[index][matrixSize-(wins-1)-6] += splitsHand[0][matrixSize-5] * playerSum;
            }
            // DL
            if (splitsHand[0][matrixSize-4] > 0) {
                playerMatrix[index][matrixSize-(wins-2)-6] += splitsHand[0][matrixSize-4] * playerSum;
            }
        }
        return;
    }

    std::vector<std::vector<long double>> dealerHand(1, std::vector<long double>(matrixSize, 0));
    std::vector<std::vector<long double>> dealerMatrix(matrixSize, std::vector<long double>(matrixSize, 0));
    dealerMatrix[matrixSize-10][matrixSize-10] = 1;
    dealerMatrix[matrixSize-9][matrixSize-9] = 1;
    dealerMatrix[matrixSize-8][matrixSize-8] = 1;
    dealerMatrix[matrixSize-7][matrixSize-7] = 1;
    dealerMatrix[matrixSize-6][matrixSize-6] = 1;
    dealerMatrix[matrixSize-5][matrixSize-5] = 1;
    dealerMatrix[matrixSize-4][matrixSize-4] = 1;
    dealerMatrix[matrixSize-3][matrixSize-3] = 1;
    dealerMatrix[matrixSize-2][matrixSize-2] = 1;
    dealerMatrix[matrixSize-1][matrixSize-1] = 1;

    if (dealerCards[0] == 11 || dealerCards[1] == 11) {
        for (int i=0; i<cardType; i++) {
            if (dealerCards[0]+dealerCards[1]+(i+1) <= high) {
                // S -> S
                dealerHand[0][dealerCards[0]+dealerCards[1]-(cardType)+i] += probs[i];
            } else {
                // S -> H
                dealerHand[0][dealerCards[0]+dealerCards[1]-3+i] += probs[i];
            }
        }
    } else {
        for (int i=1; i<cardType; i++) {
            if (dealerCards[0]+dealerCards[1]+(i+1) <= high) {
                // H -> H
                dealerHand[0][dealerCards[0]+dealerCards[1]+cardType-3+i] += probs[i];
            } else {
                // H -> W
                if (splitsHand.size() == 0) {
                    if (D == false) {
                        // SW
                        dealerHand[0][matrixSize-7] += probs[i];
                    } else {
                        // DW
                        dealerHand[0][matrixSize-8] += probs[i];
                    }
                } else {
                    int wins = 1;
                    if (D == true) {
                        wins += 1;
                    }
                    if (playerSum > 1) {
                        playerSum = 1;
                    }
                    // S
                    for (int j=cardType+2; j<=high; j++) {
                        if (splitsHand[0][j-(cardType+2)] > 0) {
                            // SW
                            dealerHand[0][matrixSize-(wins+1)-6] += splitsHand[0][j-(cardType+2)] * playerSum * probs[i];
                        }
                    }
                    // H
                    for (int j=4; j<=high; j++) {
                        if (splitsHand[0][j-4+cardType] > 0) {
                            // SW
                            dealerHand[0][matrixSize-(wins+1)-6] += splitsHand[0][j-4+cardType] * playerSum * probs[i];
                        }
                    }
                    // D
                    for (int j=cardType+1; j<=high; j++) {
                        if (splitsHand[0][j-(cardType+1)+cardType+(high-3)] > 0) {
                            // DW
                            dealerHand[0][matrixSize-(wins+2)-6] += splitsHand[0][j-(cardType+1)+cardType+(high-3)] * playerSum * probs[i];
                        }
                    }
                    // SL
                    if (splitsHand[0][matrixSize-5] > 0) {
                        dealerHand[0][matrixSize-(wins-1)-6] += splitsHand[0][matrixSize-5] * playerSum * probs[i];
                    }
                    // DL
                    if (splitsHand[0][matrixSize-4] > 0) {
                        dealerHand[0][matrixSize-(wins-2)-6] += splitsHand[0][matrixSize-4] * playerSum * probs[i];
                    }
                }
            }
        }
        if (dealerCards[0]+dealerCards[1]+(cardType+1) <= high) {
            // H -> S
            dealerHand[0][dealerCards[0]+dealerCards[1]-1] += probs[0];
        } else if (dealerCards[0]+dealerCards[1]+(1) <= high) {
            // H -> H
            dealerHand[0][dealerCards[0]+dealerCards[1]+cardType-4+1] += probs[0];
        } else {
            // H -> W
            if (splitsHand.size() == 0) {
                if (D == false) {
                    // SW
                    dealerHand[0][matrixSize-7] += probs[0];
                } else {
                    // DW
                    dealerHand[0][matrixSize-8] += probs[0];
                }
            } else {
                int wins = 1;
                if (D == true) {
                    wins += 1;
                }
                if (playerSum > 1) {
                    playerSum = 1;
                }
                // S
                for (int i=cardType+2; i<=high; i++) {
                    if (splitsHand[0][i-(cardType+2)] > 0) {
                        // SW
                        dealerHand[0][matrixSize-(wins+1)-6] += splitsHand[0][i-(cardType+2)] * playerSum * probs[0];
                    }
                }
                // H
                for (int i=4; i<=high; i++) {
                    if (splitsHand[0][i-4+cardType] > 0) {
                        // SW
                        dealerHand[0][matrixSize-(wins+1)-6] += splitsHand[0][i-4+cardType] * playerSum * probs[0];
                    }
                }
                // D
                for (int i=cardType+1; i<=high; i++) {
                    if (splitsHand[0][i-(cardType+1)+cardType+(high-3)] > 0) {
                        // DW
                        dealerHand[0][matrixSize-(wins+2)-6] += splitsHand[0][i-(cardType+1)+cardType+(high-3)] * playerSum * probs[0];
                    }
                }
                // SL
                if (splitsHand[0][matrixSize-5] > 0) {
                    dealerHand[0][matrixSize-(wins-1)-6] += splitsHand[0][matrixSize-5] * playerSum * probs[0];
                }
                // DL
                if (splitsHand[0][matrixSize-4] > 0) {
                    dealerHand[0][matrixSize-(wins-2)-6] += splitsHand[0][matrixSize-4] * playerSum * probs[0];
                }
            }
        }
    }

    // S
    for (int i=cardType+2; i<=high; i++) {
        if (i > dealerHigh || (i == dealerHigh && dealerHard == false)) {
            if (splitsHand.size() == 0) {
                if (D == false) {
                    if (i < playerSum) {
                        // SW
                        dealerMatrix[i-(cardType+2)][matrixSize-7] = 1;
                    } else if (i > playerSum) {
                        // SL
                        dealerMatrix[i-(cardType+2)][matrixSize-5] = 1;
                    } else if (i == playerSum) {
                        // T
                        dealerMatrix[i-(cardType+2)][matrixSize-6] = 1;
                    }
                } else {
                    if (i < playerSum) {
                        // DW
                        dealerMatrix[i-(cardType+2)][matrixSize-8] = 1;
                    } else if (i > playerSum) {
                        // DL
                        dealerMatrix[i-(cardType+2)][matrixSize-4] = 1;
                    } else if (i == playerSum) {
                        // T
                        dealerMatrix[i-(cardType+2)][matrixSize-6] = 1;
                    }
                }
            } else {
                int wins = 0;
                if (D == false) {
                    if (i < playerSum) {
                        // SW
                        wins += 1;
                    } else if (i > playerSum) {
                        // SL
                        wins -= 1;
                    }
                } else {
                    if (i < playerSum) {
                        // DW
                        wins += 2;
                    } else if (i > playerSum) {
                        // DL
                        wins -= 2;
                    }
                }
                if (playerSum > 1) {
                    playerSum = 1;
                }
                // S
                for (int j=cardType+2; j<=high; j++) {
                    if (splitsHand[0][j-(cardType+2)] > 0) {
                        if (i < j) {
                            // SW
                            dealerMatrix[i-(cardType+2)][matrixSize-(wins+1)-6] += splitsHand[0][j-(cardType+2)] * playerSum;
                        } else if (i > j) {
                            // SL
                            dealerMatrix[i-(cardType+2)][matrixSize-(wins-1)-6] += splitsHand[0][j-(cardType+2)] * playerSum;
                        } else if (i == j) {
                            // T
                            dealerMatrix[i-(cardType+2)][matrixSize-(wins)-6] += splitsHand[0][j-(cardType+2)] * playerSum;
                        }
                    }
                }
                // H
                for (int j=4; j<=high; j++) {
                    if (splitsHand[0][j-4+cardType] > 0) {
                        if (i < j) {
                            // SW
                            dealerMatrix[i-(cardType+2)][matrixSize-(wins+1)-6] += splitsHand[0][j-4+cardType] * playerSum;
                        } else if (i > j) {
                            // SL
                            dealerMatrix[i-(cardType+2)][matrixSize-(wins-1)-6] += splitsHand[0][j-4+cardType] * playerSum;
                        } else if (i == j) {
                            // T
                            dealerMatrix[i-(cardType+2)][matrixSize-(wins)-6] += splitsHand[0][j-4+cardType] * playerSum;
                        }
                    }
                }
                // D
                for (int j=cardType+1; j<=high; j++) {
                    if (splitsHand[0][j-(cardType+1)+cardType+(high-3)] > 0) {
                        if (i < j) {
                            // DW
                            dealerMatrix[i-(cardType+2)][matrixSize-(wins+2)-6] += splitsHand[0][j-(cardType+1)+cardType+(high-3)] * playerSum;
                        } else if (i > j) {
                            // DL
                            dealerMatrix[i-(cardType+2)][matrixSize-(wins-2)-6] += splitsHand[0][j-(cardType+1)+cardType+(high-3)] * playerSum;
                        } else if (i == j) {
                            // T
                            dealerMatrix[i-(cardType+2)][matrixSize-(wins)-6] += splitsHand[0][j-(cardType+1)+cardType+(high-3)] * playerSum;
                        }
                    }
                }
                // SL
                if (splitsHand[0][matrixSize-5] > 0) {
                    dealerMatrix[i-(cardType+2)][matrixSize-(wins-1)-6] += splitsHand[0][matrixSize-5] * playerSum;
                }
                // DL
                if (splitsHand[0][matrixSize-4] > 0) {
                    dealerMatrix[i-(cardType+2)][matrixSize-(wins-2)-6] += splitsHand[0][matrixSize-4] * playerSum;
                }
            }
        } else {
            for (int j=0; j<cardType; j++) {
                if (i+(j+1) <= high) {
                    // S -> S
                    dealerMatrix[i-(cardType+2)][i-(cardType+2)+(j+1)] += probs[j];
                } else {
                    // S -> H
                    dealerMatrix[i-(cardType+2)][i-(cardType+2)+(cardType-2)+(j+1)] += probs[j];
                }
            }
        }
    }
    // H
    for (int i=4; i<=high; i++) {
        if (i >= dealerHigh) {
            if (splitsHand.size() == 0) {
                if (D == false) {
                    if (i < playerSum) {
                        // SW
                        dealerMatrix[i-4+cardType][matrixSize-7] = 1;
                    } else if (i > playerSum) {
                        // SL
                        dealerMatrix[i-4+cardType][matrixSize-5] = 1;
                    } else if (i == playerSum) {
                        // T
                        dealerMatrix[i-4+cardType][matrixSize-6] = 1;
                    }
                } else {
                    if (i < playerSum) {
                        // DW
                        dealerMatrix[i-4+cardType][matrixSize-8] = 1;
                    } else if (i > playerSum) {
                        // DL
                        dealerMatrix[i-4+cardType][matrixSize-4] = 1;
                    } else if (i == playerSum) {
                        // T
                        dealerMatrix[i-4+cardType][matrixSize-6] = 1;
                    }
                }
            } else {
                int wins = 0;
                if (D == false) {
                    if (i < playerSum) {
                        // SW
                        wins += 1;
                    } else if (i > playerSum) {
                        // SL
                        wins -= 1;
                    }
                } else {
                    if (i < playerSum) {
                        // DW
                        wins += 2;
                    } else if (i > playerSum) {
                        // DL
                        wins -= 2;
                    }
                }
                if (playerSum > 1) {
                    playerSum = 1;
                }
                // S
                for (int j=cardType+2; j<=high; j++) {
                    if (splitsHand[0][j-(cardType+2)] > 0) {
                        if (i < j) {
                            // SW
                            dealerMatrix[i-4+cardType][matrixSize-(wins+1)-6] += splitsHand[0][j-(cardType+2)] * playerSum;
                        } else if (i > j) {
                            // SL
                            dealerMatrix[i-4+cardType][matrixSize-(wins-1)-6] += splitsHand[0][j-(cardType+2)] * playerSum;
                        } else if (i == j) {
                            // T
                            dealerMatrix[i-4+cardType][matrixSize-(wins)-6] += splitsHand[0][j-(cardType+2)] * playerSum;
                        }
                    }
                }
                // H
                for (int j=4; j<=high; j++) {
                    if (splitsHand[0][j-4+cardType] > 0) {
                        if (i < j) {
                            // SW
                            dealerMatrix[i-4+cardType][matrixSize-(wins+1)-6] += splitsHand[0][j-4+cardType] * playerSum;
                        } else if (i > j) {
                            // SL
                            dealerMatrix[i-4+cardType][matrixSize-(wins-1)-6] += splitsHand[0][j-4+cardType] * playerSum;
                        } else if (i == j) {
                            // T
                            dealerMatrix[i-4+cardType][matrixSize-(wins)-6] += splitsHand[0][j-4+cardType] * playerSum;
                        }
                    }
                }
                // D
                for (int j=cardType+1; j<=high; j++) {
                    if (splitsHand[0][j-(cardType+1)+cardType+(high-3)] > 0) {
                        if (i < j) {
                            // DW
                            dealerMatrix[i-4+cardType][matrixSize-(wins+2)-6] += splitsHand[0][j-(cardType+1)+cardType+(high-3)] * playerSum;
                        } else if (i > j) {
                            // DL
                            dealerMatrix[i-4+cardType][matrixSize-(wins-2)-6] += splitsHand[0][j-(cardType+1)+cardType+(high-3)] * playerSum;
                        } else if (i == j) {
                            // T
                            dealerMatrix[i-4+cardType][matrixSize-(wins)-6] += splitsHand[0][j-(cardType+1)+cardType+(high-3)] * playerSum;
                        }
                    }
                }
                // SL
                if (splitsHand[0][matrixSize-5] > 0) {
                    dealerMatrix[i-4+cardType][matrixSize-(wins-1)-6] += splitsHand[0][matrixSize-5] * playerSum;
                }
                // DL
                if (splitsHand[0][matrixSize-4] > 0) {
                    dealerMatrix[i-4+cardType][matrixSize-(wins-2)-6] += splitsHand[0][matrixSize-4] * playerSum;
                }
            }
        } else {
            for (int j=1; j<cardType; j++) {
                if (i+(j+1) <= high) {
                    // H -> H
                    dealerMatrix[i-4+cardType][i-4+cardType+(j+1)] += probs[j];
                } else {
                    // H -> W
                    if (splitsHand.size() == 0) {
                        if (D == false) {
                            // SW
                            dealerMatrix[i-4+cardType][matrixSize-7] += probs[j];
                        } else {
                            // DW
                            dealerMatrix[i-4+cardType][matrixSize-8] += probs[j];
                        }
                    } else {
                        int wins = 1;
                        if (D == true) {
                            wins += 1;
                        }
                        if (playerSum > 1) {
                            playerSum = 1;
                        }
                        // S
                        for (int k=cardType+2; k<=high; k++) {
                            if (splitsHand[0][k-(cardType+2)] > 0) {
                                // SW
                                dealerMatrix[i-4+cardType][matrixSize-(wins+1)-6] += splitsHand[0][k-(cardType+2)] * playerSum * probs[j];
                            }
                        }
                        // H
                        for (int k=4; k<=high; k++) {
                            if (splitsHand[0][k-4+cardType] > 0) {
                                // SW
                                dealerMatrix[i-4+cardType][matrixSize-(wins+1)-6] += splitsHand[0][k-4+cardType] * playerSum * probs[j];
                            }
                        }
                        // D
                        for (int k=cardType+1; k<=high; k++) {
                            if (splitsHand[0][k-(cardType+1)+cardType+(high-3)] > 0) {
                                // DW
                                dealerMatrix[i-4+cardType][matrixSize-(wins+2)-6] += splitsHand[0][k-(cardType+1)+cardType+(high-3)] * playerSum * probs[j];
                            }
                        }
                        // SL
                        if (splitsHand[0][matrixSize-5] > 0) {
                            dealerMatrix[i-4+cardType][matrixSize-(wins-1)-6] += splitsHand[0][matrixSize-5] * playerSum * probs[j];
                        }
                        // DL
                        if (splitsHand[0][matrixSize-4] > 0) {
                            dealerMatrix[i-4+cardType][matrixSize-(wins-2)-6] += splitsHand[0][matrixSize-4] * playerSum * probs[j];
                        }
                    }
                }
            }
            if (i+(cardType+1) <= high) {
                // H -> S
                dealerMatrix[i-4+cardType][i-4+(3)] += probs[0];
            } else if (i+1 <= high) {
                // H -> H
                dealerMatrix[i-4+cardType][i-4+cardType+(1)] += probs[0];
            } else {
                // H -> W
                if (splitsHand.size() == 0) {
                    if (D == false) {
                        // SW
                        dealerMatrix[i-4+cardType][matrixSize-7] += probs[0];
                    } else {
                        // DW
                        dealerMatrix[i-4+cardType][matrixSize-8] += probs[0];
                    }
                } else {
                    int wins = 1;
                    if (D == true) {
                        wins += 1;
                    }
                    if (playerSum > 1) {
                        playerSum = 1;
                    }
                    // S
                    for (int j=cardType+2; j<=high; j++) {
                        if (splitsHand[0][j-(cardType+2)] > 0) {
                            // SW
                            dealerMatrix[i-4+cardType][matrixSize-(wins+1)-6] += splitsHand[0][j-(cardType+2)] * playerSum * probs[0];
                        }
                    }
                    // H
                    for (int j=4; j<=high; j++) {
                        if (splitsHand[0][j-4+cardType] > 0) {
                            // SW
                            dealerMatrix[i-4+cardType][matrixSize-(wins+1)-6] += splitsHand[0][j-4+cardType] * playerSum * probs[0];
                        }
                    }
                    // D
                    for (int j=cardType+1; j<=high; j++) {
                        if (splitsHand[0][j-(cardType+1)+cardType+(high-3)] > 0) {
                            // DW
                            dealerMatrix[i-4+cardType][matrixSize-(wins+2)-6] += splitsHand[0][j-(cardType+1)+cardType+(high-3)] * playerSum * probs[0];
                        }
                    }
                    // SL
                    if (splitsHand[0][matrixSize-5] > 0) {
                        dealerMatrix[i-4+cardType][matrixSize-(wins-1)-6] += splitsHand[0][matrixSize-5] * playerSum * probs[0];
                    }
                    // DL
                    if (splitsHand[0][matrixSize-4] > 0) {
                        dealerMatrix[i-4+cardType][matrixSize-(wins-2)-6] += splitsHand[0][matrixSize-4] * playerSum * probs[0];
                    }
                }
            }
        }
    }
    // D
    for (int i=cardType+1; i<=high; i++) {
        dealerMatrix[i-(cardType+1)+cardType+(high-3)][i-(cardType+1)+cardType+(high-3)] = 1;
    }
    
    // long double x = 0.0;
    // for (int i=0; i<dealerHand[0].size(); i++) {
    //     for (int j=0; j<dealerMatrix[0].size(); j++) {
    //         x += dealerMatrix[i][j];
    //         std::cout << dealerMatrix[i][j] << ", ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << x << std::endl;

    dealerMatrix = matrixPower(dealerMatrix, dealerMatrix, high*2);
    dealerHand = matrixMultiplication(dealerHand, dealerMatrix);

    playerMatrix[index][matrixSize-10] += dealerHand[0][matrixSize-10];
    playerMatrix[index][matrixSize-9] += dealerHand[0][matrixSize-9];
    playerMatrix[index][matrixSize-8] += dealerHand[0][matrixSize-8];
    playerMatrix[index][matrixSize-7] += dealerHand[0][matrixSize-7];
    playerMatrix[index][matrixSize-6] += dealerHand[0][matrixSize-6];
    playerMatrix[index][matrixSize-5] += dealerHand[0][matrixSize-5];
    playerMatrix[index][matrixSize-4] += dealerHand[0][matrixSize-4];
    playerMatrix[index][matrixSize-3] += dealerHand[0][matrixSize-3];
    playerMatrix[index][matrixSize-2] += dealerHand[0][matrixSize-2];
    playerMatrix[index][matrixSize-1] += dealerHand[0][matrixSize-1];

    return;
}

void player(std::vector<std::vector<long double>> &playerMatrix, std::vector<int> dealerCards, std::vector<int> hit, std::vector<std::vector<long double>> splitsHand) {
    // {A, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    std::vector<long double> probs = {1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 4.0/13};

    // S
    for (int i=cardType+2; i<=high; i++) {
        if (std::find(hit.begin(), hit.end(), i*-1) != hit.end()) {
            for (int j=0; j<cardType; j++) {
                if (i+(j+1) <= high) {
                    // S -> S
                    playerMatrix[i-(cardType+2)][i-(cardType+2)+(j+1)] += probs[j];
                } else {
                    // S -> H
                    playerMatrix[i-(cardType+2)][i-(cardType+2)+(cardType-2)+(j+1)] += probs[j];
                }
            }
        } else {
            dealer(playerMatrix, dealerCards, i, false, i-(cardType+2), splitsHand);
        }
    }
    // H
    for (int i=4; i<=high; i++) {
        if (std::find(hit.begin(), hit.end(), i) != hit.end()) {
            for (int j=1; j<cardType; j++) {
                if (i+(j+1) <= high) {
                    // H -> H
                    playerMatrix[i-4+cardType][i-4+cardType+(j+1)] += probs[j];
                } else {
                    // H -> L
                    if (splitsHand.size() == 0) {
                        // SL
                        playerMatrix[i-4+cardType][matrixSize-5] += probs[j];
                    } else {
                        dealer(playerMatrix, dealerCards, probs[j], false, i-4+cardType, splitsHand);
                    }
                }
            }
            if (i+(cardType+1) <= high) {
                // H -> S
                playerMatrix[i-4+cardType][i-4+(3)] += probs[0];
            } else if (i+1 <= high) {
                // H -> H
                playerMatrix[i-4+cardType][i-4+cardType+(1)] += probs[0];
            } else {
                // H -> L
                if (splitsHand.size() == 0) {
                    // SL
                    playerMatrix[i-4+cardType][matrixSize-5] += probs[0];
                } else {
                    dealer(playerMatrix, dealerCards, probs[0], false, i-4+cardType, splitsHand);
                }
            }
        } else {
            dealer(playerMatrix, dealerCards, i, false, i-4+cardType, splitsHand);
        }
    }
    // D
    for (int i=cardType+1; i<=high; i++) {
        dealer(playerMatrix, dealerCards, i, true, i-(cardType+1)+cardType+(high-3), splitsHand);
    }

    return;
}

void hand(std::vector<std::vector<long double>> &playerHand, std::vector<int> playerCards, std::vector<int> hit, std::vector<int> dd){
    // {A, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    std::vector<long double> probs = {1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 4.0/13};

    if ((playerCards[0] == 11 || playerCards[1] == 11) && std::find(hit.begin(), hit.end(), (playerCards[0]+playerCards[1])*-1) != hit.end()) {
        for (int i=0; i<cardType; i++) {
            if (playerCards[0]+playerCards[1]+(i+1) <= high) {
                // S -> S
                playerHand[0][playerCards[0]+playerCards[1]-(cardType)+i] += probs[i];
            } else {
                // S -> H
                playerHand[0][playerCards[0]+playerCards[1]-3+i] += probs[i];
            }
        }
    } else if ((playerCards[0] == 11 || playerCards[1] == 11) && std::find(dd.begin(), dd.end(), (playerCards[0]+playerCards[1])*-1) != dd.end()) {
        for (int i=0; i<cardType; i++) {
            if (playerCards[0]+playerCards[1]+(i+1) <= high) {
                // S -> D
                playerHand[0][playerCards[0]+playerCards[1]+high-3+i] += probs[i];
            } else {
                // S -> D
                playerHand[0][playerCards[0]+playerCards[1]+cardType-2+i] += probs[i];
            }
        }
    } else if ((playerCards[0] != 11 || playerCards[1] != 11) && std::find(hit.begin(), hit.end(), playerCards[0]+playerCards[1]) != hit.end()) {
        for (int i=1; i<cardType; i++) {
            if (playerCards[0]+playerCards[1]+(i+1) <= high) {
                // H -> H
                playerHand[0][playerCards[0]+playerCards[1]+cardType-3+i] += probs[i];
            } else {
                // H -> L
                // SL
                playerHand[0][matrixSize-5] += probs[i];
            }
        }
        if (playerCards[0]+playerCards[1]+(cardType+1) <= high) {
            // H -> S
            playerHand[0][playerCards[0]+playerCards[1]-1] += probs[0];
        } else if (playerCards[0]+playerCards[1]+(1) <= high) {
            // H -> H
            playerHand[0][playerCards[0]+playerCards[1]+cardType-4+1] += probs[0];
        } else {
            // H -> L
            // SL
            playerHand[0][matrixSize-5] += probs[0];
        }
    } else if ((playerCards[0] != 11 || playerCards[1] != 11) && std::find(dd.begin(), dd.end(), playerCards[0]+playerCards[1]) != dd.end()) {
        for (int i=1; i<cardType; i++) {
            if (playerCards[0]+playerCards[1]+(i+1) <= high) {
                // H -> D
                playerHand[0][playerCards[0]+playerCards[1]+cardType+(cardType+1)-3+i] += probs[i];
            } else {
                // H -> L
                // DL
                playerHand[0][matrixSize-4] += probs[i];
            }
        }
        if (playerCards[0]+playerCards[1]+(cardType+1) <= high) {
            // H -> D
            playerHand[0][playerCards[0]+playerCards[1]+cardType+(cardType+1)-4+(cardType+1)] += probs[0];
        } else if (playerCards[0]+playerCards[1]+(1) <= high) {
            // H -> D
            playerHand[0][playerCards[0]+playerCards[1]+cardType+(cardType+1)-4+1] += probs[0];
        } else {
            // H -> L
            // DL
            playerHand[0][matrixSize-4] += probs[0];
        }
    } else if (playerCards[0] == 11 || playerCards[1] == 11) {
        playerHand[0][playerCards[0]+playerCards[1]-cardType-2] = 1;
    } else {
        playerHand[0][playerCards[0]+playerCards[1]+cardType-4] = 1;
    }

    return;
}

void splits(std::vector<std::vector<long double>> &playerMatrix, std::vector<int> dealerCards, std::vector<int> hit, std::vector<int> dd, std::vector<std::vector<long double>> &playerHand, std::vector<int> splitsCards) {
    // {A, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    std::vector<long double> probs = {1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 4.0/13};

    // S
    for (int i=cardType+2; i<=high; i++) {
        if (std::find(hit.begin(), hit.end(), i*-1) != hit.end()) {
            for (int j=0; j<cardType; j++) {
                if (i+(j+1) <= high) {
                    // S -> S
                    playerMatrix[i-(cardType+2)][i-(cardType+2)+(j+1)] += probs[j];
                } else {
                    // S -> H
                    playerMatrix[i-(cardType+2)][i-(cardType+2)+(cardType-2)+(j+1)] += probs[j];
                }
            }
        } else {
            playerMatrix[i-(cardType+2)][i-(cardType+2)] = 1;
        }
    }
    // H
    for (int i=4; i<=high; i++) {
        if (std::find(hit.begin(), hit.end(), i) != hit.end()) {
            for (int j=1; j<cardType; j++) {
                if (i+(j+1) <= high) {
                    // H -> H
                    playerMatrix[i-4+cardType][i-4+cardType+(j+1)] += probs[j];
                } else {
                    // H -> L
                    // SL
                    playerMatrix[i-4+cardType][matrixSize-5] += probs[j];
                }
            }
            if (i+(cardType+1) <= high) {
                // H -> S
                playerMatrix[i-4+cardType][i-4+(3)] += probs[0];
            } else if (i+1 <= high) {
                // H -> H
                playerMatrix[i-4+cardType][i-4+cardType+(1)] += probs[0];
            } else {
                // H -> L
                // SL
                playerMatrix[i-4+cardType][matrixSize-5] += probs[0];
            }
        } else {
            playerMatrix[i-4+cardType][i-4+cardType] = 1;
        }
    }
    // D
    for (int i=cardType+1; i<=high; i++) {
        playerMatrix[i-(cardType+1)+cardType+(high-3)][i-(cardType+1)+cardType+(high-3)] = 1;
    }
    
    playerMatrix = matrixPower(playerMatrix, playerMatrix, high*2);
    playerHand = matrixMultiplication(playerHand, playerMatrix);
    
    std::vector<std::vector<long double>> splitsHand(1, std::vector<long double>(matrixSize, 0));
    std::vector<std::vector<long double>> splitsHandTemp(1, std::vector<long double>(matrixSize, 0));
    std::vector<std::vector<long double>> splitsMatrix(matrixSize, std::vector<long double>(matrixSize, 0));
    splitsMatrix[matrixSize-10][matrixSize-10] = 1;
    splitsMatrix[matrixSize-9][matrixSize-9] = 1;
    splitsMatrix[matrixSize-8][matrixSize-8] = 1;
    splitsMatrix[matrixSize-7][matrixSize-7] = 1;
    splitsMatrix[matrixSize-6][matrixSize-6] = 1;
    splitsMatrix[matrixSize-5][matrixSize-5] = 1;
    splitsMatrix[matrixSize-4][matrixSize-4] = 1;
    splitsMatrix[matrixSize-3][matrixSize-3] = 1;
    splitsMatrix[matrixSize-2][matrixSize-2] = 1;
    splitsMatrix[matrixSize-1][matrixSize-1] = 1;

    for (int i=1; i<cardType; i++) {
        splitsCards.push_back((i+1));
        hand(splitsHandTemp, splitsCards, hit, dd);
        for (int j=0; j<splitsHandTemp[0].size(); j++) {
            splitsHand[0][j] += probs[i] * splitsHandTemp[0][j];
            splitsHandTemp[0][j] = 0;
        }
        splitsCards.pop_back();
    }
    splitsCards.push_back(11);
    hand(splitsHandTemp, splitsCards, hit, dd);
    for (int i=0; i<splitsHandTemp[0].size(); i++) {
        splitsHand[0][i] += probs[0] * splitsHandTemp[0][i];
        splitsHandTemp[0][i] = 0;
    }
    splitsCards.pop_back();

    player(splitsMatrix, dealerCards, hit, playerHand);

    playerMatrix = matrixPower(splitsMatrix, splitsMatrix, high*2);
    playerHand = matrixMultiplication(splitsHand, playerMatrix);

    return;
}

void cycle(std::vector<int> dealerCards, std::vector<int> playerCards, std::vector<std::vector<long double>> &playerHand) {
    // {A, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    std::vector<long double> probs = {1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 1.0/13, 4.0/13};

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

    // [S12, S13, S14, ..., S19, S20, S21, H04, H05, H06, ..., H19, H20, H21, D11, D12, D13, ..., D19, D20, D21, QW, TW, DW, SW, T, SL, DL, TL, QL, NB]
    // S## = soft sum; H## = hard sum; D## = double sum; 
    // QW = quadruple win; TW = triple win; DW = double win; SW = single win;
    // T = tie;  
    // SL = single loss; DL = double loss; TL = triple loss; QL = quadruple loss;
    // NB = natural blackjack;
    std::vector<std::vector<long double>> playerMatrix(matrixSize, std::vector<long double>(matrixSize, 0));
    playerMatrix[matrixSize-10][matrixSize-10] = 1;
    playerMatrix[matrixSize-9][matrixSize-9] = 1;
    playerMatrix[matrixSize-8][matrixSize-8] = 1;
    playerMatrix[matrixSize-7][matrixSize-7] = 1;
    playerMatrix[matrixSize-6][matrixSize-6] = 1;
    playerMatrix[matrixSize-5][matrixSize-5] = 1;
    playerMatrix[matrixSize-4][matrixSize-4] = 1;
    playerMatrix[matrixSize-3][matrixSize-3] = 1;
    playerMatrix[matrixSize-2][matrixSize-2] = 1;
    playerMatrix[matrixSize-1][matrixSize-1] = 1;
    
    if (playerCards[0] == playerCards[1] && std::find(split.begin(), split.end(), playerCards[0]) != split.end()) {
        std::vector<std::vector<long double>> playerHandTemp(1, std::vector<long double>(matrixSize, 0));
        playerCards.pop_back();
        for (int i=1; i<cardType; i++) {
            playerCards.push_back((i+1));
            hand(playerHandTemp, playerCards, hit, dd);
            for (int j=0; j<playerHandTemp[0].size(); j++) {
                playerHand[0][j] += probs[i] * playerHandTemp[0][j];
                playerHandTemp[0][j] = 0;
            }
            playerCards.pop_back();
        }
        playerCards.push_back(11);
        hand(playerHandTemp, playerCards, hit, dd);
        for (int i=0; i<playerHandTemp[0].size(); i++) {
            playerHand[0][i] += probs[0] * playerHandTemp[0][i];
            playerHandTemp[0][i] = 0;
        }
        playerCards.pop_back();

        splits(playerMatrix, dealerCards, hit, dd, playerHand, playerCards);
    } else {
        player(playerMatrix, dealerCards, hit, {});
        hand(playerHand, playerCards, hit, dd);
        playerMatrix = matrixPower(playerMatrix, playerMatrix, high*2);
        playerHand = matrixMultiplication(playerHand, playerMatrix);
        if ((playerCards[0] == 11 && playerCards[1] == 10) || (playerCards[0] == 10 && playerCards[1] == 11)) {
            playerHand[0][matrixSize-1] = playerHand[0][matrixSize-7];
            playerHand[0][matrixSize-7] = 0;
        }
    }

    /*
    for (int i=0; i<playerMatrix.size(); i++) {
        std::cout << " | ";
        for (int j=0; j<playerMatrix[i].size(); j++) {
            std::cout << playerMatrix[i][j] << " | ";
            
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for (int i=0; i<playerHand.size(); i++) {
        std::cout << " | ";
        for (int j=0; j<playerHand[i].size(); j++) {
            std::cout << playerHand[i][j] << " | ";
            
        }
        std::cout << std::endl;
    }
    */

    return;
}

int main() {
    auto executionTime = std::chrono::high_resolution_clock::now();

    std::string fileName;
    std::cout << "Enter file name: ";
    std::cin >> fileName;
    std::ofstream file(fileName + ".txt");

    // {2, 3, 4, 5, 6, 7, 8, 9, 10, A}
    std::vector<long double> probs = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 4.0, 1.0};

    // 2 - 11 for player and dealer cards
    std::vector<long double> stats(10, 0);
    std::vector<std::vector<long double>> playerHand(1, std::vector<long double>(matrixSize, 0));
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
                    cycle(dealerCards, playerCards, playerHand);
                    file << i << ", " << j << ", " << k << ", " << l;
                    for (int m=0; m<stats.size(); m++) {
                        stats[m] += playerHand[0][matrixSize-10+m] * probs[i-2] * probs[j-2] * probs[k-2] * probs[l-2];
                        file << ", " << playerHand[0][matrixSize-10+m];
                        playerHand[0][matrixSize-10+m] = 0;
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

    int winnings = 4;
    // long double sum = 0.0;
    // long double den = 0.0;
    std::cout << std::endl;
    for (int i=0; i<stats.size()-1; i++) {
        if (winnings >= 0) {
            std::cout << "+";
        }
        std::cout << winnings << ": " << stats[i] << std::endl;
        // sum += winnings * stats[i];
        // den += stats[i];
        winnings -= 1;
    }
    std::cout << "NB: " << stats[9] << std::endl;
    // sum += 1.5 * stats[9];
    // den += stats[9];

    // std::cout << std::endl << sum;
    // std::cout << std::endl << den;
    // std::cout << std::endl << sum/den << std::endl;

	file.close();

    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - executionTime).count() / 1000 << " milliseconds\n";
}