#include "Predictor.hpp"


/*
 * The constructor takes following parameters as input:
 * 1. model_json_file:  The model in json formatted file.
 * 2. model_params_file: File containing model parameters
 * 3. input_shape: Shape of input data to the model. Since this class will be running one inference at a time,
 *                 the input shape is required to be in format Shape(1, number_of_channels, height, width)
 *                 The input image will be resized to (height x width) size before running the inference.
 * 4. use_gpu: determine if run inference on GPU
 * 5. enable_tensorrt: determine if enable TensorRT
 * 6. dataset: data file (.rec) to be used for inference
 * 7. data_nthreads: number of threads for data loading
 * 8. data_layer_type: data type for data layer
 * 9. rgb_mean: mean value to be subtracted on R/G/B channel
 * 10. rgb_std: standard deviation on R/G/B channel
 * 11. shuffle_chunk_seed: shuffling chunk seed
 * 12. seed: shuffling seed
 * 13. benchmark: use dummy data for inference
 *
 * The constructor will:
 *  1. Create ImageRecordIter based on the given dataset file.
 *  2. Load the model and parameter files.
 *  3. Infer and construct NDArrays according to the input argument and create an executor.
 */