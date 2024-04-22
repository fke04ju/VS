#include "pch.h"
#include "frmMain.h"

using namespace System;
using namespace Excel2JSON;
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
    FreeConsole();
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew frmMain());
    
    return 0;
}
