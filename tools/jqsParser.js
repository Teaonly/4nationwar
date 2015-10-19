var fs = require('fs');

var main = function(fileName) {
  var jqsBuffer = new Buffer(fs.readFileSync(fileName, 'binary'), 'binary');
  
  // 51 51 47 61 6d 65 20 4a 51 53  = "QQGame JQS"
  if ( jqsBuffer.toString('ascii', 0, 10) !== "QQGame JQS" ) {
    console.log("This is not QQGame JQS file!");
    return;
  }

  var jqsInfo = {};

  // totalMove
  jqsInfo.totalMove = jqsBuffer[27] * 256 + jqsBuffer[26];

  // user info
  jqsInfo.east = [];
  for(var i = 60; i < 30+60; i++) {
    jqsInfo.east.push( jqsBuffer[i] );
  }
  jqsInfo.north = [];
  for(var i = 148; i < 30+148; i++) {
    jqsInfo.north.push( jqsBuffer[i] );
  }
  jqsInfo.west = [];
  for(var i = 236; i < 30+236; i++) {
    jqsInfo.west.push( jqsBuffer[i] );
  }
  jqsInfo.south = [];
  for(var i = 324; i < 30+324; i++) {
    jqsInfo.south.push( jqsBuffer[i] );
  }

  jqsInfo.record = [];
  for (var i = 0; i < jqsInfo.totalMove; i++) {
    var seq = 412 + i*10;
    var move = {};

    if ( jqsBuffer[seq] === 95 ) {
      move.xfrom = 16 - jqsBuffer[seq+3];
      move.yfrom = 16 - jqsBuffer[seq+2];
      move.xto = 16 - jqsBuffer[seq+5];
      move.yto = 16 - jqsBuffer[seq+4];

      switch ( jqsBuffer[seq+1] ) {
        case 0:
        case 8:
        case 16:
        case 24:
          move.action = 'moved';
          break;
        case 1:
        case 9:
        case 17:
        case 25:
          move.action = 'kill';
          break;
        case 2:
        case 10:
        case 18:
        case 26:
          move.action = 'killed';
          break;
        case 3:
        case 11:
        case 19:
        case 27:
        case 67:
        case 75:
        case 83:
        case 91:
          move.action = 'fired';
          break;

       default:
          throw Error('Parse Error');
          break;
      }

    } else if ( jqsBuffer[seq] === 245 ){
      switch( jqsBuffer[seq+1] ) {
        case 5:{
          move.action = 'end';
          break;
        }
        case 4: {
          move.action = 'exit';
          break;
        }
        case 3: {
          move.action = 'lose';
          break;
        }
        case 2: {
          move.action = 'skip';
          break;
        }
        default:{
          throw Error('Parse Error');
        }
      }
    } else {
      throw Error('Parse Error');
    }
    jqsInfo.record.push( move );
  }

  if ( process.argv.length === 4) {
    fs.writeFileSync(process.argv[3], JSON.stringify(jqsInfo));
  } else {
    console.log ( jqsInfo );
  }
  
};

if ( process.argv.length < 3) {
  console.log("Please input JQS file .");
} else {
  main(process.argv[2]);
}



