/*
 * Copyright © 2012  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_H_IN
#error "Include <hb.h> instead."
#endif

#pragma once

#include "hb-common.h"
#include "hb-font.h"

HB_BEGIN_DECLS

typedef struct hb_shape_plan_t hb_shape_plan_t;

HB_EXTERN hb_shape_plan_t *hb_shape_plan_create2(hb_face_t *face,
                                                 const hb_segment_properties_t *props,
                                                 const hb_feature_t *user_features,
                                                 unsigned int num_user_features,
                                                 const int *coords,
                                                 unsigned int num_coords);

HB_EXTERN void hb_shape_plan_destroy(hb_shape_plan_t *shape_plan);

HB_EXTERN hb_bool_t hb_shape_plan_execute(hb_shape_plan_t *shape_plan,
                                          hb_font_t *font,
                                          rb_buffer_t *buffer,
                                          const hb_feature_t *features,
                                          unsigned int num_features);

HB_END_DECLS
