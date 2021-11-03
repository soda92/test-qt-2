#include <string>
#include <vector>
#include <fstream>
#include <curl/curl.h>
#include <filesystem>
#include <cstdlib>
#include <iostream>
namespace fs = std::filesystem;
#include <boost/algorithm/string.hpp>
#include <QObject>

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

    // curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(handle, CURLOPT_NOPROXY, "*");
    curl_easy_setopt(handle, CURLOPT_URL, WS2S(path).c_str());
    curl_easy_setopt(handle, CURLOPT_USERNAME, WS2S(username).c_str());
    curl_easy_setopt(handle, CURLOPT_PASSWORD, WS2S(passwd).c_str());

    // curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "NLST");
    curl_easy_setopt(handle, CURLOPT_DIRLISTONLY, 1L);

    // curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
    std::string ss("");
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &ss);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writefunc);

    curl_easy_perform(handle);
    std::vector<std::string> v2;

    // QString qs = QString::fromStdWString(ss);
    // std::string qss = qs.toStdString();
    // std::cout << qss << std::endl;

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

static size_t throw_away(void *ptr, size_t size, size_t nmemb, void *data)
{
    (void)ptr;
    (void)data;
    /* we are not interested in the headers itself,
     so we only return the size we would have saved ... */
    return (size_t)(size * nmemb);
}

double get_file_size(std::wstring file_path, std::wstring username, std::wstring passwd)
{
    std::string path2 = WS2S(file_path);
    str::replace_all(path2, "\\", "/");
    const char *ftpurl = path2.c_str();
    CURL *curl;
    CURLcode res;
    long filetime = -1;
    double filesize = 0.0;
    const char *filename = strrchr(ftpurl, '/') + 1;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, path2.c_str());
        /* No download if the file */
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_NOPROXY, "*");
        curl_easy_setopt(curl, CURLOPT_URL, WS2S(file_path).c_str());
        curl_easy_setopt(curl, CURLOPT_USERNAME, WS2S(username).c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, WS2S(passwd).c_str());
        /* Ask for filetime */
        // curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, throw_away);
        curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
        /* Switch on full protocol/debug output */
        /* curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); */

        res = curl_easy_perform(curl);

        if (CURLE_OK == res)
        {
            /* https://curl.se/libcurl/c/curl_easy_getinfo.html */
            // res = curl_easy_getinfo(curl, CURLINFO_FILETIME, &filetime);
            // if ((CURLE_OK == res) && (filetime >= 0))
            // {
            // time_t file_time = (time_t)filetime;
            // printf("filetime %s: %s", filename, ctime(&file_time));
            // }
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD,
                                    &filesize);
            if ((CURLE_OK == res) && (filesize > 0.0))
                return filesize;
        }
        else
        {
            /* we failed */
            fprintf(stderr, "curl told us %d\n", res);
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    return 0;
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
        std::vector<std::wstring> vec_path;
        str::split(vec_path, path_str, [](auto c)
                   { return c == '/'; });
        auto cur = fs::path(current_path);
        for (auto folder : vec_path)
        {
            cur /= folder;
        }
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
        auto dirname = v[0];
        auto addr = v[1];
        auto path = fs::path(path_str);
        path /= dirname;
        if (!fs::is_directory(path))
        {
            // fs::create_directories(path);
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
                    fs::create_directories(out_path);
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

                if (fs::is_regular_file(out_path))
                {
                    std::cout << out_path.string() << " local size: " << fs::file_size(out_path) << "\n ";
                }
                double remote_size = get_file_size(ftp_addr2, v[2], v[3]);
                std::cout << WS2S(ftp_addr2) << " remote size: " << remote_size << "\n ";

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

                std::ofstream file;

                // std::wstring testStr = UTF8ToGBK(out_path.wstring().c_str());
                file.open(out_path.string(), std::ios::out | std::ios::binary);
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
