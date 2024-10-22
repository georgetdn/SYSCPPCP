@echo off
cls
REM Print the start time
echo/   
echo Start Time: %date% %time%
time /T

REM Define relative paths for the projects (since we are in the SYSCPPCP directory)
set GENERATOR_PROJ=SYSCPPCPcodeGenrtators\SYSCPPCPgenerator\SYSCPPCPGenerator.vcxproj
set GENERATOR_EXE=SYSCPPCPcodeGenrtators\
set TESTONE_PROJ=SYSCPPCPtest\GenTestData\GenTestData.vcxproj
set TESTTWO_PROJ=SYSCPPCPtest\QuickStart\QuickStart.vcxproj
set SQLGENERATOR_PROJ=SYSCPPCPcodeGenrtators\SmallSQLgenerator\SmallSQLgenerator.vcxproj

REM Function to check error level after each build step and display the correct project name
echo/
echo This batch file will bild SYSCPPCP system
echo The build should take about five minutes
echo/
pause

REM Compile projects in Debug mode
echo/
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo Compiling SYSCPPCPGenerator in Debug mode...
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo/
call msbuild %GENERATOR_PROJ% /p:Configuration=Debug  /p:Platform=x64 > build.log

REM Check if the program returned 0 (success)
if %ERRORLEVEL%==0 (
    echo %GENERATOR_PROJ% compiled successfully.
) else (
    echo %GENERATOR_PROJ% failed with exit code %ERRORLEVEL%.
	exit /b %ERRORLEVEL%  
)

REM Compile projects in Release mode
echo/
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo Compiling SYSCPPCPGenerator in Release mode...
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo/
call msbuild %GENERATOR_PROJ% /p:Configuration=Release  /p:Platform=x64 >> build.log

REM Check if the program returned 0 (success)
if %ERRORLEVEL%==0 (
    echo %GENERATOR_PROJ% compiled successfully.
	echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	echo/
) else (
    echo %GENERATOR_PROJ% failed with exit code %ERRORLEVEL%.
	exit /b %ERRORLEVEL%  
)


REM Execute SYSCPPCPGenerator
echo/
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo executing SYSCPPCPGenerator in Debug mode
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo/
cd %GENERATOR_EXE%debug\
SYSCPPCPGenerator    >> build.log
if %ERRORLEVEL%==0 (
    echo SYSCPPCPGenerator execute successfully.
	echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	echo/
) else (
    echo SYSCPPCPGenerator failed with exit code %ERRORLEVEL%.
	exit /b %ERRORLEVEL%  
)
cd ../../
REM Execute SYSCPPCPGenerator
echo/
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo Executing SYSCPPCPGenerator in Release mode
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo
cd %GENERATOR_EXE%release\
SYSCPPCPGenerator    >> build.log
if %ERRORLEVEL%==0 (
    echo SYSCPPCPGenerator executed successfully.
	echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	echo/
) else (
    echo SYSCPPCPGeneratorfailed with exit code %ERRORLEVEL%.
	exit /b %ERRORLEVEL%  
)
cd ../../
echo/
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo Compiling SmallSQLgenerator in Debug mode...
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo/
call msbuild %SQLGENERATOR_PROJ% /p:Configuration=Debug   /p:Platform=x64    >> build.log

if %ERRORLEVEL%==0 (
    echo %SQLGENERATOR_PROJ% compiled successfully.
	echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	echo/
) else (
    echo %SQLGENERATOR_PROJ% failed with exit code %ERRORLEVEL%.
	exit /b %ERRORLEVEL%  
)


echo/
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo Compiling SmallSQLgenerator in Release mode...
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo/
call msbuild %SQLGENERATOR_PROJ% /p:Configuration=Release  /p:Platform=x64   >> build.log
if %ERRORLEVEL%==0 (
    echo %SQLGENERATOR_PROJ% compiled successfully.
		echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	echo/
) else (
    echo %SQLGENERATOR_PROJ% failed with exit code %ERRORLEVEL%.
	exit /b %ERRORLEVEL%  
)

cd %GENERATOR_EXE%debug\
echo/
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo Executng SmallSQLgenerator debug
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo/
SmallSQLgenerator     >> build.log
if %ERRORLEVEL%==0 (
    echo SmallSQLgenerator executed successfully.
		echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	echo/
) else (
    echo SmallSQLgenerator failed with exit code %ERRORLEVEL%.
	exit /b %ERRORLEVEL%  
)
cd ../../

echo/
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo Compiling GenTestData in Release mode...
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo/
call msbuild %TESTONE_PROJ% /p:Configuration=Release  /p:Platform=x64    >> build.log
if %ERRORLEVEL%==0 (
    echo %TESTONE_PROJ% compiled successfully.
	echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	echo/
) else (
    echo %TESTONE_PROJ% failed with exit code %ERRORLEVEL%.
	exit /b %ERRORLEVEL%  
)


echo/
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo Compiling QuickStart in Release mode...
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo/
call msbuild %TESTTWO_PROJ% /p:Configuration=Release   /p:Platform=x64   >> build.log

if %ERRORLEVEL%==0 (
    echo %TESTTWO_PROJ% compiled successfully.
	echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	echo/
) else (
    echo %TESTTWO_PROJ% failed with exit code %ERRORLEVEL%.
	exit /b %ERRORLEVEL%  
)

echo/
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo/
echo All projects compiled successfully.
echo To start getting familiar with the system read
	echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo/
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo Compiling GenTestData in Debug mode...
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo/
call msbuild %TESTONE_PROJ% /p:Configuration=Debug   /p:Platform=x64    >> build.log
if %ERRORLEVEL%==0 (
    echo %TESTONE_PROJ% compiled successfully.
	echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	echo/
) else (
    echo %TESTONE_PROJ% failed with exit code %ERRORLEVEL%.
	exit /b %ERRORLEVEL%  
)
echo/
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo Compiling QuickStart in Debug mode...
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo/
call msbuild %TESTTWO_PROJ% /p:Configuration=Debug   /p:Platform=x64   >> build.log
if %ERRORLEVEL%==0 (
    echo %TESTTWO_PROJ% compiled successfully.
	echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	echo/
) else (
    echo %TESTTWO_PROJ% failed with exit code %ERRORLEVEL%.
	exit /b %ERRORLEVEL%  
)
REM Print the end time
echo End Time: %date% %time%
time /T
echo/	
	pause
		start "" "tutorials/GettingStarted.txt"
exit /b 0
