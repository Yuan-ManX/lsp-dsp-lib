/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-dsp-lib
 * Created on: 31 мар. 2020 г.
 *
 * lsp-dsp-lib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * lsp-dsp-lib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with lsp-dsp-lib. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PRIVATE_DSP_ARCH_AARCH64_ASIMD_FFT_CONST_H_
#define PRIVATE_DSP_ARCH_AARCH64_ASIMD_FFT_CONST_H_

#ifndef PRIVATE_DSP_ARCH_AARCH64_ASIMD_IMPL
    #error "This header should not be included directly"
#endif /* PRIVATE_DSP_ARCH_AARCH64_ASIMD_IMPL */

/*
    // Generator code:
    int main()
    {
        // Cosines
        printf("A_RE:\n");
        for (int i=0; i<16; ++i)
        {
            int n = 4 << i;
            for (size_t k=0; (k<8); ++k)
                printf("%.16f, ", cos(M_PI * (k%n) / n));
            printf("\n");
        }

        // Sines
        printf("\nA_IM:\n");
        for (int i=0; i<16; ++i)
        {
            int n = 4 << i;
            for (size_t k=0; (k<8); ++k)
                printf("%.16f, ", sin(M_PI * (k%n) / n));
            printf("\n");
        }

        // Both
        printf("\nA:\n");
        for (int i=0; i<16; ++i)
        {
            int n = 4 << i;
            for (size_t k=0; (k<8); ++k)
                printf("%.16f, ", cos(M_PI * (k%n) / n));
            printf("\n");
            for (size_t k=0; (k<8); ++k)
                printf("%.16f, ", sin(M_PI * (k%n) / n));
            printf("\n");
        }

        printf("\nDW_RE:\n");
        for (int i=0; i<16; ++i)
            printf("LSP_DSP_VEC4(%.16f),\n", cos(M_PI / (4 << i)));

        printf("\nDW_IM:\n");
        for (int i=0; i<16; ++i)
            printf("LSP_DSP_VEC4(%.16f),\n", sin(M_PI / (4 << i)));

        printf("\nDW:\n");
        for (int i=0; i<16; ++i)
            printf("LSP_DSP_VEC4(%.16f), LSP_DSP_VEC4(%.16f),\n", cos(M_PI / (1 << i)), sin(M_PI / (1 << i)));


        return 0;
    }

 */

namespace lsp
{
    namespace asimd
    {
        IF_ARCH_AARCH64(
            static const float XFFT_A[] __lsp_aligned16 =
            {
                1.0000000000000000, 0.7071067811865476, 0.0000000000000000, -0.7071067811865475, 1.0000000000000000, 0.7071067811865476, 0.0000000000000000, -0.7071067811865475,
                0.0000000000000000, 0.7071067811865475, 1.0000000000000000, 0.7071067811865476, 0.0000000000000000, 0.7071067811865475, 1.0000000000000000, 0.7071067811865476,
                1.0000000000000000, 0.9238795325112867, 0.7071067811865476, 0.3826834323650898, 0.0000000000000000, -0.3826834323650897, -0.7071067811865475, -0.9238795325112867,
                0.0000000000000000, 0.3826834323650898, 0.7071067811865475, 0.9238795325112867, 1.0000000000000000, 0.9238795325112867, 0.7071067811865476, 0.3826834323650899,
                1.0000000000000000, 0.9807852804032304, 0.9238795325112867, 0.8314696123025452, 0.7071067811865476, 0.5555702330196023, 0.3826834323650898, 0.1950903220161283,
                0.0000000000000000, 0.1950903220161282, 0.3826834323650898, 0.5555702330196022, 0.7071067811865475, 0.8314696123025452, 0.9238795325112867, 0.9807852804032304,
                1.0000000000000000, 0.9951847266721969, 0.9807852804032304, 0.9569403357322088, 0.9238795325112867, 0.8819212643483550, 0.8314696123025452, 0.7730104533627370,
                0.0000000000000000, 0.0980171403295606, 0.1950903220161282, 0.2902846772544623, 0.3826834323650898, 0.4713967368259976, 0.5555702330196022, 0.6343932841636455,
                1.0000000000000000, 0.9987954562051724, 0.9951847266721969, 0.9891765099647810, 0.9807852804032304, 0.9700312531945440, 0.9569403357322088, 0.9415440651830208,
                0.0000000000000000, 0.0490676743274180, 0.0980171403295606, 0.1467304744553617, 0.1950903220161282, 0.2429801799032639, 0.2902846772544623, 0.3368898533922201,
                1.0000000000000000, 0.9996988186962042, 0.9987954562051724, 0.9972904566786902, 0.9951847266721969, 0.9924795345987100, 0.9891765099647810, 0.9852776423889412,
                0.0000000000000000, 0.0245412285229123, 0.0490676743274180, 0.0735645635996674, 0.0980171403295606, 0.1224106751992162, 0.1467304744553617, 0.1709618887603012,
                1.0000000000000000, 0.9999247018391445, 0.9996988186962042, 0.9993223845883495, 0.9987954562051724, 0.9981181129001492, 0.9972904566786902, 0.9963126121827780,
                0.0000000000000000, 0.0122715382857199, 0.0245412285229123, 0.0368072229413588, 0.0490676743274180, 0.0613207363022086, 0.0735645635996674, 0.0857973123444399,
                1.0000000000000000, 0.9999811752826011, 0.9999247018391445, 0.9998305817958234, 0.9996988186962042, 0.9995294175010931, 0.9993223845883495, 0.9990777277526454,
                0.0000000000000000, 0.0061358846491545, 0.0122715382857199, 0.0184067299058048, 0.0245412285229123, 0.0306748031766366, 0.0368072229413588, 0.0429382569349408,
                1.0000000000000000, 0.9999952938095762, 0.9999811752826011, 0.9999576445519639, 0.9999247018391445, 0.9998823474542126, 0.9998305817958234, 0.9997694053512153,
                0.0000000000000000, 0.0030679567629660, 0.0061358846491545, 0.0092037547820598, 0.0122715382857199, 0.0153392062849881, 0.0184067299058048, 0.0214740802754695,
                1.0000000000000000, 0.9999988234517019, 0.9999952938095762, 0.9999894110819284, 0.9999811752826011, 0.9999705864309741, 0.9999576445519639, 0.9999423496760239,
                0.0000000000000000, 0.0015339801862848, 0.0030679567629660, 0.0046019261204486, 0.0061358846491545, 0.0076698287395311, 0.0092037547820598, 0.0107376591672645,
                1.0000000000000000, 0.9999997058628822, 0.9999988234517019, 0.9999973527669782, 0.9999952938095762, 0.9999926465807072, 0.9999894110819284, 0.9999855873151432,
                0.0000000000000000, 0.0007669903187427, 0.0015339801862848, 0.0023009691514258, 0.0030679567629660, 0.0038349425697062, 0.0046019261204486, 0.0053689069639963,
                1.0000000000000000, 0.9999999264657179, 0.9999997058628822, 0.9999993381915255, 0.9999988234517019, 0.9999981616434870, 0.9999973527669782, 0.9999963968222944,
                0.0000000000000000, 0.0003834951875714, 0.0007669903187427, 0.0011504853371138, 0.0015339801862848, 0.0019174748098554, 0.0023009691514258, 0.0026844631545960,
                1.0000000000000000, 0.9999999816164293, 0.9999999264657179, 0.9999998345478677, 0.9999997058628822, 0.9999995404107661, 0.9999993381915255, 0.9999990992051678,
                0.0000000000000000, 0.0001917475973107, 0.0003834951875714, 0.0005752427637321, 0.0007669903187427, 0.0009587378455533, 0.0011504853371138, 0.0013422327863743,
                1.0000000000000000, 0.9999999954041073, 0.9999999816164293, 0.9999999586369661, 0.9999999264657179, 0.9999998851026849, 0.9999998345478677, 0.9999997748012666,
                0.0000000000000000, 0.0000958737990960, 0.0001917475973107, 0.0002876213937629, 0.0003834951875714, 0.0004793689778549, 0.0005752427637321, 0.0006711165443218,
                1.0000000000000000, 0.9999999988510269, 0.9999999954041073, 0.9999999896592414, 0.9999999816164293, 0.9999999712756709, 0.9999999586369661, 0.9999999437003151,
                0.0000000000000000, 0.0000479368996031, 0.0000958737990960, 0.0001438106983686, 0.0001917475973107, 0.0002396844958122, 0.0002876213937629, 0.0003355582910527,
                1.0000000000000000, 0.9999999997127567, 0.9999999988510269, 0.9999999974148104, 0.9999999954041073, 0.9999999928189177, 0.9999999896592414, 0.9999999859250787,
                0.0000000000000000, 0.0000239684498084, 0.0000479368996031, 0.0000719053493702, 0.0000958737990960, 0.0001198422487667, 0.0001438106983686, 0.0001677791478878
            };

            static const float XFFT_DW[] __lsp_aligned16 =
            {
                LSP_DSP_VEC4(0.0000000000000000), LSP_DSP_VEC4(0.0000000000000000),
                LSP_DSP_VEC4(-1.0000000000000000), LSP_DSP_VEC4(0.0000000000000000),
                LSP_DSP_VEC4(0.0000000000000000), LSP_DSP_VEC4(1.0000000000000000),
                LSP_DSP_VEC4(0.7071067811865476), LSP_DSP_VEC4(0.7071067811865475),
                LSP_DSP_VEC4(0.9238795325112867), LSP_DSP_VEC4(0.3826834323650898),
                LSP_DSP_VEC4(0.9807852804032304), LSP_DSP_VEC4(0.1950903220161282),
                LSP_DSP_VEC4(0.9951847266721969), LSP_DSP_VEC4(0.0980171403295606),
                LSP_DSP_VEC4(0.9987954562051724), LSP_DSP_VEC4(0.0490676743274180),
                LSP_DSP_VEC4(0.9996988186962042), LSP_DSP_VEC4(0.0245412285229123),
                LSP_DSP_VEC4(0.9999247018391445), LSP_DSP_VEC4(0.0122715382857199),
                LSP_DSP_VEC4(0.9999811752826011), LSP_DSP_VEC4(0.0061358846491545),
                LSP_DSP_VEC4(0.9999952938095762), LSP_DSP_VEC4(0.0030679567629660),
                LSP_DSP_VEC4(0.9999988234517019), LSP_DSP_VEC4(0.0015339801862848),
                LSP_DSP_VEC4(0.9999997058628822), LSP_DSP_VEC4(0.0007669903187427),
                LSP_DSP_VEC4(0.9999999264657179), LSP_DSP_VEC4(0.0003834951875714),
                LSP_DSP_VEC4(0.9999999816164293), LSP_DSP_VEC4(0.0001917475973107),
                LSP_DSP_VEC4(0.9999999954041073), LSP_DSP_VEC4(0.0000958737990960),
            };
        )
    }
}

#endif /* PRIVATE_DSP_ARCH_AARCH64_ASIMD_FFT_CONST_H_ */
