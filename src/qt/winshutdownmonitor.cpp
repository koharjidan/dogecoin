<<<<<<< HEAD
// Copyright (c) 2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
=======
// Copyright (c) 2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
>>>>>>> f568462ca04b73485d7e41266a2005155ff69707
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "winshutdownmonitor.h"

#if defined(Q_OS_WIN) && QT_VERSION >= 0x050000
#include "init.h"
<<<<<<< HEAD
=======
#include "util.h"
>>>>>>> f568462ca04b73485d7e41266a2005155ff69707

#include <windows.h>

#include <QDebug>

<<<<<<< HEAD
=======
#include <openssl/rand.h>

>>>>>>> f568462ca04b73485d7e41266a2005155ff69707
// If we don't want a message to be processed by Qt, return true and set result to
// the value that the window procedure should return. Otherwise return false.
bool WinShutdownMonitor::nativeEventFilter(const QByteArray &eventType, void *pMessage, long *pnResult)
{
       Q_UNUSED(eventType);

       MSG *pMsg = static_cast<MSG *>(pMessage);

<<<<<<< HEAD
=======
       // Seed OpenSSL PRNG with Windows event data (e.g.  mouse movements and other user interactions)
       if (RAND_event(pMsg->message, pMsg->wParam, pMsg->lParam) == 0) {
            // Warn only once as this is performance-critical
            static bool warned = false;
            if (!warned) {
                LogPrint("%s: OpenSSL RAND_event() failed to seed OpenSSL PRNG with enough data.\n", __func__);
                warned = true;
            }
       }

>>>>>>> f568462ca04b73485d7e41266a2005155ff69707
       switch(pMsg->message)
       {
           case WM_QUERYENDSESSION:
           {
               // Initiate a client shutdown after receiving a WM_QUERYENDSESSION and block
               // Windows session end until we have finished client shutdown.
               StartShutdown();
               *pnResult = FALSE;
               return true;
           }

           case WM_ENDSESSION:
           {
               *pnResult = FALSE;
               return true;
           }
       }

       return false;
}

void WinShutdownMonitor::registerShutdownBlockReason(const QString& strReason, const HWND& mainWinId)
{
    typedef BOOL (WINAPI *PSHUTDOWNBRCREATE)(HWND, LPCWSTR);
    PSHUTDOWNBRCREATE shutdownBRCreate = (PSHUTDOWNBRCREATE)GetProcAddress(GetModuleHandleA("User32.dll"), "ShutdownBlockReasonCreate");
    if (shutdownBRCreate == NULL) {
<<<<<<< HEAD
        qDebug() << "registerShutdownBlockReason : GetProcAddress for ShutdownBlockReasonCreate failed";
=======
        qWarning() << "registerShutdownBlockReason: GetProcAddress for ShutdownBlockReasonCreate failed";
>>>>>>> f568462ca04b73485d7e41266a2005155ff69707
        return;
    }

    if (shutdownBRCreate(mainWinId, strReason.toStdWString().c_str()))
<<<<<<< HEAD
        qDebug() << "registerShutdownBlockReason : Successfully registered: " + strReason;
    else
        qDebug() << "registerShutdownBlockReason : Failed to register: " + strReason;
=======
        qWarning() << "registerShutdownBlockReason: Successfully registered: " + strReason;
    else
        qWarning() << "registerShutdownBlockReason: Failed to register: " + strReason;
>>>>>>> f568462ca04b73485d7e41266a2005155ff69707
}
#endif
