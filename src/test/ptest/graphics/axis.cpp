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
        void axis_apply_log1(float *x, const float *v, float zero, float norm_x, size_t count);
        void axis_apply_log2(float *x, float *y, const float *v, float zero, float norm_x, float norm_y, size_t count);
    }

    IF_ARCH_X86(
        namespace sse2
        {
            void axis_apply_log1(float *x, const float *v, float zero, float norm_x, size_t count);
            void axis_apply_log2(float *x, float *y, const float *v, float zero, float norm_x, float norm_y, size_t count);
        }

        IF_ARCH_X86_64(
            namespace sse3
            {
                void x64_axis_apply_log1(float *x, const float *v, float zero, float norm_x, size_t count);
                void x64_axis_apply_log2(float *x, float *y, const float *v, float zero, float norm_x, float norm_y, size_t count);
            }
        )
    )

    IF_ARCH_ARM(
        namespace neon_d32
        {
            void axis_apply_log1(float *x, const float *v, float zero, float norm_x, size_t count);
            void axis_apply_log2(float *x, float *y, const float *v, float zero, float norm_x, float norm_y, size_t count);
        }
    )

    IF_ARCH_AARCH64(
        namespace asimd
        {
            void axis_apply_log1(float *x, const float *v, float zero, float norm_x, size_t count);
            void axis_apply_log2(float *x, float *y, const float *v, float zero, float norm_x, float norm_y, size_t count);
        }
    )

    typedef void (* axis_apply_log1_t)(float *x, const float *v, float zero, float norm_x, size_t count);
    typedef void (* axis_apply_log2_t)(float *x, float *y, const float *v, float zero, float norm_x, float norm_y, size_t count);
}

//-----------------------------------------------------------------------------
// Performance test for logarithmic axis calculation
PTEST_BEGIN("dsp.graphics", axis, 5, 1000)

    void call(const char *label, float *x, const float *v, size_t count, axis_apply_log1_t apply)
    {
        if (!PTEST_SUPPORTED(apply))
            return;

        char buf[80];
        sprintf(buf, "%s x %d", label, int(count));
        printf("Testing %s points...\n", buf);

        PTEST_LOOP(buf,
            apply(x, v, 1.0f, 1.0f, count);
        );
    }

    void call(const char *label, float *x, float *y, const float *v, size_t count, axis_apply_log2_t apply)
    {
        if (!PTEST_SUPPORTED(apply))
            return;

        char buf[80];
        sprintf(buf, "%s x %d", label, int(count));
        printf("Testing %s points...\n", buf);

        PTEST_LOOP(buf,
            apply(x, y, v, 1.0f, 1.0f, 1.0f, count);
        );
    }

    PTEST_MAIN
    {
        size_t buf_size     = 1 << MAX_RANK;
        uint8_t *data       = NULL;
        float *ptr          = alloc_aligned<float>(data, buf_size * 3, 64);

        float *x            = ptr;
        float *y            = &x[buf_size];
        float *v            = &y[buf_size];

        for (size_t i=0; i<buf_size; ++i)
             v[i]          = randf(0.0f, 1.0f);

        #define CALL1(func) \
            call(#func, x, v, count, func)
        #define CALL2(func) \
            call(#func, x, y, v, count, func)

        for (size_t i=MIN_RANK; i <= MAX_RANK; ++i)
        {
            size_t count = 1 << i;

            CALL1(generic::axis_apply_log1);
            IF_ARCH_X86(CALL1(sse2::axis_apply_log1));
            IF_ARCH_X86_64(CALL1(sse3::x64_axis_apply_log1));
            IF_ARCH_ARM(CALL1(neon_d32::axis_apply_log1));
            IF_ARCH_AARCH64(CALL1(asimd::axis_apply_log1));
            PTEST_SEPARATOR;

            CALL2(generic::axis_apply_log2);
            IF_ARCH_X86(CALL2(sse2::axis_apply_log2));
            IF_ARCH_X86_64(CALL2(sse3::x64_axis_apply_log2));
            IF_ARCH_ARM(CALL2(neon_d32::axis_apply_log2));
            IF_ARCH_AARCH64(CALL2(asimd::axis_apply_log2));
            PTEST_SEPARATOR2;
        }

        free_aligned(data);
    }
PTEST_END


