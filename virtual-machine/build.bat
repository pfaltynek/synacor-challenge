@echo off
if not exist out md out
rem call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
rem set compilerflags=/nologo /Od /Zi /EHsc /Fo./out/  /Fd./out/vc140.pdb
set compilerflags=/Od /Zi /EHsc /Fo./out/ /Fd./out/vc140.pdb
set linkerflags=/OUT:out/app.exe /PDB:out/
cl.exe %compilerflags% main.cpp /link %linkerflags%