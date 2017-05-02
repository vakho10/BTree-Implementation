#pragma once

// Workaround function for calucating child's position
// As it turns out the -num1 % num2 returns negative number in C++
int modForMin(int m, int n)
{
	return m >= 0 ? m % n : ((m % n) + n) % n;
}