@echo off

pushd build

del /q *

set MainFile=../src/main.c

set MainCompilerFlags=/Fe"raytracer"

set CommonCompilerFlags=/nologo /MD /EHsc /Od /Zi /W4 /wd4100 /wd4099 /wd4204 /wd4996 /wd4505 /we4702 /wd4201 /wd4200 /wd4133
set Defines=/D_CRT_SECURE_NO_WARNINGS
set LinkLibraries=Kernel32.lib Ole32.lib

cl %CommonCompilerFlags% %MainCompilerFlags% %Defines% %MainFile% /link %LinkLibraries% 

popd
