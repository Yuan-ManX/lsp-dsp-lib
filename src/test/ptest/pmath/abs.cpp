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

#include <lsp-plug.in/common/alloc.h>
#include <lsp-plug.in/common/types.h>
#include <lsp-plug.in/dsp/dsp.h>
#include <lsp-plug.in/test-fw/helpers.h>
#include <lsp-plug.in/test-fw/ptest.h>

#define MIN_RANK 8
#define MAX_RANK 16

namespace lsp
{
    namespace generic
    {
        void abs1(float *src, size_t count);
        void abs2(float *dst, const float *src, size_t count);
    }

    IF_ARCH_X86(
        namespace sse
        {
            void abs1(float *src, size_t count);
            void abs2(float *dst, const float *src, size_t count);
        }
    )

    IF_ARCH_X86_64(
        namespace avx
        {
            void x64_abs1(float *src, size_t count);
            void x64_abs2(float *dst, const float *src, size_t count);
        }
    )

    IF_ARCH_ARM(
        namespace neon_d32
        {
            void abs1(float *src, size_t count);
            void abs2(float *dst, const float *src, size_t count);
        }
    )

    IF_ARCH_AARCH64(
        namespace asimd
        {
            void abs1(float *src, size_t count);
            void abs2(float *dst, const float *src, size_t count);
        }
    )

    typedef void (* abs1_t)(float *src, size_t count);
    typedef void (* abs2_t)(float *dst, const float *src, size_t count);
}

//-----------------------------------------------------------------------------
// Performance test
PTEST_BEGIN("dsp.pmath", abs, 5, 1000)

    void call(const char *label, float *dst, size_t count, abs1_t func)
    {
        if (!PTEST_SUPPORTED(func))
            return;

        char buf[80];
        sprintf(buf, "%s x %d", label, int(count));
        printf("Testing %s numbers...\n", buf);

        PTEST_LOOP(buf,
            func(dst, count);
        );
    }

    void call(const char *label, float *dst, const float *src, size_t count, abs2_t func)
    {
        if (!PTEST_SUPPORTED(func))
            return;

        char buf[80];
        sprintf(buf, "%s x %d", label, int(count));
        printf("Testing %s numbers...\n", buf);

        PTEST_LOOP(buf,
            func(dst, src, count);
        );
    }

    PTEST_MAIN
    {
        size_t buf_size = 1 << MAX_RANK;
        uint8_t *data   = NULL;
        float *dst      = alloc_aligned<float>(data, buf_size * 3, 64);
        float *src      = &dst[buf_size];
        float *backup   = &src[buf_size];

        for (size_t i=0; i < buf_size*3; ++i)
            dst[i]          = randf(-1.0f, 1.0f);
        dsp::copy(backup, dst, buf_size);

        #define CALL(...) \
            dsp::copy(dst, backup, buf_size); \
            call(__VA_ARGS__);

        for (size_t i=MIN_RANK; i <= MAX_RANK; ++i)
        {
            size_t count = 1 << i;

            CALL("generic::abs1", dst, count, generic::abs1);
            IF_ARCH_X86(CALL("sse::abs1", dst, count, sse::abs1));
            IF_ARCH_X86_64(CALL("avx::x64_abs1", dst, count, avx::x64_abs1));
            IF_ARCH_ARM(CALL("neon_d32::abs1", dst, count, neon_d32::abs1));
            IF_ARCH_AARCH64(CALL("asimd::abs1", dst, count, asimd::abs1));
            PTEST_SEPARATOR;

            CALL("generic::abs2", dst, src, count, generic::abs2);
            IF_ARCH_X86(CALL("sse::abs2", dst, src, count, sse::abs2));
            IF_ARCH_X86_64(CALL("avx::x64_abs2", dst, src, count, avx::x64_abs2));
            IF_ARCH_ARM(CALL("neon_d32::abs2", dst, src, count, neon_d32::abs2));
            IF_ARCH_AARCH64(CALL("asimd::abs2", dst, src, count, asimd::abs2));
            PTEST_SEPARATOR2;
        }

        free_aligned(data);
    }
PTEST_END


