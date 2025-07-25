/*----------------------------------------------------------------------------
  ChucK Strongly-timed Audio Programming Language
    Compiler, Virtual Machine, and Synthesis Engine

  Copyright (c) 2003 Ge Wang and Perry R. Cook. All rights reserved.
    http://chuck.stanford.edu/
    http://chuck.cs.princeton.edu/

  This program is free software; you can redistribute it and/or modify
  it under the dual-license terms of EITHER the MIT License OR the GNU
  General Public License (the latter as published by the Free Software
  Foundation; either version 2 of the License or, at your option, any
  later version).

  This program is distributed in the hope that it will be useful and/or
  interesting, but WITHOUT ANY WARRANTY; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  MIT Licence and/or the GNU General Public License for details.

  You should have received a copy of the MIT License and the GNU General
  Public License (GPL) along with this program; a copy of the GPL can also
  be obtained by writing to the Free Software Foundation, Inc., 59 Temple
  Place, Suite 330, Boston, MA 02111-1307 U.S.A.
-----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// file: chuck_symbol.h
// desc: symbols in the syntax, adapted from Tiger compiler by Appel Appel
//
// author: Ge Wang (ge@ccrma.stanford.edu | gewang@cs.princeton.edu)
// adapted from: Andrew Appel (appel@cs.princeton.edu)
// date: Autumn 2002
//-----------------------------------------------------------------------------
#ifndef __CHUCK_SYMBOL_H__
#define __CHUCK_SYMBOL_H__

#include "chuck_def.h"


#if defined(_cplusplus) || defined(__cplusplus)
extern "C" {
#endif


typedef struct S_Symbol_ * S_Symbol;

/* Make a unique symbol from a given c_str.
 *  Different calls to make_symbol("foo") will yield the same S_Symbol
 *  value, even if the "foo" c_str are at different locations. */
S_Symbol insert_symbol( c_constr );

/* Extract the underlying c_str from a symbol */
c_str S_name(S_Symbol);

/* S_table is a mapping from S_Symbol->any, where "any" is represented
 *     here by void*  */
typedef struct TAB_table_ *S_table;

/* Make a new table */
S_table S_empty(void);
S_table S_empty2(unsigned int size);

/* Enter a binding "sym->value" into "t", shadowing but not deleting
 *    any previous binding of "sym". */
void S_enter(S_table t, S_Symbol sym, void * value);
void S_enter2(S_table t, c_constr str, void * value);

/* Look up the most recent binding of "sym" in "t", or return NULL
 *    if sym is unbound. */
void * S_look(S_table t, S_Symbol sym);
void * S_look2(S_table t, c_constr str);

/* Start a new "scope" in "t".  Scopes are nested. */
void S_beginScope(S_table t);

/* Remove any bindings entered since the current scope began,
   and end the current scope. */
void S_endScope(S_table t);
void S_pop(S_table t);


#if defined(_cplusplus) || defined(__cplusplus)
}
#endif




// moved here from archaic chuck_utils.h | 1.5.2.0 (ge)
#if defined(_cplusplus) || defined(__cplusplus)
extern "C" {
#endif


typedef struct U_boolList_ * U_boolList;
struct U_boolList_
{
    t_CKBOOL head;
    U_boolList tail;
};


void * checked_malloc( t_CKINT size );
c_str cc_str( char * );
U_boolList U_BoolList( t_CKBOOL head, U_boolList tail );


#if defined(_cplusplus) || defined(__cplusplus)
}
#endif



#endif
