#include "FileDialog.h"
#include <Windows.h>
#include <iostream>
#include <commdlg.h>
#include <shobjidl.h> 
#include <cstdlib>
using namespace std;

bool FileDialog::Open(char& out_char)
{
    // Initialize COM Library
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    IFileOpenDialog* pFileOpen;

    // Create the FileOpenDialog object
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

    if (SUCCEEDED(hr)) {
        // Show the Open dialog box
        hr = pFileOpen->Show(NULL);

        // Get the file name from the dialog box
        if (SUCCEEDED(hr)) {
            IShellItem* pItem;
            hr = pFileOpen->GetResult(&pItem);
            if (SUCCEEDED(hr)) {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                // pszFilePath now contains the selected file path
                // ...
                //char narrowString[MAX_PATH];
                std::wcstombs(&out_char, pszFilePath, MAX_PATH);
                //std::wcstombs(&out_path, pszFilePath, MAX_PATH);
                cout << "OPEN PATH " << out_char << endl;

                CoTaskMemFree(pszFilePath);
                pItem->Release();

                //return out_char;
                return true;
            }
        }
        pFileOpen->Release();
    }
    CoUninitialize();

    return false;

	//return nullptr;
}