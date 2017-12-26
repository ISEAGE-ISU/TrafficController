#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <curl/curl.h>

namespace CDC {
  struct FileOps {
    static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
    static bool tftp_download(const char *url, const char *save_file);
  };
}

#endif // FILE_OPS_H
