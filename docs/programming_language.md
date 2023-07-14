# Aurum Programming Language

## Aurum is "gold" in Latin

### FileType: au

\
*Aurum* is an untyped, interpreted, stack-oriented programming language that's focused on simplicity and minimalism.

### Why?

I wanted to learn how to create a programming language from scratch. I was going to go all in and create a compiled, general-purpose, statically typed programming language, but I decided to start small and work my way up. I read on [reddit](https://www.reddit.com/r/ProgrammingLanguages/comments/4jtwf8/comment/d39l2rm/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button) that creating a stack-oriented language is a good place to start, so I did.

## File Structure

The `bin/` directory is used to store the executable files for my interpreter.

The `lib/` is the standard library for Aurum.

The `src/` directory contains the source code for the interpreter.

The `tests/` directory contains the test files for the interpreter.

The `docs/` directory contains the documentation for the interpreter.

## Plan

- [x] Part 1: Lexer
- [x] Part 2: Interpreter
- [ ] Part 3: Standard Library
- [ ] Part 4: Documentation
- [ ] Part 5: Tests

## Definitions

The `lexer` is responsible for taking the source code of a program as input and breaking it down into a sequence of tokens, which are meaningful units of the programming language such as keywords, identifiers, operators, and literals.

The `interpreter` directory contains the implementation of the code execution phase of the interpreter. An interpreter is a program that translates a high-level programming language into machine code line by line during runtime.

## Syntax

The syntax of Aurum is inspired by Forth and Lua.

## Goal

The end goal is to create a imperative, interpreted, general-purpose, dynamically typed programming language.
