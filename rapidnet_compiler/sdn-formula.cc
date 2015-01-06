/*
 * sdn-formula.cc
 *
 *  Created on: Nov 24, 2014
 *      Author: cchen
 */

#include "sdn-formula.h"


/* ***************************** TERM ************************************** */

int Term::GetValue() { //dummy return
  return 0;
}

void Term::PrintTerm(){}

/* ***************************** TERM ************************************** */






/* *************************** CONSTRAINT ************************************** */

Constraint::Constraint(Operator opt, Term* exprL, Term* exprR):
    op(opt),leftE(exprL),rightE(exprR){}

Constraint::~Constraint()
{
	delete leftE;
	delete rightE;
}

Constraint::Operator Constraint::GetOperator() {
  return op;
}

Term* Constraint::GetLeftE() {
  return leftE;
}

Term* Constraint::GetRightE() {
  return rightE;
}

void Constraint::PrintConstraint()
{
  leftE->PrintTerm();
  PrintOp();
  rightE->PrintTerm();
}

void Constraint::PrintOp() {
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

FunctionSchema::FunctionSchema(string n, vector<Variable::TypeCode>& d, Variable::TypeCode r):
  name(n),domain(d),range(r){}

FunctionSchema::~FunctionSchema() {}

string FunctionSchema::GetName() {
  return name;
}

vector<Variable::TypeCode>& FunctionSchema::GetDomainTypes() {
  return domain;
}

Variable::TypeCode FunctionSchema::GetRangeType() {
  return range;
}

void FunctionSchema::PrintSchema() {
  cout << name;
}

/* ************************* FUNCTION SCHEMA ********************************** */









/* **************************** USER FUNCTION ********************************* */


UserFunction::UserFunction(FunctionSchema* s, vector<Term*>& a):
    schema(s),args(a){}

UserFunction::~UserFunction(){}

FunctionSchema* UserFunction::GetSchema() {
  return schema;
}

vector<Term*>& UserFunction::GetArgs() {
  return args;
}

void UserFunction::PrintTerm() {
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

IntVal::IntVal(int v):value(v){}

IntVal::~IntVal(){}

int IntVal::GetIntValue() {
  return value;
}

void IntVal::PrintTerm() {
  cout << value;
}

/* ********************************** IntVal ********************************** */






/* ****************************** DoubleVal ********************************** */

DoubleVal::DoubleVal(double v):value(v){}

DoubleVal::~DoubleVal(){}

double DoubleVal::GetDoubleValue() {
  return value;
}

void DoubleVal::PrintTerm() {
  cout << value;
}

/* ****************************** DoubleVal ********************************** */





/* ****************************** StringVal ********************************* */


StringVal::StringVal(string v):value(v){}

StringVal::~StringVal(){}

string StringVal::GetStringValue() {
  return value;
}

void StringVal::PrintTerm() {
  cout << value;
}

/* ****************************** StringVal ********************************* */







/* ****************************** BoolVal *********************************** */

BoolVal::BoolVal(double v):value(v){}

BoolVal::~BoolVal(){}

bool BoolVal::GetBoolValue() {
  return value;
}

void BoolVal::PrintTerm() {
  cout << value;
}

/* ****************************** BoolVal *********************************** */






/* *************************** Arithmetic *********************************** */

Arithmetic::Arithmetic(ArithOp opt, Term* exprL, Term* exprR):
    op(opt), leftE(exprL), rightE(exprR){}

Arithmetic::ArithOp Arithmetic::GetArithOp() {
  return op;
}

Term* Arithmetic::GetLeftE() {
  return leftE;
}

Term* Arithmetic::GetRightE() {
  return rightE;
}

void Arithmetic::PrintTerm() {
  leftE->PrintTerm();
  PrintOp();
  rightE->PrintTerm();
}

void Arithmetic::PrintOp() {
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













