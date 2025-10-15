// Copyright (C) 2024 Cade Weinberg
//
// This file is part of inf.
//
// inf is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// inf is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with inf.  If not, see <http://www.gnu.org/licenses/>.

%{
#include <boost/log/trivial.hpp>

#include "core/compile.hpp"
#include "core/lex.hpp"

static void yyerror(lexer *lexer, context *context, char const *msg);
%}

%require "3.8"
%language "c++"
%locations
%header
%define lr.type ielr
%define api.namespace {inf}
%define api.value.type {operand}
%define api.location.type {location}

%code requires {
#include "core/lex.hpp"
#include "env/context.hpp"
#include "imr/operand.hpp"
#include "imr/location.hpp"
}

%param {lexer *lexer}
%parse-param{context *context}

%token ';' '(' ')'
%token INTEGER
%left '+' '-'
%left '*' '/' '%'

%%

input:
      %empty
    | expression ';' { $$ = $1; }
    ;

expression:
      infix { $$ = $1; }
    | error { $$ = $1; yyerrok; }
    ;

infix:
       primary
     | infix '+' infix { $$ = compile_add($1, $3, context); }
     | infix '-' infix { $$ = compile_sub($1, $3, context); }
     | infix '*' infix { $$ = compile_mul($1, $3, context); }
     | infix '/' infix { $$ = compile_div($1, $3, context); }
     | infix '%' infix { $$ = compile_mod($1, $3, context); }
     | '(' infix ')' { $$ = $2; }
     ;

primary:
      INTEGER
    ;

%%

static void yyerror(lexer *lexer, context *context, char const *msg) {
    BOOST_LOG_TRIVIAL(error) << msg;
}
