﻿@echo off
cd .\A4Warping\cmake-build-debug\
start segment.exe
:WAIT1
tasklist |find /i "segment.exe" > nul
if %errorlevel%==0 (goto WAIT1)
cd ..\..\
start python test.py
:WAIT2
tasklist |find /i "python.exe"  > nul
if %errorlevel%==0 (goto WAIT2)
cd .\writeHelper\cmake-build-debug\
start writeHelper.exe
tasklist |find /i "writeHelper.exe" > nul
if %errorlevel%==0 (goto WAIT1)
cd ..\..\A4Warping\result\
start result.csv
mshta vbscript:msgbox("Finish! Result store in:dir:./A4Warping/result/result.csv and... Images store in ./A4Warping/data/ Thanks!",64,"Finish!")(window.close)
