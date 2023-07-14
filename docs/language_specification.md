# Language Specification Document

Reference: [Specification Language](https://en.wikipedia.org/wiki/Specification_language)

## Stack manipulation

```text
dup   ( a -- a a )
drop  ( a -- )
swap  ( a b -- b a )
over  ( a b -- a b a )
rot   ( a b c -- b c a )
```

## Arithmetic operators

```text
+  ( a b -- a+b )
-  ( a b -- a-b )
*  ( a b -- a*b )
/  ( a b -- a/b )
%  ( a b -- a%b )
```

## Comparison operators

```text
=   ( a b -- a = b )
<   ( a b -- a < b )
>   ( a b -- a > b )
<=  ( a b -- a <= b )
>=  ( a b -- a >= b )
```

## Bitwise operators

```text
&   ( a b -- a & b )
|   ( a b -- ab )
~   ( a -- ~a )
^   ( a b -- a ^ b )
>>  ( a b -- a >> b )
<<  ( a b -- a << b )
```

### Types (Future)

```text
int    ( -- int ) 64 bit signed integer
bool   ( -- bool ) 8 bit boolean
```

### While Loop

```text
<starting-value> while <condition> do
  <op>
end
```

### If Statement

```text
if <condition> then
  <statement>
elseif <condition> then
  <statement>
elseif <condition> then
  <statement>
end
```

### Include directive

```text
include <file.ah>
```

### Constants (Future)

```text
final <name> <value> end
```

### Macros

```text
define <name> <value> end
```

### Comments

// This is a comment

/*
  This is also a comment.
*/
