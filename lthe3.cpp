#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Solution{
    int label;
    int alpha;
    int beta;
    int gamma;
};


bool compare_alpha(const Solution& s1, const Solution& s2){
    return s1.alpha > s2.alpha;
}

bool compare_beta(const Solution& s1, const Solution& s2){
    if(s1.beta == s2.beta){
        return s1.gamma > s2.gamma;
    }
    return s1.beta > s2.beta;
}


class PartialMax {
private:
    vector<int> tree;

public:
    PartialMax(int size) {
        tree.resize(size + 1, 0);
    }

    static size_t lsb(size_t i)
    {
        return i & (i ^ (i - 1));
    }

    size_t size() const
    {
        return tree.size() - 1;
    }

    void update(size_t k, int delta)
    {
        // Visit all nodes i whose range contains A[k].
        // Choose max
        for (size_t i = k; i <= size(); i = i + lsb(i))
        {
            tree[i] = max(tree[i], delta);
        }
    }

    int query(size_t k) const
    {
        int maxVal = 0;

        for (size_t i = k; i > 0; i = i - lsb(i))
        {
            maxVal = max(maxVal, tree[i]);
        }

        return maxVal;
    }

    void print(){
        for(int i=0; i<tree.size(); i++){
            cout << tree[i] << " ";
        }
        cout << endl;
    }
};


int main(){
    int n;
    cin >> n;

    vector<Solution> solutions(n);

    for(int i=0; i<n; i++){
        cin >> solutions[i].alpha >> solutions[i].beta >> solutions[i].gamma;
        solutions[i].label = i+1;
    }


    vector<Solution> alphaRank(solutions);
    vector<Solution> betaRank(solutions);

    vector<int> betaRankTable(n+1);

    sort(alphaRank.begin(), alphaRank.end(), compare_alpha);
    sort(betaRank.begin(), betaRank.end(), compare_beta);

    vector<vector<Solution>> alpha_RSR;
    vector<vector<Solution>> beta_RSR;

    for(int i=0; i<n; i++){
        vector<Solution> batch;
        int current = alphaRank[i].alpha;
        batch.push_back(alphaRank[i]);
        if(i != n-1) {
            for (int j = i + 1; j < n; j++) {
                if (current == alphaRank[j].alpha) {
                    batch.push_back(alphaRank[j]);
                    i++;
                }
                else {
                    break;
                }
            }
        }
        alpha_RSR.push_back(batch);
    }

    int b_batchno = 0;

    for(int i=0; i<n; i++){
        vector<Solution> batch;
        int current = betaRank[i].beta;
        batch.push_back(betaRank[i]);
        betaRankTable[betaRank[i].label] = b_batchno;
        if(i != n-1) {
            for (int j = i + 1; j < n; j++) {
                if (current == betaRank[j].beta) {
                    batch.push_back(betaRank[j]);
                    betaRankTable[betaRank[j].label] = b_batchno;
                    i++;
                }
                else {
                    break;
                }
            }
        }
        beta_RSR.push_back(batch);
        b_batchno++;
    }

    for(auto & item : alpha_RSR){
       sort(item.begin(), item.end(), compare_beta);
    }

    vector<int> paretoOptimalLabels;

    PartialMax ft(n);


    for(int i=0; i<alpha_RSR.size(); i++){
        //operating on alpha batch
        vector<bool> batch_updates(n, false);
        for(int j=0; j<alpha_RSR[i].size(); j++){
            int index = betaRankTable[alpha_RSR[i][j].label]+1;
            int prevMax = ft.query(index);

            if(alpha_RSR[i][j].gamma > prevMax){
                ft.update(index, alpha_RSR[i][j].gamma);
                paretoOptimalLabels.push_back(alpha_RSR[i][j].label);
                batch_updates[index] = true;
            }
            else if(alpha_RSR[i][j].gamma == prevMax){
                if(prevMax){
                    if(batch_updates[index]){
                        paretoOptimalLabels.push_back(alpha_RSR[i][j].label);
                    }
                }
            }
            else{}
        }
    }


    sort(paretoOptimalLabels.begin(), paretoOptimalLabels.end());


    for(auto l : paretoOptimalLabels)
        cout << l << endl;



    return 0;
}