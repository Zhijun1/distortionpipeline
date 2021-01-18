/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "xf_stereo_pipeline_config.h"

extern "C" {
void stereopipeline_accel(ap_uint<INPUT_PTR_WIDTH>* img_L,
                          ap_uint<OUTPUT_PTR_WIDTH>* img_disp,
                          float* cameraMA_l,
                          float* distC_l,
                          float* irA_l,
                          int rows,
                          int cols) {
// clang-format off
    #pragma HLS INTERFACE m_axi     port=img_L  offset=slave bundle=gmem1
    #pragma HLS INTERFACE m_axi     port=img_disp  offset=slave bundle=gmem6
    #pragma HLS INTERFACE m_axi     port=cameraMA_l  offset=slave bundle=gmem2
    #pragma HLS INTERFACE m_axi     port=distC_l  offset=slave bundle=gmem3
    #pragma HLS INTERFACE m_axi     port=irA_l  offset=slave bundle=gmem2
    #pragma HLS INTERFACE s_axilite port=rows               
    #pragma HLS INTERFACE s_axilite port=cols               
    #pragma HLS INTERFACE s_axilite port=return
    // clang-format on

    ap_fixed<32, 12> cameraMA_l_fix[XF_CAMERA_MATRIX_SIZE], 
        distC_l_fix[XF_DIST_COEFF_SIZE], irA_l_fix[XF_CAMERA_MATRIX_SIZE];

    for (int i = 0; i < XF_CAMERA_MATRIX_SIZE; i++) {
// clang-format off
        #pragma HLS PIPELINE II=1
        // clang-format on
        cameraMA_l_fix[i] = (ap_fixed<32, 12>)cameraMA_l[i];
        irA_l_fix[i] = (ap_fixed<32, 12>)irA_l[i];
    }
    for (int i = 0; i < XF_DIST_COEFF_SIZE; i++) {
// clang-format off
        #pragma HLS PIPELINE II=1
        // clang-format on
        distC_l_fix[i] = (ap_fixed<32, 12>)distC_l[i];
    }

    int _cm_size = 9, _dc_size = 5;

    xf::cv::Mat<XF_8UC3, XF_HEIGHT, XF_WIDTH, XF_NPPC1> mat_L(rows, cols);

    xf::cv::Mat<XF_32FC3, XF_HEIGHT, XF_WIDTH, XF_NPPC1> mapxLMat(rows, cols);

    xf::cv::Mat<XF_32FC3, XF_HEIGHT, XF_WIDTH, XF_NPPC1> mapyLMat(rows, cols);

    xf::cv::Mat<XF_8UC3, XF_HEIGHT, XF_WIDTH, XF_NPPC1> leftRemappedMat(rows, cols);

    //xf::cv::Mat<XF_8UC1, XF_HEIGHT, XF_WIDTH, XF_NPPC1> rightRemappedMat(rows, cols);

// clang-format off
    #pragma HLS DATAFLOW
    // clang-format on

    xf::cv::Array2xfMat<INPUT_PTR_WIDTH, XF_8UC3, XF_HEIGHT, XF_WIDTH, XF_NPPC1>(img_L, mat_L);

    xf::cv::InitUndistortRectifyMapInverse<XF_CAMERA_MATRIX_SIZE, XF_DIST_COEFF_SIZE, XF_32FC3, XF_HEIGHT, XF_WIDTH,
                                           XF_NPPC1>(cameraMA_l_fix, distC_l_fix, irA_l_fix, mapxLMat, mapyLMat,
                                                     _cm_size, _dc_size);
    xf::cv::remap<XF_REMAP_BUFSIZE, XF_INTERPOLATION_BILINEAR, XF_8UC3, XF_32FC3, XF_8UC3, XF_HEIGHT, XF_WIDTH,
                  XF_NPPC1, XF_USE_URAM>(mat_L, leftRemappedMat, mapxLMat, mapyLMat);

    xf::cv::xfMat2Array<OUTPUT_PTR_WIDTH, XF_8UC3, XF_HEIGHT, XF_WIDTH, XF_NPPC1>(leftRemappedMat, img_disp);

}
}
