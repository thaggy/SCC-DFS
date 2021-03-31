#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node * next;
}Node;

typedef struct graph{
    int numVertices;
    //0 == white , 1 == grey, 2 == black
    int* color;
    //Lowest Value
    int* low;
    int* pi;
    Node ** adjList;
    Node * head;
    Node * tail;
}Graph;
//COMPLEXITY : The complexity of this program will be O(V+E) , DFS will run in O(V+E) time and the function that computes OutDegrees is in O(V+E) since it will only run items V times and E times so combining them makes it O(V+E). And in the main function, finding Min and Max takes only O(V), and the creation of the graph takes O(V + E) time as we scan the list E times and the initialization takes O(V) time so O(V+E). and then the Free function simply takes O(V+E) times as we free V nodes and E nodes.


Node* createNode(int);
void ADJList(Graph*, int, int, int);
void PrintADJList(Graph*);
void freeList(Graph*);
int smallest(int,int);
void DFS(Graph*,int);
void OutDegree(Graph*);
void insertAtTail(Graph*,int);

int main(void)
{
    FILE* fp;
    //Two temporary ints used to find search through the list
    int traverseA;
    int traverseB;

    //Ints that will be storing the minimum and maximum ints found to make our list more efficient later
    //Max gets a random dummy value since we know that -1 can't be the max
    int max=-1;
    //Min is different since technically a negative number would be lower than any number such as 0 or 1 so we will have to take a specific measure to fix this
    int min = -1;

    //Finding file which I have named in this case mcf for Machine C file
    fp = fopen("mcf.txt", "r");
    
    //If its not found, exit the program to avoid seg faults or anything bad
    if (!fp)
    {
        printf("File not found\n");
        exit(0);
    }
    
    //Finding Min and Max by reading file until EOF is hit
    while (fscanf(fp,"%d %d\n", &traverseA, &traverseB) != EOF)
    {
        if (min == -1)
        {
            min = traverseA;
        }
        //Searching for lowest number
        if (min > traverseA)
        {
            min = traverseA;
        }
        else if (min > traverseB)
        {
            min = traverseB;
        }

        //Finding max number
        if (max < traverseA)
        {
            max = traverseA;
        }
        if (max < traverseB)
        {
            max = traverseB;
        }
    }

    //Rewinding so that the file is usable
    rewind(fp);
    
    //Creating head of our list
    Graph * list_ptr = malloc(sizeof(Graph));
    list_ptr->adjList = (Node**)malloc((max+1) * sizeof(Node));
    list_ptr->color = malloc((max+1)*sizeof(int));
    list_ptr->low = malloc((max+1)*sizeof(int));
    list_ptr->pi = malloc((max+1)*sizeof(int));
    list_ptr->head = NULL;
    list_ptr->tail = NULL;
    for(int i = min; i < max+1; i++)
    {
        list_ptr->adjList[i] = createNode(i);
        list_ptr->color[i] = 0;
        list_ptr->low[i] = -1;
        list_ptr->pi[i] = -1;
    }
    list_ptr->numVertices = max;
    //Reading Ints from file for DIRECTED GRAPH
    for(int i = 1; i < max+1; i++)
    {
        while (fscanf(fp, "%d %d", &traverseA, &traverseB) != EOF)
        {
           // printf("%d %d, %d\n",traverseA,traverseB,i);
            ADJList(list_ptr, i, traverseA, traverseB);
        }
        rewind(fp);
    }
    
    PrintADJList(list_ptr);
    for(int i = min; i < max+1; i++)
        if(list_ptr->color[i] == 0)
            DFS(list_ptr, i);
    for(Node * temp = list_ptr->head; temp !=NULL;temp = temp->next)
    {
        printf("%d ",temp->data);
    }
    printf("\n");
    OutDegree(list_ptr);
    freeList(list_ptr);
    return 0;
}

//O(1) does not traverse anything
void insertAtTail(Graph* ptr, int insert)
{
    Node * insertion = createNode(insert);
    if(!ptr->tail)
    {
        //printf("Adding to link list - %d\n",insert);
        //The list is empty
        ptr->tail = insertion;
        ptr->head = insertion;
    }
    else
    {
        if(insert != ptr->tail->data)
        {
           //printf("Adding to link list - %d\n",insert);
            ptr->tail->next = insertion;
            ptr->tail = insertion;
        }
    }
}
//O(1) does not traverse anything
Node* createNode(int data)
{
    //Creating Node
    Node* temp = NULL;
    temp = malloc(sizeof(Node));
    //If the node is empty, malloc did something wrong so we should exit the code
    if (!temp)
    {
        printf("Oops! Something went wrong!");
        exit(9);
    }
    //Setting all the values to the desired values
    temp->data = data;
    temp->next = NULL;
    return temp;
}

//O(E) time
void PrintADJList(Graph* head)
{
    //Print ADJ List
    for(int i = 1; i < head->numVertices + 1; i++)
    {
        printf("%d - ",head->adjList[i]->data);
        if(head->adjList[i]->next)
        {
            for(Node * temp = head->adjList[i]->next; temp != NULL; temp = temp->next)
            {
                printf("%d ",temp->data);
            }
        }
        printf("\n");
    }
}

//O(V)
void ADJList(Graph * head, int check, int first, int second)
{
    //Looping through the list
    for(int i = 1; i < head->numVertices+1; i++)
    {
        //If the data, is in first or second, we want to insert the opposite
        if(head->adjList[i]->data == first && check == first)
        {
            Node * insert = createNode(second);
            insert->next = head->adjList[i]->next;
            head->adjList[i]->next = insert;
        }
    }
}

//O(V+E)
void freeList(Graph* head)
{
    Node * temp;
    Node * trail;
    for(int i=0;i<head->numVertices+1;i++)
    {
        temp = head->adjList[i];
        if(!head->adjList[i])
        {
            //The list there doesn't exist
        }
        else
        {
            while(temp->next != NULL)
            {
                trail = temp;
                temp = temp->next;
                free(trail);
            }
        }
        free(temp);
    }
    temp= head->head;
    while(temp->next != NULL)
    {
        trail = temp;
        temp = temp->next;
        free(trail);
    }
    free(temp);
    free(head);
}
//O(1)
int smallest(int a, int b)
{
    if(a > b)
        return b;
    else
        return a;
}
//O(V+E)
void DFS(Graph* ptr, int vertex)
{
    //DFS runs in O(V+E)
    //Setting lowest value of vertex equal to itself as it is the currently lowest discovered value
    ptr->low[vertex] = vertex;
    //Setting Color to grey
    ptr->color[vertex] = 1;
    //For each adj of Vertex
    for(Node* temp = ptr->adjList[vertex]->next;temp != NULL;temp = temp->next)
    {
        //If the color is white
        if(ptr->color[temp->data] == 0)
        {
            //Set its predecessor
            ptr->pi[temp->data] = vertex;
            //Call DFS on the discovered DFS
            DFS(ptr, temp->data);
            //See which Vertex's connected lowest value is smaller
            ptr->low[vertex] = smallest(ptr->low[vertex],ptr->low[temp->data]);
        }
        //If a back edge is found, see which has the smallest lowest connected value
        else if(ptr->color[temp->data] == 1 && ptr->pi[vertex] != temp->data)
        {
            ptr->low[vertex] = smallest(vertex, ptr->low[temp->data]);
        }
        else if(ptr->color[temp->data] == 1 && ptr->pi[vertex] == temp->data)
        {
            ptr->low[vertex] = smallest(vertex, ptr->low[temp->data]);
        }
    }
    //Set color to black
    ptr->color[vertex] = 2;

    insertAtTail(ptr, ptr->low[vertex]);
}

//O(V+E)
void OutDegree(Graph* ptr)
{
    //Counter values that will be used later
    int counter = 0;
    //Creating 2D array
    int** arr = calloc((ptr->numVertices+1),(ptr->numVertices+1)*sizeof(int*));
    //O(V)
    for(int i =0; i<ptr->numVertices+1;i++)
    {
        arr[i] = calloc((ptr->numVertices+1),(ptr->numVertices+1)*sizeof(int));
    }
    //Initializing 2D array
    //O(V)
    for(int i=1;i<ptr->numVertices+1;i++)
    {
        arr[ptr->low[i]][0] = ptr->low[i];
    }
    //Counting How many non zero (Total SCC there Are)
    //O(V)
    for(int i = 0; i < ptr->numVertices+1;i++)
    {
        if(arr[i][0]!=0)
            counter++;
    }
    //Checking where all the SCCs connect
    //O(E)
    for(int i = 1; i<ptr->numVertices+1; i++)
    {
        for(Node * temp = ptr->adjList[i]->next; temp !=NULL; temp = temp->next)
        {
            if(ptr->low[temp->data] != ptr->low[i])
                arr[ptr->low[i]][ptr->low[temp->data]] = 1;
        }
    }
    int finalCountdown = 0;
    //Counting how many SCCs are connected
    //O(V)
    for(Node * temp = ptr->head; temp!=NULL;temp = temp->next)
    {
        for(int i = 1; i<ptr->numVertices+1; i++)
        {
            finalCountdown += arr[temp->data][i];
        }
        printf("%d ",finalCountdown);
        finalCountdown = 0;
    }
    printf("\n");
    //Freeing memory
    //O(V)
    for(int i =0; i<ptr->numVertices+1;i++)
    {
        free(arr[i]);
    }
    free(arr);
}
