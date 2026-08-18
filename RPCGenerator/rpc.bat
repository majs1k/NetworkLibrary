@echo off

copy /Y  "unityserver.idl" ".\bin\Release\net8.0\unityserver.idl"

start /wait "" ".\bin\Release\net8.0\RpcGenerator.exe"

move /Y "ServerProxy.h" "..\NetworkLibrary\Rpc\"

exit
