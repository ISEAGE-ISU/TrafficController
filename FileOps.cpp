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

unsigned char CDC::FileOps::key[64] = {
 0x73, 0x7f, 0xf8, 0xc2, 0x1a, 0xcc, 0xa1, 0x57, 0x64, 0x95, 0xe3, 0xea,
 0xa3, 0x16, 0xac, 0x20, 0x7c, 0x25, 0x72, 0xb6, 0xdc, 0x3a, 0x97, 0xd0,
 0x02, 0x1f, 0x70, 0x8e, 0x40, 0xf9, 0xf1, 0xb0, 0xe6, 0x23, 0xd1, 0xa0,
 0x0e, 0xc9, 0x80, 0x9b, 0xad, 0x0d, 0x8a, 0x38, 0xb5, 0xe6, 0x96, 0xea,
 0x8b, 0x66, 0x08, 0x99, 0xf9, 0x16, 0x1e, 0x4d, 0x9b, 0x91, 0x04, 0xfb,
 0xc5, 0x97, 0x0e, 0x57
};

unsigned int CDC::FileOps::key_len = 64;
