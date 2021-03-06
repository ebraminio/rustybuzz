/*
 * Copyright © 1998-2004  David Turner and Werner Lemberg
 * Copyright © 2004,2007,2009  Red Hat, Inc.
 * Copyright © 2011,2012  Google, Inc.
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
 * Red Hat Author(s): Owen Taylor, Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_H_IN
#error "Include <hb.h> instead."
#endif

#pragma once

#include "hb-common.h"
#include "hb-font.h"
#include "hb-unicode.h"

HB_BEGIN_DECLS

/* Loop over clusters. Duplicated in foreach_syllable(). */
#define foreach_cluster(buffer, start, end)                                                                            \
    for (unsigned int _count = rb_buffer_get_length(buffer),                                                           \
                      start = 0,                                                                                       \
                      end = _count ? rb_buffer_next_cluster(buffer, 0) : 0;                                            \
         start < _count;                                                                                               \
         start = end, end = rb_buffer_next_cluster(buffer, start))

/**
 * hb_glyph_info_t:
 * @codepoint: either a Unicode code point (before shaping) or a glyph index
 *             (after shaping).
 * @cluster: the index of the character in the original text that corresponds
 *           to this #hb_glyph_info_t, or whatever the client passes to
 *           hb_buffer_add(). More than one #hb_glyph_info_t can have the same
 *           @cluster value, if they resulted from the same character (e.g. one
 *           to many glyph substitution), and when more than one character gets
 *           merged in the same glyph (e.g. many to one glyph substitution) the
 *           #hb_glyph_info_t will have the smallest cluster value of them.
 *           By default some characters are merged into the same cluster
 *           (e.g. combining marks have the same cluster as their bases)
 *           even if they are separate glyphs, hb_buffer_set_cluster_level()
 *           allow selecting more fine-grained cluster handling.
 *
 * The #hb_glyph_info_t is the structure that holds information about the
 * glyphs and their relation to input text.
 */
typedef struct hb_glyph_info_t
{
    hb_codepoint_t codepoint;
    /*< private >*/
    hb_mask_t mask;
    /*< public >*/
    uint32_t cluster;

    /*< private >*/
    hb_var_int_t var1;
    hb_var_int_t var2;
} hb_glyph_info_t;

typedef enum { /*< flags >*/
               HB_GLYPH_FLAG_UNSAFE_TO_BREAK = 0x00000001,
               HB_GLYPH_FLAG_DEFINED = 0x00000001 /* OR of all defined flags */
} hb_glyph_flags_t;

typedef struct hb_glyph_position_t
{
    hb_position_t x_advance;
    hb_position_t y_advance;
    hb_position_t x_offset;
    hb_position_t y_offset;

    /*< private >*/
    hb_var_int_t var;
} hb_glyph_position_t;

typedef struct hb_segment_properties_t
{
    hb_direction_t direction;
    hb_script_t script;
    const char *language;
} hb_segment_properties_t;

#define HB_SEGMENT_PROPERTIES_DEFAULT                                                                                  \
    {                                                                                                                  \
        HB_DIRECTION_INVALID, HB_SCRIPT_INVALID, 0                                                                     \
    }

HB_EXTERN bool hb_segment_properties_equal(const hb_segment_properties_t *a, const hb_segment_properties_t *b);

typedef struct rb_buffer_t rb_buffer_t;

typedef enum {
    HB_BUFFER_CONTENT_TYPE_INVALID = 0,
    HB_BUFFER_CONTENT_TYPE_UNICODE,
    HB_BUFFER_CONTENT_TYPE_GLYPHS
} hb_buffer_content_type_t;

typedef enum { /*< flags >*/
               HB_BUFFER_FLAG_DEFAULT = 0x00000000u,
               HB_BUFFER_FLAG_BOT = 0x00000001u, /* Beginning-of-text */
               HB_BUFFER_FLAG_EOT = 0x00000002u, /* End-of-text */
               HB_BUFFER_FLAG_PRESERVE_DEFAULT_IGNORABLES = 0x00000004u,
               HB_BUFFER_FLAG_REMOVE_DEFAULT_IGNORABLES = 0x00000008u,
               HB_BUFFER_FLAG_DO_NOT_INSERT_DOTTED_CIRCLE = 0x00000010u
} hb_buffer_flags_t;

typedef enum {
    HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES = 0,
    HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS = 1,
    HB_BUFFER_CLUSTER_LEVEL_CHARACTERS = 2,
    HB_BUFFER_CLUSTER_LEVEL_DEFAULT = HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES
} hb_buffer_cluster_level_t;

HB_EXTERN hb_buffer_cluster_level_t rb_buffer_get_cluster_level(rb_buffer_t *buffer);

HB_EXTERN hb_direction_t rb_buffer_get_direction(rb_buffer_t *buffer);

HB_EXTERN hb_codepoint_t rb_buffer_get_invisible_glyph(rb_buffer_t *buffer);

HB_EXTERN void rb_buffer_pre_allocate(rb_buffer_t *buffer, unsigned int size);

HB_EXTERN void rb_buffer_reverse(rb_buffer_t *buffer);

HB_EXTERN void rb_buffer_reverse_range(rb_buffer_t *buffer, unsigned int start, unsigned int end);

HB_EXTERN unsigned int rb_buffer_get_length(const rb_buffer_t *buffer);

void rb_buffer_set_length(rb_buffer_t *buffer, unsigned int len);

hb_glyph_info_t *rb_buffer_get_cur(rb_buffer_t *buffer, unsigned int i);

hb_glyph_position_t *rb_buffer_get_cur_pos(rb_buffer_t *buffer);

hb_glyph_info_t *rb_buffer_get_prev(rb_buffer_t *buffer);

hb_glyph_info_t *rb_buffer_get_out_info(rb_buffer_t *buffer);

unsigned int rb_buffer_backtrack_len(rb_buffer_t *buffer);

unsigned int rb_buffer_lookahead_len(rb_buffer_t *buffer);

unsigned int rb_buffer_next_serial(rb_buffer_t *buffer);

void rb_buffer_set_cluster(hb_glyph_info_t *info, unsigned int cluster, unsigned int mask);

void rb_buffer_move_to(rb_buffer_t *buffer, unsigned int i);

void rb_buffer_swap_buffers(rb_buffer_t *buffer);

void rb_buffer_remove_output(rb_buffer_t *buffer);

void rb_buffer_clear_output(rb_buffer_t *buffer);

void rb_buffer_clear_positions(rb_buffer_t *buffer);

unsigned int rb_buffer_next_cluster(rb_buffer_t *buffer, unsigned int start);

void rb_buffer_replace_glyphs(rb_buffer_t *buffer,
                              unsigned int num_in,
                              unsigned int num_out,
                              const hb_codepoint_t *glyph_data);

void rb_buffer_merge_clusters(rb_buffer_t *buffer, unsigned int start, unsigned int end);

void rb_buffer_merge_out_clusters(rb_buffer_t *buffer, unsigned int start, unsigned int end);

void rb_buffer_unsafe_to_break(rb_buffer_t *buffer, unsigned int start, unsigned int end);

void rb_buffer_unsafe_to_break_from_outbuffer(rb_buffer_t *buffer, unsigned int start, unsigned int end);

void rb_buffer_sort(rb_buffer_t *buffer,
                    unsigned int start,
                    unsigned int end,
                    int (*compar)(const hb_glyph_info_t *, const hb_glyph_info_t *));

void rb_buffer_replace_glyph(rb_buffer_t *buffer, hb_codepoint_t glyph_index);

hb_glyph_info_t *rb_buffer_output_glyph(rb_buffer_t *buffer, hb_codepoint_t glyph_index);

void rb_buffer_output_info(rb_buffer_t *buffer, hb_glyph_info_t glyph_info);

void rb_buffer_copy_glyph(rb_buffer_t *buffer);

void rb_buffer_next_glyph(rb_buffer_t *buffer);

void rb_buffer_next_glyphs(rb_buffer_t *buffer, unsigned int n);

void rb_buffer_skip_glyph(rb_buffer_t *buffer);

void rb_buffer_reset_masks(rb_buffer_t *buffer, hb_mask_t mask);

void rb_buffer_set_masks(
    rb_buffer_t *buffer, hb_mask_t value, hb_mask_t mask, unsigned int cluster_start, unsigned int cluster_end);

void rb_buffer_delete_glyph(rb_buffer_t *buffer);

hb_glyph_info_t *rb_buffer_get_info(rb_buffer_t *buffer);

hb_glyph_position_t *rb_buffer_get_pos(rb_buffer_t *buffer);

enum hb_buffer_scratch_flags_t {
    HB_BUFFER_SCRATCH_FLAG_DEFAULT = 0x00000000u,
    HB_BUFFER_SCRATCH_FLAG_HAS_NON_ASCII = 0x00000001u,
    HB_BUFFER_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES = 0x00000002u,
    HB_BUFFER_SCRATCH_FLAG_HAS_SPACE_FALLBACK = 0x00000004u,
    HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT = 0x00000008u,
    HB_BUFFER_SCRATCH_FLAG_HAS_UNSAFE_TO_BREAK = 0x00000010u,
    HB_BUFFER_SCRATCH_FLAG_HAS_CGJ = 0x00000020u,

    /* Reserved for complex shapers' internal use. */
    HB_BUFFER_SCRATCH_FLAG_COMPLEX0 = 0x01000000u,
    HB_BUFFER_SCRATCH_FLAG_COMPLEX1 = 0x02000000u,
    HB_BUFFER_SCRATCH_FLAG_COMPLEX2 = 0x04000000u,
    HB_BUFFER_SCRATCH_FLAG_COMPLEX3 = 0x08000000u,
};

hb_buffer_scratch_flags_t *rb_buffer_get_scratch_flags(rb_buffer_t *buffer);

int rb_buffer_get_max_ops(rb_buffer_t *buffer);

void rb_buffer_set_max_ops(rb_buffer_t *buffer, int ops);

int rb_buffer_decrement_max_ops(rb_buffer_t *buffer);

unsigned int rb_buffer_get_idx(rb_buffer_t *buffer);

void rb_buffer_set_idx(rb_buffer_t *buffer, unsigned int idx);

unsigned int rb_buffer_get_out_len(rb_buffer_t *buffer);

void rb_buffer_set_out_len(rb_buffer_t *buffer, unsigned int idx);

bool rb_buffer_have_separate_output(rb_buffer_t *buffer);

hb_codepoint_t rb_buffer_get_context(rb_buffer_t *buffer, unsigned int idx1, unsigned int idx2);

unsigned int rb_buffer_get_context_len(rb_buffer_t *buffer, unsigned int idx);

HB_EXTERN hb_glyph_position_t *rb_buffer_get_glyph_positions(rb_buffer_t *buffer, unsigned int *length);

HB_EXTERN hb_script_t rb_buffer_get_script(rb_buffer_t *buffer);

HB_EXTERN hb_buffer_flags_t rb_buffer_get_flags(rb_buffer_t *buffer);

HB_EXTERN void rb_buffer_set_direction(rb_buffer_t *buffer, hb_direction_t direction);

HB_EXTERN hb_segment_properties_t rb_buffer_get_segment_properties(rb_buffer_t *buffer);

HB_END_DECLS
