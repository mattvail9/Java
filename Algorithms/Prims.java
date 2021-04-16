/*
* Prim's Algorithm
* Finds minimum spaning tree (MST) for
* a weighted undirected graph
*/
import java.util.ArrayList;

public class Prims
{
	public static void main(String[] args){
		
		/* Connect verticies
		* 0----1
		* | \  /\
		* |  \/  \
		* 2---3---4
		*/
		
		//Adjacency matrix representing weighted edges between verticies
		int[][] connections = 
		{	
			  /*0  1  2  3  4*/	
	/*0*/	{ 0, 2, 4, 1, 0 },
	/*1*/	{ 2, 0, 0, 6, 3 },
	/*2*/	{ 4, 0, 0, 3, 0 },
	/*3*/	{ 1, 6, 3, 0, 5 },
	/*4*/	{ 3, 0, 0, 5, 0 } 
		};
		
		
		//Find the cheapest edge on the graph
		int start_vertex = 0; 				    //Arbitrarily choose 0 as start vertex
		int cheapest = Integer.MAX_VALUE;	//Set current cheapest edge to infinity*
		
		//checks every edge once for the cheapest weight
		for(int r = 0; r < connections.length; r++){
			for(int c = r; c < connections[r].length; c++){
				if(connections[r][c] != 0 && connections[r][c] < cheapest){
					start_vertex = r;
					cheapest = connections[r][c];
				} 
			}
		}
		
		System.out.printf("Start: %d Cheapest Edge: %d\n", start_vertex, cheapest);
		
		ArrayList<Integer> verticies = new ArrayList<Integer>();	//unvisited verticies
		for(int i = 0; i < connections.length; i++){
			if(i != start_vertex){
				verticies.add(i);
			}
		}
		ArrayList<String> minimum_spanning_tree = new ArrayList<String>();
		
		ArrayList<Integer> forest = new ArrayList<Integer>();
		forest.add(start_vertex);
		
		while( forest.size() < connections.length ){
			
			int cheapest_edge = Integer.MAX_VALUE;	//Set current cheapest edge to infinity*
			int chepest_neighbor = -1;	            //stores the cheapest edge value
			int vertex = -1;			                  //the vertex chosen from forest with cheapest edge
			
			for(int v : forest){		//loop over vertices already in mst
				for(int n = 0; n < connections.length; n++){
					int edge = connections[v][n];
          
          //if vertex is connected && does not form a cycle && is minimum edge...
					if(edge != 0 && !forest.contains(n) && edge < cheapest_edge){
            //update vlaues
						cheapest_edge = edge;
						vertex = v;
						chepest_neighbor = n;
					}
				}
			}
			forest.add(chepest_neighbor);	//add vertex with cheapest edge weight
			minimum_spanning_tree.add(vertex + " -> " + chepest_neighbor);
		}
		
		for(String s : minimum_spanning_tree){
			System.out.printf("%s\n", s);
		}
	}
}
