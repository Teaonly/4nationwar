'use strict';

var g = {};

$(document).ready(function () {
  g.player = new ChessPlayer();
  g.player.loadGame(game1000);
  g.player.showGame();
});
