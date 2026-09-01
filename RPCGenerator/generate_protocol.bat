@echo off

start /wait "" ".\bin\Release\net8.0\RpcGenerator.exe"

move /Y "RpcServerProxy.h" "..\NetworkLibrary\Rpc\"
move /Y "RpcServerProxy.cpp" "..\NetworkLibrary\Rpc\"
move /Y "RpcServerStub.h" "..\NetworkLibrary\Rpc\"
move /Y "RpcServerStub.cpp" "..\NetworkLibrary\Rpc\"

move /Y "RpcClientProxy.h" "..\NetworkLibrary\Rpc\"
move /Y "RpcClientProxy.cpp" "..\NetworkLibrary\Rpc\"
move /Y "RpcClientStub.h" "..\NetworkLibrary\Rpc\"
move /Y "RpcClientStub.cpp" "..\NetworkLibrary\Rpc\"

move /Y "DbProxy.h" "..\NetworkLibrary\Database\"
move /Y "DbProxy.cpp" "..\NetworkLibrary\Database\"
move /Y "DbStub.h" "..\NetworkLibrary\Database\"
move /Y "DbStub.cpp" "..\NetworkLibrary\Database\"

del /Q ".\bin\Release\net8.0\RpcServerProxy.h"
del /Q ".\bin\Release\net8.0\RpcServerProxy.cpp"
del /Q ".\bin\Release\net8.0\RpcServerStub.h"
del /Q ".\bin\Release\net8.0\RpcServerStub.cpp"

del /Q ".\bin\Release\net8.0\RpcClientProxy.h"
del /Q ".\bin\Release\net8.0\RpcClientProxy.cpp"
del /Q ".\bin\Release\net8.0\RpcClientStub.h"
del /Q ".\bin\Release\net8.0\RpcClientStub.cpp"

del /Q ".\bin\Release\net8.0\DbProxy.h"
del /Q ".\bin\Release\net8.0\DbProxy.cpp"
del /Q ".\bin\Release\net8.0\DbStub.h"
del /Q ".\bin\Release\net8.0\DbStub.cpp"

exit
