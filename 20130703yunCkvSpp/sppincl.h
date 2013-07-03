#ifndef _SPP_INCL_H_
#define _SPP_INCL_H_
#include "spp_incl/tlog.h"			//日志
#include "spp_incl/tstat.h"			//统计
#include "spp_incl/tprocmon.h"		//监控
#include "spp_incl/tcommu.h"		//通讯组件
#include "spp_incl/serverbase.h"	//服务器容器
#include "spp_incl/benchapi.h"
#include "spp_incl/asyn_api.h"
#include "spp_incl/spp_async_interface.h"
#include "spp_incl/CommDef.h"
#include "spp_incl/AsyncFrame.h"
#include "spp_incl/IState.h"
#include "spp_incl/IAction.h"
#include "spp_incl/ActionInfo.h"
#include "spp_incl/ActionSet.h"

#define GROUPID(x) (((x)|1<<31))
using namespace tbase::tlog;
using namespace tbase::tstat;
using namespace tbase::tprocmon;
using namespace tbase::tcommu;
using namespace spp::comm;
using namespace spp::comm;
USING_ASYNCFRAME_NS;

#endif
