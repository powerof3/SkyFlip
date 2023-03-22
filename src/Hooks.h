#pragma once

namespace SkyFlip
{
	inline constexpr std::array brWorldspaces{ "arnimaDUPLICATE003"sv, "Arnimatown"sv, "arnimaEvermore"sv, "ArnimaJehanna"sv, "ArnimaJehannaSlums"sv };

	inline bool elvasInstalled{ false };

	inline RE::TESWorldSpace* currentWorldspace{ nullptr };
	inline RE::TESWorldSpace* oldWorldspace{ nullptr };

	void Install();
}
