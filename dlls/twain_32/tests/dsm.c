/* Unit test suite for Twain DSM functions
 *
 * Copyright 2009 Jeremy White, CodeWeavers, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */
#include <stdarg.h>
#include <assert.h>
#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winerror.h"
#include "winuser.h"
#include "twain.h"

#include "wine/test.h"

static DSMENTRYPROC pDSM_Entry;

static BOOL dsm_RegisterWindowClasses(void)
{
    WNDCLASSA cls;

    cls.style = 0;
    cls.lpfnWndProc = DefWindowProc;
    cls.cbClsExtra = 0;
    cls.cbWndExtra = 0;
    cls.hInstance = GetModuleHandleA(0);
    cls.hIcon = 0;
    cls.hCursor = LoadCursorA(0, IDC_ARROW);
    cls.hbrBackground = GetStockObject(WHITE_BRUSH);
    cls.lpszMenuName = NULL;
    cls.lpszClassName = "TWAIN_dsm_class";
    if (!RegisterClassA(&cls)) return FALSE;

    return TRUE;
}


static void get_condition_code(TW_IDENTITY *appid, TW_STATUS *status)
{
    TW_UINT16 rc;
    rc = pDSM_Entry(appid, NULL, DG_CONTROL, DAT_STATUS, MSG_GET, status);
    ok(rc == TWRC_SUCCESS, "Condition code not available, rc %d\n", rc);
}

static void test_sources(TW_IDENTITY *appid)
{
    TW_UINT16 rc;
    TW_IDENTITY source;
    TW_STATUS status;

    memset(&source, 0, sizeof(source));
    rc = pDSM_Entry(appid, NULL, DG_CONTROL, DAT_IDENTITY, MSG_GETFIRST, &source);
    get_condition_code(appid, &status);
    todo_wine
    ok(rc == TWRC_SUCCESS || rc == TWRC_FAILURE, "Get first error code, rc %d, cc %d\n", rc, status.ConditionCode);
    if (rc == TWRC_SUCCESS)
        ok(status.ConditionCode == TWCC_SUCCESS,"Get first invalid condition code, rc %d, cc %d\n", rc, status.ConditionCode);
    if (rc == TWRC_FAILURE)
        ok(status.ConditionCode == TWCC_NODS,"Get first invalid condition code, rc %d, cc %d\n", rc, status.ConditionCode);

    while (rc == TWRC_SUCCESS)
    {
        trace("Got scanner %s\n", source.ProductName);
        memset(&source, 0, sizeof(source));
        rc = pDSM_Entry(appid, NULL, DG_CONTROL, DAT_IDENTITY, MSG_GETNEXT, &source);
        get_condition_code(appid, &status);
        ok(rc == TWRC_SUCCESS || rc == TWRC_ENDOFLIST, "Get next source failed, rc %d, cc %d\n", rc, status.ConditionCode);
    }

    memset(&source, 0, sizeof(source));
    rc = pDSM_Entry(appid, NULL, DG_CONTROL, DAT_IDENTITY, MSG_GETDEFAULT, &source);
    get_condition_code(appid, &status);
    ok(rc == TWRC_SUCCESS || rc == TWRC_FAILURE, "Get default error code, rc %d, cc %d\n", rc, status.ConditionCode);
    if (rc == TWRC_SUCCESS)
    {
        todo_wine
        ok(status.ConditionCode == TWCC_SUCCESS,"Get default invalid condition code, rc %d, cc %d\n", rc, status.ConditionCode);
    }

    if (rc == TWRC_FAILURE)
        ok(status.ConditionCode == TWCC_NODS,"Get default invalid condition code, rc %d, cc %d\n", rc, status.ConditionCode);

    if (rc == TWRC_SUCCESS && status.ConditionCode == TWCC_SUCCESS)
    {
        rc = pDSM_Entry(appid, NULL, DG_CONTROL, DAT_IDENTITY, MSG_OPENDS, &source);
        get_condition_code(appid, &status);

        if (rc == TWRC_SUCCESS && status.ConditionCode == TWCC_SUCCESS)
        {
            rc = pDSM_Entry(appid, NULL, DG_CONTROL, DAT_IDENTITY, MSG_CLOSEDS, &source);
            get_condition_code(appid, &status);
            ok(rc == TWRC_SUCCESS, "Close DS Failed, rc %d, cc %d\n", rc, status.ConditionCode);
        }
    }

    if (winetest_interactive)
    {
        trace("Interactive, so trying userselect\n");
        memset(&source, 0, sizeof(source));
        rc = pDSM_Entry(appid, NULL, DG_CONTROL, DAT_IDENTITY, MSG_USERSELECT, &source);
        get_condition_code(appid, &status);
        ok(rc == TWRC_SUCCESS || rc == TWRC_CANCEL, "Userselect failed, rc %d, cc %d\n", rc, status.ConditionCode);
    }

}

START_TEST(dsm)
{
    TW_IDENTITY appid;
    TW_UINT16 rc;
    HANDLE hwnd;
    HMODULE htwain;

    if (!dsm_RegisterWindowClasses()) assert(0);

    htwain = LoadLibraryA("twain_32.dll");
    if (! htwain)
    {
        skip("twain_32.dll not available, skipping tests\n");
        return;
    }
    pDSM_Entry = (void*)GetProcAddress(htwain, "DSM_Entry");
    ok(pDSM_Entry != NULL, "Unable to GetProcAddress DSM_Entry\n");
    if (! pDSM_Entry)
    {
        skip("DSM_Entry not available, skipping tests\n");
        return;
    }

    memset(&appid, 0, sizeof(appid));
    appid.Version.Language = TWLG_ENGLISH_USA;
    appid.Version.Country = TWCY_USA;
    appid.ProtocolMajor = TWON_PROTOCOLMAJOR;
    appid.ProtocolMinor = TWON_PROTOCOLMINOR;
    appid.SupportedGroups = DG_CONTROL | DG_IMAGE;

    hwnd = CreateWindow("TWAIN_dsm_class", "Twain Test", 0,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                        NULL, NULL, GetModuleHandleA(0), NULL);

    rc = pDSM_Entry(&appid, NULL, DG_CONTROL, DAT_PARENT, MSG_OPENDSM, (TW_MEMREF) &hwnd);
    ok(rc == TWRC_SUCCESS, "MSG_OPENDSM returned %d", rc);

    test_sources(&appid);

    rc = pDSM_Entry(&appid, NULL, DG_CONTROL, DAT_PARENT, MSG_CLOSEDSM, (TW_MEMREF) &hwnd);
    ok(rc == TWRC_SUCCESS, "MSG_CLOSEDSM returned %d", rc);

    DestroyWindow(hwnd);
    FreeLibrary(htwain);
}