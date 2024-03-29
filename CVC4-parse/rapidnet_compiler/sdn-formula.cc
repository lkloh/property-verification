/*
 * sdn-formula.cc
 *
 *  Created on: Nov 24, 2014
 *      Author: cchen
 */

#include "sdn-formula.h"
#include <stdexcept>

//NS_LOG_COMPONENT_DEFINE ("Formula"); /* MUST ADD BACK */


/* ***************************** TERM ************************************** */

int Term::GetValue() { //dummy return
  return 0;
}

void Term::PrintTerm(){}

/* ***************************** TERM ************************************** */






/* ***************************** FORMULA *********************************** */

Connective::Connective(ConnType ct, Formula* formL, Formula* formR):
    conntype(ct), leftF(formL), rightF(formR){}

Connective::~Connective(){}

Connective::ConnType Connective::GetConnType() {
  return conntype;
}

Formula* Connective::GetLeftF() {
  return leftF;
}

Formula* Connective::GetRightF() {
  return rightF;
}

/* ***************************** FORMULA *********************************** */




/* ***************************** Quantifier *********************************** */


Quantifier::Quantifier(QuanType q, vector<Variable*>& b, Formula* f):
  quantype(q),boundVarList(b),fml(f){}

Quantifier::~Quantifier(){}


vector<Variable*>& Quantifier::GetBoundVariables() {
  return boundVarList;
}

Quantifier::QuanType Quantifier::GetQuantifierType() {
  return quantype;
}

Formula* Quantifier::GetQuantifierFormula() {
  return fml;
}

/* ***************************** Quantifier *********************************** */






/* ************************* PredicateSchema ******************************** */

PredicateSchema::PredicateSchema(string n, vector<Variable::TypeCode>& t):
  name(n),types(t){}

PredicateSchema::~PredicateSchema(){}

string PredicateSchema::GetName() {
  return name;
}

vector<Variable::TypeCode>& PredicateSchema::GetTypes () {
  return types;
}

/* ************************* PredicateSchema ******************************** */








/* ************************* PredicateInstance ******************************** */

PredicateInstance::PredicateInstance(PredicateSchema* s, vector<Term*>& a):
  schema(s),args(a){}

PredicateInstance::~PredicateInstance(){}

PredicateSchema* PredicateInstance::GetSchema() {
  return schema;
}

vector<Term*>& PredicateInstance::GetArgs() {
  return args;
}

/* ************************* PredicateInstance ******************************** */









/* *************************** CONSTRAINT ************************************** */

Constraint::Constraint(Operator opt, Term* exprL, Term* exprR):
    op(opt),leftE(exprL),rightE(exprR){}

Constraint::Constraint(const Constraint& cst)
{
	op = cst.op;
	leftE = cst.leftE->Clone();
	rightE = cst.rightE->Clone();
}

Constraint::~Constraint()
{
	Variable* var = dynamic_cast<Variable*>(leftE);
	if (var == NULL)
	{
		delete leftE;
	}

	var = dynamic_cast<Variable*>(rightE);
	if (var == NULL)
	{
		delete rightE;
	}
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

void
Constraint::ReplaceVar(const VarMap& vmap)
{
	VarMap::const_iterator itm;
	Variable* var = dynamic_cast<Variable*>(leftE);
	if (var != NULL)
	{
		itm = vmap.find(var);
		if (itm != vmap.end())
		{
			leftE = itm->second;
		}
	}
	else
	{
		leftE->ReplaceVar(vmap);
	}

	var = dynamic_cast<Variable*>(rightE);
	if (var != NULL)
	{
		itm = vmap.find(var);
		if (itm != vmap.end())
		{
			rightE = itm->second;
		}
	}
	else
	{
		rightE->ReplaceVar(vmap);
	}
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

FunctionSchema::FunctionSchema(const FunctionSchema& fs)
{
	name = fs.name;
	domain = fs.domain;
	range = fs.range;
}

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

UserFunction::UserFunction(const UserFunction& uf)
{
	schema = new FunctionSchema(*(uf.schema));
	vector<Term*>::const_iterator itu;
	for (itu = uf.args.begin(); itu != uf.args.end(); itu++)
	{
		args.push_back((*itu)->Clone());
	}
}

UserFunction::~UserFunction()
{
	vector<Term*>::iterator it;
	for (it = args.begin(); it != args.end(); it++)
	{
		Variable* var = dynamic_cast<Variable*>((*it));
		if (var == NULL)
		{
			delete (*it);
		}
	}
}

FunctionSchema* UserFunction::GetSchema() {
  return schema;
}

void
UserFunction::ReplaceVar(const VarMap& vmap)
{
	vector<Term*>::iterator it;
	for (it = args.begin(); it != args.end(); it++)
	{
		Variable* var = dynamic_cast<Variable*>(*it);
		if (var != NULL)
		{
			VarMap::const_iterator itm = vmap.find(var);
			if (itm != vmap.end())
			{
				(*it) = itm->second;
			}
		}
		else
		{
			(*it)->ReplaceVar(vmap);
		}
	}
}

vector<Term*>& UserFunction::GetArgs() {
  return args;
}

UserFunction*
UserFunction::Clone()
{
	return (new UserFunction(*this));
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

IntVal::IntVal(const IntVal& iv)
{
	value = iv.value;
}

int IntVal::GetIntValue() {
  return value;
}

IntVal*
IntVal::Clone()
{
	return (new IntVal(*this));
}

void IntVal::PrintTerm() {
  cout << value;
}

/* ********************************** IntVal ********************************** */






/* ****************************** DoubleVal ********************************** */

DoubleVal::DoubleVal(double v):value(v){}

DoubleVal::DoubleVal(const DoubleVal& dv)
{
	value = dv.value;
}

double DoubleVal::GetDoubleValue() {
  return value;
}

DoubleVal*
DoubleVal::Clone()
{
	return (new DoubleVal(*this));
}

void DoubleVal::PrintTerm() {
  cout << value;
}

/* ****************************** DoubleVal ********************************** */





/* ****************************** StringVal ********************************* */


StringVal::StringVal(string v):value(v){}

StringVal::StringVal(const StringVal& str)
{
	value = str.value;
}

string StringVal::GetStringValue() {
  return value;
}

StringVal*
StringVal::Clone()
{
	return (new StringVal(*this));
}

void StringVal::PrintTerm() {
  cout << "\"" << value << "\"";
}

/* ****************************** StringVal ********************************* */







/* ****************************** BoolVal *********************************** */

BoolVal::BoolVal(double v):value(v){}

BoolVal::BoolVal(const BoolVal& bv)
{
	value = bv.value;
}

bool BoolVal::GetBoolValue() {
  return value;
}

BoolVal*
BoolVal::Clone()
{
	return (new BoolVal(*this));
}

void BoolVal::PrintTerm() {
  cout << value;
}

/* ****************************** BoolVal *********************************** */






/* *************************** Arithmetic *********************************** */

Arithmetic::Arithmetic(ArithOp opt, Term* exprL, Term* exprR):
    op(opt), leftE(exprL), rightE(exprR){}

Arithmetic::Arithmetic(const Arithmetic& ari)
{
	op = ari.op;
	leftE = ari.leftE->Clone();
	rightE = ari.rightE->Clone();
}

Arithmetic::~Arithmetic()
{
	Variable* var = dynamic_cast<Variable*>(leftE);
	if (var == NULL)
	{
		delete leftE;
	}

	var = dynamic_cast<Variable*>(rightE);
	if (var == NULL)
	{
		delete rightE;
	}
}

Arithmetic::ArithOp Arithmetic::GetArithOp() {
  return op;
}

Term* Arithmetic::GetLeftE() {
  return leftE;
}

Term* Arithmetic::GetRightE() {
  return rightE;
}

Arithmetic*
Arithmetic::Clone()
{
	return (new Arithmetic(*this));
}

void
Arithmetic::ReplaceVar(const VarMap& vmap)
{
	VarMap::const_iterator itm;
	Variable* var = dynamic_cast<Variable*>(leftE);
	if (var != NULL)
	{
		itm = vmap.find(var);
		if (itm != vmap.end())
		{
			leftE = itm->second;
		}
	}
	else
	{
		leftE->ReplaceVar(vmap);
	}

	var = dynamic_cast<Variable*>(rightE);
	if (var != NULL)
	{
		itm = vmap.find(var);
		if (itm != vmap.end())
		{
			rightE = itm->second;
		}
	}
	else
	{
		rightE->ReplaceVar(vmap);
	}
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













