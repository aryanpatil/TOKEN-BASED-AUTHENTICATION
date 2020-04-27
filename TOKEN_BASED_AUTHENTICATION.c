#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX 3

// GLOBAL VARIABLES
int T=0;
int N=0;

typedef enum{PLATINUM, DIAMOND, GOLD, REGULAR} Membership_level_tag;

typedef enum{L1, L2} Sub_access_level_tag;

// MEMBERSHIP HISTORY STRUCTURE
typedef struct History
{
    int time;
    Membership_level_tag prev_membership;
    Membership_level_tag new_membership;
    struct History *next;
}   history;

// CUSTOMER INFO STRUCTURE
typedef struct Customer_info
{
    int user_id;
    char password[10];
    Membership_level_tag membership_level;
    Sub_access_level_tag sub_access_level;
    history *membership_history;
    int renewal_time;
    struct Customer_info *left;
    struct Customer_info *right;
    int height;
}   customer_info;

// TOKEN STRUCTURE
typedef struct Token_node_tag
{
    int token_no;
    int user_id;
    int arrival_time;
    int waiting_time;
    struct Token_node_tag *next;
}   token_node;

// TOKEN TREE STRUCTURE
typedef struct Token_tree
{
    int token_no;
    int user_id;
    int arrival_time;
    int waiting_time;
    struct Token_tree *left;
    struct Token_tree *right;
    int height;
}   token_tree;

// QUEUE TAG
typedef struct Queue_tag
{
    token_node *front;
    token_node *rear;
}   queue;

// PRIORITY QUEUE TAG
typedef struct Priority_q_tag
{
    queue active;
    queue waiting;
    int n;
    int w_n;
}   pq_type;

pq_type pq[4][2];


// DEPTH OF TREE OR SUBTREE
int depth(customer_info *root)
{
    int retval,lh,rh;

    if(root==NULL)
    {
        retval=-1;
    }
    else if((root->left==NULL)&&(root->right==NULL))
    {
        retval=0;
    }
    else
    {
        lh=depth(root->left);
        rh=depth(root->right);

        if(lh > rh)
        {
            retval = 1 + lh;
        }
        else
        {
            retval = 1 + rh;
        }
        return retval;
    }
}

// TREE ROTATE LEFT
customer_info *rotate_left(customer_info *root)
{
    customer_info *temp;
    if((root!=NULL)&&(root->right!=NULL))
    {
        
        temp=root->right;
        root->right=temp->left;
        temp->left=root;
        root->height=depth(root);
        temp->height=depth(temp);
    }
    return temp;
}

// TREE ROTATE RIGHT
customer_info *rotate_right(customer_info *root)
{
    customer_info *temp;
    if((root!=NULL)&&(root->left!=NULL))
    {
        
        temp=root->left;
        root->left=temp->right;
        temp->right=root;
        root->height=depth(root);
        temp->height=depth(temp);
    }
    return temp;
}

// PARENT AND CHILD BOTH IMBALANCED ON RIGHT
customer_info *RR(customer_info *root)
{
    root=rotate_left(root);
    return root;
}

// PARENT AND CHILD BOTH IMBALANCED ON LEFT
customer_info *LL(customer_info *root)
{
    root=rotate_right(root);
    return root;
}

// PARENT ON LEFT AND CHILD ON RIGHT
customer_info *LR(customer_info *root)
{
    root->left=rotate_left(root->left);
    root=rotate_right(root);
    return root;
}

// PARENT ON RIGHT AND CHILD ON LEFT
customer_info *RL(customer_info *root)
{
    root->right=rotate_left(root->right);
    root=rotate_left(root);
    return root;
}

// BALANCE FACTOR OF A NODE
int BF(customer_info *root)
{
    int lh,rh;
    
    if(root==NULL)
    {
        return 0;
    }

    if(root->left==NULL)
    {
        lh=0;
    }
    else
    {
        lh = 1 + root->left->height;
    }

    if(root->right==NULL)
    {
        rh=0;
    }
    else
    {
        rh = 1 + root->right->height;
    }

    return (lh-rh);
}

// INSERTING CUSTOMER IN DATABASE
customer_info *insert(customer_info *root, customer_info **info)
{
    if(root==NULL)
    {
        root=*info;
    }
    else if((*info)->user_id > root->user_id)
    {
        root->right=insert(root->right,info);
        if(BF(root)==-2)
        {
            if((*info)->user_id > root->right->user_id)
            {
                root=RR(root);
            }
            else
            {
                root=RL(root);
            }   
        }
    }
    else if((*info)->user_id < root->user_id)
    {
        root->left=insert(root->left,info);
        if(BF(root)==2)
        {
            if((*info)->user_id < root->left->user_id)
            {
                root=LL(root);
            }
            else
            {
                root=LR(root);
            }
        }
    }
    root->height = depth(root);
    return root;
}

// SEARCHING FOR CUSTOMER IN DATABASE
customer_info *search(customer_info *root, int data)
{
    customer_info *retval=NULL;
    if(root!=NULL)
    {
        if(root->user_id == data)
        {
            retval=root;
        }
        else if(root->user_id < data)
        {
            retval=search(root->right,data);
        }
        else
        {
            retval=search(root->left,data);
        }
    }
    return retval;
}

// UPDATING CUSTOMER MEMBERSHIP
void update_membership(customer_info **root)
{
    int id;
    char pswd[10];
    printf("Enter user_id:\n");
    scanf("%d",&id);

    printf("Enter password:\n");
    scanf("%s",pswd);

    customer_info *ptr;
    ptr=search(*root,id);

    if(ptr==NULL)
    {
        printf("Invalid Username!\n");
    }
    else if(strcmp(ptr->password,pswd)!=0)
    {
        printf("Invalid password!\n");
    }
    else
    {
        if(ptr->membership_level==REGULAR)
        {
            printf("Sorry! Regular customers cannot update their membership.\n");
        }
        else if(ptr->membership_level==PLATINUM)
        {
            printf("You are already a Platinum member!");
        }
        else
        {
            if(ptr->membership_level==GOLD)
            {
                ptr->membership_level=DIAMOND;
                printf("Membership updated from Gold to Diamond successfully!\n");
                
                history *new;
                new=(history*)malloc(sizeof(history));
                new->next = NULL;
                new->prev_membership=GOLD;
                new->new_membership=DIAMOND;
                new->time=T;
                if(ptr->membership_history!=NULL)
                {
                    new->next=ptr->membership_history;
                }
                ptr->membership_history=new;
            }
            else
            {
                ptr->membership_level=PLATINUM;
                printf("Membership updated from Diamond to Platinum successfully!\n");

                history *new;
                new=(history*)malloc(sizeof(history));
                new->next=NULL;
                new->prev_membership=DIAMOND;
                new->new_membership=PLATINUM;
                new->time=T;
                if(ptr->membership_history!=NULL)
                {
                    new->next=ptr->membership_history;
                }
                ptr->membership_history=new;
            }
        }
    }
}

// VIEWING MEMBERSHIP HISTORY OF A CUSTOMER
void view_membership_history(customer_info **root)
{
   int id;
    char pswd[10];
    printf("Enter user_id:\n");
    scanf("%d",&id);

    printf("Enter password:\n");
    scanf("%s",pswd);

    customer_info *ptr;
    ptr=search(*root,id);

    if(ptr==NULL)
    {
        printf("Invalid Username!\n");
    }
    else if(strcmp(ptr->password,pswd)!=0)
    {
        printf("Invalid password!\n");
    }
    else
    {
        if(ptr->membership_history==NULL)
        {
            printf("No history available!\n");
        }
        else
        {
            history *p=ptr->membership_history;
            printf("TIME\tOLD\tNEW\n");
            while(p!=NULL)
            {
                printf("%d\t%d\t%d\n",p->time,p->prev_membership,p->new_membership);
                p = p->next;
            }
        }
    }     
}

/*  
    QUEUE AUX FUNCTIONS
*/


// INITIALIZING QUEUES
void initialiseQ()
{
    int i=0,j=0;

    for(i=0;i<4;i++)
    {
        for(j=0;j<2;j++)
        {
            pq[i][j].active.front = pq[i][j].active.rear = NULL;
            pq[i][j].waiting.front = pq[i][j].waiting.rear = NULL;
            pq[i][j].n = pq[i][j].w_n = 0;
        }
    }
}

// DISPLAY ACTIVE AND WAITING LIST
void displayQ()
{
    char *mem[4] = {"PLATINUM", "DIAMOND", "GOLD", "REGULAR"};
    char *lvl[2] = {"L1", "L2"};

    int i=0,j=0;

    token_node *ptr;

    for(i=0;i<4;i++)
    {
        printf("%s:\n",mem[i]);
        printf("_________\n");
        for(j=0;j<2;j++)
        {
            printf("%s:\n",lvl[j]);
            printf("ACTIVE:\n");
            printf("TOKEN_NO\tUSER_ID\t\tWAIT_TIME\tARRIVAL\n");

            // PRINTING ACTIVE LIST
            ptr = pq[i][j].active.front;
            while(ptr!=NULL)
            {
                printf("%d\t\t%d\t\t%d\t\t%d\n", ptr->token_no, ptr->user_id, ptr->waiting_time, ptr->arrival_time);
                ptr=ptr->next;
            }
            printf("______________________________________________________________________________\n");
            printf("WAITING:\n");
            printf("TOKEN_NO\tUSER_ID\t\tWAIT_TIME\tARRIVAL\n");

            // PRINTING WAIT LIST
            ptr = pq[i][j].waiting.front;
            while(ptr!=NULL)
            {
                printf("%d\t\t%d\t\t%d\t\t%d\n", ptr->token_no, ptr->user_id, ptr->waiting_time, ptr->arrival_time);
                ptr=ptr->next;
            }
            printf("______________________________________________________________________________\n");
            printf("\n");
        }
    }
}

// INSERTING IN ACTIVE LIST
void insert_active(token_node **new, int i1, int i2)
{
    // IF ACTIVE QUEUE IS EMPTY
    if((pq[i1][i2].active.front==NULL)&&(pq[i1][i2].active.rear==NULL))
    {
        pq[i1][i2].active.front = pq[i1][i2].active.rear = *new;
    }
    // IF ACTIVE QUEUE IS NON-EMPTY
    else
    {
        pq[i1][i2].active.rear->next = *new;
        pq[i1][i2].active.rear = *new;
    }

    // INCREMENTING COUNT
    pq[i1][i2].n++;

}

// INSERTING IN WAIT LIST
void insert_wait(token_node **new, int i1, int i2)
{
    // IF WAITING QUEUE IS EMPTY
    if((pq[i1][i2].waiting.front==NULL)&&(pq[i1][i2].waiting.rear==NULL))
    {
        pq[i1][i2].waiting.front = pq[i1][i2].waiting.rear = *new;
    }
    // IF ACTIVE QUEUE IS NON-EMPTY
    else
    {
        pq[i1][i2].waiting.rear->next = *new;
        pq[i1][i2].waiting.rear = *new;
    }
    pq[i1][i2].w_n++;
}


/* 
   AUXILLIARY FUNCTIONS FOR TOKEN TREE
   -----------------------------------
*/


// DEPTH OF TOKEN_TREE OR SUBTREE
int depth_token(token_tree *root)
{
    int retval,lh,rh;

    if(root==NULL)
    {
        retval=-1;
    }
    else if((root->left==NULL)&&(root->right==NULL))
    {
        retval=0;
    }
    else
    {
        lh=depth_token(root->left);
        rh=depth_token(root->right);

        if(lh > rh)
        {
            retval = 1 + lh;
        }
        else
        {
            retval = 1 + rh;
        }
        return retval;
    }
}

// TOKEN_TREE ROTATE LEFT
token_tree *rotate_left_token(token_tree *root)
{
    token_tree *temp;
    if((root!=NULL)&&(root->right!=NULL))
    {
        
        temp=root->right;
        root->right=temp->left;
        temp->left=root;
        root->height=depth_token(root);
        temp->height=depth_token(temp);
    }
    return temp;
}

// TOKEN_TREE ROTATE RIGHT
token_tree *rotate_right_token(token_tree *root)
{
    token_tree *temp;
    if((root!=NULL)&&(root->left!=NULL))
    {
        
        temp=root->left;
        root->left=temp->right;
        temp->right=root;
        root->height=depth_token(root);
        temp->height=depth_token(temp);
    }
    return temp;
}

// PARENT AND CHILD BOTH IMBALANCED ON RIGHT
token_tree *RR_token(token_tree *root)
{
    root=rotate_left_token(root);
    return root;
}

// PARENT AND CHILD BOTH IMBALANCED ON LEFT
token_tree *LL_token(token_tree *root)
{
    root=rotate_right_token(root);
    return root;
}

// PARENT ON LEFT AND CHILD ON RIGHT
token_tree *LR_token(token_tree *root)
{
    root->left=rotate_left_token(root->left);
    root=rotate_right_token(root);
    return root;
}

// PARENT ON RIGHT AND CHILD ON LEFT
token_tree *RL_token(token_tree *root)
{
    root->right=rotate_left_token(root->right);
    root=rotate_left_token(root);
    return root;
}

// BALANCE FACTOR OF A TOKEN_TREE_NODE
int BF_token(token_tree *root)
{
    int lh,rh;
    
    if(root==NULL)
    {
        return 0;
    }

    if(root->left==NULL)
    {
        lh=0;
    }
    else
    {
        lh = 1 + root->left->height;
    }

    if(root->right==NULL)
    {
        rh=0;
    }
    else
    {
        rh = 1 + root->right->height;
    }

    return (lh-rh);
}

// INSERTING TOKEN IN TOKEN_TREE
token_tree *insert_token(token_tree *info, token_tree *root)
{
    if(root==NULL)
    {
        root=info;
    }
    else if(info->token_no > root->token_no)
    {
        root->right=insert_token(info, root->right);
        if(BF_token(root)==-2)
        {
            if(info->token_no > root->right->token_no)
            {
                root=RR_token(root);
            }
            else
            {
                root=RL_token(root);
            }   
        }
    }
    else if(info->token_no < root->token_no)
    {
        root->left=insert_token(root->left,info);
        if(BF_token(root)==2)
        {
            if(info->token_no < root->left->token_no)
            {
                root=LL_token(root);
            }
            else
            {
                root=LR_token(root);
            }
        }
    }
    root->height = depth_token(root);
    return root;
}


// DELETING TOKEN FROM TREE
token_tree *Delete_token(token_tree *T,int x)
{
	token_tree *p;
	
	if(T==NULL)
	{
		return NULL;
	}
	else
		if(x > T->token_no)		// insert in right subtree
		{
			T->right=Delete_token(T->right,x);
			if(BF_token(T)==2)
				if(BF_token(T->left)>=0)
					T=LL_token(T);
				else
					T=LR_token(T);
		}
		else
			if(x < T->token_no)
			{
				T->left=Delete_token(T->left,x);
				if(BF_token(T)==-2)	//Rebalance during windup
					if(BF_token(T->right)<=0)
						T=RR_token(T);
					else
						T=RL_token(T);
			}
			else
			{
				//data to be deleted is found
				if(T->right!=NULL)
				{	//delete its inorder succesor
					p=T->right;
					
					while(p->left!= NULL)
						p=p->left;  
					
					T->token_no=p->token_no;
                    T->arrival_time=p->arrival_time;
                    T->user_id=p->user_id;
                    T->waiting_time=p->waiting_time;
					T->right=Delete_token(T->right,p->token_no);
					
					if(BF_token(T)==2)//Rebalance during windup
						if(BF_token(T->left)>=0)
							T=LL_token(T);
						else
							T=LR_token(T);
				}
				else
					return(T->left);
			}
	T->height=depth_token(T);
	return(T);
}







// CREATING TOKEN
void create_token(token_tree **root, customer_info **cust)
{
    int id;
    char pswd[10];
    printf("Enter user_id:\n");
    scanf("%d",&id);

    printf("Enter password:\n");
    scanf("%s",pswd);

    customer_info *ptr;
    ptr=search(*cust,id);

    if(ptr==NULL)
    {
        printf("Invalid Username!\n");
    }
    else if(strcmp(ptr->password,pswd)!=0)
    {
        printf("Invalid password!\n");
    }
    else
    {
        token_node *new;
        new = (token_node*)malloc(sizeof(token_node));
        new->token_no = N++;
        new->user_id = id;
        new->arrival_time = T;
        new->next = NULL;

       

        // INSERTING TOKEN IN QUEUE
        int index1,index2;
        index1 = ptr->membership_level;
        index2 = ptr->sub_access_level;

        if(pq[index1][index2].n==MAX)
        {
            insert_wait(&new, index1, index2);
            // FOR WAITING TIME, IT WILL BE 10 MORE THAN ITS 3RD PREDECESSOR
            token_node *ptr;
            int i=0,j=0;

            if(pq[index1][index2].w_n <=MAX)                // IF ELEMENTS IN WAIT LIST ARE LESS THAN MAX
            {
                ptr = pq[index1][index2].active.front;

                for(i=0;i<pq[index1][index2].w_n - 1;i++)
                {
                    ptr = ptr->next;
                }

                pq[index1][index2].waiting.rear->waiting_time = 10 + ptr->waiting_time;
            }
            else                                            // ELEMENTS IN WAIT LIST ARE MORE THAN MAX
            {
                ptr = pq[index1][index2].waiting.front;
                j = pq[index1][index2].w_n - MAX - 1;

                for(i=0;i<j;i++)
                {
                    ptr = ptr->next;
                }

                pq[index1][index2].waiting.rear->waiting_time = 10 + ptr->waiting_time;

            }
        }
        else
        {
            insert_active(&new, index1, index2);
            new->waiting_time = 10;
        }

        // PRINTING OUTPUT
        printf("YOUR TOKEN NO.%d IS READY!\nWAITING TIME: %d units\n",new->token_no,new->waiting_time);

        // INSERTING TOKEN IN TOKEN_TREE
        token_tree *nptr;
        nptr = (token_tree*)malloc(sizeof(token_tree));
        nptr->user_id = new->user_id;
        nptr->token_no = new->token_no;
        nptr->arrival_time = new->arrival_time;
        nptr->waiting_time = new->waiting_time;
        nptr->left = NULL;
        nptr->right = NULL;
        nptr->height = 0;
        
        *root = insert_token(nptr, *root);
    }
}

/* 
    FUNCTIONS RELATED TO TIME 
*/

// DECREASING RENEWAL TIME FOR CUSTOMERS
void renewal_time_decrease(customer_info *root, int t)
{
    customer_info *ptr = root;
    if(ptr!=NULL)                                      // INORDER TRAVERSAL
    {
        renewal_time_decrease(root->left, t);

        ptr->renewal_time -= t;
        if(ptr->renewal_time < 1)
        {
            while(ptr->renewal_time <= 0)
            {
                ptr->renewal_time = 30 + ptr->renewal_time;
            }
            if(ptr->membership_level!=REGULAR)
            {
                history *new;
                new=(history*)malloc(sizeof(history));
                new->next=NULL;
                new->prev_membership=ptr->membership_level;
                new->new_membership=REGULAR;
                new->time=T;
                
                if(ptr->membership_history!=NULL)
                {
                    new->next=ptr->membership_history;
                }
                ptr->membership_history=new;

                ptr->membership_level = REGULAR;
            }    
        }

        renewal_time_decrease(root->right, t);
    }
}

// UPDATING ACTIVE AND WAIT LIST
void update_list(token_tree **root, int t)
{
    token_node *ptr;
    int i=0,j=0,k=0;
    
    for(i=0;i<4;i++)
    {
        for(j=0;j<2;j++)
        {
            // DECREMENTING WAITING TIME FOR CUSTOMERS
            ptr=pq[i][j].active.front;
            while(ptr!=NULL)
            {
                ptr->waiting_time -= t;
                ptr = ptr->next;
            }

            ptr=pq[i][j].waiting.front;
            while(ptr!=NULL)
            {
                ptr->waiting_time -= t;
                ptr = ptr->next;
            }
            // VALIDATING ACTIVE & WAIT LIST
            token_node *temp;
            ptr=pq[i][j].active.front;
            for(k=0;(k<MAX)&&(ptr!=NULL);k++)
            {
                if(ptr->waiting_time <= 0)
                {
                    temp = ptr;
                    // PRINTING OUTPUT
                    printf("DELETING TOKEN NO.%d AS SERVICES HAVE BEEN AVAILED!\n",temp->token_no);
                    // DELETING TOKEN FROM TREE
                    *root = Delete_token(*root, ptr->token_no);
                    ptr = ptr->next;
                    pq[i][j].active.front = ptr;
                    free(temp);
                    pq[i][j].n--;

                    if(pq[i][j].w_n > 0)
                    {
                        temp = pq[i][j].waiting.front;
                        // SHIFT FROM WAIT TO ACTIVE LIST;
                        insert_active(&temp, i, j);
                        pq[i][j].waiting.front = temp->next;
                        temp->next=NULL;
                        pq[i][j].w_n--;
                    }

                }
                else
                {
                    ptr = ptr->next;
                }
            }


            
        }
    }
}

// SKIP TIME FUNCTION
void skip_time(customer_info *root, token_tree **t_root)
{
    int t;
    printf("Enter the time to be skipped in units:\n");
    scanf("%d",&t);
    T+=t;

    // 1. DECREASING RENEWAL TIME FOR CUSTOMERS

    renewal_time_decrease(root, t);

    // 2. UPDATING THE ACTIVE AND WAIT LIST

    update_list(t_root, t);
}

// PRINTING CHANGES IN NEW FILE
void print_output(customer_info *root, FILE *fp)
{
    if(root!=NULL)
    {
        print_output(root->left, fp);
        fprintf(fp,"%d %s %u %u %d\n",root->user_id, root->password, root->membership_level, root->sub_access_level, root->renewal_time);
        print_output(root->right, fp);
    }
}

void printfile(customer_info *root)
{
    FILE *fp;
    fp = fopen("output_file.txt","w");
    print_output(root,fp);
    fclose(fp);
}

void main()
{
    // CREATION OF DATABASE IN AVL TREE
    customer_info *database=NULL;
    customer_info *new;
    int fil=0;
    FILE *fp;
    fp = fopen("input_file.txt","r");
    while(fil<40)
    {
        new=(customer_info*)malloc(sizeof(customer_info));
        fscanf(fp,"%d %s %u %u %d",&new->user_id,new->password,&new->membership_level,&new->sub_access_level,&new->renewal_time);
        new->membership_history=NULL;
        new->left = new->right = NULL;
        new->height = 0;
        // INSERTING NEW TREE NODE
        database=insert(database,&new);
        fil++;
    }
    fclose(fp);

    // INITIALISING QUEUE STRUCTURES
    initialiseQ();

    // CREATING TOKEN TREE
    token_tree *token_root=NULL;
    
    // CONSOLE OUTPUT
    int input = 0, exit = 0;
    while(exit==0)
    {
        printf("Enter input:\n1. Create new token\n2. Display waiting and active list\n3. Update Membership\n4. View membership history\n5. Skip time\n6. Exit\n");
        scanf("%d",&input);
        switch(input)
        {
            case 1: create_token(&token_root, &database);
                    break;

            case 2: displayQ();
                    break;

            case 3: update_membership(&database);
                    break;

            case 4: view_membership_history(&database);
                    break;

            case 5: skip_time(database, &token_root);
                    break;

            case 6: exit = 1;
                    printfile(database);
                    break;

            default: printf("Invalid Input!\n");
        }
    }
}