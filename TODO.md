# Roadmap for building the pity interpreter

- Build the AST data structure
- Build pity lexer and test
- Implement the basic functionality of Pity objects just for the sake of completely accomplish the parsing process and test
- Write a module for interacting with PRCE2 and implement the `Regex` and `Match` object
- Build AST by parsing Tokens. test, test, and test the AST AST
- Setup the list of pity vm opcodes
- Implement the compiler and its debugger (While compiling, apply Tail Call optimization and NaN Boxing). Test, test, and test
- Implement the bytecode runner. Test, test and test
- Complete the implementation of pity objects, test, test and test
- Optimize the AST with the following optimizations

1. Constant folding, test AST
2. Common subexpression elimination, test AST
3. Dead code elimination, test AST

- Apply the following optimization during the compilation process

1. Constant propagation, test compiler
2. Code hoisting, test compiler
3. (Maybe)? Register Allocation

- Test language features
- Test objects
- Write 150 rosetta codes to test, test and test
- Write documentation
