#pragma once
#include "lua.hpp"
#include "sdl.hpp"

struct BlendModeWrapper : GPU_BlendMode{
	int GetSC()const;
	int GetDC()const;
	int GetSA()const;
	int GetDA()const;
	int GetCE()const;
	int GetAE()const;
	void SetSC(int);
	void SetDC(int);
	void SetSA(int);
	void SetDA(int);
	void SetCE(int);
	void SetAE(int);
};

struct Texture : GPU_Image{
	uint16_t GetW() const;
	uint16_t GetH() const;
	BlendModeWrapper GetBlendMode() const;
	void SetBlendMode(BlendModeWrapper);
	int GetBytesPerPixel() const;
	int GetFilterMode() const;
	void SetFilterMode(int);
	bool HasMipmaps() const;
	bool IsAlias() const;
	int GetNumLayers() const;
	int GetRefCount() const;
	int GetSnapMode() const;
	void SetSnapMode(int);
	uint16_t GetTexW() const;
	uint16_t GetTexH() const;
	bool UseBlending() const;
	void SetBlending(bool);
	bool UsingVirtualResolution() const;
	int GetWrapModeX() const;
	int GetWrapModeY() const;
	void SetWrapModeX(int);
	void SetWrapModeY(int);
	float GetAnchorX() const;
	float GetAnchorY() const;
	void SetAnchorX(float);
	void SetAnchorY(float);
};

void bind_graphics();
