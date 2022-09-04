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

#ifndef PRIVATE_DSP_EXPORTS_H_
#define PRIVATE_DSP_EXPORTS_H_

#include <lsp-plug.in/dsp/version.h>

#define LSP_DSP_LIB_SYMBOL(ret, name, ...) \
    namespace lsp { \
        namespace dsp { \
            LSP_DSP_LIB_PUBLIC \
            extern      ret (* name)(__VA_ARGS__); \
            \
            extern "C" { \
                LSP_DSP_LIB_PUBLIC \
                extern ret (* LSP_DSP_LIB_MANGLE(name))(__VA_ARGS__); \
            } \
        } \
    }

#endif /* PRIVATE_DSP_EXPORTS_H_ */
