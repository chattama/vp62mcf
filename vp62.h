#ifndef VP62_H
#define VP62_H

#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <vfw.h>

enum EncodeMode {
    VP62_MODE_LIVE          = 0,
    VP62_MODE_1PASS_GOOD    = 1,
    VP62_MODE_1PASS_BEST    = 2,
    VP62_MODE_NPASS_FIRST   = 3,
    VP62_MODE_NPASS_GOOD    = 4,
    VP62_MODE_NPASS_BEST    = 5,
};

struct VP62_PARAMS {
    unsigned long bitrate;
    unsigned long adjustQuantizerMax;
    unsigned long __dummy1[4];
    unsigned long autoKeyframe;
    unsigned long __dummy2[2];
    unsigned long maxFrameBtwKeys;
    unsigned long noiseReduction;
    unsigned long temporalResampling;
    unsigned long spatialResampling;
    unsigned long __dummy3[7];
    unsigned long material;
    unsigned long adjustQuantizer;
    unsigned long prebuffer;
    unsigned long optimalBuffer;
    unsigned long temporalResampDown;
    unsigned long spatialResampDown;
    unsigned long spatialResampUp;
    unsigned long __dummy4[5];
    unsigned long adjustQuantizerMin;
    unsigned long undershoot;
    unsigned long peakBitrate;
    unsigned long maxBuffer;
    unsigned long __dummy5[1];
    unsigned long valiability;
    unsigned long maxSection;
    unsigned long minSection;
    unsigned long __dummy6[1];
    unsigned long mode;
    unsigned long endUsage;
    unsigned long sharpness;
    unsigned long deleteAfterUse;
    char          firstPassFile[512];
    char          settingsFile[512];
    unsigned long __dummy7[208];
};

struct VP62_COMPVARS {
    unsigned long fccHandler;;
    unsigned long lKey;
    unsigned long lQ;
    unsigned long lDataRate;
    unsigned long lStateSize;
};

struct VP62_MCF {
    VP62_COMPVARS cvar;
    VP62_PARAMS   params;
};

struct AVS2AVI_COUNT {
    int npass;
    int current;
};

typedef VP62_COMPVARS AVS2AVI_COMPVARS;

void init_vp62(VP62_MCF* mcf) {
    memset(mcf, 0, sizeof(VP62_MCF));
    // Do not change
    mcf->cvar.fccHandler    = *(unsigned long*)"VP62";
    mcf->cvar.lKey          = 1;
    mcf->cvar.lQ            = ICQUALITY_DEFAULT;
    mcf->cvar.lDataRate     = 0;
    mcf->cvar.lStateSize    = sizeof(VP62_PARAMS);
    // What is this flag ?
    // When this is not set, it becomes an error. 
    mcf->params.__dummy1[1]         = 0x00000001;
}

void init_vp62_avs2avi(AVS2AVI_COMPVARS* cvar) {
    memset(cvar, 0, sizeof(AVS2AVI_COMPVARS));
    cvar->fccHandler        = *(unsigned long*)"vp62";
    cvar->lKey              = 1;
    cvar->lQ                = ICQUALITY_DEFAULT;
    cvar->lDataRate         = 0;
    cvar->lStateSize        = sizeof(VP62_MCF);
}

void load_vp62(const char* fn, VP62_MCF* mcf) {
    FILE* fp;
    if ((fp = fopen(fn, "rb")) == NULL) return;
    fread(mcf, sizeof(VP62_MCF), 1, fp);
    fclose(fp);
}

void save_vp62(const char* fn, VP62_MCF* mcf) {
    FILE* fp;
    if ((fp = fopen(fn, "wb")) == NULL) return;
    fwrite(mcf, sizeof(VP62_MCF), 1, fp);
    fclose(fp);
}

void load_vp62_avs2avi(const char* fn, AVS2AVI_COUNT* count, AVS2AVI_COMPVARS* cvar, VP62_MCF* p1, VP62_MCF* p2 = NULL) {
    FILE* fp;
    if ((fp = fopen(fn, "rb")) == NULL) return;
    fread(&count, sizeof(AVS2AVI_COUNT), 1, fp);
    fread(cvar, sizeof(AVS2AVI_COMPVARS), 1, fp);
    fread(p1, sizeof(VP62_MCF), 1, fp);
    if (p2 && count->npass > 1) {
        fread(cvar, sizeof(AVS2AVI_COMPVARS), 1, fp);
        fread(p2, sizeof(VP62_MCF), 1, fp);
    }
    fclose(fp);
}

void save_vp62_avs2avi(const char* fn, AVS2AVI_COUNT* count, AVS2AVI_COMPVARS* cvar, VP62_MCF* p1, VP62_MCF* p2 = NULL) {
    FILE* fp;
    if ((fp = fopen(fn, "wb")) == NULL) return;
    fwrite(&count, sizeof(AVS2AVI_COUNT), 1, fp);
    fwrite(cvar, sizeof(AVS2AVI_COMPVARS), 1, fp);
    fwrite(p1, sizeof(VP62_MCF), 1, fp);
    if (p2) {
        for (int i = 1; i < count->npass; i++) {
            fwrite(cvar, sizeof(AVS2AVI_COMPVARS), 1, fp);
            fwrite(p2, sizeof(VP62_MCF), 1, fp);
        }
    }
    fclose(fp);
}

void set_default(VP62_MCF* mcf) {

    // General Tab ////////////////////////////////////////////////////////////

    //  Bitrate (kilobits per sec)
    //      0 - 4294967295
    mcf->params.bitrate             = 100;

    //  Mode
    //      0 Realtime / Live Encoding
    //      1 Good Quality Fast Encoding
    //      2 One Pass - Best Quality
    //      3 Two Pass - First Pass
    //      4 Two Pass - Second Pass - Good Quality
    //      5 Two Pass - Second Pass - Best Quality
    mcf->params.mode                = 3;

    //  End Usage
    //      0 Stream From A Server (CBR)
    //      1 Local File Playback (VBR)
    mcf->params.endUsage            = 0;

    //  Material
    //      0 Progressive
    //      1 Interlaced
    mcf->params.material            = 0;

    //  Noise Reduction
    //      0(Off) - 6(Extreme)
    mcf->params.noiseReduction      = 0;

    //  Sharpness
    //      0(Low) - 10(High)
    mcf->params.sharpness           = 0;

    //  Auto Keyframe
    //      0 False
    //      1 True
    mcf->params.autoKeyframe        = 1;

    //  Max Frame Btw Keys
    //      0 - 2147483647
    mcf->params.maxFrameBtwKeys     = 100;

    // Advanced Tab ///////////////////////////////////////////////////////////

    // Datarate Control

    //  Undershoot (% target)
    //      50 - 100
    mcf->params.undershoot          = 50;

    //  Adjust Quantizer
    //      0 True
    //      1 False
    mcf->params.adjustQuantizer     = 0;
    //    Minimum
    //      0 - 63
    mcf->params.adjustQuantizerMin  = 4;
    //    Maximum
    //      0 - 63
    mcf->params.adjustQuantizerMax  = 56;

    //  Temporal Resampling
    //      0 False
    //      1 True
    mcf->params.temporalResampling  = 0;
    //    Down Watermark %
    //      0 - 100
    mcf->params.temporalResampDown  = 0;

    //  Spatial Resampling
    //      0 True
    //      1 False
    mcf->params.spatialResampling   = 1;
    //    Down Watermark %
    //      0 - 100
    mcf->params.spatialResampDown   = 35;
    //    Up Watermark %
    //      0 - 100
    mcf->params.spatialResampUp     = 45;

    // Streaming Parameters

    //  Peak Bitrate (as % targ)
    //      0 - 99999
    mcf->params.peakBitrate         = 100;

    //  Prebuffer (secs)
    //      0 - 30
    mcf->params.prebuffer           = 4;

    //  Optimal Buffer (secs)
    //      0 - 30
    mcf->params.optimalBuffer       = 5;

    //  Max Buffer (secs)
    //      0 - 30
    mcf->params.maxBuffer           = 6;

    // Two Pass Section Datarate

    //  Valiability
    //      0 - 100
    mcf->params.valiability         = 70;

    //  Min Section (as % targ)
    //      0 - 100
    mcf->params.minSection          = 40;

    //  Max Section (as % targ)
    //      100 - 1000
    mcf->params.maxSection          = 400;

    // Settings Tab ///////////////////////////////////////////////////////////

    //  Delete After Use
    //      0 False
    //      1 True
    mcf->params.deleteAfterUse      = 1;

    //  Settings File
    memset(&mcf->params.settingsFile, 0, sizeof(&mcf->params.settingsFile));
    strcpy((char*)&mcf->params.settingsFile, "vp62.vps");

    //  First Pass File
    memset(&mcf->params.firstPassFile, 0, sizeof(mcf->params.firstPassFile));
    strcpy((char*)&mcf->params.firstPassFile, "vp62.vpf");
}

#endif
