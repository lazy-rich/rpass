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
#include "rand.h"

#define RANDFILE	"/dev/urandom"

int
get_rand_uint32_t(uint32_t *res)
{
	int fd;
	uint32_t val;

	fd = open(RANDFILE, O_RDONLY);
	if (fd == -1) {
		warn("get_rand_uint32_t: %s", RANDFILE);
		return -1;
	}
	const ssize_t rc = read(fd, &val, sizeof(uint32_t));
       	if (rc == -1) {
		warn("get_rand_uint32_t: %s", RANDFILE);
		close(fd);
		return -1;
	} else if (rc < (ssize_t)sizeof(uint32_t)) {
		warnx("get_rand_uint32_t, short read: %s", RANDFILE);
		close(fd);
		return -1;
	}
	close(fd);
	*res = val;

	return 0;
}
