# Language Specification Document

Reference: [Specification Language](https://en.wikipedia.org/wiki/Specification_language)

This is a mathematical, Lua and Forth inspired language. It is a stack based language with dynamic typing. It is designed to be a simple language that is easy to learn and use.

I say mathematical because the syntax follows some keywords that are used typically when expressing an algorithm in LaTeX, but Lua does share the same syntax in some cases.

## Stack manipulation

```text
dup   ( a -- a a )
2dup  ( a b -- a b a b )
drop  ( a -- )
2drop ( a b -- )
swap  ( a b -- b a )
2swap ( a b c d -- c d a b )
over  ( a b -- a b a )
2over ( a b c d -- a b c d a b )
rot   ( a b c -- b c a )
peek  ( a -- a )
```

## Arithmetic operators (possible divmod for % and /)

```text
+  ( a b -- a+b )
-  ( a b -- a-b )
*  ( a b -- a*b )
```

## Bitwise operators

```text
and ( a b -- a and b )
or  ( a b -- a or b )
not ( a -- not a )
xor ( a b -- a xor b )
```

## Relational operators

```text
=  ( a b -- a == b )
!=  ( a b -- a != b )
<   ( a b -- a < b )
>   ( a b -- a > b )
<=  ( a b -- a <= b )
>=  ( a b -- a >= b )
```

## Standard Library

### Math Library (math.ah)

```text
pow ( a b -- a^b )
avg ( a b -- (a+b)/2 )
min ( a b -- min(a,b) )
max ( a b -- max(a,b) )
```

## Control Flow

### While Loop

```text
<starting-value> 
while <condition> do
  <op>
end
```

(Unsure how for loops will work)

### If Statement

```text
if <condition> then
  <statement>
elseif <condition> then
  <statement>
else 
  <statement>
end
```

## Types (Dynamic Typing)

```text
int    ( -- int ) 64 bit signed integer
bool   ( -- bool ) 8 bit boolean
string ( -- string ) 8 bit character string
```

## Include directive

```text
include <file.ah>
include "file.ah"
```

## Comments

```text
//  This is a comment
/*
  This is also a comment.
*/
```

## Strings and Characters

```text
"Hello, world!" // string
'a' // character
```

## Constants

```text
final <name> <value> end
```

## Variables (Immutable)

Work in progress.

## Macros

```text
define <name> <value> end
```

## Procedures

```text
procedure <name> ( <args> ) is
  <statements>
end

<name> ( <args> )
```