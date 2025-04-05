#include "FileDialog.hpp"

std::wstring ShowSaveFileDialog(sf::WindowHandle parentHandle) {
    OPENFILENAMEW ofn = { 0 };
    wchar_t szFile[260] = { 0 };

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = parentHandle;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
    ofn.lpstrFilter = L"Файл сохранения (.dat)\0*.dat\0";
    ofn.lpstrDefExt = L"dat";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameW(&ofn) == TRUE) {
        std::wstring path(szFile);
        if (!std::filesystem::exists(path) && path.size() < 4 || path.substr(path.size() - 4) != L".dat") {
            path += L".dat";
        }
        return path;
    }
    return L"";
}

std::wstring ShowOpenFileDialog(sf::WindowHandle parentHandle) {
    OPENFILENAMEW ofn = { 0 };
    wchar_t szFile[260] = { 0 };

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = parentHandle;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
    ofn.lpstrFilter = L"Файл сохранения (.dat)\0*.dat*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrDefExt = L"dat";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameW(&ofn) == TRUE) {
        return szFile;
    }
    return L"";
}