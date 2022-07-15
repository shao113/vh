#include "common.h"
#include "evt.h"
#include "audio.h"

void Evtf581_AudioCommand(EvtData *evt) {
   switch (evt->state) {
   case 0:
      if ((evt->d.evtf581.delay != 0) && (--evt->d.evtf581.delay != 0)) {
         return;
      }

      evt->state++;
      /* fallthrough */
   case 1:
      PerformAudioCommand(evt->d.evtf581.cmd);
      evt->functionIndex = EVTF_NULL;
   }
}
