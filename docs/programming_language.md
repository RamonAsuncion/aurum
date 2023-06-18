# Aurum Programming Language

## Aurum is "gold" in Latin

### FileType: au

\
*Aurum* is an untyped, interpreted, stack-oriented programming language that's focused on simplicity and minimalism.

## File Structure

The `bin/` directory is used to store the executable files for my interpreter.

## Plan

- [x] Part 1: Lexer
- [ ] Part 2: Interpreter 
- [ ] Part 3: Parser

## Definitions

The `lexer` is responsible for taking the source code of a program as input and breaking it down into a sequence of tokens, which are meaningful units of the programming language such as keywords, identifiers, operators, and literals.

The `parser` then takes this sequence of tokens produced by the **lexer** and applies a grammar to it, which defines the structure and syntax of the programming language. The parser verifies that the input program is syntactically correct according to the grammar and produces a parse tree or an **abstract syntax tree** (AST) as output.

The `interpreter` directory contains the implementation of the code execution phase of the interpreter. An interpreter is a program that translates a high-level programming language into machine code line by line during runtime.

## Goal

The end goal is to create a imperative, compiled, general-purpose, statically typed programming language. 

