local natureorder={"Atk","Def","Spd","SpAtk","SpDef"}
local naturename={
"Hardy","Lonely","Brave","Adamant","Naughty",
"Bold","Docile","Relaxed","Impish","Lax",
"Timid","Hasty","Serious","Jolly","Naive",
"Modest","Mild","Quiet","Bashful","Rash",
"Calm","Gentle","Sassy","Careful","Quirky"}
local typeorder={
"Fighting","Flying","Poison","Ground",
"Rock","Bug","Ghost","Steel",
"Fire","Water","Grass","Electric",
"Psychic","Ice","Dragon","Dark"}

--EDIT THESE VALUES
local targetSaveHour=0
local targetSaveMinute=18
local targetSaveSecond=42
local targetsixtiethSecond=10
local targetJirachiSeed=0xCF37
--DON'T EDIT ANYTHING BEYOND HERE
local time=0x2F64EB3
local startMinute=0
local startSecond=0
local startsixtiethSecond=0
local startHour=0
local start=0x030045C0
local joyPress={}
joyPress["A"]=1

while true do
    tid=memory.readwordunsigned(0x2024EAE)
    sid=memory.readwordunsigned(0x2024EB0)
    minute=memory.readbyte(time+0x01)
    second=memory.readbyte(time+0x02)
    sixtiethSecond=memory.readbyte(time+0x03)
    hour=memory.readbyte(time)
    money=memory.readwordunsigned(0x2F25BC4)
    if(hour==targetSaveHour and minute==targetSaveMinute and second==targetSaveSecond and sixtiethSecond==targetsixtiethSecond) then
        joypad.set(1, joyPress)
    end
    seed=memory.readdwordunsigned(0x03004818)
    gui.text(4,30,"Target Save Time:  "..targetSaveHour..":"..targetSaveMinute..":"..targetSaveSecond..":"..targetsixtiethSecond)
    gui.text(4,40,"Time:  "..hour..":"..minute..":"..second..":"..sixtiethSecond)
    gui.text(4,50,"Money: "..money)
    gui.text(4,60,"TID: "..tid)
    gui.text(4,70,"SID: "..sid)
    gui.text(4,90,"Target Jirachi Seed: 0x"..string.format("%X", targetJirachiSeed))
    gui.text(4,100,"Frame: "..vba.framecount()) 
    gui.text(4,110,"Seed: "..string.format("%8X", seed))
    print("Seed: "..string.format("%8X", seed))
    emu.frameadvance()
end