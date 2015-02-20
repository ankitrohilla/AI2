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

vector<vertex> vertices;
set<int> discoveredVertices;

vector< set<int> > subgraphs;

typedef set<int>::iterator setIntIterator;
typedef vector<int>::iterator vecIntIterator;
typedef vector< set<int> >::iterator vecSetIntIterator;

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

        while( *vIt < i && vIt != currentVertex.adjList.end() ) {
            leftVertices.push_back( *vIt );
            vIt++;
        }

        cout << endl;
        cout << i << " -> ";
        for_each( leftVertices.begin(), leftVertices.end(), [](int i){cout<<i;});

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
            subgraphs.push_back(temp);
        }

//        accomodate this vertex to all compatible sets
        for( vecSetIntIterator it = subgraphs.begin(); it != subgraphs.end(); it++ ) {
            bool isCompatible = all_of( (*it).begin(), (*it).end(), [&](int i){
                vecIntIterator sIt = find( currentVertex.adjList.begin(), currentVertex.adjList.end(), i);
                if( sIt != currentVertex.adjList.end() )
                    return true;
                else
                    return false;
            });

            if( isCompatible ) {
                for_each( (*it).begin(), (*it).end(), [&](int s) {
                    vecIntIterator vIt = find( leftVertices.begin(), leftVertices.end(), s);
                    if( vIt != leftVertices.end() )
                        leftVertices.erase( vIt );
                } );
                (*it).insert( i );
            }
        }


        cout << endl;
        cout << i << " -> ";
        for_each( leftVertices.begin(), leftVertices.end(), [](int i){cout<<i;});

//        for vertices with index lower than current one, create new sets until all are finished

    }
}
