#include<iostream>
#include<time.h>
using namespace std;
class TreeNode{
public:
int x,leftsize,rank;//rank is also counting the node itself in black height
TreeNode *left,*right,*parent;
char color;//c is 'r' or 'b'
};
class RBT{
public:
TreeNode *root,*nil;//nil is for the external node
RBT()
{root=NULL;
nil=new TreeNode;
nil->color='b';
nil->x=-1;
nil->left=nil->right=nil->parent=nil;
nil->leftsize=0;
nil->rank=1;}
void leftrot(TreeNode *a)//left rotating about a so a becomes the child of its right child.assumed that a has a right child.
{TreeNode *rc=a->right;//rc is the right child of a
 a->right=rc->left;//turn rc's left subtree into a's right subtree
 if(rc->left->x!=-1)//equivalent to rc->left!=nil
   rc->left->parent=a;//link a to rc's left child
 rc->parent=a->parent;//link a's parent to rc
 if(a->parent->x==-1)//a was the root or equivalent to a->parent==nil
  root=rc;
 else if(a==a->parent->left)//a is the left child of its parent 
 a->parent->left=rc;
 else
 a->parent->right=rc;
 rc->left=a;
 a->parent=rc;
 rc->leftsize=rc->leftsize+1+a->leftsize;//leftsize invariant maintained during rotation
 if(a==root)
 root=rc;
}
void rightrot(TreeNode *a)//right rotating about a so a becomes the child of its left child.assumed that a has a left child.
{TreeNode *lc=a->left;//lc is the left child of a
 a->left=lc->right;//turn lc's right subtree into a's left subtree
 if(lc->right->x!=-1)
  lc->right->parent=a;
 lc->parent=a->parent;
 if(a->parent->x==-1)
 root=lc;
 else if(a==a->parent->left)
 a->parent->left=lc;
 else
 a->parent->right=lc;
 lc->right=a;
 a->parent=lc;
 a->leftsize=a->leftsize-(lc->leftsize+1);//leftsize invariant maintained during rotation
 if(a==root)//then the left child becomes the root
 root=lc;
 }
void fixinsert(TreeNode *a);
void insert(int x);
void fixdel(TreeNode *a);
void del(int x);
int kthsmallest(int k)
{TreeNode * curr=root;
while(curr->x!=-1)//equivalent to curr!=nil
{if(k==curr->leftsize+1)
  return curr->x;
 else if(k<curr->leftsize+1)
  curr=curr->left;
 else
  {k=k-(curr->leftsize+1); 
   curr=curr->right;}
}
}
void threewayjoin(TreeNode *mid,RBT& t1,RBT& t2)//assuming t1 has keys less than mid->x,t2 has keys greater than mid->x...
{TreeNode *curr,*temp;
mid->color='r';
if(t1.root->rank>t2.root->rank)
  {root=t1.root;
   curr=t1.root; 
   while(curr->rank!=t2.root->rank)
    curr=curr->right;  
   mid->right=t2.root;
   t2.root->parent=mid;
   mid->rank=t2.root->rank;
   if(curr->color=='r')
    curr=curr->right;//we want curr to be black always 
   mid->left=curr;
   mid->parent=curr->parent;
   curr->parent->right=mid;
   curr->parent=mid;//curr is now the left child of mid.mid's parent is curr's parent
   if(mid->parent->color=='r')
   fixinsert(mid);}//to fix double red problem

 else if(t1.root->rank==t2.root->rank)
{root=mid;
 root->parent=nil;
 mid->right=t2.root;
 t2.root->parent=mid;
 mid->left=t1.root;
 t1.root->parent=mid;
 root->color='b';
 root->rank=t1.root->rank+1;
 }
else//when rank t1<rank t2
{root=t2.root;
 curr=t2.root;
 while(curr->rank!=t1.root->rank)
  curr=curr->left;
 mid->left=t1.root;
 t1.root->parent=mid;
 mid->rank=t1.root->rank;
 if(curr->color=='r')
  curr=curr->left;
 mid->right=curr;
 mid->parent=curr->parent;
 curr->parent->left=mid;
 curr->parent=mid;
 if(mid->parent->color=='r')
  fixinsert(mid);}
temp=mid;//-----update the leftsize of mid and its parents
while(temp->x!=-1)
{if(temp->x>=mid->x)
 {int sum=0;
  curr=temp->left;
  while(curr->x!=-1)//equivalent curr!=nil
  {sum+=(curr->leftsize+1);
   curr=curr->right;}
  temp->leftsize=sum;}
 temp=temp->parent;
 }
t1.root=t2.root=NULL;//the trees t1 and t2 have been destroyed
}
void split(int k,RBT& t1,RBT& t2)
{start:
 TreeNode *curr,*temp;
 curr=root;
 while(curr->x!=-1)//curr!=nil
 {if(curr->x==k)
  break;
  else if(curr->x>k)
  curr=curr->left;
  else
  curr=curr->right;}
 if(curr->x==-1)//equivalent to curr==nil
 {insert(k);
  goto start;}
  RBT a,b,c;
 t1.root=curr->left;//t1 has keys<k
 t2.root=curr->right;//t2 has keys>k
 if(curr==root)//the key about which the tree is split is root
  {if(curr->left->color=='r')
    {curr->left->color='b';
     curr->left->rank++;}
   if(curr->right->color=='r')
    {curr->right->color='b';
     curr->right->rank++;}  
   if(t1.root->x==-1)//t1 is nil.
    t1.root=NULL;
   if(t2.root->x==-1)
    t2.root=NULL;}
 curr=curr->parent;
 while(curr->x!=-1)//equivalent to curr!=nil
 {if(curr->x<k)//the subtree with keys <k is formed
  {a.root=curr->left;
   a.root->parent=a.nil;
   if(a.root->color=='r')
   {a.root->color='b';
    a.root->rank++;} 
   temp=curr;
   curr=curr->parent;
   if(t1.root->x==-1)
   {if(a.root->x==-1)//equivalent to a.root==nil
     a.root=NULL;
    a.insert(temp->x);
    t1.root=a.root;}
   else 
   {c.threewayjoin(temp,a,t1);
    t1.root=c.root;}}
  else//curr->x>k
  {b.root=curr->right;
   b.root->parent=b.nil;
   if(b.root->color=='r')
   {b.root->color='b';
    b.root->rank++;}
   temp=curr;
   curr=curr->parent;
   if(t2.root->x==-1)//t2 points to a nil
   {if(b.root->x==-1)//equivalent to b.root==nil
    b.root=NULL;
    b.insert(temp->x);//the join is equivalent to an insert
    t2.root=b.root;
   }
   else
   {c.threewayjoin(temp,t2,b);
   t2.root=c.root;}}}}
void display(TreeNode *curr)
{if(curr->x==-1)//equivalent to curr==nil
 return;
 display(curr->left);
 cout<<"val="<<curr->x<<"rank="<<curr->rank<<"leftsize="<<curr->leftsize<<"color="<<curr->color<<endl;
 display(curr->right);
}};
void RBT::insert(int x)
{TreeNode *p=NULL;
TreeNode *curr=root;//p is the parent of curr
if(root==NULL)
{root=new TreeNode;
 root->color='b';
 root->rank=2;//it has black nulls and itself is black
 root->leftsize=0;
 root->left=root->right=root->parent=nil;
 root->x=x;
 return;
}
while(curr->x!=-1)//equivalent to curr!=nil
{p=curr;
 if(x==curr->x)
  {cout<<"Key already present";return;}
 else if(x<curr->x)
  curr=curr->left;
 else
  curr=curr->right;
}
TreeNode* newinsert=new TreeNode;
newinsert->left=newinsert->right=nil;
newinsert->x=x;
newinsert->color='r';//new node inserted is red in color
newinsert->rank=1;//black null below the node
newinsert->leftsize=0;
if(newinsert->x<p->x)
p->left=newinsert;
else
p->right=newinsert;
newinsert->parent=p;
curr=newinsert;
while(curr->x!=-1)//equivalent to curr!=nil
{if(curr==curr->parent->left)
    curr->parent->leftsize++;
  curr=curr->parent;}//to fix the leftsize
if(newinsert->parent->color=='r')
fixinsert(newinsert);//fixing starts from bottom(newinsert) to top
}
void RBT::fixinsert(TreeNode* a)//fixes the rank and the colors.y is the uncle 
{while(a->parent->color=='r')
 {if(a->parent==a->parent->parent->left)
  {TreeNode *y=a->parent->parent->right; 
   if(y->color=='r')
   {a->parent->color='b';
    a->parent->rank++;
    y->color='b';
	y->rank++;
	a->parent->parent->color='r';//since a's gparent's both children are colored black thus no rank change for a->gp
	a=a->parent->parent;
	if(a==root)
        break;
        continue;}
   else if(a==a->parent->right)//case when y is black and a is the right child of its parent is reduced to the case
   {a=a->parent;//when y is black and a is the left child of its parent
    leftrot(a);}
	a->parent->color='b';//the case when y is black and a is the left child
	a->parent->rank++;
	a->parent->parent->color='r';
	rightrot(a->parent->parent);
	a->parent->right->rank--;}//from here the loop breaks
   else
   {TreeNode *y=a->parent->parent->left; 
   if(y->color=='r')
   {a->parent->color='b';
    a->parent->rank++;
    y->color='b';
    y->rank++;
    a->parent->parent->color='r';
    a=a->parent->parent;
    if(a==root)
    break;
    continue;
    }
   else if(a==a->parent->left)
   {a=a->parent;
    rightrot(a);}
	a->parent->color='b';
	a->parent->rank++;
	a->parent->parent->color='r';
	leftrot(a->parent->parent);
	a->parent->left->rank--;}
   }
  if(root->color=='r')
  root->rank++;//since the root turned red and u changed it to black...  
  root->color='b';
}
void RBT::del(int z)
{TreeNode *curr=root,*curr1,*temp;//curr is finally the actually removed node
while(curr->x!=z)
{if(curr->x>z)
  curr=curr->left;
 else
  curr=curr->right;
 if(curr->x==-1)//equivalent to curr==nil
  {cout<<"Key not present is "<<z<<endl;
   return;}}
char color;//stores the color of the node which is actually removed from the tree
TreeNode * x;//x is either the only child of the actual removed node or T.nil 
if(curr->left->x==-1&&curr->right->x==-1)//equivalent to curr->left==nil&&curr->right==nil.if the node to be deleted has no children
 {if(curr==root)
   root=NULL;
  else if(curr==curr->parent->right)
   curr->parent->right=nil;
  else//the node to be deleted is the left child of its parent
   curr->parent->left=nil;
  x=nil;
  }
else if(curr->left->x==-1)//equivalent  to curr->left==nil if the node to be deleted(curr) has only right child
 {if(curr==root)
  {root=curr->right;
   root->parent=nil;}
  else if(curr==curr->parent->right)//curr is right child of its parent
   curr->parent->right=curr->right;
  else
   curr->parent->left=curr->right;
  x=curr->right;}
else if(curr->right->x==-1)//equivalent to curr->right==nil if the node to be deleted has only left child
 {if(curr==root)
  {root=curr->left;
   root->parent=nil;}
  else if(curr==curr->parent->left)
   curr->parent->left=curr->left;
  else 
   curr->parent->right=curr->left;
  x=curr->left;}
else//when it has both children not nil then y is the inorder predecessor of curr
{TreeNode *y=curr->left;
 if(y->right->x==-1)//equivalent to y->right==nil
 {curr->x=y->x;
  curr=y;//curr is the left child of the key k which was to be deleted.
  x=curr->left;
  curr->parent->left=x;}
 else
 {while(y->right->x!=-1)//equivalent to y->right!=nil
  y=y->right;//so y which is the predecessor has no right child
 curr->x=y->x;//value at curr replaced with its predecessor
 curr=y;
 x=curr->left;
 curr->parent->right=x;}}//since curr was the right child of its parent
x->parent=curr->parent;
curr1=x->parent;//curr1 points to the node whose leftsize is to be corrected.moves from x->parent to the root
while(curr1->x!=-1)//loop to correct leftsize
{if(curr1->x>=curr->x)//curr1 is on a right turn
 {int sum=0;
  temp=curr1->left;
  while(temp->x!=-1)
   {sum+=(temp->leftsize+1);
    temp=temp->right;}
  curr1->leftsize=sum;}
  curr1=curr1->parent;}
color=curr->color;
curr->left=curr->right=curr->parent=NULL;
delete curr;
if(color=='b')//color of the removed node is black only then fixdel is called on the child of the actual removed node
fixdel(x);
}
void RBT::fixdel(TreeNode *x)
{TreeNode *w;
while(x!=root&&x->color=='b')
 {if(x==x->parent->left)
   {w=x->parent->right;//w rank=h,x rank=h-1,x->parent->rank=h+1
    if(w->color=='r')
	 {w->color='b';
	  w->rank++;//w->rank=h+1
	  x->parent->color='r';
	  leftrot(x->parent);
	  x->parent->rank--;//x->parent->rank=h
	  w=x->parent->right;}
    if(w->left->color=='b'&&w->right->color=='b')
     {w->color='r';
      w->rank--;
      x=x->parent;
      x->rank--;
      if(x==root)
      {x->rank--;
      break;}
      else
      continue;}
    else if(w->right->color=='b')
     {w->left->color='b';
      w->left->rank++;
      w->color='r';
      rightrot(w);
      w->rank--;
      w=x->parent->right;}//thus finally we have w as the black sister of x with its w->leftchild black and w->rightchild red.so now it depends on x->parent's color  
    w->color=x->parent->color;
    if(x->parent->color=='b')
     {x->parent->rank--;}
    x->parent->color='b';
    w->right->color='b';
    w->right->rank++;
    leftrot(x->parent);
    if(w->color=='r')
     w->rank--;//thus no change in rank of w
    w->rank++;
    if(w==root)
    w->rank--;
    x=root;
    }
 else
   {w=x->parent->left;
    if(w->color=='r')
	 {w->color='b';
	  w->rank++;
	  x->parent->color='r';
	  rightrot(x->parent);
	  x->parent->rank--;
	  w=x->parent->left;}
    if(w->left->color=='b'&&w->right->color=='b')
     {w->color='r';
      w->rank--;
      x=x->parent;
      x->rank--;
      if(x==root)
      {x->rank--;
      break;}
      else
      continue;}
    else if(w->left->color=='b')
     {w->right->color='b';
      w->right->rank++;
      w->color='r';
      leftrot(w);
      w->rank--;
      w=x->parent->left;}
    w->color=x->parent->color;
    if(x->parent->color=='b')
	x->parent->rank--;
    x->parent->color='b';
    w->left->color='b';
    w->left->rank++;
    rightrot(x->parent);
    if(w->color=='r')
     w->rank--;
    w->rank++;
    if(w==root)
    w->rank--;
    x=root;
    }
}
x->color='b';
x->rank++;
}
int main()
{clock_t t;
t=clock();
int i,j,treeno,no,t1,t2,key,t3;
 cin>>i;
 string inst;
 RBT rbt[10000];
 for(j=1;j<=i;j++)
 {cin>>inst;
  if(inst=="insert")
   {cin>>treeno;//treenumber
   cin>>no;//number of inserts
   for(int k=1;k<=no;k++)
   {int x;
    cin>>x;
    rbt[treeno].insert(x);}}
   else if(inst=="delete")
   {cin>>treeno>>no;//number of deletes
   for(int k=1;k<=no;k++)
   {int x;
    cin>>x;
    rbt[treeno].del(x);
    }}
   else if(inst=="join")
   {cin>>t1>>key>>t2>>t3;
   TreeNode *middle=new TreeNode;
   middle->x=key;
   rbt[t3].threewayjoin(middle,rbt[t1],rbt[t2]);}
   else if(inst=="split")
   {cin>>t3>>key>>t1>>t2;
    rbt[t3].split(key,rbt[t1],rbt[t2]);}
   else if(inst=="select")
   {cin>>t1>>key;
    cout<<rbt[t1].kthsmallest(key)<<endl;}
   else
   cout<<"Wrong command";
 }
 t=clock()-t;
 cout<<"Time taken="<<(float(t))/CLOCKS_PER_SEC<<endl;
 return 0;}

