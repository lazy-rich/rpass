/*
 * Copyright (c) 2020 Richard Bradshaw
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <bsd/stdlib.h>

#include "rand.h"

#define MINLEN		20	/* minimum password length */
#define MAXLEN		1024	/* maximum password length */
#define NSET		4	/* the total number of sets */
#define UPPERSETSTART	65	/* start of the upper case ascii set */
#define UPPERSETSZ	26	/* size of the upper case ascii set */
#define LOWERSETSTART	97	/* start of the lower case ascii set */
#define LOWERSETSZ	26	/* size of the lower case ascii set */
#define NUMBERSETSTART	48	/* start of the numeric ascii set */
#define NUMBERSETSZ	10	/* size of the numeric ascii set */
#define PUNCSETSTART	33	/* start of the punctuation ascii set */
#define PUNCSETSZ	15	/* size of the punctuation ascii set */

/* generate a random character in each set */
static char upper();
static char lower();
static char num();
static char punc();

static void usage(void);
static int add_set(char (*[])(), char (*)(void), int);
static unsigned long xstrtoul(const char *);

static char
upper()
{
	return arc4random_uniform(UPPERSETSZ) + UPPERSETSTART;
}

static char
lower()
{
	return arc4random_uniform(LOWERSETSZ) + LOWERSETSTART;
}

static char
num()
{
	return arc4random_uniform(NUMBERSETSZ) + NUMBERSETSTART;
}

static char
punc()
{
	return arc4random_uniform(PUNCSETSZ) + PUNCSETSTART;
}

static void
usage()
{
	fprintf(stderr, "usage: rpass, generate a password\n");
	fprintf(stderr, "-a use uppercase, lowercase, numeric and punctuation characters\n");
	fprintf(stderr, "-u -l -n -p use uppercase, lowercase, numeric and punctuation characters\n");
	fprintf(stderr, "-c <number> length of the password\n");
	exit(EXIT_FAILURE);
}

int
add_set(char (*set[])(), char (*func)(void), int index)
{
	if (index < 0 || index >= NSET)
		errx(1, "add_set: index %d is out of range", index);

	set[index] = func;

	return ++index;
}

unsigned long
xstrtoul(const char *str)
{
	char *endptr;
	unsigned long val;

	if (str == NULL)
		errx(1, "xstrtoul: invalid string");
	errno = 0;
	val = strtoul(str, &endptr, 10);
	if ((errno == ERANGE && (val == ULONG_MAX))
			|| (errno != 0 && val == 0))
		errx(1, "xstrtoul: value error: %s", str);

	return val;
}

int
main(int argc, char **argv)
{
	int c, wanted;
	unsigned long count = 0;
	char (*set[NSET])();

	wanted = 0;

	while ((c = getopt(argc, argv, "ac:lnpu")) != -1) {
		switch (c) {
		case 'a':
			wanted = add_set(set, lower, wanted);
			wanted = add_set(set, num, wanted);
			wanted = add_set(set, punc, wanted);
			wanted = add_set(set, upper, wanted);
			break;
		case 'c':
			count = xstrtoul(optarg);
			if (count < MINLEN) {
				errx(1, "Count must be >= %i", MINLEN);
			} else if (count > MAXLEN) {
				errx(1, "Count must be <= %i", MAXLEN);
			}
			break;
		case 'l':
			wanted = add_set(set, lower, wanted);
			break;
		case 'n':
			wanted = add_set(set, num, wanted);
			break;
		case 'p':
			wanted = add_set(set, punc, wanted);
			break;
		case 'u':
			wanted = add_set(set, upper, wanted);
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (wanted == 0)
		usage();

	if (count == 0)
		count = MINLEN;
	
	for (size_t i = 0; i < count; ++i) {
		unsigned int s = wanted == 1 ? 0 : arc4random_uniform(wanted);
		putchar(set[s]());
	}

	putchar('\n');

	return 0;
}
