#include "interfaces.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void interfaces::Setup() noexcept
{
	// capture the interfaces
	client = Capture<IBaseClientDLL>("client.dll", "VClient018");
	entityList = Capture<IClientEntityList>("client.dll", "VClientEntityList003");
	clientMode = **reinterpret_cast<IClientModeShared***>((*reinterpret_cast<unsigned int**>(client))[10] + 5);
	engine = Capture<IEngineClient>("engine.dll", "VEngineClient014");
	globals = **reinterpret_cast<IGlobalVars***>((*reinterpret_cast<uintptr_t**>(client))[11] + 10);
	materialSystem = Capture<IMaterialSystem>("materialsystem.dll", "VMaterialSystem080");
	studioRender = Capture<IStudioRender>("studiorender.dll", "VStudioRender026");

	// get the exported KeyValuesSystem function
	if (const HINSTANCE handle = GetModuleHandle("vstdlib.dll"))
		// set our pointer by calling the function
		keyValuesSystem = reinterpret_cast<void* (__cdecl*)()>(GetProcAddress(handle, "KeyValuesSystem"))();
}

template <typename Interface>
Interface* interfaces::Capture(const char* moduleName, const char* interfaceName) noexcept
{
	const HINSTANCE handle = GetModuleHandle(moduleName);

	if (!handle)
		return nullptr;

	// get the exported Createinterface function
	using CreateInterfaceFn = Interface*(__cdecl*)(const char*, int*);
	const CreateInterfaceFn createInterface =
		reinterpret_cast<CreateInterfaceFn>(GetProcAddress(handle, "CreateInterface"));

	// return the interface pointer by calling the function
	return createInterface(interfaceName, nullptr);
}
