#include<iostream>
#include<stdlib.h>
#include<cmath>
#include<time.h>
using namespace std;
int id=0;
float t=0;
int m,n;
//-------------------------------------------
float generate_exponential(float lambda)
{float p=((1.0*rand())/RAND_MAX);
 return (1/lambda)*log(1/(1-p));
}
//-------------------------------------------
class event//a heap of event objects will be made heaped on the basis of time
{public:
int q_no;//queue number at which finish was called
float t;//time at which finish of the queue occured
};
class heap//heap of events
{event a[500];
 int size;//size stores the max index of the heap which is occupied
 public:
 heap()
 {size=0;}//when object is created and nothing stored in the heap size is 0.
 void Insert(event x);
 event Extract_min();
 void Display()
{for(int i=1;i<=size;i++)
 {cout<<endl<<a[i].q_no<<"\t";
  cout<<a[i].t<<endl;}
}
}h;
void heap::Insert(event x)
{if(size==0)
  a[++size]=x;
  else
  a[++size]=x;//the incoming element has been stored at the last position of the heap
  int i=size;
  while(i>1)
  {if(a[i].t<a[i/2].t)//if the child is lesser than the root
   {event temp;
    temp=a[i];
    a[i]=a[i/2];
    a[i/2]=temp;
    i=i/2;}
   else
   break;
  }
}
event heap::Extract_min()
{event min;
 event temp;
 min=a[1];//min event stored
 a[1]=a[size--];//The last event which is the largest in time is placed at index 1.now we do sift down
 int i=1;
while(2*i+1<=size&&2*i<=size)//we check in the children
{if(a[i].t>a[2*i+1].t||a[i].t>a[2*i].t)
 {if(a[2*i].t<a[2*i+1].t)
  {temp=a[i];
   a[i]=a[2*i];
   a[2*i]=temp;
   i=2*i;}//now the event largest in time is at 2i
  else if(a[2*i+1].t<=a[2*i].t)
  {temp=a[i];
   a[i]=a[2*i+1];
   a[2*i+1]=temp;
   i=2*i+1;}
 }
else
break;}
if(2*i<=size)
{if(a[i].t>a[2*i].t)
 {
  temp=a[i];
  a[i]=a[2*i];
  a[2*i]=temp;}
}
return min;
}
class links//linking class for each queue
{public:
 int q_no;
 float prob;
 };
class customer
{public:
 int id;
 float t_entry;//time of entry in the system
 float t_exit;//time of exit from the system
 customer *next;
 customer()
 {next=NULL;
 }
 };
class List
{public:
 int size;//size of list
 customer *rear,*front;
 List()
 {rear=front=NULL;
  size=0;}
 void Insert(customer *c)
 {//cout<<"Inserted";
  if(rear==NULL)
   rear=front=c;
  else
  {rear->next=c;
   rear=c;
   rear->next=NULL;
   }
   ++size;
 }
 customer* Remove()
 {//cout<<"Removed\n";
  customer *temp;
  temp=front;
  front=front->next;
  if(front==NULL)
  rear=NULL;
  temp->next=NULL;
  --size;
  return temp;
  }
 void Display()
 {customer *curr;
  curr=front;
  cout<<"Front-";
  while(curr!=NULL)
  {cout<<curr->id;
   curr=curr->next;}
  cout<<"Rear\n";}
};
class OQ//Output Q
{public:
 float tot_time;//time duration of each customer in the system is added to tot_time
 int count;
 OQ()
 {tot_time=0;
  count=0;}//that is when the output q is created
 void add(customer* c)
 {count++;//when customer added to output queue counter is incremented
  c->t_exit=t;//when customer enters the output queue its exit time from the system is set.
  tot_time=tot_time+(c->t_exit-c->t_entry);
  delete c;
 }
}O;
class IOQ//Input-Output Q
{public:
 List l;
 int k;//queue number has possible values from n to m-2
 float serv_rate;
 links *li;
 int nlinks;
 void add(customer *);
 void finish();
}*IO;//IO is the array of IO queues
void IOQ::add(customer *c)
{if(l.size==0)//the server is free
 {l.Insert(c);
  float dt=generate_exponential(serv_rate);
  event y;//y is the event of the finishing time of the c customer.
  y.q_no=k;
  y.t=t+dt;
  h.Insert(y);
  }
 else
 {//cout<<"Server busy adding to end of list\n";
     l.Insert(c);}//if server busy add to the back of queue
 }
void IOQ::finish()
{customer *f;
 f=l.Remove();
 if(l.size>0)//queue is not empty event to be generated for the finishing time of the next customer in queue
 {//cout<<"There is another element in q\n";
     float dt=generate_exponential(serv_rate);
  event y;
  y.q_no=k;
  y.t=t+dt;
  h.Insert(y);}
  float p=(1.0*rand())/RAND_MAX;
  float sum=0;
  for(int i=0;i<nlinks;i++)
  {sum+=li[i].prob;//takes care of the cumulative table
   if(p<=sum)//li[i].q_no i s the index of the input-output queue to which its linked
   {if(li[i].q_no==m-1)
    O.add(f);
    else
    IO[li[i].q_no-n].add(f);
    break;
    }
  }
 }
class IQ//Input Q
{public:
 int k;//k is the queue number of the IQ
 float serv_rate;
 links *li;
 int nlinks;
 void finish();
 }*I;
 void IQ::finish()
 {customer *c=new customer;
  c->t_entry=t;
  //cout<<c->t_entry<<endl;
  c->id=++id;
  float dt=generate_exponential(serv_rate);
  event y;
  y.q_no=k;
  y.t=t+dt;
  h.Insert(y);
  float p=( (1.0*rand())/RAND_MAX);//p is in between 0 and 1
  float sum=0;
  for(int i=0;i<nlinks;i++)
  {sum+=li[i].prob;//takes care of the cumulative table
   if(p<=sum)//li[i].q_no is the index of the input-output queue to which its linked
   {IO[li[i].q_no-n].add(c);
    break;}//break is necessary so that the loop doesn't continue other wise the same object would be inserted at same position
   }
  }

int main()
{clock_t u;
u=clock();
float x;
 int i;
// cout<<"Enter simulation time\n";
 cin>>x;
// cout<<"Enter number of total queues\n";
 cin>>m;//we have m total queues,n input queues and 1 output queue.Input-Output queues=m-n-1
// cout<<"Enter the number of input queues\n";
 cin>>n;
 I=new IQ[n];//I is an array of n input queues
 if(!I)
 cout<<"I not formed\n";
 IO=new IOQ[m-n-1];//IO is an array of m-n-1 intermediate queues
 if(!IO)
 cout<<"IO not formed";
// cout<<"Enter the lambda values\n";
 for(i=0;i<=n-1;i++)
 {I[i].k=i;
  cin>>I[i].serv_rate;//lambda value in input queue
 }
 for(i=n;i<=m-2;i++)
 {IO[i-n].k=i;
 cin>>IO[i-n].serv_rate;}//lambda value in input/output queue
// cout<<"Enter the transition probabilities for "<<m-1<<" queues starting from queue 0 to "<< m-2<<endl;
 int j,qno;
 float qprob;
 for(i=0;i<n;i++)//for the first n input queues
 {cin>>I[i].nlinks;//number of queues to which its connected
  I[i].li=new links[I[i].nlinks];//links li list created...
  for(j=0;j<I[i].nlinks;j++)
  {cin>>qno;
   cin>>qprob;
   I[i].li[j].q_no=qno;
   I[i].li[j].prob=qprob;}
  }
for(i=0;i<m-n-1;i++)//for the input/output queues
 {cin>>IO[i].nlinks;
  IO[i].li=new links[IO[i].nlinks];//links li list created
  for(j=0;j<IO[i].nlinks;j++)
  {cin>>qno;
   cin>>qprob;
   IO[i].li[j].q_no=qno;
   IO[i].li[j].prob=qprob;}
  }//Input ends
event l;
for(i=0;i<n;i++)//for all n input queues we generate events for the first customer entry from each queue
{float dt=generate_exponential(I[i].serv_rate);
 l.q_no=i;
 l.t=t+dt;
 h.Insert(l);
}
int cnt=1;
while(t<=x)//simulation continues
{if(cnt>2)
 break;
 //h.Display();
 l=h.Extract_min();
 if(l.t>x)
 break;
 t=l.t;
 //cout<<"finish called at Qno="<<l.q_no<<" time="<<l.t<<endl;
 if(l.q_no<n)
 I[l.q_no].finish();//finish for an input queue
 else if(l.q_no>=n)//ie the finish is for an input/output queue
 IO[l.q_no-n].finish();
 //cnt++;
}
//after the simulation ends
//cout<<"Exited the simulation loop\n";
//cout<<"Total number of customers who reached the output "<<O.count<<endl;
float w=O.tot_time/O.count;
//cout<<"Average time spent by the consumer\n";
cout<<w<<endl;
u=clock()-u;
cout<<"Time="<<float(u)/CLOCKS_PER_SEC;
return 0;
}

