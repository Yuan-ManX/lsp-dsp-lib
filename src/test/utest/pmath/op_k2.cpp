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

#include <lsp-plug.in/common/types.h>
#include <lsp-plug.in/test-fw/utest.h>
#include <lsp-plug.in/test-fw/FloatBuffer.h>

#define MIN_RANK 8
#define MAX_RANK 16

namespace lsp
{
    namespace generic
    {
        void    add_k2(float *dst, float k, size_t count);
        void    sub_k2(float *dst, float k, size_t count);
        void    rsub_k2(float *dst, float k, size_t count);
        void    mul_k2(float *dst, float k, size_t count);
        void    div_k2(float *dst, float k, size_t count);
        void    rdiv_k2(float *dst, float k, size_t count);
        void    mod_k2(float *dst, float k, size_t count);
        void    rmod_k2(float *dst, float k, size_t count);
    }

    IF_ARCH_X86(
        namespace sse
        {
            void    add_k2(float *dst, float k, size_t count);
            void    sub_k2(float *dst, float k, size_t count);
            void    mul_k2(float *dst, float k, size_t count);
            void    div_k2(float *dst, float k, size_t count);
            void    rsub_k2(float *dst, float k, size_t count);
            void    rdiv_k2(float *dst, float k, size_t count);
        }

        namespace sse2
        {
            void    mod_k2(float *dst, float k, size_t count);
            void    rmod_k2(float *dst, float k, size_t count);
        }

        namespace avx
        {
            void    add_k2(float *dst, float k, size_t count);
            void    sub_k2(float *dst, float k, size_t count);
            void    rsub_k2(float *dst, float k, size_t count);
            void    mul_k2(float *dst, float k, size_t count);
            void    div_k2(float *dst, float k, size_t count);
            void    rdiv_k2(float *dst, float k, size_t count);
            void    mod_k2(float *dst, float k, size_t count);
            void    rmod_k2(float *dst, float k, size_t count);
            void    mod_k2_fma3(float *dst, float k, size_t count);
            void    rmod_k2_fma3(float *dst, float k, size_t count);
        }

        namespace avx2
        {
            void    add_k2(float *dst, float k, size_t count);
            void    sub_k2(float *dst, float k, size_t count);
            void    rsub_k2(float *dst, float k, size_t count);
            void    mul_k2(float *dst, float k, size_t count);
            void    div_k2(float *dst, float k, size_t count);
            void    rdiv_k2(float *dst, float k, size_t count);
            void    mod_k2(float *dst, float k, size_t count);
            void    rmod_k2(float *dst, float k, size_t count);
            void    mod_k2_fma3(float *dst, float k, size_t count);
            void    rmod_k2_fma3(float *dst, float k, size_t count);
        }
    )


    IF_ARCH_ARM(
        namespace neon_d32
        {
            void    add_k2(float *dst, float k, size_t count);
            void    sub_k2(float *dst, float k, size_t count);
            void    rsub_k2(float *dst, float k, size_t count);
            void    mul_k2(float *dst, float k, size_t count);
            void    div_k2(float *dst, float k, size_t count);
            void    rdiv_k2(float *dst, float k, size_t count);
            void    mod_k2(float *dst, float k, size_t count);
            void    rmod_k2(float *dst, float k, size_t count);
        }
    )

    IF_ARCH_AARCH64(
        namespace asimd
        {
            void    add_k2(float *dst, float k, size_t count);
            void    sub_k2(float *dst, float k, size_t count);
            void    rsub_k2(float *dst, float k, size_t count);
            void    mul_k2(float *dst, float k, size_t count);
            void    div_k2(float *dst, float k, size_t count);
            void    rdiv_k2(float *dst, float k, size_t count);
            void    mod_k2(float *dst, float k, size_t count);
            void    rmod_k2(float *dst, float k, size_t count);
        }
    )
}

typedef void (* op_k2_t)(float *dst, float k, size_t count);

UTEST_BEGIN("dsp.pmath", op_k2)

    void call(const char *label, size_t align, op_k2_t func1, op_k2_t func2)
    {
        if (!UTEST_SUPPORTED(func1))
            return;
        if (!UTEST_SUPPORTED(func2))
            return;

        UTEST_FOREACH(count, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                32, 64, 65, 100, 999, 0xfff)
        {
            for (size_t mask=0; mask <= 0x01; ++mask)
            {
                printf("Testing %s on input buffer of %d numbers, mask=0x%x...\n", label, int(count), int(mask));

                FloatBuffer src(count, align, mask & 0x01);
                src.randomize_sign();
                FloatBuffer dst1(src);
                FloatBuffer dst2(src);

                // Call functions
                func1(dst1, 0.5f, count);
                func2(dst2, 0.5f, count);

                UTEST_ASSERT_MSG(dst1.valid(), "Destination buffer 1 corrupted");
                UTEST_ASSERT_MSG(dst2.valid(), "Destination buffer 2 corrupted");

                // Compare buffers
                if (!dst1.equals_adaptive(dst2, 1e-4))
                {
                    src.dump("src ");
                    dst1.dump("dst1");
                    dst2.dump("dst2");
                    printf("index=%d, %.6f vs %.6f\n", dst1.last_diff(), dst1.get_diff(), dst2.get_diff());
                    UTEST_FAIL_MSG("Output of functions for test '%s' differs", label);
                }
            }
        }
    }

    UTEST_MAIN
    {
        #define CALL(generic, func, align) \
            call(#func, align, generic, func)

        IF_ARCH_X86(CALL(generic::add_k2, sse::add_k2, 16));
        IF_ARCH_X86(CALL(generic::sub_k2, sse::sub_k2, 16));
        IF_ARCH_X86(CALL(generic::rsub_k2, sse::rsub_k2, 16));
        IF_ARCH_X86(CALL(generic::mul_k2, sse::mul_k2, 16));
        IF_ARCH_X86(CALL(generic::div_k2, sse::div_k2, 16));
        IF_ARCH_X86(CALL(generic::rdiv_k2, sse::rdiv_k2, 16));
        IF_ARCH_X86(CALL(generic::mod_k2, sse2::mod_k2, 16));
        IF_ARCH_X86(CALL(generic::rmod_k2, sse2::rmod_k2, 16));

        IF_ARCH_X86(CALL(generic::add_k2, avx::add_k2, 32));
        IF_ARCH_X86(CALL(generic::sub_k2, avx::sub_k2, 32));
        IF_ARCH_X86(CALL(generic::rsub_k2, avx::rsub_k2, 32));
        IF_ARCH_X86(CALL(generic::mul_k2, avx::mul_k2, 32));
        IF_ARCH_X86(CALL(generic::div_k2, avx::div_k2, 32));
        IF_ARCH_X86(CALL(generic::rdiv_k2, avx::rdiv_k2, 32));
        IF_ARCH_X86(CALL(generic::mod_k2, avx::mod_k2, 32));
        IF_ARCH_X86(CALL(generic::rmod_k2, avx::rmod_k2, 32));
        IF_ARCH_X86(CALL(generic::mod_k2, avx::mod_k2_fma3, 32));
        IF_ARCH_X86(CALL(generic::rmod_k2, avx::rmod_k2_fma3, 32));

        IF_ARCH_X86(CALL(generic::add_k2, avx2::add_k2, 32));
        IF_ARCH_X86(CALL(generic::sub_k2, avx2::sub_k2, 32));
        IF_ARCH_X86(CALL(generic::rsub_k2, avx2::rsub_k2, 32));
        IF_ARCH_X86(CALL(generic::mul_k2, avx2::mul_k2, 32));
        IF_ARCH_X86(CALL(generic::div_k2, avx2::div_k2, 32));
        IF_ARCH_X86(CALL(generic::rdiv_k2, avx2::rdiv_k2, 32));
        IF_ARCH_X86(CALL(generic::mod_k2, avx2::mod_k2, 32));
        IF_ARCH_X86(CALL(generic::rmod_k2, avx2::rmod_k2, 32));
        IF_ARCH_X86(CALL(generic::mod_k2, avx2::mod_k2_fma3, 32));
        IF_ARCH_X86(CALL(generic::rmod_k2, avx2::rmod_k2_fma3, 32));

        IF_ARCH_ARM(CALL(generic::add_k2, neon_d32::add_k2, 16));
        IF_ARCH_ARM(CALL(generic::sub_k2, neon_d32::sub_k2, 16));
        IF_ARCH_ARM(CALL(generic::rsub_k2, neon_d32::rsub_k2, 16));
        IF_ARCH_ARM(CALL(generic::mul_k2, neon_d32::mul_k2, 16));
        IF_ARCH_ARM(CALL(generic::div_k2, neon_d32::div_k2, 16));
        IF_ARCH_ARM(CALL(generic::rdiv_k2, neon_d32::rdiv_k2, 16));
        IF_ARCH_ARM(CALL(generic::mod_k2, neon_d32::mod_k2, 16));
        IF_ARCH_ARM(CALL(generic::rmod_k2, neon_d32::rmod_k2, 16));

        IF_ARCH_AARCH64(CALL(generic::sub_k2, asimd::sub_k2, 16));
        IF_ARCH_AARCH64(CALL(generic::add_k2, asimd::add_k2, 16));
        IF_ARCH_AARCH64(CALL(generic::rsub_k2, asimd::rsub_k2, 16));
        IF_ARCH_AARCH64(CALL(generic::mul_k2, asimd::mul_k2, 16));
        IF_ARCH_AARCH64(CALL(generic::div_k2, asimd::div_k2, 16));
        IF_ARCH_AARCH64(CALL(generic::rdiv_k2, asimd::rdiv_k2, 16));
        IF_ARCH_AARCH64(CALL(generic::mod_k2, asimd::mod_k2, 16));
        IF_ARCH_AARCH64(CALL(generic::rmod_k2, asimd::rmod_k2, 16));
    }
UTEST_END
