# gameboy-emu

`gameboy-emu` aims to be an efficient and portable Game Boy emulator.
Only the original Game Boy (DMG) is supported at this time.

The performance is the major consideration, and please look for an
alternative emulator if accuracy is important to you.

## Build

`gameboy-emu` relies on some 3rd party packages to be fully usable and to
provide you full access to all of its features. You need to have a working
[SDL2 library](https://www.libsdl.org/) on your target system.
* macOS: `brew install sdl2`
* Ubuntu Linux / Debian: `sudo apt install libsdl2-dev`

Build the emulator.
```shell
make
```

`build/emu` is the built executable file, and you can use it to load Game Boy
ROM files.

## Key Controls

| Action            | Keyboard   | Joypad |
|-------------------|------------|--------|
| A                 | z          | A      |
| B                 | x          | B      |
| Start             | Return     | START  |
| Select            | Backspace  | BACK   |
| D-Pad             | Arrow Keys | DPAD   |
| Normal Speed      | 1          |        |
| Turbo x2 (Hold)   | Space      |        |
| Turbo X2 (Toggle) | 2          |        |
| Turbo X3 (Toggle) | 3          |        |
| Turbo X4 (Toggle) | 4          |        |
| Reset             | r          |        |
| Change Palette    | p          |        |
| Reset Palette     | Shift + p  |        |
| Fullscreen        | F11 / f    |        |
| Frameskip (Toggle)| o          |        |
| Interlace (Toggle)| i          |        |

Frameskip and Interlaced modes are both off by default. The Frameskip toggles
between 60 FPS and 30 FPS.

## License

`gameboy-emu` is licensed under the MIT License.
