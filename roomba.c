/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   antlr -gt roomba.g
 *
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

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
#define GENAST

#include "ast.h"

#define zzSET_SIZE 4
#include "antlr.h"
#include "tokens.h"
#include "dlgdef.h"
#include "mode.h"

/* MR23 In order to remove calls to PURIFY use the antlr -nopurify option */

#ifndef PCCTS_PURIFY
#define PCCTS_PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif

#include "ast.c"
zzASTgvars

ANTLR_INFO

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

void
#ifdef __USE_PROTOS
roomba(AST**_root)
#else
roomba(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  position(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(STARTC);  zzCONSUME;
  linstr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(ENDC);  zzCONSUME;
  tasks(zzSTR); zzlink(_root, &_sibling, &_tail);
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x1);
  }
}

void
#ifdef __USE_PROTOS
position(AST**_root)
#else
position(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(POSITION); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x2);
  }
}

void
#ifdef __USE_PROTOS
linstr(AST**_root)
#else
linstr(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    for (;;) {
      if ( !((setwd1[LA(1)]&0x4))) break;
      if ( (LA(1)==MOVE) ) {
        move(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (LA(1)==FLUSH) ) {
          flush(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {
          if ( (LA(1)==IF) ) {
            ife(zzSTR); zzlink(_root, &_sibling, &_tail);
          }
          else {
            if ( (LA(1)==EXEC) ) {
              exec(zzSTR); zzlink(_root, &_sibling, &_tail);
            }
            else {
              if ( (LA(1)==OPS) ) {
                ops(zzSTR); zzlink(_root, &_sibling, &_tail);
              }
              else break; /* MR6 code for exiting loop "for sure" */
            }
          }
        }
      }
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x8);
  }
}

void
#ifdef __USE_PROTOS
tasks(AST**_root)
#else
tasks(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==TASK) ) {
      tasksaux(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x10);
  }
}

void
#ifdef __USE_PROTOS
tasksaux(AST**_root)
#else
tasksaux(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(TASK); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  insttask(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x20);
  }
}

void
#ifdef __USE_PROTOS
insttask(AST**_root)
#else
insttask(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd1[LA(1)]&0x40) ) {
      inst(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(ENDT); 
  (*_root)=createASTlist(_sibling);
 zzCONSUME;

  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x80);
  }
}

void
#ifdef __USE_PROTOS
inst(AST**_root)
#else
inst(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==MOVE) ) {
    move(zzSTR); zzlink(_root, &_sibling, &_tail);
  }
  else {
    if ( (LA(1)==FLUSH) ) {
      flush(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==EXEC) ) {
        exec(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (LA(1)==IF) ) {
          ife(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {
          if ( (LA(1)==OPS) ) {
            ops(zzSTR); zzlink(_root, &_sibling, &_tail);
          }
          else {zzFAIL(1,zzerr1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x1);
  }
}

void
#ifdef __USE_PROTOS
move(AST**_root)
#else
move(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(MOVE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==RIGHT) ) {
      zzmatch(RIGHT); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==LEFT) ) {
        zzmatch(LEFT); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {
        if ( (LA(1)==UP) ) {
          zzmatch(UP); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==BOTTOM) ) {
            zzmatch(BOTTOM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x2);
  }
}

void
#ifdef __USE_PROTOS
flush(AST**_root)
#else
flush(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(FLUSH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x4);
  }
}

void
#ifdef __USE_PROTOS
ife(AST**_root)
#else
ife(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(IF); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  condicioif(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(THEN);  zzCONSUME;
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==MOVE) ) {
      move(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==FLUSH) ) {
        flush(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (LA(1)==EXEC) ) {
          exec(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {
          if ( (LA(1)==OPS) ) {
            ops(zzSTR); zzlink(_root, &_sibling, &_tail);
          }
          else {zzFAIL(1,zzerr3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x8);
  }
}

void
#ifdef __USE_PROTOS
condicioif(AST**_root)
#else
condicioif(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  sensor(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd2[LA(1)]&0x10) ) {
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        if ( (LA(1)==AND) ) {
          zzmatch(AND); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==OR) ) {
            zzmatch(OR); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzEXIT(zztasp3);
        }
      }
      sensor(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x20);
  }
}

void
#ifdef __USE_PROTOS
sensor(AST**_root)
#else
sensor(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==SENSORLIGHT) ) {
    {
      zzBLOCK(zztasp2);
      zzMake0;
      {
      zzmatch(SENSORLIGHT); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        if ( (LA(1)==GT) ) {
          zzmatch(GT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==EQUAL) ) {
            zzmatch(EQUAL); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzEXIT(zztasp3);
        }
      }
      zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      zzEXIT(zztasp2);
      }
    }
  }
  else {
    if ( (LA(1)==SENSORPROX) ) {
      {
        zzBLOCK(zztasp2);
        zzMake0;
        {
        zzmatch(SENSORPROX); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        zzmatch(EQUAL); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        {
          zzBLOCK(zztasp3);
          zzMake0;
          {
          if ( (LA(1)==ON) ) {
            zzmatch(ON); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {
            if ( (LA(1)==OFF) ) {
              zzmatch(OFF); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
            }
            else {zzFAIL(1,zzerr6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
          zzEXIT(zztasp3);
          }
        }
        zzEXIT(zztasp2);
        }
      }
    }
    else {
      if ( (LA(1)==NUM) ) {
        {
          zzBLOCK(zztasp2);
          zzMake0;
          {
          zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
          {
            zzBLOCK(zztasp3);
            zzMake0;
            {
            if ( (LA(1)==GT) ) {
              zzmatch(GT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
            }
            else {
              if ( (LA(1)==EQUAL) ) {
                zzmatch(EQUAL); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
              }
              else {zzFAIL(1,zzerr7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
            zzEXIT(zztasp3);
            }
          }
          zzmatch(SENSORLIGHT); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
          zzEXIT(zztasp2);
          }
        }
      }
      else {
        if ( (setwd2[LA(1)]&0x40) ) {
          {
            zzBLOCK(zztasp2);
            zzMake0;
            {
            {
              zzBLOCK(zztasp3);
              zzMake0;
              {
              if ( (LA(1)==ON) ) {
                zzmatch(ON); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
              }
              else {
                if ( (LA(1)==OFF) ) {
                  zzmatch(OFF); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
                }
                else {zzFAIL(1,zzerr8,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
              }
              zzEXIT(zztasp3);
              }
            }
            zzmatch(EQUAL); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
            zzmatch(SENSORPROX); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
            zzEXIT(zztasp2);
            }
          }
        }
        else {zzFAIL(1,zzerr9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x80);
  }
}

void
#ifdef __USE_PROTOS
ops(AST**_root)
#else
ops(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(OPS); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(CLAUO);  zzCONSUME;
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==CLAUT) ) {
    }
    else {
      if ( (setwd3[LA(1)]&0x1) ) {
        opsaux(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {zzFAIL(1,zzerr10,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(CLAUT);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x2);
  }
}

void
#ifdef __USE_PROTOS
opsaux(AST**_root)
#else
opsaux(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  inst(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==COMA) ) {
      zzmatch(COMA);  zzCONSUME;
      inst(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x4);
  }
}

void
#ifdef __USE_PROTOS
exec(AST**_root)
#else
exec(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(EXEC); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x8);
  }
}
