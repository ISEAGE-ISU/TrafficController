#include "FileOps.h"

size_t CDC::FileOps::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

bool CDC::FileOps::tftp_download(const std::string &url, const char *save_file, double *fileSize) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {

        FILE *fp;
        fp = fopen(save_file, "wb");
        if (fp == NULL) {
            curl_easy_cleanup(curl);
            return false;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CDC::FileOps::write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            return false;
        }
        res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, fileSize);
        if ((CURLE_OK != res) || (*fileSize <= 0.0)) {
           return false;
        }

        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return true;
}

bool CDC::FileOps::check_hmac(const std::string &fileName, const std::string &inHMAC) {
  std::ifstream inFile(fileName, std::ios::in | std::ios::binary);
  if (inFile.good()) {
    try {
      std::string hexHMAC;

      CryptoPP::HexEncoder hex(new CryptoPP::StringSink(hexHMAC));
      std::stringstream buf;
      buf << inFile.rdbuf();

      byte hash[CryptoPP::HMAC<CryptoPP::SHA256>::DIGESTSIZE];

      CryptoPP::HMAC<CryptoPP::SHA256> hmac(CDC::FileOps::key, CDC::FileOps::key_len);
      hmac.Update((unsigned char*)buf.str().c_str(), buf.str().length());
      hmac.Final(hash);
      hex.Put(hash, sizeof(hash));
      hex.MessageEnd();

      if(hexHMAC.compare(inHMAC) == 0) {
        return true;
      }
      return false;
    }
    catch (...) {
      return false;
    }
  }
  else {
    return false;
  }
}

bool unpack_firmware(const std::string &fileName) {

}

bool verify_firmware(const std::string &fileName) {

}
