/* -------------------------------------------------------------------- *
 * TMOPlugin.cpp : Template for tone mapping operator plugin            *
 *                 in Tone Mapping Studio 2004                          *
 * -------------------------------------------------------------------- */
#include "./TMOPlugin.h"
#include "./TMOKhan20.h"

/* -------------------------------------------------------------------- *
 * Insert a number of implemented operators                             *
 * -------------------------------------------------------------------- */
int iOperatorCount = 1;

/* -------------------------------------------------------------------- *
 * DLL Entry point; no changes necessary                                *
 * commenting out because TMOPlugin.cpp:16:1: error: ‘BOOL’ does not    *
 * name a type                                                          *
 * -------------------------------------------------------------------- */
/* BOOL APIENTRY DllMain(HANDLE hModule,
					  DWORD ul_reason_for_call,
					  LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
*/

/* -------------------------------------------------------------------- *
 * Returns a number of implemented operators; no changes necessary      *
 * -------------------------------------------------------------------- */
int TMOPLUGIN_API OperatorCount()
{
	return iOperatorCount;
}

/* -------------------------------------------------------------------- *
 * For each implemented operator create a new object in field operators,*
 * then return number of created operators                              *
 * For exemple :                                                        *
 *                                                                      *
 *               operators[0] = new TMOOperator1;                       *
 *               operators[1] = new TMOOperator2;                       *
 *               .                                                      *
 *               .                                                      *
 *               .                                                      *
 * -------------------------------------------------------------------- */
int TMOPLUGIN_API EnumOperators(TMO **operators)
{
	operators[0] = new TMOKhan20;
	return iOperatorCount;
}

/* -------------------------------------------------------------------- *
 * Deletes operators; no changes necessary                              *
 * -------------------------------------------------------------------- */
int TMOPLUGIN_API DeleteOperators(TMO **operators)
{
	int i;
	for (i = 0; i < iOperatorCount; i++)
		delete operators[i];
	return iOperatorCount;
}
