/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "../StanfordCPPLib/tokenscanner.h"

enum StatementType { REM, LET, PRINT, INPUT, END, GOTO, IF_THEN, RUN, LIST, CLEAR, QUIT, HELP, EMPTY};
const string StatementTypeName[13] = { "REM", "LET", "PRINT", "INPUT", "END", "GOTO", "IF", "RUN", "LIST", "CLEAR", "QUIT", "HELP", "THEN"};

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Constructor: Statement
 * ----------------------
 * To construct the base part of certain statement type.
 */
   Statement(StatementType st);
/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state) = 0;
   
/*
 * Method: getStatementType
 * Usage: st = stmt->getStatementType();
 * ----------------------------
 * This method helps to get the type of a Statement.
 */

   StatementType getStatementType();

private:
  StatementType st;
};
class _rem:public Statement
{
public:
  _rem();
  ~_rem();
  void execute(EvalState & state);
};

class _let:public Statement
{
public:
  _let(Expression *exp);
  ~_let();
  void execute(EvalState & state);
private:
  Expression *exp;
};
class _print:public Statement
{
public:
  _print(Expression *exp);
  ~_print();
  void execute(EvalState & state);
private:
  Expression *exp;
};
class _input:public Statement
{
public:
  _input(Expression *exp);
  ~_input();
  void execute(EvalState & state);
private:
  Expression *exp;
};
class _end:public Statement
{
public:
  _end();
  ~_end();
  void execute(EvalState & state);
};
class _goto:public Statement
{
public:
  _goto(int linenumber);
  ~_goto();
  void execute(EvalState & state);
  int getLineNumber();
private:
  int linenumber;
};
class _if_then:public Statement
{
public:
  _if_then(int linenumber,Expression *exp1,Expression *exp2,string op);
  ~_if_then();
  void execute(EvalState & state);
  bool conditionIsTrue(EvalState & state);
  int getLineNumber();
private:
  Expression *exp1, *exp2;
  string op;
  int linenumber;
};
class _list:public Statement
{
public:
  _list();
  ~_list();
  void execute(EvalState & state);
};

class _run:public Statement
{
public:
  _run();
  ~_run();
  void execute(EvalState & state);
};

class _clear:public Statement
{
public:
  _clear();
  ~_clear();
  void execute(EvalState & state);
};

class _help:public Statement
{
  public:
  _help();
  ~_help();
  void execute(EvalState & state);
};

class _empty:public Statement
{
public:
   _empty();
   ~_empty();
   void execute(EvalState & state);
};

/*
 * Function:stringToStatement
 * Usage:Statement *stmt = stringToStatement(str);
 * --------------------------
 * This function new a certain type of statement decided by a no line number stirng.
 * while try to form a Statement, and return NULL.
 * And never forget to delete stmt after used.
 */

Statement &stringToStatement(const string line);

#endif
