/*
 * Copyright 2009 Tony Wasserka
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

#include "wine/debug.h"

#define COBJMACROS
#include "windef.h"
#include "winbase.h"
#include "winreg.h"
#include "objbase.h"
#include "wincodec.h"
#include "wincodecs_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(wincodecs);

/******************************************
 * IWICStream implementation
 *
 */
typedef struct IWICStreamImpl
{
    const IWICStreamVtbl *lpVtbl;
    LONG ref;

    IStream *pStream;
} IWICStreamImpl;

static HRESULT WINAPI IWICStreamImpl_QueryInterface(IWICStream *iface,
    REFIID iid, void **ppv)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    TRACE("(%p,%s,%p)\n", iface, debugstr_guid(iid), ppv);

    if (!ppv) return E_INVALIDARG;

    if (IsEqualIID(&IID_IUnknown, iid) || IsEqualIID(&IID_IStream, iid) ||
        IsEqualIID(&IID_ISequentialStream, iid) || IsEqualIID(&IID_IWICStream, iid))
    {
        *ppv = This;
        IUnknown_AddRef((IUnknown*)*ppv);
        return S_OK;
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
}

static ULONG WINAPI IWICStreamImpl_AddRef(IWICStream *iface)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    ULONG ref = InterlockedIncrement(&This->ref);

    TRACE("(%p) refcount=%u\n", iface, ref);

    return ref;
}

static ULONG WINAPI IWICStreamImpl_Release(IWICStream *iface)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    ULONG ref = InterlockedDecrement(&This->ref);

    TRACE("(%p) refcount=%u\n", iface, ref);

    if (ref == 0) {
        if (This->pStream) IStream_Release(This->pStream);
        HeapFree(GetProcessHeap(), 0, This);
    }
    return ref;
}

static HRESULT WINAPI IWICStreamImpl_Read(IWICStream *iface,
    void *pv, ULONG cb, ULONG *pcbRead)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    TRACE("(%p): relay\n", This);

    if (!This->pStream) return WINCODEC_ERR_NOTINITIALIZED;
    return IStream_Read(This->pStream, pv, cb, pcbRead);
}

static HRESULT WINAPI IWICStreamImpl_Write(IWICStream *iface,
    void const *pv, ULONG cb, ULONG *pcbWritten)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    TRACE("(%p): relay\n", This);

    if (!This->pStream) return WINCODEC_ERR_NOTINITIALIZED;
    return IStream_Write(This->pStream, pv, cb, pcbWritten);
}

static HRESULT WINAPI IWICStreamImpl_Seek(IWICStream *iface,
    LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    TRACE("(%p): relay\n", This);

    if (!This->pStream) return WINCODEC_ERR_NOTINITIALIZED;
    return IStream_Seek(This->pStream, dlibMove, dwOrigin, plibNewPosition);
}

static HRESULT WINAPI IWICStreamImpl_SetSize(IWICStream *iface,
    ULARGE_INTEGER libNewSize)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    TRACE("(%p): relay\n", This);

    if (!This->pStream) return WINCODEC_ERR_NOTINITIALIZED;
    return IStream_SetSize(This->pStream, libNewSize);
}

static HRESULT WINAPI IWICStreamImpl_CopyTo(IWICStream *iface,
    IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    TRACE("(%p): relay\n", This);

    if (!This->pStream) return WINCODEC_ERR_NOTINITIALIZED;
    return IStream_CopyTo(This->pStream, pstm, cb, pcbRead, pcbWritten);
}

static HRESULT WINAPI IWICStreamImpl_Commit(IWICStream *iface,
    DWORD grfCommitFlags)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    TRACE("(%p): relay\n", This);

    if (!This->pStream) return WINCODEC_ERR_NOTINITIALIZED;
    return IStream_Commit(This->pStream, grfCommitFlags);
}

static HRESULT WINAPI IWICStreamImpl_Revert(IWICStream *iface)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    TRACE("(%p): relay\n", This);

    if (!This->pStream) return WINCODEC_ERR_NOTINITIALIZED;
    return IStream_Revert(This->pStream);
}

static HRESULT WINAPI IWICStreamImpl_LockRegion(IWICStream *iface,
    ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    TRACE("(%p): relay\n", This);

    if (!This->pStream) return WINCODEC_ERR_NOTINITIALIZED;
    return IStream_LockRegion(This->pStream, libOffset, cb, dwLockType);
}

static HRESULT WINAPI IWICStreamImpl_UnlockRegion(IWICStream *iface,
    ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    TRACE("(%p): relay\n", This);

    if (!This->pStream) return WINCODEC_ERR_NOTINITIALIZED;
    return IStream_UnlockRegion(This->pStream, libOffset, cb, dwLockType);
}

static HRESULT WINAPI IWICStreamImpl_Stat(IWICStream *iface,
    STATSTG *pstatstg, DWORD grfStatFlag)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    TRACE("(%p): relay\n", This);

    if (!This->pStream) return WINCODEC_ERR_NOTINITIALIZED;
    return IStream_Stat(This->pStream, pstatstg, grfStatFlag);
}

static HRESULT WINAPI IWICStreamImpl_Clone(IWICStream *iface,
    IStream **ppstm)
{
    IWICStreamImpl *This = (IWICStreamImpl*)iface;
    TRACE("(%p): relay\n", This);

    if (!This->pStream) return WINCODEC_ERR_NOTINITIALIZED;
    return IStream_Clone(This->pStream, ppstm);
}

static HRESULT WINAPI IWICStreamImpl_InitializeFromIStream(IWICStream *iface,
    IStream *pIStream)
{
    FIXME("(%p): stub\n", iface);
    return E_NOTIMPL;
}

static HRESULT WINAPI IWICStreamImpl_InitializeFromFilename(IWICStream *iface,
    LPCWSTR wzFileName, DWORD dwDesiredAccess)
{
    FIXME("(%p): stub\n", iface);
    return E_NOTIMPL;
}

static HRESULT WINAPI IWICStreamImpl_InitializeFromMemory(IWICStream *iface,
    BYTE *pbBuffer, DWORD cbBufferSize)
{
    FIXME("(%p): stub\n", iface);
    return E_NOTIMPL;
}

static HRESULT WINAPI IWICStreamImpl_InitializeFromIStreamRegion(IWICStream *iface,
    IStream *pIStream, ULARGE_INTEGER ulOffset, ULARGE_INTEGER ulMaxSize)
{
    FIXME("(%p): stub\n", iface);
    return E_NOTIMPL;
}


const IWICStreamVtbl WICStream_Vtbl =
{
    /*** IUnknown methods ***/
    IWICStreamImpl_QueryInterface,
    IWICStreamImpl_AddRef,
    IWICStreamImpl_Release,
    /*** ISequentialStream methods ***/
    IWICStreamImpl_Read,
    IWICStreamImpl_Write,
    /*** IStream methods ***/
    IWICStreamImpl_Seek,
    IWICStreamImpl_SetSize,
    IWICStreamImpl_CopyTo,
    IWICStreamImpl_Commit,
    IWICStreamImpl_Revert,
    IWICStreamImpl_LockRegion,
    IWICStreamImpl_UnlockRegion,
    IWICStreamImpl_Stat,
    IWICStreamImpl_Clone,
    /*** IWICStream methods ***/
    IWICStreamImpl_InitializeFromIStream,
    IWICStreamImpl_InitializeFromFilename,
    IWICStreamImpl_InitializeFromMemory,
    IWICStreamImpl_InitializeFromIStreamRegion,
};

HRESULT StreamImpl_Create(IWICStream **stream)
{
    IWICStreamImpl *pObject;

    if( !stream ) return E_INVALIDARG;

    pObject = HeapAlloc(GetProcessHeap(), 0, sizeof(IWICStreamImpl));
    if( !pObject ) {
        *stream = NULL;
        return E_OUTOFMEMORY;
    }

    pObject->lpVtbl = &WICStream_Vtbl;
    pObject->ref = 1;
    pObject->pStream = NULL;

    *stream = (IWICStream*)pObject;

    return S_OK;
}