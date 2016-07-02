'use strict';

function buildClickFunction(row, col) {
  return function() {
    console.log(">>>>:" + row + " " + col);
  };
}

$(document).ready(function () {
  // 初始化棋盘组件
  for (var i = 0; i <= 16; i++) {
    for (var j = 0; j <= 16; j++) {
      var name = "P_" + i + "_" + j
      var item = '<li id="' + name + '" ></li>'
      $("#L" + i ).append(item);
      $("#" + name).click( buildClickFunction(i,j));
    }
  }

  // 设置棋子
  var item = $("#P_10_10")
  $('<img>').attr('src', 'images/b0.png').attr("class", "chess").appendTo(item);

  var item = $("#P_4_9")
  $('<img>').attr('src', 'images/r9.png').attr("class", "chess").appendTo(item);
  
});
