# BongoCat OLED (keypress-based) for Mercutio
## Adam Hochberger
### Based off of logic from https://github.com/pedker/OLED-BongoCat-Revision

These files are meant to be used as an additional keymap on the base mechwild/mercutio folder within the qmk_firmware repo. I found in my testing that the Tap frame would quickly be overwritten by the subsequent Prep frame and would appear as if the Tap was not happening at all. Subsequently, the only change that would need to be made outside of these files would be adding `#define OLED_UPDATE_INTERVAL 66` within the `config.h` of that folder so that the cat can tap properly. 

TODO: Refactor out logic a bit and extend functionality (clock, layer display)
