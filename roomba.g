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

void move(AST* a, int& x, int& y) {
  int valorMoure = atoi(a->right->kind.c_str());
  if (a->kind == "right") x += valorMoure;
  else if (a->kind == "left") x -= valorMoure;
  else if (a->kind == "up") y += valorMoure;
  else if (a->kind == "down") y -= valorMoure;
}


bool mesGran(AST* c) {
  // el simbol > nomes fa referencia al sensorlight, pero aquest pot ser un numero
  if (c->kind == "sensorlight") {
    if (SenseLight() > atoi(c->right->kind.c_str())) return true;
    else return false;
  }
  else {
    //es un numero
    if (atoi(c->kind.c_str()) > SenseLight()) return true;
    else return false;
  }
}

bool igual(AST* c) {
  /*pot ser:
    sensorprox == ON
    sensorprox == OFF
    ON == sensorprox
    OFF == sensorprox
    sensorlight == NUM
    NUM == sensorlight
  */
  if (c->kind == "sensorprox") {
    bool ences = SenseProx();
    if (c->right->kind == "ON" and ences) return true;
    else if (c->right->kind == "OFF" and not ences) return true;
    else return false;
  }
  else if (c->kind == "ON") {
    //A la força a continuacio sera sensorprox, per aixo no ho comprovo
    if (SenseProx()) return true;
    else return false;
  }
  else if (c->kind == "OFF") {
    //A la força a continuacio sera sensorprox, per aixo no ho comprovo
    if (not SenseProx()) return true;
    else return false;
  }
  else if (c->kind == "sensorlight") {
    if (SenseLight() == atoi(c->right->kind.c_str())) return true;
    else return false;
  }
  else {
    if (SenseLight() == atoi(c->kind.c_str())) return true;
    else return false;
  }
}

bool oOr(bool fillA, bool fillB) {
  if (fillA or fillB) return true;
  else return false;
}

bool iAnd(bool fillA, bool fillB) {
  if (fillA and fillB) return true;
  else return false;
}

bool ife(AST* c, bool& esCert) {
  //evaluar la condicio
  if (c != NULL) {
    if (c->kind == "AND") {
      if (iAnd(ife(child(c, 0), esCert), ife(child(c, 1), esCert))) esCert = true;
      else esCert = false;
    }
    else if (c->kind == "OR") {
      if (oOr(ife(child(c, 0), esCert), ife(child(c, 1), esCert))) esCert = true;
      else esCert = false;
    }
    else if (c->kind == "==") {
      if (igual(child(c, 0))) esCert = true;
      else esCert = false;
    }
    else if (c->kind == ">") {
      if (mesGran(child(c, 0))) esCert = true;
      else esCert = false;
    }
  }
  if (esCert) return true;
  else return false;
}

void evaluar(AST* a, int& x, int& y) {
  if (a != NULL) {
    // cout << a->kind << endl;
    if (a->kind == "move") move(child(a, 0), x, y);
    else if (a->kind == "if") {
      bool esCert = false;
      if (ife(child(a, 0), esCert)) evaluar(child(a, 1), x, y);
    }
    else if (a->kind == "ops") {
      evaluar(child(a, 0), x, y);
    }
    else if (a->kind == "exec") {
      evaluar(child(findTask(a->down->text),0), x, y);
    }
    evaluar(a->right, x, y);
  }
}

void posicioInicial(AST* a, int& x, int& y) {
  x = atoi(a->kind.c_str());
  y = atoi(a->right->kind.c_str());
}

void findNewPosition(AST *a) { //passar x y per parametre
  if (a != NULL) {
    int x, y;
    if (a->down->kind == "position") {
      posicioInicial(child(a->down, 0), x, y);
      cout << "#####################################################################" << endl;
      cout << "##  Hola, soc en Roomba, i aquests han sigut els meus moviments:   ##" << endl;
      cout << "##  La posicio inicial era " << x << " " << y << "                                  ##" << endl;
    }
    else return;
    evaluar(child(a->down->right, 0), x, y);
      cout << "##  La posicio final es: " << x << " " << y << "                                   ##" << endl;
      cout << "#####################################################################" << endl;
    // else { IDEA PER FER-HO MES BONIC
    //   cout << "El robot roobma no s'ha mogut, la seva posicio es: " << x << " " << y << endl;
    // }
  }
  else return;
}

int main() {
  srand (time(NULL));
  root = NULL;
  ANTLR(roomba(&root), stdin);
  ASTPrint(root);
  findNewPosition(root);
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
#token DOWN "down"
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

move: MOVE^ (RIGHT | LEFT | UP | DOWN) NUM;

flush: FLUSH^ NUM;

ife: IF^ condicioif THEN! inst;
condicioif: sensor ((AND^ | OR^) sensor)*;
sensor: (SENSORLIGHT (GT^ |EQUAL^) NUM) | (SENSORPROX EQUAL^ (ON | OFF)) | (NUM (GT^ |EQUAL^) SENSORLIGHT) | ((ON | OFF) EQUAL^ SENSORPROX);  //mateix tipus dreta esquerra, light ==

ops: OPS^ CLAUO! ( | opsaux) CLAUT!;
opsaux: inst (COMA! inst)*;

exec: EXEC^ ID;