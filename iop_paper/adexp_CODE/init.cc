#include "definitions.h"
struct MergedNeuronInitGroup0
 {
    double* __restrict V;
    double* __restrict W;
    double* __restrict v1;
    double* __restrict v2;
    double* __restrict v3;
    double* __restrict v4;
    double* __restrict w1;
    double* __restrict w2;
    double* __restrict w3;
    double* __restrict w4;
    
}
;
static MergedNeuronInitGroup0 mergedNeuronInitGroup0[1];
void pushMergedNeuronInitGroup0ToDevice(unsigned int idx, double* V, double* W, double* v1, double* v2, double* v3, double* v4, double* w1, double* w2, double* w3, double* w4) {
    mergedNeuronInitGroup0[idx].V = V;
    mergedNeuronInitGroup0[idx].W = W;
    mergedNeuronInitGroup0[idx].v1 = v1;
    mergedNeuronInitGroup0[idx].v2 = v2;
    mergedNeuronInitGroup0[idx].v3 = v3;
    mergedNeuronInitGroup0[idx].v4 = v4;
    mergedNeuronInitGroup0[idx].w1 = w1;
    mergedNeuronInitGroup0[idx].w2 = w2;
    mergedNeuronInitGroup0[idx].w3 = w3;
    mergedNeuronInitGroup0[idx].w4 = w4;
}
void initializeHost() {
}
void initialize() {
    // ------------------------------------------------------------------------
    // Neuron groups
     {
        // merged neuron init group 0
        for(unsigned int g = 0; g < 1; g++) {
            const auto *group = &mergedNeuronInitGroup0[g]; 
             {
                for (unsigned int i = 0; i < ((1u)); i++) {
                    double initVal;
                    initVal = (-7.05999999999999961e-01);
                    group->V[i] = initVal;
                }
            }
             {
                for (unsigned int i = 0; i < ((1u)); i++) {
                    double initVal;
                    initVal = (0.00000000000000000e+00);
                    group->W[i] = initVal;
                }
            }
             {
                for (unsigned int i = 0; i < ((1u)); i++) {
                    double initVal;
                    initVal = (0.00000000000000000e+00);
                    group->v1[i] = initVal;
                }
            }
             {
                for (unsigned int i = 0; i < ((1u)); i++) {
                    double initVal;
                    initVal = (0.00000000000000000e+00);
                    group->w1[i] = initVal;
                }
            }
             {
                for (unsigned int i = 0; i < ((1u)); i++) {
                    double initVal;
                    initVal = (0.00000000000000000e+00);
                    group->v2[i] = initVal;
                }
            }
             {
                for (unsigned int i = 0; i < ((1u)); i++) {
                    double initVal;
                    initVal = (0.00000000000000000e+00);
                    group->w2[i] = initVal;
                }
            }
             {
                for (unsigned int i = 0; i < ((1u)); i++) {
                    double initVal;
                    initVal = (0.00000000000000000e+00);
                    group->v3[i] = initVal;
                }
            }
             {
                for (unsigned int i = 0; i < ((1u)); i++) {
                    double initVal;
                    initVal = (0.00000000000000000e+00);
                    group->w3[i] = initVal;
                }
            }
             {
                for (unsigned int i = 0; i < ((1u)); i++) {
                    double initVal;
                    initVal = (0.00000000000000000e+00);
                    group->v4[i] = initVal;
                }
            }
             {
                for (unsigned int i = 0; i < ((1u)); i++) {
                    double initVal;
                    initVal = (0.00000000000000000e+00);
                    group->w4[i] = initVal;
                }
            }
        }
    }
    // ------------------------------------------------------------------------
    // Synapse groups
    // ------------------------------------------------------------------------
    // Custom update groups
    // ------------------------------------------------------------------------
    // Custom connectivity presynaptic update groups
    // ------------------------------------------------------------------------
    // Custom connectivity postsynaptic update groups
    // ------------------------------------------------------------------------
    // Custom WU update groups
    // ------------------------------------------------------------------------
    // Synapse sparse connectivity
}

void initializeSparse() {
    // ------------------------------------------------------------------------
    // Synapse groups with sparse connectivity
    // ------------------------------------------------------------------------
    // Custom sparse WU update groups
    // ------------------------------------------------------------------------
    // Custom connectivity update sparse init groups
}
