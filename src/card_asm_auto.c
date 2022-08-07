#include "include_asm.h"

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_CountFreeBlocks);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_CreateFile);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_FileExists);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_WriteFile);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_ReadFile);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_ReadHeadOfArbitraryFile);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", CalculateChecksum);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_WaitForSwCardEvent);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_ClearSwCardEvents);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_WaitForHwCardEvent);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_ClearHwCardEvents);
