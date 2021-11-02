#include <filesystem>
#include <iostream>
#include <QObject>

namespace fs = std::filesystem;

std::wstring S2WS(std::string s)
{
    QString qs = QString::fromLocal8Bit(s.data());
    
    return qs.toStdWString();
}

std::string WS2S(std::wstring s)
{
    QString qs = QString::fromStdWString(s);
    return qs.toStdString();
}

#include <codecvt>
#include <locale>

std::wstring stringToWstring(const char *utf8Bytes)
{
    //setup converter
    using convert_type = std::codecvt_utf8<typename std::wstring::value_type>;
    std::wstring_convert<convert_type, typename std::wstring::value_type> converter;

    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
    return converter.from_bytes(utf8Bytes);
}

int main()
{
    std::wstring path = L"D:\\src\\test-qt-2\\build\\msvc\\Debug\\build/data\\dir1\\文件2.txt";
    std::wcout << path << L"\n";
    // std::string path2 = WS2S(path);
    // fs::path p = fs::path(path2);
    // std::cout << fs::file_size(p) << "\n";
    return 0;
}
