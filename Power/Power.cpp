#include "stdafx.h"

#include <PowrProf.h>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
int main(int argc, _TCHAR* argv[])
//int _tmain(int argc, _TCHAR* argv[])
{
/*
    Power [-suspend | -hibernate]
*/
    CCmdLine cmdLine;
    cmdLine.SplitLine( argc, argv );

    if ( cmdLine.HasSwitch( "-suspend" ) ) {

        //::LockWorkStation(); // ËøÆÁ
        ::SetSuspendState( FALSE, FALSE, FALSE ); // ´ý»ú

    } else if ( cmdLine.HasSwitch( "-hibernate" ) ) {

        ::SetSuspendState( TRUE, FALSE, FALSE ); // ÐÝÃß

    }
    return 0;
}

