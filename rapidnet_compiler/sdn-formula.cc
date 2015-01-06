/*
 * sdn-formula.cc
 *
 *  Created on: Nov 24, 2014
 *      Author: cchen
 */

#include "sdn-formula.h"

/* *************************** CONSTRAINT ************************************** */

Constraint::Constraint(Operator opt, Term* exprL, Term* exprR):
    op(opt),leftE(exprL),rightE(exprR){}

Constraint::~Constraint()
{
	delete leftE;
	delete rightE;
}

void
Constraint::PrintConstraint()
{
  leftE->PrintTerm();
  PrintOp();
  rightE->PrintTerm();
}

void 
Constraint::PrintOp()
{
  switch(op){
  case Constraint::EQ:
    cout << "=";
    break;
  case Constraint::NEQ:
    cout << "!=";
    break;
  case Constraint::GT:
    cout << ">";
    break;
  case Constraint::LT:
    cout << "<";
    break;
  }
}

/* *************************** CONSTRAINT ************************************** */





/* **************************** VARIABLE ************************************** */

int Variable::varCount = 0;

Variable::Variable(TypeCode t, bool b):varType(t),isbound(b) {
  Variable::varCount = Variable::varCount + 1;
	ostringstream countStream;
	countStream << Variable::varCount;  
	name =  "variable"+ countStream.str();
}

Variable::TypeCode Variable::GetVariableType() {
  return varType;
}

string Variable::GetVariableName() {
  return name;
}

void Variable::PrintTerm() {
  cout << name;
}

bool Variable::GetFreeOrBound() {
  return isbound;
}

/* **************************** VARIABLE ************************************** */








/* ************************* FUNCTION SCHEMA ********************************** */

void 
FunctionSchema::PrintSchema()
{
  cout << name;
}

/* ************************* FUNCTION SCHEMA ********************************** */









/* **************************** USER FUNCTION ********************************* */

void
UserFunction::PrintTerm()
{
  schema->PrintSchema();
  cout << "(";
  vector<Term*>::iterator it;
  for (it = args.begin(); it != args.end(); it++)
  {
    if (it != args.begin())
    {
      cout << ",";
    }
    (*it)->PrintTerm();
  }
  cout << ")";
}

/* **************************** USER FUNCTION ********************************* */





/* ********************************** IntVal ********************************** */

void
IntVal::PrintTerm()
{
  cout << value;
}

/* ********************************** IntVal ********************************** */




/* ****************************** DoubleVal ********************************** */

void
DoubleVal::PrintTerm()
{
  cout << value;
}

/* ****************************** DoubleVal ********************************** */





/* ****************************** StringVal ********************************* */

void
StringVal::PrintTerm()
{
  cout << value;
}

/* ****************************** StringVal ********************************* */







/* ****************************** BoolVal *********************************** */

void
BoolVal::PrintTerm()
{
  cout << value;
}

/* ****************************** BoolVal *********************************** */






/* *************************** Arithmetic *********************************** */

void
Arithmetic::PrintTerm()
{
  leftE->PrintTerm();
  PrintOp();
  rightE->PrintTerm();
}

void
Arithmetic::PrintOp()
{
  switch(op){
  case Arithmetic::PLUS:
    cout << "+";
    break;
  case Arithmetic::MINUS:
    cout << "-";
    break;
  case Arithmetic::TIMES:
    cout << "*";
    break;
  case Arithmetic::DIVIDE:
    cout << "/";
    break;
  }  
}

/* *************************** Arithmetic *********************************** */













