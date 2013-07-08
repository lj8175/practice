#include "GetState.h"
#include "GetInfo.h"
#include "ActionInfo.h"
#include "AsyncFrame.h"
#include "CommDef.h"
#include "msg.h"


int CGetState::HandleEncode(CAsyncFrame *pFrame,
        CActionSet *pActionSet,
        CMsgBase *pMsg) 
{
    static CGetInfo GetInfo;

    //CMsg *msg = (CMsg *)pUserData;
    CActionInfo *pAction1 = new CActionInfo(512);
    pAction1->SetID(1);
    pAction1->SetDestIp("10.136.9.77");
    pAction1->SetDestPort(9101);
    pAction1->SetProto(ProtoType_TCP);
    pAction1->SetActionType(ActionType_SendRecv_KeepAlive);
    pAction1->SetTimeout(200);
    pAction1->SetIActionPtr((IAction *)&GetInfo);

    pActionSet->AddAction(pAction1);

    return 0;
}

int CGetState::HandleProcess(CAsyncFrame *pFrame,
        CActionSet *pActionSet,
        CMsgBase *pMsg)
{
    CMsg *msg = (CMsg *)pMsg;
    int err1 = 0, err2 = 0;
    int cost1 = 0, cost2 = 0;
    int size1 = 0, size2 = 0;

    CActionSet::ActionSet &action_set = pActionSet->GetActionSet();
    CActionSet::ActionSet::iterator it = action_set.begin();
    for(; it != action_set.end(); it++ )
    {
        CActionInfo *pInfo = *it;
        char *buf = NULL;

        int id;
        pInfo->GetID(id);
        
        if( id == 1 )
        {
            pInfo->GetErrno(err1);
            pInfo->GetBuffer( &buf, size1);

            pInfo->GetTimeCost( cost1 );
        }

    }

    return STATE_ID_FINISHED;
}
