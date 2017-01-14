var fs = require('fs');
var util = require('util');
var bigInt = require('./bigint.js');

var counter = 0,
    currChk,
    startChk = 0,
    shinyJirachiArray=[0x353d,0xf500,0xecdd,0x9359,0xcf37,0x7236,0xa030,0x7360,0x3d60];
if(process.argv[2]!=null &&
   process.argv[3]!=null &&
   process.argv[4]!=null){
    fs.readFile(process.argv[2], (err, data) => {
        if (err) throw err;
        calcJir(data);
    });
} else {
    console.log("Usage: node shinyJirachiCalc.js [your RS Save File] [second delay] [1/60 second delay]")
}

function calcJir(data){
    var msg = "",
        multiStart = 0,
        shinyJirBlock = [];
    multiStart = data[0xFFC]>data[0x10F7C] ? 0 : 14;
    for(var i = multiStart; i<multiStart+14; i++){
        if(data[(i*0x1000)+0xFF4]==0x00){
            multiStart = i;
            break;
        }
    }
    console.log("Finding Shiny Jirachi Times");
    for(var j = 0; j<0xA4; j++){
        shinyJirBlock.push(data[(multiStart*0x1000)+j]);
    }
    //hours
    var h = 0;
    while(h<1){
        //minutes
        for(var i = 13; i<60; i++){
            //seconds
            for(var j = 0; j<60; j++){
                //1/60 seconds
                for(var z = 0; z<60; z++){
                    counter++;
                    currChk = chkWithIDs(shinyJirBlock, h, i, j, z);
                }
            }
        }
        h++;
    }
}

function chkWithIDs(data, h, i, j, z){
    var startChk = [
        (h).toString(16).paddingLeft("00"),
        (i).toString(16).paddingLeft("00"),
        (j).toString(16).paddingLeft("00"),
        (z).toString(16).paddingLeft("00")];
    var currChk = Chksum(data, startChk); 
    if(shinyJirachiArray.indexOf(currChk)!=-1){
        var msg = "[***] Shiny Jirachi Found!\n";
        msg+="Seed: 0x"+(currChk).toString(16)+"\n";
        msg+="At time: "+h+":"+i+":"+j+":"+z+"\n";
        if(z-parseInt(process.argv[4])>0){
            z-=parseInt(process.argv[4]);
            j-=parseInt(process.argv[3]);
        } else {
            z+=60-parseInt(process.argv[4]);
            j-=parseInt(process.argv[3])-1;
        }
        msg+="Set VBA time to: "+h+":"+i+":"+j+":"+z+"\n";
        console.log(msg);
    }
    return currChk;
}

function Chksum(vanilla0Block, data){
    var Chk = 0, tmpj, msg = "";
    vanilla0Block[15] = data[0];
    vanilla0Block[16] = data[1];
    vanilla0Block[17] = data[2];
    vanilla0Block[18] = data[3];
    
    for(var i = 0; i<(vanilla0Block.length); i+=4){
        Chk=bigInt(Chk+parseInt("0x"+vanilla0Block[i+3].toString(16).paddingLeft("00")
            +vanilla0Block[i+2].toString(16).paddingLeft("00")
            +vanilla0Block[i+1].toString(16).paddingLeft("00")
            +vanilla0Block[i+0].toString(16).paddingLeft("00"), 'hex'));
    }
    tmp = Chk>>16;
    tmp+=Chk;
    
    Chk = (tmp&0xFFFF);
    return Chk;
}

String.prototype.paddingLeft = function (paddingValue) {
    return String(paddingValue + this).slice(-paddingValue.length);
};