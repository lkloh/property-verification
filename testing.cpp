#include <iostream>
#include <cvc4/cvc4.h> 
#include "rapidnet_compiler/sdn-formula.h"
#include "rapidnet_compiler/sdn-formula-to-cvc4.h"

using namespace std;
using namespace CVC4;

int main() {
   ExprManager em;
   Expr helloworld = em.mkVar("Hello World!", em.booleanType());
   std::to_string(10);
   SmtEngine smt(&em);
   std::cout << helloworld << " is " << smt.query(helloworld) << std::endl;

   IntVal three = IntVal(3);
   Variable x = Variable(Variable::INT, true);
   if (dynamic_cast<Variable*>(&x)) {
   		std::cout << "you suck" << std::endl;
   }

   return 0;
}