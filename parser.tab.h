/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUMBER = 258,
     WORD = 259,
     STR = 260,
     OR_OP = 261,
     AND_OP = 262,
     EQUAL = 263,
     GREATER_OP = 264,
     LESS_OP = 265,
     GREATER_EQ_OP = 266,
     LESS_EQ_OP = 267,
     EOL = 268,
     CHOOSE = 269,
     SHEET = 270,
     SELECT = 271,
     ALL = 272,
     WHERE = 273,
     QUOTE = 274,
     OPEN = 275,
     CLOSE = 276,
     COMMA = 277,
     SEMICOLON = 278,
     COLUMNS = 279,
     COLON = 280,
     UPDATE = 281,
     SET = 282,
     TO = 283,
     ADD = 284,
     ROW = 285,
     O_BRACE = 286,
     C_BRACE = 287,
     INTO = 288,
     INSERT = 289
   };
#endif
/* Tokens.  */
#define NUMBER 258
#define WORD 259
#define STR 260
#define OR_OP 261
#define AND_OP 262
#define EQUAL 263
#define GREATER_OP 264
#define LESS_OP 265
#define GREATER_EQ_OP 266
#define LESS_EQ_OP 267
#define EOL 268
#define CHOOSE 269
#define SHEET 270
#define SELECT 271
#define ALL 272
#define WHERE 273
#define QUOTE 274
#define OPEN 275
#define CLOSE 276
#define COMMA 277
#define SEMICOLON 278
#define COLUMNS 279
#define COLON 280
#define UPDATE 281
#define SET 282
#define TO 283
#define ADD 284
#define ROW 285
#define O_BRACE 286
#define C_BRACE 287
#define INTO 288
#define INSERT 289




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 20 "parser.y"
{
 int intval;
 double floatval;
 char* strval;
 predicate* pval;
 query* qval;
 node* colval;
 update* uval;
 node_pair* pairsval;
 insert_query* insertval;
}
/* Line 1529 of yacc.c.  */
#line 129 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

