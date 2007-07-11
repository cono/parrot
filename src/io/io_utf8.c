/*
Copyright (C) 2001-2007, The Perl Foundation.
$Id$

=head1 NAME

src/io/io_utf8.c - IO Layer for UTF8

=head1 DESCRIPTION

Convert output to utf8. Convert input to Parrot's internal string
representation.

=head2 utf8 layer functions

*/

#include "parrot/parrot.h"
#include "io_private.h"
#include "../unicode.h"

/* HEADERIZER HFILE: none */

/* HEADERIZER BEGIN: static */

static size_t PIO_utf8_read( PARROT_INTERP,
    ParrotIOLayer *layer,
    ParrotIO *io,
    STRING **buf )
        __attribute__nonnull__(1);

static size_t PIO_utf8_write( PARROT_INTERP,
    ParrotIOLayer *l /*NN*/,
    ParrotIO *io,
    STRING *s )
        __attribute__nonnull__(1)
        __attribute__nonnull__(2);

/* HEADERIZER END: static */

static const ParrotIOLayerAPI pio_utf8_layer_api = {
    PIO_null_init,
    PIO_base_new_layer,
    PIO_base_delete_layer,
    PIO_null_push_layer,
    PIO_null_pop_layer,
    PIO_null_open,
    PIO_null_open2,
    PIO_null_open3,
    PIO_null_open_async,
    PIO_null_fdopen,
    PIO_null_close,
    PIO_utf8_write,
    PIO_null_write_async,
    PIO_utf8_read,
    PIO_null_read_async,
    PIO_null_flush,
    PIO_null_peek,
    PIO_null_seek,
    PIO_null_tell,
    PIO_null_setbuf,
    PIO_null_setlinebuf,
    PIO_null_getcount,
    PIO_null_fill,
    PIO_null_eof,
    0, /* no poll */
    0, /* no socket */
    0, /* no connect */
    0, /* no send */
    0, /* no recv */
    0, /* no bind */
    0, /* no listen */
    0  /* no accept */
};

ParrotIOLayer pio_utf8_layer = {
    NULL,
    "utf8",
    0,
    &pio_utf8_layer_api,
    0, 0
};

ParrotIOLayer *
PIO_utf8_register_layer(void)
{
    return &pio_utf8_layer;
}

static size_t
PIO_utf8_read(PARROT_INTERP, ParrotIOLayer *layer, ParrotIO *io,
              STRING **buf)
{
    STRING *s, *s2;
    String_iter iter;

    size_t len = PIO_read_down(interp, layer->down, io, buf);

    s = *buf;
    s->charset  = Parrot_unicode_charset_ptr;
    s->encoding = Parrot_utf8_encoding_ptr;
    /* count chars, verify utf8 */
    Parrot_utf8_encoding_ptr->iter_init(interp, s, &iter);
    while (iter.bytepos < s->bufused) {
        if (iter.bytepos + 4 > s->bufused) {
            const utf8_t *u8ptr = (utf8_t *)((char *)s->strstart +
                    iter.bytepos);
            UINTVAL c = *u8ptr;
            if (UTF8_IS_START(c)) {
                UINTVAL len2 = UTF8SKIP(u8ptr);
                if (iter.bytepos + len2 <= s->bufused)
                    goto ok;
                /* need len-1 more chars */
                len2--;
                s2 = NULL;
                s2 = PIO_make_io_string(interp, &s2, len2);
                s2->bufused = len2;
                s2->charset  = Parrot_unicode_charset_ptr;
                s2->encoding = Parrot_utf8_encoding_ptr;
                PIO_read_down(interp, layer->down, io, &s2);
                s->strlen = iter.charpos;
                s = string_append(interp, s, s2);
                len += len2 + 1;
                /* check last char */
            }
        }
ok:
        iter.get_and_advance(interp, &iter);
    }
    s->strlen = iter.charpos;
    return len;
}

static size_t
PIO_utf8_write(PARROT_INTERP, ParrotIOLayer *l /*NN*/, ParrotIO *io, STRING *s)
{
    STRING *dest;

    if (s->encoding == Parrot_utf8_encoding_ptr)
        return PIO_write_down(interp, l->down, io, s);

    dest = Parrot_utf8_encoding_ptr->to_encoding(interp, s,
            new_string_header(interp, 0));
    return PIO_write_down(interp, l->down, io, dest);
}

/*

=head1 SEE ALSO

F<src/io/io_passdown.c>,
F<src/io/io.c>,
F<src/io/io_layers.c>,
F<src/io/io_private.h>.

=head1 HISTORY

Initially written by Leo.

*/


/*
 * Local variables:
 *   c-file-style: "parrot"
 * End:
 * vim: expandtab shiftwidth=4:
 */
