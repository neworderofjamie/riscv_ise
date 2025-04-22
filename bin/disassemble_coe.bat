ECHO OFF
for /F "skip=2 tokens=*" %%A in (%1) do disassembler_cli_Release.exe 0x%%A