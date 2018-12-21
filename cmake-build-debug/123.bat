cd C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug
:START
start segment.exe
:WAIT1
tasklist |find /i "segment.exe"
if %errorlevel%==0 (goto WAIT1)
REM :START2
REM start python ./A4Warping/hand/test.py
REM :WAIT2
REM tasklist |find /i "python.exe"
REM if %errorlevel%==0 (goto WAIT2)
REM start ./writeHelper/cmake-build-debug/writeHelper.exe