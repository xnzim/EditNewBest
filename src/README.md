# Move/Scale New Best! Source

This folder holds the implementation for the Move/Scale New Best! Geometry Dash mod.

## Entry Point: `main.cpp`
- Hooks `PlayLayer::showNewBest` to capture the moment a new record is displayed.
- Locates the popup container by scanning the play layer''s child hierarchy.
- Applies user-selected tweaks (visibility, position, scale, lifetime) retrieved from Geode settings.
- Runs adjustments on the main thread to keep cocos2d animations stable.

## Key Behaviours
- **Position Presets:** Aligns the popup to the selected screen corner with a safe margin.
- **Custom Coordinates:** When the Custom preset is active, uses the stored X/Y values without extra offsets.
- **Scale & Duration:** Applies scale immediately and schedules a shrink/remove sequence after the chosen delay.
- **Hide Banner:** Skips all animation logic when the `hide-popup` flag is enabled.

## Extending the Mod
- Add new presets by extending the `popup-position` options in `mod.json` and mirroring them in the hook logic.
- Chain additional animations onto the `CCSequence` before `CCRemoveSelf::create()`.
- Wrap calls to `Mod::get()->getSettingValue` if you want per-profile defaults or validation.

## Debug Tips
- Enable the Geode console (`Show Platform Console`) to stream log output while tweaking positions.
- Temporarily comment out `CCRemoveSelf::create()` if you need the popup to persist for layout debugging.
- Call `newBestContainer->setOpacity(...)` or `setColor(...)` to experiment with visual themes.

Happy modding!
