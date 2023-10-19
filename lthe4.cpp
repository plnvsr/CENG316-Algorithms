#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <limits>

int sweep;

struct Point{
    int x;
    int y;
};

struct Segment{
    std::string name;
    Point start_p;
    Point end_p;
    float m;
    float c;
};

struct Event{
    Point at;
    bool type; //start or endpoint
    Segment * belongs;
};


bool xCompare(const Event & first, const Event & second){
    return first.at.x < second.at.x;
}

auto setCompare = []( const Segment * first , const Segment * second ){
    // as new segments get added to the list
    // note that the addition will happen on second one's starting point
    // the tree will be constructed accordingly
    // this comparison needs to be mirror like, i.e. sweep should be global and they both might need to move
    // because i don't know which one is first and which one is second while comparing.

    float first_y, second_y;

    first_y = first->m*sweep + first->c;
    second_y = second->m*sweep + second->c;

    return first_y < second_y;


};

float distance(const Segment * first, const Segment * second, bool secondType){
    Point to_use;
    if(secondType)
        to_use = second->start_p;
    else
        to_use = second->end_p;

    float new_y = first->m * to_use.x + first->c;

    return std::abs(to_use.y - new_y);
}

int main() {
    int N;
    std::cin >> N;

    std::vector<Event> events;

    for(int i=0; i<N; i++){
        std::string name;
        int l1x, l1y, r1x, r1y;
        std::cin >> name >> l1x >> l1y >> r1x >> r1y;
        float slope = float(r1y-l1y)/float(r1x-l1x);
        float c_val = l1y - slope*l1x;
        Segment * seg = new Segment();
        seg->name = name;
        seg->start_p.x = l1x;
        seg->start_p.y = l1y;
        seg->end_p.x = r1x;
        seg->end_p.y = r1y;
        seg->m = slope;
        seg->c = c_val;
        events.push_back({{l1x, l1y}, true, seg});
        events.push_back({{r1x, r1y}, false, seg});

    }

    std::sort(events.begin(), events.end(), xCompare);

    std::string above, below;
    float min_dist = std::numeric_limits<float>::max();

    std::set <const Segment * , decltype(setCompare)> status( setCompare );

    for(auto &e : events){
        if(e.type)
            sweep = e.belongs->start_p.x;
        else
            sweep = e.belongs->end_p.x;

        if(e.type)
            status.insert(e.belongs);

        float dist;
        auto it = status.upper_bound(e.belongs);
        if (it != status.end()){
            dist = distance(*it, e.belongs, e.type);
            if(dist < min_dist){
                min_dist = dist;
                above = (*it)->name;
                below = e.belongs->name;
            }
        }

        it = status.lower_bound(e.belongs);
        if (it != status.begin()){
            it--;
            dist = distance(*it, e.belongs, e.type);
            if(dist < min_dist){
                min_dist = dist;
                above = e.belongs->name;
                below = (*it)->name;
            }
        }

        if(!e.type)
            status.erase(e.belongs);

    }

    std::cout << above << " " << below << std::endl;
    return 0;

}
