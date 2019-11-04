#ifndef LBP_MATCHER_PREDICTOR_HPP
#define LBP_MATCHER_PREDICTOR_HPP

#include <string>
#include <set>
#include <experimental/filesystem>
#include <mxnet-cpp/MxNetCpp.h>
#include <fstream>

namespace fs = std::experimental::filesystem;
using namespace mxnet::cpp;
using namespace std::chrono;

/**
 * https://mxnet.apache.org/api/cpp/docs/tutorials/cpp_inference
 */
class Predictor {
public:
    Predictor() {}
    Predictor(const std::string& model_json_file,
              const std::string& model_params_file,
              const Shape& input_shape,
              bool use_gpu,
              bool enable_tensorrt,
              const std::string& dataset,
              const int data_nthreads,
              const std::string& data_layer_type,
              const std::vector<float>& rgb_mean,
              const std::vector<float>& rgb_std,
              int shuffle_chunk_seed,
              int seed, bool benchmark);
    void BenchmarkScore(int num_inference_batches);
    void Score(int num_skipped_batches, int num_inference_batches);
    ~Predictor();

private:
    bool CreateImageRecordIter();
    bool AdvanceDataIter(int skipped_batches);
    void LoadModel(const std::string& model_json_file);
    void LoadParameters(const std::string& model_parameters_file);
    void SplitParamMap(const std::map<std::string, NDArray> &paramMap,
                       std::map<std::string, NDArray> *argParamInTargetContext,
                       std::map<std::string, NDArray> *auxParamInTargetContext,
                       Context targetContext);
    void ConvertParamMapToTargetContext(const std::map<std::string, NDArray> &paramMap,
                                        std::map<std::string, NDArray> *paramMapInTargetContext,
                                        Context targetContext);
    void InitParameters();

    inline bool FileExists(const std::string &name) {
        std::ifstream fhandle(name.c_str());
        return fhandle.good();
    }
    int GetDataLayerType();

    std::map<std::string, NDArray> args_map_;
    std::map<std::string, NDArray> aux_map_;
    Symbol net_;
    Executor *executor_;
    Shape input_shape_;
    Context global_ctx_ = Context::cpu();

    MXDataIter *val_iter_;
    bool use_gpu_;
    bool enable_tensorrt_;
    std::string dataset_;
    int data_nthreads_;
    std::string data_layer_type_;
    std::vector<float> rgb_mean_;
    std::vector<float> rgb_std_;
    int shuffle_chunk_seed_;
    int seed_;
    bool benchmark_;
};

#endif //LBP_MATCHER_PREDICTOR_HPP
