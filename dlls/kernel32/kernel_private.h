/*
 * Kernel32 undocumented and private functions definition
 *
 * Copyright 2003 Eric Pouech
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

#ifndef __WINE_KERNEL_PRIVATE_H
#define __WINE_KERNEL_PRIVATE_H

#include "wine/server.h"

struct tagSYSLEVEL;

struct kernel_thread_data
{
    UINT                code_page;      /* thread code page */
    WORD                stack_sel;      /* 16-bit stack selector */
    WORD                htask16;        /* Win16 task handle */
    DWORD               sys_count[4];   /* syslevel mutex entry counters */
    struct tagSYSLEVEL *sys_mutex[4];   /* syslevel mutex pointers */
    void               *pad[44];        /* change this if you add fields! */
};

static inline struct kernel_thread_data *kernel_get_thread_data(void)
{
    return (struct kernel_thread_data *)NtCurrentTeb()->SystemReserved1;
}

HANDLE  WINAPI OpenConsoleW(LPCWSTR, DWORD, BOOL, DWORD);
BOOL    WINAPI VerifyConsoleIoHandle(HANDLE);
HANDLE  WINAPI DuplicateConsoleHandle(HANDLE, DWORD, BOOL, DWORD);
BOOL    WINAPI CloseConsoleHandle(HANDLE handle);
HANDLE  WINAPI GetConsoleInputWaitHandle(void);

static inline BOOL is_console_handle(HANDLE h)
{
    return h != INVALID_HANDLE_VALUE && ((UINT_PTR)h & 3) == 3;
}

/* map a real wineserver handle onto a kernel32 console handle */
static inline HANDLE console_handle_map(HANDLE h)
{
    return h != INVALID_HANDLE_VALUE ? (HANDLE)((UINT_PTR)h ^ 3) : INVALID_HANDLE_VALUE;
}

/* map a kernel32 console handle onto a real wineserver handle */
static inline obj_handle_t console_handle_unmap(HANDLE h)
{
    return wine_server_obj_handle( h != INVALID_HANDLE_VALUE ? (HANDLE)((UINT_PTR)h ^ 3) : INVALID_HANDLE_VALUE );
}

extern HMODULE kernel32_handle;

/* Size of per-process table of DOS handles */
#define DOS_TABLE_SIZE 256
extern HANDLE dos_handles[DOS_TABLE_SIZE];

extern const WCHAR *DIR_Windows;
extern const WCHAR *DIR_System;
extern const WCHAR *DIR_SysWow64;

extern VOID SYSLEVEL_CheckNotLevel( INT level );

extern void FILE_SetDosError(void);
extern WCHAR *FILE_name_AtoW( LPCSTR name, BOOL alloc );
extern DWORD FILE_name_WtoA( LPCWSTR src, INT srclen, LPSTR dest, INT destlen );

extern DWORD __wine_emulate_instruction( EXCEPTION_RECORD *rec, CONTEXT86 *context );
extern LONG CALLBACK INSTR_vectored_handler( EXCEPTION_POINTERS *ptrs );

/* return values for MODULE_GetBinaryType */
#define BINARY_UNKNOWN    0x00
#define BINARY_PE         0x01
#define BINARY_WIN16      0x02
#define BINARY_OS216      0x03
#define BINARY_DOS        0x04
#define BINARY_UNIX_EXE   0x05
#define BINARY_UNIX_LIB   0x06
#define BINARY_TYPE_MASK  0x0f
#define BINARY_FLAG_DLL   0x10
#define BINARY_FLAG_64BIT 0x20

/* module.c */
extern WCHAR *MODULE_get_dll_load_path( LPCWSTR module );
extern DWORD MODULE_GetBinaryType( HANDLE hfile, void **res_start, void **res_end );

extern BOOL NLS_IsUnicodeOnlyLcid(LCID);

extern HANDLE VXD_Open( LPCWSTR filename, DWORD access, LPSECURITY_ATTRIBUTES sa );

extern WORD DOSMEM_0000H;
extern WORD DOSMEM_BiosDataSeg;
extern WORD DOSMEM_BiosSysSeg;

/* dosmem.c */
extern BOOL   DOSMEM_Init(void);
extern LPVOID DOSMEM_MapRealToLinear(DWORD); /* real-mode to linear */
extern LPVOID DOSMEM_MapDosToLinear(UINT);   /* linear DOS to Wine */
extern UINT   DOSMEM_MapLinearToDos(LPVOID); /* linear Wine to DOS */
extern BOOL   load_winedos(void);

/* environ.c */
extern void ENV_CopyStartupInformation(void);

/* computername.c */
extern void COMPUTERNAME_Init(void);

/* locale.c */
extern void LOCALE_Init(void);
extern void LOCALE_InitRegistry(void);

/* oldconfig.c */
extern void convert_old_config(void);

extern struct winedos_exports
{
    /* for global16.c */
    void*    (*AllocDosBlock)(UINT size, UINT16* pseg);
    BOOL     (*FreeDosBlock)(void* ptr);
    UINT     (*ResizeDosBlock)(void *ptr, UINT size, BOOL exact);
    /* for instr.c */
    BOOL (WINAPI *EmulateInterruptPM)( CONTEXT86 *context, BYTE intnum );
    void (WINAPI *CallBuiltinHandler)( CONTEXT86 *context, BYTE intnum );
    DWORD (WINAPI *inport)( int port, int size );
    void (WINAPI *outport)( int port, int size, DWORD val );
    void (* BiosTick)(WORD timer);
} winedos;

/* returns directory handle for named objects */
extern HANDLE get_BaseNamedObjects_handle(void);

/* Register functions */

#ifdef __i386__
#define DEFINE_REGS_ENTRYPOINT( name, args ) \
    __ASM_GLOBAL_FUNC( name, \
                       ".byte 0x68\n\t"  /* pushl $__regs_func */       \
                       ".long " __ASM_NAME("__regs_") #name "-.-11\n\t" \
                       ".byte 0x6a," #args "\n\t" /* pushl $args */     \
                       "call " __ASM_NAME("__wine_call_from_32_regs") "\n\t" \
                       "ret $(4*" #args ")" ) /* fake ret to make copy protections happy */
#endif

#endif
