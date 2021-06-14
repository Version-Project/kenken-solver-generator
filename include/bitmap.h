#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <stdlib.h>
#include <stdbool.h>

/**
 * \file bitmap.h
 * \brief Contains all the functions used to manage bitmaps.
 */

/**
 * \typedef bitmap_t
 * \brief Corresponding to a 64-bits length binary number, used to store
 * current possibilities of a square.
 */
typedef unsigned long long bitmap_t;

/**
 * \fn bitmap_t set_bit(bitmap_t n, int p)
 * \brief Sets the bit at position p in the number n.
 *
 * This function and unset_bit() has been inspired by
 * <a href="https://www.geeksforgeeks.org/modify-bit-given-position/">
 * this article</a> from GeeksforGeeks website.
 * \param n binary number to modify.
 * \param p bit position.
 * \return the modified bitmap.
 */
bitmap_t set_bit(bitmap_t n, int p);

/**
 * \fn bitmap_t unset_bit(bitmap_t n, int p)
 * \brief Unsets the bit at position p in the number n.
 * \param n binary number to modify.
 * \param p bit position.
 * \return the modified bitmap.
 */
bitmap_t unset_bit(bitmap_t n, int p);

/**
 * \fn size_t get_rightmost_set_bit(bitmap_t n)
 * \brief Returns the position of the first set bit from the right.
 * \param n bitmap from which we want to find the position of the rightmost
 * set bit.
 * \return the position of the rightmost set bit, or 0 if there is no set bit
 * in the binary number.
**/
size_t get_rightmost_set_bit(bitmap_t n);

/**
 * \fn size_t get_set_bits_number(bitmap_t n)
 * \brief Gets the number of set bits in the number n.
 * \param n binary number.
 * \return the number of set bits.
**/
size_t get_set_bits_number(bitmap_t n);

#endif /* _BITMAP_H_ */
