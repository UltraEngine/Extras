#include "UltraEngine.h"
#include "Win32System.h"

// https://learn.microsoft.com/en-au/windows/win32/shell/links?redirectedfrom=MSDN#creating-a-shortcut-and-a-folder-shortcut-to-a-file
bool CreateShortcut(const WString& path, const WString& dest, const WString& desc)
{
    auto rpath = RealPath(path).Replace("/", "\\");
    auto rdest = RealPath(dest).Replace("/", "\\");

    String s1 = rpath.ToUtf8String();
    String s2 = desc.ToUtf8String();
    String s3 = ExtractDir(rpath).Replace("/", "\\").ToUtf8String();

    LPCSTR lpszPathObj = s1.c_str();
    LPCWSTR lpszPathLink = rdest.c_str();
    LPCSTR lpszDesc = s2.c_str();

    CoInitialize(NULL);
    HRESULT hres;
    IShellLink* psl;
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
    if (not SUCCEEDED(hres)) return false;
    IPersistFile* ppf;
    psl->SetPath(lpszPathObj);
    psl->SetWorkingDirectory(s3.c_str());
    psl->SetDescription(lpszDesc);
    hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
    if (SUCCEEDED(hres))
    {
        hres = ppf->Save(lpszPathLink, TRUE);
        ppf->Release();
    }
    psl->Release();
    return hres == S_OK;
}

WString GetPath(const UserPathEx pathid)
{
    if (pathid == PATH_PROGRAMS)
    {
        PIDLIST_ABSOLUTE pidlist;
        wchar_t path[MAX_PATH];
        HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &pidlist);
        BOOL b = SHGetPathFromIDListW(pidlist, path);
        return WString(path);
    }
    return L"";
}
