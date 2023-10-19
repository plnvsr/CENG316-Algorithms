#include <iostream>
#include <algorithm>
#include <vector>
using std::cin;
using std::cout;
using std::endl;
using std::pair; 
using std::vector;


int heuristic(vector<int> v){
    //Add anchor 0 at the beginning and n+1 at the end
    int n = v.size();
    int nonadj = 0;
    int result;
    v.insert(v.begin(), 0);
    v.push_back(n+1);

    //calculate the number of non-adjacent pairs
    for(int i=0; i<=n; i++)
    {
        if(v[i] == v[i+1]+1 || v[i] == v[i+1]-1)
            continue;
        else
            nonadj++;
                   
    }

    result = (nonadj+1)/2;
    return result;
    
}

bool is_sorted(vector<int> v){
    for(unsigned int i=0; i<v.size(); i++){
        if(v[i] != i+1)
            return false;
    }
    return true; 
}


bool dfs(vector<int>& v, int depth, int limit, vector<pair<int, int>>& moves){
    int n = v.size();
    
    if(is_sorted(v))
        return true;
    
    else if(depth + heuristic(v) > limit)
        return false;
    
    else{   
        for(int i=0; i<n; i++)
        {
            for(int j=i+1; j<n; j++)
            {
                
                reverse(v.begin()+i, v.begin()+j+1);
                moves.push_back({i, j});

                if(is_sorted(v)){ //did this move fix v?
                    return true;
                }

                else{ // solution not found after making the move

                    //does this move lead to the solution?
                    if(dfs(v, depth+1, limit, moves)){
                        return true;
                    }

                    //this move does not lead to the solution, pop back
                    //and reverse v back
                    moves.pop_back();
                    reverse(v.begin()+i, v.begin()+j+1);
                }
            }
        }
        return false; //no solution found in this run
    }
}

vector<pair<int, int>> solver(vector<int> &v){
    vector<pair<int, int>> solution;
    for(int limit = 1; dfs(v, 0, limit, solution) == false; limit++){
        if(limit == 9)
            break;
    }
    return solution;
}


int main(){

    vector<int> v; 
    int num; 

    while(cin >> num){
        v.push_back(num);
    }
    
    
    vector<pair<int, int>> moves;
    moves = solver(v);
    cout << moves.size() << endl;
    for(unsigned int i=0; i<moves.size(); i++){
        cout << moves[i].first + 1 << " " << moves[i].second + 1 << endl;
    }
    

    return 0;
}