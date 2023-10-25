extern "C" long long __cdecl llabs (long long const number)
{
	return number >= 0 ? number : -number;
}
