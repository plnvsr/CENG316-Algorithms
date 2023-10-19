#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Grades{
    int label;
    int alpha;
    int beta;
};

bool compare_alpha(const Grades& s1, const Grades& s2){
    if(s1.alpha == s2.alpha)
        return s1.beta > s2.beta;
    return s1.alpha > s2.alpha;
}

bool compare_beta(const Grades& s1, const Grades& s2) {
    return s1.beta > s2.beta;
}

class PartialSums
{
private:
    vector<double> s;

    // Returns the least significant one bit (rightmost one bit)
    // of a size_t (some unsigned int type)
    // Logically, the parameter should be >= 1.
    // To avoid efficiency loss, we do not perform a check.
    static size_t lsb(size_t i)
    {
        // i - 1             : All zero bits on the right side and
        //                     the rightmost one bit are inverted.
        // i ^ (i - 1)       : Creates a mask representing the
        //                     inverted bits. (^ = XOR)
        // i & (i ^ (i - 1)) : Applies the mask back to i to select the
        //                     rightmost one bit.
        //
        // Example:
        // i                 : 10011000
        // i - 1             : 10010111
        // i ^ (i - 1)       : 00001111
        // i & (i ^ (i - 1)) : 00001000

        return i & (i ^ (i - 1));
    }

public:
    // Since Fenwick tree is best described on a 1-based array,
    // we allocate a vector of size (n + 1), wasting the first element
    // of the vector.
    PartialSums(size_t n) : s(n + 1, 0)
    {
    }

    // Returns the size of the data structure.
    // We use the size stored in the vector,
    // rather than storing it explicitly.
    // This avoids redundancy.
    size_t size() const
    {
        return s.size() - 1;
    }

    // Logically sets A[k] <-- A[k] + delta.
    void update(size_t k, double delta)
    {
        // Visit all nodes i whose range contains A[k].
        // Add delta to the value maintained in each.
        for (size_t i = k; i <= size(); i = i + lsb(i))
        {
            s[i] += delta;
        }
    }

    // Queries for A[1] + ... + A[k].
    double query(size_t k) const
    {
        double sum = 0.0f;

        // Visit the nodes i whose ranges make up
        // [1..k] when concatenated. Sum up their
        // values to compute A[1] + ... + A[k].
        for (size_t i = k; i > 0; i = i - lsb(i))
        {
            sum += s[i];
        }

        return sum;
    }

    void print(){
        for(int i=1; i<s.size(); i++){
            cout << s[i] << " ";
        }
        cout << endl;
    }
};

int main(){

    int n;
    cin >> n;

    vector<Grades> students(n);
    for(int i=0; i<n; i++){
        cin >> students[i].alpha >> students[i].beta;
        students[i].label = i+1;
    }

    vector<int> finalRankTable(n+1);

    vector<Grades> alphaRank(students);
    vector<Grades> betaRank(students);

    sort(alphaRank.begin(), alphaRank.end(), compare_alpha);
    sort(betaRank.begin(), betaRank.end(), compare_beta);


    vector<vector<Grades>> beta_RSR;
    vector<int> betaRankTable(n+1);


    int b_batchno = 0;

    for(int i=0; i<n; i++){
        vector<Grades> batch;
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



    PartialSums ft(beta_RSR.size());


    for(int i=0; i<n; i++){
        // use alpha as time
        int rank;
        int index = betaRankTable[alphaRank[i].label] + 1;
        rank = ft.query(index) + 1;
        finalRankTable[alphaRank[i].label] = rank;
        ft.update(index, 1);

    }

    for(int i=1; i<n+1; i++)
        cout << finalRankTable[i] << endl;


}