#pragma check_stack(off)
#pragma strict_gs_check(off)
#pragma runtime_checks("", off)

#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#pragma comment(linker, "/STACK:1024,1024")
#pragma comment(linker, "/HEAP:0,0")
#pragma comment(linker, "/ALIGN:256")
#pragma comment(linker, "/NODEFAULTLIB")
#pragma comment(linker, "/ENTRY:mainCRTStartup")
#pragma comment(linker, "/INCLUDE:CreateProcessW")
#pragma comment(linker, "/INCLUDE:ExitProcess")
#pragma comment(linker, "/INCLUDE:MessageBoxW")
#pragma comment(linker, "/INCLUDE:FreeConsole")
#pragma comment(linker, "/INCREMENTAL:NO") // Needed to ensure debuggability in default debug configuration
#pragma comment(linker, "/MERGE:.pdata=.rdata")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma section(".shared", read, write)
__declspec(allocate(".shared") selectany) unsigned char _[0x80] = { 0x00 };

static __forceinline const void* NtCurrentImage(void) // cheese and rice it's jason bourne
{
    return (unsigned short*)*(unsigned char**)((unsigned short*)(*(unsigned char**)((unsigned char*)__readgsqword(0x60) + 0x20) + 0x60) + 0x04);
}

static __forceinline unsigned short NtStartupReserved2(void)
{
    return *(unsigned short*)((unsigned char*)NtCurrentImage() - 0x05A2);
}

void __stdcall mainCRTStartup()
{
    if (0x00 == NtStartupReserved2())
    {
        ExitProcess((_[0x03] = 0x68, *(unsigned short*)&_[0x3E] = 0x6969,
            !!CreateProcessW(NtCurrentImage(), 0, 0, 0, 0, 0x08000200, 0, 0, &_[0x00], &_[0x68])));
    }
    else
    {
        // Verification of expected behavior would assert that (0x6969 == NtStartupReserved2()) here 
        FreeConsole(), MessageBoxW(0, L"Yay, you re-executed yourself!", NtCurrentImage(), 0);
    }
}
