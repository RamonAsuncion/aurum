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
+    ( a b -- a+b )
-    ( a b -- a-b )
*    ( a b -- a*b )
/    ( a b -- a/b )
%    ( a b -- a%b )
```

## Comparison operators

```text
=  ( a b -- a = b )
<  ( a b -- a < b )
>  ( a b -- a > b )
<= ( a b -- a <= b )
>= ( a b -- a >= b )
```

## Bitwise operators

```text
& ( a b -- a & b )
|     ( a b -- ab )
~    ( a --~a )
>> ( a b -- a >> b )
<< ( a b -- a << b )
```

### Types (Future)

```text
int    ( -- int ) 64 bit signed integer
bool   ( -- bool ) 64 bit floating point number
float  ( -- float ) 1 bit boolean
ptr    ( -- ptr ) 64 bit pointer
addr   ( -- addr ) 64 bit address (address of the procedure)
```

### Hello World (Future)

```text
func main as void
  "Hello, World!\n" write
end

func main as 
  "Hello, World!\n" write
end
```

### While Loop

```text
<starting-value> while <condition> then
  <op>
end
```

### If Statement

```text
<condition> if
  <statement>
else <condition> then
  <statement>
else <condition> then
  <statement>
end
```

### Function Definition (Future)

```text
func <name> as <type>
  <statement>
end
```

### Require (Future)

```text
require <file.ah>
```

### Constants (Future)

```text
final <name> end
```

### Define (Future)

```text
define <name> as <value> end
```

example:

define pi as 3.14
  pi write
end

### Comments

// This is a comment

/*
  This is also a comment.
*/

### Peek (Future)

```text
peek <name> as 
  <statement>
end
```
