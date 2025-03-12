# scout_2_prototype

This is a PCB I sent off to fab in TODO_DATE. I'm kind of using it as a springboard for the next thing, whatever that is and whenever it happens!

So, yes, very much work-in-progress. License will be MIT like other kits.

## Schematics

![scout_2_prototype-schematic](/kicad/scout_2_prototype-schematic.svg)

![scout_2_prototype-keyboard_matrix-schematic](/kicad/scout_2_prototype-keyboard_matrix-schematic.svg)

## In this repo

TODO

- forked from LINK
- STLs in FOLDER

## Goals

- Design goals
  - Easy to solder. Only through-hole parts, even if it limits functionality.
  - Easy to change. Code can be edited with free software and cheap hardware.
  - Easy to play. No hidden menus, secret key combos, or overloading. One control per one function.
  - Easy to understand, code should be relatively accessible to anybody familiar with Arduino.
  - Easy to hack. Programming header, usual open source ethos stuff.
- Polyphony
  - Per my ability, four is a reasonable number of notes to play simultaneously. Three is okay but would be frustrating. Two would be funny?
  - Anyway, four notes is what D9-12 are attempting. At the time, I was thinking I could tack some math around millis() and make the square waves manually... maybe use an interrupt timer? My pin choice seems random, and there're probably better choices.
- Any kind of musical connectivity
  - MIDI
    - USB is modern but circular DIN connectors are standard and inarguably answer the question "But can it do MIDI?!"
    - I think I'd also need non-DIP chips to do USB MIDI so that's out
    - I don't if both IN and OUT are necessary or which to prefer if there can only be one. DIN connectors are big; [TRS MIDI](https://minimidi.world/) is an option.
  - Control Voltage
    - I'm open to this but don't really know the scene well enough to know what would be good. If it's easy to add, sure.
- Support for Mozzi w/o shipping w/ it
  - I want it to still feel solidly like an Oskitone instrument, for better and no doubt worse. Blame ["not invented here" syndrome](https://en.wikipedia.org/wiki/Not_invented_here).
  - Mozzi is cool as hell but it does require reading docs, which maybe breaks the "easy to understand" goal.
  - <del>Plus, even if I wanted to, I don't think Mozzi's license would permit me selling kits w/ it.</del> [Looks like they're GNU LGPL now](https://github.com/sensorium/Mozzi/pull/240), so may sbe okay.
  - That having been said, I want to engender "side-loading" things like this and make swapping to Mozzi straightforward, especially if the barrier to entry is low. So minimally it needs the right pins, an output filter, maybe other changes too.
- More musically useful than the Scout
  - Same size keys but as many as I can fit on a Prusa.
  - A semblance of control with analog input pots. Currently doing octave and glide.
  - Glide is fun but will be weird if/when polyphony is introduced. Could do poly only at 0 glide

## TODO / known issues / things for next rev

- 3D prints
  - Octave control really needs detents or some kinda tactile feedback on rotation.
- For current firmware
  - RGB LED's red is unused
  - Mute or use unused output pins D10-D12
- For next rev
  - Needs decoupling cap before amp, probably
  - Header pin connection assignments w/ 2x connectors for defaults (TODO: what'd I mean by this)
  - Move text beyond key area
  - More key mounting screw holes? Key bed subassembly with multiple parts
- Power switch is reversed. It should power on when actuator is "up" like Scout
- Needs a bigger speaker for bass!
