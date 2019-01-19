#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <iostream>

inline int max (int a, int b)
{
       return (a>b ? a : b);
}

#define XOR(a,b) ((a && !b) || (!a && b))

const int NODE_CHILDREN_MAX = 128;

class Node_t;
typedef bool (*PROCESS_FUNC)(Node_t* node);

extern int NS_UNDONE;
extern int NS_DOING;
extern int NS_DONE;

class Cell_t;

extern Cell_t* p1StartCell;
extern Cell_t* curMaxCell;
extern int     curStatusVal;
extern bool    working;

enum CELL_STATE
{
     CS_EMPTY,
     CS_P1_ALIVE,
     CS_P1_DEAD,
     CS_P2_ALIVE,
     CS_P2_DEAD
};


void getAvailableSteps (Cell_t* curCell, Cell_t**   steps, int*       size, int player);
bool DFS_DeadChain     (Cell_t* cell,    CELL_STATE s1,    CELL_STATE s2);

void foeSteps          ();
bool heuristic_func    (Node_t* node);
void mySteps           (char* dest, int numAvailbaleSteps);

void changeStateParams ();

struct Node_t
{
public:
       Node_t  (void* input);
       
       void add (Node_t* node);
       
       Node_t* parent;
       Node_t* children [NODE_CHILDREN_MAX];
       int     childrenNum;
       
       int     state;
       void*   data;
       
};

class CGraph
{
public:
       Node_t* top;
       
       void BFS   (PROCESS_FUNC pProcFunc);
       void clear ();
      
};

int NS_UNDONE = 0;
int NS_DOING  = 1;
int NS_DONE   = 2;

void changeStateParams ()
{
     if (NS_UNDONE == 0)
     {
        NS_UNDONE = 2;
        NS_DONE   = 0;
     }
     else
     {
        NS_UNDONE = 0;
        NS_DONE   = 2;
     }
}

Node_t :: Node_t (void* input) :
       parent      (NULL),
       childrenNum (0),
       state       (NS_UNDONE),
       data        (input)
{}

void Node_t :: add (Node_t* node)
{
     children [childrenNum] = node;
     node->parent           = this;
     
     childrenNum++;
     
}

struct QueueNode_t
{
       QueueNode_t* prev;
       QueueNode_t* next;
       
       void*        data;
};

class CQueue
{
public:       
       CQueue      ();
       
       void  push  (void* data);
       void* pop   ();
       void  clear ();

private:
       QueueNode_t* firstNode;
       QueueNode_t* lastNode;
        
};

CQueue :: CQueue () :
       firstNode (NULL),
       lastNode  (NULL)
{
}

void CQueue :: clear ()
{
       while (pop () != NULL);
}

void CQueue :: push (void* data)
{
     QueueNode_t* newNode = new QueueNode_t ();
     
     if (firstNode != NULL)
     {
        lastNode->next = newNode;
        newNode ->prev = lastNode;
     }
     else
     {
         firstNode     = newNode;         
         newNode->prev = NULL;
     }
     
     newNode->next = NULL;
     newNode->data = data;
     
     lastNode = newNode;
}

void* CQueue :: pop ()
{
      if (firstNode == NULL) return NULL;
      
      QueueNode_t* next = firstNode->next;
      void*        data = firstNode->data;
      
      delete firstNode;
      firstNode = next;
      
      return data;      
}

void CGraph :: BFS (PROCESS_FUNC pProcFunc)
{
     CQueue* pQueue  = new CQueue ();
     bool    working = true;
     
     pQueue->push (top);
     int numToPop     = 1;
     int numToPopNext = 0;
     
     while (working)
     {
           for (int i = 0; i < numToPop; i++)
           {
               Node_t* node = (Node_t*) pQueue->pop ();
               working      = pProcFunc (node);
               
               for (int j = 0; j < node->childrenNum; j++)
               {
                   pQueue->push (node->children [j]);
               }
               numToPopNext += node->childrenNum;
           }
           numToPop     = numToPopNext;
           numToPopNext = 0;
           
           if (numToPop == 0) working = false;
     }
     
     pQueue->clear ();
     delete pQueue;
}
void CGraph :: clear ()
{
     CQueue* pQueue  = new CQueue ();
     bool    working = true;
     
     pQueue->push (top);
     int numToPop     = 1;
     int numToPopNext = 0;
     
     while (working)
     {
           for (int i = 0; i < numToPop; i++)
           {
               Node_t* node = (Node_t*) pQueue->pop ();
               for (int j = 0; j < node->childrenNum; j++)
               {
                   pQueue->push (node->children [j]);
               }
               numToPopNext += node->childrenNum;
               delete node;
           }
           numToPop     = numToPopNext;
           numToPopNext = 0;
           
           if (numToPop == 0) working = false;
     }
     
     pQueue->clear ();
     delete pQueue;
}

struct POINT_t
{
       int x, y;
};

POINT_t add     (POINT_t  a, POINT_t  b);
bool    isEqual (POINT_t  a, POINT_t  b);
int     dist    (POINT_t* a, POINT_t* b);

const POINT_t shifts [8] = {{ 1,  0},
                            { 1, -1},
                            { 0, -1},
                            {-1, -1},
                            {-1,  0},
                            {-1,  1},
                            { 0,  1},
                            { 1,  1}};

POINT_t add (POINT_t a, POINT_t b)
{
    POINT_t pp = {a.x+b.x, a.y+b.y};
    return pp;
}

bool isEqual (POINT_t a, POINT_t b)
{
     return a.x == b.x && a.y == b.y;
}

int dist (POINT_t* a, POINT_t* b)
{
    return abs (a->x - b->x) +
           abs (a->y - b->y - abs (a->x - b->x));
}


struct StackNode_t
{
       StackNode_t* prev;
       void*        data;
};

class CStack
{
public:       
       CStack     ();
       
       void  push (void* data);
       void* pop  ();
       void clear ();

private:
       StackNode_t* curNode;
        
};

CStack :: CStack () :
       curNode (NULL)
{
}

void CStack :: clear ()
{
       while (pop () != NULL);
}

void CStack :: push (void* data)
{
     StackNode_t* newNode = new StackNode_t ();
     
     newNode->prev = curNode;
     newNode->data = data;
     
     curNode = newNode;
}

void* CStack :: pop ()
{
      if (curNode == NULL) return NULL;
      
      StackNode_t* prev = curNode->prev;
      void*        data = curNode->data;
      
      delete curNode;
      curNode = prev;
      
      return data;      
}

struct Cell_t
{
       POINT_t    pos;
       CELL_STATE state;
       int        value;
       int        nodeState;
       bool       checked;
       int        foeDist;
       Cell_t*    nearestFoe;
       
       Cell_t (POINT_t apos, CELL_STATE astate);
};

extern Cell_t* absMinValCell;
extern Cell_t* field         [10][10];
extern Cell_t* notEmptyCells [100];
extern int     notEmptyCellsNum;

void initField           ();
void clearField          ();
void setDefState         ();
void updateFoeRelations  ();
void setFoeRelationsNull ();

void addNewCell (POINT_t apos, CELL_STATE astate);

Cell_t* absMinValCell = NULL;
Cell_t* field [10][10];
Cell_t* notEmptyCells [100];
int     notEmptyCellsNum = 0;

Cell_t :: Cell_t (POINT_t apos, CELL_STATE astate) :
     pos        (apos),
     state      (astate),
     value      (0),
     nodeState  (NS_UNDONE),
     checked    (false),
     foeDist    (1024),
     nearestFoe (NULL)
{}

void initField ()
{
     for (int i = 0; i < 10; i++)
         for (int j = 0; j < 10; j++)
         {
             field [i][j] = new Cell_t ((POINT_t) {i, j}, CS_EMPTY);
         }
     
     absMinValCell        = new Cell_t ((POINT_t){-1, -1}, CS_EMPTY);
     absMinValCell->value = -4096;
}

void clearField ()
{
     for (int i = 0; i < 10; i++)
         for (int j = 0; j < 10; j++)
         {
             delete field [i][j];
         }
         
     delete absMinValCell;
}

void setDefState ()
{
     for (int i = 0; i < 10; i++)
         for (int j = 0; j < 10; j++)
         {
             field [i][j]->nodeState = NS_UNDONE;
         }
}

void updateFoeRelations ()
{
     for (int i = 0; i < notEmptyCellsNum; i++)
     {
         Cell_t* c = notEmptyCells [i];
         if (c->state == CS_P2_ALIVE || c->state == CS_P1_DEAD)
         {
             setDefState ();
             if (c->state == CS_P1_DEAD && !DFS_DeadChain (c, CS_P2_ALIVE, CS_P1_DEAD))
                continue;
                
             POINT_t dest = {c->pos.x, c->pos.y};
             for (int j = dest.x-4; j < dest.x+4; j++)
             {
                 for (int k = dest.y-4; k < dest.y+4; k++)
                 {
                     POINT_t temp = {j, k};
                     if (temp.x  >= 0 && temp.x <= 9 &&
                         temp.y  >= 0 && temp.y <= 9)
                     {
                         Cell_t* c_ = field [temp.x][temp.y];
                         
                         if (c_->checked == true) continue;
                         
                         int dist_ = dist (&c->pos, &c_->pos);
                         
                         if (c ->state   == CS_P2_ALIVE && c_->foeDist > dist_)
                         {
                             c_->foeDist    = dist_;
                             c ->foeDist    = dist_;
                             c_->nearestFoe = c;
                         }    
                         
                         c_->checked = true;
                     }
                 }
             }
         }
     }
}

void setFoeRelationsNull ()
{
     for (int i = 0; i < 10; i++)
     {
         for (int j = 0; j < 10; j++)
         {
             field [i][j]->checked = false;
             field [i][j]->foeDist = 1024;
         }
     }
}

void addNewCell (POINT_t apos, CELL_STATE astate)
{
     field [apos.x][apos.y]->state    = astate;
     notEmptyCells [notEmptyCellsNum] = field [apos.x][apos.y];
     notEmptyCellsNum++;
}

Cell_t* p1StartCell  = NULL;
Cell_t* curMaxCell   = NULL;
int     curMaxValue  = 0;
bool    working      = true;

using namespace std;

inline int sq (int a) { return a*a; }

void getAvailableSteps (Cell_t* curCell, Cell_t** steps, int* size, int player)
{
     for (int i = 0; i < 8; i++)
     {
         POINT_t temp = add (curCell->pos, shifts [i]);
         if (temp.x >= 0 && temp.x <= 9 &&
             temp.y >= 0 && temp.y <= 9)
         {
             switch (curCell->state)
             {
             case CS_P1_ALIVE:
                  if (player == 2) break;
                  if (field [temp.x][temp.y]->state == CS_EMPTY ||
                      field [temp.x][temp.y]->state == CS_P2_ALIVE)
                  {
                     steps [*size] = field [temp.x][temp.y];
                     (*size)++;
                  }
                  break;
             case CS_P2_DEAD:
                  if (player == 2) break;
                  if (field [temp.x][temp.y]->state == CS_P2_ALIVE)
                  {
                     steps [*size] = field [temp.x][temp.y];
                     (*size)++;
                  }
                  break;
                  
             //get across with cases for foe step
             case CS_P2_ALIVE:
                  if (player == 1) break;
                  if (field [temp.x][temp.y]->state == CS_EMPTY ||
                      field [temp.x][temp.y]->state == CS_P1_ALIVE)
                  {
                     steps [*size] = field [temp.x][temp.y];
                     (*size)++;
                  }
                  break;
             case CS_P1_DEAD:
                  if (player == 1) break;
                  if (field [temp.x][temp.y]->state == CS_P1_ALIVE)
                  {
                     steps [*size] = field [temp.x][temp.y];
                     (*size)++;
                  }
                  break;
             };
         }
     }
}

bool DFS_DeadChain (Cell_t* cell, CELL_STATE s1, CELL_STATE s2)
{
     cell->nodeState = NS_DOING;
     
     bool result = false;
     for (int i = 0; i < 8; i++)
     {
         POINT_t temp = add (cell->pos, shifts [i]);
         if (temp.x >= 0 && temp.x <= 9 &&
             temp.y >= 0 && temp.y <= 9)
         {
             if (field [temp.x][temp.y]->nodeState == NS_UNDONE)
             {
                 if (field [temp.x][temp.y]->state == s1)
                 {
                     result = true;
                     break;
                 }
                 if (field [temp.x][temp.y]->state == s2 &&
                     DFS_DeadChain (field [temp.x][temp.y], s1, s2))
                 {
                     result = true;
                     break;
                 }
             }
         }
     }
     
     cell->nodeState = NS_DONE;
     
     return result;
}

void foeSteps (char* input)
{
     int numSteps = strlen (input) / 2;
     for (int i = 0; i < numSteps; i++)
     {
         POINT_t dest = {input [i*2]-'0', input [i*2+1]-'0'};
         
         switch (field [dest.x][dest.y]->state)
         {
         case CS_EMPTY:
              addNewCell (dest, CS_P2_ALIVE);
              break;
              
         case CS_P1_ALIVE:
              field [dest.x][dest.y]->state = CS_P1_DEAD;
              break;
         }
         
         //updating status
         setFoeRelationsNull ();
         updateFoeRelations  ();
     }
}

bool heuristic_func (Node_t* node)
{
     Cell_t* cell = (Cell_t*)(node->data);
     cell->value  = 0;
     int numP1AliveCells = 0,
         numP2DeadCells  = 0;
     for (int i = 0; i < 8; i++)          //this shit check my neighbours to attack/clone from!
     {
         POINT_t temp = add (cell->pos, shifts [i]);
         if (temp.x  >= 0 && temp.x <= 9 &&
             temp.y  >= 0 && temp.y <= 9)
         {
             Cell_t* analysedCell = field [temp.x][temp.y];
             
             switch (cell->state)
             {
             case CS_EMPTY:
                  switch (analysedCell->state)
                  {
                  case CS_P1_ALIVE:
                       numP1AliveCells++;
                       if (cell->foeDist < 4)
                       {
                          cell->value = max (5-(cell->foeDist-3), cell->value);
                          break;
                       }
                       if (numP1AliveCells == 1)
                       { 
                           if (field [9-p1StartCell->pos.x][abs (9-p1StartCell->pos.y-1)]->state == CS_P2_ALIVE)
                           {
                               if (cell->pos.x == p1StartCell->pos.x)
                                   cell->value += 10;
                           }
                           else
                           {
                               if (cell->pos.y == p1StartCell->pos.y)
                                   cell->value += 10;
                           }
                       }
                       else cell->value +=   - shifts [i].x - shifts [i].y
                                           - 2*shifts [i].x * shifts [i].y;
                       break;
                  case CS_P1_DEAD:
                       if (cell->foeDist < 3) cell->value +=  8 + (3-cell->foeDist);
                       else                   cell->value -= 15;
                       break;
                  case CS_P2_ALIVE:
                       cell->value += 1;
                       break;
                  };
                  
                  break;
             case CS_P2_ALIVE:
                  Cell_t* foeSteps [8] = {};
                  int     foeStepsNum  = 0;
                  getAvailableSteps (cell, foeSteps, &foeStepsNum, 2);
                  if (foeStepsNum == 0)
                  {
                     break;
                  }
                  
                  numP1AliveCells++;
                  
                  switch (analysedCell->state)
                  {
                  case CS_P1_ALIVE:
                       numP1AliveCells ++;
                       if (XOR (numP1AliveCells == 1, numP2DeadCells == 1)) cell->value += 10-cell->foeDist;
                       cell->value += 10;
                       break;
                  case CS_P2_DEAD:
                       numP2DeadCells ++;
                       if (XOR (numP1AliveCells == 1, numP2DeadCells == 1)) cell->value += 10-cell->foeDist;
                       cell->value += 6;
                       break;
                  case CS_P2_ALIVE:
                       cell->value += 3;
                       break;
                  };
                  
                  break;
             };
         }         
     }
     
     //checking deads chain
     if (cell->state == CS_P2_ALIVE && numP2DeadCells  != 0
                                    && numP1AliveCells == 0)
     {
         setDefState ();
         if (!DFS_DeadChain (cell, CS_P1_ALIVE, CS_P2_DEAD)) cell->value = -1024;
     }

     if (cell->value >= curMaxCell->value && cell != field [0][0])
     {
         if (cell->value == curMaxCell->value)
         {
             if (sq (cell      ->pos.x) + sq (cell      ->pos.y) <
                 sq (curMaxCell->pos.x) + sq (curMaxCell->pos.y))
                 curMaxCell = cell;
         }
         else curMaxCell = cell;
     }
         
     return true;
}

void mySteps (char* dest, int numAvailableSteps)
{
     if (numAvailableSteps != 3)
     {
        if (field [0][0]->state != CS_EMPTY)
        {
            addNewCell   (field [9][9]->pos, CS_P1_ALIVE);
            p1StartCell = field [9][9];
            
            char temp [8] = "";
            sprintf (temp, "99");
            strcat  (dest, temp);
        }
        else
        {
            addNewCell   (field [0][0]->pos, CS_P1_ALIVE);
            p1StartCell = field [0][0];
            
            char temp [8] = "";
            sprintf (temp, "00");
            strcat  (dest, temp);
        }
     }
     
     for (int t = 0; t < numAvailableSteps; t++)
     {
          curMaxCell = absMinValCell;
          //getting available steps
          Cell_t* tempCells [8] = {};
          int     tempCellsSize = 0;
          Node_t* root = new Node_t (notEmptyCells [0]);
          for (int i = 0; i < notEmptyCellsNum; i++)
          {
              tempCellsSize = 0;
              getAvailableSteps (notEmptyCells [i], tempCells, &tempCellsSize, 1);

              for (int j = 0; j < tempCellsSize; j++)
              {
                  root->add (new Node_t (tempCells [j]));
              }
          }
         
          //calculating the bests steps
          if (root->childrenNum == 0) break;
          
          CGraph* graph = new CGraph ();
          graph->top = root;
          graph->BFS (heuristic_func);
          
          switch (curMaxCell->state)
          {
          case CS_EMPTY:
               addNewCell (curMaxCell->pos, CS_P1_ALIVE);
               break;
               
          case CS_P2_ALIVE:
               curMaxCell->state = CS_P2_DEAD;
               break;
          }
          
          graph->clear ();
          delete graph;
          
          if (curMaxCell == absMinValCell) break;
          
          //writing steps for the opponent
          char temp [8] = "";
          sprintf (temp, "%d%d", curMaxCell->pos.x, curMaxCell->pos.y);
          strcat  (dest, temp);
     }
     if (strlen (dest) / 2 < numAvailableSteps) working = false;
}

int main ()
{
    int firstStep = 0;
    scanf ("%d", &firstStep);
    initField ();
    
    char output [256] = "";
    
    if (firstStep == 1)
    {
        mySteps (output, 2);
        cout << output << endl;
        cout.flush();
        firstStep = 0;
    }
    
    while (working)
    {
          char input  [256] = "";
          output [0] = '\0';
          
          scanf ("%s", input);
          foeSteps    (input);
          if (strlen  (input) < 6) working = false;
          
          mySteps    (output, (firstStep != 0) ? 2 : 3);
          if (firstStep != 0)  firstStep  = 0;
          
          cout << output << endl;
          cout.flush();
          if (strlen (output) < 6) working = false;
    }
    
    clearField ();
    
    return 0;
}
