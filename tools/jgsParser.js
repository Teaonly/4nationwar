var fs = require('fs');

var main = function(fileName) {
  var jqsBuffer = new Buffer(fs.readFileSync(fileName, 'binary'), 'binary');
  
  // 51 51 47 61 6d 65 20 4a 51 53  = "QQGame JQS"
  if ( jqsBuffer.toString('ascii', 0, 10) !== "QQGame JQS" ) {
    console.log("This is not QQGame JQS file!");
    return;
  }

  // 基本信息
  //
  // 
  // 位置信息： 
  // QQ四国军棋将整个棋盘表示成一个17×17的矩阵，每一个棋盘位置用两个字节分别表示相关横纵坐标。
  // 将棋盘旋转到上黄下绿左紫右蓝的形式，第一个坐标为纵坐标，从下往上为00, 01, 02 .. 10；
  // 第二个坐标为横坐标，从左往右为00, 01, 02 .. 10。 
  // 说明：整个棋盘有大量冗余的地方，如00 00位置不存在，由此可推知00 00 到 05 05 矩阵，
  // 0B 00到10 05矩阵，00 0B 到05 10 矩阵和 0B 0B到10 10矩阵都不存在，还有棋盘中间部分使用5×5的坐标来表示3×3的矩阵，
  // 显然有16个位置不存在，如 06 07,07 09等。
  //
  // 棋子表示：
  // 
  // 00:黄 01:蓝 02:绿 03:紫
  //
  // 00:  无 
  // 01:  反面 
  // 02:  军旗 
  // 03:  地雷 
  // 04:  炸弹 
  // 05:  司令 
  // 06:  军长 
  // 07:  师长 
  // 08:  旅长 
  // 09:  团长 
  // 0A:  营长 
  // 0B:  连长 
  // 0C:  排长 
  // 0D:  工兵

  var jqsInfo = {};

  // totalMove
  jqsInfo.myself = jqsBuffer[15];  // 标识己方位置 00黄 01蓝 02绿 03紫
  jqsInfo.totalMove = jqsBuffer[27] * 256 + jqsBuffer[26];

  // user info
  // 20 - 2F： 第一个字节（20）表玩家颜色 00黄 01蓝 02绿 03紫，之后填充7个0
  // 28 ~ 3B： 一个20字节以’\0’结尾的字符串表示玩家名字（注：游戏中玩家名字可多
  //           余19字节，但多余部分的玩家名字字符在保存时丢弃
  // 3C ~ 59： 一个30字节的数据块表示一个5×6的矩阵，存储了布局信息，其值分别
  //           对应棋盘上每一个棋子：第一个值对应左上角第一个棋子，第二个值对应第一行第
  //           二列的棋子，依此类推。（值与棋子对应关系见相关值表）
  // 59 ~ 73： 24字节填充0
  // 74 ~ 77： 最后4字节作用未知
  //

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

  // 棋步或事件信息解析
  //
  // 说明：第一个字节用以区分信息类型5F表棋步信息，F5表事件信息。
  //     棋步信息表示下一步走棋的具体形式，事件信息则会弹出相应的对话框。
  //
  // 走棋说明：
  //     第二个字节，第5和2比特意义不明
  //     第7,6位值为0x01表有司令阵亡
  //     第4,3位对应值为颜色值表取反结果，即 11黄 10 紫 01绿 00蓝
  //     第1,0位表示信息如下表：
  //        00：移动：棋子只是单纯从初始位置移动到目的位置
  //        01: 吃：初始移动棋子覆盖掉目的位置的敌对玩家的棋子
  //        10: 反弹：初始移动棋子撞上目的位置的敌对玩家棋子后消失
  //        11: 兑：初始移动棋子与目的位置的敌对玩家棋子同时消失
  //
  //     第三、四字节：棋子移动初始位置
  //     第五、六字节：棋子移动目的位置
  //
  //     若有至少其中一方有司令阵亡，使用后四个字节：
  //     第七、八字节：司令阵亡翻军棋棋位置2（值为00表不翻军棋）
  //     第九、十字节：司令阵亡翻军棋棋位置1（值为00表不翻军棋）
  //
  // 事件信息：
  //     第二个字节：
  //        02: 超时
  //            第三个字节表示超时玩家颜色（见颜色值表）；第四个字节表已经超时次数；
  //            第五个字节表示下一个走棋玩家的颜色（见颜色值表）；之后填充5个0
  //        03: 玩家战败
  //            第三个字节表示战败玩家颜色（见颜色值表）；第四个字节表示战败方式 
  //            01被夺旗 02无子可走 03超时5次 04投降；
  //            第五个字节表示下一个走棋玩家的颜色（见颜色值表），
  //            特别地FF表示无下一个走棋的玩家；之后填充5个0
  //        04: 玩家退出
  //            第三个字节表示退出游戏玩家颜色（见颜色值表）；
  //            第四个字节为00第五个字节表示下一个走棋玩家的颜色（见颜色值表）；之后填充5个0
  //        05: 游戏结束
  //            第三个字节固定填00，第四个字节表示战局结果，02和局 04其中一方战败 06其中有玩家逃跑；
  //            第五个字节为FF表无下一个走棋的玩家，之后填充5个0
  //
  //

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

