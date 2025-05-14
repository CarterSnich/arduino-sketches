# Picotchi

Fork of Tamaguino

This project is forked from [niubit's code](https://github.com/niubit/piconsole/tree/master/soft/Tamaguino), originally based on [Alojz Jakob's Tamaguino](https://github.com/alojzjakob/Tamaguino) ([jakobdesign.com](https://jakobdesign.com/)).

I already have a DIY console but no games for it yet. While searching for something to test, I found Jakob's game.

Here are the modifications I made:

- Switched from SPI to I2C.
- Up/Down buttons are used for navigation; A button for opening the menu and selecting items, B button for going back.
- Menu navigation does not wrap around — reaching the end stops at the last item.
- Removed the ability to pause during gameplay.
- Moved graphics to a separate header file.
