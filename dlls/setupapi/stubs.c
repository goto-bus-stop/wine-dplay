/*
 * SetupAPI stubs
 *
 * Copyright 2000 James Hatheway
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
 */

#include <stdarg.h>

#include "wine/debug.h"
#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "winreg.h"
#include "cfgmgr32.h"
#include "setupapi.h"
#include "winnls.h"

WINE_DEFAULT_DEBUG_CHANNEL(setupapi);

/***********************************************************************
 *		TPWriteProfileString (SETUPX.62)
 */
BOOL WINAPI TPWriteProfileString16( LPCSTR section, LPCSTR entry, LPCSTR string )
{
    FIXME( "%s %s %s: stub\n", debugstr_a(section), debugstr_a(entry), debugstr_a(string) );
    return TRUE;
}


/***********************************************************************
 *		suErrorToIds  (SETUPX.61)
 */
DWORD WINAPI suErrorToIds16( WORD w1, WORD w2 )
{
    FIXME( "%x %x: stub\n", w1, w2 );
    return 0;
}

/***********************************************************************
 *              CM_Connect_MachineA  (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Connect_MachineA(PCSTR name, PHMACHINE machine)
{
  FIXME("(%s %p) stub\n", name, machine);
  return CR_ACCESS_DENIED;
}

/***********************************************************************
 *		CM_Connect_MachineW  (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Connect_MachineW(PCWSTR name, PHMACHINE machine)
{
  FIXME("\n");
  return  CR_ACCESS_DENIED;
}

/***********************************************************************
 *              CM_Create_DevNodeA  (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Create_DevNodeA(PDEVINST pdnDevInst, DEVINSTID_A pDeviceID, DEVINST dnParent, ULONG ulFlags)
{
  FIXME("(%p %s 0x%08x 0x%08x) stub\n", pdnDevInst, pDeviceID, dnParent, ulFlags);
  return CR_SUCCESS;
}

/***********************************************************************
 *              CM_Create_DevNodeW  (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Create_DevNodeW(PDEVINST pdnDevInst, DEVINSTID_W pDeviceID, DEVINST dnParent, ULONG ulFlags)
{
  FIXME("(%p %s 0x%08x 0x%08x) stub\n", pdnDevInst, debugstr_w(pDeviceID), dnParent, ulFlags);
  return CR_SUCCESS;
}

/***********************************************************************
 *		CM_Disconnect_Machine  (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Disconnect_Machine(HMACHINE handle)
{
  FIXME("\n");
  return  CR_SUCCESS;

}

/***********************************************************************
 *             CM_Get_Device_ID_ListA  (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Get_Device_ID_ListA(
    PCSTR pszFilter, PCHAR Buffer, ULONG BufferLen, ULONG ulFlags )
{
    FIXME("%s %p %d 0x%08x\n", debugstr_a(pszFilter), Buffer, BufferLen, ulFlags);

    if (BufferLen >= 2) Buffer[0] = Buffer[1] = 0;
    return CR_SUCCESS;
}

/***********************************************************************
 *             CM_Get_Device_ID_ListW  (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Get_Device_ID_ListW(
    PCWSTR pszFilter, PWCHAR Buffer, ULONG BufferLen, ULONG ulFlags )
{
    FIXME("%s %p %d 0x%08x\n", debugstr_w(pszFilter), Buffer, BufferLen, ulFlags);

    if (BufferLen >= 2) Buffer[0] = Buffer[1] = 0;
    return CR_SUCCESS;
}

/***********************************************************************
 *              CM_Get_Parent (SETUPAPI.@)
 */
DWORD WINAPI CM_Get_Parent(PDEVINST pdnDevInst, DEVINST dnDevInst, ULONG ulFlags)
{
    FIXME("%p 0x%08x 0x%08x stub\n", pdnDevInst, dnDevInst, ulFlags);
    *pdnDevInst = dnDevInst;
    return CR_SUCCESS;
}

/***********************************************************************
 *		SetupInitializeFileLogW(SETUPAPI.@)
 */
HSPFILELOG WINAPI SetupInitializeFileLogW(LPCWSTR LogFileName, DWORD Flags)
{
    FIXME("Stub %s, 0x%x\n",debugstr_w(LogFileName),Flags);
    return INVALID_HANDLE_VALUE;
}

/***********************************************************************
 *		SetupInitializeFileLogA(SETUPAPI.@)
 */
HSPFILELOG WINAPI SetupInitializeFileLogA(LPCSTR LogFileName, DWORD Flags)
{
    FIXME("Stub %s, 0x%x\n",debugstr_a(LogFileName),Flags);
    return INVALID_HANDLE_VALUE;
}

/***********************************************************************
 *		SetupTerminateFileLog(SETUPAPI.@)
 */
BOOL WINAPI SetupTerminateFileLog(HANDLE FileLogHandle)
{
    FIXME ("Stub %p\n",FileLogHandle);
    return TRUE;
}

/***********************************************************************
 *		RegistryDelnode(SETUPAPI.@)
 */
BOOL WINAPI RegistryDelnode(DWORD x, DWORD y)
{
    FIXME("%08x %08x: stub\n", x, y);
    return FALSE;
}

/***********************************************************************
 *      SetupCloseLog(SETUPAPI.@)
 */
void WINAPI SetupCloseLog(void)
{
    FIXME("() stub\n");
}

/***********************************************************************
 *      SetupLogErrorW(SETUPAPI.@)
 */
BOOL WINAPI SetupLogErrorW(LPCWSTR MessageString, LogSeverity Severity)
{
    FIXME("(%s, %d) stub\n", debugstr_w(MessageString), Severity);
    return TRUE;
}

/***********************************************************************
 *      SetupOpenLog(SETUPAPI.@)
 */
BOOL WINAPI SetupOpenLog(BOOL Reserved)
{
    FIXME("(%d) stub\n", Reserved);
    return TRUE;
}

/***********************************************************************
 *      SetupPromptReboot(SETUPAPI.@)
 */
INT WINAPI SetupPromptReboot( HSPFILEQ file_queue, HWND owner, BOOL scan_only )
{
    FIXME("%p, %p, %d\n", file_queue, owner, scan_only);
    return 0;
}

/***********************************************************************
 *      SetupSetSourceListA (SETUPAPI.@)
 */
BOOL WINAPI SetupSetSourceListA(DWORD flags, PCSTR *list, UINT count)
{
    FIXME("0x%08x %p %d\n", flags, list, count);
    return FALSE;
}

/***********************************************************************
 *      SetupSetSourceListW (SETUPAPI.@)
 */
BOOL WINAPI SetupSetSourceListW(DWORD flags, PCWSTR *list, UINT count)
{
    FIXME("0x%08x %p %d\n", flags, list, count);
    return FALSE;
}

/***********************************************************************
 *      SetupDiGetINFClassA (SETUPAPI.@)
 */
BOOL WINAPI SetupDiGetINFClassA(PCSTR inf, LPGUID class_guid, PSTR class_name,
        DWORD size, PDWORD required_size)
{
    FIXME("%s %p %p %d %p\n", debugstr_a(inf), class_guid, class_name, size, required_size);
    return FALSE;
}

/***********************************************************************
 *      SetupDiGetINFClassW (SETUPAPI.@)
 */
BOOL WINAPI SetupDiGetINFClassW(PCWSTR inf, LPGUID class_guid, PWSTR class_name,
        DWORD size, PDWORD required_size)
{
    FIXME("%s %p %p %d %p\n", debugstr_w(inf), class_guid, class_name, size, required_size);
    return FALSE;
}

/***********************************************************************
 *      SetupDiDestroyClassImageList (SETUPAPI.@)
 */
BOOL WINAPI SetupDiDestroyClassImageList(PSP_CLASSIMAGELIST_DATA ClassListImageData)
{
    FIXME("(%p) stub\n", ClassListImageData);
    return TRUE;
}

/***********************************************************************
 *      SetupDiGetClassImageList (SETUPAPI.@)
 */
BOOL WINAPI SetupDiGetClassImageList(PSP_CLASSIMAGELIST_DATA ClassImageListData)
{
    FIXME("(%p) stub\n", ClassImageListData);
    return FALSE;
}

/***********************************************************************
 *      SetupDiGetClassImageList (SETUPAPI.@)
 */
BOOL WINAPI SetupDiGetClassImageIndex(PSP_CLASSIMAGELIST_DATA ClassImageListData, const GUID *class, PINT index)
{
    FIXME("%p %p %p\n", ClassImageListData, class, index);
    return FALSE;
}

/***********************************************************************
 *      CM_Locate_DevNodeA (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Locate_DevNodeA(PDEVINST pdnDevInst, DEVINSTID_A pDeviceID, ULONG ulFlags)
{
    FIXME("%p %s 0x%08x: stub\n", pdnDevInst, debugstr_a(pDeviceID), ulFlags);

    return CR_FAILURE;
}

/***********************************************************************
 *      CM_Locate_DevNodeW (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Locate_DevNodeW(PDEVINST pdnDevInst, DEVINSTID_W pDeviceID, ULONG ulFlags)
{
    FIXME("%p %s 0x%08x: stub\n", pdnDevInst, debugstr_w(pDeviceID), ulFlags);

    return CR_FAILURE;
}

/***********************************************************************
 *      CM_Locate_DevNode_ExA (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Locate_DevNode_ExA(PDEVINST pdnDevInst, DEVINSTID_A pDeviceID, ULONG ulFlags, HMACHINE hMachine)
{
    FIXME("%p %s 0x%08x %p: stub\n", pdnDevInst, debugstr_a(pDeviceID), ulFlags, hMachine);

    return CR_FAILURE;
}

/***********************************************************************
 *      CM_Locate_DevNode_ExW (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Locate_DevNode_ExW(PDEVINST pdnDevInst, DEVINSTID_W pDeviceID, ULONG ulFlags, HMACHINE hMachine)
{
    FIXME("%p %s 0x%08x %p: stub\n", pdnDevInst, debugstr_w(pDeviceID), ulFlags, hMachine);

    return CR_FAILURE;
}

/***********************************************************************
 *      CM_Get_Device_Interface_List_Size_ExA (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Get_Device_Interface_List_Size_ExA(PULONG len, LPGUID class, DEVINSTID_A id,
                                                       ULONG flags, HMACHINE machine)
{
    FIXME("%p %p %s 0x%08x %p: stub\n", len, class, debugstr_a(id), flags, machine);
    return CR_FAILURE;
}

/***********************************************************************
 *      CM_Get_Device_Interface_List_Size_ExW (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Get_Device_Interface_List_Size_ExW(PULONG len, LPGUID class, DEVINSTID_W id,
                                                       ULONG flags, HMACHINE machine)
{
    FIXME("%p %p %s 0x%08x %p: stub\n", len, class, debugstr_w(id), flags, machine);
    return CR_FAILURE;
}

/***********************************************************************
 *      CM_Enumerate_Classes (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Enumerate_Classes(ULONG index, LPGUID class, ULONG flags)
{
    FIXME("%u %p 0x%08x: stub\n", index, class, flags);
    return CR_NO_SUCH_VALUE;
}
