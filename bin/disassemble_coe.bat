ECHO OFF
SETLOCAL EnableDelayedExpansion
SET B=0
FOR /F "skip=2 tokens=*" %%A IN (%1) DO (
<NUL SET  /p=!B!:
disassembler_cli_Release.exe 0x%%A
SET /A B+=4
)