#include<iostream>
#include<stdlib.h>
#include<cstring>
#include<time.h>
#define inf 100000
using namespace std;

int toss_coin(){
float x=((1.0*rand())/RAND_MAX);
if(x <= 0.5)
return 1;
else
return 0;
}

class Skiplist;//pre-declaration
class node{
friend class Skiplist;//making a class friend means that the class Skiplist can access private and protected members of class node
int x;
node *next,*down;
int hops;//tells us the number of hops in the bottom most list required to reach the next node in that level.this is for augmenting
};
 
class Skiplist{
private:
node *head;//the top-left most node of the skiplist contains -inf
node *tail;//the top right most node of the skiplist contains +inf
public:
Skiplist(){
 head=new node;
 head->x=-inf;
 head->down=NULL;
 head->hops=1;
 tail=new node;
 tail->x=inf;
 head->next=tail;
 tail->next=NULL;
 tail->down=NULL;
 tail->hops=0;//largest element of skiplist ie + infinity
}
void insert(int x)
{node *curr=head;//curr is at the top level left most node
 node *ptrs[1000];
 int i,k;
 for(i=0;i<1000;i++)
 ptrs[i]=NULL;
 i=0;
while(curr!=NULL)
 {if(x==curr->next->x)
  {cout<<"Key already present\n";
   return;}
  else if(x<curr->next->x)
  {ptrs[i++]=curr;//ptr[i] records the position from the linked list in that level from which it goes down  
   curr=curr->down;}
  else if(x>curr->next->x)
  curr=curr->next;
  }//Search ends here
 node *newinsert;//to be inserted b/w ptrs[i] and ptrs[i]->next ie the lowest level
 i--;//i is the size of the ptrs array.ptr[i-1] points to the lowest level of skiplist.
 newinsert=new node;
 newinsert->down=NULL;
 newinsert->x=x;
 newinsert->next=ptrs[i]->next;
 ptrs[i]->next=newinsert;//The node if not present is inserted at the bottom of the list
 curr=newinsert;//curr points to the newly inserted node at the bottom
 curr->hops=1;//since it is inserted at the bottom layer.all nodes at bottom layer have hops=1
//now we flip a coin 
int coin=toss_coin();//since insertion was done at the lowest level we start propagating up from the lowest level
//cout<<coin<<"\n";
i--;//at a level above the lowest from where we start
if(i>=0)
{for(int j=i;j>=0;j--)
 ptrs[j]->hops=ptrs[j]->hops+1;}
while(coin==1&&i>=0)//heads then we send the newly inserted node up the skiplist
 {newinsert=new node;
  newinsert->x=x;
  newinsert->down=curr;//the higher node in the tower is linked with the node in a level below
  newinsert->next=ptrs[i]->next;
  ptrs[i]->next=newinsert;
  curr=ptrs[i]->down;//curr points to a node below the ptrs[i] node
  k=0;//sum of hops
  while(curr!=newinsert->down)
  {k=k+curr->hops;
   curr=curr->next;}//k stores the number of hops from ptrs[i]->down to the newly inserted node in the lower level of tower
  newinsert->hops=ptrs[i]->hops-k;
  ptrs[i]->hops=k;
  curr=newinsert;//curr points to the newinsert in a level
  i--;
  coin=toss_coin();
  //cout<<coin<<"\n";
  }
if(i==-1&&coin==1)//we will have to increase the height of skiplist
{node *temp;
//cout<<"increase the height of skiplist\n";
 while(coin==1)
 {node *newhead=new node;
  newhead->x=-inf;
  newhead->down=head;
  head=newhead;//head has been updated to a level higher
  head->next=new node;//the new node to be inserted
  head->next->x=x;
  head->next->down=curr;//curr is the newinsert made at the lower level
  curr=head->next;//curr is now the newly inserted node at the higher level
  curr->next=new node;
  curr->next->x=inf;
  curr->next->next=NULL;
  curr->next->down=tail;
  tail=curr->next;
  temp=head->down;//temp is the node below the new head ie the -inf
  k=0;
  while(temp->x<x)//traverse the lower list till we reach a node just before the new insert made below
  {k=k+temp->hops;
   temp=temp->next;
   }//temp finally points to the node with value x ie curr
   //cout<<"end of temp!=curr while loop\n";
   head->hops=k;
   temp=curr->down;//temp points to the node at the lower level in the tower
   k=0;
   while(temp!=tail->down)
   {k=k+temp->hops;
    temp=temp->next;}
    //cout<<"end of temp!=tail->down while loop\n";
    curr->hops=k;
    coin=toss_coin();
    //cout<<coin<<"\n";	
    }
}


}//end of insert

void del(int x)
{node *curr=head;
 node *ptrs[1000];
 int i;
 for(i=0;i<1000;i++)
 ptrs[i]=NULL;
 i=0;
 while(curr!=NULL)
 {if(curr->next->x>=x)
  {ptrs[i++]=curr;
   curr=curr->down;}
  else 
  curr=curr->next;
  }//Search for the element to be deleted ends here
i--;//i-1 is index of the bottom most row
if(i-1>=0)
{for(int j=i-1;j>=0;j--)
ptrs[j]->hops=ptrs[j]->hops-1;}
if(ptrs[i]->next->x==x)//special case when node from the bottom is deleted.hops remain the same
{node *temp=ptrs[i]->next;
 ptrs[i]->next=temp->next;
 delete temp;
 i--;
}
while(i>=0&&ptrs[i]->next->x==x)//to delete the tower corresponding to that node
{node *temp=ptrs[i]->next;//temp is the node to be deleted
 ptrs[i]->next=temp->next;
 ptrs[i]->hops=ptrs[i]->hops+temp->hops;
 delete temp;
 i--;
 }
}
int kthsmallest(int k)
{int count=0;
node *curr=head;
while(curr)
{if(count+curr->hops==k)
 {curr=curr->next;
  return curr->x;}
 else if(count+curr->hops<k)
 {count=count+curr->hops;
  curr=curr->next;}
 else
 curr=curr->down;
}
}
void display()//displays the skiplist level by level along with the hop values
{node *curr,*start,*end;
start=head;
end=tail;
while(start!=NULL)
{curr=start;
while(curr!=NULL)
{cout<<curr->x<<" "<<curr->hops<<"->";
//cout<<curr->x<<"->";
curr=curr->next;}//traverses the level horizontally
cout<<"\n";
start=start->down;
end=end->down;
}
}
};
int main() 
{clock_t t;
t=clock();
Skiplist obj;
int inst;//number of instructions you want to execute
int noi;//number of insertions
int nod;//number of deletions
//cout<<"No of instructions\n";
cin>>inst;
int i=1;
int j;
char a[20];
int x;
while(i<=inst)
{cin>>a;
 if(strcmp(a,"insert")==0)
 {cin>>noi;
  for(j=1;j<=noi;j++)
  {cin>>x;
   obj.insert(x);}
  //obj.display();
  }
  
 if(strcmp(a,"delete")==0)
 {cin>>nod;
  for(j=1;j<=nod;j++)
  {cin>>x;
   obj.del(x);}
  //obj.display();
   }
   
 if(strcmp(a,"select")==0)
 {cin>>x;
  x=obj.kthsmallest(x);
  cout<<x<<"\n";} 
 i++; 
}
t=clock()-t;
cout<<"Time="<<float(t)/CLOCKS_PER_SEC;
return 0; 
}