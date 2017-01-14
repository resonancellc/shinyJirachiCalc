# shinyJirachiCalc
This will make RNGIng for a Shiny Wishmaker Jirachi much easier.

## How this works
When obtaining a Shiny Jirachi from the Colosseum Bonus Disc, the Gamecube/Wii will check the GBA's save for the checksum of a specific part of the save, and use that as the seed for the Jirachi.

A GBA Save File is broken into the current save and backup save each containing various pieces of data, as well as checksums for each block.

The Jirachi seed comes from the first Block 0 checksum that appears in the GBA save which has most of the data about the player.  Anything from money, tid, sid, name, to things as little as the game's setting for what the "L" button does.

Normally, RNGing a Shiny Wishmaker Jirachi can be difficult, since every last detail has to be correct, however the calculator script used in this repo calculates the checksum for every single minute, second, and 1/60th of a second, up until the first hour (there are normally many seeds that can be found in that time frame).

Any amount of play time that will result in a Shiny Jirachi Seed when saved at will be outputed by the script for you to enter into the lua script, which will save for you at the precise moment with every variable you set while playing to give you a Shiny Jirachi Seed.

## Dependencies
* Tools to compile a C Program (such as gcc)
* npm
* Node.js
* VBA-RR
* A Pre-Emerald Hoenn GBA Game

## Installation
1. Clone the repository
2. Open a terminal in the repository's directory
3. Run ```npm install``` to download the dependency
4. Compile "PSavFix.c" (For example, on linux: ```gcc PSavFix.c -o PSavFix```)

## Usage
1. Start up VBA-RR with your game
2. Find your save delay time using the "Finding your save delay" section in this readme
3. Play until you get your PokeDex and Running Shoes
4. Save then pause the game
5. Export the save file
6. In your terminal, run ```node shinyJirachiCalc.js [your RS Save File] [second delay] [1/60 second delay]```
  1. Examples: ```node shinyJirachiCalc.js ruby.sav 1 46```
7. This will present you with a list of times you can save at to obtain a Shiny Jirachi
  1. You will see a list of seeds, times, and times with the save delay taken into account
  2. If you don't, restart the emulator, and start again from step 3.
8. Edit the "rsJirachi.lua" script where it says "EDIT THESE VALUES" to be equal to the target hour, minute, second, and 1/60th of a second that the script told you to "Set VBA Time to"
  1. Read the "Setting your save delay" section in this readme to see how to do this
9. Back in VBA-RR, make sure you have the save window open so that pressing "A" will save the game
10. Click "Tools"->"Lua Scripting"->"New Lua Script Window", then select the "Browse" button, and open "rsJirachi.lua" and run it
11. After the script saves the game at the correct time, pause and export the save
12. Run ```PSavFix -nofix -MAX [your RS Save File]``` which will display all of the checksums of the GBA Block's save
13. Find the first "Segment 00" entry
  1. Where it says "Calc" it will have the block's checksum - Make sure it matches the seed you want to use for your Jirachi
  2. If it does, you can use your save file to obtain your Shiny Wishmaker Jirachi with official hardware by restoring the save to a GBA Cart, and using it with a Gamecube/Wii, or via emulation with modified VBA and Dolphin emulators
  3. If it does not, (and you're sure the save delay you set is correct) start from Step 6. and try again

## Finding your save delay
Calibrating your save delay can be done by setting the lua script to save at a specific time, such as 0:20:00:00, then letting it run until the time stops for a few seconds.  Pause it there, and find how many seconds and 1/60th seconds delay you have by subtracting 0:20:00:00 from the time you landed on.

For example, if I saved at 0:20:00:00 and the time freezes at 0:20:01:46 while it waves, then I have a 1 second and 46/60th second delay.

## Setting your save delay
If the node script outputs
```
[***] Shiny Jirachi Found!
Seed: 0xcf37
At time: 0:18:43:56
Set VBA time to: 0:18:42:10
```

then edit lua to read

```
local targetSaveHour=0
local targetSaveMinute=18
local targetSaveSecond=42
local targetsixtiethSecond=10
local targetJirachiSeed=0xCF37
```

## Notes
A GBA save is broken into two saves: the current save, and the backup save - both switch around, meaning the backup save may come first OR second.  If the current save comes second in the file, then next save will be first.

However the Jirachi redemption uses the checksum of whichever save comes first, so make sure that before using the script you save enough times so that the next save will come first.  PSavFix is invaluable for helping with this task.

Having written the node script and lua edits when I was sick, the code could definitely be improved both for efficiency and accuracy, however it gets the job done pretty fast already, so I'm good with where it is, even if it's not always 100% accurate.

## Credits
* Thanks to the info and tested save files from everyone on [this post from Project Pokemon](https://projectpokemon.org/forums/forums/topic/12650-rs-shiny-jirachi-from-wishmkr/)
* loadingNOW for their PSavFix source (PSavFix.c) which helped me find which block's checksum is used for the Jirachi seed using the Project Pokemon link
* Kaphotics for [the lua script base](https://projectpokemon.org/forums/forums/topic/15187-gen-3-lua-scripts/)
* /u/Upper90175 (Jay from /r/PokemonRNG) [for the Shiny Jirachi Spreads in an easy text file](https://www.irccloud.com/pastebin/rdxEbTm4/)
* Peterolson - Javascript BigInt Library
