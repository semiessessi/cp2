..\..\cp1\tools\llc -filetype=obj cbnf.llvm -march=x86-64 -O=3
link cbnf.llvm.obj Lexer.lib Library.lib Parser.lib Common.lib /subsystem:console /machine:x64 /OUT:"cbnf.llvm.exe"
