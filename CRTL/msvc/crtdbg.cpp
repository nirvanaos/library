extern "C" {

	int __cdecl _CrtDbgReportW (
		int            _ReportType,
		wchar_t const* _FileName,
		int            _LineNumber,
		wchar_t const* _ModuleName,
		wchar_t const* _Format,
		...) {
		// Temporary does nothing
		return 0;
	}

	int __cdecl _CrtDbgReport (
		int         _ReportType,
		char const* _FileName,
		int         _Linenumber,
		char const* _ModuleName,
		char const* _Format,
		...) {
		// Temporary does nothing
		return 0;
	}

}
