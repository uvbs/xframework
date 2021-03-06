#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "framework.h"
#include "hack.h"
#include "menu.h"
#include "game.h"
#include "render.h"
#include "chams.h"

typedef HRESULT (__thiscall *tSetScissorRect)(void*, RECT*);
typedef HRESULT (__thiscall *tEndScene)(void*, LPDIRECT3DDEVICE9);
typedef HRESULT (__thiscall *tReset)(void*, D3DPRESENT_PARAMETERS*);
typedef HRESULT (__thiscall *tPresent)(void*, RECT*, RECT*, HWND, RGNDATA*);

IDirect3DDevice9 *gameDevice;
HANDLE            hookRendererThread;
VFTManager        d3dManager;

void *oEndScene, *oReset, *oPresent;
int   frames;

void HookDevice(IDirect3DDevice9 *device);

void InitializeResources() {
	render.Initialize();
	wm.Initialize();
	InitializeFrameworkResources();
	InitializeChams();
}

void ReleaseResources() {
	wm.Release();
	render.Release();
	ReleaseFrameworkResources();
	gameDevice = NULL;
}

void UpdateDevice(IDirect3DDevice9 *device) {
	if (gameDevice != device) {
		ReleaseResources();
		gameDevice = device;
		render.SetDevice(device);
		render.vb = NULL;
		frames = 0;
	}
	if (frames++ == 30) {
		InitializeResources();
		//DEBUG_LOG("updated device %08X\n", device);
	}
}

HRESULT __fastcall hEndScene(void *thisptr, int edx, LPDIRECT3DDEVICE9 pDevice) {
	IDirect3DDevice9 *dev = *(IDirect3DDevice9**)0xCC9A408;
	//IDirect3DDevice9 *dev = (IDirect3DDevice9*)pDevice;
	
	UpdateDevice(dev);
	if (render.vb) {
		if (GetTickCount() > safetyTime) {
			render.Begin();
			UpdateCursorPosition();
			if (IsInGame()) {
				RenderHackFrame();
			}
			RenderFrameworkFrame();
			wm.Render();
			//menu.Render();
			render.End();
		}
	}
	return ((tEndScene)oEndScene)(thisptr, pDevice);
}

HRESULT __fastcall hReset(void *thisptr, int edx, D3DPRESENT_PARAMETERS *pPresentationParameters) {
	HRESULT res = ((tReset)oReset)(thisptr, pPresentationParameters);
	if (D3D_OK == res) {
		ReleaseResources();
		InitializeResources();
		//DEBUG_LOG("reset device %08X\n", thisptr);
	}
	return res;
}

HRESULT __fastcall hPresent(void *thisptr, int edx, RECT *pSourceRect, RECT *pDestRect, HWND hDestWindowOverride, RGNDATA *pDirtyRegion) {
	HRESULT res = ((tPresent)oPresent)(thisptr, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	if (res == D3DERR_DEVICELOST) {
		ReleaseResources();
		//DEBUG_LOG("lost device %08X\n", thisptr);
	}
	return res;
}

void HookDevice(IDirect3DDevice9 *device) {
	if (!CheckValidReadPtr((LPVOID)device)) {
		return;
	}
	/*
	if (!CheckValidReadPtr(*(LPVOID*)device)) {
		return;
	}
	*/
	UpdateDevice(device);
	
	d3dManager.Initialize((void***)gameDevice);
	//d3dManager.Initialize((void***)0xCC9A408);
	oEndScene = d3dManager.oldVFT[42];
	d3dManager.HookFunction(42, hEndScene);
	HookChams();
	d3dManager.Hook();
	//DEBUG_LOG("hooked device %08X from %08X to %08X\n", d3dManager.instance, d3dManager.oldVFT, d3dManager.newVFT);
}

DWORD WINAPI HookRendererThread(LPVOID) {
	while (1) {
		IDirect3DDevice9 *dev = *(IDirect3DDevice9**)0xCC9A408;
		
		Sleep(100);
		if (dev != NULL && dev != gameDevice) {
			HookDevice(dev);
		}
	}
	return 0;
}

void HookRenderer() {
	hookRendererThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HookRendererThread, 0, 0, 0);
}

void UnhookRenderer() {
	TerminateThread(hookRendererThread, 0);
	d3dManager.Unhook();
	ReleaseResources();
}
