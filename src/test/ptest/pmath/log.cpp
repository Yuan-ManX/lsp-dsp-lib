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
        void logb1(float *dst, size_t count);
        void logb2(float *dst, const float *src, size_t count);
        void loge1(float *dst, size_t count);
        void loge2(float *dst, const float *src, size_t count);
        void logd1(float *dst, size_t count);
        void logd2(float *dst, const float *src, size_t count);
    }

    IF_ARCH_X86(
        namespace sse2
        {
            void logb1(float *dst, size_t count);
            void logb2(float *dst, const float *src, size_t count);
            void loge1(float *dst, size_t count);
            void loge2(float *dst, const float *src, size_t count);
            void logd1(float *dst, size_t count);
            void logd2(float *dst, const float *src, size_t count);
        }
    )

    IF_ARCH_X86_64(
        namespace avx2
        {
            void x64_logb1(float *dst, size_t count);
            void x64_logb2(float *dst, const float *src, size_t count);
            void x64_loge1(float *dst, size_t count);
            void x64_loge2(float *dst, const float *src, size_t count);
            void x64_logd1(float *dst, size_t count);
            void x64_logd2(float *dst, const float *src, size_t count);

            void x64_logb1_fma3(float *dst, size_t count);
            void x64_logb2_fma3(float *dst, const float *src, size_t count);
            void x64_loge1_fma3(float *dst, size_t count);
            void x64_loge2_fma3(float *dst, const float *src, size_t count);
            void x64_logd1_fma3(float *dst, size_t count);
            void x64_logd2_fma3(float *dst, const float *src, size_t count);
        }
    )

    IF_ARCH_ARM(
        namespace neon_d32
        {
            void logb1(float *dst, size_t count);
            void logb2(float *dst, const float *src, size_t count);
            void loge1(float *dst, size_t count);
            void loge2(float *dst, const float *src, size_t count);
            void logd1(float *dst, size_t count);
            void logd2(float *dst, const float *src, size_t count);
        }
    )

    IF_ARCH_AARCH64(
        namespace asimd
        {
            void logb1(float *dst, size_t count);
            void logb2(float *dst, const float *src, size_t count);
            void loge1(float *dst, size_t count);
            void loge2(float *dst, const float *src, size_t count);
            void logd1(float *dst, size_t count);
            void logd2(float *dst, const float *src, size_t count);
        }
    )

    typedef void (* log1_t)(float *dst, size_t count);
    typedef void (* log2_t)(float *dst, const float *src, size_t count);
}

//-----------------------------------------------------------------------------
// Performance test
PTEST_BEGIN("dsp.pmath", log, 5, 1000)

    void call(const char *label, float *dst, const float *src, size_t count, log1_t func)
    {
        if (!PTEST_SUPPORTED(func))
            return;

        char buf[80];
        sprintf(buf, "%s x %d", label, int(count));
        printf("Testing %s numbers...\n", buf);

        PTEST_LOOP(buf,
            dsp::copy(dst, src, count);
            func(dst, count);
        );
    }

    void call(const char *label, float *dst, const float *src, size_t count, log2_t func)
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
            dst[i]          = randf(0.0f, 1.0f);
        dsp::copy(backup, dst, buf_size);

        #define CALL(func) \
            dsp::copy(dst, backup, buf_size); \
            call(#func, dst, src, count, func);


        for (size_t i=MIN_RANK; i <= MAX_RANK; ++i)
        {
            size_t count = 1 << i;

            CALL(generic::logb1);
            IF_ARCH_X86(CALL(sse2::logb1));
            IF_ARCH_X86_64(CALL(avx2::x64_logb1));
            IF_ARCH_X86_64(CALL(avx2::x64_logb1_fma3));
            IF_ARCH_ARM(CALL(neon_d32::logb1));
            IF_ARCH_AARCH64(CALL(asimd::logb1));
            PTEST_SEPARATOR;

            CALL(generic::logb2);
            IF_ARCH_X86(CALL(sse2::logb2));
            IF_ARCH_X86_64(CALL(avx2::x64_logb2));
            IF_ARCH_X86_64(CALL(avx2::x64_logb2_fma3));
            IF_ARCH_ARM(CALL(neon_d32::logb2));
            IF_ARCH_AARCH64(CALL(asimd::logb2));
            PTEST_SEPARATOR;

            CALL(generic::loge1);
            IF_ARCH_X86(CALL(sse2::loge1));
            IF_ARCH_X86_64(CALL(avx2::x64_loge1));
            IF_ARCH_X86_64(CALL(avx2::x64_loge1_fma3));
            IF_ARCH_ARM(CALL(neon_d32::loge1));
            IF_ARCH_AARCH64(CALL(asimd::loge1));
            PTEST_SEPARATOR;

            CALL(generic::loge2);
            IF_ARCH_X86(CALL(sse2::loge2));
            IF_ARCH_X86_64(CALL(avx2::x64_loge2));
            IF_ARCH_X86_64(CALL(avx2::x64_loge2_fma3));
            IF_ARCH_ARM(CALL(neon_d32::loge2));
            IF_ARCH_AARCH64(CALL(asimd::loge2));
            PTEST_SEPARATOR;

            CALL(generic::logd1);
            IF_ARCH_X86(CALL(sse2::logd1));
            IF_ARCH_X86_64(CALL(avx2::x64_logd1));
            IF_ARCH_X86_64(CALL(avx2::x64_logd1_fma3));
            IF_ARCH_ARM(CALL(neon_d32::logd1));
            IF_ARCH_AARCH64(CALL(asimd::logd1));
            PTEST_SEPARATOR;

            CALL(generic::logd2);
            IF_ARCH_X86(CALL(sse2::logd2));
            IF_ARCH_X86_64(CALL(avx2::x64_logd2));
            IF_ARCH_X86_64(CALL(avx2::x64_logd2_fma3));
            IF_ARCH_ARM(CALL(neon_d32::logd2));
            IF_ARCH_AARCH64(CALL(asimd::logd2));
            PTEST_SEPARATOR2;
        }

        free_aligned(data);
    }
PTEST_END



