# Aurum Programming Language

https://medium.com/young-coder/how-i-wrote-a-lexer-39f4f79d2980

## Aurum is "gold" in Latin

### FileType: au, aur

\
*Aurum*  is an imperative, general-purpose, statically typed, compiled language that's
focused on simplicity and minimalism.

## Plan

- [ x ] Part 1: Lexer
- [ x ] Part 2: Parser
- [ ] Part 3: Evaluator

## Definitions

The `lexer` is responsible for taking the source code of a program as input and breaking it down into a sequence of tokens, which are meaningful units of the programming language such as keywords, identifiers, operators, and literals.

The `parser` then takes this sequence of tokens produced by the **lexer** and applies a grammar to it, which defines the structure and syntax of the programming language. The parser verifies that the input program is syntactically correct according to the grammar and produces a parse tree or an **abstract syntax tree** (AST) as output.

The `compiler` directory contains the implementation of the code generation and optimization phases of the compiler. A compiler is a program that takes source code written in a high-level programming language as input, and translates it into executable machine code that can be run directly by a computer.

The `runtime` directory contains the implementation of any necessary runtime libraries that are required by the compiled program.

## Statically vs Dynamically typed language

- Statically typed languages are much faster because a dynamically type language has
to perform type checking during run time.

Aurum was initially going to be an interpreted, dynamically typed language and the goal was to eventually
transition to a compiled language. However, I would rather suffer through the pain now.

a. This is seen with Cpython (the reverse can also be seen).
b. CPython can be defined as both an interpreter and a compiler as it compiles Python code into bytecode before interpreting it.

## Design

1. Functions (there must be one called main) - fn
2. PascalCase (name convention)
3. The language is going to have semicolons. The Walter Bright in a discussion in the
D programming language forms said ["Because semicolons add redundancy which helps diagnose and isolate common programming errors."](http://forum.dlang.org/post/hbo546$2snd$3@digitalmars.com)

Popular languages have generated code produced by a tool like ANTLR, Flex, or Bison based on a given grammar or set of rules.

### Handle Multiple Operators

J programming language uses a combination of a **trie** and a **hash table** for its lexer. The trie is used to quickly match individual characters of operators, while the hash table is used to handle multi-character operators and to store their associated tokens.

The [trie data structure](https://en.wikipedia.org/wiki/Trie) used for locating specific keys from within a set.

Just like C, you can define a string in Aurum with an array of bytes or as a null-terminated string literal.

```c
TrieNode *create_trie_node() 
{
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    node->is_operator = 0;
    memset(node->children, 0, sizeof(node->children));
    return node;
}

void insert_operator(TrieNode *root, const char *op) 
{
    TrieNode *current = root;
    while (*op) {
        int index = (int)*op;
        if (!current->children[index]) {
            current->children[index] = create_trie_node();
        }
        current = current->children[index];
        op++;
    }
    current->is_operator = 1;
}

if (operators_trie->children[(int)c]);
```

## Goals

- Aurum has a more flexible macro system than C, which allows for more powerful code generation and metaprogramming.

- Aurum built-in features and functions that are not present in C, such as closures, first-class functions, and coroutines.

- Aurum has a simpler syntax than C, which can make it easier to read and write.

- It's whitespace independent

- Aurum is pass by value. (I will need a & operator)

- Aurum has type safety. Maybe borrow checker?

## Influence

Haskell, Rust, J, C, Python, Javascript.

[HolyC - Datatypes](https://tinkeros.github.io/WbGit/Doc/HolyC.html)

U0      void, but ZERO size!
I8      char
U8      unsigned char
I16     short
U16     unsigned short
I32     int
U32     unsigned int
I64     long (64-bit)
U64     unsigned long (64-bit)
F64     double
F32     float.

I'm hoping to allow up to [256 bits](https://en.wikipedia.org/wiki/Arbitrary-precision_arithmetic)? I could possibly just do like I64 F64?


Some programming languages have an option to use, arbitrary-precision numbers for all integer arithmetic. So python supports the int type which supports arbitrary-precision arithmetic, so it can handle integers of any size. I'm unsure if I should let the user decide.

[J - Control Structures / Math Operations](https://www.jsoftware.com/help/jforc/control_structures.htm)

### Links

- Building a Compiler (Backend) like LLVM with [QBE](https://c9x.me/compile/). QBE is extremely small but very functional.

- J Programming langauge is written in [Portable C](https://www.jsoftware.com/#/README) that allows it to be available for Windows, Linux, Mac, iOS, Android and Raspberry Pi.

### End Goal

Operating system on x86 platform that runs 8-bit ascii using Aurum and x86 assembly. x86 assembly can make it easier to interface with low-level hardware and to take advantage of the capabilities of modern CPUs. At the end of the day everything is just a text file.

## Other

I like how Rust, Zig, and HolyC use the following types
u32, i32... etc. I'm going to use the same types.
https://doc.rust-lang.org/book/ch03-02-data-types.html
