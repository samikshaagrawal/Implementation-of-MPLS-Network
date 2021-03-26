// tinA1Q1.cpp : Defines the entry point for the console application.
//
#include<bits/stdc++.h>
#include "stdio.h"
#include "iostream"
#include "string.h"
#include "string"
#include "iostream"
#include "fstream"

using namespace std;

#define ld long double
#define parameters 5
#define max_no_of_connections 200
#define max_no_of_nodes 30

void storeTopologyFile(string filename, ld topology[max_no_of_nodes][max_no_of_nodes][2], int &nodes, int &edges)
{
	ifstream file1;
	int source, destination;
	ld distance, capacity;
	filename+=".txt";
	//cout<<filename<<endl;
	file1.open(filename);
	if(!file1)
	{
		cout<< "file does not exist"<<endl;
		file1.clear();
		exit(1);
	}
	file1>>nodes>>edges;
	//cout<<nodes<<" "<<edges<<endl;
	for(int i=0; i<edges; i++)
	{
		file1>>source>>destination>>distance>>capacity;
		topology[source][destination][0] = distance;
		topology[destination][source][0] = distance;
        topology[source][destination][1] = capacity;
        topology[destination][source][1] = capacity;
	}
	file1.close();
	// printing the read topology
    /*for(int i = 0; i < nodes; i++)
    {
        for(int j = 0; j < nodes; j++)
        {
        	 cout<<i<<" "<<j<<" "<<topology[i][j][0]<<" "<<topology[i][j][1]<<endl;
        }
    }*/
}

void floydWarshallShortestPath(int &nodes, ld distMetric[][max_no_of_nodes], ld pathParentNode[][max_no_of_nodes])
{
	for(int k=0;k<nodes;k++)
	{
		for(int i=0;i<nodes;i++)
		{
			for(int j=0; j<nodes; j++)
			{
				if (distMetric[i][k] + distMetric[k][j] < distMetric[i][j])
                {  
                    distMetric[i][j] = distMetric[i][k] + distMetric[k][j];
                    pathParentNode[i][j] = pathParentNode[k][j];
					//cout<<distMetric[i][j]<<" "<<pathParentNode[i][j]<<endl;
                }
			}
		}
	}
}

void storeConnectionFile(string filename, ld conn[][parameters], int &no_of_connections)
{
	ifstream file1;
	filename+=".txt";
	file1.open(filename);
	if(!file1)
	{
		cout<< "file does not exist"<<endl;
		file1.clear();
		exit(1);
	}
	file1>>no_of_connections;
	cout<<no_of_connections<<endl;
	int source, destination;
	ld bMin, bAvg, bMax;
	for(int i=0; i<no_of_connections; i++)
	{
		file1>>source>>destination>>bMin>>bAvg>>bMax;
		//cout<<source<<" "<<destination<<" "<<bMin<<" "<<bAvg<<" "<<bMax<<endl;
		conn[i][0] = source;
		conn[i][1] = destination;
        conn[i][2] = bMin;
		conn[i][3] = bAvg;
		conn[i][4] = bMax;
	}
	file1.close();
	// printing the read topology
   /* for(int k = 0; k < no_of_connections; k++)
    {
        for(int j = 0; j < 5; j++)
        {
        	cout<<conn[k][j]<<" ";
        }
        printf("\n");
    }*/
}

void initialize_distMetric_and_pathParentNode(ld distMetric[][max_no_of_nodes], ld pathParentNode[][max_no_of_nodes], ld topology[max_no_of_nodes][max_no_of_nodes][2], int &nodes, string flag)
{
	for(int i=0; i<nodes; i++)
	{
		for(int j=0; j<nodes;j++)
		{
			if(i==j)
			{
				pathParentNode[i][j]=-1;
				distMetric[i][j]=0;
			}
			else if(topology[i][j][0]!=-1)
			{
				const char *temp = flag.c_str();
				if(strcmp(temp, "hop")==0)
					distMetric[i][j]=1;
				else
					distMetric[i][j]=topology[i][j][0];
				pathParentNode[i][j]=i;
			}
			else
			{
				pathParentNode[i][j]=-1;
				distMetric[i][j]=INT_MAX;
			}
			//cout<<distMetric[i][j]<<" "<<pathParentNode[i][j]<<endl; 
		}
	}
}

void write_shortest_fw_path(int shortest_fw_path[][max_no_of_nodes][max_no_of_nodes], string rank, ld pathParentNode[][max_no_of_nodes], int nodes, int source, int dest)
{
	ofstream file1;
	int count_nodes_in_path, parent_node=0;
	string filename =  "shortest_path"+rank+".txt";
	file1.open(filename);
	if(rank=="1")
	{
		for(int j=0; j<nodes; j++)
	    {
			for(int i=0; i<nodes;i++)
			{
				count_nodes_in_path=0;
				if(i!=j)
				{
					file1<<i<<" "<<j<<endl;
					parent_node=pathParentNode[j][i];
					while(parent_node!=-1)
					{
						shortest_fw_path[i][j][count_nodes_in_path]=parent_node;
						count_nodes_in_path+=1;
						file1<<parent_node<<" ";
						parent_node=pathParentNode[j][parent_node];
					}
					file1<<endl;
				
				}
				else
					shortest_fw_path[i][j][count_nodes_in_path]=-1;
			}
		}
	}
	else
	{
		int i=source, j=dest; 
		count_nodes_in_path=0;
					file1<<i<<" "<<j<<endl;
					parent_node=pathParentNode[i][j];
					while(parent_node!=-1)
					{
						shortest_fw_path[j][i][count_nodes_in_path]=parent_node;
						count_nodes_in_path+=1;
						file1<<parent_node<<" ";
						parent_node=pathParentNode[i][parent_node];
					}
					file1<<endl;
				
	}
	file1.close();
}

void delete_sp(int source, int destination,ld distMetric[][max_no_of_nodes], ld pathParentNode[][max_no_of_nodes], int shortest_fw_path1[][max_no_of_nodes][max_no_of_nodes])
{
	int src=source,dest=shortest_fw_path1[source][destination][0], i=0;
	while(dest!=-1)
	{
		pathParentNode[src][dest]=-1;
		pathParentNode[dest][src]=-1;
		distMetric[src][dest]=INT_MAX;
		distMetric[dest][src]=INT_MAX;
		i++;
		src=dest;
		dest=shortest_fw_path1[source][destination][i];
	}
}

void write_rt(ld topology[][max_no_of_nodes][2], int shortest_path1[][max_no_of_nodes][max_no_of_nodes],int shortest_path2[][max_no_of_nodes][max_no_of_nodes],string flag, int nodes, string filename)
{
	filename+=".txt";
	ofstream file1;
	int count=0,prev,current;
	file1.open(filename);
	for(int i=0;i<nodes;i++)
	{
		for(int j=0;j<nodes;j++)
		{
			if(i==j)
				continue;
			prev=i;count=0;
			ld cost_dist=0;
			current=shortest_path1[i][j][count];
			file1<<"Source = "<<i<<" Destination = "<<j<<endl;
			file1<<"Shortest Path 1: ";
			while(current!=-1)
			{
				file1<<current<<" ";
				cost_dist+=topology[prev][current][0];
				prev=current;
				count++;
				current=shortest_path1[i][j][count];

			}
			file1<<endl;
			if(flag=="hop")
				file1<<"Path Cost is "<<count<<endl;
			else
				file1<<"Path Cost is "<<cost_dist<<endl;

			prev=i;count=0;
			cost_dist=0;
			current=shortest_path2[i][j][count];
			file1<<"Shortest Path 2: ";
			while(current!=-1)
			{
				file1<<current<<" ";
				cost_dist+=topology[prev][current][0];
				prev=current;
				count++;
				current=shortest_path2[i][j][count];

			}
			file1<<endl;
			if(flag=="hop")
				file1<<"Path Cost is "<<count<<endl;
			else
				file1<<"Path Cost is "<<cost_dist<<endl;

		}
	}
	file1.close();

}

void decreased_capacity(int shortest_fw_path[][max_no_of_nodes][max_no_of_nodes], ld bRes, int source, int destination, ld topology[][max_no_of_nodes][2])
{
	int prev, curr, count=0;
	prev=source;
	curr=shortest_fw_path[source][destination][count];
	while(prev!=destination)
	{
		topology[prev][curr][1]-=bRes;
		topology[curr][prev][1]-=bRes;
		prev=curr;
		count++;
		curr=shortest_fw_path[source][destination][count];
	}
}

void establishConnections(ld connections[][parameters], int  shortest_fw_path1[][max_no_of_nodes][max_no_of_nodes], int shortest_fw_path2[][max_no_of_nodes][max_no_of_nodes], int nodes, int no_of_connections, int p, ld topology[][max_no_of_nodes][2], int fw_table[][max_no_of_connections][parameters], string output, string fw_file, string flag, string pathfile)
{
	int s=0;
	int source, destination, successfulConnections=0, vci=300;
	ld bMin, bAvg, bMax, bRes;
	int node_conn_no[max_no_of_connections]={0};
	ofstream file1;
		output+=".txt";
		file1.open(output);
	for(int i=0;i<no_of_connections;i++)
	{
		int last_vci=vci;
		source=connections[i][0];
		destination=connections[i][1];
		bMin=connections[i][2];
		bAvg=connections[i][3];
		bMax=connections[i][4];
		if(p==0)
		{
			bRes= bAvg+0.25*(bMax-bMin);
			if(bMax<bRes)
				bRes=bMax;
		}
		else
			bRes= bMax;
		int prev=source, count=0, curr=shortest_fw_path1[source][destination][count],flag1=0,flag2=0;
		while(prev!=destination)
		{
			if(topology[prev][curr][1]>=bRes)
			{
			}
			else
			{
				flag1=1;
				break;
			}
			prev=curr;
			count++;
			curr=shortest_fw_path1[source][destination][count];
		}
		if(flag1==1)
		{
			prev=source;count=0;curr=shortest_fw_path2[source][destination][count];
			while(prev!=destination)
			{
				if(topology[prev][curr][1]>=bRes)
				{}
				else
				{
					flag2=1;break;
				}
				prev=curr;
				count++;
				curr=shortest_fw_path2[source][destination][count];
			}
		}
		int path;
		if(flag2==1)
		{
			s++;
			continue;
		}
		else
		{

			successfulConnections++;
			int interface_matrix[max_no_of_nodes][max_no_of_nodes];
			for(int m=0;m<nodes;m++)
			{
				for(int n=0;n<nodes;n++)
				{
					interface_matrix[m][n]=n;
				}
			}
			if(flag1==0)
			{
				path=1;
				decreased_capacity(shortest_fw_path1,bRes,source,destination, topology);
			}
			else
			{
				path=2;
				decreased_capacity(shortest_fw_path2,bRes,source,destination, topology);
			}
			
			int count1=0,temp;
			if(path==1)
			{
				prev=source;
				//node_conn_no[prev]++;
				curr=shortest_fw_path1[source][destination][count1];
				fw_table[prev][node_conn_no[prev]][0]=-2;
				fw_table[prev][node_conn_no[prev]][1]=-2;
				fw_table[prev][node_conn_no[prev]][2]=interface_matrix[prev][curr];
				fw_table[prev][node_conn_no[prev]][3]=vci;
				node_conn_no[prev]++;
				while(curr!=destination)
				{
				
					temp=interface_matrix[prev][curr];
					prev=curr;count1++;
					curr=shortest_fw_path1[source][destination][count1];
					fw_table[prev][node_conn_no[prev]][0]=temp;
					fw_table[prev][node_conn_no[prev]][1]=vci;
					vci++;
					fw_table[prev][node_conn_no[prev]][2]=interface_matrix[prev][curr];
					fw_table[prev][node_conn_no[prev]][3]=vci;
					node_conn_no[prev]++;
				}
				temp=interface_matrix[prev][curr];
				fw_table[curr][node_conn_no[curr]][0]=temp;
				fw_table[curr][node_conn_no[curr]][1]=vci;
				vci++;
				fw_table[curr][node_conn_no[curr]][2]=-2;
				fw_table[curr][node_conn_no[curr]][3]=-2;
				node_conn_no[curr]++;
			}
			else
			{
				count1=0;
				prev=source;
				
				curr=shortest_fw_path2[source][destination][count1];
				fw_table[prev][node_conn_no[prev]][0]=-2;
				fw_table[prev][node_conn_no[prev]][1]=-2;
				fw_table[prev][node_conn_no[prev]][2]=interface_matrix[prev][curr];
				fw_table[prev][node_conn_no[prev]][3]=vci;
				node_conn_no[prev]++;
				while(curr!=destination)
				{
					temp=interface_matrix[prev][curr];
					prev=curr;count1++;
					curr=shortest_fw_path2[source][destination][count1];
					fw_table[prev][node_conn_no[prev]][0]=temp;
					fw_table[prev][node_conn_no[prev]][1]=vci;
					vci++;
					fw_table[prev][node_conn_no[prev]][2]=interface_matrix[prev][curr];
					fw_table[prev][node_conn_no[prev]][3]=vci;
					node_conn_no[prev]++;
				}
				temp=interface_matrix[prev][curr];
				fw_table[curr][node_conn_no[curr]][0]=temp;
				fw_table[curr][node_conn_no[curr]][1]=vci;
				fw_table[curr][node_conn_no[curr]][2]=-2;
				fw_table[curr][node_conn_no[curr]][3]=-2;
				node_conn_no[prev]++;
			}

		}
    		file1<<"connection : "<<i<<" source : "<<source<<" destination : "<< destination<<" Label list :  ";
			while(last_vci!=vci)
			{
				file1<<last_vci<<" ";
				last_vci++;
			}
			
			int count5=0;
			prev=source;
			ld cost_dist=0;
			if(path==1)
			{	curr=shortest_fw_path1[source][destination][count5];
				while(curr!=-1)
				{
					//file1<<"-"<<curr<<endl;
					cost_dist+=topology[prev][curr][0];
					prev=curr;
					count5++;
					curr=shortest_fw_path1[source][destination][count5];
				}
				file1<<" Path cost : ";
				if(flag=="hop")
					file1<<count5<<endl;
				else
					file1<<cost_dist<<endl;
			}
			else
			{
				curr=shortest_fw_path2[source][destination][count5];
				while(curr!=-1)
				{
					//file1<<"-"<<curr<<endl;
					cost_dist+=topology[prev][curr][0];
					prev=curr;
					count5++;
					curr=shortest_fw_path2[source][destination][count5];
				}
				file1<<" Path cost : ";
				if(flag=="hop")
					file1<<count5<<endl;
				else
					file1<<cost_dist<<endl;
		}
		
	}
	file1.close();
	pathfile+=".txt";
	file1.open(pathfile);
	cout<<"Connections Requested: "<<no_of_connections<<"  Connections Admitted : "<<successfulConnections<<endl;
	file1<<"Connections Requested: "<<no_of_connections<<"  Connections Admitted : "<<successfulConnections<<endl;
	file1.close();
	fw_file+=".txt";
	file1.open(fw_file);
	for(int i=0;i<nodes;i++)
	{
		file1<<"Node : "<<i<<endl;
		file1<<"Interface_in    Label_in      Interface_out     Label_out"<<endl;
		for(int j=0; j<node_conn_no[i];j++)
		{   
			if(fw_table[i][j][0]!=-1)
			file1<<fw_table[i][j][0]<<"             "<<fw_table[i][j][1]<<"             "<<fw_table[i][j][2]<<"              "<<fw_table[i][j][3]<<endl;
		}
		file1<<endl;
	}

	/*for(int i=0;i<nodes;i++)
	{
		cout<<"Node : "<<i<<endl;
		cout<<"Interface_in    Label_in      Interface_out     Label_out"<<endl;
		for(int j=0; j<node_conn_no[i];j++)
		{   
			cout<<fw_table[i][j][0]<<"             "<<fw_table[i][j][1]<<"             "<<fw_table[i][j][2]<<"              "<<fw_table[i][j][3]<<endl;
		}
		file1<<endl;
	}
*/
	file1.close();
	
}

int main(int argc,char ** argv)
{
	
	int nodes, edges, no_of_connections;
	string flag(argv[12]);
	//string flag1=string(flag);
	ld topology[max_no_of_nodes][max_no_of_nodes][2];
	ld distMetric1[max_no_of_nodes][max_no_of_nodes], pathParentNode1[max_no_of_nodes][max_no_of_nodes];
	ld distMetric2[max_no_of_nodes][max_no_of_nodes], pathParentNode2[max_no_of_nodes][max_no_of_nodes];
	for(int i = 0; i <max_no_of_nodes; i++)
    {
        for(int j = 0; j < max_no_of_nodes; j++)
        {
        	topology[i][j][0]=topology[i][j][1]=-1;
        }
    }
	memset(distMetric1, -1, sizeof(distMetric1));
	memset(distMetric2, -1, sizeof(distMetric2));
	memset(pathParentNode1, -1, sizeof(pathParentNode1));
	memset(pathParentNode1, -1, sizeof(pathParentNode1));
	ld connections[max_no_of_connections][parameters];
	string s(argv[2]);
	//cout<<argv[5]<<endl;
	//ecout<<s<<endl;
	storeTopologyFile(s, topology,  nodes, edges );
	initialize_distMetric_and_pathParentNode(distMetric1, pathParentNode1, topology,nodes,flag);
	initialize_distMetric_and_pathParentNode(distMetric2, pathParentNode2, topology,nodes,flag);
	/*for(int m=0;m<14;m++)
			{
				for(int n=0;n<14;n++)
					cout<<distMetric2[m][n]<<" ";
				cout<<endl;
			}
	*/
	floydWarshallShortestPath(nodes, distMetric1, pathParentNode1);
	/*for(int i=0;i<nodes;i++)
	{
		for(int j=0;j<nodes;j++)
		{
			cout<<distMetric1[i][j]<<" ";
		}
		cout<<endl;
	}*/
	int shortest_fw_path1[max_no_of_nodes][max_no_of_nodes][max_no_of_nodes];
	int shortest_fw_path2[max_no_of_nodes][max_no_of_nodes][max_no_of_nodes];
	memset(shortest_fw_path1, -1, sizeof(shortest_fw_path1));
	memset(shortest_fw_path2, -1, sizeof(shortest_fw_path2));
	//cout<<"Hello2"<<endl;
	write_shortest_fw_path(shortest_fw_path1, "1", pathParentNode1, nodes,0,0);
	for(int i=0;i<nodes;i++)
	{
		for(int j=0;j<nodes;j++)
		{
			if(i!=j)
			{	
				delete_sp(i,j,distMetric2, pathParentNode2, shortest_fw_path1);
				floydWarshallShortestPath(nodes, distMetric2, pathParentNode2);
				/*for(int m=0;m<14;m++)
				{
					for(int n=0;n<14;n++)
						cout<<distMetric2[m][n]<<" ";
					cout<<endl;
				}
				cout<<endl;
				*/
				write_shortest_fw_path(shortest_fw_path2, "2", pathParentNode2, nodes,i,j);
				initialize_distMetric_and_pathParentNode(distMetric2, pathParentNode2, topology,nodes,flag);
			}
		}
	}
	string routing_file(argv[6]);
	write_rt(topology, shortest_fw_path1, shortest_fw_path2, flag, nodes, routing_file);
	string conn_file(argv[4]);
	storeConnectionFile(conn_file,connections ,no_of_connections);
	/*for(int m=0;m<nodes;m++)
			{
				for(int n=0;n<nodes;n++)
				{
					int l=0;
					cout<<m<<" "<<n<<endl;
					while(shortest_fw_path2[m][n][l]!=-1)
					{
						cout<<shortest_fw_path2[m][n][l]<<" ";
						l++;
					}
					cout<<endl;
				}
			}*/
				
    //const char* res=argv[14].c_str();
	int p=atoi(argv[14]);
	int fw_table[max_no_of_nodes][max_no_of_connections][parameters];
	memset(fw_table, 0, sizeof(fw_table));
	string fw_file(argv[8]);
	string path_file(argv[10]);
	establishConnections(connections, shortest_fw_path1, shortest_fw_path2, nodes, no_of_connections, 0, topology,fw_table, "output_file",fw_file,flag,path_file);
	return 0;
}

