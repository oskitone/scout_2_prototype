# scout_2_prototype

![scout_2_prototype](/misc/s2p.jpg)

This is a prototype around a PCB I sent off to fab in March 2022... that I'm slowly bringing back to life. I'm kind of using it as a springboard for the next thing, whatever that is and whenever it happens!

Very much work-in-progress. License will be MIT like other kits.

Forked from scout's [2-25_keys](https://github.com/oskitone/scout/tree/2-25_keys) branch

## In this repo

![top view](/misc/model-top.png)

- Kicad project
- OpenSCAD for scaffolding and keys models
- STLs for 3D-printing

## Goals

### Overarching Oskitone design goals

- Easy to solder. Only through-hole parts, even if it limits functionality.
- Easy to change. Code can be edited with free software and cheap hardware.
- Easy to play. No hidden menus, secret key combos, or overloading. One control per one function.
- Easy to understand. Code should be relatively accessible to anybody familiar with Arduino.
- Easy to hack. Programming header (TODO? hmm), usual open source ethos stuff.

### And specific to the next ~synth

- Polyphony
  - Per my ability, four is a reasonable number of notes to play simultaneously. Three is okay but would be frustrating. Two would be funny?
  - Anyway, four notes is what D9-12 are attempting. At the time, I was thinking I could tack some math around millis() and make the square waves manually... maybe use an interrupt timer? My pin choice seems random, and there're probably better choices.
- Any kind of musical connectivity
  - MIDI
    - USB is modern but circular DIN connectors are standard and inarguably answer the question "But can it do MIDI?!"
    - I think I'd also need non-DIP chips to do USB MIDI so that's out
    - I don't know if both IN and OUT are necessary or which to prefer if there can only be one. DIN connectors are big; [TRS MIDI](https://minimidi.world/) is an option.
  - Control Voltage
    - I'm open to this but don't really know the scene well enough to know what would be good. If it's easy to add, sure.
- Support for Mozzi w/o shipping w/ it
  - I want it to still feel solidly like an Oskitone instrument, for better and no doubt worse. Blame ["not invented here" syndrome](https://en.wikipedia.org/wiki/Not_invented_here).
  - Mozzi is cool as hell but it does require reading docs, which maybe breaks the "easy to understand" goal.
  - <del>Plus, even if I wanted to, I don't think Mozzi's license would permit me selling kits w/ it.</del> [Looks like they're GNU LGPL now](https://github.com/sensorium/Mozzi/pull/240), so may be okay.
  - That having been said, I want to engender "side-loading" things like this and make swapping to Mozzi straightforward, especially if the barrier to entry is low. So minimally it needs the right pins, an output filter, maybe other changes too.
- More musically useful than the Scout
  - Same size keys but as many as I can fit on a Prusa.
  - A semblance of control with analog input pots. Currently doing octave and glide.
  - Glide is fun but will be weird if/when polyphony is introduced. Could do poly only at 0 glide

## TODO / known issues / things for next rev

- 3D prints
  - Octave control really needs detents or some kinda tactile feedback on rotation.
- For next rev
  - Needs decoupling cap before amp, probably
  - Self-document pin connection assignments in schematic
  - Move text beyond key area
  - More key mounting screw holes? Key bed subassembly with multiple parts
  - Reverse power switch direction. It should power on when actuator is "up" like Scout
  - Note UART hookup orientation on silkscreen
- Needs a bigger speaker for bass!

Anything else?

https://github.com/user-attachments/assets/99f4f9d2-c58f-4821-b225-9032d2b2e849

## Uploading new code

The UART header [works the same as the OG Scout](https://oskitone.github.io/scout/change-the-arduino-code).

The cable's black wire should be on the left and green on the right.

## BOM

| Designator           | Footprint                                         | Quantity | Designation        |
| -------------------- | ------------------------------------------------- | -------- | ------------------ |
| SW1-SW25             | SW_PUSH_6mm                                       | 25       | SPST               |
| BT1                  | PinHeader_1x02_P2.54mm_Vertical                   | 1        | 4.5v               |
| LS1                  | PinHeader_1x02_P2.54mm_Vertical                   | 1        | Speaker            |
| RV1                  | Potentiometer_Bourns_PTV09A-1_Single_Vertical     | 1        | 10k Log            |
| Y1                   | Resonator-3Pin_W7.0mm_H2.5mm                      | 1        | 16.00MHz           |
| U2                   | DIP-8_W7.62mm                                     | 1        | LM386              |
| R4,R5,R7,R6          | R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal | 4        | 10K                |
| J1                   | AUDIO-JACK                                        | 1        | AudioJack2_SwitchT |
| C2,C4,C5             | C_Disc_D5.0mm_W2.5mm_P5.00mm                      | 3        | .1uF               |
| C6,C1                | CP_Radial_D8.0mm_P3.50mm                          | 2        | 220uF              |
| U1                   | DIP-28_W7.62mm                                    | 1        | ATmega328P-PU      |
| D1-D25,D27           | D_DO-41_SOD81_P10.16mm_Horizontal                 | 26       | D                  |
| J2                   | PinHeader_1x06_P2.54mm_Horizontal                 | 1        | Conn_01x06_Male    |
| R8                   | R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal | 1        | 1m                 |
| R9,R13               | R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal | 2        | 10k                |
| R10,R1,R11,R3,R2,R12 | R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal | 6        | 220                |
| D26                  | LED_D5.0mm-4_RGB_Staggered_Pins                   | 1        | LED_CRGB           |
| J4,J3                | DIN5-RA-PTH                                       | 2        | DIN5               |
| SW26                 | SW_Slide_1P2T_CK_OS102011MS2Q                     | 1        | SW_SPST            |
| RV2,RV3              | Potentiometer_Bourns_PTV09A-1_Single_Vertical     | 2        | 10k                |
| C3                   | CP_Radial_D5.0mm_P2.00mm                          | 1        | 1uF                |
| U3                   | DIP-8_W7.62mm                                     | 1        | 6N138              |

## Schematics

![scout_2_prototype-schematic](/kicad/scout_2_prototype-schematic.svg)

![scout_2_prototype-keyboard_matrix-schematic](/kicad/scout_2_prototype-keyboard_matrix-schematic.svg)

## 3D Models

Normally I'd put them on thingiverse/etc, but here they're included in the repo in the `stls` folder.

### OpenSCAD dependencies

Assumes `parts_cafe` is in a sibling directory and is up to date on the `main` branch. Here's how I've got it:

    \ oskitone
        \ parts_cafe
        \ scout_2_prototype

You'll also need to install the [Orbitron](https://fonts.google.com/specimen/Orbitron) font.
