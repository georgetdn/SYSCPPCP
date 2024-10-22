@echo off

for /D %%d in ("SYSCPPCPcodeGenrtators\SmallSQLgenerator\SmallSQL*") do (
    rmdir /S /Q "%%d"
)
del /Q SYSCPPCPcodeGenrtators\debug\*.*
del /Q SYSCPPCPcodeGenrtators\release\*.*
rmdir /S /Q "SYSCPPCPcodeGenrtators\SYSCPPCPgenerator\.vs
rmdir /S /Q "SYSCPPCPcodeGenrtators\SYSCPPCPgenerator\SYSCPPCP*"
rmdir /S /Q "SYSCPPCPcodeGenrtators\SmallSQLgenerator\.vs
rmdir /S /Q "SYSCPPCPcodeGenrtators\SmallSQLgenerator\SYSCPPCP*"


rmdir /S /Q   "SmallSQL\debug\temp"
del /Q SmallSQL\debug\*.*
rmdir /S /Q   "SmallSQL\release\temp"	
del /Q SmallSQL\release\*.*
del /Q SmallSQLHeaders\*.*
del /Q SmallSQLsource\*.*
ren "SYSCPPCPheaders\Database.h" "Database.h_" 
ren "SYSCPPCPheaders\Record.h" "Record.h_"
del /Q SYSCPPCPheaders\*.h
ren "SYSCPPCPheaders\Database.h_" "Database.h"
ren "SYSCPPCPheaders\Record.h_" "Record.h"
ren "SYSCPPCPlibs\debug\SYSCPPCPd.lib" "SYSCPPCPd.lib_" 
ren "SYSCPPCPlibs\debug\SYSCPPCPd.idb" "SYSCPPCPd.idb_" 
ren "SYSCPPCPlibs\debug\SYSCPPCPd.pdb" "SYSCPPCPd.pdb_"
REM Delete files without the underscore
REM Delete only files that do not end with an underscore
for %%f in (SYSCPPCPlibs\debug\*.lib SYSCPPCPlibs\debug\*.pdb SYSCPPCPlibs\debug\*.idb) do (
    if not "%%~xf"==".lib_" if not "%%~xf"==".pdb_" if not "%%~xf"==".idb_" (
        del /Q "%%f"
    )
)
ren SYSCPPCPlibs\debug\SYSCPPCPd.lib_ SYSCPPCPd.lib 
ren SYSCPPCPlibs\debug\SYSCPPCPd.idb_ SYSCPPCPd.idb 
ren SYSCPPCPlibs\debug\SYSCPPCPd.pdb_ SYSCPPCPd.pdb 


ren SYSCPPCPlibs\release\SYSCPPCP.lib SYSCPPCP.lib_ 
ren SYSCPPCPlibs\release\SYSCPPCP.pdb SYSCPPCP.pdb_
for %%f in (SYSCPPCPlibs\release\*.lib SYSCPPCPlibs\release\*.pdb SYSCPPCPlibs\release\*.idb) do (
    if not "%%~xf"==".lib_" if not "%%~xf"==".pdb_"  (
        del /Q "%%f"
    )
)
ren SYSCPPCPlibs\release\SYSCPPCP.lib_ SYSCPPCP.lib 
ren SYSCPPCPlibs\release\SYSCPPCP.pdb_ SYSCPPCP.pdb 

del /Q SYSCPPCPsource\*.*

del /Q SYSCPPCPtest\TestOne\Debug\*.*
del /Q SYSCPPCPtest\TestOne\Release\*.*
rmdir /S /Q  "SYSCPPCPtest\TestOne\TestOne"
rmdir /S /Q  "SYSCPPCPtest\TestOne\x64"

del /Q SYSCPPCPtest\TestThree\Debug\*.*
del /Q SYSCPPCPtest\TestThree\Release\*.*
rmdir /S /Q  "SYSCPPCPtest\TestThree\TestThree"
rmdir /S /Q  "SYSCPPCPtest\TestThree\x64"

del /Q SYSCPPCPtest\TestTwo\Debug\*.*
del /Q SYSCPPCPtest\TestTwo\Release\*.*
rmdir /S /Q  "SYSCPPCPtest\TestTwo\TestTwo"
rmdir /S /Q  "SYSCPPCPtest\TestTwo\x64"

rmdir /S /Q   "SYSCPPCPvcxproj\temp"
del /Q "SYSCPPCPvcxproj\*.*"



