/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_SRC_PARSER_YACCER_HPP_INCLUDED
# define YY_YY_SRC_PARSER_YACCER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 26 "./src/parser/yaccer.y" /* yacc.c:1909  */

    #include <deque>
    #include "ast.hpp"
    #include "statement.hpp"
    using std::deque;

    struct ClassMember{
        bool isField;
        union{
            Method* method;
            Field* field;
        };
    };

#line 59 "./src/parser/yaccer.hpp" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    TRUE = 259,
    FALSE = 260,
    BOOLEAN = 261,
    BREAK = 262,
    CLASS = 263,
    CONTINUE = 264,
    VOID = 265,
    EXTENDS = 266,
    RETURN = 267,
    IF = 268,
    ELSE = 269,
    WHILE = 270,
    THIS = 271,
    TOK_NULL = 272,
    NEW = 273,
    ERROR = 274,
    ARR = 275,
    THIS_DOT = 276,
    ID = 277,
    LIT_STR = 278,
    LIT_INT = 279,
    EQ = 280,
    DIFF = 281,
    LESS_EQ = 282,
    GREAT_EQ = 283,
    AND = 284,
    OR = 285,
    PREC_UNARY_OP = 286,
    FILLED_BRACK = 287,
    PREC_ELSELESS_IF = 288
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 41 "./src/parser/yaccer.y" /* yacc.c:1909  */

  struct Node* nodePointer;
  char* _str;
  int _int;
  class ClassDeclaration* _classDecl;
  class Block* _block;
  class Program* _program;
  class VarDec* _varDec;
  class Method* _methodDec;
  struct Type* _type;
  deque<ClassDeclaration*>* _classDecls; 
  deque<ClassMember>* _classMembers; 
  deque<Parameter*>* _params;
  Parameter* _param;
  Expression* _expr;
  ObjExpression* _objExpr;
  deque<Expression*>* _exprList;
  Statement* _statement;

#line 125 "./src/parser/yaccer.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_PARSER_YACCER_HPP_INCLUDED  */