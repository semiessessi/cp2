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

Language Overview
=================

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