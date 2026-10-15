# Hexius (13.4 edition)

## Features

- **Full lawn (wide view)** toggle in Settings - see more lawn. (add widescreen fix yourself tho)
- Custom tab in settings and it's customizable!
- Camera stays away from lawnmowers on the left side when starting a match

---

# Build

Uses **CMake**.

Just run `build.bat`. If it explodes, you're probably missing:

- Android NDK **r25+**
- CMake **3.18+**
- Ninja
- Patience

Output: `libHexius.so`

---

# Usage

1. Yeet `libHexius.so` into your APK's `lib/arm64-v8a/` folder (make sure to delete armeabi-v7a)
2. Paste this code in `smali_classes5\com\popcap\PvZ2\PvZ2GameActivity`
   ```
   .line 57
    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

    const-string v0, "Hexius"
   ```
3. Launch the game
4. Enable "Full lawn (wide view)" in Settings

---

# Target

PvZ2 **13.4** (ARM64).

Got a different version? Cool. Update `offsets.h` yourself. good luck

---

# To-do list

- None, check out [9.6.1 Soggylib repository](https://github.com/aasu-rel/Soggylib) instead

---

# Credits

- **[Soggylib](https://github.com/aasu-rel/Soggylib)** - Me obviously (base library)
- **[And64InlineHook](https://github.com/rprop/and64inlinehook)** - Rprop
- **[Blazey's Example Mod](https://github.com/BlazeyLol/PVZ2ExpansionMod)** - Original repo for 9.6.1 libbing
- **[Plants vs Zombie discord (not offical one)](https://discord.gg/pvz)** - Awesome community
- **[Original Full lawn tab](https://github.com/CongJian833/PvZ2-LawnZoomTab)** - Original repo for full lawn from 9.8.1 [video](https://www.bilibili.com/video/BV1iNbX6DEy7/)
- **[Renojackson's Awesome lib](https://github.com/RenoJson/ARM64_example_injection_for_PvZ2/)** - Bunches of cool stuff for ARM64 lib (9.6.1)

---

# License

MIT. Do whatever, just don't blame me.

PvZ2 belongs to PopCap / EA. Not affiliated, not endorsed, not sued (yet). No game assets included — go play the game.
