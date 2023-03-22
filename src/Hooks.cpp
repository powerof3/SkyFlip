#include "Hooks.h"

namespace SkyFlip
{
	struct UpdateSkyHitArt
	{
		static void thunk(RE::Sky* a_this)
		{
			func(a_this);

			if (const auto& root = a_this->root) {
				const auto tes = RE::TES::GetSingleton();
				const auto worldspace = tes ? tes->worldSpace : nullptr;

				if (worldspace && currentWorldspace != worldspace) {
					oldWorldspace = currentWorldspace;
					currentWorldspace = worldspace;

					if (is_flipped_worldspace(worldspace)) {
						flip_sky(a_this, worldspace, true);

					} else if (is_flipped_worldspace(oldWorldspace)) {
						flip_sky(a_this, worldspace, false);

					} else {
						logger::info("[{}] : Loaded", worldspace->GetFormEditorID());
					}
				}
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;

	private:
		static bool is_flipped_worldspace(const RE::TESWorldSpace* a_worldspace)
		{
			return a_worldspace && std::ranges::find(brWorldspaces, a_worldspace->GetFormEditorID()) != brWorldspaces.end();
		}

		static void flip_sky(const RE::Sky* a_sky, RE::TESWorldSpace* a_worldspace, const bool a_flip)
		{
			logger::info("[{}] : {} sky",
				a_worldspace->GetFormEditorID(), a_flip ? "Flipping" : "Reverting");

			RE::NiMatrix3 rotation{};
			if (a_flip) {
				rotation.SetEulerAnglesXYZ(0.0, 0.0, RE::NI_PI);  // 0,0,180
			}

			a_sky->root->local.rotate = rotation;

			if (elvasInstalled) {
				const auto sun = a_sky->sun;
				if (const auto sunRoot = sun ? sun->GetRoot() : nullptr) {
					sunRoot->local.rotate = rotation;
				}
			}

			RE::NiUpdateData data{};
			a_sky->root->Update(data);
		}
	};

	void Install()
	{
		logger::info("{:*^30}", "HOOKS");

		elvasInstalled = GetModuleHandle(L"EVLaS") != nullptr;
		logger::info("ELVAS : detected ({})", elvasInstalled);

		REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(25682, 26229) };
		stl::write_thunk_call<UpdateSkyHitArt>(target.address() + OFFSET(0x358, 0x668));

		logger::info("Installed sky flip hooks");

		logger::info("{:*^30}", "GAME");
	}
}
