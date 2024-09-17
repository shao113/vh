#include "common.h"
#include "audio.h"

s16 gSpellSounds[72] = {
    AUDIO_CMD_PLAY_XA(52),  AUDIO_CMD_PLAY_XA(51),  AUDIO_CMD_PLAY_XA(52),  AUDIO_CMD_PLAY_XA(53),
    AUDIO_CMD_PLAY_XA(51),  AUDIO_CMD_PLAY_XA(55),  AUDIO_CMD_PLAY_XA(56),  AUDIO_CMD_PLAY_XA(57),
    AUDIO_CMD_PLAY_XA(58),  AUDIO_CMD_PLAY_XA(60),  AUDIO_CMD_PLAY_XA(67),  AUDIO_CMD_PLAY_XA(184),
    AUDIO_CMD_PLAY_XA(60),  AUDIO_CMD_PLAY_XA(62),  AUDIO_CMD_PLAY_XA(64),  AUDIO_CMD_PLAY_XA(65),
    AUDIO_CMD_PLAY_XA(141), AUDIO_CMD_PLAY_XA(143), AUDIO_CMD_PLAY_XA(149), AUDIO_CMD_PLAY_XA(141),
    AUDIO_CMD_PLAY_XA(143), AUDIO_CMD_PLAY_XA(144), AUDIO_CMD_PLAY_XA(141), AUDIO_CMD_PLAY_XA(169),
    AUDIO_CMD_PLAY_XA(145), AUDIO_CMD_PLAY_XA(141), AUDIO_CMD_PLAY_XA(147), AUDIO_CMD_PLAY_XA(149),
    AUDIO_CMD_PLAY_XA(151), AUDIO_CMD_PLAY_XA(72),  AUDIO_CMD_PLAY_XA(153), AUDIO_CMD_PLAY_XA(151),
    AUDIO_CMD_PLAY_XA(143), AUDIO_CMD_PLAY_XA(141), AUDIO_CMD_PLAY_XA(141), AUDIO_CMD_PLAY_XA(141),
    AUDIO_CMD_PLAY_XA(141), AUDIO_CMD_PLAY_XA(151), AUDIO_CMD_PLAY_XA(58),  AUDIO_CMD_PLAY_XA(154),
    AUDIO_CMD_PLAY_XA(156), AUDIO_CMD_PLAY_XA(158), AUDIO_CMD_PLAY_XA(160), AUDIO_CMD_PLAY_XA(154),
    AUDIO_CMD_PLAY_XA(162), AUDIO_CMD_PLAY_XA(69),  AUDIO_CMD_PLAY_XA(164), AUDIO_CMD_PLAY_XA(166),
    AUDIO_CMD_PLAY_XA(167), AUDIO_CMD_PLAY_XA(141), AUDIO_CMD_PLAY_XA(141), AUDIO_CMD_PLAY_XA(149),
    AUDIO_CMD_PLAY_XA(169), AUDIO_CMD_PLAY_XA(169), AUDIO_CMD_PLAY_XA(170), AUDIO_CMD_PLAY_XA(141),
    AUDIO_CMD_PLAY_XA(172), AUDIO_CMD_PLAY_XA(174), AUDIO_CMD_PLAY_XA(69),  AUDIO_CMD_PLAY_XA(178),
    AUDIO_CMD_PLAY_XA(180), AUDIO_CMD_PLAY_XA(181), AUDIO_CMD_PLAY_XA(182), AUDIO_CMD_PLAY_XA(57),
    AUDIO_CMD_PLAY_XA(60),  AUDIO_CMD_PLAY_XA(141), AUDIO_CMD_PLAY_XA(72),  AUDIO_CMD_PLAY_XA(154),
    AUDIO_CMD_PLAY_XA(151), AUDIO_CMD_PLAY_XA(158), AUDIO_CMD_PLAY_XA(162), AUDIO_CMD_PLAY_XA(160),
};

s16 gSpellSounds2[72] = {
    0,
    0,
    0,
    AUDIO_CMD_PLAY_XA(179),
    0,
    0,
    0,
    0,
    AUDIO_CMD_PLAY_XA(59),
    0,
    AUDIO_CMD_PLAY_XA(68),
    AUDIO_CMD_PLAY_XA(185),
    AUDIO_CMD_PLAY_XA(61),
    AUDIO_CMD_PLAY_XA(63),
    0,
    AUDIO_CMD_PLAY_XA(66),
    0,
    0,
    0,
    AUDIO_CMD_PLAY_XA(142),
    0,
    0,
    AUDIO_CMD_PLAY_XA(142),
    0,
    AUDIO_CMD_PLAY_XA(146),
    AUDIO_CMD_PLAY_XA(142),
    AUDIO_CMD_PLAY_XA(148),
    AUDIO_CMD_PLAY_XA(149),
    AUDIO_CMD_PLAY_XA(152),
    0,
    AUDIO_CMD_PLAY_XA(59),
    AUDIO_CMD_PLAY_XA(152),
    0,
    0,
    0,
    AUDIO_CMD_PLAY_XA(142),
    AUDIO_CMD_PLAY_XA(142),
    AUDIO_CMD_PLAY_XA(152),
    AUDIO_CMD_PLAY_XA(59),
    0,
    AUDIO_CMD_PLAY_XA(157),
    AUDIO_CMD_PLAY_XA(159),
    AUDIO_CMD_PLAY_XA(161),
    AUDIO_CMD_PLAY_XA(155),
    AUDIO_CMD_PLAY_XA(163),
    AUDIO_CMD_PLAY_XA(70),
    AUDIO_CMD_PLAY_XA(179),
    AUDIO_CMD_PLAY_XA(165),
    AUDIO_CMD_PLAY_XA(168),
    0,
    0,
    0,
    0,
    0,
    0,
    AUDIO_CMD_PLAY_XA(142),
    AUDIO_CMD_PLAY_XA(173),
    AUDIO_CMD_PLAY_XA(175),
    AUDIO_CMD_PLAY_XA(70),
    AUDIO_CMD_PLAY_XA(179),
    0,
    0,
    0,
    0,
    AUDIO_CMD_PLAY_XA(61),
    AUDIO_CMD_PLAY_XA(142),
    0,
    AUDIO_CMD_PLAY_XA(155),
    AUDIO_CMD_PLAY_XA(152),
    AUDIO_CMD_PLAY_XA(159),
    AUDIO_CMD_PLAY_XA(163),
    AUDIO_CMD_PLAY_XA(161),
};