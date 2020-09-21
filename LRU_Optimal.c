// Page_replacement_LRU_Optimal.cpp : Defines the entry point for the console application.

#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<malloc.h>
#include<stdbool.h>
#include<string.h>


//-------------------------------------LRU-------------------------------------------------------------- 
typedef struct Node{             

	int ele;
	struct Node *left;
	struct Node *right;
}node;

typedef struct Stack{

	node *head;
	int capacity;
	int elements_present;

} stack;

void createStack(int cap,stack *s)
{
	s->head=NULL;
	s->capacity=cap;
	s->elements_present=0;
}

bool isFull(stack *s)
{
	if(s->elements_present==s->capacity) return true;

	return false;
}

bool isEmpty(stack *s)
{
	if(s->elements_present==0)
		 return true;
	return false;
}

void push(stack *s,int ele) //Adding in front of stack
{
	if(isFull(s))  return;

	node *n=(node*)malloc(sizeof(node));
	
	n->left=NULL;
	n->ele=ele;
    n->right=s->head;
	if(s->head!=NULL)
	  s->head->left=n;

	s->head=n;
    
	s->elements_present++;
}

void pop(stack *s)
{
	if(isEmpty(s)) return;
	node *tmp;
	tmp=s->head;

	s->head=tmp->right;
	tmp->right->left=NULL;

	s->elements_present--;
	free(tmp);
}

node *Found_1(stack *s,int ele)
{
	node *tmp;
	int flag=0;
	tmp=s->head;

	while(tmp!=NULL)
	{
		if(tmp->ele==ele)
		{
			flag=1;
			break;
		}

		tmp=tmp->right;
	}

	if(flag==1) 
		return tmp;


	return NULL;
}

void MoveToTop(stack *s,int ele,node *pos)
{
	node *tmp=s->head;
	node *prev=NULL;

	while(tmp!=pos)
	{
		prev=tmp;
		tmp=tmp->right;
	}


	prev->right=tmp->right;
	if(tmp->right!=NULL)
	  tmp->right->left=prev;

	tmp=s->head;
	//Putting the node in front
	tmp->left=pos;
	pos->right=tmp;
	pos->left=NULL;

	s->head=pos;  //Updating the head of stack

}

void PopTheOldest(stack *s)
{
	if(isEmpty(s)) return;

	node *tmp,*prev=NULL;
	tmp=s->head;

	while(tmp->right!=NULL)
	{
		prev=tmp;
		tmp=tmp->right;
	}

	if(prev!=NULL)
	  prev->right=NULL;
	free(tmp);
	s->elements_present--;
}

void PrintStack(node *head)
{
	if(head==NULL)  return;

	PrintStack(head->right);
	printf("%d ",head->ele);
}

//--------------------------------------------------------------------------------------

//------------------------------------Optimal-------------------------------------------
typedef struct Node_ref_time{       
	int ele;
	struct Node_ref_time *next;
}node_r;

typedef struct Node_pg_no{
	int ele;
	node_r *side;
	struct Node_pg_no *down;
}node_pg;

struct PgNo_List{
	node_pg *head;
};

void Add_to_PgNo_List(struct PgNo_List *list ,int ele)
{
	node_pg *n=(node_pg*)malloc(sizeof(node_pg));

	node_pg *tmp=list->head;

	if(tmp==NULL) list->head=n;
	else
	{
		while(tmp->down!=NULL)
		{
			tmp=tmp->down;
		}
	   tmp->down=n;
	}

	
	n->ele=ele;
	n->down=NULL;
	n->side=NULL;
}

void Add_to_List(node_pg *pos,int referenced_at)
{
	node_r *n=(node_r*)malloc(sizeof(node_r));

	if(pos->side==NULL) //No list yet
	{
		pos->side=n;
	}

	else
	{
		node_r *tmp;
		tmp=pos->side;

		while(tmp->next!=NULL)
		{
			tmp=tmp->next;
		}

		tmp->next=n;
	}

	n->next=NULL;
	n->ele=referenced_at;
}

void Traverse(struct PgNo_List *LL)
{
	node_pg *tmp;

	tmp=LL->head;

	while(tmp!=NULL)
	{
		node_r *tmp_1;
		printf("%d --> ",tmp->ele);
		tmp_1=tmp->side;

		while(tmp_1!=NULL)
		{
			printf("%d ",tmp_1->ele);
			tmp_1=tmp_1->next;
		}

		tmp=tmp->down;
		printf("\n");
	}
}

node_pg *Found_2(struct PgNo_List *LL,int pr)
{
	node_pg *tmp;

	tmp=LL->head;
	if(tmp==NULL) return NULL;

	while(tmp!=NULL && tmp->ele!=pr)
	{
		tmp=tmp->down;
	}

	if(tmp==NULL) return NULL;
	else return tmp;
}

int Check(int *stack,int ele,int pf)
{
	int i;

	for(i=0;i<pf;i++)
	{
		if(stack[i]==ele)
			return i;
	}

	return -1;
}

void Remove_this_reference(node_pg *pos)
{
		node_r *t;
		t=pos->side;
		pos->side=t->next;
		free(t);
}

void PrintStack_Optimal(int *stack,int pf)
{
	int i;
	for(i=0;i<pf;i++)
	{
		if(stack[i]!=-1)
			printf("%d ",stack[i]);
	}
}

//---------------------------------------------------------------------


int main()
{

	int ar[50],i=0,pf,n=0;
	float misses=0;

	char ch,str[1000];
	int c=0;
//-----------------------------------------Reading from input file-----------------------------------------------------
    char f[20];
	printf("File name..? ");
	scanf("%s",f);

	FILE *fptr;
	fptr=fopen(f,"r");
	if(fptr==NULL)
		printf("Doesn't exist");

	else
	{
	
		fseek(fptr,25,SEEK_SET);

		while(1)
		{
			ch=fgetc(fptr);
			if(ch=='\n') break;

			str[c++]=ch;
		}
	
		char *pt=strtok(str,", ");
		
		while(pt!=NULL)
		{
			ar[i]=atoi(pt);
			i++;
			pt = strtok (NULL, ", ");
		}

		fseek(fptr,18,SEEK_CUR);

		pf=(int)fgetc(fptr)-48;
	}

	fclose(fptr);
	n=i;
//----------------------------------------------------------------------------------------------------------	

//--------------------------------LRU-----------------------------------------------------------------------
	
	stack *s=(stack *)malloc(sizeof(stack));
	
	createStack(pf,s);
	node *pos;

	for(i=0;i<n;i++)
	{
		pos=Found_1(s,ar[i]);  
		if(pos!=NULL) //Found in the stack
			{
				MoveToTop(s,ar[i],pos); // Bring the page to the top of stack as to indicate that it has been recently referred to
				printf("  ");
		    }
		else //Not found  in stack then push the page
		{
			printf("* ");
			if(isFull(s)) 
			 PopTheOldest(s); //no space in stack then pop the oldest page from stack
			push(s,ar[i]); //push the current one
			misses++;
		}

		PrintStack(s->head);
		printf("\n");
	}

	printf("Page fault ratio in LRU algorithm: %f/%d = %f\n\n",misses,n,misses/n);
//---------------------------------------------------------------------------------------------------------

//---------------------------------Optimal-----------------------------------------------------------------
	int pr;
	misses=0;
	struct PgNo_List *List_pr=(struct PgNo_List*)malloc(sizeof(struct PgNo_List));
	List_pr->head=NULL; //Initializing the list of page numbers

	//------------Creating the reference links-------------------
	for(i=0;i<n;i++)   //Traversing through the sequence
	{
		pr=ar[i];      
		node_pg *index=Found_2(List_pr,pr);

		if(index!=NULL) //If found in the list of unique page numbers 
			Add_to_List(index,i); //Add a node containing the time of reference i.e. 'i' of that page which is present at
		                         // index "index" in the list of unique page numbers
		
		else //If not found in the list of unique page numbers
			{
				Add_to_PgNo_List(List_pr,pr);  //Add it to the list
				node_pg *index_new=Found_2(List_pr,pr); //Now check the index 
				Add_to_List(index_new,i); //Then add the time of reference
		     }
	
	}
	//-----------------------End of creating the list of time of reference of each unique page------------------
	
	int *stack=(int*)malloc(sizeof(int)*pf);

	for(i=0;i<pf;i++)
		stack[i]=-1;

	int count=0,found_in_stack_at,element_in_stack,j,next_fut_ref,farthest,pg_to_be_replaced,last_ref;
	int *in_at=(int*)malloc(sizeof(int)*n); //to keep track of reference time of pages in stack 
	                                                     //used during tie occuring at placement of pages 

	for(i=0;i<n;i++) //Going through the sequence
	{
		pr=ar[i];

		found_in_stack_at=Check(stack,pr,pf);

		if(found_in_stack_at==-1) //Not found in the stack
		  {
			  if(count==pf) //Stack is full
			  {
				  farthest=INT_MIN;
				  last_ref=n;
				  for(j=0;j<pf;j++) //Find out the page to be replaced i.e. the page which has been referenced farthest in the future
				  {
					  element_in_stack=stack[j]; //pick each page from stack
					  node_pg *index=Found_2(List_pr,element_in_stack); 

					  if(index->side==NULL) //Check if it has any reference in future from the given sequence
						  next_fut_ref=INT_MAX; //returning NULL means it has no future reference
					                           //So its the best candidate to be replaced

					  else
						  next_fut_ref=index->side->ele; 

					  if(farthest<=next_fut_ref) 
						 {
							  if(next_fut_ref==INT_MAX)   
								  farthest=next_fut_ref;

							  if(farthest==next_fut_ref)
								{
									if(in_at[j]<last_ref)  //In case of TIE, FSFC order is used to break it
										                   //If 2 pages can be replaced as they won't be referred to in the future, then replace
														   //the page that has been in the stack for the longer time
									  {
										  last_ref=in_at[j];
										  pg_to_be_replaced=j;
									  }
							    }

							  else 
							  {
							     farthest=next_fut_ref;
							     pg_to_be_replaced=j;
							  }
					     }
				  }
				  stack[pg_to_be_replaced]=pr;
				  in_at[pg_to_be_replaced]=i; //pg at index "pg_to_be_replaced"  in stack is added at time 'i'
			  }

			  else{
				  stack[count]=pr;
				  in_at[count]=i;
				  count++;

			  }
			printf("* ");
			misses++;
		  }

		else 
			{
				printf("  ");
				//in_at[found_in_stack_at]=i;
		    }
		node_pg *index=Found_2(List_pr,pr);
        Remove_this_reference(index);
	//	Traverse(List_pr);
		PrintStack_Optimal(stack,pf);
		printf("\n");
	}
	printf("Page fault ratio in Optimal algorithm: %f/%d = %f\n\n",misses,n,misses/n);
//-----------------------------------------------------------------------------------
	getch();
}


/*
20
1 2 3 2 1 5 2 1 6 2 5 6 3 1 3 6 1 2 4 3
3
*/

/*
20
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
3*/


