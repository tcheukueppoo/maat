# Roadmap for building the pity interpreter

- Build the AST data structure
- Build pity lexer and test
- Implement the basic functionality of Pity objects just for the sake of completely accomplish the parsing process and test
- Write a module for interacting with PRCE2 and implement the `Regex` and `Match` object
- Build AST by parsing Tokens
- Write Deparser for testing AST
- Setup the list of pity vm opcodes
- Build compiler data structure
- Implement the compiler and its debugger (While compiling, apply Tail Call optimization and NaN Boxing). Test, test, and test
- Implement the bytecode runner. Test, test and test
- Complete the implementation of pity objects, test, test and test
- Optimize the AST with the following optimizations

1. Constant folding, test AST by deparsing
2. Common subexpression elimination, test AST ...
3. Dead code elimination, test AST ...

- Apply the following optimization during the compilation process

1. Constant propagation, test compiler
2. Code hoisting, test compiler
3. (Maybe)? Register Allocation, test compiler

- Make the deparser be `pityfy` for pityfying
- Test language features individualy
- Test objects
- Write 150 rosetta codes to test `pity` as a whole
- Write documentation
- Call friends to make builders for plan9, BSDs, Linux, Mac and Windows platforms
- Call friends to setup CI/CD pipeline to package build pity for the following linux and BSD distributions

1. (Open|Free|Net)BSD
2. Debian & Ubuntu
3. Alpine Linux
4. Archlinux
5. 

- Call friends/(or why not me?) to make windows installer GUI app
