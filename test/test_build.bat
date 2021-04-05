..\..\cp1\tools\llc -filetype=obj cbnf.llvm -march=x86-64
link cbnf.llvm.obj Library.lib /subsystem:console /machine:x64 /OUT:"cbnf.llvm.exe"
