@echo off

copy /Y  "unityserver.idl" ".\bin\Release\net8.0\unityserver.idl"

start /wait "" ".\bin\Release\net8.0\RpcGenerator.exe"

move /Y "RpcServerProxy.h" "..\NetworkLibrary\Rpc\"
move /Y "RpcServerStub.h" "..\NetworkLibrary\Rpc\"

exit
