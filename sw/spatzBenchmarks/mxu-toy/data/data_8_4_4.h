// Copyright 2022 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "layer.h"

const gemm_layer gemm_l = {
	.M = 8,
	.N = 4,
	.K = 4,
	.TA = 0,
	.TB = 1,
	.ALPHA = 0,
	.dtype = FP64,
	.expand = 0
};


static double gemm_A_dram [8*4] __attribute__((section(".data"))) = {0.2995562877502637, 0.24237779080894106, 0.28320739441795323, -0.23294220898382634, 0.6711532211923003, 0.7817673742546976, -1.7903080050220619, -1.748402801489203, 0.17347640142361004, 0.26684314514213736, 0.4285173645433462, 0.2318702888580676, -1.0214571469180447, -0.8272632292381021, 1.7342104609172728, -1.4920727283504345, 0.4212009947066779, 1.504572275678796, 1.2715909667771088, 0.618669993885617, -0.03720300727052493, 1.0352472972512925, -0.010085521702337353, 0.08948016046560357, 0.08403017765893717, 0.3900034836035703, -0.5798414554742108, 0.875893884648022, 0.39238484373268534, 1.108461181451433, 0.8651734647119074, 0.855215961472287};


static double gemm_B_dram [4*4] __attribute__((section(".data"))) = {0.6551143052731048, 1.9312142505772192, -0.28692658848988356, -0.226715890674857, 0.6284835523998145, 0.14532018661151294, 0.6076122224111371, -0.21202045663845112, -1.2405276437359183, -0.6426551784819938, 0.6058412077786697, 0.5717787885763634, -0.32069813902911876, 0.8636024370428342, 1.3109621860249783, 0.6217140784054469};


static double gemm_C_dram [8*4] __attribute__((section(".data"))) = {0.16048920142982762, -0.9214988800859956, -1.108152357089335, -0.18737793842779638, 0.4428268084752764, -0.23210767943346114, -0.03265155768725795, -2.4472388194699217, 1.4654350727468453, 0.9460834933616517, -1.1149258145774585, 1.1145903864961688, 0.03089355171661811, -1.3457475275202437, 1.1755166198951001, -2.6327923840518515, -0.3402723143550538, 0.9906934705391687, -0.8522497497179303, 2.1413916159323136, -1.3971667891606305, -0.3466771947976861, 0.8272179859465014, -0.8144713393247613, 0.87049067708538, 0.42876533202174155, -1.0822943886075476, 0.08978802838707413, -0.5701157444892732, -0.43797201089529525, 0.9723184024569489, 2.5161385989502594};


static const double gemm_checksum[8] = {0.9315521338675612, -3.7156475744497075, 1.6987138618089972, 1.137088160054231, 6.651906152423485, 2.433759800069301, 0.2363944665214806, 5.019028241847918};


