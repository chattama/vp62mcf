
#include "vp62.h"

int main(int argc, char* argv[]) {

    AVS2AVI_COUNT count;
    AVS2AVI_COMPVARS cvar;
    VP62_MCF p1;
    VP62_MCF p2;

    init_vp62_avs2avi(&cvar);

    init_vp62(&p1);
    init_vp62(&p2);

    set_default(&p1);
    set_default(&p2);

    count.npass = 2;
    count.current = 1;

    p1.params.mode = VP62_MODE_NPASS_FIRST;
    p2.params.mode = VP62_MODE_NPASS_BEST;

    //load_vp62_avs2avi("vp62.bin", &count, &cvar, &p1, &p2);
    save_vp62_avs2avi("vp62.bin", &count, &cvar, &p1, &p2);

    //load_vp62("vp62.mcf", &mcf);
    //save_vp62("vp62.mcf", &mcf);

    // ex:
    //  avs2avi version.avs version.avi -w -P 2 -l vp62.bin
    //  mencoder -ffourcc VP62 -of lavf -ovc vfw -xvfwopts codec=vp6vfw.dll:compdata=vp62.mcf -oac copy -o version.avi version.avs
    //  VirtualDub.video.SetCompData(2056,base64enc(VP62_MCF));

    return 0;
}
