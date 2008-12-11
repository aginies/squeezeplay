/*
 * Copyright © 2006 Red Hat, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of
 * Red Hat, Inc. not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission. Red Hat, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * RED HAT, INC. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL RED HAT, INC. BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Carl D. Worth <cworth@cworth.org>
 */

#include "cairo-test.h"
#include <stddef.h>

#define SIZE 10
#define PAD 2

static cairo_test_draw_function_t draw;

cairo_test_t test = {
    "device-offset-positive",
    "Simple test using a surface with a positive device-offset as a source.",
    SIZE, SIZE,
    draw
};

static void
draw_square (cairo_t *cr)
{
    cairo_rectangle (cr,
		     PAD, PAD,
		     SIZE - 2 * PAD,
		     SIZE - 2 * PAD);
    cairo_fill (cr);
}

static cairo_test_status_t
draw (cairo_t *cr, int width, int height)
{
    cairo_surface_t *surface, *target;
    cairo_t *cr2;

    /* First draw a shape in blue on the original destination. */
    cairo_set_source_rgb (cr, 0, 0, 1); /* blue */
    draw_square (cr);

    /* Then, create an offset surface and repeat the drawing in red. */
    target = cairo_get_group_target (cr);
    surface = cairo_surface_create_similar (target,
					    cairo_surface_get_content (target),
					    SIZE / 2, SIZE / 2);
    cr2 = cairo_create (surface);

    cairo_set_source_rgb (cr2, 1, 0, 0); /* red */
    draw_square (cr2);

    cairo_destroy (cr2);

    cairo_surface_set_device_offset (surface, + SIZE / 2, + SIZE / 2);

    /* Finally, copy the offset surface to the original destination.
    * The final result should be a blue square with the upper-left
    * quarter red. */
    cairo_set_source_surface (cr, surface, SIZE / 2, SIZE / 2);

    cairo_paint (cr);

    cairo_surface_destroy (surface);

    return CAIRO_TEST_SUCCESS;
}

int
main (void)
{
    return cairo_test (&test);
}
