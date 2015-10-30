var fs = require("fs");
var exec = require('child_process').exec;
var run = function (command, callback){
    exec(command, function(error, stdout, stderr){
        if ( callback !== undefined) {
            callback(stdout, stderr);
        }
    });
};

var zeroPadding = function(num) {
  var pading = "000000000000" + num;
  return pading.slice(-6);
}

var nn = 0;
var doRename = function(items, n) {
  if ( n >= items.length ) {
    return;
  }

  var ftype = items[n].slice(-4);
  if ( ftype !== '.jgs' ) {
    console.log(">>>>>>>:" + items[n] + " " + ftype);
    doRename(items, n+1);
    return;
  }

  var cmd = 'cp "' +  pathFrom + '/' + items[n] + '" ';
  cmd = cmd + '"' + pathTo + '/' + zeroPadding(nn+start) + '.jgs"';

  run(cmd, function(){
    nn++;
    doRename(items, n+1);
  });
};



if (process.argv.length <= 4) {
  console.log("Usage: " + __filename + " path/from  path/to start");
  process.exit(-1);
}

var pathFrom = process.argv[2];
var pathTo = process.argv[3]
var start = parseInt(process.argv[4]);

fs.readdir(pathFrom, function(err, items) {
  doRename(items, 0);
});




