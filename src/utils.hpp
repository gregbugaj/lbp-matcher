#ifndef LBP_MATCHER_UTILS_HPP
#define LBP_MATCHER_UTILS_HPP


#include <string>
#include <fstream>
#include <vector>
#include <mxnet-cpp/MxNetCpp.h>

using namespace mxnet::cpp;

#define TRY \
  try {
#define CATCH \
  } catch(dmlc::Error &err) { \
    LG << "Status: FAIL";\
    LG << "With Error: " << MXGetLastError(); \
    return 1; \
  }

bool isFileExists(const std::string &filename) {
    std::ifstream fhandle(filename.c_str());
    return fhandle.good();
}

bool check_datafiles(const std::vector<std::string> &data_files) {
    for (size_t index=0; index < data_files.size(); index++) {
        if (!(isFileExists(data_files[index]))) {
            LG << "Error: File does not exist: "<< data_files[index];
            return false;
        }
    }
    return true;
}

bool setDataIter(MXDataIter *iter , const std::string &useType,
                 const std::vector<std::string> &data_files, int batch_size) {
    if (!check_datafiles(data_files)) {
        return false;
    }

    iter->SetParam("batch_size", batch_size);
    iter->SetParam("shuffle", 1);
    iter->SetParam("flat", 1);

    if (useType ==  "Train") {
        iter->SetParam("image", data_files[0]);
        iter->SetParam("label", data_files[1]);
    } else if (useType == "Label") {
        iter->SetParam("image", data_files[2]);
        iter->SetParam("label", data_files[3]);
    }

    iter->CreateDataIter();
    return true;
}



#endif //LBP_MATCHER_UTILS_HPP
