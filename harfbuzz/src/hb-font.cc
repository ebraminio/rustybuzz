/*
 * Copyright © 2009  Red Hat, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#include "hb.hh"

#include "hb-font.hh"
#include "hb-machinery.hh"

#include "hb-ot.h"
#include "hb-ot-font.hh"


/**
 * SECTION:hb-font
 * @title: hb-font
 * @short_description: Font objects
 * @include: hb.h
 *
 * Font objects represent a font face at a certain size and other
 * parameters (pixels per EM, points per EM, variation settings.)
 * Fonts are created from font faces, and are used as input to
 * hb_shape() among other things.
 **/

extern "C" {
  hb_bool_t rb_ot_get_nominal_glyph (const rb_ttf_parser_t *ttf_parser, hb_codepoint_t unicode,
                                     hb_codepoint_t *glyph);

  hb_bool_t rb_ot_get_variation_glyph (const rb_ttf_parser_t *ttf_parser, hb_codepoint_t unicode,
                                       hb_codepoint_t variation_selector, hb_codepoint_t *glyph);

  hb_bool_t rb_ot_get_glyph_name (const rb_ttf_parser_t *ttf_parser, hb_codepoint_t glyph,
                                  char *name, unsigned int len);
}

hb_position_t
hb_font_get_glyph_h_advance_default (hb_font_t *font,
                     hb_codepoint_t glyph)
{
  hb_position_t ret;
  font->get_glyph_h_advances (1, &glyph, 0, &ret, 0);
  return ret;
}

static hb_position_t
hb_font_get_glyph_v_advance_default (hb_font_t *font,
                     hb_codepoint_t glyph)
{
  hb_position_t ret;
  font->get_glyph_v_advances (1, &glyph, 0, &ret, 0);
  return ret;
}

static hb_bool_t
hb_font_get_glyph_h_origin_default (hb_font_t *font,
                    hb_codepoint_t glyph,
                    hb_position_t *x,
                    hb_position_t *y)
{
  hb_bool_t ret = font->parent->get_glyph_h_origin (glyph, x, y);
  if (ret)
    font->parent_scale_position (x, y);
  return ret;
}

static hb_bool_t
hb_font_get_glyph_contour_point_default (hb_font_t *font,
                     hb_codepoint_t glyph,
                     unsigned int point_index,
                     hb_position_t *x,
                     hb_position_t *y)
{
  hb_bool_t ret = font->parent->get_glyph_contour_point (glyph, point_index, x, y);
  if (ret)
    font->parent_scale_position (x, y);
  return ret;
}

/**
 * hb_font_get_glyph:
 * @font: a font.
 * @unicode:
 * @variation_selector:
 * @glyph: (out):
 *
 *
 *
 * Return value:
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph (hb_font_t *font,
           hb_codepoint_t unicode, hb_codepoint_t variation_selector,
           hb_codepoint_t *glyph)
{
  if (unlikely (variation_selector))
    return font->get_variation_glyph (unicode, variation_selector, glyph);
  return font->get_nominal_glyph (unicode, glyph);
}

/*
 * hb_font_t
 */

DEFINE_NULL_INSTANCE (hb_font_t) =
{
  HB_OBJECT_HEADER_STATIC,

  nullptr, /* parent */
  const_cast<hb_face_t *> (&_hb_Null_hb_face_t),

  1000, /* x_scale */
  1000, /* y_scale */
  1<<16, /* x_mult */
  1<<16, /* y_mult */

  0, /* x_ppem */
  0, /* y_ppem */
  0, /* ptem */

  0, /* num_coords */
  nullptr, /* coords */

  /* Zero for the rest is fine. */
};


static hb_font_t *
_hb_font_create (hb_face_t *face)
{
  hb_font_t *font;

  if (unlikely (!face))
    face = hb_face_get_empty ();
  if (!(font = hb_object_create<hb_font_t> ()))
    return hb_font_get_empty ();

  hb_face_make_immutable (face);
  font->parent = hb_font_get_empty ();
  font->face = hb_face_reference (face);
  font->x_scale = font->y_scale = hb_face_get_upem (face);
  font->x_mult = font->y_mult = 1 << 16;

  return font;
}

/**
 * hb_font_create: (Xconstructor)
 * @face: a face.
 *
 *
 *
 * Return value: (transfer full):
 *
 * Since: 0.9.2
 **/
hb_font_t *
hb_font_create (hb_face_t *face, const rb_ttf_parser_t *ttf_parser)
{
  hb_font_t *font = _hb_font_create (face);
  font->ttf_parser = ttf_parser;
  font->user_data = &font->face->table;
  return font;
}

/**
 * hb_font_get_empty:
 *
 *
 *
 * Return value: (transfer full)
 *
 * Since: 0.9.2
 **/
hb_font_t *
hb_font_get_empty ()
{
  return const_cast<hb_font_t *> (&Null(hb_font_t));
}

/**
 * hb_font_destroy: (skip)
 * @font: a font.
 *
 *
 *
 * Since: 0.9.2
 **/
void
hb_font_destroy (hb_font_t *font)
{
  if (!hb_object_destroy (font)) return;

  if (font->destroy)
    font->destroy (font->user_data);

  hb_font_destroy (font->parent);
  hb_face_destroy (font->face);

  free (font->coords);

  free (font);
}

hb_face_t* hb_font_face(hb_font_t *font)
{
    return font->face;
}

/**
 * hb_font_set_scale:
 * @font: a font.
 * @x_scale:
 * @y_scale:
 *
 *
 *
 * Since: 0.9.2
 **/
void
hb_font_set_scale (hb_font_t *font,
           int x_scale,
           int y_scale)
{
  if (hb_object_is_immutable (font))
    return;

  font->x_scale = x_scale;
  font->y_scale = y_scale;
  font->mults_changed ();
}

/**
 * hb_font_get_scale:
 * @font: a font.
 * @x_scale: (out):
 * @y_scale: (out):
 *
 *
 *
 * Since: 0.9.2
 **/
void
hb_font_get_scale (hb_font_t *font,
           int *x_scale,
           int *y_scale)
{
  if (x_scale) *x_scale = font->x_scale;
  if (y_scale) *y_scale = font->y_scale;
}

/**
 * hb_font_set_ptem:
 * @font: a font.
 * @ptem: font size in points.
 *
 * Sets "point size" of the font.  Set to 0 to unset.
 *
 * There are 72 points in an inch.
 *
 * Since: 1.6.0
 **/
void
hb_font_set_ptem (hb_font_t *font, float ptem)
{
  if (hb_object_is_immutable (font))
    return;

  font->ptem = ptem;
}

/*
 * Variations
 */

static void
_hb_font_adopt_var_coords_normalized (hb_font_t *font,
                      int *coords, /* 2.14 normalized */
                      unsigned int coords_length)
{
  free (font->coords);

  font->coords = coords;
  font->num_coords = coords_length;
}

void
hb_font_set_variations (hb_font_t *font,
             const int *coords,
             unsigned int coords_length)
{
  int *normalized = coords_length ? (int *) calloc (coords_length, sizeof (int)) : nullptr;
    for (unsigned int i = 0; i < coords_length; ++i) {
    normalized[i] = coords[i];
  }

  _hb_font_adopt_var_coords_normalized(font, normalized, coords_length);
}

/**
 * hb_font_get_var_coords_normalized:
 *
 * Return value is valid as long as variation coordinates of the font
 * are not modified.
 *
 * Since: 1.4.2
 */
const int *
hb_font_get_var_coords_normalized (hb_font_t *font,
                   unsigned int *length)
{
  if (length)
    *length = font->num_coords;

  return font->coords;
}


hb_bool_t hb_font_t::get_font_h_extents (hb_font_extents_t *extents)
{
  memset (extents, 0, sizeof (*extents));
  return hb_ot_get_font_h_extents (this, extents);
}

hb_bool_t hb_font_t::get_nominal_glyph (hb_codepoint_t unicode, hb_codepoint_t *glyph)
{
  *glyph = 0;
  return rb_ot_get_nominal_glyph (ttf_parser, unicode, glyph);
}

unsigned int hb_font_t::get_nominal_glyphs (unsigned int count, const hb_codepoint_t *first_unicode,
                                            unsigned int unicode_stride, hb_codepoint_t *first_glyph,
                                            unsigned int glyph_stride)
{
  uint i = 0;
  for (; i < count; ++i) {
    if (!rb_ot_get_nominal_glyph (ttf_parser, *first_unicode, first_glyph)) {
      break;
    }

    first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
    first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
  }

  return i;
}

hb_bool_t hb_font_t::get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
                                          hb_codepoint_t *glyph)
{
  *glyph = 0;
  return rb_ot_get_variation_glyph (ttf_parser, unicode, variation_selector, glyph);
}

hb_position_t hb_font_t::get_glyph_h_advance (hb_codepoint_t glyph)
{
  return hb_font_get_glyph_h_advance_default (this, glyph);
}

hb_position_t hb_font_t::get_glyph_v_advance(hb_codepoint_t glyph)
{
  return hb_font_get_glyph_v_advance_default (this, glyph);
}

void hb_font_t::get_glyph_h_advances (unsigned int count, const hb_codepoint_t *first_glyph,
                                      unsigned int glyph_stride, hb_position_t *first_advance,
                                      unsigned int advance_stride)
{
  return hb_ot_get_glyph_h_advances (this, user_data, count,
                                     first_glyph, glyph_stride,
                                     first_advance, advance_stride);
}

void hb_font_t::get_glyph_v_advances (unsigned int count, const hb_codepoint_t *first_glyph,
                                      unsigned int glyph_stride, hb_position_t *first_advance,
                                      unsigned int advance_stride)
{
  return hb_ot_get_glyph_v_advances (this, user_data, count,
                                     first_glyph, glyph_stride,
                                     first_advance, advance_stride);
}

hb_bool_t hb_font_t::get_glyph_h_origin (hb_codepoint_t glyph, hb_position_t *x, hb_position_t *y)
{
  *x = *y = 0;
  return hb_font_get_glyph_h_origin_default (this, glyph, x, y);
}

hb_bool_t hb_font_t::get_glyph_v_origin (hb_codepoint_t glyph, hb_position_t *x, hb_position_t *y)
{
  *x = *y = 0;
  return hb_ot_get_glyph_v_origin (this, user_data, glyph, x, y);
}

hb_bool_t hb_font_t::get_glyph_extents (hb_codepoint_t glyph, hb_glyph_extents_t *extents)
{
  memset (extents, 0, sizeof (*extents));
  return hb_ot_get_glyph_extents (this, user_data, glyph, extents);
}

hb_bool_t hb_font_t::get_glyph_contour_point (hb_codepoint_t glyph, unsigned int point_index, hb_position_t *x, hb_position_t *y)
{
  *x = *y = 0;
  return hb_font_get_glyph_contour_point_default (this, glyph, point_index, x, y);
}

hb_bool_t hb_font_t::get_glyph_name (hb_codepoint_t glyph, char *name, unsigned int size)
{
  if (!size) return false;
  if (size) *name = '\0';
  return rb_ot_get_glyph_name (ttf_parser, glyph, name, size);
}

bool hb_font_get_glyph_extents(hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents)
{
  return font->get_glyph_extents(glyph, extents);
}
