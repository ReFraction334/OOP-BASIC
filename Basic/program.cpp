/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;
#define DEBUG 
Program::Program() {
   // Replace this stub with your own code
}

Program::~Program() {
   // Replace this stub with your own code
}

void Program::clear() {
   for (int i = getFirstLineNumber(); i != -1; i = getNextLineNumber(i))
      removeSourceLine(i);
}

void Program::addSourceLine(int lineNumber, string line) {
   auto iter = lineList.find(lineNumber);
   if (iter != lineList.end())
      lineList.erase(lineNumber);
   lineList[lineNumber] = Line(line,NULL);
}

void Program::removeSourceLine(int lineNumber) {
   if (lineList.find(lineNumber) != lineList.end())
      lineList.erase(lineNumber);
}

string Program::getSourceLine(int lineNumber) {
   auto iter = lineList.find(lineNumber);
   return iter == lineList.end() ? "" : iter->second.sourceLine;
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
   if (lineList.find(lineNumber) == lineList.end())
      error("LINE NUMBER ERROR\n");
   lineList[lineNumber].prasedStatement = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
   if (lineList.find(lineNumber) == lineList.end())
      return NULL;
   return lineList[lineNumber].prasedStatement;
}

int Program::getFirstLineNumber() {
   return lineList.empty() ? -1 : lineList.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
   auto iter=lineList.upper_bound(lineNumber);
   return iter == lineList.end() ? -1 : iter->first;
}