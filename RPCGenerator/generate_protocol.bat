@echo off

copy /Y  "my_idl.txt" ".\bin\Release\net8.0\"

start /wait "" ".\bin\Release\net8.0\RpcGenerator.exe"

move /Y "RpcServerProxy.h" "..\NetworkLibrary\Rpc\"
move /Y "RpcServerProxy.cpp" "..\NetworkLibrary\Rpc\"
move /Y "RpcServerStub.h" "..\NetworkLibrary\Rpc\"
move /Y "RpcServerStub.cpp" "..\NetworkLibrary\Rpc\"

move /Y "RpcClientProxy.h" "..\NetworkLibrary\Rpc\"
move /Y "RpcClientProxy.cpp" "..\NetworkLibrary\Rpc\"
move /Y "RpcClientStub.h" "..\NetworkLibrary\Rpc\"
move /Y "RpcClientStub.cpp" "..\NetworkLibrary\Rpc\"

move /Y "DatabaseServerProxy.h" "..\NetworkLibrary\Database\"
move /Y "DatabaseServerProxy.cpp" "..\NetworkLibrary\Database\"
move /Y "DatabaseServerStub.h" "..\NetworkLibrary\Database\"
move /Y "DatabaseServerStub.cpp" "..\NetworkLibrary\Database\"

exit
