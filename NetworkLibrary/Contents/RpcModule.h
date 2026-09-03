#pragma once

#include "ResponseCode.h"
#include "Player.h"
#include "Character.h"
#include "OmokGame.h"

#include "../Rpc/RpcServerProxy.h"
#include "../Rpc/RpcClientProxy.h"
#include "../Database/DbProxy.h"

extern RpcServerProxy g_RpcProxy;
extern RpcClientProxy g_ClientRpcProxy;

extern DbProxy g_DbProxy;
