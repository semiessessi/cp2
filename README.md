CP2 - Compiler Project 2
========================

Following from the work on CP1, this is a new project to build a compiler that is suitable for bootstrapping.

Aims
----

* Minimal language, with as many features expressed in the standard library as possible
* LLVM as the foundation
* C binary compatible
* Faster than C
* Powerful, modern library

High Level Overview
===================

Components
----------

* first pass generic lexer (C++)
* first pass generic parser (C++)
* first pass grammar tool (C++)
* tests (C++)*
* first pass compiler (C++)*

This list will grow with time... items marked * are incomplete

The first pass toolchain only has to work properly, not be fast. The idea is to then build a next generation of toolchain using this, which is meant to be fast.

First Pass Lexer
----------------

This is surprisingly complete and fast for a naive implementation. It has two classes of rules, and some hard coded logic for strings (which may be unnecessary).

The rules are comment or lexeme rules, a lexeme is either a regex or a constant string to be matched, a comment rule defines line and block comment markers. The tokens produced store their filename, line and column number for error reporting purposes.

The lexer has one large optimisation, which is to use fixed string matching instead of regex where possible. This enables it to work on 100KBs of file in a semi-reasonable time - e.g. the abc rule test handles 76KB of input in 1.5s in the release build.

First Pass Parser
-----------------

The parser is a generic, naive, top-down parser.

The parser is optimised in several ways, but is still quite slow:

* each production is handled iteratively
* AST nodes are pool allocated
* token names etc. use character pointers rather than std::string

Language Design Overview
========================

One type to rule them all...
----------------------------

The type system is based on a single irreducible type, the byte.

All other types are defined by combining or extending bytes and other types, producing a rich type system.

New types are defined with the 'type' keyword. e.g.

type byte bool;

They can involve fixed size arrays, aggregate types and type decorators, e.g.

align< 4 > integer type byte[ 4 ] int32;
align< 4 > real type byte[ 4 ] float32;

struct type
{
    int32 quot;
    int32 rem;
} div_t;

In these examples div_t is an aggregate, int32 and float32 are 4-byte arrays with some additional properties.

The decorators integer and real are defined implicitly by their usage, the other decorators are built-in. Any unrecognised type decorators are used to define type categories.

Built-in type decorators:
* address - this type is for representing memory addresses
* align< n > - use n-byte alignment
* generic< type-category-1 [, type-category-2[, type-category-3 ... ]...] - more on this later...
* struct - use C struct layout rules

Constants
---------

Addressing
----------

Functions, operators
--------------------

Intrinsics
----------