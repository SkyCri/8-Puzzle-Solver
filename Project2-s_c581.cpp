// Project2Version2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <sys/time.h>
using namespace std;

struct Board {
    int tile[3][3];
    int g_value;
    int h_value;
    vector<char> path;
    struct Board* up;
    struct Board* down;
    struct Board* left;
    struct Board* right;

    Board(int arr[3][3]) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                tile[i][j] = arr[i][j];

        g_value = 0;
        h_value = 0;
        up = NULL;
        down = NULL;
        left = NULL;
        right = NULL;
    }

    Board(int arr[3][3], int previous_g_value, vector<char> previous_path) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                tile[i][j] = arr[i][j];

        g_value = previous_g_value + 1;
        h_value = 0;
        up = NULL;
        down = NULL;
        left = NULL;
        right = NULL;

        for (int i = 0; i < previous_path.size(); i++)
            path.push_back(previous_path[i]);
    }
};

/*
* Prints out the tiles of the board
*/
void print(Board *node) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (node->tile[i][j] == -1)
                cout << "# ";
            else
                cout << node->tile[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/*
* Moves the blank space in the direction specified
*/
bool move(Board *node, char direction) {
    int x = 0;
    int y = 0;

    for(int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (node->tile[i][j] == -1) {
                x = j;
                y = i;
            }

    switch (direction) {
    case 'u':
        if (y != 0) {
            int temp = node->tile[y][x];
            node->tile[y][x] = node->tile[y - 1][x];
            node->tile[y - 1][x] = temp;
            return true;
        }
        break;
    case 'd':
        if (y != 2) {
            int temp = node->tile[y][x];
            node->tile[y][x] = node->tile[y + 1][x];
            node->tile[y + 1][x] = temp;
            return true;
        }
        break;
    case 'l':
        if (x != 0) {
            int temp = node->tile[y][x];
            node->tile[y][x] = node->tile[y][x - 1];
            node->tile[y][x - 1] = temp;
            return true;
        }
        break;
    case 'r':
        if (x != 2) {
            int temp = node->tile[y][x];
            node->tile[y][x] = node->tile[y][x + 1];
            node->tile[y][x + 1] = temp;
            return true;
        }
        break;
    }

    return false;
}

/*
* This function calculates the number of misplaced tiles, h1
*/
int heuristicFunction1(Board* node) {
    int correct_node[3][3] = { { 1, 2, 3 },{ 8, -1, 4 },{ 7, 6, 5 } },
        sum = 0;

    for (int i = 0; i < 9; i++)
        for(int j = 0; j < 3; j++)
        if (node->tile[i][j] != correct_node[i][j])
            sum++;

    if (node->tile[1][1] != -1)
        sum--;
    return sum;
}

/*
* This function calculates the distance each misplaced tile is from its correct tile, h2
*/
int heuristicFunction2(Board *node) {
    int correct_node[3][3] = { { 1, 2, 3 },{ 8, -1, 4 },{ 7, 6, 5 } },
        sum = 0;

    for (int k = 1; k < 9; k++) {
        int current_x,
            current_y,
            correct_x,
            correct_y;

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                if (node->tile[i][j] == k) {
                    current_x = j;
                    current_y = i;
                }
                if (correct_node[i][j] == k) {
                    correct_x = j;
                    correct_y = i;
                }
            }

        sum += abs(correct_x - current_x);
        sum += abs(correct_y - current_y);
    }
    return sum;
}

/*
* This function calculates the number of tiles not in order around the edge of the board
*/
int heuristicFunction3(Board* node) {
    int sum = 0;

    for (int i = 0; i < 2; i++) {
        if (node->tile[i][0] + 1 % 8 != node->tile[i + 1][0] % 8 || node->tile[i][0] == -1 || node->tile[i + 1][0] == -1)
            sum += 2;

        if (node->tile[i][2] + 1 % 8 != node->tile[i + 1][2] % 8 || node->tile[i][2] == -1 || node->tile[i + 1][2] == -1)
            sum += 2;

        if (node->tile[2][2 - i] + 1 % 8 != node->tile[2][1 - i] % 8 || node->tile[2][2 - i] == -1 || node->tile[2][1 - i] == -1)
            sum += 2;

        if (node->tile[2 - i][0] + 1 % 8 != node->tile[1 - i][0] % 8 || node->tile[2 - i][0] == -1 || node->tile[1 - i][0] == -1)
            sum += 2;
    }

    if (node->tile[1][1] != -1)
        sum++;

    return sum;
}

int heuristicFunction4(Board *node)
{
    int sum = 0;
    int correct_node[3][3] = { { 1, 2, 3 },{ 8, -1, 4 },{ 7, 6, 5 } },
        goal[4] = {0,0,0,0};

    int reach[4] = {0,0,0,0},
        setgoal = 0;


    do
    {
        if(setgoal == 0)
            for(int y = 0; y < 3; y++)
            {
                int x = 0;
                goal[setgoal] = goal[setgoal] + correct_node[x][y];
                reach[setgoal] = reach[setgoal] + node->tile[x][y];
                }
        else if(setgoal == 1)
            for(int x = 0; x < 3; x++)
            {
                int y = 2;
                goal[setgoal] = goal[setgoal] + correct_node[x][y];
                reach[setgoal] = reach[setgoal] + node->tile[x][y];
            }
        else if(setgoal == 2)

            for(int y = 0; y < 3; y++)
            {
                int x = 2;
                goal[setgoal] = goal[setgoal] + correct_node[x][y];
                reach[setgoal] = reach[setgoal] + node->tile[x][y];
            }
        else if(setgoal == 3)
            for(int x = 0; x < 3; x++)
            {
            int y = 0;
            goal[setgoal] = goal[setgoal] + correct_node[x][y];
            reach[setgoal] = reach[setgoal] + node->tile[x][y];
            }
        else
            {
                cout << "Something's wrong." << endl;
                return 0;
            }

        if(reach[setgoal] != goal[setgoal])
            sum++;

        setgoal++;
    }while(setgoal < 4);

    return sum;
}

/*
* This function calculates for each column and row of the board how many tiles are not in their correct row and column
* There is no regard for the order they are in in the row, or whether they are in the spot, just whether the row and column contains them
*/
int heuristicFunction5(Board* node) {
    int correct_node[3][3] = { { 1, 2, 3 },{ 8, -1, 4 },{ 7, 6, 5 } },
        sum = 18;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++) {
                if (correct_node[i][j] == node->tile[i][k])
                    sum--;
                if (correct_node[j][i] == node->tile[k][i])
                    sum--;
            }

    return sum;
}

int heuristicFunction6(Board* node)
{
    int sum = 0;

    for (int i = 0; i < 2; i++)
    {
        if (node->tile[i][0] > node->tile[i + 1][0] || node->tile[i][0] == -1 || node->tile[i + 1][0] == -1)
            sum += 1;

        if (node->tile[i][2] > node->tile[i + 1][2] || node->tile[i][2] == -1 || node->tile[i + 1][2] == -1)
            sum += 1;

        if (node->tile[2][2 - i] > node->tile[2][1 - i] || node->tile[2][2 - i] == -1 || node->tile[2][1 - i] == -1)
            sum += 1;

        if (node->tile[2 - i][0] > node->tile[1 - i][0] || node->tile[2 - i][0] == -1 || node->tile[1 - i][0] == -1)
            sum += 1;
    }

    if (node->tile[1][1] != -1)
        sum += 1;

    return sum;
}

/*
* Checks if two boards contain the same tiles
*/
bool compareTwoBoard(Board* board1, Board* board2) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board1->tile[i][j] != board2->tile[i][j])
                return false;
    return true;
}

/*
* This function checks if the given successor board is equal to any of the Board in a list
*/
int compareSuccessorToList(Board *successor, vector<Board*> list) {
    for (int i = 0; i < list.size(); i++)
        if (compareTwoBoard(successor, list[i])) {
            return i;
        }

    return -1;
}

/*
* When a child is generated that is the same as one on close, and has a shorter path to that board, reset the g values and the path of it, and all its successors recursively
*/
void reorder_g_values(Board* node, int new_g_value, char direction, vector<char> previous_path) {
    if (node == NULL)
        return;

    node->g_value = new_g_value;

    if (direction != 's') {
        node->path = previous_path;
        node->path.push_back(direction);
    }

    reorder_g_values(node->up, new_g_value + 1, 'u', node->path);
    reorder_g_values(node->down, new_g_value + 1, 'd', node->path);
    reorder_g_values(node->left, new_g_value + 1, 'l', node->path);
    reorder_g_values(node->right, new_g_value + 1, 'r', node->path);

    return;
}

/*
* Responsible for all the logic of generating a child, and dealing with open and closed lists
*/
void generateChild(Board *best_node, vector<Board*> &open, vector<Board*> &close, char direction, int heuristic) {
    bool continue_ = true;
    Board *successor = new Board(best_node->tile, best_node->g_value, best_node->path);

    //OLD SAME IS INDEX OF OPEN WHERE THEY ARE EQUAL
    if (move(successor, direction)) {
        successor->path.push_back(direction);
        switch (heuristic)
        {
        case 1:
            successor->h_value = heuristicFunction1(successor);
            break;
        case 2:
            successor->h_value = heuristicFunction2(successor);
            break;
        case 3:
            successor->h_value = heuristicFunction3(successor);
            break;
        case 4:
            successor->h_value = heuristicFunction4(successor);
            break;
        case 5:
            successor->h_value = heuristicFunction5(successor);
            break;
        case 6:
            successor->h_value = heuristicFunction6(successor);
            break;
        default:
            break;
        }

        int old_same_index = compareSuccessorToList(successor, open);
        if (old_same_index != -1) {
            if (successor->g_value < open[old_same_index]->g_value) {
                Board* temp = open[old_same_index];
                open.erase(open.begin() + old_same_index);
                delete(temp);
                open.push_back(successor);

                switch (direction) {
                case 'u':
                    best_node->up = successor;
                    break;
                case 'd':
                    best_node->down = successor;
                    break;
                case 'l':
                    best_node->left = successor;
                    break;
                case 'r':
                    best_node->right = successor;
                }
            }
            else {
                delete(successor);
            }
            continue_ = false;
        }

        if (continue_) {
            old_same_index = compareSuccessorToList(successor, close);
            if (old_same_index != -1) {
                if (successor->g_value < close[old_same_index]->g_value) {
                    Board* temp = close[old_same_index];
                    close.erase(close.begin() + old_same_index);
                    delete(temp);
                    close.push_back(successor);

                    switch (direction) {
                    case 'u':
                        best_node->up = successor;
                        break;
                    case 'd':
                        best_node->down = successor;
                        break;
                    case 'l':
                        best_node->left = successor;
                        break;
                    case 'r':
                        best_node->right = successor;
                    }
                    reorder_g_values(successor, successor->g_value, 's', successor->path);
                }
                else {
                    delete(successor);
                }
                continue_ = false;
            }
        }

        if (continue_)
            open.push_back(successor);
    }
}

/*
* This function reorders each of the open states based on the function f(n) = g(n) + h(n)
*/
void reorderList(vector<Board*> list) {
    int size = list.size();
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++)
            if (list[j]->g_value + list[j]->h_value > list[j + 1]->g_value + list[j + 1]->h_value) {
                Board *store = list[j + 1];
                list[j] = list[j + 1];
                list[j + 1] = store;
            }
}

/*
* This is the main algorithm of the program, responsible finding the goal state in the shortest possible path
*/
Board* aStar(vector<Board*> &open, vector<Board*> &close, Board* goal_node, int heuristic) {

    cout << "Running Heuristic Function " << heuristic << endl << endl;

    while (open.size() != 0) {
        Board* best_node = open.front();
        open.erase(open.begin());

        if (compareTwoBoard(best_node, goal_node))
            return best_node;

        generateChild(best_node, open, close, 'u', heuristic);
        generateChild(best_node, open, close, 'd', heuristic);
        generateChild(best_node, open, close, 'l', heuristic);
        generateChild(best_node, open, close, 'r', heuristic);
        close.push_back(best_node);
        reorderList(open);
    }

    return NULL;
}

int heuristicHelper(Board *node, int heuristicalFunction) {
    switch (heuristicalFunction) {
    case 1:
        return heuristicFunction1(node);
    case 2:
        return heuristicFunction2(node);
    case 3:
        return heuristicFunction3(node);
    case 4:
        return heuristicFunction4(node);
    case 5:
        return heuristicFunction5(node);
    case 6:
        return heuristicFunction6(node);
        break;
    default:
        return 0;
    }
}

void printResults(Board* node, vector<Board*> open, vector<Board*> close, timeval begTime, timeval endTime, int runcount) {

    const double runtime = endTime.tv_sec - begTime.tv_sec + (endTime.tv_usec - begTime.tv_usec) / 1000000.0;

    cout << "Execution Time: " << runtime << endl;
    cout << "Nodes Generated: " << (open.size() + close.size()) << endl;
    cout << "Nodes Expanded: " << close.size() << endl;
    cout << "Depth of Tree: " << node->g_value << endl;
    cout << "Total Path: ";
    for (int i = 0; i < node->path.size(); i++)
        cout << node->path[i] << " ";
    cout << endl;

    Board* agent;
    int start_data[3][3] = { {2,8,3},{1,6,4},{-1,7,5} };
    int start_data2[3][3] = {{2, 8, 1},{-1, 4, 3},{7, 6, 5}};

    if (runcount == 1)
        agent = new Board(start_data);
    else
        agent = new Board(start_data2);
    print(agent);
    for (int i = 0; i < node->path.size(); i++) {
        move(agent, node->path[i]);
        print(agent);
    }
    delete(agent);
}

/*
* Resets list in between different heuristic tests and initial tests
*/
void clearList(vector<Board*> &list) {
    for (int i = 0; i < list.size(); i++)
        delete(list[i]);
    list.clear();
}

int main()
{
    int start_data[3][3] = {{2,8,3},{1,6,4},{-1,7,5}};
    //int start_data2[3][3] = {{2,1,6},{4,-1,8},{7,5,3}};
    int start_data2[3][3] = {{2, 8, 1},{-1, 4, 3},{7, 6, 5}};
    int goal_data[3][3] = { {1,2,3},{8,-1,4},{7,6,5} };

    timeval begTime, endTime;

    Board* start_node;

    for (int runcount = 1; runcount <=2; runcount++)
    {
        for (int i = 1; i <= 6; i++)
        {
            int heuristicFunctionSelect = i;
            if (runcount == 1)
                start_node = new Board(start_data);
            else
                start_node = new Board(start_data2);

            Board* goal_node = new Board(goal_data);
            start_node->h_value = heuristicHelper(start_node, heuristicFunctionSelect);
            vector<Board*> open;
            vector<Board*> close;
            open.push_back(start_node);

            gettimeofday(&begTime, NULL);

            Board* end = aStar(open, close, goal_node, heuristicFunctionSelect);

            gettimeofday(&endTime, NULL);

            printResults(end, open, close, begTime, endTime, runcount);
            clearList(open);
            clearList(close);
        }
    }

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
