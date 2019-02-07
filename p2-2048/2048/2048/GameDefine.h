//
//  GameDefine.h
//  2048
//
//  Created by WEN YANG on 2/6/19.
//  Copyright © 2019 WEN YANG. All rights reserved.
//

#ifndef GameDefine_h
#define GameDefine_h

//color
#define GAMEBACKCOLOR makeColorRgb(62, 78, 91)
#define SCORECOLOR    makeColorRgb(39, 55, 67)
#define TILEDCOLOR    makeColorRgb(128, 141, 146)

#define TILEDColor    makeColorRgb(90, 111, 127)

//audio, move, hit
#define MOVEAUDIO         [AudioTool setAudioMove:@"move" setFormatMove:@"wav"]
#define HITAUDIO          [AudioTool setAudioHit:@"hit" setFormatHit:@"wav"]
#define STOVAUDIIO        [AudioTool setAudioStOv:@"dis" setFormatStOv:@"wav"]


//size of number square
#define GAME_TILED_WIDTH        60 *SCREEN_Width/320
#define GAME_TILED_HEIGTH       GAME_TILED_WIDTH

//width of board
#define GAME_TILED_BOARD_WIDTH  10

//gap of field
#define GAME_ROWS               4
#define GAME_COLS               4

//bottom size
#define BTN_SIZE                60 *SCREEN_Width/320
#define LAB_WIDTH               100 *SCREEN_Width/320


#endif /* GameDefine_h */
