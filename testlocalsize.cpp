#include <filesystem>
#include <iostream>
#include <QObject>
#include <QFile>

namespace fs = std::filesystem;

std::wstring S2WS(std::string s)
{
    QString qs = QString::fromStdString(s);
    return qs.toStdWString();
}

std::string WS2S(std::wstring s)
{
    QString qs = QString::fromStdWString(s);
    return qs.toStdString();
}

int main()
{
    QString qs = QString::fromStdWString(
        L"D:\\src\\test-qt-2\\build\\msvc\\Debug/build\\data\\dir1\\文件2.txt");
    QFile qf(qs);
    std::cout << qf.size() << "\n";
    return 0;
}
