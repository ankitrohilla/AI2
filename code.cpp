#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int n, e, k;

class vertex {
    public:

    int id;
    vector<int> adjList;
    vertex() {}
    vertex( int id ) {
        this->id = id;
    }

    void show() {
        cout << id << " - ";
        for_each( adjList.begin(), adjList.end(), [](int i){cout<<i<<" ";});
        cout << endl;
    }

};

//order on the basis on number of elements
bool order( set<int> s1, set<int> s2 ) {
    return s1.size() > s2.size();
}

vector<vertex> vertices;
set<int> discoveredVertices;

set< set<int>, bool(*)(set<int>,set<int>) > subgraphs(order);

typedef set<int>::iterator setIntIterator;
typedef vector<int>::iterator vecIntIterator;
typedef set< set<int>, bool(*)(set<int>,set<int>) >::iterator setSetIntIterator;

main() {

    // since vertices are indexed with 1
    vertices.push_back( *(new vertex(0)) );

    cin >> n >> e >> k;

    for( int i = 1; i <= n; i++ ) {
        vertex temp = *(new vertex(i));
        vertices.push_back( temp );
    }

    for( int i = 1; i <= e; i++ ) {
        int v1, v2;
        cin >> v1 >> v2;
        vertices[v1].adjList.push_back( v2 );
        vertices[v2].adjList.push_back( v1 );
    }

    for( int i = 1; i <= n; i++ ) {
        sort( vertices[i].adjList.begin(), vertices[i].adjList.end() );
    }

    for_each(vertices.begin(), vertices.end(), [](vertex v){v.show();});

    // START

    for( int i = 1; i <= n; i++ ) {

        vertex& currentVertex = vertices[i];
        vector<int> leftVertices;
        vecIntIterator vIt = currentVertex.adjList.begin();

        while( *vIt < i ) {
            leftVertices.push_back( *vIt );
            vIt++;
        }


//        add all its neighbour and itself to discovered set if not already
        bool isNewDiscovered = false;

        discoveredVertices.insert( i );

        for_each( currentVertex.adjList.begin(), currentVertex.adjList.end(), [&](int i){
            pair<setIntIterator, bool> p = discoveredVertices.insert( i );
            if( !isNewDiscovered )
                isNewDiscovered = p.second;
        });

//        if this vertex explored any vertex that has not been explored previously, create new set containing only this vertex
        if( isNewDiscovered ) {
            set<int> temp;
            temp.insert(i);
            subgraphs.insert(temp);
        }

//        accomodate this vertex to all compatible sets
        for( setSetIntIterator it = subgraphs.begin(); it != subgraphs.end(); it++ ) {
            bool isCompatible = all_of( (*it).begin(), (*it).end(), [&](int i){
                vecIntIterator sIt = find( currentVertex.adjList.begin(), currentVertex.adjList.end(), i);
                if( sIt != currentVertex.adjList.end() )
                    return true;
                else
                    return false;
            });

//            if( isCompatible )
//                (*it).insert( i );
        }

        (*subgraphs.begin()).insert(5);



//        for vertices with index lower than current one, create new sets until all are finished

    }
}
