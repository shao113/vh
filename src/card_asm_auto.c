#include "include_asm.h"

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_ReadRegularSaveIntoBuf);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_WriteRegularSaveFromBuf);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_WriteFileListingFromBuf);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_ReadFileListingIntoBuf);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_WriteRegularSave);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_LoadRegularSave);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", ReloadBattle);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_LoadInBattleSave);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_WriteInBattleSave);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_WriteFileListing);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_ReadFileListing);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_EmbedIntIntoCaption);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_EmbedIntIntoSjisCaption);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_UpdateCaption);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_WriteRegularSaveFrom);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_ReadRegularSaveInto);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_WriteFileListingFrom);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_ReadFileListingInto);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_WriteInBattleSaveFrom);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_ReadInBattleSaveInto);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_Init);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_CheckState);

INCLUDE_ASM("asm/nonmatchings/card_asm_auto", Card_Format);

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
