#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    std::cout << "Simple IP Geolocation" << std::endl;
    std::string lookupip;
    std::cout << " IP \033[38;2;255;50;50m->\033[0m ";
    std::cin >> lookupip;

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "http://ip-api.com/json/" + lookupip;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return 1;
        }
    }

    Json::CharReaderBuilder readerBuilder;
    Json::Value jsonData;
    std::string errs;
    std::istringstream iss(readBuffer);

    if (Json::parseFromStream(readerBuilder, iss, &jsonData, &errs)) {
        try {
            std::cout << "\n IP\033[38;2;255;50;50m.............:\033[0m " << lookupip
                << "\n City\033[38;2;255;50;50m...........:\033[0m " << jsonData["city"].asString()
                << "\n State/Province\033[38;2;255;50;50m.:\033[0m " << jsonData["regionName"].asString()
                << "\n Country\033[38;2;255;50;50m........:\033[0m " << jsonData["country"].asString()
                << "\n Postal Code\033[38;2;255;50;50m....:\033[0m " << jsonData["zip"].asString()
                << "\n Longitude\033[38;2;255;50;50m......:\033[0m " << jsonData["lon"].asDouble()
                << "\n Latitude\033[38;2;255;50;50m.......:\033[0m " << jsonData["lat"].asDouble()
                << "\n Timezone\033[38;2;255;50;50m.......:\033[0m " << jsonData["timezone"].asString()
                << "\n ASN\033[38;2;255;50;50m............:\033[0m " << jsonData["as"].asString()
                << "\n Provider\033[38;2;255;50;50m.......:\033[0m " << jsonData["org"].asString() << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Invalid IP!" << std::endl;
        }
    }
    else {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
    }

    std::cout << std::endl;

    system("pause");

    return 0;
}