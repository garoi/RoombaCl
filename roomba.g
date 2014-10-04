#header
<<
#include <string>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <time.h>

using namespace std;

// struct to store information about tokens
typedef struct {
  string kind;
  string text;
} Attrib;

// function to fill token information (predeclaration)
void zzcr_attr(Attrib *attr, int type, char *text);

// fields for AST nodes
#define AST_FIELDS string kind; string text;
#include "ast.h"

// macro to create a new AST node (and function predeclaration)
#define zzcr_ast(as,attr,ttype,textt) as=createASTnode(attr,ttype,textt)
AST* createASTnode(Attrib* attr,int ttype, char *textt);
>>

<<
#include <cstdlib>
#include <cmath>

//global structures
AST *root;


struct Pos {
  int x;
  int y;
} novaPos;

// function to fill token information
void zzcr_attr(Attrib *attr, int type, char *text) {
  if (type == ID) {
    attr->kind = "id";
    attr->text = text;
  }
  else {
    attr->kind = text;
    attr->text = "";
  }
}

// function to create a new AST node
AST* createASTnode(Attrib* attr, int type, char* text) {
  AST* as = new AST;
  as->kind = attr->kind;
  as->text = attr->text;
  as->right = NULL;
  as->down = NULL;
  return as;
}


/// create a new "list" AST node with one element
AST* createASTlist(AST *child) {
 AST *as=new AST;
 as->kind="list";
 as->right=NULL;
 as->down=child;
 return as;
}

AST *findTask(string id) {
  AST *n = root->down->right->right->down;
  while (n != NULL and (n->down->text != id)) n = n->right;
  if (n == NULL) {cout << "NOT FOUND: " << id << " " << endl;}
  return n->down->right;
}

/// get nth child of a tree. Count starts at 0.
/// if no such child, returns NULL
AST* child(AST *a, int n) {
AST *c=a->down;
for (int i=0; c!=NULL && i<n; i++) c=c->right;
return c;
}



/// print AST, recursively, with indentation
void ASTPrintIndent(AST *a,string s)
{
  if (a==NULL) return;

  cout<<a->kind;
  if (a->text!="") cout<<"("<<a->text<<")";
  cout<<endl;

  AST *i = a->down;
  while (i!=NULL && i->right!=NULL) {
    cout<<s+"  \\__";
    ASTPrintIndent(i,s+"  |"+string(i->kind.size()+i->text.size(),' '));
    i=i->right;
  }

  if (i!=NULL) {
      cout<<s+"  \\__";
      ASTPrintIndent(i,s+"   "+string(i->kind.size()+i->text.size(),' '));
      i=i->right;
  }
}

/// print AST
void ASTPrint(AST *a)
{
  while (a!=NULL) {
    cout<<" ";
    ASTPrintIndent(a,"");
    a=a->right;
  }
}

bool SenseProx() { return (rand() % 2) == 0;}

int SenseLight() { return rand() % 100;}

void move(AST* a) {
  int valorMoure = atoi(a->right->kind.c_str());
  if (a->kind == "right") novaPos.x += valorMoure;
  else if (a->kind == "left") novaPos.x -= valorMoure;
  else if (a->kind == "up") novaPos.y += valorMoure;
  else if (a->kind == "down") novaPos.y -= valorMoure;
}
//parametre 1 condicio, parametre 2 instruccio
void ife(AST* c, AST* i){
  cout << "cond " << c->kind << endl;
  cout << "inst " << i->kind << endl;
}
void ops(){}
void exec(){}

void findNewPosition(AST* a) {
  if (a != NULL) {
    cout << a->kind << endl;
    if (a->kind == "move") move(child(a, 0));
    else if (a->kind == "if") ife(child(a, 0), child(a, 1));
    else if (a->kind == "ops") ops();
    else if (a->kind == "exec") exec(); //no cridara a exec sino findNewPosition amb la tasca
    findNewPosition(a->right);
  }
}

void posicioInicial(AST* a) {
  novaPos.x = atoi(a->kind.c_str());
  novaPos.y = atoi(a->right->kind.c_str());
}

void evaluar(AST *a) { //passar x y per parametre
  if (a != NULL) {
    if (a->down->kind == "position") posicioInicial(child(a->down, 0));
    else return;
    if (a->down->right->kind == "list") findNewPosition(child(a->down->right, 0)); //no fa falta la comprovacio
    else {
      cout << "El robot rooma no s'ha mogut, la seva posicio es: " << novaPos.x << " " << novaPos.y << endl;
    }
  }
  else return;
}

int main() {
  srand (time(NULL));
  root = NULL;
  ANTLR(roomba(&root), stdin);
  ASTPrint(root);
  //findNewPosition() ha de estar aqui per collos
  evaluar(root);
}
>>

#lexclass START
#token STARTC "startcleaning"
#token ENDC "endcleaning"
#token NUM "[0-9]+"
#token POSITION "position"
#token RIGHT "right"
#token UP "up"
#token MOVE "move"
#token BOTTOM "down"
#token LEFT "left"
#token FLUSH "flush"
#token THEN "then"
#token CLAUO "\["
#token CLAUT "\]"
#token COMA "\,"
#token OPS "ops"
#token SENSORPROX "sensorprox"
#token SENSORLIGHT "sensorlight"
#token ON "ON"
#token OFF "OFF"
#token IF "if"
#token AND "AND"
#token OR "OR"
#token EQUAL "=="
#token GT "\>"
#token EXEC "exec"
#token TASK "TASK"
#token ENDT "ENDTASK"
#token ID "[a-zA-Z]+[0-9]*"
#token SPACE "[\t \ \n]" << zzskip();>>


roomba: position STARTC! linstr ENDC! tasks <<#0=createASTlist(_sibling);>>;

position: POSITION^ NUM NUM;
linstr: (move | flush | ife | exec | ops)* <<#0=createASTlist(_sibling);>>;
tasks: (tasksaux)* <<#0=createASTlist(_sibling);>>;
tasksaux: TASK^ ID insttask;

insttask: (inst)* ENDT! <<#0=createASTlist(_sibling);>>;

inst: move | flush | exec | ife | ops;

move: MOVE^ (RIGHT | LEFT | UP | BOTTOM) NUM;

flush: FLUSH^ NUM;

ife: IF^ condicioif THEN! (move | flush | exec | ops);
condicioif: sensor ((AND^ | OR^) sensor)*;
sensor: (SENSORLIGHT (GT^ |EQUAL^) NUM) | (SENSORPROX EQUAL^ (ON | OFF)) | (NUM (GT^ |EQUAL^) SENSORLIGHT) | ((ON | OFF) EQUAL^ SENSORPROX);  //mateix tipus dreta esquerra, light ==

ops: OPS^ CLAUO! ( | opsaux) CLAUT!;  //pot ser buit
opsaux: inst (COMA! inst)*;

exec: EXEC^ ID;