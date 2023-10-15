#include "lib.hpp"
#include "hooks_slink.hpp"
#include "config.hpp"
#include "hooks.hpp"
#include "xlink.hpp"
#include "utils.hpp"
#include "binaryoffsethelper.hpp"

extern "C" void exl_main(void* x0, void* x1)
{
	// Init Hooking system
	exl::hook::Initialize();

	char buf[500];
	PRINT("CSM: Loading Custom SLink Mod...");

	/* Initialize app version */
	int versionIndex = csm::helpers::InitializeAppVersion();
	if (versionIndex == 0xffff'ffff)
	{
		PRINT("CSM: Incompatible game version!");
		return;
	}
	PRINT("CSM: Version Index %d", csm::helpers::GetAppVersionIndex());

	// Install Function hooks
	PRINT("CSM: Hooking Functions...");
	csm::hooks::searchAssetCallTableByName::InstallAtOffset(
		csm::helpers::GetAppVersionOffset(s_searchAssetCallTableByNameOffsets));

	csm::hooks::searchAssetCallTableByNameInlinedOne::InstallAtOffset(
		csm::helpers::GetAppVersionOffset(s_searchAssetCallTableByNameInlinedOneOffsets));


	// SLink System vtable hooking
	PRINT("CSM: Hooking SLink...");
	auto ptr = csm::hooks_slink::GetHookFuncPtr();
	csm::hooks_slink::s_OriginalHookedFunc = *ptr;
	*ptr = csm::hooks_slink::SLinkHookCallback;

	return;
}