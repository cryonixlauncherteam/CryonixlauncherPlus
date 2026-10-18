<div align="center">

<img src="app_mojo/src/main/assets/mojo.png" alt="Cryonix Launcher Plus logo" width="112" />

# ❄️ CRYONIX LAUNCHER PLUS

<a href="https://github.com/cryonixlauncherteam/CryonixlauncherPlus">
  <img src="https://readme-typing-svg.demolab.com?font=JetBrains+Mono&weight=700&size=22&pause=1000&color=36A3FF&center=true&vCenter=true&width=620&lines=Your+Minecraft.+Your+World.;A+new+way+to+play+Java+Edition.;Built+for+Android.+Made+for+players." alt="Animated Cryonix Launcher Plus tagline" />
</a>

**A Minecraft: Java Edition launcher for Android — reimagined as Cryonix Launcher Plus.**

<p>
  <a href="https://github.com/cryonixlauncherteam/CryonixlauncherPlus/releases">
    <img src="https://img.shields.io/github/downloads/cryonixlauncherteam/CryonixlauncherPlus/total?style=for-the-badge&label=TOTAL%20RELEASE%20DOWNLOADS&color=168BFF" alt="Total GitHub release downloads" />
  </a>
  <a href="https://github.com/cryonixlauncherteam/CryonixlauncherPlus/stargazers">
    <img src="https://img.shields.io/github/stars/cryonixlauncherteam/CryonixlauncherPlus?style=for-the-badge&color=7C6CFF" alt="GitHub stars" />
  </a>
  <a href="https://github.com/cryonixlauncherteam/CryonixlauncherPlus/commits/main">
    <img src="https://img.shields.io/github/last-commit/cryonixlauncherteam/CryonixlauncherPlus?style=for-the-badge&color=303846" alt="Last commit" />
  </a>
</p>

<p>
  <a href="https://github.com/cryonixlauncherteam/CryonixlauncherPlus/releases">Get Releases</a> ·
  <a href="https://github.com/cryonixlauncherteam/CryonixlauncherPlus/issues">Report a Bug</a> ·
  <a href="https://github.com/cryonixlauncherteam/CryonixlauncherPlus">Source Code</a>
</p>

</div>

---

## ✦ About Cryonix Launcher Plus

Cryonix Launcher Plus is an Android launcher project based on [MojoLauncher](https://github.com/MojoLauncher/MojoLauncher), itself based on [PojavLauncher](https://github.com/PojavLauncherTeam/PojavLauncher). The project aims to make Minecraft: Java Edition accessible on Android with a distinct Cryonix identity and a clean, practical launcher experience.

### What it aims to support

- Minecraft: Java Edition on compatible Android devices
- Multiple Minecraft versions and instances
- Mod-loader workflows, including Fabric and Forge where supported
- A custom Cryonix Launcher Plus interface and launcher branding
- Ongoing improvements to usability, compatibility and performance

*Available features depend on the current build, device, Minecraft version and required components.*

## 📥 Downloads

<div align="center">

[![Latest Release](https://img.shields.io/github/v/release/cryonixlauncherteam/CryonixlauncherPlus?style=for-the-badge&label=LATEST%20RELEASE&color=168BFF)](https://github.com/cryonixlauncherteam/CryonixlauncherPlus/releases/latest)
[![Total Downloads](https://img.shields.io/github/downloads/cryonixlauncherteam/CryonixlauncherPlus/total?style=for-the-badge&label=TOTAL%20DOWNLOADS&color=7C6CFF)](https://github.com/cryonixlauncherteam/CryonixlauncherPlus/releases)

</div>

1. Open the [Releases page](https://github.com/cryonixlauncherteam/CryonixlauncherPlus/releases).
2. Choose a published release and download its Android APK asset, if available.
3. Install only builds you trust, and make sure your device meets the build's requirements.

**About the download counter:** The badge updates automatically from downloads of files attached to GitHub Releases. This repository currently has no published releases, so the count may show **0** until release assets are published and downloaded. GitHub does not include ordinary source-code ZIP downloads in this release-asset counter.

## 🛠️ Build from source

You will need a compatible JDK, Android SDK and the Android build tools required by the project.

Clone the repository:

```bash
git clone https://github.com/cryonixlauncherteam/CryonixlauncherPlus.git
cd CryonixlauncherPlus
```

Build the debug APK:

```bash
./gradlew :app_mojo:assembleDebug
```

On Windows, use:

```bat
gradlew.bat :app_mojo:assembleDebug
```

The generated APK is normally located under the module's `build/outputs/apk/` directory. Build requirements may change as the project evolves.

## 🧭 Project direction

- [ ] Continue refining the Cryonix Launcher Plus visual identity
- [ ] Improve the landscape mobile interface and navigation
- [ ] Polish button feedback and Motion Bounce animations
- [ ] Improve instance-management and launcher-screen usability
- [ ] Keep existing launch, account and game-runtime functionality stable

This list describes project goals, not a guarantee that every item is already implemented.

## 🐛 Issues and contributions

Found a bug or have an idea? Please [open an issue](https://github.com/cryonixlauncherteam/CryonixlauncherPlus/issues) with clear reproduction steps, device details and relevant logs. Contributions should be submitted as pull requests with a description of the change and how it was tested.

## 📜 License and acknowledgements

Cryonix Launcher Plus builds on open-source work. Please review the original licenses and preserve their notices when modifying or redistributing the project.

- [MojoLauncher](https://github.com/MojoLauncher/MojoLauncher)
- [PojavLauncher](https://github.com/PojavLauncherTeam/PojavLauncher) — GNU LGPLv3
- [Android Support Libraries](https://android.googlesource.com/platform/prebuilts/maven_repo/android/+/master/NOTICE.txt) — Apache License 2.0
- [Holy GL4ES](https://github.com/artdeell/gl4es_extra_extra/) — MIT
- [OpenJDK multiarch](https://github.com/PojavLauncherTeam/openjdk-multiarch-jdk8u)
- [GLFW](https://github.com/MojoLauncher/glfw) — zlib
- [SDL](https://github.com/MojoLauncher/MojoSDL) — zlib
- [LWJGL](https://github.com/LWJGL/lwjgl3) — BSD-3-Clause
- [mojoexec](https://github.com/MojoLauncher/mojoexec) — MIT
- [Mesa 3D](https://docs.mesa3d.org/license.html)
- [pro-grade](https://github.com/pro-grade/pro-grade) — Apache License 2.0
- [bhook](https://github.com/bytedance/bhook) — MIT
- [Authlib-Injector](https://github.com/yushijinhun/authlib-injector)
- [OpenAL Soft](https://github.com/kcat/openal-soft/)
- [Oboe](https://github.com/google/oboe) — Apache License 2.0

Minecraft is a trademark of Microsoft/Mojang. Cryonix Launcher Plus is an independent project and is not affiliated with or endorsed by Microsoft or Mojang. Refer to each upstream component for its full license and attribution requirements.

---

<div align="center">

**CRYONIX LAUNCHER PLUS** · Play your way.

<sub>Made with care for the Minecraft community.</sub>

</div>
