/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "statement.h"
#include "evalstate.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

#define DEBUG        

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
         break;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
   try
   {
      TokenScanner scanner;
      scanner.ignoreWhitespace();
      scanner.scanNumbers();
      scanner.setInput(line);
      if (scanner.hasMoreTokens())
      {
         std::string token =scanner.nextToken();
         if (scanner.getTokenType(token) == NUMBER)
         {
            int linenumber = std::stoi(token);
            if(!scanner.hasMoreTokens())
               program.removeSourceLine(linenumber);
            else
            {
               std::string str = line.substr(line.find(token) + token.length());
               Statement *stmt = &stringToStatement(str);
               StatementType st = stmt->getStatementType();
               switch(st)
               {
                  case RUN:
                  case LIST:
                  case CLEAR:
                  case QUIT:
                  case HELP:
                     error("SYNTAX ERROR\n");
                  default:
                     program.addSourceLine(linenumber,line);
                     program.setParsedStatement(linenumber,stmt);
               }
            }
         }
         else if (scanner.getTokenType(token) == WORD)
         {
            //Try to form a Statement, and if there's a syntax error return NULL.
            scanner.setInput(line);
            Statement *stmt = &stringToStatement(line);
            StatementType st = stmt->getStatementType();
            //Execute process.
            switch (st)
            {
               case RUN:
                  for (int i = program.getFirstLineNumber(); i != -1; i = program.getNextLineNumber(i))
                  {
                     Statement *stmt = program.getParsedStatement(i);
                     StatementType st = stmt->getStatementType();
                     while(st == GOTO || st == IF_THEN)
                     {
                        if (st == GOTO)
                        {
                           i = reinterpret_cast<_goto *> (stmt)->getLineNumber();
                           
                        }
                        if (st == IF_THEN)
                           i = (reinterpret_cast<_if_then *>(stmt)->conditionIsTrue(state) ) ? reinterpret_cast<_if_then *>(stmt)->getLineNumber() : program.getNextLineNumber(i);
                        if (i == -1) break;
                        if (program.getSourceLine(i) == "")
                           error("LINE NUMBER ERROR\n");
                        stmt = program.getParsedStatement(i);
                        st = stmt->getStatementType();
                     }
                     if (i == -1) break;
                     program.getParsedStatement(i)->execute(state);
                  }
                  break;
               case LIST:
                  for (int i = program.getFirstLineNumber(); i != -1; i = program.getNextLineNumber(i))
                     cout << program.getSourceLine(i) << endl;
                  break;
               case CLEAR:
                  program.clear();
                  state.clear();
                  break;
               case REM:
               case END:
               case GOTO:
               case IF_THEN:
                  error("SYNTAX ERROR\n");
               default:
               stmt->execute(state);
            }
            delete stmt;
         }
      }
   }
   catch (ErrorException &ex)
   {
      cout << ex.getMessage();
   }
   // Expression *exp = parseExp(scanner);
   // int value = exp->eval(state);
   // cout << value << endl;
   // delete exp;
}
