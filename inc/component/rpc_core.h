//
// Created by hujianzhe
//

#ifndef UTIL_C_COMPONENT_RPC_CORE_H
#define	UTIL_C_COMPONENT_RPC_CORE_H

#include "../datastruct/list.h"
#include "../datastruct/rbtree.h"
#include "../sysapi/process.h"

typedef struct RpcItem_t {
	ListNode_t listnode; /* user use, library not use this field */
	void* originator; /* user use, library not use this field */
	void* timeout_ev; /* user use, library not use this field */
	RBTreeNode_t m_treenode;
	char m_has_reg;
	int id;
	long long timestamp_msec;
	void* async_req_arg;
	void(*async_callback)(struct RpcItem_t*);
	Fiber_t* fiber;
	void* ret_msg;
} RpcItem_t;

typedef struct RpcFiberCore_t {
	Fiber_t* msg_fiber;
	Fiber_t* cur_fiber;
	Fiber_t* sche_fiber;
	Fiber_t* from_fiber;
	int ret_flag;
	size_t stack_size;
	void* new_msg;
	RpcItem_t* reply_item;
	void(*msg_handler)(struct RpcFiberCore_t*, void*);
	RBTree_t rpc_item_tree;
} RpcFiberCore_t;

typedef struct RpcAsyncCore_t {
	RBTree_t rpc_item_tree;
} RpcAsyncCore_t;

#ifdef __cplusplus
extern "C" {
#endif

__declspec_dll int rpcGenId(void);
__declspec_dll RpcItem_t* rpcItemSet(RpcItem_t* item, int rpcid);

__declspec_dll RpcAsyncCore_t* rpcAsyncCoreInit(RpcAsyncCore_t* rpc);
__declspec_dll void rpcAsyncCoreDestroy(RpcAsyncCore_t* rpc);
__declspec_dll RpcItem_t* rpcAsyncCoreRegItem(RpcAsyncCore_t* rpc, RpcItem_t* item, void* req_arg, void(*ret_callback)(RpcItem_t*));
__declspec_dll RpcItem_t* rpcAsyncCoreUnregItem(RpcAsyncCore_t* rpc, RpcItem_t* item);
__declspec_dll RpcItem_t* rpcAsyncCoreCallback(RpcAsyncCore_t* rpc, int rpcid, void* ret_msg);
__declspec_dll RpcItem_t* rpcAsyncCoreCancel(RpcAsyncCore_t* rpc, RpcItem_t* item);
__declspec_dll void rpcAsyncCoreCancelAll(RpcAsyncCore_t* rpc, RBTree_t* item_set);

__declspec_dll RpcFiberCore_t* rpcFiberCoreInit(RpcFiberCore_t* rpc, Fiber_t* sche_fiber, size_t stack_size, void(*msg_handler)(RpcFiberCore_t*, void*));
__declspec_dll void rpcFiberCoreDestroy(RpcFiberCore_t* rpc);
__declspec_dll RpcItem_t* rpcFiberCoreRegItem(RpcFiberCore_t* rpc, RpcItem_t* item);
__declspec_dll RpcItem_t* rpcFiberCoreUnregItem(RpcFiberCore_t* rpc, RpcItem_t* item);
__declspec_dll RpcItem_t* rpcFiberCoreYield(RpcFiberCore_t* rpc);
__declspec_dll RpcItem_t* rpcFiberCoreResume(RpcFiberCore_t* rpc, int rpcid, void* ret_msg);
__declspec_dll void rpcFiberCoreResumeMsg(RpcFiberCore_t* rpc, void* new_msg);
__declspec_dll RpcItem_t* rpcFiberCoreCancel(RpcFiberCore_t* rpc, RpcItem_t* item);
__declspec_dll void rpcFiberCoreCancelAll(RpcFiberCore_t* rpc, RBTree_t* item_set);

#ifdef __cplusplus
}
#endif

#endif // !UTIL_C_COMPONENT_RPC_CORE_H
