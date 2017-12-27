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

bool CDC::FileOps::unpack_firmware(const std::string &fileName) {
  struct archive_entry *entry;
  std::vector<std::string> fileNames;
  struct archive *arc = archive_read_new();

  archive_read_support_filter_all(arc);
  archive_read_support_format_all(arc);
  int r = archive_read_open_filename(arc, fileName.c_str(), 10240); // Note 1
  if (r != ARCHIVE_OK)
    return false;
  while (archive_read_next_header(arc, &entry) == ARCHIVE_OK) {
    fileNames.push_back(archive_entry_pathname(entry));
    archive_read_data_skip(arc);
  }

  if (!fileNames.empty() && std::find(fileNames.begin(), fileNames.end(), "firmware.bin") != fileNames.end() &&
        std::find(fileNames.begin(), fileNames.end(), "manifest") != fileNames.end()) {
    //Found firmware.bin & manifest
    try {
      std::experimental::filesystem::remove("/tmp/manifest");
      std::experimental::filesystem::remove("/tmp/firmware.bin"); //remove old firmware
    } catch(...) {}

    std::stringstream cmd;
    cmd << "tar vzxf " << fileName << " -C /tmp/";
    if (system(cmd.str().c_str()) < 0) { //Ghetto extract
      return false;
    }

    r = archive_read_free(arc);
    if (r != ARCHIVE_OK)
      return false;
    else
      return true;
  }
  else {
    archive_read_free(arc);
    return false;
  }
}

bool CDC::FileOps::verify_hmac() {
  std::ifstream manifest("/tmp/manifest");
  if (manifest.good()) {
    std::string hmacStr;
    std::getline(manifest, hmacStr);
    if (hmacStr.empty() || hmacStr.length() < 64) { //Invalid HMAC!
      return false;
    }
    return CDC::FileOps::check_hmac("/tmp/firmware.bin", hmacStr);
  }
  else {
    return false;
  }
}
