var fs = require('fs');


var main = function(fileName) {
  var jqsBuffer = new Buffer(fs.readFileSync(fileName, 'binary'), 'binary');
  
  // 51 51 47 61 6d 65 20 4a 51 53  = "QQGame JQS"
  if ( jqsBuffer.toString('ascii', 0, 10) !== "QQGame JQS" ) {
    console.log("This is not QQGame JQS file!");
    return;
  }

  var jqsInfo = {};

  // self
  if ( jqsBuffer[15] === 0 ) {
    jqsInfo.self = 'east';
  } else if ( jqsBuffer[15] === 1 ) {
    jqsInfo.self = 'south';
  } else if ( jqsBuffer[15] === 2 ) {
    jqsInfo.self = 'west';
  } else if ( jqsBuffer[15] === 3 ) {
    jqsInfo.self = 'north';
  } else {
    throw Error('Parse Error');
  }

  // totalMove
  jqsInfo.totalMove = jqsBuffer[27] * 256 + jqsBuffer[26];

  // user info
  jqsInfo.east = [];
  for(var i = 60; i < 30+60; i++) {
    jqsInfo.east.push( jqsBuffer[i] );
  }
  jqsInfo.south = [];
  for(var i = 148; i < 30+148; i++) {
    jqsInfo.south.push( jqsBuffer[i] );
  }
  jqsInfo.west = [];
  for(var i = 236; i < 30+236; i++) {
    jqsInfo.west.push( jqsBuffer[i] );
  }
  jqsInfo.north = [];
  for(var i = 324; i < 30+324; i++) {
    jqsInfo.north.push( jqsBuffer[i] );
  }

  

};

if ( process.argv.length !== 3) {
  console.log("Please input JQS file .");
} else {
  main(process.argv[2]);
}





