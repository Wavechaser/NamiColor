// NAMICOLOR 3.1

// Original concept by HikariDragon, current code by Wavechaser. 

// DEFINE_UI_PARAMS([variable name], [label], DCTLUI_SLIDER_FLOAT, [default value], [min value], [max value], [step])
// DEFINE_UI_PARAMS([variable name], [label], DCTLUI_COMBO_BOX,    [default value], [enum list], [enum label list])
// DEFINE_UI_PARAMS([variable name], [label], DCTLUI_CHECK_BOX,    [default value])
// DEFINE_UI_PARAMS([variable name], [label], DCTLUI_VALUE_BOX,    [default value])

// Process mode
DEFINE_UI_PARAMS(inputType,     Input Type,         DCTLUI_COMBO_BOX,       0, {neg, rev, pfe}, {Negatives, Reversals, Log-to-Log})

// Input color space
DEFINE_UI_PARAMS(inputColor,    Input Color Space,  DCTLUI_COMBO_BOX,       0, {AdobeRGB, ProPhotoRGB, bypass}, {Adobe RGB, ProPhoto RGB (D65), Bypass})

// Range control
DEFINE_UI_PARAMS(inputGain,     Input Gain, 	    DCTLUI_SLIDER_FLOAT, 	1.0,  0.0, 2.0, 0.020)
DEFINE_UI_PARAMS(allGain,       Master Gain, 	    DCTLUI_SLIDER_FLOAT, 	0.0, -0.5, 0.5, 0.001)
DEFINE_UI_PARAMS(allShift,      Master Shift,       DCTLUI_SLIDER_FLOAT,    0.0, -1.0, 1.0, 0.001)

// Red channel
DEFINE_UI_PARAMS(R_black,       R Blackpoint,       DCTLUI_SLIDER_FLOAT,    0.0, -0.5, 0.5, 0.001)
DEFINE_UI_PARAMS(R_gain,        R Gain,             DCTLUI_SLIDER_FLOAT,    0.0, -1.0, 1.0, 0.001)
DEFINE_UI_PARAMS(R_shift,       R Shift,            DCTLUI_SLIDER_FLOAT,    0.0, -1.0, 1.0, 0.001)

// Green channel
DEFINE_UI_PARAMS(G_black,       G Blackpoint,       DCTLUI_SLIDER_FLOAT,    0.0, -0.5, 0.5, 0.001)
DEFINE_UI_PARAMS(G_gain,        G Gain,             DCTLUI_SLIDER_FLOAT,    0.0, -1.0, 1.0, 0.001)
DEFINE_UI_PARAMS(G_shift,       G Shift,            DCTLUI_SLIDER_FLOAT,    0.0, -1.0, 1.0, 0.001)

// Blue channel
DEFINE_UI_PARAMS(B_black,       B Blackpoint,       DCTLUI_SLIDER_FLOAT,    0.0, -0.5, 0.5, 0.001)
DEFINE_UI_PARAMS(B_gain,        B Gain,             DCTLUI_SLIDER_FLOAT,    0.0, -1.0, 1.0, 0.001)
DEFINE_UI_PARAMS(B_shift,       B Shift,            DCTLUI_SLIDER_FLOAT,    0.0, -1.0, 1.0, 0.001)

// Cineon lift
DEFINE_UI_PARAMS(postLift,      Fit to Cineon Base, DCTLUI_CHECK_BOX,   0)

// __DEVICE__ float3 


__DEVICE__ float3 transform(int p_Width, int p_Height, int p_X, int p_Y, float p_R, float p_G, float p_B)
{
    // Applies input to Rec. 2020 color space transform basing on selection.
    if (inputColor == AdobeRGB)
    {
        p_R = (p_R *  0.86965940f) + (p_G *  0.08676942f) + (p_B *  0.03409159f);
        p_G = (p_R *  0.09357638f) + (p_G *  0.90511022f) + (p_B *  0.00546303f);
        p_B = (p_R *  0.01676546f) + (p_G *  0.06225891f) + (p_B *  0.92799144f);
    }

    if (inputColor == ProPhotoRGB)
    {
        p_R = (p_R *  1.23494656f) + (p_G * -0.05505079f) + (p_B * -0.15449642f);
        p_G = (p_R * -0.00207296f) + (p_G *  1.06640173f) + (p_B * -0.05916268f);
        p_B = (p_R *  0.01921087f) + (p_G * -0.04657966f) + (p_B *  1.02600473f);
    }

    // Determines if inversion is needed and scales gain based on process mode.
    float inputScale = inputType == neg ? 16.0f : 1.0f;
	float invScale   = inputType == neg ? -1.0f : 1.0f;

    // Imports input into log space. Bypasses log10f() if pfe is chosen.
    // Then fits input into (0, 1) range. No +1.0f offset is needed for pfe.
    float r_init;
    float g_init;
    float b_init;

    if (inputType == neg)
    {
        r_init = invScale * (_log10f (inputScale * p_R));
        g_init = invScale * (_log10f (inputScale * p_G));
        b_init = invScale * (_log10f (inputScale * p_B));

        r_init = (r_init * inputGain) + 1.0f;
        g_init = (g_init * inputGain) + 1.0f;
        b_init = (b_init * inputGain) + 1.0f;
    }

    if (inputType == rev)
    {
        r_init = invScale * (_log10f (inputScale * p_R));
        g_init = invScale * (_log10f (inputScale * p_G));
        b_init = invScale * (_log10f (inputScale * p_B));

        r_init = (r_init * inputGain) + 0.8f;
        g_init = (g_init * inputGain) + 0.8f;
        b_init = (b_init * inputGain) + 0.8f;
    }
    
    if (inputType == pfe)
    {
        r_init = (p_R * inputGain);
        g_init = (p_G * inputGain);
        b_init = (p_B * inputGain);
    }

    // Applies independent channel shift and master shift.
    float r_1 = r_init + R_shift + allShift;
    float g_1 = g_init + G_shift + allShift;
    float b_1 = b_init + B_shift + allShift;

    // Scales each channel and lifts blackpoints
    float r_final = (r_1 + R_black) / ((1.0f - R_gain - allGain) + R_black);
    float g_final = (g_1 + G_black) / ((1.0f - G_gain - allGain) + G_black);
    float b_final = (b_1 + B_black) / ((1.0f - B_gain - allGain) + B_black);

    // Applies Cineon base
    if (postLift)
    {
        r_final = (r_final + (93.0f/1023.0f)) / (1.0f + (93.0f/1023.0f));
        g_final = (g_final + (93.0f/1023.0f)) / (1.0f + (93.0f/1023.0f));
        b_final = (b_final + (93.0f/1023.0f)) / (1.0f + (93.0f/1023.0f));
    }
    
    return make_float3(r_final, g_final, b_final);
}
