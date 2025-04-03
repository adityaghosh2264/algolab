/*Aditya Ghosh
23MA10072
Assignment 9*/
#include<iostream>
#include <limits.h>
using namespace std;
/* struct for an adjacency list node */
struct Node {
    int vertex;
    int weight;
    struct Node* next;
};
struct Graph{
    int V;      //stores number of vertices
    Node** array;
};
struct edge{
    int u;
    int v;
    int weight;
};
int abs(int num){ // calculates the absolute value of a number
    if(num<0) num = -num;
    return num;
}
/* create a new adjacency list node */
Node* create_node(int vertex, int weight) {
    Node* temp = new Node[1];
    temp->vertex = vertex;
    temp->weight = weight;
    temp->next = NULL;
    return temp;
}

/* creating a graph with v vertices */
Graph* create_graph(int V) {
    Graph* graph = new Graph[1];
    graph->V = V;
    graph->array = new Node*[V+1];
    for (int i = 1; i <= V; i++) graph->array[i] = nullptr;
    return graph;
}

/*adding an edge to the graph */
void addedge(Graph* graph, int u, int v, int weight) {
    // add edge from u to v
    Node* temp = create_node(v, weight);
    temp->next = graph->array[u];
    graph->array[u] = temp;
    
    // add edge from v to u since this is an undirected graph
    temp = create_node(u, weight);
    temp->next = graph->array[v];
    graph->array[v] = temp;
}
void brutedfs(Graph* graph, int cur, int skip_u, int skip_v, int* vis, int* vertW, int* comp_sum) {
    vis[cur] = 1;
    *comp_sum += vertW[cur];
    
    Node* temp = graph->array[cur];
    while (temp != NULL) {
        int adj = temp->vertex;
        // skip the edge if it is the excluded edge
        if ((cur == skip_u && adj == skip_v) || (cur == skip_v && adj == skip_u)) {
            temp = temp->next;
            continue;
        }
        if (!vis[adj]) {
            brutedfs(graph, adj, skip_u, skip_v, vis, vertW, comp_sum);
        }
        temp = temp->next;
    }
}
/*in the brute force implementation, we perform a fresh dfs for every edge removal and then calculate the sum of weights for one of the components. 
The sum of weights of the other component can be obtained by using total_sum. Performing a dfs for every edge removal is O(n) which is 
performed for n-1 edges hence the brute force approach is O(n^2)*/
void brute_force(Graph* graph, edge* edges, int num_edge, int n, int* vertW, int total_sum) {
    int max_vul = INT_MIN;
    int ans_u = -1, ans_v = -1;
    
    for (int i = 0; i < num_edge; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int weight = edges[i].weight;
        //calloc has been used since it initialises all values to 0 by default which is required initially for the vis array
        int* vis = (int*)calloc(n + 1, sizeof(int)); //vis array for dfs
        int comp_sum = 0;
        brutedfs(graph, u, u, v, vis, vertW, &comp_sum); // start dfs from u while excluding the edge (u, v)
        delete[] vis;
        
        int other_sum = total_sum - comp_sum;
        int diff = abs(comp_sum - other_sum);

        int vul = weight - diff;
        
        if (vul > max_vul) {
            max_vul = vul;
            ans_u = u;
            ans_v = v;
        }
    }
    
    cout<<"Edge with the largest vulnerability computed using first method: "<<ans_u<<" "<<ans_v<<"\n";
}

/*linear time algorithm implementation:
We choose an arbitrary root (vertex 1) and compute the sum of the weights in every subtree. For each edge (u,v) where v is a child of u, one component has sum = S (subtree sum of v) and
the other component has sum = total_sum - S. Unlike the brute force approach this helps us in finding all required subtree sums in one go rather than
performing dfs for every edge removal*/
int dfs(Graph* graph, int u, int parent, int* vertW, int total_sum, int* max_vul, int* max_u, int* max_v) {
    int subtree_sum = vertW[u];
    Node* temp = graph->array[u];
    while (temp != NULL) {
        int v = temp->vertex;
        int edge_weight = temp->weight;
        if (v == parent) {
            temp = temp->next;
            continue;
        }
        int child_sum = dfs(graph, v, u, vertW, total_sum, max_vul, max_u, max_v);
        int diff = abs(child_sum - (total_sum - child_sum));
        int vul = edge_weight - diff;
        if (vul > *max_vul) {
            *max_vul = vul;
            *max_u = u;
            *max_v = v;
        }
        subtree_sum += child_sum;
        temp = temp->next;
    }
    return subtree_sum;
}

int main() {
    //freopen("input.txt","r",stdin);
    int n;
    cout<<"Write n: ";
    cin>>n;
    
    int* vertW = new int[n+1]; // allocate vertex weights array (1-indexed)
    cout<<"Write weights of vertices: "; //input the weight array
    for(int i = 1; i <= n; i++) cin>>vertW[i];
    
    int num_edge = n - 1;
    edge* edges = new edge[num_edge];
    
    Graph* graph = create_graph(n);
    
    cout<<"Write the edges and their weights \n"; 
    for(int i = 0; i < num_edge; i++) {
        int u, v, weight;
        cin>>u>>v>>weight;
        edges[i].u = u;
        edges[i].v = v;
        edges[i].weight = weight;
        addedge(graph, u, v, weight);
    }
    
    // compute sum of all vertex weights
    int total_sum = 0;
    for (int i = 1; i <= n; i++) {
        total_sum += vertW[i];
    }
    
    // Compute using brute-force method
    brute_force(graph, edges, num_edge, n, vertW, total_sum);
    
    // linear time implementation
    int max_vul = INT32_MIN;
    int ans_u = -1, ans_v = -1;
    // start DFS from vertex 1 and calculate the subtree sum for all vertices
    dfs(graph, 1, -1, vertW, total_sum, &max_vul, &ans_u, &ans_v);
    cout<<"Edge with the largest vulnerability computed using second method: "<<ans_u<<" "<<ans_v<<"\n";
    
    /* Free allocated memory */
    delete[] edges;
    delete[] graph;
    delete[] vertW;

    return 0;
}