#include "stdafx.h"

// flag - 0/1/2 : toggle / on / off
void SetSpeaker( int flag ) {
    // ����Ƿ���ڻ������豸
    UINT num = ::mixerGetNumDevs();
    if (num == 0) return;

    // �ٶ�ϵͳ��ֻ����һ�������������ԾͲ��� mixerOpen() ��
    //HMIXER mix;
    //MMRESULT mmr = ::mixerOpen(&mix, 0, 0L, 0L, 0L);

    // ������������ÿ������ߣ��ҵ� Master Volume
    DWORD dwMasterLineID = 0L;
    MIXERCAPS caps;
    MMRESULT mmr = ::mixerGetDevCaps(0, &caps, sizeof(MIXERCAPS));
    for (DWORD d=0; d < caps.cDestinations; d++) {
        MIXERLINE mxl;
        mxl.cbStruct = sizeof(MIXERLINE);
        mxl.dwDestination = d;
        mmr = ::mixerGetLineInfo(0, &mxl, MIXER_GETLINEINFOF_DESTINATION);
        if (mxl.dwComponentType == MIXERLINE_COMPONENTTYPE_DST_SPEAKERS) {
            dwMasterLineID = mxl.dwLineID;
            break;
        }
    }

    // �ҵ� Master Volume �� mute control
    MIXERCONTROL mc;
    MIXERLINECONTROLS mxlc;
    mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
    mxlc.dwLineID = dwMasterLineID;
    mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
    mxlc.cControls = 1;
    mxlc.cbmxctrl = sizeof(MIXERCONTROL);
    mxlc.pamxctrl = &mc;
    mmr = ::mixerGetLineControls(0, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);

    // ���þ���/�Ǿ���
    MIXERCONTROLDETAILS_BOOLEAN b;
    MIXERCONTROLDETAILS mxcd;
    mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
    mxcd.dwControlID = mc.dwControlID;
    mxcd.cChannels = 1; // for MIXERCONTROL_CONTROLF_UNIFORM
    mxcd.cMultipleItems = 0;
    mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
    mxcd.paDetails = &b;
    mmr = ::mixerGetControlDetails(0, &mxcd, MIXER_GETCONTROLDETAILSF_VALUE);

    switch ( flag ) {
    case 1:
        b.fValue = FALSE;
        break;
    case 2:
        b.fValue = TRUE;
        break;
    case 0:
    default:
        b.fValue = ! b.fValue;
        break;
    }

    mmr = ::mixerSetControlDetails(0, &mxcd, MIXER_GETCONTROLDETAILSF_VALUE);

    //::mixerClose(mix);
}

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
int main(int argc, _TCHAR* argv[])
//int _tmain(int argc, _TCHAR* argv[])
{
/*
    Speaker [-on | -off]
*/
    CCmdLine cmdLine;
    cmdLine.SplitLine( argc, argv );

    int flag = 0;
    if ( cmdLine.HasSwitch( "-on" ) ) {
        flag = 1;
    } else if ( cmdLine.HasSwitch( "-off" ) ) {
        flag = 2;
    }
    SetSpeaker( flag );
    return 0;
}

