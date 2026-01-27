#include "definitions.h"

struct MergedNeuronUpdateGroup0
 {
    double* __restrict V;
    double* __restrict W;
    uint32_t* __restrict recordSpk;
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
static MergedNeuronUpdateGroup0 mergedNeuronUpdateGroup0[1];
void pushMergedNeuronUpdateGroup0ToDevice(unsigned int idx, double* V, double* W, uint32_t* recordSpk, double* v1, double* v2, double* v3, double* v4, double* w1, double* w2, double* w3, double* w4) {
    mergedNeuronUpdateGroup0[idx].V = V;
    mergedNeuronUpdateGroup0[idx].W = W;
    mergedNeuronUpdateGroup0[idx].recordSpk = recordSpk;
    mergedNeuronUpdateGroup0[idx].v1 = v1;
    mergedNeuronUpdateGroup0[idx].v2 = v2;
    mergedNeuronUpdateGroup0[idx].v3 = v3;
    mergedNeuronUpdateGroup0[idx].v4 = v4;
    mergedNeuronUpdateGroup0[idx].w1 = w1;
    mergedNeuronUpdateGroup0[idx].w2 = w2;
    mergedNeuronUpdateGroup0[idx].w3 = w3;
    mergedNeuronUpdateGroup0[idx].w4 = w4;
}
void updateNeurons(double t, unsigned int recordingTimestep) {
     {
        // merged neuron update group 0
        for(unsigned int g = 0; g < 1; g++) {
            const auto *group = &mergedNeuronUpdateGroup0[g]; 
            const unsigned int numRecordingWords = ((1u) + 31) / 32;
            std::fill_n(&group->recordSpk[recordingTimestep * numRecordingWords], numRecordingWords, 0);
            
            for(unsigned int i = 0; i < (1u); i++) {
                double Isyn = 0;
                double _lw3 = group->w3[i];
                double _lV = group->V[i];
                double _lw2 = group->w2[i];
                double _lW = group->W[i];
                double _lv1 = group->v1[i];
                double _lv2 = group->v2[i];
                double _lw1 = group->w1[i];
                double _lv3 = group->v3[i];
                double _lv4 = group->v4[i];
                double _lw4 = group->w4[i];
                // test whether spike condition was fulfilled previously
                // calculate membrane potential
                const double _i = Isyn + (7.00000000000000000e+00);
                if(_lV >= (1.00000000000000006e-01))
                 {
                    _lV = (-7.05999999999999961e-01);
                }
                
                _lv1 = (1.06761565836298922e-01) * (-((_lV) - (-7.05999999999999961e-01)) + ((2.00000000000000004e-02) * exp(((_lV) - (-5.04000000000000004e-01)) * (5.00000000000000000e+01))) + (3.33333333333333398e-02) * (_i - (_lW)));
                _lw1 = (6.94444444444444406e-03) * (((4.00000000000000000e+00) * (_lV - (-7.05999999999999961e-01))) - _lW);
                const double _halfDT = 1.00000000000000006e-01 * 0.5;
                double _tmpV = _lV + (_halfDT * _lv1);
                double _tmpW = _lW + (_halfDT * _lw1);
                _lv2 = (1.06761565836298922e-01) * (-((_tmpV) - (-7.05999999999999961e-01)) + ((2.00000000000000004e-02) * exp(((_tmpV) - (-5.04000000000000004e-01)) * (5.00000000000000000e+01))) + (3.33333333333333398e-02) * (_i - (_tmpW)));
                _lw2 = (6.94444444444444406e-03) * (((4.00000000000000000e+00) * (_tmpV - (-7.05999999999999961e-01))) - _tmpW);
                _tmpV = _lV + (_halfDT * _lv2);
                _tmpW = _lW + (_halfDT * _lw2);
                _lv3 = (1.06761565836298922e-01) * (-((_tmpV) - (-7.05999999999999961e-01)) + ((2.00000000000000004e-02) * exp(((_tmpV) - (-5.04000000000000004e-01)) * (5.00000000000000000e+01))) + (3.33333333333333398e-02) * (_i - (_tmpW)));
                _lw3 = (6.94444444444444406e-03) * (((4.00000000000000000e+00) * (_tmpV - (-7.05999999999999961e-01))) - _tmpW);
                _tmpV = _lV + (1.00000000000000006e-01 * _lv3);
                _tmpW = _lW + (1.00000000000000006e-01 * _lw3);
                _lv4 = (1.06761565836298922e-01) * (-((_tmpV) - (-7.05999999999999961e-01)) + ((2.00000000000000004e-02) * exp(((_tmpV) - (-5.04000000000000004e-01)) * (5.00000000000000000e+01))) + (3.33333333333333398e-02) * (_i - (_tmpW)));
                _lw4 = (6.94444444444444406e-03) * (((4.00000000000000000e+00) * (_tmpV - (-7.05999999999999961e-01))) - _tmpW);
                const double _sixthDT = 1.00000000000000006e-01 * 0.166666667;
                _lV += _sixthDT * (_lv1 + (2.0 * (_lv2 + _lv3)) + _lv4);
                if(_lV <= -0.4)
                 {
                    _lW += _sixthDT * (_lw1 + (2.0 * (_lw2 + _lw3)) + _lw4);
                }
                
                // test for and register a true spike
                if ((_lV > -0.4)) {
                    group->recordSpk[(recordingTimestep * numRecordingWords) + (i / 32)] |= (1 << (i % 32));
                    // spike reset code
                    _lV = (1.00000000000000006e-01);
                    _lW += (8.05000000000000049e-01);
                }
                group->w3[i] = _lw3;
                group->V[i] = _lV;
                group->w2[i] = _lw2;
                group->W[i] = _lW;
                group->v1[i] = _lv1;
                group->v2[i] = _lv2;
                group->w1[i] = _lw1;
                group->v3[i] = _lv3;
                group->v4[i] = _lv4;
                group->w4[i] = _lw4;
            }
        }
    }
}
