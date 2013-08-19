#include<iostream>//edges are undirected and weighted fot the 200 vertex graphWe've to implement it similar to the  BFS algorithm as it is used in shortest pathvertices start from zero
#include<time.h>
using namespace std;
#define size 200
struct node
{int v;
 int wt;
struct node *next;};//We've to create an adjacency list
int seen[size];//keeps the seen vertices by the dijkstra
bool covered[size];//boolean whether vertex has been discovered by dijkstra
bool blocked[size];//if v is a vertex then blocked[v]=true means the vertex is blocked
int pos;
long int A[size];//This array stores the min path length from the start vertex
                 //ie greedy dijkstra value corresponding to the vertex v in v-1th cell

//-------------------------------------------------------------
int greedymin(int v,node **Graph)//this function calculates the closest unexplored vertex to v.pos gives the position of the vertex.min returns the distance
{
node *curr;
int flag=0;//flag becomes 1 as soon as the curr points to an undiscovered node
int min;
curr=Graph[v];
while(curr!=NULL)
{if(covered[curr->v]==false)
 {flag=1;
  min=A[v]+curr->wt;//initialising min
  pos=curr->v;
  curr=curr->next;
  break;}
curr=curr->next;}//this loop is there for initialising min
if(flag==0)//all vertices connected to v have been explored.Thus v isn't a frontier vertex
return -1;
while(curr!=NULL)
{if(covered[curr->v]==false&&min>(A[v]+curr->wt))
 {flag=1;
  min=A[v]+curr->wt;
  pos=curr->v;}
 curr=curr->next;
}
return min;
}
//-------------------------------------------------------------
void dijkstra(node **Graph,int source,int n,int previous[])//source is the current place of the robot.this sub routine
{for(int i=0;i<n;i++)
 {covered[i]=false;
  previous[i]=-1;//previous vertices undefined.previous[i] stores the vertex before i in the path
  A[i]=1000000;
  seen[i]=-1;}//no vertex has been seen
int temp;//this will store the parent vertex of the vertex which is brought into the explored region
int top=-1;
int flag;
int min,sum,pos1;
seen[++top]=source;//first vertex is seen.ie seen[0]=source
A[source]=0;
covered[source]=true;
do
{flag=0;
for(int i=0;i<=top;i++)//in this loop we're going through every seen vertex and trying to find the unknown vertices connecting them
{
sum=greedymin(seen[i],Graph);//returns the minimum sum A[seen[i]]+edgewt and also the position of the point to which this edge points is stored in pos

if(sum==-1)//that means the vertex seen isn't a frontier vertex
continue;
else
flag++;//flag tells us about the number of vertices brought from undiscovered to discovered region.if flag=1 then first frontier vertex is added.this is done for minimum computation
if(flag==1)
{min=sum;//for the first undiscovered vertex at the frontier, min is assigned
pos1=pos;
temp=seen[i];
continue;
}
if(min>sum)
{min=sum;
pos1=pos;
temp=seen[i];}
}//end of for.pos1 contains the vertex to be added to the seen region and min contains the min greedy sum at the frontier
if(flag!=0)
{seen[++top]=pos1;
 //cout<<"Vertex added to explored region is"<<pos1<<endl;
 //cout<<"previous vertex in shortest path is"<<temp<<endl;
 A[pos1]=min;
 previous[pos1]=temp;
 covered[pos1]=true;}
}
while(flag!=0);

}
//--------------------------------------------------------
void stentz(node **Graph,int start,int goal,int n)
{int v=start;//v is the current location of the robot.initial-start
 int previous[n];//store the parent to print the path
 while(v!=goal)//till the bot doesn't reach the goal
  {for(int i=0;i<n;i++)
   previous[i]=-1;
  dijkstra(Graph,v,n,previous);
  if(A[goal]==1000000)
  {cout<<"No path to goal exists";
   return;}
  int j=0;//j is the size of the path array.initially it is -1
  int path[n];
  path[j]=goal;
  int i=goal;
  while(previous[i]!=v)//v is the robot's current vertex
  {path[++j]=previous[i];
   i=previous[i];
   }//storing the shortest path from goal to start excluding the goal and start vertices.thus it'll be printed in reverse
  path[++j]=v;
  i=j;
  cout<<"Found new path from "<<v<<" of length "<<A[goal]<<":"<<endl;
  while(i>=0)//path array has to be read in reverse
  cout<<path[i--]<<" ";//path printed now check in the path which vertex is blocked
  cout<<endl;
  i=j;
  int b;//b is the name of blocked vertex
  while(i>=0)//robot starts traversing the path
  {if(blocked[path[i]]==true)//the path[i] vertex is blocked and has to be deleted from the graph
   {b=path[i];
    v=path[i+1];//the robot stops at the node just before the blocked node
    node *curr,*temp;
    curr=Graph[b];//curr points to the starting of the list corresponding to the blocked vertex
    while(curr!=NULL)
    {temp=curr;
     curr=curr->next;
     delete temp;}
    Graph[b]=NULL;//the list of blocked vertex is  not in the graph
    //we also have to search the graph and delete all the nodes which contains v=b
   for(i=0;i<n;i++)
   {node *curr=Graph[i];
    if(curr==NULL)
    continue;
    if(curr->v==b)
    {Graph[i]=curr->next;
     delete curr;
     continue;
    }
    while(curr!=NULL)
    {if(curr->v==b)
     {if(curr==Graph[i])//curr is head of list
      {Graph[i]=curr->next;
       delete curr;
      }
      node *temp=curr;
      curr=Graph[i];
      while(curr->next!=temp)
      curr=curr->next;
      //after the loop curr points to node previous to node to be deleted
      curr->next=temp->next;
     delete temp;}
     curr=curr->next;
     }
   }
   break;//because the bot can't continue moving that path
   }
  v=path[i];//as the current vertex path[i] wasn't an obstruction
  i--;//if no obstruction keep on moving
  }
  //cout<<"current vertex of the bot "<<v;
  if(v==goal)
  {cout<<"Goal reached\n";
   return;}

 }
}
//--------------------------------------------------------
int main()
{clock_t t;
t=clock();
int n;//n is the number of vertices
cin>>n;//vertices inputted
int i,k,v,wt;
int start,goal;
cin>>start>>goal;//start and goal vertices inputted
node **Graph;
Graph=new node* [n];
if(Graph==NULL)
{cout<<"Graph not created";
 return 0;}
for(i=0;i<n;i++)
Graph[i]=NULL;
for(i=0;i<n;i++)//for every vertex
{cin>>k;//number of edges incident on ith vertex
 int j;
 for(j=1;j<=k;j++)
 {cin>>v;
 cin>>wt;
 if(Graph[i]==NULL)
 {Graph[i]=new node;
  Graph[i]->v=v;
  Graph[i]->wt=wt;
  Graph[i]->next=NULL;}
 else
 {node *curr;
  curr=new node;
  curr->v=v;
  curr->wt=wt;
  curr->next=Graph[i];//attaching the new node at starting of list
  Graph[i]=curr;}}}
int b;//b is the number of blocked vertices
cin>>b;
for(i=0;i<n;i++)
blocked[i]=false;
for(i=1;i<=b;i++)
{cin>>v;
blocked[v]=true;}//vertex is blocked
stentz(Graph,start,goal,n);
t=clock()-t;
cout<<"Time="<<float(t)/CLOCKS_PER_SEC;
return 0;
}
