/*
 * Copyright 2008 Jacek Caban for CodeWeavers
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

#define COBJMACROS

#include "windef.h"
#include "winbase.h"
#include "winuser.h"
#include "winreg.h"
#include "ole2.h"
#include "wininet.h"
#include "shlwapi.h"

#include "wine/debug.h"

#include "mshtml_private.h"
#include "resource.h"

WINE_DEFAULT_DEBUG_CHANNEL(mshtml);

#define HTMLLOCATION_THIS(iface) DEFINE_THIS(HTMLLocation, HTMLLocation, iface)

static HRESULT WINAPI HTMLLocation_QueryInterface(IHTMLLocation *iface, REFIID riid, void **ppv)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);

    *ppv = NULL;

    if(IsEqualGUID(&IID_IUnknown, riid)) {
        TRACE("(%p)->(IID_IUnknown %p)\n", This, ppv);
        *ppv = HTMLLOCATION(This);
    }else if(IsEqualGUID(&IID_IHTMLLocation, riid)) {
        TRACE("(%p)->(IID_IHTMLLocation %p)\n", This, ppv);
        *ppv = HTMLLOCATION(This);
    }else if(dispex_query_interface(&This->dispex, riid, ppv)) {
        return *ppv ? S_OK : E_NOINTERFACE;
    }

    if(*ppv) {
        IUnknown_AddRef((IUnknown*)*ppv);
        return S_OK;
    }

    WARN("(%p)->(%s %p)\n", This, debugstr_guid(riid), ppv);
    return E_NOINTERFACE;
}

static ULONG WINAPI HTMLLocation_AddRef(IHTMLLocation *iface)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    LONG ref = InterlockedIncrement(&This->ref);

    TRACE("(%p) ref=%d\n", This, ref);

    return ref;
}

static ULONG WINAPI HTMLLocation_Release(IHTMLLocation *iface)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    LONG ref = InterlockedDecrement(&This->ref);

    TRACE("(%p) ref=%d\n", This, ref);

    if(!ref) {
        if(This->doc && This->doc->location == This)
            This->doc->location = NULL;
        release_dispex(&This->dispex);
        heap_free(This);
    }

    return ref;
}

static HRESULT WINAPI HTMLLocation_GetTypeInfoCount(IHTMLLocation *iface, UINT *pctinfo)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    return IDispatchEx_GetTypeInfoCount(DISPATCHEX(&This->dispex), pctinfo);
}

static HRESULT WINAPI HTMLLocation_GetTypeInfo(IHTMLLocation *iface, UINT iTInfo,
                                              LCID lcid, ITypeInfo **ppTInfo)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    return IDispatchEx_GetTypeInfo(DISPATCHEX(&This->dispex), iTInfo, lcid, ppTInfo);
}

static HRESULT WINAPI HTMLLocation_GetIDsOfNames(IHTMLLocation *iface, REFIID riid,
                                                LPOLESTR *rgszNames, UINT cNames,
                                                LCID lcid, DISPID *rgDispId)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    return IDispatchEx_GetIDsOfNames(DISPATCHEX(&This->dispex), riid, rgszNames, cNames, lcid, rgDispId);
}

static HRESULT WINAPI HTMLLocation_Invoke(IHTMLLocation *iface, DISPID dispIdMember,
                            REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams,
                            VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    return IDispatchEx_Invoke(DISPATCHEX(&This->dispex), dispIdMember, riid, lcid,
            wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}

static HRESULT WINAPI HTMLLocation_put_href(IHTMLLocation *iface, BSTR v)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%s)\n", This, debugstr_w(v));
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_get_href(IHTMLLocation *iface, BSTR *p)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);

    TRACE("(%p)->(%p)\n", This, p);

    if(!p)
        return E_POINTER;

    if(!This->doc || !This->doc->url) {
        FIXME("No current URL\n");
        return E_NOTIMPL;
    }

    *p = SysAllocString(This->doc->url);

    return S_OK;
}

static HRESULT WINAPI HTMLLocation_put_protocol(IHTMLLocation *iface, BSTR v)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%s)\n", This, debugstr_w(v));
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_get_protocol(IHTMLLocation *iface, BSTR *p)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%p)\n", This, p);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_put_host(IHTMLLocation *iface, BSTR v)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%s)\n", This, debugstr_w(v));
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_get_host(IHTMLLocation *iface, BSTR *p)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%p)\n", This, p);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_put_hostname(IHTMLLocation *iface, BSTR v)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%s)\n", This, debugstr_w(v));
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_get_hostname(IHTMLLocation *iface, BSTR *p)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%p)\n", This, p);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_put_port(IHTMLLocation *iface, BSTR v)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%s)\n", This, debugstr_w(v));
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_get_port(IHTMLLocation *iface, BSTR *p)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%p)\n", This, p);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_put_pathname(IHTMLLocation *iface, BSTR v)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%s)\n", This, debugstr_w(v));
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_get_pathname(IHTMLLocation *iface, BSTR *p)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    WCHAR buf[INTERNET_MAX_PATH_LENGTH];
    URL_COMPONENTSW url = {sizeof(url)};
    DWORD size = 0;
    HRESULT hres;

    TRACE("(%p)->(%p)\n", This, p);

    if(!This->doc || !This->doc->url) {
        FIXME("No current URL\n");
        return E_NOTIMPL;
    }

    hres = CoInternetParseUrl(This->doc->url, PARSE_PATH_FROM_URL, 0, buf, sizeof(buf), &size, 0);
    if(SUCCEEDED(hres)) {
        *p = SysAllocString(buf);
        if(!*p)
            return E_OUTOFMEMORY;
        return S_OK;
    }

    url.dwUrlPathLength = 1;
    if(!InternetCrackUrlW(This->doc->url, 0, 0, &url)) {
        FIXME("InternetCrackUrl failed\n");
        return E_FAIL;
    }

    if(!url.dwUrlPathLength) {
        *p = NULL;
        return S_OK;
    }

    *p = SysAllocStringLen(url.lpszUrlPath, url.dwUrlPathLength);
    if(!*p)
        return E_OUTOFMEMORY;
    return S_OK;
}

static HRESULT WINAPI HTMLLocation_put_search(IHTMLLocation *iface, BSTR v)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%s)\n", This, debugstr_w(v));
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_get_search(IHTMLLocation *iface, BSTR *p)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%p)\n", This, p);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_put_hash(IHTMLLocation *iface, BSTR v)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%s)\n", This, debugstr_w(v));
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_get_hash(IHTMLLocation *iface, BSTR *p)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%p)\n", This, p);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_reload(IHTMLLocation *iface, VARIANT_BOOL flag)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%x)\n", This, flag);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_replace(IHTMLLocation *iface, BSTR bstr)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%s)\n", This, debugstr_w(bstr));
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_assign(IHTMLLocation *iface, BSTR bstr)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%s)\n", This, debugstr_w(bstr));
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLLocation_toString(IHTMLLocation *iface, BSTR *String)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    FIXME("(%p)->(%p)\n", This, String);
    return E_NOTIMPL;
}

static HRESULT HTMLLocation_value(IUnknown *iface, LCID lcid, WORD flags, DISPPARAMS *params,
        VARIANT *res, EXCEPINFO *ei, IServiceProvider *caller)
{
    HTMLLocation *This = HTMLLOCATION_THIS(iface);
    HRESULT hres;

    TRACE("(%p)\n", This);

    switch(flags) {
    case DISPATCH_PROPERTYGET: {
        BSTR str;

        hres = IHTMLLocation_get_href(HTMLLOCATION(This), &str);
        if(FAILED(hres))
            return hres;

        V_VT(res) = VT_BSTR;
        V_BSTR(res) = str;
        break;
    }
    default:
        FIXME("unimplemented flags %x\n", flags);
        return E_NOTIMPL;
    }

    return S_OK;
}

#undef HTMLLOCATION_THIS

static const IHTMLLocationVtbl HTMLLocationVtbl = {
    HTMLLocation_QueryInterface,
    HTMLLocation_AddRef,
    HTMLLocation_Release,
    HTMLLocation_GetTypeInfoCount,
    HTMLLocation_GetTypeInfo,
    HTMLLocation_GetIDsOfNames,
    HTMLLocation_Invoke,
    HTMLLocation_put_href,
    HTMLLocation_get_href,
    HTMLLocation_put_protocol,
    HTMLLocation_get_protocol,
    HTMLLocation_put_host,
    HTMLLocation_get_host,
    HTMLLocation_put_hostname,
    HTMLLocation_get_hostname,
    HTMLLocation_put_port,
    HTMLLocation_get_port,
    HTMLLocation_put_pathname,
    HTMLLocation_get_pathname,
    HTMLLocation_put_search,
    HTMLLocation_get_search,
    HTMLLocation_put_hash,
    HTMLLocation_get_hash,
    HTMLLocation_reload,
    HTMLLocation_replace,
    HTMLLocation_assign,
    HTMLLocation_toString
};

static const dispex_static_data_vtbl_t HTMLLocation_dispex_vtbl = {
    HTMLLocation_value,
    NULL,
    NULL
};

static const tid_t HTMLLocation_iface_tids[] = {
    IHTMLLocation_tid,
    0
};
static dispex_static_data_t HTMLLocation_dispex = {
    &HTMLLocation_dispex_vtbl,
    DispHTMLLocation_tid,
    NULL,
    HTMLLocation_iface_tids
};


HTMLLocation *HTMLLocation_Create(HTMLDocument *doc)
{
    HTMLLocation *ret = heap_alloc(sizeof(*ret));

    ret->lpHTMLLocationVtbl = &HTMLLocationVtbl;
    ret->ref = 1;
    ret->doc = doc;

    init_dispex(&ret->dispex, (IUnknown*)HTMLLOCATION(ret),  &HTMLLocation_dispex);

    return ret;
}
