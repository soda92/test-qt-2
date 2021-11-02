#include <string>
#include <vector>
#include <fstream>
#include <fmt/core.h>
#include <curl/curl.h>
#include <filesystem>
#include <cstdlib>
#include <iostream>
namespace fs = std::filesystem;
#include <boost/algorithm/string.hpp>

namespace str = boost::algorithm;

static size_t my_fwrite2(void *buffer, size_t size, size_t nmemb, std::ofstream *stream)
{
    (*stream).write(static_cast<const char *>(buffer), size * nmemb);
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

fs::path get_config_path(std::string arg)
{
    auto path = fs::path(arg);
    path = path.parent_path();
    path /= "config.txt";
    fmt::print("path: {}\n", path.string());

    if (fs::is_regular_file(path))
    {
        return path;
    }
    path = path.parent_path();
    path /= "config.example.txt";
    fmt::print("path: {}\n", path.string());

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path /= "config";
    path /= "config.txt";
    fmt::print("path: {}\n", path.string());

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path /= "config";
    path /= "config.example.txt";
    fmt::print("path: {}\n", path.string());

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = path.parent_path().parent_path();
    path /= "config.txt";
    fmt::print("path: {}\n", path.string());

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = path.parent_path();
    path /= "config.example.txt";
    fmt::print("path: {}\n", path.string());

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path = path.parent_path();
    path /= "config";
    path /= "config.txt";
    fmt::print("path: {}\n", path.string());

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path = path.parent_path();
    path /= "config";
    path /= "config.example.txt";
    fmt::print("path: {}\n", path.string());

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path = path.parent_path().parent_path();
    path /= "config";
    path /= "config.txt";
    fmt::print("path: {}\n", path.string());

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path = path.parent_path().parent_path();
    path /= "config";
    path /= "config.example.txt";
    fmt::print("path: {}\n", path.string());

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path = path.parent_path().parent_path().parent_path();
    path /= "config";
    path /= "config.txt";
    fmt::print("path: {}\n", path.string());

    if (fs::is_regular_file(path))
    {
        return path;
    }

    path = fs::path(arg);
    path = path.parent_path().parent_path().parent_path();
    path /= "config";
    path /= "config.example.txt";
    fmt::print("path: {}\n", path.string());

    if (fs::is_regular_file(path))
    {
        return path;
    }

    fmt::print("error: can't find config file\n");
    return fs::path("");
}

int main()
{
#ifdef WIN32
    // system("chcp 65001");
#endif

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

    std::string current_path = fs::current_path().string();
    fmt::print("current path: {}\n", current_path);
    auto path = get_config_path(current_path);
    if (path.string() == "")
    {
        return -1;
    }
    fmt::print("path: {}\n", path.string());

    std::ifstream file(path);
    std::vector<std::string> data;
    std::string line;
    while (std::getline(file, line))
    {
        data.push_back(line);
    }
    auto path_str = data[0];
    std::vector<std::string> v;
    str::split(v, path_str, [](auto c)
               { return c == '='; });
    path_str = v.at(1);
    fmt::print("{}\n", path_str);

    if (!(path_str[1] == ':'))
    {
        auto cur = fs::path(current_path);
        cur /= path_str;
        path_str = cur.string();
    }
    fmt::print("out path: {}\n", path_str);

    data.erase(data.begin());

    auto src_path_str = data[0];
    // fmt::print("src_path_str: {}\n", src_path_str);

    std::vector<std::string> v_dest;
    str::split(v_dest, src_path_str, [](auto c)
               { return c == '='; });
    src_path_str = v_dest.at(1);
    fmt::print("src path dir: {}\n", src_path_str);
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
        auto ftp_addr = fmt::format("ftp://{}/{}/", addr, src_path_str);

        curl_easy_reset(handle);
        curl_easy_setopt(handle, CURLOPT_URL, ftp_addr.c_str());
        auto username = v[2];
        curl_easy_setopt(handle, CURLOPT_USERNAME, username.c_str());
        auto passwd = v[3];
        curl_easy_setopt(handle, CURLOPT_PASSWORD, passwd.c_str());
        // curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "NLST");
        curl_easy_setopt(handle, CURLOPT_DIRLISTONLY, 1L);
        // curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(handle, CURLOPT_NOPROXY, "*");
        std::string ss = "";
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &ss);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writefunc);

        curl_easy_perform(handle);
        std::vector<std::string> v2;

        str::split(v2, ss, [](auto c)
                   { return c == '\n'; });

        std::vector<std::string> dirnames;
        for (auto i : v2)
        {
            std::string trimed = str::trim_copy(i);
            if (trimed != "")
            {
                fmt::print("{}\n", trimed);
                dirnames.push_back(trimed);
            }
        }

        for (auto dirname : dirnames)
        {
            fmt::print("{}\n", dirname);
            // std::string testStr = UTF8ToGBK(dirname.c_str());
            // std::cout << dirname << std::endl;
            continue;
            auto ftp_addr = fmt::format("ftp://{}/{}/{}/", addr, src_path_str, dirname);
            curl_easy_reset(handle);
            curl_easy_setopt(handle, CURLOPT_URL, ftp_addr.c_str());
            auto username = v[2];
            curl_easy_setopt(handle, CURLOPT_USERNAME, username.c_str());
            auto passwd = v[3];
            curl_easy_setopt(handle, CURLOPT_PASSWORD, passwd.c_str());
            // curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "NLST");
            curl_easy_setopt(handle, CURLOPT_DIRLISTONLY, 1L);
            // curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(handle, CURLOPT_NOPROXY, "*");
            std::string ss = "";
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, &ss);
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writefunc);

            curl_easy_perform(handle);

            std::vector<std::string> vv2;

            str::split(vv2, ss, [](auto c)
                       { return c == '\n'; });

            std::vector<std::string> dirnames;
            for (auto i : vv2)
            {
                std::string trimed = str::trim_copy(i);
                if (trimed != "")
                {
                    fmt::print("{}\n", trimed);
                    dirnames.push_back(trimed);
                }
            }
            for (auto filename : dirnames)
            {
                fs::path out_path = fs::path(path_str);
                out_path /= dirname;
                if (!fs::is_directory(out_path))
                {
                    fs::create_directory(out_path);
                }
                out_path /= filename;

                fmt::print("out file path: {}\n", out_path.string());
                // const char *strFilename = out_path.string().c_str();

                // struct FtpFile ftpfile = {
                //     strFilename, /* name to store the file as if successful */
                //     NULL};
                auto ftp_addr = fmt::format("ftp://{}/{}/{}/{}", addr, src_path_str, dirname, filename);
                curl_easy_reset(handle);
                curl_easy_setopt(handle, CURLOPT_URL, ftp_addr.c_str());
                auto username = v[2];
                curl_easy_setopt(handle, CURLOPT_USERNAME, username.c_str());
                auto passwd = v[3];
                curl_easy_setopt(handle, CURLOPT_PASSWORD, passwd.c_str());
                // curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "NLST");
                // curl_easy_setopt(handle, CURLOPT_DIRLISTONLY, 1L);
                // curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
                curl_easy_setopt(handle, CURLOPT_NOPROXY, "*");

                // curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writefunc);
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, my_fwrite2);
                /* Set a pointer to our struct to pass to the callback */

                std::ofstream file;
                // std::string testStr = UTF8ToGBK(out_path.string().c_str());
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
