//NEIL JACKSON 16390881
//PROJECT:
//GRAPH: BIPARTITE GRAPH
//ALGORITHM: MINIMISE DIAMETER

#include <iostream>
#include "Matrix09.h"
#include "Vector09.h"
#include <cmath>
#include <string>
#include <list>


using namespace std;

void get_inputs(int &N1,int &N2,double &p,double &q);
Matrix make_graph(int N1, int N2, double p, double q);
int prob_precision(double p, double q);
Matrix max_graph(Matrix Graph);
void get_diameter(Matrix Graph, int &start_node, int &end_node);
void minimise_diameter(Matrix &Graph, int start_node, int end_node);


int main()
{
    int N1, N2;
    double p,q;
    //get inputs from user
    get_inputs(N1,N2,p,q);
    int N = N1+N2;
    Matrix bipartite(N);
    //create bipartire graph
    bipartite = make_graph(N1,N2,p,q);
    bipartite.print();
    //get largest connected bipartite graph
    std::cout<< "Largest connected Graph" <<std::endl;

    Matrix max_bipartite = max_graph(bipartite);
    max_bipartite.print();

    //set nodes which will be altered
    int start_node = 0;
    int end_node = 0;
    //get diameter establishes the two nodes furthest away from each other
    get_diameter(max_bipartite, start_node, end_node);
    //minimies graph connects them
    minimise_diameter(max_bipartite, start_node,end_node);
    std::cout << std::endl << "Nodes connected: " << start_node << "," << end_node << std::endl;

    max_bipartite.print();

    return 0;
}

//FUNCTION get_inputs gets user inputs for graph making
void get_inputs(int &N1,int &N2,double &p,double &q) {

    //change values to those received by input, N1, N2, p,q
    //check if all values are correct range
    bool valid = false;

    while(!valid){
        valid = true;

        std::cout << "Please enter the size of N1 and N2: "<<std::endl << "N1: ";
        std::cin >> N1;
        if(cin.fail() || N1 <=0 || N1 !=(int)N1) {
            cin.clear();
            cin.ignore(256, '\n'); // ignores all input

            std::cout << "Incorrect input, please enter positive integers only!" << std::endl << std::endl;
            valid = false;
        }
    }
    valid = false;
    while(!valid){
        valid = true;
        std::cout << "N2: ";
        std::cin >> N2;
        if(cin.fail() || N2 <=0 || N2!=(int)N2) {
            cin.clear();
            cin.ignore(256,'\n');
            std::cout << "Incorrect input, please enter positive integers only!" << std::endl;
            valid = false;
        }
    }
    valid = false;
    while(!valid){
        valid = true;
        std::cout << "Please enter the probabilites p and q: "<<std::endl << "p: ";
        std::cin >> p;
        if(cin.fail() || p<0 || p>1) {
            cin.clear();
            cin.ignore(256, '\n');
            std::cout << "Incorrect range for p!" <<std::endl;
            valid = false;
            }
        }

    valid = false;
    while(!valid){
        valid = true;
        std::cout << "q: ";
        std::cin >> q;
        if(cin.fail() || p>q || q<0 || q>1 ){
            cin.clear();
            cin.ignore(256, '\n');
            if( p > q) {
                std::cout << "q must be greater than p!" <<std::endl;
            }
            else {
                std::cout << "Incorrect range for q" << std::endl;
            }
        valid = false;
        }
    }
}

//FUNCTION make_graph
Matrix make_graph(int N1, int N2, double  p,double q) {
    int N = N1+N2;
    Matrix Mat(N);
    Mat.zero();
    int prob_mult = pow(10, prob_precision(p,q));
    for(int i=1; i<N;i++){
        for(int j=0; j<i;j++){
            int rand_int = rand()%prob_mult; // random int between 0-10^(-1*log10(p or q)) uniformly distributed
            double prob;
            if((i< N1 && j< N1) || (i >= N1 && j >= N1)) { //check if a random integer is lower than that probability
               prob = p; //if both i and j are in N1 or N2, set prob to p, else q meaning they are in opposite "groups"
            }
            else{
                prob = q;
            }
            //check if probability (random int) is less than prob check (p,q) times the max of the interval of rand int
            if(rand_int < prob*prob_mult) {
                Mat.setij(i,j, 1.0);
                Mat.setij(j,i,1.0); //adjoint is symmetric since not directed so set both sides of the diagonal
            }
            else{
                Mat.setij(i,j,0.0);
                Mat.setij(j,i,0.0);
            }
        }
    }
    return Mat;
}

//Function: returns precision (amount of decimal place after '.') needed for calculating bound for probabiity
//parameters: double p, double q
//example: (0.01, 0.999) returns 3
//Max six digit precision
int prob_precision(double p, double q){
    string p_str, q_str;
    p_str = std::to_string(p); //converts double to string with only chars after '.' !!Warning!! C++ 11 only
    q_str = std::to_string(q);
    //format string from example (0.01000 -> 01)
    q_str = q_str.substr(q_str.find('.')+1);//removes everything before decimal point
    p_str = p_str.substr(p_str.find('.')+1);
    q_str.erase( q_str.find_last_not_of('0') + 1, std::string::npos ); //removes trailing zeros from to_string func
    p_str.erase( p_str.find_last_not_of('0') + 1, std::string::npos );

    int precision = max(q_str.length(), p_str.length());
    //returns max amount of digits after decimal place for p and q
    return precision;
}


//Function to return the largest connected graph, ie max graph
Matrix max_graph(Matrix Graph) {
    unsigned int N = Graph.size();

    //implement BFS algorithm to find those connected to first node
    //it visits nodes until all nodes are visited
    // if there are no nodes left, starts a new graph number on new node and goes again
    //visited array
    unsigned int visited[N];
    visited[0] = 1;
    for(unsigned int i=1;i<N;i++){
        visited[i] = 0; //all unvisited nodes set to zero
    }
    unsigned int graph_no = 1; // graph number
    //que list
    list<int> q;
    q.push_back(0); // push first node to q

    bool finished;
    do{
        while(!q.empty()){
            unsigned int current_node = q.front();
            q.pop_front();
            visited[current_node] = graph_no;
            //runs while there are still nodes to check in que
            for(unsigned int j=0;j<N;j++){
                if(Graph.getij(current_node,j) == 1 && visited[j] == 0){
                    visited[j] = graph_no; //set node to graph number in visited array
                    //if node is connected, add it to to queue
                    q.push_back(j);
                }
            }
        }
        finished = true;

        unsigned int check = 0;
        do {
            if(visited[check]==0){
                q.push_back(check);
                graph_no++;
                finished = false;

            }
            check++;
        }while(check<N && finished);

    }while(!finished);

    //now we have each node as position in visited, so we check which has highest amount of nodes
    unsigned int curr_max = 0;
    unsigned int max_hit = -1;
    unsigned int current = 0;
    unsigned int hit=0;
    while(current < N){
        for(unsigned int i = 0;i<N;i++){
            if(visited[i]==current){
                hit++;
            }
        }
        if(hit>curr_max){
            curr_max=hit;
            max_hit = current;
        }
        current++;
    }

    //once we know which graph number has the highest amount of nodes, we set everthing else to zero
    for(unsigned int i=1;i<N;i++){
        for (unsigned int j=0;j<i;j++){
            if(visited[j] != max_hit){
                Graph.setij(i,j,0);
                Graph.setij(j,i,0);
            }
        }
    }

    return Graph;
}


//Function finds diameter nodes of Matrix,
void get_diameter(Matrix Graph, int &start_node, int &end_node){
    /*
    To do this, pick a node (on largest connected graph)
    find node furthest away from it
    then start again and find node furthest away from that, those two nodes are our diameter nodes
    */
    unsigned int N = Graph.size();
    //to find diameter we find furthest node away from starting node, then find distance of furthest node from that
    //find starting node
    int start = -1;
    for(unsigned int i=0;i<N;i++){
        for(unsigned int j=0;j<i;j++){
            if(Graph.getij(i,j)==1){
                start = j;
            }
        }
    }
    if(start == -1){
        std::cout << "Error-get_diameter: no starting node" <<std::endl;
    }
    unsigned int visited[N];
    for(unsigned int i=0;i<N;i++){
        visited[i] = 0;
    }
    visited[start] = 1;

    list<int> q;
    q.push_back(start); // push first node to q
    unsigned int current_node = -1;
    while(!q.empty()){
            current_node = q.front();
            q.pop_front();
            visited[current_node] = 1;
            //runs while there are still nodes to check in que
            for(unsigned int j=0;j<N;j++){
                if(Graph.getij(current_node,j) == 1 && visited[j] == 0){
                    visited[j] = 1;
                    q.push_back(j);
                }
            }
        }

    start_node = current_node; //we have our start node (the last current node on previous search)
    //the diameter is the distance to the furthest node away
    for(unsigned int i=0;i<N;i++){
        visited[i] = 0;
    }
    start=current_node;
    q.push_back(start); // push first node to q

    while(!q.empty()){

            current_node = q.front();
            q.pop_front();
            visited[current_node] = 1;
            //runs while there are still nodes to check in queue
            for(unsigned int j=0;j<N;j++){
                if(Graph.getij(current_node,j) == 1 && visited[j] == 0){
                    visited[j] = 1; //set node 1 signifying its visited
                    q.push_back(j);
                }
            }
        }
    end_node = current_node; // and the last node on previous search is our end node
}

//connect the two nodes
void minimise_diameter(Matrix &Graph, int start_node, int end_node){
    Graph.setij(start_node,end_node,1);
    Graph.setij(end_node, start_node,1);
}


