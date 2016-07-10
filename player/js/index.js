'use strict';

var g = {};
var ui = {};

ui.onBtnReset = function() {
  g.player.doReset();
  g.player.showGame();
};

ui.onBtnForward = function() {
  g.player.doForward();
  g.player.showGame();
};

ui.onBtnBackward = function() {
  g.player.doBackward();
  g.player.showGame();
};

$(document).ready(function () {
  g.player = new ChessPlayer();
  g.player.loadGame(game1000);
  g.player.showGame();

  // setup action handlers
  $("#btnReset").click(ui.onBtnReset);
  $("#btnForward").click(ui.onBtnForward);
  $("#btnBackward").click(ui.onBtnBackward);
});
