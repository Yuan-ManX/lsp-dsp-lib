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

#ifndef PRIVATE_DSP_ARCH_X86_SSE_FASTCONV_H_
#define PRIVATE_DSP_ARCH_X86_SSE_FASTCONV_H_

#ifndef PRIVATE_DSP_ARCH_X86_SSE_IMPL
    #error "This header should not be included directly"
#endif /* PRIVATE_DSP_ARCH_X86_SSE_IMPL */

#include <private/dsp/arch/x86/sse/fastconv/parse.h>
#include <private/dsp/arch/x86/sse/fastconv/papply.h>
#include <private/dsp/arch/x86/sse/fastconv/apply.h>
#include <private/dsp/arch/x86/sse/fastconv/irestore.h>
#include <private/dsp/arch/x86/sse/fastconv/restore.h>

namespace lsp
{
    namespace sse
    {
        void fastconv_parse_apply(float *dst, float *tmp, const float *c, const float *src, size_t rank)
        {
            // Do direct FFT
            fastconv_parse_internal(tmp, src, rank);

            // Apply complex convolution
            fastconv_parse_apply_internal(tmp, c, rank);

            // Do reverse FFT
            fastconv_restore_internal(dst, tmp, rank);
        }

        void fastconv_apply(float *dst, float *tmp, const float *c1, const float *c2, size_t rank)
        {
            // Apply complex convolution
            fastconv_apply_internal(tmp, c1, c2, rank);

            // Do reverse FFT
            fastconv_restore_internal(dst, tmp, rank);
        }
    }
}

#endif /* PRIVATE_DSP_ARCH_X86_SSE_FASTCONV_H_ */
