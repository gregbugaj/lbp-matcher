#include <iostream>
#include <experimental/filesystem>
#include <set>
#include <mxnet-cpp/MxNetCpp.h>
#include "utils.hpp"
#include "Lenet.hpp"

namespace fs = std::experimental::filesystem;
using namespace mxnet::cpp;
using namespace std::chrono;

void train_mxnet();

/*The global context, change them if necessary*/
static mxnet::cpp::Context global_ctx(mxnet::cpp::kCPU, 0);
// static Context global_ctx(mxnet::cpp::kCPU,0);


int main(int argc, char const *argv[]) {
    std::cout << "MxNet Base";
    try {
        Lenet lenet;
        lenet.Run(2);
        MXNotifyShutdown();
    } catch (dmlc::Error &err) {
        LG << "Status: FAIL";
        LG << "With Error: " << MXGetLastError();
        return 1;
    }
    return 0;
}

fs::path getTestDeckDirectory(const std::string &folder) {
    auto path = fs::current_path();
    path /= "../test-deck";
    path /= folder;
    return path;
}
