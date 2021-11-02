#include <string>
#include <vector>
#include <fstream>
#include <curl/curl.h>
#include <filesystem>
#include <cstdlib>
#include <iostream>
namespace fs = std::filesystem;
#include <boost/algorithm/string.hpp>
#include <qt5/QtCore/QObject>

namespace str = boost::algorithm;

static size_t my_fwrite2(void *buffer, size_t size, size_t nmemb, std::wofstream *stream)
{
    (*stream).write(static_cast<const wchar_t *>(buffer), size * nmemb);
    return size * nmemb;
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string *string)
{
    string->append(static_cast<char *>(ptr), size * nmemb);
    return size * nmemb;
}

void write_callback(void *data, size_t size, size_t nmemb, void *ptr)
{
    fwrite(data, size, nmemb, stdout);
}

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

fs::path get_config_path(std::wstring arg)
{
    auto path = fs::path(arg);
    path = path.parent_path();
    path /= "config.txt";
    std::wcout << L"path: " << path.wstring() << L"\n";

    if (fs::is_regular_file(path))
    {
        return path;
    }
    path = path.parent_path();
    path /= "config.example.txt";
    std::wcout << L"path: " << path.wstring() << L"\n";

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path /= "config";
    path /= "config.txt";
    std::wcout << L"path: " << path.wstring() << L"\n";

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path /= "config";
    path /= "config.example.txt";
    std::wcout << L"path: " << path.wstring() << L"\n";

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = path.parent_path().parent_path();
    path /= "config.txt";
    std::wcout << L"path: " << path.wstring() << L"\n";

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = path.parent_path();
    path /= "config.example.txt";
    std::wcout << L"path: " << path.wstring() << L"\n";

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path = path.parent_path();
    path /= "config";
    path /= "config.txt";
    std::wcout << L"path: " << path.wstring() << L"\n";

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path = path.parent_path();
    path /= "config";
    path /= "config.example.txt";
    std::wcout << L"path: " << path.wstring() << L"\n";

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path = path.parent_path().parent_path();
    path /= "config";
    path /= "config.txt";
    std::wcout << L"path: " << path.wstring() << L"\n";

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path = path.parent_path().parent_path();
    path /= "config";
    path /= "config.example.txt";
    std::wcout << L"path: " << path.wstring() << L"\n";

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path = path.parent_path().parent_path().parent_path();
    path /= "config";
    path /= "config.txt";
    std::wcout << L"path: " << path.wstring() << L"\n";

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path = path.parent_path().parent_path().parent_path();
    path /= "config";
    path /= "config.example.txt";
    std::wcout << L"path: " << path.wstring() << L"\n";

    if (fs::is_regular_file(path))
    {
        return path;
    }

    std::wcout << L"error: can't find config file\n";
    return fs::path("");
}

std::vector<std::wstring> get_path_ftp_dir_listing(std::wstring username, std::wstring passwd, std::wstring path)
{
    CURL *handle = curl_easy_init();
    // curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "NLST");
    curl_easy_setopt(handle, CURLOPT_DIRLISTONLY, 1L);

    curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(handle, CURLOPT_NOPROXY, "*");
    curl_easy_setopt(handle, CURLOPT_URL, WS2S(path).c_str());
    curl_easy_setopt(handle, CURLOPT_USERNAME, WS2S(username).c_str());
    curl_easy_setopt(handle, CURLOPT_PASSWORD, WS2S(passwd).c_str());

    // curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "NLST");
    curl_easy_setopt(handle, CURLOPT_DIRLISTONLY, 1L);

    // curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
    std::string ss = "";
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &ss);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writefunc);

    curl_easy_perform(handle);
    std::vector<std::string> v2;

    str::split(v2, ss, [](auto c)
               { return c == '\n'; });

    std::vector<std::wstring> dirnames;
    for (auto i : v2)
    {
        std::wstring trimed = str::trim_copy(S2WS(i));
        if (trimed != L"")
        {
            std::wcout << trimed << L"\n";
            dirnames.push_back(trimed);
        }
    }
    curl_easy_cleanup(handle);
    return dirnames;
}

int main()
{
    CURL *handle;
    /* global initialization */
    int rc = curl_global_init(CURL_GLOBAL_ALL);
    if (rc)
        return rc;

    /* initialization of easy handle */
    handle = curl_easy_init();
    if (!handle)
    {
        curl_global_cleanup();
        return CURLE_OUT_OF_MEMORY;
    }

    std::wstring current_path = fs::current_path().wstring();

    std::wcout << L"current path: " << current_path << L"\n";
    auto path = get_config_path(current_path);
    if (path.wstring() == L"")
    {
        return -1;
    }
    std::wcout << L"path: " << path.wstring() << L"\n";

    std::wifstream file(path);
    std::vector<std::wstring> data;
    std::wstring line;
    while (std::getline(file, line))
    {
        data.push_back(line);
    }
    auto path_str = data[0];
    std::vector<std::wstring> v;
    str::split(v, path_str, [](auto c)
               { return c == '='; });
    path_str = v.at(1);
    std::wcout << path_str << L"\n";

    if (!(path_str[1] == ':'))
    {
        auto cur = fs::path(current_path);
        cur /= path_str;
        path_str = cur.wstring();
    }
    std::wcout << L"out path: " << path_str << L"\n";

    data.erase(data.begin());

    auto src_path_str = data[0];

    std::vector<std::wstring> v_dest;
    str::split(v_dest, src_path_str, [](auto c)
               { return c == '='; });
    src_path_str = v_dest.at(1);
    std::wcout << L"src path dir: " << src_path_str << L"\n";

    data.erase(data.begin());

    for (auto line : data)
    {
        str::split(v, line, str::is_space());
        // for (auto i : v)
        // {
        //     fmt::print("{}\n", i);
        // }
        auto addr = v[1];
        auto dirname = v[0];
        auto path = fs::path(path_str);
        path /= dirname;
        if (!fs::is_directory(path))
        {
            fs::create_directories(path);
        }
        auto ftp_addr = L"ftp://" + addr + L"/" + src_path_str + L"/";

        std::vector<std::wstring> dirnames = get_path_ftp_dir_listing(v[2], v[3], ftp_addr);

        for (auto dirname : dirnames)
        {
            std::wcout << dirname << L"\n";
            // std::wstring testStr = UTF8ToGBK(dirname.c_str());
            // std::cout << dirname << std::endl;
            // continue;
            auto ftp_addr2 = L"ftp://" + addr + L"/" + src_path_str + L"/" + dirname + L"/";
            std::vector<std::wstring> filenames = get_path_ftp_dir_listing(v[2], v[3], ftp_addr2);

            for (auto filename : filenames)
            {
                fs::path out_path = fs::path(path_str);
                out_path /= dirname;
                if (!fs::is_directory(out_path))
                {
                    fs::create_directory(out_path);
                }
                out_path /= filename;

                std::wcout << "out file path: " << out_path.wstring() << L"\n";
                // const char *strFilename = out_path.wstring().c_str();

                // struct FtpFile ftpfile = {
                //     strFilename, /* name to store the file as if successful */
                //     NULL};
                std::vector<std::wstring> wvec2 = {
                    L"ftp:/",
                    addr,
                    src_path_str,
                    dirname,
                    filename};
                auto ftp_addr2 = str::join(wvec2, L"/");

                // "ftp://{}/{}/{}/", addr, src_path_str, dirname);
                curl_easy_reset(handle);
                curl_easy_setopt(handle, CURLOPT_URL, WS2S(ftp_addr2).c_str());
                auto username = v[2];
                curl_easy_setopt(handle, CURLOPT_USERNAME, WS2S(username).c_str());
                auto passwd = v[3];
                curl_easy_setopt(handle, CURLOPT_PASSWORD, WS2S(passwd).c_str());
                // curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "NLST");
                // curl_easy_setopt(handle, CURLOPT_DIRLISTONLY, 1L);
                // curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
                curl_easy_setopt(handle, CURLOPT_NOPROXY, "*");

                // curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writefunc);
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, my_fwrite2);
                /* Set a pointer to our struct to pass to the callback */

                std::wofstream file;
                // std::wstring testStr = UTF8ToGBK(out_path.wstring().c_str());
                file.open(out_path.wstring(), std::ios::out | std::ios::binary);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, &file);

                curl_easy_perform(handle);

                file.close();
            }
        }
    }

    curl_easy_cleanup(handle);

    // char temp;
    // std::cin >> temp;
    return 0;
}
