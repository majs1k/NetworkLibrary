@echo off

start /wait "" ".\bin\Release\net8.0\RpcGenerator.exe"

move /Y "RpcServerProxy.h" "..\Game\Rpc\"
move /Y "RpcServerProxy.cpp" "..\Game\Rpc\"
move /Y "RpcServerStub.h" "..\Game\Rpc\"
move /Y "RpcServerStub.cpp" "..\Game\Rpc\"

move /Y "RpcClientProxy.h" "..\Game\Rpc\"
move /Y "RpcClientProxy.cpp" "..\Game\Rpc\"
move /Y "RpcClientStub.h" "..\Game\Rpc\"
move /Y "RpcClientStub.cpp" "..\Game\Rpc\"

move /Y "DbProxy.h" "..\Game\Database\"
move /Y "DbProxy.cpp" "..\Game\Database\"
move /Y "DbStub.h" "..\Game\Database\"
move /Y "DbStub.cpp" "..\Game\Database\"

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
