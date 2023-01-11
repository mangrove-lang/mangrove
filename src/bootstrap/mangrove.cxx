// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/console>

using substrate::console;

int main(int, char **)
{
	console = {stdout, stderr};
	return 0;
}
