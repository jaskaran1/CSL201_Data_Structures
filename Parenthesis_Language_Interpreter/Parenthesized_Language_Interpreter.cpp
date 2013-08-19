#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
char *cptr,*last;//global pointers.
class Genlistnode
{public:
 char S[30];
 int flag;
 Genlistnode *down;
Genlistnode *next;
};
//------------------------------------------------------
class Value
{public:
 int flag;
 float f;
 Genlistnode *link;
 bool t;
 Value()
 {t=true;}
};//every statement on evaluation returns value type object
//------------------------------------------------------
class Env
{public:
 Value *a[26];
 Env *parent;
 Env()
 {int i;
  for(i=0;i<26;i++)
  a[i]=NULL;
  parent=NULL;}
};
//------------------------------------------------------
void addToEnv(char x,Value v,Env &e)
{Value *v1=new Value;
 *v1=v;
 e.a[x-'a']=v1;}
//------------------------------------------------------
Value FindinEnv(char x[],Env &e)
{char* k;
 k=x;
 if(e.a[*k-'a']!=NULL)
  return *(e.a[*k-'a']);
 else
 {if(e.parent==NULL)
   {Value v;
    v.t=false;
    return v;}
  else
  return FindinEnv(x,*e.parent);}
}
//------------------------------------------------------
void displaylist(Genlistnode *p)
{Genlistnode *curr=NULL;
 curr=p;
 while(curr!=NULL)
 {if(curr->flag==1)
  displaylist(curr->down);
  else
  puts(curr->S);
  curr=curr->next;
  }
}
//Function to display generalised list
//------------------------------------------------------
Genlistnode*  makelist()//parser function.parses and creates genlist
{Genlistnode *head=NULL;
 Genlistnode *curr=NULL;
 Genlistnode *temp=NULL;
 int i;
 while(cptr<last)//till the whole string isn't parsed 
 {if(*cptr=='(')
   {temp=new Genlistnode;
    temp->flag=1;
    temp->next=NULL;
    temp->down=NULL;
    cptr=cptr+2;//to skip the space and (
    if(head==NULL)
     head=temp;
    else
    {curr=head;
     while(curr->next!=NULL)
     curr=curr->next;
     curr->next=temp;
     }
    temp->down=makelist();
      }
  else if(*cptr==')')
   {cptr=cptr+2;
    return head;}
  else//if neither a left or a right paranthesis
  {temp=new Genlistnode;
   temp->flag=0;//atom
   temp->next=NULL;
   temp->down=NULL;
   i=0;
   while(*cptr!=' ')
   temp->S[i++]=*(cptr++);//we're adding the string character by character to head->s
   cptr++;//to skip the space
   temp->S[i]='\0';
   //puts(temp->S);//just to see the output
   if(head==NULL)
   head=temp;
   else
   {curr=head;
    while(curr->next!=NULL)
    curr=curr->next;
    curr->next=temp;
    }
   }//end of outer else
   }//end of while

return head;//returns head to the p=makelist(cptr,end);
}
//------------------------------------------------------
int isnumber(char s[])
{char* k;
 k=s;
 while(*k-'0'>=0&&*k-'0'<=9||*k=='.')
 k++;
 if(*k=='\0')
 return 1;
 else
 return 0;}
//-------------------------------------------------------
int isvar(char s[])
{if(s[0]-'a'>=0&&s[0]-'a'<=25)
 return 1;
 else
 return 0;}
//----------------------------------
float number(char s[])
{float num=0.0;
 int p=1;
 char *k;
 k=s;
 while(*k!='\0')
 {if(*k-'0'>=0&&*k-'0'<=9)
  {num=10*num+(*k-'0');
   k++;}
  else if(*k=='.')
  {k++;
   break;}
   }
while(*k!='\0')
{num=10*num+(*k-'0');
 p=p*10;
 k++;}
return num/p;
}
//-------------------------------------------------------
Value evalNode(Genlistnode *p,Env& e);//pre-declaration
//-------------------------------------------------------
Value evalList(Genlistnode *p,Env& e)
{Value v;
 if(p->flag==0)//node is an atom
{if(strcmp(p->S,"begin")==0)
 {p=p->next;
  while(p->next!=NULL)
  {evalNode(p,e);//we keep on evaluating the nodes because according to begin the return value of begin is the value of the last node
   p=p->next;}
  return evalNode(p,e);}
 
 else if(strcmp(p->S,"lambda")==0)
  {Value new_val;
    new_val.flag=1;
    new_val.link=p;
    return new_val;}
  else if(strcmp(p->S,"define")==0)//we also have to print the value
  {p=p->next;//points to the alphabet
   v=evalNode(p->next,e);
   addToEnv(p->S[0],v,e);
   return v;
   }
  else if(strcmp(p->S,"set")==0)
  {p=p->next;
   char *k;
   k=p->S;
   v=FindinEnv(p->S,e);
   if(v.t==false)
   cout<<"Variable "<<*k<<"hasn't been defined\n";
   else
   {v=evalNode(p->next,e);//evaluating the expression in current environment
    addToEnv(*k,v,e);}
   }
  else if(strcmp(p->S,"if")==0)
  {p=p->next;
   v=evalNode(p,e);
   if(v.f==1)
   return evalNode(p->next,e);
   else
   return evalNode(p->next->next,e);
   }
  else if(strcmp(p->S,"while")==0)
  {p=p->next;//p points to the condition list
   v=evalNode(p,e);
   while(v.f==1)
   {evalNode(p->next,e);//evaluates body of while list
    v=evalNode(p,e);//evaluates the condition again
    }
   }
  else if(isvar(p->S))//a function call.a new environment has to be created
  {v=FindinEnv(p->S,e);
   if(v.t==false)
   cout<<"Function hasn't been defined\n";
   else
   {if(v.link==NULL)
    cout<<"A variable has been defined by this name not a function\n";
    else
    {p=p->next;//p now points to the first argument of the function call
     Env e1;//an inner environment is created
     e1.parent=&e;
     Genlistnode *q,*arglist,*body,*curr;
     q=v.link;//q points to the list starting from lambda
     arglist=q->next->down;
     body=q->next->next->down;
     curr=arglist;
     while(curr!=NULL&&p!=NULL)
     {addToEnv(curr->S[0],evalNode(p,e),e1);
      curr=curr->next;
      p=p->next;}
     if(curr==NULL&&p==NULL)
     return evalList(body,e1);
     else
     cout<<"Arguments didn't match\n";}//end of second innermost else
     }
    }//end of else if(isvar(p->S))
    else if(strcmp(p->S,"+")==0) 
    {Value v1=evalNode(p->next,e);
     Value v2=evalNode(p->next->next,e);
     Value v3;
     v3.flag=0;
     v3.link=NULL;
     v3.f=v1.f+v2.f;
     return v3;}
    else if(strcmp(p->S,"-")==0) 
    {Value v1=evalNode(p->next,e);
     Value v2=evalNode(p->next->next,e);
     Value v3;
     v3.link=NULL;
     v3.flag=0;
     v3.f=v1.f-v2.f;
     return v3;}
    else if(strcmp(p->S,"*")==0) 
    {Value v1=evalNode(p->next,e);
     Value v2=evalNode(p->next->next,e);
     Value v3;
     v3.flag=0;
     v3.link=NULL;
     v3.f=v1.f*v2.f;
     return v3;}
    else if(strcmp(p->S,"/")==0) 
    {Value v1=evalNode(p->next,e);
     Value v2=evalNode(p->next->next,e);
     Value v3;
     v3.flag=0;
     v3.link=NULL;
     v3.f=v1.f/v2.f;
     return v3;}
    else if(strcmp(p->S,"%")==0)
    {Value v1=evalNode(p->next,e);
     Value v2=evalNode(p->next->next,e);
     Value v3;
     v3.flag=0;
     v3.link=NULL;
     v3.f=int(v1.f)%int(v2.f);
     return v3;}
    else if(strcmp(p->S,"==")==0)
    {Value v1=evalNode(p->next,e);
     Value v2=evalNode(p->next->next,e);
     Value v3;
     if(v1.f==v2.f)
     {v3.flag=0;
      v3.link=NULL; 
      v3.f=1.0;
      }
     else
     {v3.flag=0;
      v3.link=NULL;
      v3.f=0.0;
      }
     return v3;}
    else if(strcmp(p->S,">=")==0)
     {Value v1=evalNode(p->next,e);
      Value v2=evalNode(p->next->next,e);
      Value v3;
      if(v1.f>=v2.f)
      {v3.flag=0;
       v3.link=NULL;
       v3.f=1.0;}
      else
      {v3.flag=0;
       v3.link=NULL;
       v3.f=0.0;}
      return v3;}
    else if(strcmp(p->S,"<=")==0)
    {Value v1=evalNode(p->next,e);
     Value v2=evalNode(p->next->next,e);
     Value v3;
     if(v1.f<=v2.f)
      {v3.flag=0;
       v3.link=NULL;
       v3.f=1.0;}
      else
      {v3.flag=0;
       v3.link=NULL;
       v3.f=0.0;}
      return v3;}
    else if(strcmp(p->S,">")==0)
    {Value v1=evalNode(p->next,e);
     Value v2=evalNode(p->next->next,e);
     Value v3;
     if(v1.f>v2.f)
      {v3.flag=0;
       v3.link=NULL;
       v3.f=1.0;}
      else
      {v3.flag=0;
       v3.link=NULL;
       v3.f=0.0;}
      return v3;}
    else if(strcmp(p->S,"<")==0)
    {Value v1=evalNode(p->next,e);
     Value v2=evalNode(p->next->next,e);
     Value v3;
     if(v1.f<v2.f)
      {v3.flag=0;
       v3.link=NULL;
       v3.f=1.0;}
      else
      {v3.flag=0;
       v3.link=NULL;
       v3.f=0.0;}
      return v3;}
}
else//if node p is a list  ie p->flag=1.a function call
{Genlistnode *arglist,*body,*curr;
 arglist=p->down->next->down;
 body=p->down->next->next->down;
 p=p->next;
 Env e1;//an inner environment is created
 e1.parent=&e;
 curr=arglist;
     while(curr!=NULL&&p!=NULL)
     {addToEnv(curr->S[0],evalNode(p,e),e1);
      curr=curr->next;
      p=p->next;}
     if(curr==NULL&&p==NULL)
     return evalList(body,e1);
     else
     cout<<"Arguments didn't match\n";

}     
}
//-------------------------------------------------------
Value evalNode(Genlistnode *p,Env& e)
{if(p->flag==0)//ie an atom
 {Value a;
  a.flag=0;
  a.link=NULL;//because its an atom
  if(isnumber(p->S))
  {a.f=number(p->S);
   return a;
   }
  else if(isvar(p->S))
  return FindinEnv(p->S,e);
   }
  
 else//p->flag=1
 {return evalList(p->down,e);}
}
//----------------------------------------------------------
int main()
{char *str;
 Genlistnode* p;
 str=new char[500];
 float ans;
 Env e;//starting environment ie the outermost environment
while(1)
{
 cin.getline(str,500);//str points to the zeroth element ie bracket
 cptr=str;
 char *temp;
 temp=str;
 while(*temp!='\0')
 temp++;
 last=temp;

 //cout<<"Ans:";
 p=makelist();//you parse the string element by element creating a generalised list
 //displaylist(p);
 //p contains the head of the generalised list
 Value ans;//the answer returned is in the form of a value object
 ans=evalNode(p,e);
 if(ans.flag==0)
 cout<<ans.f<<endl;}
return 0;

}
