#pragma once
#include "UltraEngine.h"
#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"

using namespace UltraEngine;

enum UserPathEx
{
    PATH_PROGRAMS // start menu programs path
};

extern bool CreateShortcut(const WString& path, const WString& dest, const WString& desc = "");
extern WString GetPath(const UserPathEx pathid);
