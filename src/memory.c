/*
Copyright (C) 2001-2006, The Perl Foundation.
$Id$

=head1 NAME

src/memory.c - Memory allocation

=head1 DESCRIPTION

The memory (mem) API handles memory allocation,

Basically just a wrapper C<around malloc/calloc/realloc/free()> with an
setup function to initialize the memory pools.

=head2 Functions

=over 4

=cut

*/

#include "parrot/parrot.h"

/* for PANIC */
#define interpreter NULL

/*

=item C<void *
mem_sys_allocate(size_t size)>

Uses C<malloc> to allocate system memory.

=cut

*/

void *
mem_sys_allocate(size_t size)
{
    void * const ptr = malloc((size_t)size);
#ifdef DETAIL_MEMORY_DEBUG
    fprintf(stderr, "Allocated %i at %p\n", size, ptr);
#endif
    if (!ptr)
        PANIC("Out of mem");
    return ptr;
}

void *
mem__internal_allocate(size_t size, const char *file, int line)
{
    void * const ptr = malloc((size_t)size);
#ifdef DETAIL_MEMORY_DEBUG
    fprintf(stderr, "Internal malloc %i at %p (%s/%d)\n", size, ptr, file, line);
#endif
    if (!ptr)
        PANIC("Out of mem");
    return ptr;
}

/*

=item C<void *
mem_sys_allocate_zeroed(size_t size)>

Uses C<calloc> to allocate system memory.

=cut

*/

void *
mem_sys_allocate_zeroed(size_t size)
{
    void * const ptr = calloc(1, (size_t)size);
#ifdef DETAIL_MEMORY_DEBUG
    fprintf(stderr, "Allocated %i at %p\n", size, ptr);
#endif
    if (!ptr)
        PANIC("Out of mem");
    return ptr;
}

void *
mem__internal_allocate_zeroed(size_t size, const char *file, int line)
{
    void * const ptr = calloc(1, (size_t)size);
#ifdef DETAIL_MEMORY_DEBUG
    fprintf(stderr, "Internal malloc %i at %p (%s/%d)\n", size, ptr, file, line);
#endif
    if (!ptr)
        PANIC("Out of mem");
    return ptr;
}

/*

=item C<void *
mem_sys_realloc(void *from, size_t size)>

Resize a chunk of system memory.

=cut

*/

void *
mem__sys_realloc(void *from, size_t size)
{
    void *ptr;
#ifdef DETAIL_MEMORY_DEBUG
    fprintf(stderr, "Freed %p (realloc -- %i bytes)\n", from, size);
#endif
    ptr = realloc(from, size);
#ifdef DETAIL_MEMORY_DEBUG
    fprintf(stderr, "Allocated %i at %p\n", size, ptr);
#endif
    if (!ptr)
         PANIC("Out of mem");
    return ptr;
}

void *
mem__internal_realloc(void *from, size_t size, const char *file, int line)
{
    void * const ptr = realloc(from, size);
#ifdef DETAIL_MEMORY_DEBUG
    fprintf(stderr, "internal free of %p (realloc -- %i bytes) (%s/%d)\n", from, size, file, line);
    fprintf(stderr, "Internal malloc %i at %p (%s/%d)\n", size, ptr, file, line);
#endif
    if (!ptr)
        PANIC("Out of mem");
    return ptr;
}
#undef interpreter

/*

=item C<void
mem_sys_free(void *from)>

Free a chunk of memory back to the system.

=cut

*/

void
mem_sys_free(void *from)
{
#ifdef DETAIL_MEMORY_DEBUG
    fprintf(stderr, "Freed %p\n", from);
#endif
    if (from)
	free(from);
}

void
mem__internal_free(void *from, const char *file, int line)
{
#ifdef DETAIL_MEMORY_DEBUG
    fprintf(stderr, "Internal free of %p (%s/%d)\n", from, file, line);
#endif
    free(from);
}

/*

=item C<void
mem_setup_allocator(Interp *interpreter)>

Initializes the allocator.

=cut

*/

void
mem_setup_allocator(Interp *interpreter)
{
    interpreter->arena_base = mem_sys_allocate_zeroed(sizeof(struct Arenas));
    SET_NULL(interpreter->arena_base->sized_header_pools);

#if PARROT_GC_MS
    Parrot_gc_ms_init(interpreter);
#endif
#if PARROT_GC_IMS
    Parrot_gc_ims_init(interpreter);
#endif
#if PARROT_GC_GMS
    Parrot_gc_gms_init(interpreter);
#endif

    Parrot_initialize_memory_pools(interpreter);
    Parrot_initialize_header_pools(interpreter);

}

/*

=back

=cut

*/

/*
 * Local variables:
 * c-indentation-style: bsd
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 *
 * vim: expandtab shiftwidth=4:
*/
