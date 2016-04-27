@echo off
java -jar CrossRun.jar "interactor.exe tests/%1 output.txt" %2

if errorlevel 1 (
	echo Some error occured
	exit 1
)

echo Processes are finished, the calculated value is:
type output.txt
