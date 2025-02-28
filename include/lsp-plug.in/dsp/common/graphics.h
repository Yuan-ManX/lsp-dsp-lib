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

#ifndef LSP_PLUG_IN_DSP_COMMON_GRAPHICS_H_
#define LSP_PLUG_IN_DSP_COMMON_GRAPHICS_H_

#include <lsp-plug.in/dsp/common/types.h>

#ifdef __cplusplus
namespace lsp
{
    namespace dsp
    {
#endif /* __cplusplus */

    #pragma pack(push, 1)
        typedef struct LSP_DSP_LIB_TYPE(hsla_hue_eff_t)
        {
            float h, s, l, a;   // Hue, saturation, lightness, alpha
            float thresh;       // alpha threshold
        } LSP_DSP_LIB_TYPE(hsla_hue_eff_t);

        typedef struct hsla_alpha_eff_t
        {
            float h, s, l, a;   // Hue, saturation, lightness, alpha
        } LSP_DSP_LIB_TYPE(hsla_alpha_eff_t);

        typedef struct LSP_DSP_LIB_TYPE(hsla_sat_eff_t)
        {
            float h, s, l, a;   // Hue, saturation, lightness, alpha
            float thresh;       // alpha threshold
        } LSP_DSP_LIB_TYPE(hsla_sat_eff_t);

        typedef struct LSP_DSP_LIB_TYPE(hsla_light_eff_t)
        {
            float h, s, l, a;   // Hue, saturation, lightness, alpha
            float thresh;       // alpha threshold
        } LSP_DSP_LIB_TYPE(hsla_light_eff_t);
    #pragma pack(pop)

#ifdef __cplusplus
    }
}
#endif /* __cplusplus */

/** Do logarithmic vector apply for 1D-schema:
 *  x[i] = x[i] + norm_x * logf(absf(v[i]*zero))
 *
 * @param x destination vector for X coordinate
 * @param v delta vector to apply
 * @param zero graphics zero point
 * @param norm_x X norming factor
 */
LSP_DSP_LIB_SYMBOL(void, axis_apply_log1, float *x, const float *v, float zero, float norm_x, size_t count);

/** Do logarithmic vector apply for 2D-schema:
 *  x[i] = x[i] + norm_x * logf(absf(v[i]*zero))
 *  y[i] = y[i] + norm_y * logf(absf(v[i]*zero))
 *
 * @param x destination vector for X coordinate
 * @param y destination vector for Y coordinate
 * @param v delta vector to apply
 * @param zero graphics zero point
 * @param norm_x X norming factor
 * @param norm_y Y norming factor
 */
LSP_DSP_LIB_SYMBOL(void, axis_apply_log2, float *x, float *y, const float *v, float zero, float norm_x, float norm_y, size_t count);

/** Convert RGBA32 -> BGRA32 color
 *
 * @param dst target buffer
 * @param src source buffer
 * @param count number of samples to process
 */
LSP_DSP_LIB_SYMBOL(void, rgba32_to_bgra32, void *dst, const void *src, size_t count);

/** Convert ABGR32 -> BGRA32 color
 *
 * @param dst target buffer
 * @param src source buffer
 * @param count number of samples to process
 */
LSP_DSP_LIB_SYMBOL(void, abgr32_to_bgra32, void *dst, const void *src, size_t count);

/** Set the specific alpha value for the Premultiplied BGRA format
 * @param dst target buffer
 * @param src source buffer
 * @param alpha the alpha value to set
 * @param count number of color values to process
 */
LSP_DSP_LIB_SYMBOL(void, pbgra32_set_alpha, void *dst, const void *src, uint8_t alpha, size_t count);

/** Set the specific alpha value for the Premultiplied RGBA format
 * @param dst target buffer
 * @param src source buffer
 * @param alpha the alpha value to set
 * @param count number of color values to process
 */
LSP_DSP_LIB_SYMBOL(void, prgba32_set_alpha, void *dst, const void *src, uint8_t alpha, size_t count);

/** Convert ABGR32 -> BGR32 color with Alpha=0xff
 *
 * @param dst target buffer
 * @param src source buffer
 * @param count number of samples to process
 */
LSP_DSP_LIB_SYMBOL(void, abgr32_to_bgrff32, void *dst, const void *src, size_t count);

/**
 * Fill array with RGBA colors
 * @param dst array to store RGBA
 * @param r red amount
 * @param g green amount
 * @param b blue amount
 * @param a alpha amount
 * @param count number of elements
 */
LSP_DSP_LIB_SYMBOL(void, fill_rgba, float *dst, float r, float g, float b, float a, size_t count);

/**
 * Fill array with HSLA colors
 * @param dst array to store HSLA
 * @param h hue amount
 * @param s saturation amount
 * @param l lightness amount
 * @param a alpha amount
 * @param count number of elements
 */
LSP_DSP_LIB_SYMBOL(void, fill_hsla, float *dst, float h, float s, float l, float a, size_t count);

/**
 * Convert RGBA to HSLA color
 * @param dst array to store HSLA
 * @param src array containing RGBA
 * @param count number of structures to convert
 */
LSP_DSP_LIB_SYMBOL(void, rgba_to_hsla, float *dst, const float *src, size_t count);

/**
 * Convert HSLA to RGBA color
 * @param dst array to store RGBA
 * @param src array containing HSLA
 * @param count number of structures to convert
 */
LSP_DSP_LIB_SYMBOL(void, hsla_to_rgba, float *dst, const float *src, size_t count);

/** Convert RGBA (float) -> BGRA32 color
 * Alpha color is applied to result by formula:
 *   A' = (1 - A) * 255
 *   R' = R * (1 - A) * 255
 *   G' = G * (1 - A) * 255
 *   B' = B * (1 - A) * 255
 *
 * @param dst target buffer (4 bytes per pixel)
 * @param src source buffer (4 floats per pixel)
 * @param count number of samples to process
 */
LSP_DSP_LIB_SYMBOL(void, rgba_to_bgra32, void *dst, const float *src, size_t count);

/** Convert RGBA32 (4 bytes) -> BGRA32 (4 bytes) color with reversed alpha channel
 * Alpha color is applied to result by formula:
 *   A' = (1 - A)
 *   R' = R * (1 - A) / 255
 *   G' = G * (1 - A) / 255
 *   B' = B * (1 - A) / 255
 *
 * @param dst target buffer (4 bytes per pixel)
 * @param src source buffer (4 bytes per pixel)
 * @param count number of samples to process
 */
LSP_DSP_LIB_SYMBOL(void, rgba32_to_bgra32_ra, void *dst, const void *src, size_t count);

/**
 * Generate set of pixels with applied hue shift effect
 * @param dst target array to store pixels
 * @param v hue shift in range (-1 .. +1)
 * @param eff effect parameters
 * @param count number of points to process
 */
LSP_DSP_LIB_SYMBOL(void, eff_hsla_hue, float *dst, const float *v, const LSP_DSP_LIB_TYPE(hsla_hue_eff_t) *eff, size_t count);

/**
 * Generate set of pixels with applied saturation shift effect
 * @param dst target array to store pixels
 * @param v saturation shift in range (-1 .. +1)
 * @param eff effect parameters
 * @param count number of points to process
 */
LSP_DSP_LIB_SYMBOL(void, eff_hsla_sat, float *dst, const float *v, const LSP_DSP_LIB_TYPE(hsla_sat_eff_t) *eff, size_t count);

/**
 * Generate set of pixels with applied lighness shift effect
 * @param dst target array to store pixels
 * @param v lightness shift in range (-1 .. +1)
 * @param eff effect parameters
 * @param count number of points to process
 */
LSP_DSP_LIB_SYMBOL(void, eff_hsla_light, float *dst, const float *v, const LSP_DSP_LIB_TYPE(hsla_light_eff_t) *eff, size_t count);

/**
 * Generate set of pixels with applied alpha effect
 * @param dst target array to store pixels
 * @param v alpha shift in range (-1 .. +1)
 * @param eff effect parameters
 * @param count number of points to process
 */
LSP_DSP_LIB_SYMBOL(void, eff_hsla_alpha, float *dst, const float *v, const LSP_DSP_LIB_TYPE(hsla_alpha_eff_t) *eff, size_t count);

/**
 * Perform cubic smooth of linear-scaled data using x^2*(3-2*x) polynom
 * @param dst target buffer to store interpolation data, excludes start and stop samples
 * @param start start interpolation value
 * @param stop end interpolation value
 * @param count number of samples to calculate
 */
LSP_DSP_LIB_SYMBOL(void, smooth_cubic_linear, float *dst, float start, float stop, size_t count);

/**
 * Perform cubic smooth of logarithmic-scaled data using x^2*(3-2*x) polynom
 * @param dst target buffer to store interpolation data, excludes start and stop samples
 * @param start start interpolation value
 * @param stop end interpolation value
 * @param count number of samples to calculate
 */
LSP_DSP_LIB_SYMBOL(void, smooth_cubic_log, float *dst, float start, float stop, size_t count);

#endif /* LSP_PLUG_IN_DSP_COMMON_GRAPHICS_H_ */
