/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/simpio.h"
#include "parser.h"
using namespace std;

#define DEBUG

/* Implementation of the Statement class */

Statement::Statement() {}

Statement::Statement(StatementType st):st(st){}

Statement::~Statement() {}

StatementType Statement::getStatementType()
{
   return st;
}

/*Implementation of _rem class*/

_rem::_rem():Statement(REM){}

_rem::~_rem(){}

void _rem::execute(EvalState & state){}

/*Implementation of _let class*/

_let::_let(Expression *exp):Statement(LET),exp(exp)
{
   if (exp->getType()!=COMPOUND)
      error("SYNTAX ERROR\n");
   CompoundExp *cexp = reinterpret_cast<CompoundExp *>(exp);
   if (cexp->getOp() != "=" || cexp->getLHS()->getType()!= IDENTIFIER)
      error("SYNTAX ERROR\n");
}

_let::~_let()
{
   delete exp;
}

void _let::execute(EvalState & state)
{
   exp->eval(state);
}

/*Implementation of _print class*/
_print::_print(Expression *exp):Statement(PRINT),exp(exp){}

_print::~_print()
{
   delete exp;
}

void _print::execute(EvalState & state)
{
   cout << exp->eval(state) << endl;
}

/*Implementation of _input class*/
_input::_input(Expression *exp):Statement(INPUT),exp(exp)
{
   if (exp->getType() != IDENTIFIER)
      error("SYNTAX ERROR\n");
}
_input::~_input()
{
   delete exp;
}
void _input::execute(EvalState & state)
{
   string str;
   do
   {
      cout << " ? ";
      str=getLine();
      TokenScanner scanner;
      scanner.scanNumbers();
      scanner.ignoreWhitespace();
      scanner.setInput(str);
      if (scanner.hasMoreTokens())
      {
         int flag = 0;
         string token = scanner.nextToken();
         while (token == "-")
         {
            flag++;
            token = scanner.nextToken();
         }
         if (scanner.getTokenType(token) == NUMBER && token.find(".") == std::string::npos && !scanner.hasMoreTokens())
         {
            state.setValue(exp->toString(),(flag & 1 ? -1 : 1) * stoi(token));
            break;
         }
      }
      cout << "INVALID NUMBER\n";
   }while(true);
}

/*Implementation of _end class*/

_end::_end():Statement(END){}

_end::~_end(){}

void _end::execute(EvalState & state)
{
   if (true)
      error("");
}

/*Implementation of _goto class*/

_goto::_goto(int linenumber):Statement(GOTO),linenumber(linenumber){}

_goto::~_goto(){}

void _goto::execute(EvalState & state){}

int _goto::getLineNumber()
{
   return linenumber;
}
/*Implementation of _if_then class*/

_if_then::_if_then(int linenumber,Expression *exp1,Expression *exp2,string op):Statement(IF_THEN),linenumber(linenumber),exp1(exp1),exp2(exp2),op(op){}

_if_then::~_if_then()
{
   delete exp1;
   delete exp2;
}

void _if_then::execute(EvalState & state){}

int _if_then::getLineNumber()
{
   return linenumber;
}

bool _if_then::conditionIsTrue(EvalState & state)
{
   int left,right;
   switch (exp1->getType())
   {
      case CONSTANT:
      left = reinterpret_cast<ConstantExp *>(exp1)->eval(state);
      switch (exp2->getType())
      {
         case CONSTANT:
            right = reinterpret_cast<ConstantExp *>(exp2)->eval(state);
            break;
         case IDENTIFIER:
            right = reinterpret_cast<IdentifierExp *>(exp2)->eval(state);
            break;
         case COMPOUND:
            right = reinterpret_cast<CompoundExp *>(exp2)->eval(state);
            break;
      }
      break;
      case IDENTIFIER:
      left = reinterpret_cast<IdentifierExp *>(exp1)->eval(state);
      switch (exp2->getType())
      {
         case CONSTANT:
            right = reinterpret_cast<ConstantExp *>(exp2)->eval(state);
            break;
         case IDENTIFIER:
            right = reinterpret_cast<IdentifierExp *>(exp2)->eval(state);
            break;
         case COMPOUND:
            right = reinterpret_cast<CompoundExp *>(exp2)->eval(state);
            break;
      }
      break;
      case COMPOUND:
      left = reinterpret_cast<CompoundExp *>(exp1)->eval(state);
      switch (exp2->getType())
      {
         case CONSTANT:
            right = reinterpret_cast<ConstantExp *>(exp2)->eval(state);
            break;
         case IDENTIFIER:
            right = reinterpret_cast<IdentifierExp *>(exp2)->eval(state);
            break;
         case COMPOUND:
            right = reinterpret_cast<CompoundExp *>(exp2)->eval(state);
            break;
      }
      break;
   }
   if ((left < right && op == "<") || (left == right && op == "=") || (left > right && op == ">"))
      return true;
   return false;
}
/*Implementation of _run class*/

_run::_run():Statement(RUN){}

_run::~_run(){}

void _run::execute(EvalState & state){}

/*Implementation of _list class*/

_list::_list():Statement(LIST){}

_list::~_list(){}

void _list::execute(EvalState &state){}

/*Implementation of _clear class*/

_clear::_clear():Statement(CLEAR){}

_clear::~_clear(){}

void _clear::execute(EvalState &state){}

/*Implementation of _help class*/

_help::_help():Statement(HELP){}

_help::~_help(){}

void _help::execute(EvalState & state){
   cout << "Wonder how to use it? Refer to Minimal BASIC interpreter.pdf";
}
/*Implementation of _empty class*/

_empty::_empty():Statement(EMPTY){}

_empty::~_empty(){}

void _empty::execute(EvalState & state){}


/* Implementation of functions */
Statement &stringToStatement(const string line)
{
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   scanner.setInput(line);
   // Return empty line.
   if (!scanner.hasMoreTokens())
   {
      _empty *emp = new _empty;
      return *emp;
   }
   std::string token = scanner.nextToken();
   //REM
   if (token == StatementTypeName[REM])
   {
      _rem *rem = new _rem;
      return *rem;
   }
   //LET
   if (token == StatementTypeName[LET])
   {
      try{
         Expression *exp = parseExp(scanner);
         _let *let = new _let(exp);
         return *let;
      }
      catch(ErrorException &ex)
      {
      }
   }
   //PRINT
   if (token == StatementTypeName[PRINT])
   {
      try
      {
         Expression *exp = parseExp(scanner);
         _print *print = new _print(exp);
         return *print;
      }
      catch(ErrorException &ex){}
   }
   //INPUT
   if (token == StatementTypeName[INPUT])
   {
      try
      {
         Expression *exp = parseExp(scanner);
         _input *inp = new _input(exp);
         return *inp;
      }
      catch(ErrorException &ex){}
   }
   //END
   if (token == StatementTypeName[END] && !scanner.hasMoreTokens())
   {
      _end *ed = new _end(); 
      return *ed;
   }
   //GOTO
   if (token == StatementTypeName[GOTO] && scanner.hasMoreTokens())
   {
      token = scanner.nextToken();
      if (scanner.getTokenType(token) == NUMBER && !scanner.hasMoreTokens())
      {
         int linenumber = stoi(token);
         _goto *gt = new _goto(linenumber);
         return *gt;
      }
   }
   //IF_THEN
   if (token == StatementTypeName[IF_THEN] && scanner.hasMoreTokens())
   {
      Expression *exp1,*exp2;
      string op = "";
      int plc = 0;
      if (line.find("=") != string::npos)
      {
         op = "=";
         plc = line.find("=");
      }
      if (line.find("<") != string::npos)
      {
         op = "<";
         plc = line.find("<");
      }
      if (line.find(">") != string::npos)
      {
         op = ">";
         plc = line.find(">");
      }
      if (op == "" || line.find("THEN") == string::npos)
         error("SYNTAX ERROR\n");
      try
      {
         scanner.setInput(line.substr(line.find(token)+token.length(),plc - line.find(token) - token.length()));
         exp1 = parseExp(scanner);
         scanner.setInput(line.substr(plc + 1,line.find("THEN") - plc - 1));
         exp2 = parseExp(scanner);
         scanner.setInput(line.substr(line.find("THEN") + 4));
         token = scanner.nextToken();
         if (scanner.getTokenType(token) == NUMBER && !scanner.hasMoreTokens())
         {
            int linenumber = stoi(token);
            _if_then *ift = new _if_then(linenumber,exp1,exp2,op);
            return *ift;
         }
      }
      catch(ErrorException &ex){}
   }
   //RUN
   if (token == StatementTypeName[RUN] && !scanner.hasMoreTokens())
   {
      _run *run = new _run;
      return *run;
   }
   //LIST
   if (token == StatementTypeName[LIST] && !scanner.hasMoreTokens())
   {
      _list *lis = new _list;
      return *lis;
   }
   //CLEAR
   if (token == StatementTypeName[CLEAR] && !scanner.hasMoreTokens())
   {
      _clear *clr = new _clear;
      return *clr;
   }
   //QUIT
   if (token == StatementTypeName[QUIT] && !scanner.hasMoreTokens())
      exit(0);
   //HELP
   if (token == StatementTypeName[HELP] && !scanner.hasMoreTokens())
   {
      _help *hlp = new _help;
      return *hlp;
   }

   error("SYNTAX ERROR\n");
   exit(0);

}
