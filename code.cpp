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


//        for vertices with index lower than current one, create new sets until all are finished
        vector< set<int> > newSets;
        while( !leftVertices.empty() ) {

            for_each( subgraphs.begin(), subgraphs.end(), [&](set<int> s){
                set<int> temp;
                temp.insert(i);
                for_each( s.begin(), s.end(), [&](int p){
                    vecIntIterator adjvIt = find( currentVertex.adjList.begin(), currentVertex.adjList.end(), p);
                    vecIntIterator lefvIt = find( leftVertices.begin(), leftVertices.end(), p);

                    if( adjvIt != currentVertex.adjList.end() ) {
                        temp.insert(p);
                        if( lefvIt != leftVertices.end() )
                            leftVertices.erase( lefvIt );
                    }

                });
                if( temp.size()>1 )
                    newSets.push_back(temp);
            });

            for_each( newSets.begin(), newSets.end(), [&](set<int> s){
                subgraphs.push_back( s );
            });
            newSets.clear();

        }

        cout << endl;
        cout << i << " -> ";
        for_each( leftVertices.begin(), leftVertices.end(), [](int i){cout<<i;});


        cout << "\nSets - ";
        for_each( subgraphs.begin(), subgraphs.end(), [](set<int> s) {
            cout << "{ ";
            for_each( s.begin(), s.end(), [](int i){cout << i << ",";});
            cout << "}, ";
        });

        for( vecSetIntIterator currentvsIt = subgraphs.begin(); currentvsIt != subgraphs.end()-1; currentvsIt++ ) {
            for( vecSetIntIterator comparevsIt = currentvsIt+1; comparevsIt != subgraphs.end(); comparevsIt++ ) {
                if( (*comparevsIt).size() < 2 ) {
                    goto DONE_ITERATION;
                }


                if( includes( (*currentvsIt).begin(), (*currentvsIt).end(), (*comparevsIt).begin(), (*comparevsIt).end() ) &&
                    includes( (*comparevsIt).begin(), (*comparevsIt).end(), (*currentvsIt).begin(), (*currentvsIt).end() ) ) {

                    cout << "\n2 sets -> \n";
                    cout << "{ ";
                    for_each( (*currentvsIt).begin(), (*currentvsIt).end(), [](int i){cout << i << ",";});
                    cout << "}, ";
                    cout << "{ ";
                    for_each( (*comparevsIt).begin(), (*comparevsIt).end(), [](int i){cout << i << ",";});
                    cout << "}";

                    cout << "\nDeleting comparevsIt { ";
                    for_each( (*comparevsIt).begin(), (*comparevsIt).end(), [](int i){cout << i << ",";});
                    cout << "}, ";

                    subgraphs.erase( comparevsIt );

                    comparevsIt--;

                    cout << "\ncomparevsIt after deletion { ";
                    for_each( (*comparevsIt).begin(), (*comparevsIt).end(), [](int i){cout << i << ",";});
                    cout << "}, ";

                }
            }
        }

        DONE_ITERATION:

        cout << "\nSets after duplicate removal - ";
        for_each( subgraphs.begin(), subgraphs.end(), [](set<int> s) {
            cout << "{ ";
            for_each( s.begin(), s.end(), [](int i){cout << i << ",";});
            cout << "}, ";
        });
    }

    cout << "\nNumber of sets - " << subgraphs.size();
    fflush(stdout);

//    remove subsets of sets
    bool isRedundancy = true;
    while( isRedundancy ) {

        isRedundancy = false;

//        compare each set to each set
        for( vecSetIntIterator currentvsIt = subgraphs.begin(); currentvsIt != subgraphs.end()-1; currentvsIt++ ) {
            cout << "\nChecking subsets for set number " << currentvsIt-subgraphs.begin();
            cout << " -> { ";
            for_each( (*currentvsIt).begin(), (*currentvsIt).end(), [](int i){cout << i << ",";});
            cout << "},";
            for( vecSetIntIterator comparevsIt = currentvsIt+1; comparevsIt != subgraphs.end(); comparevsIt++ ) {
                if( (*comparevsIt).size() < 2 ) {
                    goto DONE;
                }

                if( includes( (*currentvsIt).begin(), (*currentvsIt).end(), (*comparevsIt).begin(), (*comparevsIt).end() ) ) {

                    cout << "\n2 sets -> \n";
                    cout << "{ ";
                    for_each( (*currentvsIt).begin(), (*currentvsIt).end(), [](int i){cout << i << ",";});
                    cout << "}, ";
                    cout << "{ ";
                    for_each( (*comparevsIt).begin(), (*comparevsIt).end(), [](int i){cout << i << ",";});
                    cout << "}";


                    isRedundancy= true;

                    cout << "\nDeleting comparevsIt { ";
                    for_each( (*comparevsIt).begin(), (*comparevsIt).end(), [](int i){cout << i << ",";});
                    cout << "}, ";

                    subgraphs.erase( comparevsIt );

                    comparevsIt--;

                    cout << "\ncomparevsIt after deletion { ";
                    for_each( (*comparevsIt).begin(), (*comparevsIt).end(), [](int i){cout << i << ",";});
                    cout << "}, \n";


                } else if( includes( (*comparevsIt).begin(), (*comparevsIt).end(), (*currentvsIt).begin(), (*currentvsIt).end() ) ) {

                    cout << "\n2 sets -> \n";
                    cout << "{ ";
                    for_each( (*currentvsIt).begin(), (*currentvsIt).end(), [](int i){cout << i << ",";});
                    cout << "}, ";
                    cout << "{ ";
                    for_each( (*comparevsIt).begin(), (*comparevsIt).end(), [](int i){cout << i << ",";});
                    cout << "}";


                    isRedundancy= true;

                    cout << "\nDeleting currentvsIt { ";
                    for_each( (*currentvsIt).begin(), (*currentvsIt).end(), [](int i){cout << i << ",";});
                    cout << "}, \n";

                    subgraphs.erase( currentvsIt );
                }
            }
        }
    }

    DONE:

    cout << "\nSets - ";fflush(stdout);
    for_each( subgraphs.begin(), subgraphs.end(), [](set<int> s) {
        cout << "{ ";
        for_each( s.begin(), s.end(), [](int i){cout << i << ",";});
        cout << "}, ";
    });

    cout << "\nNumber of sets - " << subgraphs.size();
    fflush(stdout);
    cout << endl;

    // PRINT TO OUTPUT FILE
    for( int i = 0; i <  subgraphs.size(); i++ ) {
        cout << "\n#" << i+1 << " " << subgraphs[i].size() << endl;
        for( setIntIterator it = subgraphs[i].begin(); it != subgraphs[i].end(); it++ ) {
            if( it != subgraphs[i].begin() )
                cout << " ";
                cout << *it;
        }
    }

}
