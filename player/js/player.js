"use strict";

var ChessPlayer = function() {
  // 初始化棋盘组件
  $("#board").empty();
  for (var i = 0; i <= 16; i++) {
    $("#board").append("<ul id='L" + (16-i) + "'></ul>");
  }

  for (var i = 0; i <= 16; i++) {
    for (var j = 0; j <= 16; j++) {
      var name = "P_" + i + "_" + j
      var item = '<li id="' + name + '" ></li>'
      $("#L" + i ).append(item);
    }
  }

  //$('<img>').attr('src', 'images/b0.png').attr("class", "chess").appendTo(item);

  this.emptyBoard();
  this.show = "ALL";
  this.game = null;
};

ChessPlayer.prototype = {

  emptyBoard: function() {
    this.board = [];
    for (var i = 0; i <= 16; i++) {
      this.board.push([])
      for (var j = 0; j <= 16; j++) {
        var pos = {};
        pos.y = i;
        pos.x = j;
        pos.piece = null;

        if (i<=5 && j<=5) {
          pos.piece = undefined;
        } else if ( j >=11 && i<= 5) {
          pos.piece = undefined;
        } else if ( j >=11 && j>= 11) {
          pos.piece = undefined;
        } else if ( j <=5 &&  j>= 11) {
          pos.piece = undefined;
        }
        this.board[i].push(pos);
      }
    }
  },

  loadGame: function(newGame) {
    this.game = newGame;
    // 摆放棋盘
    for (var i = 0; i <= 16; i++) {
      for (var j = 0; j <= 16; j++) {
        if (i<=5 && j<=5) {
          continue;
        } else if ( j >=11 && i<= 5) {
          continue;
        } else if ( j >=11 && j>= 11) {
          continue;
        } else if ( j <=5 &&  j>= 11) {
          continue;
        }
      }
    }

    
  },






};
